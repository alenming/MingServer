#include <iostream>
#include <string>
#include <ctime> 
#include "TestClient.h"
#include "GameNetworkNode.h"

//#include "Lua/LuaRegiest.h"
//#include "Lua/LuaStack.h"

#include "log/LogFileHandler.h"
#include "log/LogConsoleHandler.h"
#include "log/LogManager.h"

#include "CommonHead.h"
#include <io.h>
#include <cstringt.h>
#include "windows.h"

using namespace std;

void listFiles(const char * dir)
{
	char dirNew[200];
	strcpy(dirNew, dir);
	strcat(dirNew, "\\*.*");    // 在目录后面加上"\\*.*"进行第一次搜索

	intptr_t handle;
	_finddata_t findData;

	handle = _findfirst(dirNew, &findData);
	if (handle == -1)        // 检查是否成功
		return;

	do
	{
		if (findData.attrib & _A_SUBDIR)
		{
			if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
				continue;

			//cout << findData.name << "\t<dir>\n";

			KX_LOGDEBUG(" findData.name=%s <dir>", findData.name);

			// 在目录后面加上"\\"和搜索到的目录名进行下一次搜索
			strcpy(dirNew, dir);
			strcat(dirNew, "\\");
			strcat(dirNew, findData.name);

			listFiles(dirNew);
		}
		else
		{
			KX_LOGDEBUG(" findData.name=%s size=%d", findData.name, findData.size);
			//cout << findData.name << "\t" << findData.size << " bytes.\n";
		}
			
	} while (_findnext(handle, &findData) == 0);

	_findclose(handle);    // 关闭搜索句柄
}



void show_file(char path[], int level = 0)
{
	char find_path[128];
	sprintf(find_path, "%s*", path);

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	bool bContinue = true;

	hFind = FindFirstFile(find_path, &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE)
		return;

	while (bContinue)
	{
		if (strcmp(FindFileData.cFileName, "..") && strcmp(FindFileData.cFileName, "."))
		{
			for (int i = 0; i < level; ++i)
				cout << "  ";
			cout << FindFileData.cFileName << endl;

			if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
			{
				sprintf(find_path, "%s%s\\", path, FindFileData.cFileName);
				show_file(find_path, level + 1);
			}
		}
		bContinue = FindNextFile(hFind, &FindFileData);
	}

}


void   Delay(int   time)//time*1000为秒数 
{
	clock_t   now = clock();

	while (clock() - now < time);
}

int main(int argc, char ** argv) 
{
	
	//TestClient::getInstance()->onServerInit();
	//auto poll = CGameNetworkNode::getInstance()->getPoller();
	//poll->poll();

	//Delay(3 * 1000);   //延时5秒
	//TestClient::getInstance()->login();

	//while (true)
	//{
	//	poll->poll();
	//}

	//char temp = ' ';
	//cout << "登录中,任意键关闭" << endl;
	//scanf("%c", &temp);
 //   TestClient::destroy();

	LogManager::getInstance()->setShowTime(false);
	LogManager::getInstance()->setShowDate(false);
	LogManager::getInstance()->addHandler(1, new LogConsoleHandler());
	LogFileHandler* pFileHandle = new LogFileHandler();
	pFileHandle->setFilePath("../bin/");
	pFileHandle->setFileName("TestOut");
	pFileHandle->setFastModel(false);
	LogManager::getInstance()->addHandler(2, pFileHandle);

	//regiestLuaFunction();
	//LuaStack::getInstance()->executeScriptFile("main.lua");
	//LogManager::destroy();
	
	std::string dir = "./";

	//listFiles(dir.c_str());

	show_file("./");

	while (true)
	{

	}
    return 0;
}
