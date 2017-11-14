//
//  Lansquenet.h
//  SingleEye
//
//  Created by qinfen on 13-12-11.
//
//
/**
 *  雇佣兵
 */
#ifndef __SingleEye__Lansquenet__
#define __SingleEye__Lansquenet__

#include "GameInclude.h"
USING_NS_CC;
USING_NS_CC_EXT;
class FightLayer;

class Lansquenet : public CCNode
{
public:
    Lansquenet(Enemy_Data *pData,CCLayer *layer,int level=0);
    Lansquenet(const char *name , CCLayer *layer);
    virtual void onEnter();
    virtual void onExit();
    ~Lansquenet();
    inline FSMSystem<Lansquenet> *GetFSMSystem(){return m_pFSMSystem;};
    int getType(){return m_nType;};
    bool CreateFsmSys();
    virtual void update(float delta);
    CCNode* getSprite();
    bool checkCollision(Hero *bullet);
    void skillUpdate(float dt);
    int getAtk(){return m_nAtk;};
private:
    CCArmature *m_pSprite;
    CCSprite *m_sprBarrier;  //当无雇佣兵使用默认栅栏时使用此资源
    char m_strHead[Max_Ani_Strlen];  //资源名字开头
    int m_nType;
    int m_nHp;
    int m_nAtk;
    int m_nDef;
    Game_Timer m_timeCD;
    FightLayer *m_pParentLayer;
    bool m_bUse;
    CCMenuItemSprite *m_mSkillMenu;
    CCProgressTimer *m_pMark;
    CCSprite *m_pGlint;
    
    FSMSystem<Lansquenet> *m_pFSMSystem;
    void skillCallBack(CCObject *pSender);
    void animationEvent(cocos2d::extension::CCArmature *armature, cocos2d::extension::MovementEventType movementType, const char *movementID);
};

#endif /* defined(__SingleEye__Lansquenet__) */
