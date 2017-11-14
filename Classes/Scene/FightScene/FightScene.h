//
//  FightScene.h
//  SingleEye
//
//  Created by qinfen on 13-10-30.
//
//

#ifndef SingleEye_FightScene_h
#define SingleEye_FightScene_h

#include "GameInclude.h"
USING_NS_CC;
USING_NS_CC_EXT;
#define Max_Angle  (-70)   //弹弓拉动的最大角度
#define Min_Angle  (70)    //最小角度
#define Max_Length (100)
//#define Barrier_Num (4)
#define Barrier_Y  (200)      //栅栏的y轴坐标
#define Atk_Barrier_DY (100)  //到达栅栏攻击点y轴的位移
#define Max_Game_Hero (3)
#define Max_Game_Prop (3)
#define Max_Game_Lans (4)     //一次性显示雇佣兵的最大数量
#define Power_Time  (1.5)     //弹弓蓄力时间
#define Power_Time_Prop  (1)   //铁弹弓减少蓄力的时间
#define Fire_Delta_Time (0.5)   //火龙果 石榴蓄力攻击 自动发射的间隔时间
#define LianNu_Time (5)  //连弩使用时间
#define LianNu_DeltaTime (0.5)  //按住屏幕不动 连弩自动发射的间隔时间
#define Explosion_Num (6)
#define Wave_Time (15)     //15秒刷一波怪
#define Wave_Delay_Time (3)  //3秒的刷怪延迟
#define Max_Enemy (15)   //游戏中怪的最大数
#define EnemyInfo_Num (8)  //大于多少怪就提示 大波怪兽来临 只提醒一次
#define Ship_Num  (3)     //boss技能释放船的个数
#define Barrier_HP (100)   //栅栏基础生命值
#define Award_Time (4)    //怪物死亡时爆奖 奖励物品在屏幕停留时间
#define ZhaDanShangHai (21)  //炸弹伤害
#define BingJingShangHai (2)  //冰晶伤害
#define HongZhaJi_Time (6)   //轰炸机在屏幕出现时间
#define HongZha_DeltaTime (0.5)  //轰炸机放炸弹的间隔时间
#define HongZhaJi_Atk (100)
#define Max_ShiBei (4)     //屏幕出现最多石碑数量
#define Max_Fog (5)         //屏幕出现最多烟雾数量
#define PVP_Defult_Level (19)  //pvp刷怪 默认读取的关卡数
#define PVP_Coefficient_One (1.5)  //pvp加成系数
#define PVP_Coefficient_Two  (2)   //pvp加成系数
#define First_Barrier (1)     //第一防线
#define Second_Barrier (2)    //第二防线
#define ShowResultDelay (1.5)  //游戏结束界面的延迟
#define UNLOCKTAG (-1)
#define YinFuTime (5)      //雇佣兵技能 音符停留时间
#define HOLE_TIME (30)    //黑洞停留时间
#define MAX_INK (5)       //屏幕出现墨水最大数
#define SPEAK_TIME (5)    //怪说话时间
#define Max_Energy (3970.8)  //招魂数
#define ENERGY (2)       //招魂基数
#define MAX_MUBEI (5)    //最大墓碑数
//#define FAST_CLEARANCE  //调试用，快速通关，直接点暂停 继续游戏就是胜利 重新开始就是失败
#pragma mark - FightLayer
typedef enum _Fight_Sprites_
{
    Fight_Sprites_BG = 1,
    Fight_Sprites_Hero1, //非普通英雄
    Fight_Sprites_Hero2,
    Fight_Sprites_Hero3,
    Fight_Sprites_Prop_Tray1,  //道具托盘
    Fight_Sprites_Prop_Tray2,
    Fight_Sprites_Prop_Tray3,
    Fight_Sprites_Prop1,
    Fight_Sprites_Prop2,
    Fight_Sprites_Prop3,
    Fight_Sprites_PropNum1,
    Fight_Sprites_PropNum2,
    Fight_Sprites_Catapult_Frame, //发射器架
    Fight_Sprites_Catapult_Tray, //发射器托盘
    Fight_Sprites_Barrier1, //防护栏
    Fight_Sprites_Barrier2,
    Fight_Sprites_Barrier3,
    Fight_Sprites_Barrier4,
    Fight_Sprites_Btn_Pause, //暂停按钮
    Fight_Sprites_Time,  //游戏时间
    Fight_Sprites_Bullet,
    Fight_Sprites_Gold,   //金币icon
    Fight_Sprites_Gold_Num, //金币数字
    Fight_Sprites_Gem,    //宝石icon
    Fight_Sprites_Gem_Num, //宝石数字
    Fight_Sprites_ZhaDan,  //炸弹
    Fight_Sprites_ZhaDan_Area,  //爆炸区域
    Fight_Sprites_BingJing,   //冰晶
    Fight_Sprites_Progress,  //进度条
    Fight_Sprites_Progress_Icon, // 进度条图标 虫
    Fight_Sprites_Baozha,  //爆炸特效
    Fight_Sprites_ZhunXin,  //准心
    Fight_Sprites_XuLi,   //蓄力
    Fight_Sprites_Burn,   //燃烧
    Fight_Sprites_Trans1,  //传送门1
    Fight_Sprites_Trans2,  //传送门2
    Fight_Sprites_LansHp_Progress,     //雇佣兵 血条
    Fight_Sprites_LansHp_Progress_Bar,
    Fight_Sprites_BestTime,  //最好时间  挑战模式
    Fight_Sprites_HongZhaJi,  //轰炸机
    Fight_Sprites_Ship,
    Fight_Sprites_Tornado,
    Fight_Sprites_FireBall,
    Fight_Sprites_FireFall,
    Fight_Sprites_Ice,
    Fight_Sprites_ChongJiBo,
    Fight_Sprites_Hole,
    Fight_Sprites_EnemyIcon,
    Fight_Sprites_EnemyName,
    Fight_Sprites_Load_BG,
    Fight_Sprites_Load_Progress,
    Fight_Sprites_Load_ProgressBar,
    Fight_Sprites_Load_Txt,
    Fight_Sprites_ShuaXin,  //刷新药 特效
    Fight_Sprites_TeachLayer,
    Fight_Sprites_MoveToCatapult, //正在跳上弹弓的水果
    Fight_Sprites_Tips,
    Fight_Sprites_Energy_Frame,
    Fight_Sprites_Energy_Bar,
    Fight_Sprites_Energy_Txt,
    Fight_Sprites_Energy_Particle,
    Fight_Sprites_Combo_Txt,
    Fight_Sprites_Combo_Num,
}Fight_Sprites; //场景中的所有精灵元素

