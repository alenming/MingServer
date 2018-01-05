#ifndef __LOG_CONSOLE_HANDLER_H__
#define __LOG_CONSOLE_HANDLER_H__

#include "LogManager/LogManager.h"
//#include "../Common/CommonHead.h"

class LogConsoleHandler : public ILogHandler
{
public:
	LogConsoleHandler()
	{	
	}
	virtual ~LogConsoleHandler()
	{
	}
    virtual bool onLog(int level, const std::string& log);
};



#endif
