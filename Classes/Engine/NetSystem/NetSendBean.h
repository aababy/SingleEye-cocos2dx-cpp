//
//  NetSendBean.h
//  coslua
//
//  Created by chency on 13-6-9.
//
//

#ifndef coslua_NetSendBean_h
#define coslua_NetSendBean_h


#include "EngineDefine.h"
#include "cocos2d.h"

USING_NS_CC;

class NetSendBean {
    
public:
    
    NetSendBean();
    void init(CCObject *pSender, byte *pMsg, int msgLen, int _cmd);

    bool operator<(const NetSendBean &other) const;
public:
    CCObject *m_pMsgSender;
    int m_nPackageID;
    // 发送的内容
    byte *m_pSendBuffer;
    // 内容长度（m_pSendBuffer的长度）
    int m_nSendLen;
    //命令字
    int16_t cmd;
    

};

#endif
