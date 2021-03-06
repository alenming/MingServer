#ifndef __SESSIONSERVER_H__
#define __SESSIONSERVER_H__

#include "KXServer.h"
#include "ServerConfig.h"


class LoginServer : public KxBaseServer
{
private:
    LoginServer(void);
    virtual ~LoginServer(void);

public:
    static LoginServer* getInstance();
    static void destroy();

	//动态更新服务器
	bool dynamicUpdate();											
	//服务器信息初始化
	virtual bool initServerInfo();
	//服务器模块初始化
    virtual bool onServerInit();
	//设置服务器立刻关闭 
	void setSessionServerClose();							
	//设置服务器平滑开始标志
	void setSessionServerClosing();							
	//得到平滑关闭标志位
	bool getClosingFlag(){ return m_IsClosing; }

private:
	void AddModelType();

private:
	bool					m_IsClosing;	//开启平滑关闭时的标志
	static LoginServer*		m_Instance;		//单例

	ServerData				m_ServerData;
	ServerConfig			m_ServerConfig;	//服务器数读取配置表
};

#endif //__SESSIONSERVER_H__
