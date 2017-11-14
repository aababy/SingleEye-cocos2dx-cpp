//
//  NetRecvThread.h
//  coslua
//
//  Created by chency on 13-6-9.
//
//

#ifndef coslua_NetRecvThread_h
#define coslua_NetRecvThread_h

#include "socketcc.h"
#include "NetDataDefine.h"


class NetRecvBean;
class NetRecvThread {


public:
    NetRecvThread();
    ~NetRecvThread();
    void Init();
    void Start();
    void Stop();
    void Pause();//暂停
    void Resume();//继续
    void SetAlertException(bool b);
    int64_t GetLastRecvTime();// 获得最后一次接收数据的时间
    void ReleaseHandle();
    void RecvData();
    // 获得接收包的数据的进度
    float GetRecvPercent();
    int GetRecvPackageNum();
    static void* run(void *pthread);
private:
    void ReleasePackage(NetRecvBean *pnrb);
    void DealException(const SocketException &se);//处理异常
    bool RecvTimeOut();// 接收包超时
//    void GetDATA_RES_MESSAGE(byte **p, DATA_RES_MESSAGE *pData_Res_Message);

private:
    #pragma mark- 以下是接收命令
    // 处理接收到的包
    bool DealCommPackage(NetRecvBean *pnrb);
    bool Deal_CMD_REQ_HEARTBEAT_PACKET(NetRecvBean *pnrb);
    bool Deal_CMD_RES_MESSAGE(NetRecvBean *pnrb);
    //2.6.3.1.1	战斗请求成功后返回(CMD_RES_LOGIN)
    bool Deal_CMD_RES_LOGIN(NetRecvBean *pnrb);

    //2.4.6.3.1	校验更新用户基本信息成功时服务器返回更新后信息(CMD_RES_UPDATE_USERINFO) 	21
    bool Deal_CMD_RES_UPDATE_USERINFO(NetRecvBean *pnrb);

