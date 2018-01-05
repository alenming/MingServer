#include "LogConsoleHandler_s.h"

bool LogConsoleHandler_s::onLog(int level, const std::string& log)
{
    printf("%s\n", log.c_str());
    return true;
}



