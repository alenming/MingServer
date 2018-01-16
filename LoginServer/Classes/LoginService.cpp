#include "LoginService.h"
#include "common/GameUserManager.h"
#include "GateManager.h"
#include "helper/BufferTool.h"

#include "Protocol.h"

using namespace std;


void LoginService::processService(int maincmd, int subcmd, int uid, char *buffer, int len, IKxComm *commun)
{
	if (maincmd != MAIN_CMD::CMD_LOGIN_SERVER)
	{
		KX_LOGERROR(" LoginService::processService-> maincmd !=  MAIN_CMD::CMD_LOGIN_SERVER");
		return;
	}
	switch (subcmd)
	{
	case LOGIN_CMD::CMD_C2S_LOGIN:
		CMD_C2S_LOGIN(uid, buffer, len, commun);
		break;
	default:
		break;
	}
}

void LoginService::CMD_C2S_LOGIN(int uid, char *buffer, int len, IKxComm *commun)
{
	// 直接转发给游戏服务器
	Head* head = reinterpret_cast<Head*>(buffer);

	if (head->uid != uid || head->uid < 0)
	{
		return;
	}

	LOGIN_DATA *loginCS = reinterpret_cast<LOGIN_DATA*>(head->data());

	// 获得CGameUser
	GameUser* pGameUser = CGameUserManager::getInstance()->getGameUser(uid);
	if (NULL == pGameUser)
	{
		//新用户
		pGameUser = CGameUserManager::getInstance()->newGameUser(uid, loginCS->accountId);
	}
	else
	{
		// 如果不是新用户，断线后会在一段时间内自动移除
		// 改方法会剔除移除列表数据，不让它自动释放，因为我胡汉三又回来了
		CGameUserManager::getInstance()->donotDeleteUser(uid);
		pGameUser->refreshModel(MODELTYPE_USER);
	}

	int nCurTime = KxBaseServer::getInstance()->getTimerManager()->getTimestamp();
	UserModel *pUserModel = dynamic_cast<UserModel*>(pGameUser->getModel(MODELTYPE_USER));


	CMD_S2C_LOGIN(uid);
}

void LoginService::CMD_S2C_LOGIN(int uid)
{
	// 开始下发数据
	LOGIN_DATA loginSC;

	BufferData* buffer = newBufferData(MAIN_CMD::CMD_LOGIN_SERVER, LOGIN_CMD::CMD_S2C_LOGIN);

	GameUser* pGameUser = CGameUserManager::getInstance()->getGameUser(uid);
	UserModel* pUserModel = dynamic_cast<UserModel*>(pGameUser->getModel(MODELTYPE_USER));

	loginSC.accountId = pGameUser->getAccountId();
	buffer->writeData(loginSC);

	//重新写入数据长度
	char* bu = buffer->getBuffer();
	Head* head = reinterpret_cast<Head*>(bu);
	head->uid = uid;
	head->length = buffer->getDataLength();

	//发送用户数据
	GateManager::getInstance()->Send(buffer->getBuffer(), head->length);
	KX_LOGDEBUG("LoginFinish uid = %d", head->uid);
}

void LoginService::processUserReconect(int uid, char *buffer, int len, IKxComm *commun)
{
	// 获得CGameUser
	GameUser* pGameUser = CGameUserManager::getInstance()->getGameUser(uid);
	if (NULL == pGameUser)
	{
		return;
	}

	//重置数据
	CGameUserManager::getInstance()->reSetGameUserData(uid);
	// 如果不是新用户，断线后会在一段时间内自动移除
	// 改方法会剔除移除列表数据，不让它自动释放，因为我胡汉三又回来了
	CGameUserManager::getInstance()->donotDeleteUser(uid);
}

