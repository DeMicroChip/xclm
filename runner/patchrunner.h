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
#ifndef PATCHER_H
#define PATCHER_H

#include <boost/filesystem/path.hpp>    // for path
#include <string>                       // for string, streamsize
#include <system_error>                 // for error_code
#include "runner/abstractrunner.h"      // for AbstractRunner

/** forward declaration */
class AbstractVersion;

/**
 * @brief The Patcher class
 */
class PatchRunner : public AbstractRunner
{
    public:
        explicit PatchRunner(const AbstractVersion &baseVersion, const boost::filesystem::path &xclmFilePath);
        virtual ~PatchRunner() = default;

        virtual std::error_code run();

    protected:
        std::error_code makeBackup(const boost::filesystem::path &fileName) const;
        std::error_code patchFile(const boost::filesystem::path &fileName, const std::streamsize &offset, const std::string &strDigest) const;

        const std::string &mDigestToSearch;
        const boost::filesystem::path compilerBinDir;
        const boost::filesystem::path xclmFile;
};

#endif // PATCHER_H
