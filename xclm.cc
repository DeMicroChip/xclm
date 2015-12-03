/*****************************************************************************
 *
 * xclm.c - A replacement license server for Microchip embedded compilers
 *
 *****************************************************************************/
#include <algorithm>
#include <array>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string.h>
#include "xclm.h"
#include "logging.h"
#include "sha0.h"
#include "patcher.h"
#include "version.cc"

/**
 * @brief dump 10 digits of a lookup table
 * @param index
 */
static void printXc8Key(uint32_t index)
{
    if ((index + 10) >= XC8_KEY_V202.size())
    {
        Logger::dLog("Dump hash: Index is greater the key size. Index = %ud + 10", index);
        std::cout << "damn you got me.";
    }
    else
    {
        fprintf(stdout, "%.10s", &XC8_KEY_V202[index]);
    }
}

/**
 * @brief DIGEST0
 */
static const std::string DIGEST0 = "0164b8a914cd2a5e74c4f7ff082c4d97f1edf880";
/**
 * @brief simple SHA0 self test method
 */
static void hastTest()
{
    Hash::SHA0 hash;
    std::array<unsigned char, 3> buf = {{ 0x61, 0x62, 0x63 }}; // 'a', 'b', 'c'
    hash.update(buf);

    Hash::SHA0::digest_t digest;
    hash.digest(digest);

    std::string result = Logger::hexStr(digest.data(), digest.size());
    std::cout << "Calculated: " << result.c_str() << std::endl;
    std::cout << "Should be : " << DIGEST0.c_str() << std::endl;
    std::cout << "Valid     : " << std::boolalpha << (DIGEST0 == result.c_str()) << std::endl;
}

/**
 * @brief calc digest in MICROCHIP mode about given file
 * @param progName
 * @return
 */
static Hash::SHA0::digest_t  hashing(const std::string& progName)
{
    // set position to end of file to get length of file
    std::ifstream  readFile(progName, std::ifstream::binary | std::ifstream::ate);
    auto  fileLength = readFile.tellg();
    readFile.seekg(0, std::ifstream::beg);

    Hash::SHA0 md(Hash::SHA0_MODE::MICROCHIP);

    std::array<char, MAX_READ_SIZE>  buf;
    for (std::streamsize i = fileLength; i > 0;)
    {
        auto readSize = i > MAX_READ_SIZE ? MAX_READ_SIZE : i;
        readFile.read(buf.data(), readSize);
        md.update(buf.data(), readSize);
        i -= readSize;
    }

    Hash::SHA0::digest_t digest;
    md.digest(digest);

    return digest;
}

/**
 * @brief print help screen
 * @return
 */
static int printHelp()
{
    std::cerr << "[XCLM replacement and patcher (c)2015]" << std::endl;
    std::cerr << std::endl;
    std::cerr << "Copy this file into your XC compiler bin directory. Make a backup of the original xclm file before replacing with this version." << std::endl;
    std::cerr << std::endl;
    std::cerr << "--help             this screen" << std::endl;
    std::cerr << "--hashtest         test internal SHA0 implementation" << std::endl;
    std::cerr << "--patch            patch XC compiler" << std::endl;
    std::cerr << "--selfhashing      calculate digest about this file and print" << std::endl;
    std::cerr << "--hashing <file>   calculate digest about given file and print" << std::endl;

    return 255;
}

/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    LICENSE_MODEL  license = LICENSE_MODEL::LICENSE_FREE;

    Logger::initLogging();

    Logger::dLog("xclm called with commanline parameters(%d)\n", argc);
    for (uint32_t i = 0; i < argc; i++)
        Logger::dLog("%s ", argv[i]);
    Logger::dLog("\n");


    for (uint32_t i = 0; i < argc; i++)
    {
        if (!strcmp(argv[i], "swxc32") || !strcmp(argv[i], "swxc16") || !strcmp(argv[i], "swxc8"))
            license = LICENSE_MODEL::LICENSE_PRO;
        else if (!strcmp(argv[i], "swxcpp32"))
            license = LICENSE_MODEL::LICENSE_CPPFULL;
        else if (!strcmp(argv[i], "-hash"))
            printXc8Key(atoi(argv[i + 1]));
        else if (!strcmp(argv[i], "--hashtest"))
            hastTest();
        else if (!strcmp(argv[i], "--patch"))
        {
            const std::string digest = Logger::hexStr(hashing(std::string(argv[0])));
            Patcher pc(XCLM_HASH_V202, digest);
            pc.patchCompiler();
        }
        else if (!strcmp(argv[i], "--selfhashing"))
        {
            std::string result = Logger::hexStr(hashing(std::string(argv[0])));
            std::cout << "Digest about me: " << result << std::endl;
        }
        else if (!strcmp(argv[i], "--hashing"))
        {
            std::string result = Logger::hexStr(hashing(std::string(argv[i + 1])));
            std::cout << "Digest about " << argv[i + 1] << " : " << result << std::endl;
        }
        else if (!strcmp(argv[i], "--help"))
            return printHelp();
    }

    Logger::dLog("returning %d to caller.\n", license);
    Logger::deInitLogging();

    return license;
}
