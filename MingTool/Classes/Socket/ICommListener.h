#ifndef ICommListener_h__
#define ICommListener_h__

#include "../Common/CommonHead.h"

#include "json/json.h"

#define COMMUNICATION_SUCCESS 1
#define COMMUNICATION_FAIL 0

using namespace std;

class ICommListener
{

public:

	//消息接收接口
	virtual void onRecvMessage(int nCmdID, const char* szData) = 0;

	//异常处理接口
	virtual bool throwException(const Json::Value& root)
	{
		int result = root["result"].asInt();
		if (result == COMMUNICATION_SUCCESS)
		{
			return false;
		}
		else
		{
			string msg = root["msg"].asString();
			MINGLOG("-----recvMessage error: msg=%s-----", msg.c_str());
			return true;
		}
	}

};

#endif // ICommListener_h__
