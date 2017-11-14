//
//  GameDefine.h
//  SingleEye
//
//  Created by qinfen on 13-11-5.
//
//

#ifndef SingleEye_GameDefine_h
#define SingleEye_GameDefine_h

#define USING_NAMESPACE_COCOS \
USING_NS_CC;\
USING_NS_CC_EXT;\
using namespace CocosDenshion;\
using namespace std;\
using namespace engine;

#define CC_RETURN_IF(cond)  if(cond) return
#define CC_RETURN_VALUE_IF(cond,value)  if(cond) return(value)

#ifndef _BYTE_
#define _BYTE_
typedef signed char byte;
#endif

#define Max_Name_Strlen (60) //名字长度
#define Max_Ani_Strlen (20)  //动画资源长度
#define MAX_HERO  (5)  //英雄总数
#define MAX_PROPS (11)  //道具总数
#define MAX_BOSS  (3)  //BOSS总数
#define MAX_PET (MAX_BOSS)  //雇佣兵总数

#define MAX_CHAPTER_NUM (5)     //副本总数
#define LEVEL_NUM_OF_CHAPTER    (10)    //每个副本的关卡数量

#define LIFE_LEVEL_MAX_NUM          (5) //最高等级
#define LIFE_ATTACK_STAR_MAX_NUM    (5) //伤害星级最大值

#define SAVE_RECORD_FILE        ("savaRecord.dat")  //存档文件
#define CHECK_DATA_BAK_FILE     ("checkData.bak")   //防改内存破解备份文件

//TIPS小贴士可选数量
#define TIPS_PIC_NUM    (5)

#define MAGIC_NUM       (1234)

//部署位置数
#define PLAN_HERO_NUM       (3)  //todo 和 Max_Game_Hero 合并
#define PLAN_OUTFIT_NUM     (2)
#define PLAN_PET_NUM        (4)
#define SHARE_NUM           (5)

#define First_Enemy_ID  (1101)
#define First_Hero_ID   (1001)
#define First_Outfit_ID (7001)
#define HeroTotalNum (11)
#define EnemyTotalNum (30)
#define MESSAGE_LIMIT   (19)
#define MES_SYSTEM_NAME ("\system.dat")

//距离屏幕边缘最小距离
#define MIN_SPACE_TO_SCREEN_EDGE (4)

#define PINGLUN_AWARD_GEM_NUM       (20)
#if 1 == COCOS2D_DEBUG
#define TO_PINGLUN_TIME_INTERVAL    (20)     //进入游戏到弹出评论跳转页面提示的时间间隔，单位秒
#define CHECK_TO_PINGLUN_FREQ       (5)
#else
#define TO_PINGLUN_TIME_INTERVAL    (10*60)     //进入游戏到弹出评论跳转页面提示的时间间隔，单位秒
#define CHECK_TO_PINGLUN_FREQ       (60)
#endif//COCOS2D_DEBUG

//#define SIGN_DEBUG

/////////////////////////////////////////////   color
#define CLR_BLUE        ccc3(0, 58, 100)


/////////////////////////////////////////////
#define xNet            (NetWorker::Instance())->getNetSender()
#define xUser           UserData::Instance()
#define xUserInfo       (UserData::Instance())->pUserInfo
#define xSprite         (CCSpriteFrameCache::sharedSpriteFrameCache())



#define tPlist          UI_TEX_TYPE_PLIST

/////////////////////////////////////////////
#define NC_NewMessage           "NewMessage"
#define NC_Test                 "Test"
#define NC_LoadImage            "loadImageFinish"
#define NC_AsyncBeforeFinish    "asyncBeforeFinish"
#define NC_AsyncLaterFinish     "asyncBeforeAfter"


#define Max_Jump_Y_Center 320  //距离中点向上跳的最远距离  y：900以上是天空 不能跳
#define POS_X_DX 45
#define MAX_POS_X  ((CCDirector::sharedDirector()->getWinSize().width)-POS_X_DX)
#define MIN_POS_X  POS_X_DX

#define MAX_POS_Y ((CCDirector::sharedDirector()->getWinSize().height/2)+Max_Jump_Y_Center)
#define MIN_POS_Y (Barrier_Y+Atk_Barrier_DY)

//存档方式选择
#define TYPE_OF_SAVE_METHOND     (1)

