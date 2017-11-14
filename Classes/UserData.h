//
//  UserData.h
//  coslua
//
//  Created by chency on 13-9-3.
//
//

#ifndef coslua_UserData_h
#define coslua_UserData_h

#include "EngineDefine.h"
#include <iostream>
#include "CFIO.h"

using namespace std;

// 以下定义引用NetDataDefine.h里的定义如有更新以NetDataDefine.h为准
#ifndef _String_def
#define _String_def
typedef struct _String_
{
	INT16 mlen;//字符串长度，字符串为空时，此处为0
	byte *pbytes;//字节流Bytes 字符串为空时，此处不存在
}String;//字符串
#endif
#ifndef _RES_STAGE_SWEEP_def
#define _RES_STAGE_SWEEP_def
typedef struct _DropItemInfo_
{
	byte    dropType;//掉落类型（1.金币 2.体力 3.魔晶石 ｜ 4.道具 5.卡牌 6.装备 7.解锁女神卡）
	INT32	dropItemId;//掉落道具ID
    String  dropItemName;//掉落道具名称
    INT32   dropItemIdNums;//掉落数量
    String  dropItemNote;//掉落的道具说明
}DropItemInfo;//关卡掉落物品信息

typedef struct _RES_STAGE_SWEEP_
{
    INT16 gamePoint;//	Int32	获得经验
    INT16 userGameLevel;//	//Int16	玩家当前等级
    INT32 dropGoldCoin;//	//Int32	金币掉落
    INT16 dropItemCount;//	//Int16	道具掉落数
    DropItemInfo *pDropItemInfo;//    道具掉落信息列表
}RES_STAGE_SWEEP;
#endif
#define TRANSACTION_COUNT "transactionCount" //交易成功需要认证的记录条数
#define TRANSACTION_RECEIPT "transactionReceipt" //	交易成功回执信息

// 删除数组 void ** array, int len
#define DEL_ARRAY(array, len) {\
    if (len >=0 && array) {\
        for (int i = 0; i < len; i++) {\
            ENGINE_SAFE_DEL(array[i]);\
        }\
        delete array;\
        array = NULL;\
    }\
}
#define MAX_FIGHT_COUNT 50 //最大战报数
// 登陆信息
class LoginInfo
{
//public:
//    LoginInfo(){
//        pUserInfo = NULL;
//        pSoul_Point_Exchange = NULL;
//        pMoney_Exchange_RES = NULL;
//        configList = NULL;
//        isDisplayCheckIn = true;
//        displayCheckIn = true;//默认显示签到
//        newMissionNoteListCount = 0;
//        newMissionNoteList = NULL;
//    }
//    virtual ~LoginInfo(){
//        ENGINE_SAFE_DEL(pUserInfo);
//        ENGINE_SAFE_DEL(pSoul_Point_Exchange);
//        ENGINE_SAFE_DEL(pMoney_Exchange_RES);
//        ENGINE_SAFE_DELPOOL(configList);
//        ENGINE_SAFE_DEL(newMissionNoteList);
//    };
//    // 设置显示签到标志
//    void SetDisplayCheckIn(bool b){ displayCheckIn = b;}
//    // 得到显示签到标志
//    bool GetDisplayCheckIn(){return isDisplayCheckIn && displayCheckIn;}
//    INT32       sessionID;//	用法参考MessageHeader
//    byte        nextStep;//     客户端下一步操作
//    byte        forceFlag;//	强制标志位
//    string      clientVer;// 	String	客户端程序版本号
//    CPlayerInfo  *pUserInfo;//     PlayerInfo	用户信息
//    Soul_Point_Exchange	*pSoul_Point_Exchange;//灵魂碎片兑换配置
//    Money_Exchange_RES	*pMoney_Exchange_RES;//魔晶石与各种资源的兑换配置
//    INT16       configNum;//	Int16	配置项数
//    CKeyValue*    configList;//	KeyValue[configNum]	配置项清单
//    byte isDisplayCheckIn;//	byte	是否显示签到任务 0：不显示 1:显示
//    bool displayCheckIn;//显示签到界面 true显示
//    INT16 newMissionNoteListCount;//	Int16	新任务提示条数
//    string *newMissionNoteList;//	String[newMissionNoteListCount]	新任务提示列表


};


#pragma mark --存储数据管理
//存储数据结构
#define DEFINED_SAVEDATA_NAME   ("savedata.bin")

class SaveData
{
public:
    INT32   userId; //用户ID

};


