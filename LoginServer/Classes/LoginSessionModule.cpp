#include "LoginSessionModule.h"
#include "GateManager.h"
#include "LoginService.h"

LoginSessionModule::LoginSessionModule(void)
{
}

LoginSessionModule::~LoginSessionModule(void)
{
}

void LoginSessionModule::processLogic(char* buffer, unsigned int len, IKxComm *target)
{
	// 直接转发给游戏服务器
	Head* head = reinterpret_cast<Head*>(buffer);
	int nMainCmd = head->MainCommand();
	int nSubCmd = head->SubCommand();
	int uid = head->uid;
	int length = head->length;

	switch (nMainCmd)
	{
	case CMD_LOGIN_SERVER:
		LoginService::processService(nSubCmd, uid, buffer, length, target);
		break;
	default:
		break;
	}
}

void LoginSessionModule::processError(IKxComm *target)
{
    // 玩家断线
    this->userDisconnect(target);
}

// 1. 告诉所有服务器，玩家XXX掉线了
// 2. 从ClientManager中移除该玩家
void LoginSessionModule::userDisconnect(IKxComm *target)
{

}
