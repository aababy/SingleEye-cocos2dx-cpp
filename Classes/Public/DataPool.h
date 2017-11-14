//
//  DataPool.h
//  SingleEye
//
//  Created by wangwx on 13-12-5.
//
//
//常驻内存备查数据
//原型模式mode也可以放这里

#ifndef __SingleEye__DataPool__
#define __SingleEye__DataPool__

#include "GameInclude.h"

#include <vector>
#include <map>

USING_NS_CC;

#define DATAPOOL    (CDataPool::sharedDataPool())
#define xData       (CDataPool::sharedDataPool())
#define Device_Height_IP5 (1136.0)

#define xUGBool(key)            CCUserDefault::sharedUserDefault()->getBoolForKey(key)
#define xUSBool(key, value)     CCUserDefault::sharedUserDefault()->setBoolForKey(key, value)

#define xUGInt(key)             CCUserDefault::sharedUserDefault()->getIntegerForKey(key)
#define xUSInt(key, value)      CCUserDefault::sharedUserDefault()->setIntegerForKey(key, value)

#define xUGString(key)          CCUserDefault::sharedUserDefault()->getStringForKey(key)
#define xUSString(key, value)   CCUserDefault::sharedUserDefault()->setStringForKey(key, value)

#define CC_SYN_SAVERCD(varType, varName, funName)\
public: virtual varType get##funName(void) const { return varName; }\
public: virtual void set##funName(varType var){ varName = var; }

struct GMessage
{
    int     iType;          //0 系统; 1 战报
    int     iData;
    int     iLen;        //不包括空字符
    char    *msg;
    bool    bNew;        //是否是新消息.
};


struct GSign
{
    bool        bSign;
    int         iDay;
    Prop_Type   type;           //奖品类型
    
};

//校验备份数据，防止内存修改器修改内存值
//可增加其他敏感数据
class CCheckData
{
public:
    INT32   gold;   //金币
    INT32   gem;    //宝石
};
/*
//存档数据
class CSaveRecord
{
public:
    bool firsttime;
    int gold;
    int gemstone;
    int strength;
    float energy;
    
    int m_iShareCounts;
    int m_iContinueSign;
    int m_iTotalScore;
    
    bool m_bRookieGift;
    bool m_bOpenBackMusic;
    bool m_bOpenSoundEffect;
    
    int bestTime;
    bool m_bCompleteTeachLevelOne;
    bool m_bCompleteTeachLevelTwo;
    
    int currentChapter;                     //当前副本索引 0起计数
    int m_nCurrentLevel[MAX_CHAPTER_NUM];   //当前关卡索引 0起计数
    int m_nMaxUnlockChapter;                //最大解锁副本索引 0起计数
    int m_nMaxUnlockLevel[MAX_CHAPTER_NUM]; //最大解锁关卡索引 0起计数
    
    LevelRecord m_LevelRecord[MAX_CHAPTER_NUM*LEVEL_NUM_OF_CHAPTER];
    HeroRecord heroRcd[11];//todo 数量改成宏
    EnemyRecord petRcd[7];
    OutfitRecord outfitRcd[11];
    
    int m_nPlanHero[PLAN_HERO_NUM];
    int m_nPlanOutfit[PLAN_OUTFIT_NUM];
    int m_nPlanPet_1[PLAN_PET_NUM];
    int m_nPlanPet_2[PLAN_PET_NUM];
    
    int m_bTeachStepOver[TEACH_STEP_LIMIT];
};
*/
class CDataPool : public CCObject
{
public:
    static CDataPool *sharedDataPool(void);
    static void purgeDataPool(void);

private:
    CDataPool(void);
    ~CDataPool(void);
    
    static CDataPool *m_pInstance;
    
public:
    void init();
    
    void loadArchive();
    void saveArchive();
    void manualArchive();
    void checkData();
    inline void InitCheckData();
    inline void FreeCheckData();
    inline void ReadCheckData();
    inline void WriteCheckData();
    inline void ClearCheckData();
//    void InitSaveRecord();
//    void FreeSaveRecord();
    void ReadSaveRecord();
    void WriteSaveRecord();
    void ClearSaveRecord();
    void DebugSaveRecord();

    int getLevelScoreStar(int nChapter, int nLevel);
    int judgeLevelScoreStar(int nScore);
    
    bool findHeroUnlock(int nId);
    bool findPetUnlock(int nId);
    int findHeroLevel(int nId);
    int findPetLevel(int nId);

