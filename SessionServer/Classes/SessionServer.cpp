#include "SessionServer.h"
#include "SessionListener.h"
#include "SessionClient.h"
#include "ConnectModule.h"
#include "ClientModule.h"
#include "SessionConnect.h"
#include "NetworkManager.h"

#include "KxPlatform.h"

#include "SessionEvent.h"
#include "ConnectEvent.h"

#include "NetworkManager.h"

#include "LogManager_s.h"
#include "LogFileHandler_s.h"
#include "LogConsoleHandler_s.h"

using namespace std;

SessionServer* SessionServer::m_Instance = NULL;
SessionServer::SessionServer(void)
:m_IsClosing(false)
{
	m_GameEvent = new SessionEvent;
}

SessionServer::~SessionServer(void)
{
	KXSAFE_RELEASE(m_Poller);
	m_ServerConfig.clean();
	delete m_GameEvent;
}

SessionServer* SessionServer::getInstance()
{
    if (NULL == m_Instance)
    {
        m_Instance = new SessionServer();
    }
    return m_Instance;
}

void SessionServer::destroy()
{
    if (NULL != m_Instance)
    {
        delete m_Instance;
        m_Instance = NULL;
    }
}

bool SessionServer::onServerInit()
{
	KX_LOGDEBUG("==================================================");
	KxBaseServer::onServerInit();

	LogManager_s::getInstance()->setShowTime(true);
	LogManager_s::getInstance()->setShowDate(true);
	LogManager_s::getInstance()->addHandler(1, new LogConsoleHandler_s());
	LogFileHandler_s* pFileHandle = new LogFileHandler_s();
	pFileHandle->setFilePath("../bin/");
	pFileHandle->setFileName("SessionServerOut");
	pFileHandle->setFastModel(false);
	LogManager_s::getInstance()->addHandler(2, pFileHandle);

	//1.初始化轮询器
#if(KX_TARGET_PLATFORM == KX_PLATFORM_LINUX)
	m_Poller = new KxEpoller();
#else
	m_Poller = new KxSelectPoller();
#endif


    SessionListener* listener = new SessionListener();
	if (!listener->init())
    {
        return false;
    }
	m_ServerData = m_ServerConfig.getServerDataByName("SessionServer");

	char *ip = NULL;
	if (m_ServerData.ip != "0" && m_ServerData.ip != "")
	{
		ip = (char *)m_ServerData.ip.c_str();
	}
	if (!listener->listen(m_ServerData.port, ip))
	{
		return false;
	}

	ClientModule *pClientModel = new ClientModule();
	listener->setClientModule(pClientModel);
    m_Poller->addCommObject(listener, listener->getPollType());
	KX_LOGDEBUG("SessionServer Launching IP=%s Port=%d \n\n", m_ServerData.ip.c_str(), m_ServerData.port);

	ConnectModule *pConnectModule = new ConnectModule();
	ConnectEvent *pGameEvent = new ConnectEvent();
	pConnectModule->init(pGameEvent);

	std::map<int, ServerData> alldaata = m_ServerConfig.getServerData();
	for (std::map<int, ServerData>::iterator iter = alldaata.begin(); iter != alldaata.end(); ++iter)
	{
		if (iter->second.name == "SessionServer")
		{
			break;
		}
		SessionConnector *pConnector = new SessionConnector();
		if (!pConnector->init() || !pConnector->connect((char *)iter->second.ip.c_str(), iter->second.port, iter->second.serverId, true))
		{
			KX_LOGERROR("SessionServer Connect to Server Failed!");
			return false;
		}
		KX_LOGDEBUG("======>>>>>>SessionServer Connect to Server: IP=%s, Port=%d \n", iter->second.ip.c_str(), iter->second.port);
		pConnector->setModule(pConnectModule);
		m_Poller->addCommObject(pConnector, pConnector->getPollType());
		NetWorkManager::getInstance()->addServer(iter->second.serverId, pConnector);
		KXSAFE_RELEASE(pConnector);
	}

	KXSAFE_RELEASE(pConnectModule);
	KXSAFE_RELEASE(listener);
	KXSAFE_RELEASE(pClientModel);

	return true;
}

bool SessionServer::initServerInfo()
{
	// 服务器配置
	if (!m_ServerConfig.create())
	{
		return false;
	}
	return true;
}

void SessionServer::setSessionServerClose()
{
	m_IsRunning = false;
}

void SessionServer::setSessionServerClosing()
{
	m_IsClosing = true;
}


