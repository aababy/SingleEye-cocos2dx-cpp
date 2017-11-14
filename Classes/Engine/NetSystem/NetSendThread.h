//
//  NetSendThread.h
//  coslua
//
//  Created by chency on 13-6-9.
//
//

#ifndef coslua_NetSendThread_h
#define coslua_NetSendThread_h

#include "socketcc.h"
#include "cocos2d.h"
#include "EngineDefine.h"
#include "NetSendBean.h"

using namespace std;
USING_NS_CC;







class Mercemary;

class NetSendThread {
    

public:
    NetSendThread(string _ip, int _port, string _domain_name, int _domain_port);
    ~NetSendThread();
    void SetAddress(string _ip, int _port);//设置服务器地址
    void GetAddress(string &_ip, int &_port);//得到服务器地址
    void SetDomain(string _domainName, int _port);//设置服务器域名
    void Start();
    void Stop();
    void Pause();//暂停
    void Resume();//继续
    void SetAlertException(bool b);
    void CloseSocket();
    TCPClientSocket* GetSocket();
    void ReleaseHandle();
    static void* run(void *pthread);
    // 传递网络异常信息给发送类处理
    void TransferNetErrorMsg(const SocketException &se);
    void LogoutMsgSender(int cmd_index); // 注销命令
    void SendMsgToSender(int cmd_index, int cmd, void *pdata);
    bool hasMsgNeedAnswer();//有消息需要返回处理 发送了消息后该消息就成了需要接收服务器响应的消息。
    CCObject* GetMsgSender(int cmd_index);//获得网络命令发送者

    bool updateAddress;
    int netConnectcount;
    // 首次网络链接成功标志，用于判断是否切换域名链接。
    bool firstConnectOK;
    bool m_bRecvError; // 接收线程出错时设置为true；
public:
#pragma mark- 以下是发送命令
    // 以下是发送命令
    void DO_test_cmd(CCObject *pSender);

    //2.4.6.1.1	客户端发送心跳包消息CMD_REQ_HEARTBEAT_PACKET) 	0
    void DO_CMD_REQ_HEARTBEAT_PACKET(CCObject *pSender);


    //2.4.6.3	校验更新用户基本信息(CMD_REQ_UPDATE_USERINFO) 20
    void DO_CMD_REQ_UPDATE_USERINFO(CCObject *pSender);

    //2.4.6.4	购买体力 (CMD_REQ_BUY_STRENGTH) 22

