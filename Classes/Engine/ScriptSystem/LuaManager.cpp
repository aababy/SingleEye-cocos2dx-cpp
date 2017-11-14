//
//  LuaManager.cpp
//  coslua
//
//  Created by chency on 13-5-31.
//
//

#include "cocos2d.h"

#include "LuaManager.h"
#include "LuaScript.h"

#pragma mark --脚本函数对应的内部静态函数（内部调用不公开）
static int LuaYieldSeconds(lua_State * l)
{
	LuaScript * s;
	lua_pushlightuserdata(l,l);
	lua_gettable(l, LUA_GLOBALSINDEX);
	s = (LuaScript *)lua_touserdata(l,-1);

	float n = lua_tonumber(l,1);
	s->YieldSeconds(n);
	printf("waitSeconds %f\n",n);

	return (lua_yield(l, 0));
}

static int LuaYieldFrames(lua_State * l)
{
	LuaScript * s;
	lua_pushlightuserdata(l,l);
	lua_gettable(l, LUA_GLOBALSINDEX);
	s = (LuaScript *)lua_touserdata(l,-1);

	int f = lua_tonumber(l,1);
	s->YieldFrames(f);
	//printf("waitFrames %d\n",f);

	return (lua_yield(l, 0));
}

static int LuaYieldPause(lua_State * l)
{
	LuaScript * s;
	lua_pushlightuserdata(l,l);
	lua_gettable(l, LUA_GLOBALSINDEX);
	s = (LuaScript *)lua_touserdata(l,-1);

	s->YieldPause();
	printf("pause \n");

	return (lua_yield(l, 0));
}
#pragma mark- --Create, Destroy and Delete
LuaManager::LuaManager (void):
m_pLuaScript(NULL)
{
	m_pMainState = lua_open();

	//Add functions for waiting
	static const luaL_reg scriptLib[] =
	{
		{"waitSeconds", LuaYieldSeconds },
		{"waitFrames", LuaYieldFrames },
		{"pause", LuaYieldPause },
		{NULL, NULL}
	};
	LuaOpenLibrary( "script", scriptLib);

	//add the base library
	luaopen_base(m_pMainState);
	luaopen_math(m_pMainState);
	luaopen_string(m_pMainState);
	lua_settop(m_pMainState, 0);

}

LuaManager::~LuaManager(void)
{
	LuaScript * n = m_pLuaScript;
	while (n) {
		LuaScript * i = n->next_;
		delete (n);
		n=i;
	}
    m_pLuaScript = NULL;
	lua_close(m_pMainState);
}
LuaScript * LuaManager::CreateScript(void)
{
	lua_State * s = lua_newthread(m_pMainState);

	//Add reference to the new thread in the Lua Registry so
	//will not be garbage collected
	int r = luaL_ref(m_pMainState, LUA_REGISTRYINDEX);

	//Create a new LuaScript object to hold the new thread
	LuaScript * ns = new LuaScript(s,this,r);

	//Add an entry to map the new lua_State to the
	//new LuaScript object
	lua_pushlightuserdata(m_pMainState, s);
	lua_pushlightuserdata(m_pMainState, ns);
	lua_settable(m_pMainState, LUA_GLOBALSINDEX);

	//insert the new script into the list
	ns->next_ = m_pLuaScript;
	m_pLuaScript = ns;
	return ns;
}
LuaScript * LuaManager::CreateScript(const char * name)
{
    LuaScript *sc = CreateScript();
    int err = sc->LoadFile(name);
    if (err) {
        delete (sc);
        return NULL;
    }
    return sc;
}
void LuaManager::DestroyScript(LuaScript * s)
{
	if(s == m_pLuaScript){
		m_pLuaScript = s->next_;
		delete (s);
		return;
	}

	LuaScript * last = m_pLuaScript;
	for (LuaScript * l=last->next_; l != NULL; l=l->next_) {
		if (l == s) {
			last->next_ = l->next_;
			delete(l);
			return;
		}
		last = l;
	}
}
bool LuaManager::LuaOpenLibrary(const char * name, const luaL_reg * libs)
{
	if(m_pLuaScript != NULL)
		return 0;

	luaL_openlib(m_pMainState, name, libs, 0);
	return 1;
}
#pragma mark-
/**
 *	@brief	执行脚本更新
 *  外部调用该函数实现脚本的顺序执行
 *  每调用一次执行一条脚本命令
 *	@param 	elapsedSeconds 	时间间隔
 */
