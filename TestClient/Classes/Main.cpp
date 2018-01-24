#include <iostream>
#include <string>
#include <ctime> 
#include "TestClient.h"
#include "GameNetworkNode.h"

//#include "Lua/LuaRegiest.h"
//#include "Lua/LuaStack.h"
//#include "LogManager/LogFileHandler.h"
//#include "LogManager/LogConsoleHandler.h"
//#include "LogManager/LogManager.h"

using namespace std;

void   Delay(int   time)//time*1000为秒数 
{
	clock_t   now = clock();

	while (clock() - now < time);
}

int main(int argc, char ** argv) 
{
	
	TestClient::getInstance()->onServerInit();
	auto poll = CGameNetworkNode::getInstance()->getPoller();
	poll->poll();

	Delay(3 * 1000);   //延时5秒
	TestClient::getInstance()->login();

	cout << sizeof(poll) << endl;
	while (true)
	{
		poll->poll();
	}

	char temp = ' ';
	cout << "登录中,任意键关闭" << endl;
	scanf("%c", &temp);
    TestClient::destroy();

	//LogManager::getInstance()->setShowTime(true);
	//LogManager::getInstance()->setShowDate(true);
	//LogManager::getInstance()->addHandler(1, new LogConsoleHandler());
	//LogFileHandler* pFileHandle = new LogFileHandler();
	//pFileHandle->setFilePath("../bin/");
	//pFileHandle->setFileName("TestOut");
	//pFileHandle->setFastModel(false);
	//LogManager::getInstance()->addHandler(2, pFileHandle);

	//regiestLuaFunction();
	//LuaStack::getInstance()->executeScriptFile("main.lua");
	//LogManager::destroy();

	//while (true)
	//{

	//}
    return 0;
}