//新手引导
//注意修改此处时，对应更新fightTutorialsEnd()和gameTutorialsEnd()
typedef enum
{
    TEACH_STEP_ENTER_MAOXIAN = 0,   //进入冒险
    TEACH_STEP_ENTER_FUBEN1,         //进入副本1
    TEACH_STEP_ENTER_FUBEN2,
    TEACH_STEP_ENTER_FUBEN3,
    TEACH_STEP_ENTER_FUBEN4,
    TEACH_STEP_ENTER_FUBEN5,
    TEACH_STEP_ENTER_LEVEL1,        //进入关卡1
    TEACH_STEP_ENTER_LEVEL2,        //进入关卡2
    TEACH_STEP_ENTER_FIGHT1,          //进入第1关战斗
    TEACH_STEP_ENTER_FIGHT2,          //进入第2关战斗
    TEACH_STEP_ENTER_SELECT_HERO,   //进入英雄选择
    TEACH_STEP_ENTER_SELECT_OUTFIT, //进入道具选择
    TEACH_STEP_ENTER_SELECT_PET,    //进入宠物选择
    TEACH_STEP_SELECT_HERO,         //选择英雄 部署界面
    TEACH_STEP_SELECT_HERO_G,       //选择英雄 游戏界面
    TEACH_STEP_SELECT_OUTFIT,       //选择道具
    TEACH_STEP_SELECT_PET,          //选择宠物
    TEACH_STEP_SHOP_SELECT_OUTFIT,  //商店-选择道具
    TEACH_STEP_SHOP_BUY_OUTFIT,      //商店-购买道具
    TEACH_STEP_USE_DANGONG1,        //弹弓普通用法
    TEACH_STEP_USE_DANGONG2,        //弹弓蓄力用法
    TEACH_STEP_USE_HONGZHAJI,       //道具轰炸机
    TEACH_STEP_USE_RAND_OUTFIT,     //随机道具用法
    TEACH_STEP_YAOJIANG,            //摇奖
    TEACH_STEP_QIANGBAO,            //抢宝怪第一次出现 提示
    TEACH_STEP_QIANGBAO_SLIDE,      //第一次进入抢宝界面
    TEACH_STEP_INK1,
    TEACH_STEP_INK2,
    TEACH_STEP_INK3,
    TEACH_STEP_SHIBEI1,
    TEACH_STEP_SHIBEI2,
    TEACH_STEP_SHIBEI3,
    TEACH_STEP_LIMIT
}ETeachStepIndex;

enum ETavernOperationType
{
    TAVERN_OP_TYPE_NULL = 0,
    TAVERN_OP_TYPE_UNLOCK,          //解锁
    TAVERN_OP_TYPE_TRAIN,           //训练
    TAVERN_OP_TYPE_NUM
};

typedef struct _Game_Timer_
{
    float mCurTime;
    float mTotalTime;
}Game_Timer;

typedef struct _Emitter_Info_
{
    char cEmitterFrame[Max_Name_Strlen];   //发射器框架的资源名
    char cEmitterTray[Max_Name_Strlen];    //发射器托盘的资源名
    char cEmitterName[Max_Name_Strlen];   //发射器名字
}Emitter_Info;   //发射器信息

typedef struct _Fight_Wave_Element_
{
    int mType;
//    int mPointCount;
//    cocos2d::CCPoint *mArrayPoint;
     cocos2d::CCPoint mArrayPoint;
}Fight_Wave_Element;

typedef struct _Fight_Wave_Data_
{
    int mElementCount;
    Fight_Wave_Element **mArrayElement;
}Fight_Wave_Data;

typedef struct _Fight_Level_Data_
{
    int nWaveCount;
    Fight_Wave_Data **mArrayWave;
}Fight_Level_Data;

typedef struct _Fight_Level_Info_   //关卡数据信息
{
    int nLevelID;    //关卡id
//    Emitter_Info *pEmitterInfo;   //发射器
//    char cBarrierName[Max_Name_Strlen];  //栅栏资源名
//    int nBarrierHp;
//    int nBarrierType;
    char cBackgroudName[Max_Name_Strlen];  //背景资源名
    float fTime;   //每一关卡的时间
}Fight_Level_Info;

typedef struct _Enemy_Data_
{
    int mType;                 //类型
    char mSourceName[Max_Ani_Strlen];  //资源名
    bool mRange;               //是否是近程攻击
    int mCD;                   //cd
    int  mHp;                 //生命值
    int  mHpAdd;              //生命成长
    int  mAtk;               //普通伤害
    int  mAtk2;               //基本攻击力
    int  mAtkAdd;             //攻击力升级附加
    int  mSkillAtk;            //技能攻击力
    int  mSkillAtkAdd;         //技能附加伤害
    int  mDuration;            //持续时间
    int  mMagicID;             //技能索引
    float mStar;               //星级
    float mVel;               //速度
    int mDistance;            //攻击距离
    int mAtkFrequency;        //攻击频率
    int mBuy;                 //解锁条件
    int mBuyGold;            //解锁金币
    int mBuyGem;             //解锁宝石
    int mRecruitGold;        //招募金币
    int mRecruitGem;         //招募宝石
    int mUpGold;            //升级金币
    int mUpGem;             //升级宝石
    int mGold;               //死亡掉落金币
    float mFrequency;          //死亡掉落机率（暴率）
//     int mSkill;                 //技能索引
//    int mUnlock;                //解锁条件索引
//    int mZhaomuMoney;           //招募用金币数量
    
}Enemy_Data;  //怪物数据