void LuaManager::Update(float elapsedSeconds)

{
	LuaScript * n = m_pLuaScript;
	while(n && m_pLuaScript)
		n = n->Update(elapsedSeconds);
}
#pragma mark- --获得配置参数

/**
 *	@brief	获得基本数据类型的模板函数
 *
 *	@param 	pL 	lua_State
 *	@param 	name 	const char*
 *
 *	@return	T
 */
template <class T>
inline T PopLuaNumber(lua_State* pL, const char* name)

{
    lua_settop(pL, 0);

    lua_getglobal(pL, name);

    //判断类型是否匹配
    if (!lua_isnumber(pL, 1))
    {
        std::string err("<lua_isnumber> Cannot retrieve: ");
        err += name;
        throw (err.c_str());
    }

    //获得抛出的变量
    T val = (T)lua_tonumber(pL, 1);

    // 删除变量
    lua_pop(pL, 1);

    return val;
}
int LuaManager::GetInt(char* variableName)
{
    return PopLuaNumber<int>(m_pMainState, variableName);
}

double LuaManager::GetFloat(char* variableName)
{
    return PopLuaNumber<float>(m_pMainState, variableName);
}

double LuaManager::GetDouble(char* variableName)
{
    return PopLuaNumber<double>(m_pMainState, variableName);
}

std::string LuaManager::GetString(char* variableName)
{
    lua_settop(m_pMainState, 0);

    lua_getglobal(m_pMainState, variableName);

    //判断类型是否匹配
    if (!lua_isstring(m_pMainState, 1))
    {
        std::string err("<lua_isstring> Cannot retrieve: ");
        err += variableName;
        throw (err.c_str());
    }

    //获得抛出的变量
    std::string s = lua_tostring(m_pMainState, 1);
    // 删除变量
    lua_pop(m_pMainState, 1);
    
    return s;
}

bool LuaManager::GetBool(char* variableName)
{
    lua_settop(m_pMainState, 0);

    lua_getglobal(m_pMainState, variableName);

    //判断类型是否匹配
    if (!lua_isboolean(m_pMainState, 1))
    {
        std::string err("<lua_isboolean> Cannot retrieve: ");
        err += variableName;
        throw (err.c_str());
    }
    //获得抛出的变量
    bool b = lua_toboolean(m_pMainState, 1);
    // 删除变量
    lua_pop(m_pMainState, 1);

    return b;
}

int LuaManager::GetFuncResult(char *funcName,int nParam1, ...)
{
    va_list args;
    va_start(args, nParam1);
    int count = 0;
    int array[4];
    array[count] = nParam1;
    count++;
    int n = va_arg(args, int);
    array[count] = n;
    while(n!=-1)
    {
        count++;
        n = va_arg(args, int);
        array[count] = n;
    }
    va_end(args);
    lua_settop(m_pMainState, 0);
    lua_getglobal(m_pMainState, funcName);
    for (int i = 0; i<count; i++)
    {
        lua_pushnumber(m_pMainState, array[i]);
    }
    lua_call(m_pMainState, count, 1);
    //判断类型是否匹配
    if (!lua_isnumber(m_pMainState, 1))
    {
        std::string err("<lua_isnumber> Cannot retrieve: ");
        err += funcName;
        throw (err.c_str());
    }
    int res = (int)lua_tonumber(m_pMainState, 1);
    lua_pop(m_pMainState, count);
    return res;
}

int LuaManager::GetFuncResultWithoutParam(char *funcName)
{
    lua_settop(m_pMainState, 0);
    lua_getglobal(m_pMainState, funcName);
    lua_call(m_pMainState, 0, 1);
    //判断类型是否匹配
    if (!lua_isnumber(m_pMainState, 1))
    {
        std::string err("<lua_isnumber> Cannot retrieve: ");
        err += funcName;
        throw (err.c_str());
    }
    int res = (int)lua_tonumber(m_pMainState, 1);
    lua_pop(m_pMainState, 1);
    return res;
}
#pragma mark-
void LuaManager::SetGlobalNumber(const char * name, double val)
{
	lua_pushnumber(m_pMainState, val);
	lua_setglobal(m_pMainState,name);
}

void LuaManager::SetGlobalInteger(const char * name, int val)
{
	lua_pushinteger(m_pMainState, val);
	lua_setglobal(m_pMainState,name);
}

void LuaManager::SetGlobalString(const char * name, const char * val)
{
	lua_pushstring(m_pMainState, val);
	lua_setglobal(m_pMainState,name);
}