    /**
     *	@brief	<#Description#>
     *
     *	@param 	pSender 	sender
     *	@param 	oldStrength 	客户端原来体力数
     *	@param 	strength 	购买体力数
     *	@param 	gemstone 	花费宝石数
     *	@param 	oldGemstone 	客户端宝石数
     */
    void DO_CMD_REQ_BUY_STRENGTH(CCObject *pSender, INT32 oldStrength, INT32 strength, INT32 gemstone, INT32 oldGemstone);
    //2.4.6.7	酒馆解锁(CMD_REQ_ RECRUIT) 28
    /**
     *	@brief	<#Description#>
     *
     *	@param 	pSender 	sender
     *	@param 	oldGemstone 	客户端宝石数
     *	@param 	upgradeType 	解锁类型（1 解锁英雄  2 解锁佣兵）
     *	@param 	heroOrMercemaryId 	解锁的英雄或佣兵ID
     *	@param 	grade 	解锁的英雄或佣兵等级
     */
    void DO_CMD_REQ_RECRUIT(CCObject *pSender, INT32 oldGemstone, INT16 upgradeType, INT32 heroOrMercemaryId, INT32 grade);
    //2.4.6.8	消耗宝石 (CMD_REQ_EXPEND) 30
    /**
     *	@brief	<#Description#>
     *
     *	@param 	pSender 	sender
     *	@param 	oldGemstone 	客户端宝石数
     *	@param 	gemstone 	花费宝石数
     */
    void DO_CMD_REQ_EXPEND(CCObject *pSender, INT32 oldGemstone, INT32 gemstone);
    //2.4.6.9	酒馆升级(CMD_REQ_UPGRADE) 32
    /**
     *	@brief	<#Description#>
     *
     *	@param 	pSender 	sender
     *	@param 	oldGemstone 	客户端宝石数
     *	@param 	upgradeType 	升级类型（1 升级英雄  2 升级佣兵）
     *	@param 	oldHeroOrMercemaryId 	原英雄（佣兵）ID
     *	@param 	oldGrade 	原英雄（佣兵）等级
     *	@param 	newHeroOrMercemaryId 	现英雄（佣兵）ID
     *	@param 	newGrade 	现英雄（佣兵）等级
     */
    void DO_CMD_REQ_UPGRADE(CCObject *pSender, INT32 oldGemstone, INT16 upgradeType, INT32 oldHeroOrMercemaryId, INT32 oldGrade, INT32 newHeroOrMercemaryId, INT32 newGrade);
    //2.4.6.10	玩家注册战斗昵称 (CMD_REQ_REGISTER_USERNAME) 34
    /**
     *	@brief	<#Description#>
     *
     *	@param 	pSender 	sender
     *	@param 	userName 	玩家昵称
     *	@param 	oldGemstone 	客户端宝石数
     */
    void DO_CMD_REQ_REGISTER_USERNAME(CCObject *pSender, string userName, INT32 oldGemstone);
    /**
     *	@brief	<#Description#>
     *
     *	@param 	pSender 	sender
     *	@param 	_isSuccess 	1 为功方胜利，0为功方失败
     *	@param oldGemstone 客户端宝石数(减去宝石消耗之后的宝石数)
     *	@param 	gemstone 	宝石奖励
     *  @param  gemstoneUsed	宝石消耗（-1表示新手引导
     *	@param 	defendUserId 	守方玩家Id(守方在胜利一定场次后可获得宝石奖励)
     *	@param 	score	Int32	客户端总积分
     *  @param  hp Int32 伤害值（只有世界boss战才有， PVP和副本中为0）
     */
    void DO_CMD_REQ_AF_UPDATE_USERINFO(CCObject *pSender, byte _isSuccess, INT32 oldGemstone, INT32 gemstone, INT32 gemstoneUsed, INT32 defendUserId, INT32 score, INT32 hp=0);
    //2.4.6.12	签到 (CMD_REQ_SIGNIN) 38
    /**
     *	@brief	<#Description#>
     *
     *	@param oldGemstone 客户端宝石数
     *	@param 	oldGemstone 	<#oldGemstone description#>
     */
    void DO_CMD_REQ_SIGNIN(CCObject *pSender, INT32 oldGemstone);
    //2.4.6.14	分享领奖信息更新 (CMD_REQ_APP_PODIUM)  42
    /**
     *	@brief	<#Description#>
     *
     *	@param 	pSender 	sender
     *	@param 	shareTimes 	任分享次数
     *	@param oldGemstone 客户端宝石数
     */
    void DO_CMD_REQ_APP_PODIUM(CCObject *pSender, INT32 shareTimes, INT32 oldGemstone);
    //2.4.6.15	摇奖 (CMD_REQ_SHAKE) 44
    /**
     *	@brief	<#Description#>
     *
     *	@param 	pSender 	sender
     *	@param 	fightModel 	战斗模式 0 PVP/ 1 PVE
     *	@param oldGemstone 客户端宝石数
     */
    void DO_CMD_REQ_SHAKE(CCObject *pSender, INT16 fightModel, INT32 oldGemstone);
    //2.4.6.16	获取排行榜玩家信息 (CMD_REQ_RANKING) 46
    /**
     *	@brief	<#Description#>
     *
     *	@param 	pSender 	sender
     *	@param oldGemstone 客户端宝石数
     */
    void DO_CMD_REQ_RANKING(CCObject *pSender, INT32 oldGemstone);
    //2.4.6.17	发起战斗 (CMD_REQ_FIGHT) 48
    /**
     *	@brief	<#Description#>
     *
     *	@param 	pSender 	sender
     *	@param 	rankingUserId 	战斗玩家Id
     *	@param 	isRevenge 	是否复仇（0 不是  1 是）
     *	@param oldGemstone 客户端宝石数
     *	@param 	_enemy_mercemaryNum 	当前佣兵数长度
     *	@param _enemy_listMercemary 当前玩家佣兵数量列表
     */
    void DO_CMD_REQ_FIGHT(CCObject *pSender, INT32 rankingUserId, byte isRevenge, INT32 oldGemstone, INT16 _enemy_mercemaryNum, Mercemary **_enemy_listMercemary);
    //2.4.6.20	世界BOSS战 (CMD_REQ_BOSS_FIGHT)  52
    /**
     *	@brief	<#Description#>
     *
     *	@param 	pSender 	sender
     *	@param 	killValue 	累计伤害值（5000~20000之间的值，为-1时表示杀死BOSS）
     *	@param oldGemstone 客户端宝石数
     */
    void DO_CMD_REQ_BOSS_FIGHT(CCObject *pSender, INT32 killValue, INT32 oldGemstone);
    //2.4.6.21	快速匹配玩家信息 (CMD_REQ_FAST_MATCH) 54
    /**
     *	@brief	<#Description#>
     *
     *	@param 	pSender 	sender
     *	@param oldGemstone 客户端宝石数
     */
    void DO_CMD_REQ_FAST_MATCH(CCObject *pSender, INT32 oldGemstone);
    //2.4.6.21	购买宝石--首次请求(CMD_REQ_BUY_GEMSTONE) 56
    /**
     *	@brief	<#Description#>
     *
     *	@param 	pSender 	sender
     *	@param 	productType 	产品类型
     *	@param oldGemstone 客户端宝石数
     */
    void DO_CMD_REQ_BUY_GEMSTONE(CCObject *pSender, byte productType, INT32 oldGemstone);
    //2.4.6.22	购买宝石--二次验证(CMD_REQ_VALIDATE_GEMSTONE) 58
    /**
     *	@brief	<#Description#>
     *
     *	@param 	pSender 	sender
     *	@param 	transactionReceipt 	交易收据
     *	@param oldGemstone 客户端宝石数
     */
    void DO_CMD_REQ_VALIDATE_GEMSTONE(CCObject *pSender, string& transactionReceipt, INT32 oldGemstone);
    //2.4.6.22	关卡解锁(CMD_REQ_COPY_RECRUIT) 60
    /**
     *	@brief	<#Description#>
     *
     *	@param 	pSender 	sender
     *  @param  upgradeType 解锁类型（1 解锁英雄  2 解锁佣兵）
     *	@param 	heroOrMercemaryId 	解锁的英雄或佣兵ID
     *	@param 	grade 	解锁的英雄或佣兵等级
     */
    void DO_CMD_REQ_COPY_RECRUIT(CCObject *pSender, INT16 upgradeType, INT32 heroOrMercemaryId, INT32 grade);
    //2.4.6.22	世界boss战--boss信息(CMD_REQ_BOSS_INFO) 62
    /**
     *	@brief	<#Description#>
     *
     *	@param 	pSender 	sender
     *	@param oldGemstone 客户端宝石数
     */
    void DO_CMD_REQ_BOSS_INFO(CCObject *pSender, INT32 oldGemstone);
    //2.4.6.23	世界boss战--排行榜请求(CMD_REQ_BOSS_RANKING) 64
    /**
     *	@brief	<#Description#>
     *
     *	@param 	pSender 	sender
     *	@param oldGemstone 客户端宝石数
     */
    void DO_CMD_REQ_BOSS_RANKING(CCObject *pSender, INT32 oldGemstone);
    //2.4.6.24	世界boss战--奖励领取(CMD_REQ_BOSS_REWARD) 66
    /**
     *	@brief	<#Description#>
     *
     *	@param 	pSender 	sender
     *	@param oldGemstone 客户端宝石数
     */
    void DO_CMD_REQ_BOSS_REWARD(CCObject *pSender, INT32 oldGemstone);
    //2.4.6.26	世界Boss战--战斗结束信息更新 (CMD_REQ_BOSS_UPDATE_FIGHT) 69
    /**
     *	@brief	<#Description#>
     *
     *	@param 	pSender 	sender
     *	@param 	isSuccess 	1 胜利，0失败
     *	@param 	oldGemstone 	客户端宝石数(减去宝石消耗之后的宝石数)
     *  @param  gemstone    宝石奖励
     *	@param 	gemstoneUsed 	宝石消耗
     *	@param 	hp 	伤害值
     */
    void DO_CMD_REQ_BOSS_UPDATE_FIGHT(CCObject *pSender, byte isSuccess, INT32 oldGemstone, INT32 gemstone, INT32 gemstoneUsed, INT32 hp);
    //2.4.6.28	获取分享内容 (CMD_REQ_APP_SENDING_PODIUM)  72
    /**
     *	@brief	<#Description#>
     *
     *	@param 	pSender 	sender
     *	@param 	shareAppId 	分享平台ID（预留占位，暂时不用）
     */
    void DO_CMD_REQ_APP_SENDING_PODIUM(CCObject *pSender, byte shareAppId);
    //2.4.6.29	世界BOSS战--发起战斗 (CMD_REQ_BOSS_BEGIN_FIGHT)  74
    void DO_CMD_REQ_BOSS_BEGIN_FIGHT(CCObject *pSender);


#pragma mark --注册相关  


private:
    bool Connect();
    // 发送一条网络命令
    void SendData(const NetSendBean &pnsb);
    // 创建消息并推送到消息队列。内部调用
    void CreateAndPushMsg(CCObject *pSender, byte *pMsg, int msgLen, int cmd, bool showNetInfo=true);
    // 清空发送队列
    void RemoveSendList();
    // 注册发送命令的回调处理类 由NetSendThread调用
    void RegisterMsgSender(int cmd_index, CCObject *pMsgSender);

    void ErrorToSentMsg(const SocketException &se);
    void ErrorToSendingMsg(const SocketException &se);
    void TransferNetErrorMsgToSender(CCObject* pSender, const SocketException &se);
    string getNetErrorDescription(const SocketException &se);
    
    byte CheckReackablility();
    byte reackablility;//网络链接状态 返回：0-不可用 1-wifi 2-3G网络
private:
    void testtest();
    string server_ip;
    int server_port;
    string server_domain_name;
    int server_domain_port;
    TCPClientSocket *m_pSocket;
    bool m_bSocketNeedInit;
    bool m_bAlertException;
    bool m_bStop;
    bool m_bPause;//暂停
    bool m_bPauseOld;
    pthread_t m_pThead_send;
    IPAddress address;
    int serverPort;
    std::set<NetSendBean> m_setSendList;
    // 网络命令-发送类（处理类）
    // 发送命令做记录，如果收到服务器回执，发送消息给对应的类处理 （发送命令序号，发送者）
    std::map<int, CCObject*> m_mapMsgSender;
    bool m_bShowNetInfo; // 本次联网是否显示联网请求

};

#endif
