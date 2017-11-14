//
//  Enemy.h
//  SingleEye
//
//  Created by qinfen on 13-11-11.
//
//

#ifndef __SingleEye__Enemy__
#define __SingleEye__Enemy__

#include "GameInclude.h"

#define PosionTime (4)
#define Enemy_Accelerate (20)
#define MOVE_DELTA_X (200)
#define MOVE_DELTA_Y (200)
#define COMBO_TIME (4)
#define BLOOD_DY (0)

USING_NS_CC;
USING_NS_CC_EXT;
class FightLayer;
class Hero;
class SnatchLayer;
enum Enemy_State
{
    Enemy_State_Move = 1,
    Enemy_State_Atk,
    Enemy_State_UnderAtk,
    Enemy_State_SlowDown,
    Enemy_State_Jump,
    Enemy_State_Stay,
    Enemy_State_DrillStay,
    Enemy_State_Stop,
    Enemy_State_Sprint,
    Enemy_State_Invisibility,
    Enemy_State_Dodge,
    Enemy_State_Shield,  //护盾
    Enemy_State_Eat,
    Enemy_State_Die,
    Enemy_State_MuBei,
    Enemy_State_DrillOut,
};

typedef  enum _Enemy_Type_
{
    Enemy_Type_None = 0,
    Enemy_Type_GeBuLin = First_Enemy_ID,
    Enemy_Type_JuXingGeBuLin,
    Enemy_Type_FengXiYi,
    Enemy_Type_RuanNiGuai,
    Enemy_Type_aBoss,
    Enemy_Type_bBoss,
    Enemy_Type_XueYuGangXie,
    Enemy_Type_ShuiJingZhangYu,
    Enemy_Type_ChangMaoXueGuai,
    Enemy_Type_BingBaoHeTun,
    Enemy_Type_cBoss,
    Enemy_Type_dBoss,
    Enemy_Type_ShaMoJiaChong,
    Enemy_Type_HuangMoGeBuLin,
    Enemy_Type_ShaMoFeiLong,
    Enemy_Type_ShaBaoJuRen,
    Enemy_Type_eBoss,
    Enemy_Type_fBoss,
    Enemy_Type_XiaoGui,
    Enemy_Type_DiYuMoMeng,
    Enemy_Type_DiYuFengQuan,
    Enemy_Type_LingHunShouGeZhe,
    Enemy_Type_gBoss,
    Enemy_Type_hBoss,
    Enemy_Type_JiaoTangKuiLei,
    Enemy_Type_YouEMo,
    Enemy_Type_TangXinMo,
    Enemy_Type_HeiTangEMo,
    Enemy_Type_iBoss,
    Enemy_Type_jBoss,
}Enemy_Type;

enum Effect_Sprite
{
    Effect_BingKuai = 100,
    Effect_TengMan = 101,
    Effect_HuDun,
    Effect_BingXuLi,  //dBoss技能效果
    Effect_MoFaLun_A,
    Effect_MoFaLun_B,
    Effect_BossHuDun,
    Effect_Hp_Progress,
    Effect_Hp_Bar,
    Effect_Flag,
    Effect_Star,
    Effect_Sleep,
    Effect_Particle,
    Effect_Dlg,
};


class Enemy : public CCNode
{
public:
    ~Enemy();
    Enemy();
    Enemy(Enemy_Data *pData,CCLayer *layer,bool pve);
    Enemy(Enemy_Data *pData,CCLayer *layer,int add,int level); //加成系数 等级 
    Enemy(Enemy_Data *pData,SnatchLayer *layer);
    Enemy(Enemy_Data *pdata,CCLayer *layer,int allhp,int lefthp,int defenseAdd);
    void initData(Enemy_Data *pData);
    virtual void onEnter();
    virtual void onExit();
    void addPoints(Fight_Wave_Element *pData);
    void addPoint(CCPoint pos);
    void moveUpdate(float  dt);
    bool checkCollision(Hero *pHero);
    bool checkCollision(CCNode *object);
    bool checkCollisionLayer(CCLayer *layer);
    bool checkCollision(CCPoint pos);
    bool checkCollisionArm(CCNode *object,int dt = 1);
    void setCurrentState(State<Enemy> *pState);
    State<Enemy>* getPreviousState(){return m_pCurrentState;};
    virtual void update(float delta);
    CCArmature* getSprite(){return m_pSprite;};
    void enemyAtk();
    void enemyHurt(bool isHurt);
    void enemySlowDown();
    void enemyStop();
    float getVel(){return m_fVel;};
    int getAtk(){return m_nAtk;};
    int getSkillAtk(){return m_nSkillAtk;};
    int getType(){return m_nType;};
    void stopSkillUpdate();
    
