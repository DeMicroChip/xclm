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
#ifndef TRYRUN_H
#define TRYRUN_H

#include <boost/filesystem/path.hpp>    // for path
#include <string>                       // for string
#include <system_error>                 // for error_code
#include "abstractrunner.h"             // for AbstractRunner

class AbstractVersion;

/**
 * @brief try patching an unknown compiler version
 */
class TestRunner : public AbstractRunner
{
    public:
        explicit TestRunner(const boost::filesystem::path& compilerBin, const AbstractVersion& hashTbl)
        : AbstractRunner(),
          hashTable(hashTbl),
          compilerBinDir(compilerBin) {
        }
        virtual ~TestRunner() = default;

        virtual std::error_code run() override;

    protected:
        virtual std::error_code checkNeededFilesExists(const boost::filesystem::path &compilerBinDir) const override;

    private:
        bool searchDigest(const boost::filesystem::path &compilerBinDir, const std::string strDigest) const;
        bool searchHiddenTable(const boost::filesystem::path &compilerBinDir) const;

    private:
        const AbstractVersion& hashTable;
        const boost::filesystem::path compilerBinDir;
        static constexpr auto xclmBin = "xclm";
};

#endif // TRYRUN_H