    void unlockPlanHero(int index);
    void unlockPlanOutfit(int index);
    void unlockPlanPet(int index, int sub);
    int getPlanHero(int index);
    int getPlanOutfit(int index);
    int getPlanPet(int index, int sub);
    void checkPlanOutfit();
    void setPlanHero(int index, int value);
    void setPlanOutfit(int index, int value);
    void setPlanPet(int index, int value, int sub);
    void unsetPlanHero(int value);
    void unsetPlanOutfit(int value);
    void unsetPlanPet(int value);
    //是否已出战
    bool isHeroBeenPlan(int nId);
    bool isOutfitBeenPlan(int nId);
    bool isPetBeenPlan(int nId);
    
    void loadOutfitData();
    void unloadOutfitData();//TODO 调用
    COutfitData *getOutfitData(int nID);
    
    void loadEnemyData();
    void unloadEnemyData();
    Enemy_Data *getEnemyData(int nID);
    
    void loadHeroData();
    void unloadHeroData();
    Game_Hero_Data *getHeroData(int nID);
    
    void initEnemyRecord();
    void freeEnemyRecord();
    EnemyRecord *getEnemyRecord(int nID);
    void setEnemyRecord(int nID, EnemyRecord & rcd);
    
    void initHeroRecord();
    void freeHeroRecord();
    HeroRecord *getHeroRecord(int nID);
    void setHeroRecord(int nID, HeroRecord & rcd);
    
    void initOutfitRecord();
    void freeOutfitRecord();
    OutfitRecord *getOutfitRecord(int nID);
    void setOutfitRecord(int nID, OutfitRecord & rcd);
    
    void initLevelRecord();
    void freeLevelRecord();
    LevelRecord *getLevelRecord(int nChapter, int nLevel);
    void setLevelRecord(int nChapter, int nLevel, LevelRecord &rcd);
    void freshLevelScore(int nChapter, int nLevel, int nScore);
    
    Prop_Data* getPropData(int nID);
    
    int findLockHero(int level); //通过等级查找该关卡的解锁英雄
    
    bool isValidOutfitId(int id);
    int findBagOutfitNum(int outfitId);
    void changeBagOutfitNum(int outfitId, int nValue);

    bool changeBagGoldNum(int nValue);    
    bool changeBagGoldNumForGame(int nValue);
    
    bool changeBagGemNum(int nValue);
    bool changeBagGemNumForGame(int nValue);
    void changePower(int nValue);
    
    bool isHaveTieDanGong();
    int getZhanLanFromBag();
    
    void addEffectCache(const char *name);//  加载特效文件
    void removeEffectCache(const char *name);//  移除特效文件
    
    int getEnemyAwardID();   //杀死怪后 奖励返回结果
    
    void playedLevel(int nChapter,int level);  //游戏胜利 传入副本和关卡数 判断是否解锁下一副本或关卡
    
    bool handleMsg(const Message &msg);
    void readFileForVector(vector <GMessage> &vec, const char * name);
    void writeFileForVector(vector <GMessage> &vec, const char * name);
    void saveAndFreeMessage();
    void checkMessage(float dt);
    int getNewMessageCount();
    void makeAllMessageToOld();
    void playGameBgSound(const char *name,bool bloop);
    int playGameEffect(const char *name);
    
    void readFileForSign(vector <GSign> &vec);
    void writeFileForSign(vector <GSign> &vec);
    
    void changeShareCounts(int iChange);
    void setAccpetSharePrize(int iLevel);
    void getAccpetSharePrize(bool bAcceptPrize[]);
    string getDefaultShareTxt();
    bool checkSign();
    
    bool isPet(int id);
    bool isUseGoldToTrain(int trainGold, int level);
    void playKeytone();
    void tryPlayBgMuisc(EBackMusicType type, bool bForceChange = false);
    
    void loadFightSceneResource();
    void removeFightSceneResource();
    void loadResource();
    void removeResource();
    
    bool fightTutorialsEnd(); //打斗中的教程已全部结束
    bool fubenTutorialsEnd();//副本介绍的展示已全部结束
    bool gameTutorialsEnd();  //打斗外的教程已全部结束
    
    int getHeroReference();
    void resetHeroReference();
    //存档数据读取函数
    CC_SYN_SAVERCD(bool, firsttime, FirstTime);
    CC_SYN_SAVERCD(int, gold, GoldCoin);
    CC_SYN_SAVERCD(int, m_iTotalScore, TotalScore);
    
    bool isUnlockDefense2();
    void CBLackOfGem(int id, int iTag);
    void CBLackOfGold(int id, int iTag);
    
    void waitSyncPetData();
public:
    bool firsttime;
    int m_DesVer;//密钥版本
    int gold;
    int m_iTotalScore;
    bool m_bGetBossPrize;
    float energy;
    
