#include <stdarg.h>
#include "logging.h"

std::string   Logger::logFileTemplate("xclm_XXXXXX");
std::ofstream Logger::logFile;

const std::array<char, 16> Logger::hexmap = {{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' }};
const std::string          Logger::logFileName("/tmp/xclm.log");

/*
 *
 */
void Logger::dLog(const char* msg, ...)
{
    va_list args;

    va_start(args, msg);
    if (logFile.good())
    {
        char buf[255];
        vsnprintf(buf, sizeof(buf) - 1, msg, args);
        logFile << buf << std::endl;
        logFile.flush();
    }
    va_end(args);
}

/*
 *
 */
void Logger::initLogging()
{

    logFile.open(logFileName.c_str(), std::ofstream::out | std::ofstream::app);
    if (logFile.good())
        logFile << std::endl << std::endl << "xclm running ..." << std::endl;
}

/*
 *
 */
void Logger::deInitLogging()
{
    if (logFile.good())
        logFile.close();
}
