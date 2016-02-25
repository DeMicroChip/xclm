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
#include <array>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <cstdio>
#include "xclm.h"
#include "logging.h"
#include "hashes/sha0.h"
#include "util.h"
#include "poptions.h"
#include "version/v202.h"
#include "version/vunknown.h"
#include "runner/patchrunner.h"
#include "runner/testrunner.h"
#include "runner/digestrunner.h"
#include "runner/selftestrunner.h"


/**
 * @brief global loggin instance => log
 */
Logger  LOG;


/**
 * @brief test DIGEST0
 */
static const std::string DIGEST0("0164b8a914cd2a5e74c4f7ff082c4d97f1edf880");

/**
 * @brief simple SHA0 self test method
 */
static void hastTest()
{
    Hash::SHA0 hash;
    std::array<unsigned char, 3> buf = {{ 'a', 'b', 'c' }};
    hash.update(buf);

    Hash::SHA0::digest_t digest;
    hash.digest(digest);

    auto result = xclm::toHexString(digest);
    std::cout << "Calculated: " << result.c_str() << std::endl;
    std::cout << "Should be : " << DIGEST0.c_str() << std::endl;
    std::cout << "Valid     : " << std::boolalpha << (DIGEST0 == result.c_str()) << std::endl;
}

/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    LICENSE_MODEL  license(LICENSE_MODEL::LICENSE_FREE);

    try
    {
        ProgrammOptions po(argc, argv);

        if (po.isHelp())
            return po.printHelp();

        if (po.requestSwxcLicense())
            license = LICENSE_MODEL::LICENSE_PRO;
        else if (po.requestSwxcppLicense())
            license = LICENSE_MODEL::LICENSE_CPPFULL;

        if (po.dumpHashDigit())
            XclmV202().printDigest(std::cout, po.hashDigitOffset());
        else if (po.isLicenseInfo())
            std::cout << u8"Node Configuration" << std::endl;

        if (po.isSelftest())
            SelftestRunner().run();
        else if (po.isTryRun())
            TestRunner(po.compilerBinDir(), XclmV202()).run();
        else if (po.isHashing())
            DigestRunner(po.hashingFileName()).run();
        else if (po.isSelfHashing())
            DigestRunner(po.programName()).run();
        else if(po.isPatch())
            PatchRunner(XclmV202(), boost::filesystem::path(po.programName())).run();
        else if (po.isPatchUnknown())
            PatchRunner(XclmUnknown(po.unknownDigest()), boost::filesystem::path(po.programName())).run();
    }
    catch (std::exception &ex)
    {
        std::cerr << ex.what();
        LOG << ex.what();
    }

    LOG << "returning " << license << " to caller." << std::endl;

    return license;
}