    char* getResHead(){return m_strHead;};  //返回资源开头名
    
    bool CreateFsmSys();
    bool HandleMsg(const Message &msg);
    int getState(){return m_nState;};
    void setState(int nState){ m_nState = nState;};
    void animationEvent(cocos2d::extension::CCArmature *armature, cocos2d::extension::MovementEventType movementType, const char *movementID);
    void frameEvent(Bone *bone, const char *evt, int origiFrameIndex, int currentFrameIndex);
    void minusBlood(int atk,bool powerState,int atkAdd = 0);
    void stopUpdate();
    inline FSMSystem<Enemy> *GetFSMSystem(){return m_pFSMSystem;};
    bool isInvincible();  //是否无敌
    bool isHitInInvincible(); //无敌状态是否能击中
    void replicateCallBack(CCObject *pSender);
    void replicatedCallBack(CCObject *pSender);
    Enemy_Data* getData();
    void setUseSkill(bool result){ m_bUseSkill = result;};  //技能是否使用 某些boss技能在莫中情况下只能使用一次
    bool getUseSkill(){return m_bUseSkill;};
    void playShieldAnimation();
    void playBondageEffectAnimation();  //藤蔓缠绕特效
    void playFrozenEffectAnimation();  //冰冻效果
    void playSleepEffectAnimation(); //沉睡效果
    void stopEffectAnimation();    //停止动画
    void closeDlg();
    void createTransfer();   //生成传送门
    void createShip();       //生成海盗船
    void createTornado();    //生成龙卷风
    void createIceBerg();    //生成冰山
    void createIceWall();    //生成冰墙
    void atkTimeUpdate(float dt);
    float getAtkFrequency(){return m_atkFrequency.mTotalTime;};
    float getSkillCD(){return m_cdTime.mTotalTime;};
    bool isCanEat();   //是否具有吃子弹的功能
    void jiaCheng();
    void CancelJiaCheng();
    void createLockHero(); //锁定英雄
    void createStoneWalling();
    void createHuDun();
    void cancelHuDun();
    bool isBoss();
    void enemyPoison(int time,int atk);
    void setAccelerateX(float x){m_fAccelerateX = x;};
    void setAccelerateY(float y){m_fAccelerateY = y;};
    int getGold(){return m_nGold;};
    void startDisapperUpdate();
    void enemyBondage(int time,int atk);
    void setUseCDSkill(bool res){m_bUseCDSkill = res;};
    bool getRebound(){return m_bRebound;};
    void createSlowDelayAction(float time);
    void createStopDelayAction(float time);
    void removeMissBaojiCallBack(CCNode *pSender);
    void speakInitiative();  //主动说话。
    void speakSkill();  //收到攻击时 能够反弹和吞噬普通攻击的怪说话
    void hitMubei();
    void dieWithBoom();
    void removeJiaCheng();
private:
    FightLayer *m_pParentLayer;
    SnatchLayer *m_pQiangBaoLayer;
    int m_nState;
    CCArmature *m_pSprite;
    Enemy_Data *m_pData;
    char m_strHead[Max_Ani_Strlen];  //资源名字开头
    int m_nType;
    int m_nHp;
    int m_nCurrentHp;
    int m_nAtk;
    int m_nSkillAtk;
    float m_fVel;
    int m_nGold;
    int m_nDistance;
    bool m_bRange;   // true:近程 false:远程
    float m_fAccelerateX;
    float m_fAccelerateY;
    bool m_bUseSkill;   //是否使用技能  用于只是用一次技能的判断
    float m_fAward;  //爆奖概率
    Game_Timer m_gameTime;
    Game_Timer m_atkFrequency;   //攻击频率   如果是boss就是技能2的cd
    Game_Timer m_cdTime;    //boss 技能1的cd
    State<Enemy> *m_pCurrentState;
    FSMSystem<Enemy> *m_pFSMSystem;
    int lastHitMe;  //最后一次攻击我的
    bool isEat;   //是否已经吃过   地狱疯犬
    bool isJiaCheng;  //是否已加成
    bool isFuHuo;  //是否复活
    int  m_nFuHuoHp;  //复活的血量
    bool bossFirstSkill;  //区分boss两个技能 true表示第一个技能使用cd时间 false表示第二个技能 使用atk时间
    bool m_bHuDun; //boss 护盾
    bool m_bJiaCheng; //小鬼 加成
    int m_nPoisonAtk;
    Game_Timer m_PoisonTime;
    bool m_bMoveFree;  //是否自由移动 增加左右移动
    bool m_bQiangBao; //是否枪宝
    bool m_bLipin;  //枪宝游戏中 是否是礼品
    int m_nHeroAtk; //中毒和缠绕时受伤害值
    
