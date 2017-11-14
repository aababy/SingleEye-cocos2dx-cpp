//
//  EnemyInvisibilityState.cpp
//  SingleEye
//
//  Created by qinfen on 13-12-10.
//
//

#include "EnemyInvisibilityState.h"

USING_NS_CC_EXT;

static EnemyInvisibilityState *instance = NULL;

EnemyInvisibilityState* EnemyInvisibilityState::getInstance()
{
    if (!instance)
    {
        instance = new EnemyInvisibilityState;
    }
    return instance;
}

void EnemyInvisibilityState::Enter(Enemy *pRole)
{
    if(pRole->getState() == Enemy_State_Invisibility||pRole->getState()== Enemy_State_Die)
        return;
    pRole->setState(Enemy_State_Invisibility);
    pRole->stopUpdate();
    pRole->closeDlg();
    pRole->stopSkillUpdate();
    createInvisibilityAction(pRole);
}

void EnemyInvisibilityState::createInvisibilityAction(Enemy *pRole)
{
    DATAPOOL->addEffectCache(ShanXianRes);
    DATAPOOL->playGameEffect(Sound_ShanXian);
    CCArmature *arm = CCArmature::create("shanxian");
    arm->setPosition(pRole->getSprite()->getPosition());
    arm->getAnimation()->play("shanxian");
    arm->getAnimation()->setMovementEventCallFunc(pRole, movementEvent_selector(Enemy::animationEvent));
    pRole->addChild(arm,100);
    CCFadeOut *fadeOut = CCFadeOut::create(0.2f);
    CCSequence *seq = CCSequence::create(
                                         fadeOut,
                                         CCDelayTime::create(2),
                                         CCCallFuncN::create(pRole, callfuncN_selector(EnemyInvisibilityState::invisibilityCallBack)),
                                         NULL);
    pRole->getSprite()->runAction(seq);
}

void EnemyInvisibilityState::Exit(Enemy *pRole)
{
    
}

void EnemyInvisibilityState::setMoveInfo(const Fight_Wave_Element *pMoveInfo)
{
    if (pMoveInfo)
    {
        m_tMoveInfo = (*pMoveInfo);
    }
}

void EnemyInvisibilityState::invisibilityCallBack(CCObject *pSender)
{
    CCArmature *arm = (CCArmature*)pSender;
    Enemy *enemy = (Enemy*)(arm->getParent());
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    float spriteWidth = enemy->getSprite()->getContentSize().width;
    int _x = arc4random()%(int)(size.width - spriteWidth ) + (int)(spriteWidth/2);
    int _y = enemy->getPositionY();
    
    enemy->setPosition(ccp(_x, _y));
    CCFadeIn *fadeIn = CCFadeIn::create(0.2f);
    enemy->getSprite()->runAction(CCSequence::create(fadeIn,
                                                     CCCallFuncN::create(enemy, callfuncN_selector(EnemyInvisibilityState::fadeInCallBack)),
                                                     NULL));
}

void EnemyInvisibilityState::fadeInCallBack(cocos2d::CCObject *pSender)
{
    CCArmature *arm = (CCArmature*)pSender;
    Enemy *enemy = (Enemy*)(arm->getParent());
    EnemyMoveState *_instance = EnemyMoveState::getInstance();
    enemy->GetFSMSystem()->ChangeState(_instance);
}