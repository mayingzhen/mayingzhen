#include "lua_state.h"

CLuaState::CLuaState()
{
	/* ��ʼ��Lua */    
	m_L = lua_open();  

	/* ����Lua������ */    
	luaL_openlibs(m_L);  
}

CLuaState::~CLuaState()
{
	/* ���Lua */    
	lua_close(m_L);   
}

lua_State * CLuaState::GetLuaState()
{
	return m_L;
}

float CLuaState::ReadIniFile(const char *filename,const char *index) //��ȡ�����ļ���
{
	if (luaL_loadfile(m_L, filename) || lua_pcall(m_L, 0, 0, 0))
	{
		//error(L, "cannot run configuration file: %s",lua_tostring(L, -1));
		::MessageBox(NULL,lua_tostring(m_L, -1),"luaL_loadfile",MB_OK);
		lua_pop(m_L, 1);
		return 0;
	}

	lua_getglobal(m_L, index);

	if (!lua_isnumber(m_L, -1))
	{
		::MessageBox(NULL,"lua index not number","lua_isnumber",MB_OK);
		lua_pop(m_L, 1);
		return 0;
	}

	float num = lua_tonumber(m_L, -1);

	lua_pop(m_L, 1);  /* remove number */

	return num;
}