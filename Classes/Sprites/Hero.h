//
//  Hero.h
//  SingleEye
//
//  Created by qinfen on 13-11-12.
//
//

#ifndef __SingleEye__Hero__
#define __SingleEye__Hero__

#include "GameInclude.h"
USING_NS_CC;
USING_NS_CC_EXT;
#define Accelerate (15)
enum Hero_State
{
    Hero_State_Normal= 1,
    Hero_State_Ready = 2,
    Hero_State_Move = 3,
    Hero_State_CoolDown=4,
    Hero_State_Power = 5,
    Hero_State_Xuli = 6,
    Hero_State_PowerMove = 7,
};

enum Hero_Type
{
    Hero_Type_None = 0,
    Hero_Type_PingGuo = First_Hero_ID,
    Hero_Type_ShiLiu,
    Hero_Type_CaoMei,
    Hero_Type_XiangJiao,
    Hero_Type_HuoLongGuo,
    Hero_Type_MangGuo,
    Hero_Type_TaoZi,
    Hero_Type_LiuLian,
    Hero_Type_XiGua,
    Hero_Type_Lizi,
    Hero_Type_BoLuo,
    Hero_Type_LianNu,
};

class Hero : public CCNode
{
public:
    Hero(Game_Hero_Data *pData);
    Hero(CCArmature *arm,int atk);
    Hero();
    virtual void onEnter();
    virtual void onExit();
    
    virtual ~Hero();
    void setState(int nState);
    Game_Hero_Data* getData();
    void setAngle(float pAngle);
    bool moveUpdate(float dt);
    extension::CCArmature* getSprite(){return m_pSprite;};
    int getType(){return m_nType;};
    void coolDown();  //进入cd状态
    void cancelCoolDown();
    bool isCoolDown();
    bool isPowerState();
    int getAtk(){return m_nAtk;};
    int getPowerAtk(){return m_nSkillAtk;};
    void setPowerState();
    void setXuli(CCObject *psender);
    void cancelPowerState();
    void cancelXuli();
    void animationEvent(cocos2d::extension::CCArmature *armature, cocos2d::extension::MovementEventType movementType, const char *movementID);
    void reverseX(){m_fAccelerate_X*=-1;};
    void reverseY();
    bool isReverseY();
    bool atkSelf(){return m_bAtkSelf;};
    void setAtkSelf(bool res){m_bAtkSelf = res;};
    float  getDuration(){return m_nDuration;};
    int getAddAtk();  //缠绕和中毒伤害
    float getAccelerateX(){return m_fAccelerate_X;};
    void setHitTarget(){ m_bHitTarget = true;};
    bool getHitTarget(){return m_bHitTarget;};
    bool getAtkEnemy(){return m_bAtkEnemy;};
    void setAtkEnemy(bool res){m_bAtkEnemy = res;};
    bool getRemove(){return m_bRemove;};
    void setRemove(bool res){m_bRemove = res;};
    void setHit(); //设置已集中目标
    bool getHit();
    int getHeroReference(){return m_nReference;};
    bool isLianNuBullet();
    void setRemoveTimeUpdate();  //石榴  火龙果 蓄力发射一段时间后自动消失
private:
    extension::CCArmature *m_pSprite;
//    extension::CCArmature *m_pEffect;  //蓄力特效
//    extension::CCArmature *m_pBurn;  //蓄力特效 英雄后面的燃烧特效
    CCProgressTimer *m_pMask;   //cd状态时 灰色遮罩
    int m_nState;
    int m_nType;
    char m_strHead[20];  //名字资源开头
    Game_Timer m_coolDown; //冷却时间
    int m_nAtk;
    int m_nSkillAtk;
    int m_nBuyGold;
    float m_fAccelerate_X;
    float m_fAccelerate_Y;
    bool m_bAtkSelf;    //是否攻击自己方
    float m_nDuration;
    bool m_bHitTarget;
    bool m_bAtkEnemy;   //敌方 和 自己方的栅栏都攻击
    bool m_bRemove;    //攻击后是否移除
    bool m_bHit;
    int m_nReference;
    bool m_bRemoveTime;
    
    Game_Hero_Data *m_pData;
    
    void playNormalAction();
    void playReadyAction();
    void playMoveAction();
    void playPowerMoveAction();
    void playCoolDownAction();
    void coolDownUpdate(float dt);  //cd状态倒计时
    void pitayaPowerStateUpdate(float dt);  //火龙果蓄力状态倒计时
};

#endif /* defined(__SingleEye__Hero__) */
