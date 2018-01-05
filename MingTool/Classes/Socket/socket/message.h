#ifndef MESSAGE_H
#define MESSAGE_H

#include "Util.h"
MING_BEGIN

class Message
{
public:
	
    
    char HEAD0;
    char HEAD1;
    char HEAD2;
    char HEAD3;
    char ProtoVersion;
    
    UnChar_t serverVersion[4];
    UnChar_t length[4];
    UnChar_t commandId[4];
    /**
      * 消息的数据
      */
    char* data;
	
	int type;
	unsigned int type_selfdefine;
	
	
	Message();
    int datalength();
	~Message();

	// 实现一个深拷贝;
	Message* clone();

	static Message* createMessage(const char* data, size_t dataSize, int commandId);
};

MING_END
#endif

