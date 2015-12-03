#ifndef __LOGGING_H__
#define __LOGGING_H__

#include <string>
#include <fstream>
#include <array>

class Logger
{
    public:
        static void initLogging();
        static void deInitLogging();
        static void dLog(const char* msg, ...);

        template<typename T>
        static std::string hexStr(T data, std::size_t  len)
        {
            std::string s(len * 2, ' ');
            for (int i = 0; i < len; ++i)
            {
                s[2 * i]     = Logger::hexmap[(data[i] & 0xF0) >> 4];
                s[2 * i + 1] = Logger::hexmap[data[i] & 0x0F];
            }
            return s;
        }

        template<typename T, std::size_t N>
        static std::string hexStr(const std::array<T, N> &arr) {
            return hexStr(arr.data(), arr.size());
        }


    private:
        static const std::array<char, 16> hexmap;
        static const std::string logFileName;

        static std::string logFileTemplate;
        static std::ofstream logFile;
};

#endif //  __LOGGING_H__
