//
//  MsgSystem.h
//  coslua
//
//  Created by chency on 13-5-26.
//
//

#ifndef coslua_MsgSystem_h
#define coslua_MsgSystem_h

#include "cocos2d.h"
#include "Message.h"

USING_NS_CC;

// 定义一个函数类型指针
typedef bool (CCObject::*HANDLE_MSG)(const Message &msg);
#define MSGSYS_HANLE_MSG(_CBHandle_) (HANDLE_MSG)(&_CBHandle_)

class MsgSystem {
    
    
public:
    
    // 单例设计
    static MsgSystem* Instance();

    // 注册接收者和消息处理函数
    void RegisterObjAndHandler(CCObject *pObj, HANDLE_MSG pfun);
    // 注消接收者
    void ClearObjHandler(CCObject *pObj);
    
    // 发送消息
    // delay>0 表明是延迟消息，否则是及时消息
    void SendMsg(CCObject *sender, CCObject *receiver, int msgType, int64_t delay = 0, void *extraData = NULL);

    // 发送延迟消息
    // 由系统主循环调用
    void SendDelayedMsg();
    

private:  
    MsgSystem(){};
    
    MsgSystem(const MsgSystem&);
    MsgSystem& operator=(const MsgSystem&);

    void SendMsg(CCObject *receiver, const Message &msg);
    

private:
    // 注册的消息接收类以及对应的消息处理方法
    std::map<CCObject*, HANDLE_MSG> m_mapObj;

    // 延迟消息队列
    std::set<Message> m_setDelayedMsg;
    
};


#endif
