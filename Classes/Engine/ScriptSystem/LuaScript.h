//
//  LuaScript.h
//  coslua
//
//  Created by chency on 13-5-31.
//
//

#ifndef coslua_LuaScript_h
#define coslua_LuaScript_h

extern "C"{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};

class LuaManager;
class LuaScript{
public:
	int LoadFile(const char * name);
	void LoadString(const char * buffer);

	//used internally by library- not for end user use
	void YieldFrames(int num);
	void YieldSeconds(float secs);
	void YieldPause();
	void YieldResume();

	

	LuaScript(lua_State *, LuaManager * man, int registryRef);
	~LuaScript(void);
	LuaScript * Update(float elapsedSeconds);
	LuaScript * next_;
	int registryRef_;

private:
	typedef enum {
		YM_NONE,
		YM_FRAME,
		YM_TIME,
		YM_PAUSE
	} YIELDMODE;

	lua_State * state_;
	LuaManager * manager_;
	YIELDMODE yieldMode_;
	int waitFrames_;
	float waitTime_;
};

#endif