typedef enum _Fight_Zorder_
{
    Fight_Zorder_BG = -7,
    Fight_Zorder_Enemy = -6,
    Fight_Zorder_Barrier = -5,
    Fight_Zorder_Hero = -4,
    Fight_Zorder_Prop = -3,
    Fight_Zorder_Catapult = -2,
    Fight_Zorder_Bullet = -1,
    Fight_Zorder_Down = 1,
    Fight_Zorder_Mid = 2,
    Fight_Zorder_Up = 3,
    Fight_Zorder_Top = 100,
}Fight_Zorder;

typedef struct _Fight_Mode_Info_
{
    bool bInNetConn; //是否处于网络连接中
    bool bInPause;  //是否处于暂停状态
    byte mLogicState; //状态
    byte mScriptState; //脚本状态
}Fight_Mode_Info;//打斗类型信息

typedef enum _Fight_LogicState_
{
    Fight_LogicState_None = 0,  //无
    Fight_LogicState_Enter,    //进场
    Fight_LogicState_Create,   //出怪
    Fight_LogicState_Ready,    //就绪
    Fight_LogicState_ACT,      //攻击
}Fight_LogicState;  //程序状态

typedef enum _Fight_Script_
{
    Fight_Script_Pause = 0,
    Fight_Script_Run,
}Fight_Script;

