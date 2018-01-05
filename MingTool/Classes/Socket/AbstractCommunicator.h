#ifndef AbstractCommunicator_h__
#define AbstractCommunicator_h__

#include "ICommListener.h"

MING_BEGIN

class AbstractCommunicator
{

public:

	AbstractCommunicator();
	virtual ~AbstractCommunicator();

public:

	//添加消息监听器
	virtual void addListener(ICommListener* pListener);

	//移除消息监听器
	virtual void removeListener(ICommListener* pListener);

	//接收消息，并向监听者下发
	virtual void recvMessage( int nCmdID, const char* szData );

	//发送消息
	virtual void sendMessage( int nCmdID, const char* szData );

protected:

	 const char*  constructErrorMessage(int nCmdID, const char* szError);

protected:

	//监听器队列
	vector<ICommListener*> mVecListener;

};

MING_END

#endif //AbstractCommunicator_h__