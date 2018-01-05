#ifndef __KX_LOG_CONSOLE_HANDLER_H__
#define __KX_LOG_CONSOLE_HANDLER_H__

#include "LogManager_s.h"

class LogConsoleHandler_s : public ILogHandler_s
{
public:
	LogConsoleHandler_s()
	{	
	}
	virtual ~LogConsoleHandler_s()
	{
	}
    virtual bool onLog(int level, const std::string& log);
};



#endif // __KX_LOG_CONSOLE_HANDLER_H__