typedef struct _Move_Path_Spe_
{
    CCPoint mPoint1;
    CCPoint mPoint2;
    CCPoint mPoint3;
}Move_Path_Spe;  //特殊怪的移动轨迹点

typedef struct _Move_Path_Boss_
{
    CCPoint mPoint1;
    CCPoint mPoint2;
    CCPoint mPoint3;
    CCPoint mPoint4;
}Move_Path_Boss;  //Boss怪的移动轨迹点

typedef enum _Game_Type_
{
    Game_Type_Level = 0,
    Game_Type_Endless,
    Game_Type_PVP,
    Game_Type_Boss,  //boss战
}Game_Type;

typedef enum _Award_Tag_
{
    Award_Tag_Gold = 101,
    Award_Tag_ZhaDan,
    Award_Tag_BingJing,
    Award_Tag_LianNu,
    Award_Tag_Gem,
}Award_Tag;
class Lansquenet;

typedef enum _Charpter_
{
    Charpter_1 = 1,
    Charpter_2,
    Charpter_3,
    Charpter_4,
    Charpter_5,
}_Charpter;

class FightLayer : public CCLayer
{
public:
    CREATE_FUNC(FightLayer);
    FightLayer();
    virtual ~FightLayer();
    bool init();
    virtual void draw(void);
    void onEnter();
    void onExit();
    void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
    void update(float dt);
    void removeEnemyFromArray(CCObject *object);  //移除怪物节点
    void addEnemy(CCObject *object,bool isBoss);  //添加怪物节点
    void setLevel(int chapter, int level);        //设置游戏章节和关卡数
    void setSpeHero(int *hero);                   //设置游戏英雄
    void setProps(int *props);                    //设置道具
    void setLansquenet(int *lans);                //设置第一雇佣兵防线
    void setLansquenetTwo(int *lans);             //设置第二雇佣兵防线
    void setGameType(Game_Type type);             //设置游戏模式
    void setXuliSoundID(int nID){m_nXuliSoundID = nID;};     //设置蓄力音效
    void animationEvent(cocos2d::extension::CCArmature *armature, cocos2d::extension::MovementEventType movementType, const char *movementID);
    void minusBarrierHp(int atk);  //护栏减血
    void addBarrierHp(int atk);    //护栏加血
    void playBarrierUnderAtkAni(); //播放护栏受到攻击动画
    void changeGoldNum(int gold);  //金币数
    void changeGemNum(int gem);    //宝石数
    CCArray* getBulletArray(){return m_pBullet;};
    void addInk(CCObject *object);  //添加墨水效果
    void explosionUpdate(float dt);  //连续爆炸  菠萝蓄力技能
    void removeBullet(Hero *object); //移除子弹
    void createOneEnemy(int nType,CCPoint pos); //创建一个怪
    void createTransfer(Enemy *enemy);   //释放黑洞传送技能
    void createShip(Enemy *enemy);       //释放海盗船攻击技能
    void createTornado(Enemy *enemy);    //释放龙卷风攻击技能
    void createIceBerg(Enemy *enemy);    //释放冰山技能
    void createLockHero(Enemy *enemy);   //释放英雄锁定技能
    void createStoneWalling(Enemy *enemy);  //释放石墙技能
    void createIceWall(Enemy *enemy);       //释放冰墙技能
    void createSelf(Enemy *enemy);          //分身技能
    void createFog(Enemy *enemy);           //释放烟雾的技能
    void createShipLans(Lansquenet *lans);  //释放海盗船技能（雇佣兵）
    void createTornadoLans(Lansquenet *lans);   //释放龙卷风技能（雇佣兵）
    void createIceWallLans(Lansquenet *lans);   //释放冰强技能（雇佣兵）
    void createHole(Lansquenet *lans);          //释放黑洞技能（雇佣兵）
    void createFireBall(Lansquenet *lans);      //释放火球技能（雇佣兵）
    void createFireFall(Lansquenet *lans);      //释放石头坠落攻击技能（雇佣兵）
    void createEnemyBack(Lansquenet *lans);     //释放击退技能（雇佣兵）
    void gameRestart(float cleanGem);
    void gameQuit(bool sendMsg);
    void enemyDie(bool isboss);    //怪物死亡 判断游戏是否结束
    void awardGold(CCPoint pos);   //游戏中杀死怪爆金币
    CCArray* getEnemyArray(){return m_pEnemy;};
    bool areaAttack(CCPoint point,CCObject *object,float radius);   //区域攻击检测
    bool isShowTeachLayer(){return m_bShowTeachLayer;};             //是否显示教学画面
    void setCreateEnemy(bool res){m_bcreateEnemy = res;};
    Game_Type getGameType(){return m_eType;};
    bool isDisableQuitBtn();                               //暂停按钮是否失效
    bool isMaxInk();
    bool isMaxShiBei();                                   //石碑
    bool isEnergy(){return m_bEnergy;};
    void cancelKuangBao();                                //取消狂暴效果
    void retainHuoQiuCount(){m_nHuoQiuCount++;};
    void addMuBei(CCPoint pos);
    bool isMaxMuBei();
    void retainMuBeiCount(){m_nMuBeiCount++;};
    void minusMuBeiCount(){m_nMuBeiCount--;};
    void setBossHp(int hp){ m_nBossHp = hp;};
    void addBossHurtHp(int atk){m_nBossHurtHp+=atk;};
    void resetTrayPos();                                   //重置弹弓拉动位置
    float getCatapultPower(){return m_fPower;};
    bool getPartner(){return m_bPartner;};
    void setPartner(bool res){ m_bPartner = res;};
private:
#pragma mark - msg
    bool handleMsg(const Message &msg);
    
#pragma mark - init
    bool initLevelResInfo();   //读取数据 初始化关卡所需资源
    bool initPublicInfo();   //读取数据 初始化公共资源
    bool initHeroInfo();  //读取数据 初始化英雄
    bool initPropInfo();
    bool initLansInfo();  //读取数据 初始化雇佣兵（栅栏)
    
#pragma mark - logic
    void checkState();//检查此时程序的状态
    void pullCatapult(CCPoint point);          //拉动弹弓
    bool createEnemy();                        //创建怪
    bool getLevelData(int nLevel);             //读取怪物数据信息
    void fireBullet(Hero *pHero , float pAngle); //发射子弹
    void fireBulletLianNu(float pAngle);         //连弩发射子弹
    void createNewBullet();                      //创建新子弹
    void setCoolDown(Hero *pHero);  //设置冷却
    void touchCatapult(CCPoint &point);
    void createWave(float dt);  /////
    int  getLevelNum();
    void updateProgress();  //更新进度条
    void updateEnemyZorder();  //更新怪物移动后的遮罩问题
    void updateLianNu(float dt);  //连弩使用时间更新
    void updateLianNuDelta(float dt);   //连弩使用间隔时间刷新
    void rotateLianNu(CCPoint point);   //旋转连弩
    void createEffect(const char *fileName,const char *aniName,CCPoint pos,int zOrder,CCObject *target = NULL,bool loadFile = true); //添加特效
    void createPeel(CCPoint pos,int num);  //产生香蕉皮
    void checkHeroAtk(Hero *_bullet,Enemy *_enemy,int _type,int index); //英雄攻击
    void checkSpriteCollision(Hero **_bullet);   //检查游戏中的碰撞
    bool checkPeelCollision(CCSprite *peel);  //检查是否踩中香蕉皮
    void checkMuBei();                       //检查是否触发墓碑技能
    void readData();
    void refreshData(float cleanGem);        //重置数据
    void checkTrans();                        //检查是否触发传说技能
    void checkBarrierCollision(Hero *_bullet);   //当英雄受到技能反攻自己方的时候检查碰撞
    void updateHpBar();   //更新血条
    void updatePropNum();
    int getAward(int tag);   //计算获得奖品的种类和数量
    void checkShiBei();       //检查是否触发石碑技能
    void usePropBegan(CCPoint location);  //
    void selectHero(CCPoint location);   //选择英雄
    void useSecondLansquenet(); //使用第二防线
    void playLoseResult();
    void checkLansSkill();  //检测雇佣兵技能
    void updateGemNum();
    void loadingUpdate(float dt);   //loading界面
    void cleanArmatureDataCache();
    void createQiangBaoGuai();     //创建抢宝怪
    bool checkQiangBaoGuaiCollision(Hero **_bullet);   //检测抢宝怪是否被击中
//    void coundPicNums();
    void checkSpeak(float dt);              //检查是否触发怪物说话
    void enemySpeak();                      //说话
    void updateEnergy(CCPoint pos);         //招魂
    void starEnergyState();                 //开始招魂狂暴状态
    void endEnergyState();
    void createPartnerAni();                //创建助阵小伙伴
    void levelStatistics();                 //
    void checkCollision(float dt);
    bool collisionMuBei(Hero *bullet,Enemy *enemy);
    bool collisionShiBei(Hero *bullet,Enemy *enemy);
    void getTotalWave();
    bool isBoss(int type);
    void beganLianNuTime(float dt);
    void checkTeachLayer(float dt);
    void checkPeel(float dt);
    void checkResult(float dt);
    void checkNextWave(float dt);
    void checkPull(float dt);
    void touchAward(CCPoint location);
    void touchInk(CCPoint location);
    void touchShiBei(CCPoint location);
    void touchFog(CCPoint location);
    void touchEnd(CCPoint location);
#pragma mark - callbackfunc
    void menuPauseCallBack(CCObject* pSender);
    void catapultResetCallBack();  //弹弓已发射且已复位
    void bulletReadyCallBack(CCObject* pSender);
    void bingkuaiRemoveCallBack(CCObject *pSender);
    void peelCallBack(CCObject *pSender);
    void icebergCallBack(CCObject *pSender, void *data);
    void removeAwardCallBack(CCNode* pSender);  //移除屏幕中掉落奖品
    void hongZhaJiCallBack(CCNode *pSender);
    void kuangBaoYaoCallBack(CCNode *pSender);
    void shuaXinCallBack(CCNode *pSender);
    void lockHeroCallBack(CCNode *pSender);
    void awardActionCallBack(CCNode *pSender);
    void loadingComplateCallBack(CCNode *pSender);
    void waveDelayCallBack(CCNode *pSender);
    void disapperCallBack(CCNode *pSender);
    void dangongFireCallBack();
    void dangongPowerFireCallBack();
    void hongzhajiCallBack();
    void newHeroCallBack();
    void qiangBaoCallBack();
    void qiangbaoguaiCallBack(CCNode *pSender);
    void showQiangBaoCallBack(CCNode *pSender);
    void startQiangBaoTeachCallBack();
//    void loadingCallBack(float dt);
    void removeParticleCallBack(CCNode *pSender);
    void updateCombo(CCPoint pos);
    void removeBMFontCallBack(CCNode *pSender);
    void removeEnergyParticleCallBack(CCNode *pSender);
    void removeEnergyParticleCleanStateCallBack(CCNode *pSender);
    void partnerShowCallBack(CCNode *pSender);
public:
    void showWinResultCallBack(CCNode *pSender);
    void showLoseResultCallBack(CCNode *pSender);
    void remoteAtkCallBack_Sprite(CCNode *pSender,void *data);
    void remoteAtkCallBack_Armature(CCNode *pSender,void *data);
    void remoteAtkCallBack_Armature_Tornado(CCNode *pSender,void *data);
    void remoteAtkCallBack_Armature_HuoQiu(CCNode *pSender,void *data);
    void remoteAtkCallBack_ArmatureLans(CCNode *pSender,void *data);
    void removeCallBack_Sprite(CCNode *pSender);
    void removeEnemyCallBack(CCNode *pSender);
    void removeAtkCallBack_Armature(CCNode *pSender);
    void removeArmatureWithClean(CCNode *pSender);
private:
#pragma mark - Scheduler
    void timeUpdate(float dt);
    void waveUpdate(float dt);
    void hongZhaJiUpdate(float dt); //轰炸机轰炸
#pragma mark - var
private:
    int m_nGameState;
    bool bTouchedTray;
    float m_fAngle;     //旋转的角度
    int m_nTrayWidth;
    int m_nMaxOffset_x;  //
    float m_fCatapult_Org_X;
    float m_fCatapult_Org_Y;
    float m_fPower;  //拉弹弓的力度（长度）
    float m_fBullet_Org_Y;
    
