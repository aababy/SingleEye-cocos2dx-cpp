//
//  GameNetDef.h
//  coslua
//
//  Created by chency on 13-6-18.
//
//

#ifndef coslua_CardGameNetDef_h
#define coslua_CardGameNetDef_h

//#define NET_SEND_THREAD_DELAY 200000           // 微妙级 200毫秒
#define NET_SEND_THREAD_DELAY 20000           // 微妙级 20毫秒
//#define NET_RECV_THREAD_DELAY 200000           // 微妙级 200毫秒
#define NET_RECV_THREAD_DELAY 20000           // 微妙级 20毫秒
#define NET_SEND_TIMEOUT 3              // 秒
#define NET_RECV_TIMEOUT_S_HEAD 0            // 秒 读协议头时的超时秒
#define NET_RECV_TIMEOUT_S_BODY 15            // 秒 读协议体时的超时秒 读协议体需要更长的超时时间
//#define NET_RECV_TIMEOUT_US 200000      // 微妙 接收超时的微妙 超时=超时秒+超时毫秒
#define NET_RECV_TIMEOUT_US 20000      // 微妙(20毫秒) 接收超时的微妙 超时=超时秒+超时毫秒
#define NET_MSG_DELAY (1) // 网络消息延迟毫秒数。网络发送消息没有和cocos2d-x的消息同步，有可能会导致刷新的时候更改界面元素引起死机,使用网络消息时需要发送延迟类型的消息。
#define NET_OFF_LINE 100 //判断离线的时间（秒）。超过这个时间没有正常收发一次命令视为离线，在下次网络请求前需要先发送登录请求。 
#define NET_PING_INTERVAL 60000          // 毫秒
typedef enum
{
    CMD_REQ_HEARTBEAT_PACKET    =0,     //	心跳包
    CMD_REQ_LOGIN               =1,     //	客户端发送登录消息
    CMD_RES_LOGIN               =2,     //	服务器发送登录成功消息
    CMD_REQ_REG_USER_NAME       =5,     //2.6.4.1	客户端发送用户名和女神选择(CMD_REQ_REG_USER_NAME) 5
    CMD_RES_REG_USER_NAME       =6,     //2.6.4.1.1	返回用户信息(CMD_RES_REG_USER_NAME)  6
    CMD_RES_MESSAGE             =10,    //	服务发送提示消息



    //装备相关
    CMD_REQ_UPDATE_USERINFO         =20,    //2.4.6.3	校验更新用户基本信息(CMD_REQ_UPDATE_USERINFO) 20
    CMD_RES_UPDATE_USERINFO         =21,    //2.4.6.3.1	校验更新用户基本信息成功时服务器返回更新后信息(CMD_RES_UPDATE_USERINFO) 	21
    CMD_REQ_BUY_STRENGTH         =22,    //2.4.6.4	购买体力 (CMD_REQ_BUY_STRENGTH) 22
    CMD_RES_BUY_STRENGTH         =23,    //2.4.6.4.1	购买体力更新成功发送返回更新后信息(CMD_RES__BUY_STRENGTH) 	23

    CMD_REQ_RECRUIT         =28,    //2.4.6.7	酒馆解锁(CMD_REQ_RECRUIT) 28
    CMD_RES_RECRUIT         =29,    //2.4.6.7.1	酒馆解锁更新成功发送返回提示信息(CMD_RES_RECRUIT) 	29

    CMD_REQ_EXPEND         =30,    //2.4.6.8	消耗宝石 (CMD_REQ_EXPEND) 30
    CMD_RES_EXPEND         =31,    //2.4.6.8.1	消耗宝石更新发送返回提示信息(CMD_RES_EXPEND) 	31

    CMD_REQ_UPGRADE         =32,    //2.4.6.9	酒馆升级(CMD_REQ_UPGRADE) 32
    CMD_RES_UPGRADE         =33,    //2.4.6.9.1	酒馆升级更新成功发送返回提示信息(CMD_RES_UPGRADE) 	33

    CMD_REQ_REGISTER_USERNAME         =34,    //2.4.6.10	玩家注册战斗昵称 (CMD_REQ_REGISTER_USERNAME) 34
    CMD_RES_REGISTER_USERNAME         =35,    //2.4.6.10.1	注册昵称成功发送返回提示信息(CMD_RES_REGISTER_USERNAME) 35

    CMD_REQ_AF_UPDATE_USERINFO         =36,    //2.4.6.11	战斗结束信息更新 (CMD_REQ_AF_UPDATE_USERINFO) 36
    CMD_RES_AF_UPDATE_USERINFO         =37,    //2.4.6.11.1	战斗结束信息更新成功后的提示信息(CMD_RES_AF_UPDATE_USERINFO) 	37

    CMD_REQ_SIGNIN         =38,    //2.4.6.12	签到 (CMD_REQ_SIGNIN) 38
    CMD_RES_SIGNIN         =39,    //2.4.6.12.1	签到后返回信息 (CMD_RES_SIGNIN) 39

    CMD_REQ_APP_PODIUM         =42,    //2.4.6.14	分享领奖信息更新 (CMD_REQ_APP_PODIUM)  42
    CMD_RES_APP_PODIUM         =43,    //2.4.6.14.1	分享领奖成功后的提示信息(CMD_RES_APP_PODIUM) 	43

    CMD_REQ_SHAKE         =44,    //2.4.6.15	摇奖 (CMD_REQ_SHAKE) 44
    CMD_RES_SHAKE         =45,    //2.4.6.15.1	摇奖返回的信息(CMD_RES_SHAKE) 	45

    CMD_REQ_RANKING         =46,    //2.4.6.16	获取排行榜玩家信息 (CMD_REQ_RANKING) 46
    CMD_RES_RANKING         =47,    //2.4.6.16.1	获取排行榜玩家返回的信息(CMD_RES_RANKING) 	47

    CMD_REQ_FIGHT         =48,    //2.4.6.17	发起战斗 (CMD_REQ_FIGHT) 48
    CMD_RES_FIGHT         =49,    //2.4.6.17.1	发起战斗返回的信息(CMD_RES_FIGHT) 	49

    CMD_REQ_SYSTEM_MESSAGE         =50,    //2.4.6.18	主动推送系统消息 (CMD_REQ_SYSTEM_MESSAGE) 50
    CMD_REQ_FIGHT_REPORT = 51,              //2.4.6.19	战报消息推送 (CMD_REQ_FIGHT_REPORT) 51

    CMD_REQ_BOSS_FIGHT         =52,    //2.4.6.20	世界BOSS战 (CMD_REQ_BOSS_FIGHT)  52
    CMD_RES_BOSS_FIGHT = 53,              //2.4.6.20.1	世界BOSS战返回提示信息(CMD_RES_BOSS_FIGHT) 	53
    CMD_REQ_FAST_MATCH         =54,    //2.4.6.21	快速匹配玩家信息 (CMD_REQ_FAST_MATCH) 54
    CMD_RES_FAST_MATCH = 55,              //2.4.6.21.1	快速匹配玩家返回的信息(CMD_RES_FAST_MATCH) 	55

    CMD_REQ_BUY_GEMSTONE         =56,    //2.4.6.21	购买宝石--首次请求(CMD_REQ_BUY_GEMSTONE) 56
    CMD_RES_BUY_GEMSTONE = 57,              //2.4.6.21.1	购买宝石--首次请求发送返回更新后信息(CMD_RES_BUY_GEMSTONE) 	57
    CMD_REQ_VALIDATE_GEMSTONE         =58,    //2.4.6.22	购买宝石--二次验证(CMD_REQ_VALIDATE_GEMSTONE) 58
    CMD_RES_VALIDATE_GEMSTONE = 59,              //2.4.6.22.1	购买宝石--二次验证发送返回更新后信息(CMD_RES_VALIDATE_GEMSTONE) 	59
    CMD_REQ_COPY_RECRUIT         =60,    //2.4.6.22	关卡解锁(CMD_REQ_COPY_RECRUIT) 60
    CMD_RES_COPY_RECRUIT = 61,   //2.4.6.22.1	关卡解锁更新成功发送返回提示信息(CMD_RES_COPY_RECRUIT) 	61

    CMD_REQ_BOSS_INFO         =62,    //2.4.6.22	世界boss战--boss信息(CMD_REQ_BOSS_INFO) 62
    CMD_RES_BOSS_INFO = 63,   //2.4.6.22.1	世界boss战--boss信息返回提示信息(CMD_RES_BOSS_INFO) 	63
    CMD_REQ_BOSS_RANKING         =64,    //2.4.6.23	世界boss战--排行榜请求(CMD_REQ_BOSS_RANKING) 64
    CMD_RES_BOSS_RANKING = 65,   //2.4.6.23.1	世界boss战--排行榜返回提示信息(CMD_RES_BOSS_RANKING) 	65
    CMD_REQ_BOSS_REWARD         =66,    //2.4.6.24	世界boss战--奖励领取(CMD_REQ_BOSS_REWARD) 66
    CMD_RES_BOSS_REWARD = 67,   //2.4.6.24.1	世界boss战--奖励领取返回提示信息(CMD_RES_BOSS_REWARD) 	67
    CMD_REQ_BOSS_REWARD_REPORT         =68,    //2.4.6.25	世界Boss战--领奖消息推送 (CMD_REQ_BOSS_REWARD_REPORT) 68
    CMD_REQ_BOSS_UPDATE_FIGHT = 69,   //2.4.6.26	世界Boss战--战斗结束信息更新 (CMD_REQ_BOSS_UPDATE_FIGHT) 69
    CMD_RES_BOSS_UPDATE_FIGHT         =70,    //2.4.6.26.1	世界Boss战--战斗结束信息更新成功后的提示信息(CMD_RES_BOSS_UPDATE_FIGHT) 	70
    CMD_REQ_BOSS_FIGHT_REPORT = 71,   //2.4.6.27	世界Boss战--战斗中战报消息推送 (CMD_REQ_BOSS_FIGHT_REPORT) 71

    CMD_REQ_APP_SENDING_PODIUM =72,    //2.4.6.28	获取分享内容 (CMD_REQ_APP_SENDING_PODIUM)  72
    CMD_RES_APP_SENDING_PODIUM = 73,   //2.4.6.28.1	获取分享内容返回的信息(CMD_RES_APP_SENDING_PODIUM) 	73

    CMD_REQ_BOSS_BEGIN_FIGHT =74,    //2.4.6.29	世界BOSS战--发起战斗 (CMD_REQ_BOSS_BEGIN_FIGHT)  74
    CMD_RES_BOSS_BEGIN_FIGHT = 75,   //2.4.6.29.1	世界BOSS战--发起战斗返回的信息(CMD_RES_BOSS_BEGIN_FIGHT) 	75



    
    
}NET_CMD;


#endif
