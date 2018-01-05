#include "ClientModule.h"
#include "KxCore.h"
#include "SessionClient.h"
#include "SessionConnect.h"
#include "NetworkManager.h"
#include "Protocol.h"
#include "SessionServer.h"
#include "BufferData.h"
#include <set>


ClientModule::ClientModule(void)
{
}

ClientModule::~ClientModule(void)
{
}

void ClientModule::processLogic(char* buffer, unsigned int len, IKxComm *target)
{
    SessionClient* pClient = dynamic_cast<SessionClient*>(target);
	ServerConfig * pServerConfig = SessionServer::getInstance()->getServerCoinfig();

	// 直接转发给游戏服务器
	Head* head = reinterpret_cast<Head*>(buffer);
	int nMainCmd = head->MainCommand();
	int nSubCmd = head->SubCommand();
	head->uid = pClient->getGuestId();//服务器之间通讯用玩家ID

	//LOGIN_DATA* a = reinterpret_cast<LOGIN_DATA*>(head->data());

	KX_LOGDEBUG("ClientModule onRecy Message!");

	pClient->sendDataToServer(nMainCmd, nSubCmd, buffer, len);
}

void ClientModule::processError(IKxComm *target)
{
    // 玩家断线
    this->userDisconnect(target);
}

// 1. 告诉所有服务器，玩家XXX掉线了
// 2. 从ClientManager中移除该玩家
void ClientModule::userDisconnect(IKxComm *target)
{
    SessionClient *pClient = dynamic_cast<SessionClient*>(target);
    if (pClient == NULL)
    {
        return;
    }

    // 设置下线消息
    Head head;
    head.MakeCommand(1, 1);
    head.length = sizeof(head);
    head.uid = pClient->getUserId();

    // 发送数据到后端告知角色下线
    // 发送下线消息给后端指定的服务器
    pClient->sendDataToAllServer(reinterpret_cast<char*>(&head), sizeof(head));
    // 关闭socket、从NetWorkManager中和clean移除
    pClient->clean();
}
