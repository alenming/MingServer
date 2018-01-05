#ifndef __LUA_STACK_H_
#define __LUA_STACK_H_

#include "../Common/CommonHead.h"

extern "C"{
#include "luajit/lua.h"
#include "luajit/lauxlib.h"
#include "luajit/lualib.h"
}

#if _MSC_VER > 1800
#pragma comment(lib,"lua51-2015.lib")
#else
#pragma comment(lib,"lua51.lib")
#endif



class LuaStack
{
public:
	LuaStack();
	virtual ~LuaStack();
	static LuaStack* getInstance();
	static void destroy();
private:
	int luaLoadBuffer(lua_State *L, const char *chunk, int chunkSize, const char *chunkName);
public:
	lua_State* getLuaState();
	int executeScriptFile(const char* filename);
	int executeFunction(int numArgs);
	int executeGlobalFunction(const char* functionName);
	int executeString(const char *codes);
	void addSearchPath(const char* path);

protected:
	lua_State *_state;
	static LuaStack* m_Instance;
};



#endif // __LUA_STACK_H_
