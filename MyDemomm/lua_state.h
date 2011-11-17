#ifndef C_LUA_STATE_HH_
#define C_LUA_STATE_HH_

#include "public.h"

#pragma comment(lib,"lua5.1.lib")

extern "C" 
{ 
#include "lua.h"    
#include "lualib.h"    
#include "lauxlib.h"
}

class CLuaState : public CSingleton<CLuaState>
{
public:
	CLuaState();
	~CLuaState();

	lua_State *GetLuaState();

	float ReadIniFile(const char *filename,const char *index); //读取配置文件项

private:
	lua_State *m_L;

};

#endif