typedef struct _Game_Hero_Data_
{
    int mType;                //类型
    char mSourceName[Max_Ani_Strlen];  //动画资源名
    int mCoolDown;             //冷却时间(ms)
    int mAtk;                   //普通伤害
    int mAtk2;                  //基本攻击力
    int mAtkAdd;                 //升级附加
    int mSkillAtk;              //技能伤害
    int mSkillAtkAdd;             //技能伤害附加
    float mDuration;              //持续时间
    float mStar;                   //星级
    int mBuyGold;                //解锁金币
    int mBuyGem;                //解锁宝石
    int mRecruitGold;           //招募金币
    int mRecruitGem;            //招募宝石
    int mUpGold;                //升级金币
    int mUpGem;                //升级宝石
    int mBuy;                   //解锁条件
    int mMagicID;               //技能索引
    int mUnLockLevel;           //解锁关卡
}Game_Hero_Data; //游戏中英雄的相关信息

typedef struct _Prop_Data_
{
    int  mID;
    char m_sourceName[Max_Ani_Strlen];
    int  mHp;
}Prop_Data;   //游戏中使用的道具所需数据

class COutfitData
{
public:
    COutfitData()
    {
        m_nId = 0;
        m_nGoldPrice = 0;
        m_nGemPrice = 0;
        m_nHp = 0;
        m_nMaxNum = 0;//最多拥有数量
        
        m_pName = NULL;
        m_pSourceName = NULL;
        m_pInfo = NULL;
        m_pUse = NULL;
    }
    ~COutfitData()
    {
        CC_SAFE_RELEASE(m_pName);
        CC_SAFE_RELEASE(m_pSourceName);
        CC_SAFE_RELEASE(m_pInfo);
        CC_SAFE_RELEASE(m_pUse);
    }
public:
    int  m_nId;
    int  m_nGoldPrice;
    int  m_nGemPrice;
    int  m_nHp;
    int  m_nMaxNum;//最多拥有数量 0 表示无限
    
    cocos2d::CCString *m_pName;
    cocos2d::CCString *m_pSourceName;    //资源名
    cocos2d::CCString *m_pInfo;
    cocos2d::CCString *m_pUse;
};
//
//typedef struct _OutfitData
//{
//    int  m_nId;
//    char m_name[Max_Ani_Strlen];//todo 3个文本长度，改成堆内存
//    char m_sourceName[Max_Ani_Strlen];   //资源名
//    char m_info[Max_Ani_Strlen];
//    char m_use[Max_Ani_Strlen];
//    int  m_nGoldPrice;
//    int  m_nGemPrice;
//    int  m_nHp;
//}OutfitData;

typedef struct _HeroRecord
{
    bool m_bUnlock; //解锁
    int m_nLevel;   //等级
}HeroRecord;

typedef struct _EnemyRecord
{
    bool m_bUnlock;
    int m_nLevel;
    int m_nHpPercent;
}EnemyRecord;

typedef struct _OutfitRecord
{
///    bool m_bUnlock;
    int m_nNum;   //数量
}OutfitRecord;

//关卡记录
typedef struct
{
///    bool m_bUnlock; //解锁
    int m_nScore;   //得分
}LevelRecord;

typedef struct _BagCell
{
    int m_nId;
    int m_nNum;
    bool m_bSelect;
}BagCell;

typedef enum _Stop_Type_
{
    Stop_Type_None = 0,
    Stop_Type_Frozen = 1,  //冰冻
    Stop_Type_Bondage = 2, //束缚
    Stop_Type_Sleep,
}Stop_Type;

//typedef struct _PlayerData_
//{
//    bool firsttime;
//    int gold;
//    int gem;
//    int currentChapter;
//    int currentLevel;
//    
//    bool buiedHero[MAX_HERO];  //英雄是否购买  true为已买 类型为其索引位置
//    int heroLevel[MAX_HERO];
//    
//    bool buiedProps[MAX_PROPS]; //道具是否购买 true为已买
//    int propsNum[MAX_PROPS];
//    std::vector<Prop_Data> props;
//}PlayerData;
//static PlayerData _playerData;
///////////////////////////////////////////////////
#define ARENA_MOENYTIME (15*60)//站位倒计时长15分钟
#define ARENA_CLRMOEY (10)//清空倒计时花费魔晶币
//#define NVNU_BUYUSE (10)//购买互动次数,消费魔晶
//#define NVNU_BUYSAVE (10)//购买解救次数,消费魔晶
//#define NVNU_BUYQD (10)//购买抢夺次数,消费魔晶
#define NVNU_HELPUSE (10)//求救,消费魔晶
//#define NVNU_JIANJINUSE (10)//监禁,消费魔晶
#define NVNU_HD_MONEY   (10)//女神互动,消耗金币
#define NVNU_HD_MJBIN   (100)//女神互动,消耗魔晶币
////

