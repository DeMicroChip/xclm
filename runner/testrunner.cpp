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
#include "testrunner.h"
#include <array>                        // for array
#include <boost/filesystem/operations.hpp>  // for exists, is_directory
#include <iostream>                     // for operator<<, basic_ostream, etc
#include "abstractrunner.h"
#include "hashes/sha0.h"                // for SHA0, SHA0::digest_t
#include "util.h"                       // for toHexString
#include "version/abstractversion.h"    // for AbstractVersion
#include "xclm_error.h"                 // for error_code, make_error_code, etc


/**
 * @brief TryRun::run
 * @param binDir
 * @return
 */
std::error_code TestRunner::run()
{
    xclm::boost::system::error_code  bec;

    if (!boost::filesystem::exists(compilerBinDir, bec))
    {
        std::cerr << u8"[ERROR](" << compilerBinDir.string() << ") " << bec.message() << ". Code: " << bec.value() << std::endl;
        return bec;
    }

    if (!boost::filesystem::is_directory(compilerBinDir))
    {
        std::cerr << u8"[ERROR](" << compilerBinDir.string() << ") is not a directory." << std::endl;
        return xclm::make_error_code(xclm::errc::not_a_directory);
    }

    std::error_code ec = checkNeededFilesExists(compilerBinDir);
    if (ec)
        return ec;

    Hash::SHA0::digest_t digest;
    ec = hashing(compilerBinDir / xclmBin, digest);
    if (ec)
    {
        std::cerr << u8"[ERROR] Unable to calculate digest about " << xclmBin << ". " << ec.message() << ". Code: " << ec.value() << std::endl;
        return ec;
    }

    auto strDigest = xclm::toHexString(digest);
    std::cout << "Digest about '" << xclmBin << "' : " << strDigest << std::endl;

    bool failed = !searchDigest(compilerBinDir, strDigest);
    failed = !searchHiddenTable(compilerBinDir);

    return failed ? xclm::make_error_code(xclm::errc::try_run_failed) : NOERROR;
}

/**
 * @brief TryRun::checkNeededFilesExists
 * @param compilerBinDir
 * @return
 */
std::error_code TestRunner::checkNeededFilesExists(const boost::filesystem::path &compilerBinDir) const
{
    xclm::boost::system::error_code  ec;
    auto xclm(compilerBinDir / xclmBin);
    if (!boost::filesystem::exists(xclm, ec))
    {
        std::cerr << u8"[ERROR](" << xclm.string() << ") " << ec.message() << ". Code: " << ec.value() << std::endl;
        return ec;
    }
    return AbstractRunner::checkNeededFilesExists(compilerBinDir);
}

/**
 * @brief search for digest inside compiler binaries
 * @return
 */
bool TestRunner::searchDigest(const boost::filesystem::path &compilerBinDir, const std::string strDigest) const
{
    bool found(true);
    for (const auto& file : filesToPatch)
    {
        std::streampos  offset;
        auto fullPath(compilerBinDir / file);

        std::cout << "Search for digest in '" << fullPath.string() << "': ";

        auto ec = findOffset(fullPath, offset, strDigest);
        if (ec)
        {
            std::cout << "not found." << std::endl;
            found = true;
        }
        else
            std::cout << "found at " << offset << std::endl;
    }

    return found;
}

/**
 * @brief search for known hidden hash table inside xclm
 * @return
 */
bool TestRunner::searchHiddenTable(const boost::filesystem::path &compilerBinDir) const
{
    std::streampos  offset;
    auto fullPath = compilerBinDir / xclmBin;
    std::cout << "Search for hash table in '" << fullPath.string() << "': ";
    auto ec = findOffset(fullPath, offset, hashTable.hiddenHashTable());
    if (ec)
    {
        std::cout << "not found." << std::endl;
        return false;
    }

    std::cout << "found at " << offset << std::endl;
    return true;
}