    float m_fGamePercent;
    
    int  m_nFireCount;
    int  m_nHitCount;

    Fight_Mode_Info m_eModeInfo; //程序状态
    Fight_Level_Info *m_pLevelInfo;  //关卡资源
    Fight_Level_Data *m_pLevelData;  //关卡数据
    int  m_nTime;  //游戏时间
    int m_nBestTime;
    int m_nCurrentWave;  //当前波
    int m_nTotalWave;
    CCArray *m_pEnemy; //怪列表
    CCArray *m_pBarrier; //障碍列表
    
    CCArray *m_pBullet; //子弹列表
    CCArray *m_pHero;  //英雄列表
    CCArray *m_pInk;  //墨水列表
    CCArray *m_pPeel;  //香蕉皮
    CCArray *m_pShiBei; //石碑
    CCArray *m_pIceWall; //冰墙
    CCArray *m_pMuBei;
    CCArray *m_pFog; //雾
    
    CCArray *m_pAward;//掉落列表
    
    int m_nBarrierCurrentHp;
    int m_nBarrierTotalHp;
    int m_nBarrierCurrentHp_Second;
    int m_nBarrierTotalHp_Second;
    int m_nCurrentBarrier;  //当前防线 1 2
    int m_nGold;  //金币数
    int m_nGem; //宝石数
    int m_nGainGold; //游戏中获得的金币数
    int m_nCurrentChapter;  //当前章节
    int m_nCurrentLevel;   //当前关卡
    int *m_nHaveSpeHero;   //游戏中可以使用的特殊英雄
    int *m_nUsePropType;  //游戏中使用的道具类型
    int *m_nUseLansTypeOne;  //使用雇佣兵类型
    int *m_nUseLansTypeTwo;
    
