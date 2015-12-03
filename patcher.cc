#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <cstring>
#include <ctime>
#include "patcher.h"

// files to patch
const std::array<std::string, 4>  Patcher::filesToPatch = {{ "picc", "picc18", "cgpic", "cgpic18" }};

/**
 * @brief Patcher::Patcher
 * @param digest
 */
Patcher::Patcher(const std::string &digestToSearch, const std::string &digest)
       : mDigestToSearch(digestToSearch),
         mDigest(digest)
{
}

/**
 * @brief Patcher::findOffset
 * @param file
 * @return
 */
std::streampos Patcher::findOffset(std::ifstream &file, const std::streamsize &length)
{
    file.seekg(0, std::ifstream::beg);

    std::array<char, MAX_READ_SIZE>  buf;
    for (std::streamsize i = length; i > 0;)
    {
        auto readSize = i > MAX_READ_SIZE ? MAX_READ_SIZE : i;

        buf.fill(0x00);
        file.read(buf.data(), readSize);

        auto it = std::search(buf.begin(), buf.end(), mDigestToSearch.begin(), mDigestToSearch.end());
        if (it != buf.end()) // found
        {
            auto offset = std::distance(buf.begin(), it);
            return (length - i) + offset;
        }

        i -= readSize;
        if (i > mDigest.size())
        {
            // jump hash size back for a full search
            i += mDigest.size();
            file.seekg(-mDigest.size(), std::ifstream::cur);
        }
    }

    return -1;
}

/**
 * @brief Patcher::makeBackup
 * @param file
 * @return
 */
bool Patcher::makeBackup(std::ifstream &file, const std::string &fileName, const std::streamsize &lenght)
{
    try
    {
//        auto timeNow = std::chrono::system_clock::now();
//        auto timeNowTimeT = std::chrono::system_clock::to_time_t(timeNow);
        std::stringstream  ss;
        ss << fileName << ".bak";// << std::put_time(std::localtime(&timeNowTimeT), "%Y%m%d");

        std::cout << "Create backup => " << ss.str();

        std::ofstream  oFile(ss.str(), std::ifstream::binary | std::ifstream::trunc);
        if (oFile.bad())
        {
            std::cerr << "Unable to create file '" << ss.str() << "'";
            return false;
        }

        file.seekg(0, std::ifstream::beg);

        std::array<char, MAX_READ_SIZE>  buf;
        for (std::streamsize i = lenght; i > 0; )
        {
            auto readSize = i > MAX_READ_SIZE ? MAX_READ_SIZE : i;
            file.read(buf.data(), readSize);
            oFile.write(buf.data(), readSize);
            i -= readSize;
        }
        oFile.close();

        std::cout << " done; ";

        return true;
    }
    catch (std::exception &ex)
    {
        std::cerr << ex.what();
    }
    return false;
}

/**
 * @brief Patcher::patchFile
 * @param fileName
 * @param offset
 * @return
 */
bool Patcher::patchFile(const std::string &fileName, const std::streamsize &offset)
{
    try
    {
        std::ofstream  oFile(fileName, std::ofstream::out | std::ofstream::in | std::ofstream::binary);
        if (oFile.bad())
        {
            std::cerr << "Unable to open file " << fileName << " for patching. ";
            return false;
        }

        oFile.seekp(offset, std::ofstream::beg);
        oFile.write(mDigest.c_str(), mDigest.size());
        oFile.close();

        return true;
    }
    catch (std::exception &ex)
    {
        std::cerr << ex.what();
    }
    return false;
}

/**
 * @brief Patcher::patchCompiler
 * @return
 */
bool Patcher::patchCompiler()
{
    for (std::string fileName : filesToPatch)
    {
        std::cout << fileName << " [";
        std::ifstream  file("./" + fileName, std::ifstream::binary | std::ifstream::ate);
        if (file.bad())
        {
            std::cerr << "Can not find file '" << fileName << "'" << std::endl;
            continue;
        }
        auto length = file.tellg();
        auto offset = findOffset(file, length);
        if (offset < 0)
        {
            std::cout << " Nothing found to patch. Already patched?" << std::endl;
            file.close();
            continue;
        }
        std::cout << "Found offset: " << offset << "; ";
        if (!makeBackup(file, fileName, length))
        {
            std::cout << " Failed." << std::endl;
            file.close();
            continue;
        }
        file.close();

        if (!patchFile(fileName, offset))
        {
            std::cout << "Patch failed." << std::endl;
            continue;
        }

        std::cout << "Patched.]" << std::endl;
    }
}
