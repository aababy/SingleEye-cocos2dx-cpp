//
//  NetTest.h
//  coslua
//
//  Created by chency on 13-9-3.
//
//

#ifndef coslua_NetTest_h
#define coslua_NetTest_h

#include "GameInclude.h"
#include "GameDefine.h"

USING_NAMESPACE_COCOS

//#define NET_TEST          //上传到服务器的版本请屏蔽掉这个宏.  by luoxp

class NetTest : public CCObject
{
public:
    // 单例设计
    static NetTest* Instance();
    void init();
    void begin();
    void DO_CMD_REQ_LOGIN();
    void HandleMsg(const Message &msg);//消息事件处理函数
private:
    int index_getFuBen; // 得到副本的记录数，用于测试时获取全部副本
};

#endif