    //2.4.6.4.1	购买体力更新成功发送返回更新后信息(CMD_RES_BUY_STRENGTH) 	23
    bool Deal_CMD_RES_BUY_STRENGTH(NetRecvBean *pnrb);
    //2.4.6.7.1	酒馆解锁更新成功发送返回提示信息(CMD_RES_RECRUIT) 	29
    bool Deal_CMD_RES_RECRUIT(NetRecvBean *pnrb);
    //2.4.6.8.1	消耗宝石更新发送返回提示信息(CMD_RES_EXPEND) 	31
    bool Deal_CMD_RES_EXPEND(NetRecvBean *pnrb);
    //2.4.6.9.1	酒馆升级更新成功发送返回提示信息(CMD_RES_UPGRADE) 	33
    bool Deal_CMD_RES_UPGRADE(NetRecvBean *pnrb);
    //2.4.6.10.1	注册昵称成功发送返回提示信息(CMD_RES_REGISTER_USERNAME) 35
    bool Deal_CMD_RES_REGISTER_USERNAME(NetRecvBean *pnrb);
    //2.4.6.11.1	战斗结束信息更新成功后的提示信息(CMD_RES_AF_UPDATE_USERINFO) 	37
    bool Deal_CMD_RES_AF_UPDATE_USERINFO(NetRecvBean *pnrb);
    //2.4.6.12.1	签到后返回信息 (CMD_RES_SIGNIN) 39
    bool Deal_CMD_RES_SIGNIN(NetRecvBean *pnrb);
    //2.4.6.14.1	分享领奖成功后的提示信息(CMD_RES_APP_PODIUM) 	43
    bool Deal_CMD_RES_APP_PODIUM(NetRecvBean *pnrb);
    //2.4.6.15.1	摇奖返回的信息(CMD_RES_SHAKE) 	45
    bool Deal_CMD_RES_SHAKE(NetRecvBean *pnrb);
    //2.4.6.16.1	获取排行榜玩家返回的信息(CMD_RES_RANKING) 	47
    bool Deal_CMD_RES_RANKING(NetRecvBean *pnrb);
    //2.4.6.17.1	发起战斗返回的信息(CMD_RES_FIGHT) 	49
    bool Deal_CMD_RES_FIGHT(NetRecvBean *pnrb);
    //2.4.6.18	主动推送系统消息 (CMD_REQ_SYSTEM_MESSAGE) 50
    bool Deal_CMD_REQ_SYSTEM_MESSAGE(NetRecvBean *pnrb);
    ////2.4.6.19	战报消息推送 (CMD_REQ_FIGHT_REPORT) 51
    bool Deal_CMD_REQ_FIGHT_REPORT(NetRecvBean *pnrb);
    //2.4.6.20.1	世界BOSS战返回提示信息(CMD_RES_BOSS_FIGHT) 	53
    bool Deal_CMD_RES_BOSS_FIGHT(NetRecvBean *pnrb);
    //2.4.6.21.1	快速匹配玩家返回的信息(CMD_RES_FAST_MATCH) 	55
    bool Deal_CMD_RES_FAST_MATCH(NetRecvBean *pnrb);
    //2.4.6.21.1	购买宝石--首次请求发送返回更新后信息(CMD_RES_BUY_GEMSTONE) 	57
    bool Deal_CMD_RES_BUY_GEMSTONE(NetRecvBean *pnrb);
    //2.4.6.22.1	购买宝石--二次验证发送返回更新后信息(CMD_RES_VALIDATE_GEMSTONE) 	59
    bool Deal_CMD_RES_VALIDATE_GEMSTONE(NetRecvBean *pnrb);
    //2.4.6.22.1	关卡解锁更新成功发送返回提示信息(CMD_RES_COPY_RECRUIT) 	61
    bool Deal_CMD_RES_COPY_RECRUIT(NetRecvBean *pnrb);
    //2.4.6.22.1	世界boss战--boss信息返回提示信息(CMD_RES_BOSS_INFO) 	63
    bool Deal_CMD_RES_BOSS_INFO(NetRecvBean *pnrb);
    //2.4.6.23.1	世界boss战--排行榜返回提示信息(CMD_RES_BOSS_RANKING) 	65
    bool Deal_CMD_RES_BOSS_RANKING(NetRecvBean *pnrb);
    //2.4.6.24.1	世界boss战--奖励领取返回提示信息(CMD_RES_BOSS_REWARD) 	67
    bool Deal_CMD_RES_BOSS_REWARD(NetRecvBean *pnrb);
    //2.4.6.25	世界Boss战--领奖消息推送 (CMD_REQ_BOSS_REWARD_REPORT) 68
    bool Deal_CMD_REQ_BOSS_REWARD_REPORT(NetRecvBean *pnrb);//服务器主动推送
    //2.4.6.26.1	世界Boss战--战斗结束信息更新成功后的提示信息(CMD_RES_BOSS_UPDATE_FIGHT) 	70
    bool Deal_CMD_RES_BOSS_UPDATE_FIGHT(NetRecvBean *pnrb);
    //2.4.6.27	世界Boss战--战斗中战报消息推送 (CMD_REQ_BOSS_FIGHT_REPORT) 71
    bool Deal_CMD_REQ_BOSS_FIGHT_REPORT(NetRecvBean *pnrb);//服务器主动推送
    //2.4.6.28.1	获取分享内容返回的信息(CMD_RES_APP_SENDING_PODIUM) 	73
    bool Deal_CMD_RES_APP_SENDING_PODIUM(NetRecvBean *pnrb);
    //2.4.6.29.1	世界BOSS战--发起战斗返回的信息(CMD_RES_BOSS_BEGIN_FIGHT) 	75
    bool Deal_CMD_RES_BOSS_BEGIN_FIGHT(NetRecvBean *pnrb);



#pragma mark --接收注册命令

private:
    bool m_bAlertException;
    bool m_bStop;
    bool m_bPause;//暂停
    long long recvBeginTime; // 接收数据开始的时间， 用于判断超时
    int countTime;
    pthread_t m_pThead_recv;
    TCPClientSocket *m_pSocket;
    //long long m_nLastRecvTime;
    NetRecvBean *m_pCurrentRecvBean;
     //int netRecvPackagecount;


};

#endif