    CC_SYNTHESIZE(int, m_iShareCounts, ShareCounts);
    CC_SYNTHESIZE(int, m_iContinueSign, ContinueSign);
    CC_SYNTHESIZE(bool, m_bRequest, Request);
    CC_SYNTHESIZE(int, m_iAttackAdd, AttackAdd);
    CC_SYNTHESIZE(int, m_iDefenseAdd, DefenseAdd);    
    CC_SYNTHESIZE(int, m_iPvPId, PvPId);
    CC_SYNTHESIZE(bool, m_bPingLunYet, PingLunRcd);
    
    bool m_bIsShowPingLunYet;
    
    bool m_bRookieGift; //新手礼包
    
    bool m_bOpenBackMusic;      //打开背景音乐
    bool m_bOpenSoundEffect;    //打开音效
    
    int bestTime;  //挑战模式最长时间
    
    float zoom;  //屏幕适配放大比例
    
    //副本和关卡
    int currentChapter;                     //当前副本索引 0起计数
    int m_nCurrentLevel[MAX_CHAPTER_NUM];   //当前关卡索引 0起计数
    int m_nMaxUnlockChapter;                //最大解锁副本索引 0起计数
    int m_nMaxUnlockLevel[MAX_CHAPTER_NUM]; //最大解锁关卡索引 0起计数

    int m_nDefenseSetGoalIndex_1;//test
    int m_nDefenseSetGoalIndex_2;//test

    int m_nEnemyNum;
    int m_nHeroNum;
    int m_nOutfitNum;
    
    EBackMusicType m_eCurrentBackMusic;
    vector <GMessage> m_vSystem;
    
    vector <GSign> m_vSign;
    //vector <GMessage> m_vFight;
    int m_iNewMsgCount;
    int m_iCountTime;                   //计数次数
    
    bool m_bTeachStepOver[TEACH_STEP_LIMIT];//新手引导教程记录 true表示已经执行过此教程
    
    HeroRecord **m_pHeroRecord;
    
    CCheckData *m_pCheckBakData;
 //   CSaveRecord *m_pSaveRcd;
    
    bool m_bCompleteTeachLevelOne;
    bool m_bCompleteTeachLevelTwo;
    
    
    struct cc_timeval m_runTime;
    bool m_bGamePause;
    
    
private:
    bool changeBagGoldNum(int nValue, bool bAlert);            
    bool changeBagGemNum(int nValue, bool bAlert);    
    
    //部署情况 -1表示未解锁,0表示尚未设置,大于0表示设置的id
    int m_nPlanHero[PLAN_HERO_NUM];
    int m_nPlanOutfit[PLAN_OUTFIT_NUM];
    int m_nPlanPet_1[PLAN_PET_NUM];
    int m_nPlanPet_2[PLAN_PET_NUM];
    
    Enemy_Data **m_pEnemyData;  //怪数据
    EnemyRecord **m_pEnemyRecord;
    
    Game_Hero_Data** m_pHeroData; //英雄数据

    COutfitData **m_pOutfitData; //道具数据
    OutfitRecord **m_pOutfitRecord;
    
//todo 两种实现方式，哪个效率高
//    typedef std::map<int, LevelRecord*> LevelRecordContainer;
//    LevelRecordContainer m_levelRecordList;//关卡记录 key：副本索引*LEVEL_NUM_OF_CHAPTER + 关卡索引
    LevelRecord **m_pLevelRecord;
    int m_nHeroReference;
};

inline void CDataPool::InitCheckData()
{
    m_pCheckBakData = new CCheckData();
}
inline void CDataPool::FreeCheckData()
{
    ENGINE_SAFE_DEL(m_pCheckBakData);
}
inline void CDataPool::ReadCheckData()//读取数据
{
    if(m_pCheckBakData)
    {
        ClearCheckData();
        CFIO::FileRead(CHECK_DATA_BAK_FILE,(byte *)m_pCheckBakData,sizeof(CCheckData));
    }
};
inline void CDataPool::WriteCheckData()//写入数据
{
    if(!m_pCheckBakData)
    {
        return;
    }
    m_pCheckBakData->gold = gold;
    m_pCheckBakData->gem = xUser->pUserInfo->gemstone;

    CFIO::FileWrite(CHECK_DATA_BAK_FILE,(byte *)m_pCheckBakData,sizeof(CCheckData));
};
inline void CDataPool::ClearCheckData()//清空数据
{
    memset(m_pCheckBakData,0x00,sizeof(CCheckData));
}





#endif /* defined(__SingleEye__DataPool__) */