    Game_Timer m_powerTime;  //蓄力时间
    Game_Timer m_fireDeltaTime; //发射间隔时间   连弩和火龙果的蓄力攻击
    Game_Timer m_lianNuDeltaTime; //连弩发射间隔时间
    Game_Timer m_lianNuUseTime;  //连弩使用时间
    
    Game_Timer m_waveTime;  //一波的间隔时间
    
    int m_nXuliSoundID;  //蓄力音效的id
    bool m_bIsUseLianNu; //是否使用连弩
    bool m_bIsFireLianNu; //是否发射连弩
    bool m_bDelayComplate;
    bool m_bcreateEnemy;
    
    Game_Type m_eType;
    Game_Timer m_explosionNum;  //菠萝蓄力攻击爆炸个数
    float explosionPosX; //爆炸的x坐标
    bool m_bUseHongZhaJi;//是否使用轰炸机
    bool m_bUseKuangBao;  //是否使用狂暴药剂
    bool m_bUseShuaXin;//是否使用刷新球
    bool m_bLockHero;//是否锁定英雄
    int m_LansIndex;//pvp获取雇佣兵的索引
    int m_touchPropIndex;
    bool m_bIsComplateLoading; //是否完成载入
    
    int m_nGameGainGem;//游戏中获得宝石数
    int m_nGameUseGem;
    
