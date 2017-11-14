//
//  LuaManager.h
//  coslua
//
//  Created by chency on 13-5-31.
//
//

#ifndef coslua_LuaManager_h
#define coslua_LuaManager_h
#include <string>

extern "C"{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};

using namespace std;

class LuaScript;

class LuaManager{
public:
	LuaManager (void);
	~LuaManager(void);

	bool LuaOpenLibrary(const char *name, const luaL_reg *libs);

	void SetGlobalNumber(const char *name, double val);
	void SetGlobalInteger(const char *name, int val);
	void SetGlobalString(const char *name, const char *val);

	LuaScript* CreateScript(void);
    LuaScript* CreateScript(const char *name);
	void DestroyScript(LuaScript *s);

	void Update(float elapsedSeconds);
    
    int GetInt(char* variableName);
    double GetFloat(char* variableName);
    double GetDouble(char* variableName);
    std::string GetString(char* variableName);
    bool GetBool(char* variableName);
    int GetFuncResult(char *funcName,int nParam1, ...);
    int GetFuncResultWithoutParam(char *funcName);
private:
	lua_State *m_pMainState;
	LuaScript *m_pLuaScript;
};

#endif