//存储数据管理类
class SaveDataManage
{
public:
    SaveDataManage():m_pSaveData(NULL)
    {ReadData();};//初始化存储数据
    ~SaveDataManage()
    {
        ENGINE_SAFE_DEL(m_pSaveData);
    }
    void ReadData()//读取数据
    {
        if(!m_pSaveData)
        {
             m_pSaveData = new SaveData();
        }
        if(m_pSaveData)
        {
            ClearData();
            CFIO::FileRead(DEFINED_SAVEDATA_NAME,(byte *)m_pSaveData,sizeof(SaveData));

        }
    };
    void WriteData()//写入数据
    {
        if(!m_pSaveData)
            return;
        CFIO::FileWrite(DEFINED_SAVEDATA_NAME,(byte *)m_pSaveData,sizeof(SaveData));
    };
    void ClearData()//清空数据
    {
        memset(m_pSaveData,0x00,sizeof(SaveData));
    }
    SaveData *m_pSaveData;
};
//
//2.3.8	技能(Skill)
class Skill{
public:
    INT32 skillId;//	Int32	技能Id
    INT32 skillHurt;//	Int32	技能伤害

};
//2.3.7	道具属性(Prop)
//class Prop{
//public:
//    INT32 propId;//	Int32	道具Id
//    INT32 goldPrice;//	Int32	价格（金币）
//    INT32 gemstonePrice;//	Int32	价格（宝石）
//
//};
//2.3.6	佣兵属性(Mercemary)
class Mercemary{
public:
    INT32 mercemaryId;//	Int32	佣兵Id
    INT32 grade;//	Int32	等级

};
//2.3.5	英雄属性(Hero)
class HeroInfo{
public:
    INT32 heroId;//	Int32	英雄Id
    INT32 grade;//	Int32	等级

};
//用户信息
class UserInfo{
public:
    UserInfo(){
        userId = 0;
        userName = "";
        goldCoin = 0;
        strength = 0;

        heroNum = 0;
        listHero = NULL;
        mercemaryNum = 0;
        listMercemary = NULL;



    }
    ~UserInfo(){
        DEL_ARRAY(listHero, heroNum);
        DEL_ARRAY(listMercemary, mercemaryNum);

    }
public:
    INT32 userId;//	Int32	用户Id
    string userName;//	String	用户昵称
    INT32 goldCoin;//	Int32	金币数量（占位，可以不用传递）
    INT32 gemstone;//	Int32	宝石数量
    INT32 strength;//	Int32	体力数量
    INT16 heroNum;//	Int16	英雄数长度
    HeroInfo **listHero;//	Hero[heroNum]玩家英雄数量列表
    INT16 mercemaryNum;//	Int16	佣兵数长度
    Mercemary **listMercemary;//	Mercemary[mercemaryNum]	玩家佣兵数量列表
    byte isSignIn;//	byte	当天是否签到 (1 签到  0 未签到)
    INT32 signInNum;//	Int32	已经连续签到数
    INT32 shareTimes;//	Int32	分享次数

};
//2.3.15	战斗玩家信息(RankingUser)
class RankingUser{
public:
    INT32 userId;//	Int32	玩家ID
    string userName;//	String	玩家名称
    INT32 rankingStatus;//	Int32	是否正在战斗（1 正在战斗 0 空闲）
    INT32 score;//	Int32;//	玩家积分(修改为排名，字段名不修改)

};
//2.3.17	世界Boss战--排行榜(BossFightRanking)
class BossFightRanking{
public:
    INT32 number;//	Int32	排名
    INT32 userId;//	Int32	玩家ID
    string userName;//	String	玩家名称
    INT32 hp;//	Int32	伤害值

};
#pragma mark --UserData
class UserData
{
public:
    // 单例设计
    static UserData* Instance();
    UserData(){
        sessionID = 0;
        pSDManage = NULL;
////////////////////////////////////////////////////
        pUserInfo = NULL;

        rankingListCount = 0;
        rankingList = NULL;
        enemy_mercemaryNum = 0;
        enemy_listMercemary = NULL;
        rankingCount = 0;
        rankingInfoList = NULL;
    };
    virtual ~UserData(){
        DEL_ARRAY(rankingList , rankingListCount);
        DEL_ARRAY(enemy_listMercemary , enemy_mercemaryNum);
        ENGINE_SAFE_DEL(pSDManage);
        ENGINE_SAFE_DEL(pUserInfo);
        DEL_ARRAY(rankingInfoList , rankingCount);
    };
    string GetCardgameLibVersion();// 获得cardgameLib.a 文件版本号
    void init(int _userID);
    int addTransaction(string &transactionReceipt);//加入需要验证的交易记录
    int delTransaction();//删除交易记录
    void SaveUserInfo(UserInfo *_pUserInfo);//保存用户信息
    UserInfo* GetSavedUserInfo();//得到已经保存的用户信息（从本地磁盘中获得）
    string GetConfigValue(string key);// 获得配置项值

    void Parse_RankingList(byte **p);//解析排行榜，快速匹配共用
    void Parse_RES_FIGHT(byte **p);//解析发起战斗信息
    void Parse_CMD_RES_LOGIN(byte **p, LoginInfo *pLoginInfo);// 解析登陆信息

