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
#include <iostream>
#include <exception>
#include <boost/filesystem/path.hpp>
#include "xclm.h"
#include "logging.h"
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
            XclmV202().printDigest(std::cout, po.hashDigitOffset(), 8U);
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
