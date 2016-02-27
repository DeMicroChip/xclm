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
#ifndef XCLM_ERROR_H
#define XCLM_ERROR_H

#include <system_error>
#include <boost/system/error_code.hpp>

namespace xclm
{
    /**
     * @brief error constants
     */
    enum class errc {
        success = 0,
        try_run_failed,
        digest_not_found,
        not_a_directory,
        selftest_failed,
        backup_failed,
        patch_failed
    };

    /**
     * @brief The xclm_error_catergory class using std::error_code standard
     */
    class error_catergory : public std::error_category
    {
        public:
            explicit error_catergory() = default;
            virtual ~error_catergory() = default;

            /**
             * @brief name
             * @return
             */
            virtual const char* name() const noexcept override {
                return u8"xclm error category";
            }

            /**
             * @brief message
             * @param errCode
             * @return
             */
            virtual std::string message(int errCode) const override {
                switch (static_cast<xclm::errc>(errCode)) {
                    case xclm::errc::success:
                        return u8"Success";
                    case xclm::errc::try_run_failed:
                        return u8"Try run has failed. This version of XC compiler cannot be patched.";
                    case xclm::errc::digest_not_found:
                        return u8"Digest could not be found.";
                    case xclm::errc::not_a_directory:
                        return u8"Path is not a directory.";
                    case xclm::errc::selftest_failed:
                        return u8"Self test of SHA0 has failed.";
                    case xclm::errc::backup_failed:
                        return u8"Backup of file failed.";
                    case xclm::errc::patch_failed:
                        return u8"Patch of file 'xclm' failed.";
                    default:
                        break;
                 }
                return u8"Unknown error occured.";
            }
    };

    /**
     * template to create a xclm specific error code
     */
    template<typename T>
    std::error_code make_error_code(const T& code) {
        return std::error_code(static_cast<int>(code), xclm::error_catergory());
    }

    /** constexpr of non literal is not supported by C++11 */
    #define NOERROR  xclm::make_error_code(xclm::errc::success)

    /**
     * @brief let compiler cast from boost::system::error_code to std::error_code
     */
    namespace boost {
        namespace system {
            struct error_code : ::boost::system::error_code {
                operator std::error_code () const {
                   return std::make_error_code(static_cast<std::errc>(value()));
               }
            };
        }
    }
}

#endif // XCLM_ERROR_H