#define SCENE_TXT_BOARD_M (1)//文字描边宽度中号
#define SCENE_TXT_BOARD_S (0.5f)//文字描边宽度小号
#define SCENE_NAME_LIMIT    (7)//名字显示最大位数
#define USING_TXT_LEN   (300)//系统临时字符长度
#define SCEEN_TITLE_H   (60)//场景标题高度
#define SCEEN_TITLE_LOFF (100)//场景标题水平偏移
#define SCEEN_CLOSE_HOFF  (8)//场景标题退出按扭右偏移
#define SCEEN_HEADSEL_FT  (0.3f)//女神头像选中动画帧时间
#define SCEEN_BOXSEL_FT  (0.5f)//框选中动画帧时间

#define LAYERBOX_BG_HOFF  (65)//背景框中心水平偏移
#define LAYERBOX_EXIT_ROFF  (78)//按扭退出水平偏移
#define LAYERBOX_EXIT_TOFF  (132)//按扭退出垂直偏移
#define LAYERROLE_VOFF  (70)//角色妹子垂直偏移

#define RULT_GETTIME_MAX    (5)//最大额外抽奖次数

///////////////////////////////////////////// for old code
#define ENGINE_MAX_STRLEN   (50)



typedef enum _SCENE_ID_
{
    SCENE_LOGO = 0,//初始化加载界面
    SCENE_LOGIN,//注册界面
    SCENE_MAIN,//主界面
    SCENE_TRANS,//过渡界面
    SCENE_SIXZONE,//阵型界面
    SCENE_NVSHEN,//女神系统
    SCENE_ARENA,//竞技场界面
    SCENE_NVNU,//女奴系统
    SCENE_FIGHT,//战场界面
    SCENE_RESULT,//战场结算界面
    SCENE_HERO,//抽将系统
    SCENE_EQUIP,//装备系统
    SCENE_COPY,//副本系统
    SCENE_ROLE,//角色系统
    SCENE_PACKET,//背包系统
    SCENE_OPRETE,//活动系统
    SCENE_MAIL,//邮件系统
    SCENE_SIZE,//
}SCENE_ID;

typedef enum _FIGHT_SOUND_TYPE_
{
    FIGHT_SOUND_TYPE_NOR = 0,//普通and精英副本
    FIGHT_SOUND_TYPE_AO,//奥林匹斯山
    FIGHT_SOUND_TYPE_EYE,//索伦之眼
}FIGHT_SOUND_TYPE;//战场声音类型

typedef struct _RESULT_GETPORP_
{
    int mExtUseList[RULT_GETTIME_MAX];//消耗
}RESULT_GETPORP;//战斗抽取支付魔晶币

enum Prop_Type
{
    Prop_Type_None = 0,
    Prop_Type_ZhaDan = First_Outfit_ID,
    Prop_Type_BingJing,
    Prop_Type_ShuangFeng,
    Prop_Type_HongZhaJi,
    Prop_Type_KuangBaoYao,
    Prop_Type_HuiFuYao,
    Prop_Type_ShiZhaLan,
    Prop_Type_TieZhaLan,
    Prop_Type_TieDanGong,
    Prop_Type_LianNu,
    Prop_Type_ShuaXinQiu,
};

typedef enum
{
    BACKMUSIC_TYPE_TITLE = 0,
    BACKMUSIC_TYPE_HOME,
    BACKMUSIC_TYPE_LIMIT
}EBackMusicType;


typedef enum _SceneTag
{
    ST_None = 0,
    ST_Load,
    ST_Login,
    ST_PVP,
    ST_PLan,
    ST_Fight,
    ST_Story,
    ST_Home,
    ST_Title,
    ST_Message,
    ST_BuyGem,
    ST_BuyGold,
    ST_Shop,
    ST_Dungeon,
    ST_Huodong,
    ST_Defense,
    ST_Tavern,
    ST_Level,
    ST_Share,
    ST_Sign,
    ST_BuyPower,
    ST_Boss,
    ST_Setting,
    ST_SelectHero,
    ST_SelectPet,
    ST_SelectOutfit,
    ST_About,
    ST_PingLun,
    ST_COUNT,
}SceneTag;


struct SceneData {
    SceneTag aTag;
    int iData;
    void* pData;        
};


#define HERO_ID_APPLE   (1001)

#endif
