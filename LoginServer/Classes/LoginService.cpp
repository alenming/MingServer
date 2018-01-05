#include "LoginService.h"
#include "helper/BufferTool.h"
#include "common/GameUserManager.h"
#include "GateManager.h"

using namespace std;


void LoginService::processService(int subcmd, int uid, char *buffer, int len, IKxComm *commun)
{
	switch (subcmd)
	{
	case LOGIN_CMD::CMD_LOGIN_CS:
		processUserLogin(uid, buffer, len, commun);
		break;
	default:
		break;
	}
}

void LoginService::processUserLogin(int uid, char *buffer, int len, IKxComm *commun)
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
		pGameUser = CGameUserManager::getInstance()->newGameUser(uid);
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



	// 开始下发数据
	LOGIN_DATA loginSC;

	BufferData* bufferRe = newBufferData(MAIN_CMD::CMD_LOGIN_SERVER, LOGIN_CMD::CMD_LOGIN_SC);
	loginSC.accountId = loginCS->accountId;
	loginSC.passwd = loginCS->passwd;
	bufferRe->writeData(loginSC);

	//重新写入数据长度
	char* bu = bufferRe->getBuffer();
	Head* headRe = reinterpret_cast<Head*>(bu);
	headRe->uid = uid;
	headRe->length = bufferRe->getDataLength();

	GateManager::getInstance()->Send(bufferRe->getBuffer(), headRe->length);

	//发送用户数据
	//sendUserModel(loginCS->id, pUserModel);
	KX_LOGDEBUG("LoginFinish uid = %d", headRe->uid);
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

