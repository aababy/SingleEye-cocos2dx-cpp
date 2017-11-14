//
//  LuaScript.cpp
//  coslua
//
//  Created by chency on 13-5-31.
//
//

#include "cocos2d.h"
#include "LuaScript.h"

LuaScript::LuaScript(lua_State * l, LuaManager * man, int registryRef):
next_(NULL)
,registryRef_(registryRef)
,state_(l)
,manager_(man)
,yieldMode_(YM_NONE)
,waitFrames_(0)
,waitTime_(0)
{

}

LuaScript::~LuaScript(void)
{
	luaL_unref(state_, LUA_REGISTRYINDEX, registryRef_);
}

int LuaScript::LoadFile(const char * name)
{
	printf("LuaScript::LoadFile() - %s\n",name);
	int err = luaL_loadfile(state_, name);
	if(err){
		printf("luaL_loadfile Error- %s\n",lua_tostring(state_,-1));
		CCLOG("luaL_loadfile Error- %s\n",lua_tostring(state_,-1));
		lua_pop(state_,1);
	}

	lua_resume(state_, 0);
    return err;
}

void LuaScript::LoadString(const char * buffer)
{
	int err = luaL_loadstring(state_, buffer);
	if(err){
		printf("%s",lua_tostring(state_,-1));
		lua_pop(state_,1);
	}

	lua_resume(state_, 0);
}

LuaScript * LuaScript::Update(float elapsedSeconds)
{
	if (yieldMode_ == YM_TIME) {
		waitTime_ -= elapsedSeconds;
		if(waitTime_ > 0)
			return next_;
	}

	if( yieldMode_ == YM_FRAME){
		--waitFrames_;
		if (waitFrames_ > 0) {
			return next_;
		}
	}

	if( yieldMode_ == YM_PAUSE){
		return next_;
	}

	yieldMode_ = YM_NONE;
	lua_resume(state_, 0);
	return next_;
}

void LuaScript::YieldFrames(int num)
{
	yieldMode_ = YM_FRAME;
	waitFrames_ = num;
}

void LuaScript::YieldSeconds(float secs)
{
	printf("YieldSeconds \n");
	yieldMode_ = YM_TIME;
	waitTime_ = secs;
}

void LuaScript::YieldPause()
{
	yieldMode_ = YM_PAUSE;
}

void LuaScript::YieldResume()
{
	printf("resume \n");
	yieldMode_ = YM_NONE;
}
