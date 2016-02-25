/*
 * The MIT License (MIT)
 *
 * Copyright (c)2015-2016, does not matter
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <cstring>
#include <ctime>
#include "patchrunner.h"

/**
 * @brief PatchRunner::PatchRunner
 * @param baseVersion
 * @param compilerBin
 */
PatchRunner::PatchRunner(const AbstractVersion &baseVersion, const boost::filesystem::path &xclmFilePath)
           : mDigestToSearch(baseVersion.xclmDigest()),
             compilerBinDir(xclmFilePath.parent_path()),
             xclmFile(xclmFilePath)
{
}

/**
 * @brief PatchRunner::run
 * @return
 */
std::error_code PatchRunner::run()
{
    auto ec = checkNeededFilesExists(compilerBinDir);
    if (ec)
        return ec;

    Hash::SHA0::digest_t  digest;
    ec = hashing(xclmFile, digest);
    if (ec)
        return ec;

    for (const auto& fileName : filesToPatch)
    {
        const auto fullFile(compilerBinDir / fileName);
        std::cout << fullFile.string() << " [";

        std::streampos offset;
        ec = findOffset(fullFile, offset, mDigestToSearch);
        if (ec)
        {
            std::cout << " Nothing found to patch. Already patched?" << std::endl;
            continue;
        }
        std::cout << "Found offset: " << offset << "; ";

        ec = makeBackup(fullFile);
        if (ec)
        {
            std::cout << " Failed." << std::endl;
            continue;
        }

        ec = patchFile(fullFile, offset, xclm::toHexString(digest));
        if (ec)
        {
            std::cout << "Patch failed." << std::endl;
            continue;
        }

        std::cout << "Patched.]" << std::endl;
    }
}

/**
 * @brief Patcher::makeBackup
 * @param file
 * @param fileName
 * @param fileLenght
 * @return
 */
std::error_code PatchRunner::makeBackup(const boost::filesystem::path &fullFile) const
{
    try
    {
        auto timeNow = std::chrono::system_clock::now();
        auto timeNowTimeT = std::chrono::system_clock::to_time_t(timeNow);
        std::stringstream  ss;
        ss << fullFile.filename().string() << "-" << std::put_time(std::localtime(&timeNowTimeT), "%Y%m%d") << ".bak";

        const boost::filesystem::path backupFile(compilerBinDir / ss.str());
        std::cout << "Create backup => " << backupFile.string();

        std::ifstream  iFile(fullFile.string(), std::ios::binary | std::ios::in);
        if (iFile.fail())
        {
            std::cerr << "Unable to open file '" << fullFile.string() << "'";
            return std::error_code(errno, std::generic_category());
        }

        std::ofstream  oFile(backupFile.string(), std::ios::binary | std::ios::trunc);
        if (oFile.fail())
        {
            std::cerr << "Unable to create file '" << backupFile.string() << "'";
            return std::error_code(errno, std::generic_category());
        }

        const auto &fileLength = streamSize(iFile);

        std::array<char, MAX_READ_SIZE>  buf;
        for (auto i = fileLength; i > 0; )
        {
            auto readSize = i > MAX_READ_SIZE ? MAX_READ_SIZE : i;
            iFile.read(buf.data(), readSize);
            oFile.write(buf.data(), readSize);
            i -= readSize;
        }
        iFile.close();
        oFile.close();

        std::cout << " done; ";

        return NOERROR;
    }
    catch (std::exception &ex)
    {
        std::cerr << ex.what();
    }

    return xclm::make_error_code(xclm::errc::backup_failed);
}

/**
 * @brief Patcher::patchFile
 * @param fileName
 * @param offset
 * @return
 */
std::error_code PatchRunner::patchFile(const boost::filesystem::path &fileName, const std::streamsize &offset, const std::string &strDigest) const
{
    try
    {
        std::ofstream  oFile(fileName.string(), std::ios::out | std::ios::in | std::ios::binary);
        if (oFile.fail())
        {
            std::cerr << "Unable to open file " << fileName.string() << " for patching. ";
            return std::error_code(errno, std::generic_category());
        }

        oFile.seekp(offset, std::ios::beg);
        oFile.write(strDigest.c_str(), strDigest.size());
        oFile.close();

        return NOERROR;
    }
    catch (std::exception &ex)
    {
        std::cerr << ex.what();
    }
    return xclm::make_error_code(xclm::errc::patch_failed);
}
