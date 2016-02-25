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
#ifndef __LOGGING_H__
#define __LOGGING_H__

#include <string>
#include <sstream>
#include <fstream>
#include <chrono>
#include <iomanip>

/**
 * @brief The Logger class
 */
class Logger
{
    public:
        explicit Logger(const std::string &fileName = std::string(u8"/tmp/xclm.log")) {
            logFile.open(fileName.c_str(), std::ios::out | std::ios::app);
            if (logFile.good()) {
                auto timeNow = std::chrono::system_clock::now();
                auto timeNowTimeT = std::chrono::system_clock::to_time_t(timeNow);

                logFile << std::endl << std::endl;
                *this << u8"xclm running ..." << std::endl;
            }
        }

        ~Logger() {
            if (logFile.good()) {
                logFile.flush();
                logFile.close();
            }
        }

        // this is the type of std::cout
        typedef std::basic_ostream<char, std::char_traits<char> > CoutType;
        // this is the function signature of std::endl
        typedef CoutType& (*StandardEndLine)(CoutType&);
        // define an operator<< to take in std::endl
        Logger& operator << (StandardEndLine manip) {
            if (logFile.good()) {
                manip(sb);

                auto timeNow = std::chrono::system_clock::now();
                auto timeNowTimeT = std::chrono::system_clock::to_time_t(timeNow);
                logFile << std::put_time(std::localtime(&timeNowTimeT), u8"%F %T") << u8": " << sb.str();

                // clear the error state and empty the stringstream
                std::stringstream().swap(sb);
            }

            return *this;
        }

        template<typename T>
        Logger& operator << (const T &toLog) {
            sb << toLog;
            return *this;
        }

    private:
        std::ofstream logFile;
        std::stringstream  sb;
};

extern Logger LOG;

#endif //  __LOGGING_H__