    bool m_bDanGongFire_Teach;
    bool m_bDanGongPowerFire_Teach;
    bool m_bStartingFire_Teach;  //是否进行弹弓教学操作
    bool m_bStartingPowerFire_Teach;
    
    bool m_bUseHongZhaji_Teach;
    bool m_bStartingHongZhaJi_Teach;
    bool m_bUseNewHero_Teach;
    bool m_bStartingUseNewHero_Teach;
    
    bool m_bDaoShuComplate;
    bool m_bEnemyInfo;  //大波怪兽来临提示
    bool m_bEnemyInfoShow;
    
    bool m_bShowTeachLayer;
//    int m_nLoadingPicNums; //载入资源总张数
//    int m_nLoadingPicNum;  //已经加载的资源
    bool m_bHitComb;  //连击是否击中 用于桃子和菠萝技能 只算一次击中
    
    bool m_bShowResultLock;  //显示成功界面锁  防止多次调用
    
    bool m_bCanPullCatapult;  //怪物打完 无法拉弹弓
    
    bool m_bIsSnatch;  //是否抢宝
    
    CCArmature *m_pQiangbaoGuai;  //抢宝怪
    bool m_bCompleteWave;  //完成所有刷怪  关卡模式使用
    
    bool m_bStartQiangBao_Teach;
    int m_nBossType;  //boss战
    int m_nXuLiDeltaTime;  //蓄力的改变值
    bool m_bTwoBoss; //是否有boss分身
    bool m_bFirstWave;
    
