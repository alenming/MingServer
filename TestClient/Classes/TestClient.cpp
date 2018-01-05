#include "TestClient.h"
#include "BufferTool.h"

#include "GameNetworkNode.h"
#include <string>

#include "LogManager_s.h"
#include "LogFileHandler_s.h"
#include "LogConsoleHandler_s.h"


TestClient* TestClient::m_Instance = NULL;
TestClient::TestClient(void)
:m_ServerCon(nullptr)
{
}

TestClient::~TestClient(void)
{
	delete m_ServerCon;
}

TestClient* TestClient::getInstance()
{
    if (NULL == m_Instance)
    {
        m_Instance = new TestClient();
    }
    return m_Instance;
}

void TestClient::destroy()
{
	CGameNetworkNode::getInstance()->destroy();

    if (NULL != m_Instance)
    {
        delete m_Instance;
        m_Instance = NULL;
    }
}

bool TestClient::onServerInit()
{

	LogManager_s::getInstance()->setShowTime(true);
	LogManager_s::getInstance()->setShowDate(true);
	LogManager_s::getInstance()->addHandler(1, new LogConsoleHandler_s());
	LogFileHandler_s* pFileHandle = new LogFileHandler_s();
	pFileHandle->setFilePath("../bin/");
	pFileHandle->setFileName("TestClientOut");
	pFileHandle->setFastModel(false);
	LogManager_s::getInstance()->addHandler(2, pFileHandle);
	

	m_ServerCon = new ConnectServerConf();
	m_ServerCon->serverGroup = 1;
	m_ServerCon->host = "127.0.0.1";
	m_ServerCon->port = 8888;


	if (CGameNetworkNode::getInstance()->connectToServer((char *)m_ServerCon->host.c_str(), m_ServerCon->port))
	{
		KX_LOGDEBUG("ClientTest Connect to IP=%s Port=%d successful", m_ServerCon->host.c_str(), m_ServerCon->port);
	}

	return true;
}

bool TestClient::login()
{
	BufferData* buffer = newBufferData(MAIN_CMD::CMD_LOGIN_SERVER, LOGIN_CMD::CMD_LOGIN_CS);

	LOGIN_DATA da = LOGIN_DATA();
	da.accountId = 123456;
	da.passwd = 111111;
	da.accountId = 0;//无用ID
	buffer->writeData(da);

	//重新写入数据长度
	char* bu = buffer->getBuffer();
	Head* head = reinterpret_cast<Head*>(bu);
	head->length = buffer->getDataLength();
	head->uid = 0;//服务器用ID
	bool isSuccessful = false;
	if (CGameNetworkNode::getInstance()->sendData(buffer->getBuffer(), buffer->getDataLength()) > 0)
	{
		KX_LOGDEBUG("CGameNetworkNode::sendData to server successful!");
		isSuccessful = true;
		
	}
	deleteBufferData(buffer);
	return isSuccessful;
}