    Game_Timer m_moveLRTime;  //左右移动的时间
    bool m_bMoveFreeComplete; //左右移动是否结束
    int m_bMoveFreeDirection;  // 左右移动
    int m_bMoveUDDirection;  //上下移动
    
    bool m_bMoveLRBack;  //左右移动是否已返回
    bool m_bMoveLRArrival;  //左右移动是否已到达目标点
    float m_fLRPosX;   //左右移动的目标点
    float m_fOriginalPosX;  //左右移动的原始x坐标点
    float m_fUDPosY;  //上下移动的目标点
    bool m_bMoveUDArrival;
    float m_fOriginalPosY;
    int m_nCurrentPosIndex;
    bool m_bRecordPos;
    
    bool m_bUseCDSkill;  //是否在使用cdupdate的技能。
    bool m_bUseAtkSkill;  //是否在使用atkupdate的技能。
    
    bool m_bRebound;  //是否反弹伤害
    Game_Timer m_SlowStopTime;
    bool m_bIsSlowAction;
    
    int m_bHurtCount; //被打次数
    Game_Timer m_comboTime;  //某一个时间内连续被打
    bool m_bPowerAtk;  //是否收到蓄力攻击
    bool m_bMuBeiHit;
    bool m_bBoom;

    void disapperUpdate(float dt);
    void disapperCallBack();
    bool searchTarget(CCNode *target);
    void createBullet(const char * name);
    void createBulletAni(const char *fileName,const char *aniName,bool retain);
    void createFallAni(const char *fileName ,const char *aniName);
    void createFlash();
    void createSelf();
    void createFog();
    bool isCanAtk(); //是否能攻击 判断攻击范围
    void actionCallBack(CCObject *pSender);
    void jiaChengUpdate(float dt); //小鬼 加成
    void fuhuoCallBack(CCObject *pSender);
    void enemyCDUpdate(float dt);
    void enemyAtkUpdata(float dt);
    void updateHpBar();
    void poisonUpdate(float dt);
    void bondageUpdate(float dt);
    void removeHpNumCallBack(CCNode *pSender);
    void playBaojiAction();
    float getMoveLRAccelerateX();
    void getNextPos();
    float getMoveLRAccelerateXBoss();
    float getMoveLRAccelerateYBOss();
    void createPos(CCPoint &pos);
    void slowDownEndCallback(CCNode* pSender);
    void stopEndCallBack(CCNode* pSender, void *pData);
    void slowStopUpdate(float dt);  //减速和停止的时间更新
    void removeMofalun();
};

#endif /* defined(__SingleEye__Enemy__) */