    Game_Timer m_speakTime; //对话检查时间
    int m_nCombo;
    
//    int m_nComboCount; //连击次数
    float m_nEnergy; //
    bool m_bEnergy;
    bool m_bShowPartner; //
    bool m_bPartner;  //助战英雄
    int m_nTornadoCount;
    int m_nHuoQiuCount;
    int m_nBossHp;
    int m_nBossHurtHp;  //boss受到的伤害值
    int m_nMuBeiCount;
    float m_fLoadPercent;
    bool m_bLastWave;
    bool m_bcreatePartner;
};


#pragma mark - FightScene
class FightScene : public CCScene
{
public:
    CREATE_FUNC(FightScene);
    virtual ~FightScene();
    bool init();
    void setGameLevel(int chapter, int level);
    void setGameSpeHero(int *hero);
    void setGameProps(int *props);
    void setGameLans(int *lans);
    void setGameLansTwo(int *lans);
    void setGameType(Game_Type type);
    void setBossHp(int hp);
    virtual void onEnter();
    virtual void onExit();
private:
//    Game_Type m_enumType;
//    int m_nCurrentChapter;  //当前章节
//    int m_nCurrentLevel;   //当前关卡
//    int *m_nHaveSpeHero;   //游戏中可以使用的特殊英雄
//    int *m_nUsePropType;  //游戏中使用的道具类型
//    int *m_nUseLansTypeOne;  //使用雇佣兵类型
//    int *m_nUseLansTypeTwo;
};

#pragma mark - PauseLayer
class PauseLayer : public CCLayer
{
public:
    CREATE_FUNC(PauseLayer);
    virtual ~PauseLayer();
    bool init();
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onEnter();
private:
    void callbackContinue(CCObject *pSender);
    void callbackRestart(CCObject *pSender);
    void callbackExit(CCObject *pSender);
    CCMenuItemSprite *m_itemExit;
};



#endif
