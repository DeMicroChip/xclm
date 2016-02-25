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
#ifndef POPTIONS_H
#define POPTIONS_H

#include <sstream>
#include <boost/program_options.hpp>
#include <cstdint>
#include "version/v202.h"
#include "logging.h"

class ProgrammOptions
{
    public:
        explicit ProgrammOptions(int argc, char *argv[])
        : cmdOptions(u8"Options used by the patcher") {
            try {
                progName = argv[0];

                cmdOptions.add_options()
                    (HELP, u8"this screen")
                    (HASHTEST, u8"test internal SHA0 implementation")
                    (PATCH, u8"patch known version of XC compiler")
                    (SELFHASHING, u8"calculate digest about this file and print it")
                    (HASHING, boost::program_options::value<std::uint32_t>(), u8"calculate digest about given file and print it")
                    (TRYRUN, boost::program_options::value<std::string>(), u8"Check if an unsupported version of XC compiler can be patched. Give the path to the compiler bin directory.")
                    (PATCHUNKNOWN, boost::program_options::value<std::string>(), u8"Patch an unsupported version of XC compiler. arg is the new digest. Digest is printed by running --tryrun.");

                boost::program_options::options_description xclmOptions(u8"Hidden xclm command line arguments");
                xclmOptions.add_options()
                    (DUMPHASH, boost::program_options::value<int>(), u8"dump 10 digits of hidden hash table")
                    (SWXC_FULL, u8"request license for xc8 or xc16 or xc32 compiler")
                    (SWXCPP_FULL, u8"request license for xcpp32 compiler")
                    (LICENSEINFO, u8"show license information");


                boost::program_options::options_description allOptions;
                allOptions.add(cmdOptions).add(xclmOptions);

                auto func = std::bind(&ProgrammOptions::parseNonStdParameter, this, std::placeholders::_1);
                auto parsed = boost::program_options::command_line_parser(argc, argv).options(allOptions).allow_unregistered().extra_parser(func).run();
                boost::program_options::store(parsed, cmdOpts);
                boost::program_options::notify(cmdOpts);
            }
            catch (std::exception &ex) {
                LOG << u8"Parser error: " << ex.what() << std::endl;
                lastError =  ex.what();
            }

            std::stringstream sb;
            sb << u8"xclm called with command line parameters " << argc << std::endl;
            for (uint32_t i = 0; i < argc; i++)
                sb << argv[i] << u8" ";

            LOG << sb.str() << std::endl;
        }
        ~ProgrammOptions() = default;

        /**
         * @brief isHelp
         * @return
         */
        const bool isHelp() const {
            return cmdOpts.count(HELP) || lastError.size();
        }

        /**
         * @brief printHelp
         * @return
         */
        int printHelp() const {
            std::cerr << "[XCLM replacement and patcher (c)2015-2016]" << std::endl;
            std::cerr << "Copy this file into your XC compiler bin directory. Make a backup of the original xclm file before replacing with this version." << std::endl;
            std::cerr << "Known versions of xclm are" << std::endl;
            std::cerr << "\t" << XclmV202().name() << std::endl;
            std::cerr << std::endl;

            if (lastError.size())
                std::cerr << "[ERROR] " << lastError << std::endl << std::endl;

            std::cerr << cmdOptions << std::endl;

            return 255;
        }

        /**
         * @brief returns the name of the called programm. It is the content of argv[0]
         * @return
         */
        const std::string& programName() const {
            return progName;
        }

        const bool requestSwxcLicense() const {
            return cmdOpts.count(SWXC_FULL);
        }

        const bool requestSwxcppLicense() const {
            return cmdOpts.count(SWXCPP_FULL);
        }

        const bool dumpHashDigit() const {
            return cmdOpts.count(DUMPHASH);
        }

        const int& hashDigitOffset() const {
            return cmdOpts[DUMPHASH].as<int>();
        }

        const bool isLicenseInfo() const {
            return cmdOpts.count(LICENSEINFO);
        }

        const bool isSelfHashing() const {
            return cmdOpts.count(SELFHASHING);
        }

        const bool isPatch() const {
            return cmdOpts.count(PATCH);
        }

        const bool isHashing() const {
            return cmdOpts.count(HASHING);
        }

        const std::string& hashingFileName() const {
            return cmdOpts[HASHING].as<std::string>();
        }

        const bool isTryRun() const {
            return cmdOpts.count(TRYRUN);
        }

        const std::string& compilerBinDir() const {
            return cmdOpts[TRYRUN].as<std::string>();
        }

        const bool isSelftest() const {
            return cmdOpts.count(HASHTEST);
        }

        const bool isPatchUnknown() const {
            return cmdOpts.count(PATCHUNKNOWN);
        }

        const std::string& unknownDigest() const {
            return cmdOpts[PATCHUNKNOWN].as<std::string>();
        }

    private:
        /**
         * @brief parse all non standard xclm command line arguments
         * @param s
         * @return
         */
        std::pair<std::string, std::string> parseNonStdParameter(const std::string &s) {
            if ((s == u8"swxc32") || (s == u8"swxc16") || (s == u8"swxc8"))
                return std::make_pair<std::string, std::string>(SWXC_FULL, std::string());
            else if (s == u8"swxcpp32")
                return std::make_pair<std::string, std::string>(SWXCPP_FULL, std::string());
            else if (s == u8"-hash")
                return std::make_pair<std::string, std::string>(DUMPHASH, std::string());
            else if (s == u8"-licenseinfo")
                return std::make_pair<std::string, std::string>(LICENSEINFO, std::string());

            return std::make_pair<std::string, std::string>(std::string(), std::string());
        }

    private:
        boost::program_options::options_description cmdOptions;
        boost::program_options::variables_map cmdOpts;
        std::string lastError;
        std::string progName;


        static constexpr auto HELP = u8"help";
        static constexpr auto HASHTEST = u8"hashtest";
        static constexpr auto PATCH = u8"patch";
        static constexpr auto PATCHUNKNOWN = u8"patchunknown";
        static constexpr auto SELFHASHING = u8"selfhashing";
        static constexpr auto HASHING = u8"hashing";
        static constexpr auto TRYRUN = u8"tryrun";

        // non standard xclm commandline arguments
        static constexpr auto DUMPHASH = u8"hash";
        static constexpr auto SWXC_FULL = u8"swxc8";
        static constexpr auto SWXCPP_FULL = u8"swxcpp32";
        static constexpr auto LICENSEINFO = "licenseinfo";
        static constexpr auto FULL_CHECKOUT = "full-checkout-for-compilers";
        static constexpr auto LIC_INFO = "licinfo";
};

#endif // POPTIONS_H