    void Parse_UserInfo(byte **p, UserInfo  *_pUserInfo);//解析用户信息
    void Parse_BossFightRankingList(byte **p);//解析世界boss战排行榜信息列表

private:
    
    
public:
    INT16   protocolVer;// 	Int16	通讯协议版本：1
    INT32       sessionID;//	用法参考MessageHeader
    INT32   deviceId;//	Int32	设备id（保留字段暂不用,可直接传0）
    byte    platformId;//	Byte	手机开发平台 id：1:ios 2:Andorid
    string  mac;//	String	设备的mac地址 （必传）
    string  IMSI;//	String	传递实际的SIM卡号，手机无法获取则传“0”.
    string  clientVer;// 	String	客户端程序版本号
    byte    dialup;// 	Byte 	客户端拨号方式：
    //0－wifi
    //1－3G
    //2 -2G
    string  channelId;//	String	渠道id (没有时填0)
    string  loginName;//	String	登录名(保留)
    string  loginPasswd;//	String	登录密码(保留)

    int     transactionCount;//交易成功需要认证的记录条数
    string  transactionReceiptArray[20];//	交易成功回执信息

    //数据管理
    SaveDataManage *pSDManage;//数据管理信息

    UserInfo *pUserInfo;

    byte isSuccess; //	byte	0:失败1:成功
    string message;//	String	失败的提示信息

    INT16 upgradeType;//	Int16	升级类型（1 升级英雄  2 升级佣兵）
    INT32 heroOrMercemaryId;//	Int32	解锁的英雄或佣兵ID
    INT32 grade;//	Int32	解锁的英雄或佣兵等级

    INT32 oldHeroOrMercemaryId;//	Int32	原英雄（佣兵）ID
    INT32 oldGrade;//	Int32	原英雄（佣兵）等级
    INT32 newHeroOrMercemaryId;//	Int32	现英雄（佣兵）ID
    INT32 newGrade;//	Int32	现英雄（佣兵）等级

    INT32 score;//	Int32	奖励积分（功方失败积分为0）
    INT32 totalScore;//	Int32	总积分

    //排行榜使用
    //快速匹配玩家使用
    INT16 rankingListCount;//	Int16	玩家数量
    RankingUser **rankingList;//	RankingInfo[ rankingListCount]	玩家信息列表

    //战斗中敌人数据
    INT32 enemy_userId;//	Int32	用户Id
    string enemy_userName;//	String	用户昵称
    INT16 enemy_mercemaryNum;//	Int16	佣兵数长度
    Mercemary **enemy_listMercemary;//	Mercemary[mercemaryNum]	玩家佣兵数量列表

    //摇奖数据
    INT32 shake_goldCoin;//	Int32	金币数量
    INT32 shake_gemstone;//	Int32	宝石数量
    byte shake_returnType;//	Byte	类型（0 无1 英雄  reGrade为1 2 佣兵  reGrade为1 3道具 reid为道具ID， reGrade为0）
    INT32 shake_reId;//	Int32	ID
    INT32 shake_reGrade;//	Int32	等级

    //服务器主动推送的消息
    INT32 configId;//	Int32	活动Id(为0 表示消息提示)
    string content_action;//	String	活动内容

    // 2.4.6.19	战报消息
    INT32 attackUserId;//	Int32	攻击方userid
    string content_fighting;//	String	战报消息内容

    //2.4.6.22.1	世界boss战
    INT16 bossId;//	Int16	BossId
    INT32 allHp;//	Int32	总血量
    INT32 leftHp;//	Int32	剩余血量
    INT16 fightCount;//	Int16	战报数量
    string fightInfoList[MAX_FIGHT_COUNT];//	战报信息列表（xxx消耗boss hp 250点）
    INT16 rankingCount;//	Int16	排行榜数量
    BossFightRanking **rankingInfoList;//	BossFightRanking[rankingCount]	排行榜信息列表
    INT32 todayMyRanking;//	Int32	当前玩家当天排名（为0 表示无名次或者未参加战斗）
    INT32 yestodayMyRanking;//	Int32	当前玩家昨天排名（为0 表示无名次或者未参加战斗）
    byte isReward;//	Byte	是否能领取奖品（0未参加战斗，无奖品可领取，1 有奖品）
    INT32 addGemstone;//	Int32	奖励的宝石数
    INT32 bombNum;//	Int32	屌丝炸弹数量
    INT32 bomberNum;//	Int32	土豪轰炸机数量
    INT32 gold;//	Int32	奖励的金币数量

    string bossFightRewardStr;//世界Boss战--领奖消息推送 消息内容（格式：您今天参加世界boss战获得的奖励还未领取）只限当天，隔天不能领取
    string bossFightStr;//世界Boss战--战斗中战报消息推送 消息内容（格式：xxx在boss战中壮烈牺牲 xxx在boss战中获胜）

    byte isBegin;//是否可以发起战斗  1  正在活动中 0  活动还未开始 -1  活动已经结束








    
};

#endif
