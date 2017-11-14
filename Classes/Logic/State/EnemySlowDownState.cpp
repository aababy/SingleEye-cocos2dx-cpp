//
//  EnemySlowDownState.cpp
//  SingleEye
//
//  Created by qinfen on 13-11-19.
//
//

#include "EnemySlowDownState.h"

static EnemySlowDownState *instance = NULL;


EnemySlowDownState* EnemySlowDownState::getInstance()
{
    if (!instance)
    {
        instance = new EnemySlowDownState;
    }
    return instance;
}

void EnemySlowDownState::Enter(Enemy *pRole)
{
    if(pRole->getState() == Enemy_State_Stop||pRole->getState()== Enemy_State_Die||pRole->getType() == Enemy_Type_FengXiYi)
        return;
    if(pRole->getState() == Enemy_State_SlowDown)
    {
        createDelayAction(pRole);
        return;
    }
    
    pRole->setState(Enemy_State_SlowDown);
    pRole->stopUpdate();
    pRole->stopSkillUpdate();
    pRole->schedule(schedule_selector(Enemy::moveUpdate), pRole->getVel()*2/1000.0);
    createMoveAction(pRole);
    createDelayAction(pRole);
}

void EnemySlowDownState::createMoveAction(Enemy *pRole)
{
    if(pRole->getType() == Enemy_Type_iBoss)
    {
        pRole->getSprite()->getAnimation()->play("yidong_1");
    }
    else if(pRole->getType() == Enemy_Type_jBoss)
    {
        pRole->getSprite()->getAnimation()->play("yidong_2");
    }
    else
    {
        pRole->getSprite()->getAnimation()->play("yidong");
    }
    pRole->getSprite()->getAnimation()->setMovementEventCallFunc(pRole, movementEvent_selector(Enemy::animationEvent));
}

void EnemySlowDownState::createDelayAction(Enemy *pRole)
{
//    DATAPOOL->addEffectCache(XingXingRes);
//    CCArmature *arm = CCArmature::create("xingxing");
//    arm->getAnimation()->play("xingxing");
//    pRole->addChild(arm,1,Effect_Star);
//    arm->setPosition(ccp(0, pRole->getSprite()->getContentSize().height));
//    CCSequence *_seq = CCSequence::create(
//                                          CCDelayTime::create(m_fTime),
//                                          CCCallFuncN::create(
//                                                              arm,
//                                                               callfuncN_selector(EnemySlowDownState::slowDownEndCallback)),
//                                          NULL);
//    pRole->runAction(_seq);
    pRole->createSlowDelayAction(m_fTime);
}

void EnemySlowDownState::Exit(Enemy *pRole)
{
    
}

void EnemySlowDownState::setMoveInfo(const Fight_Wave_Element *pMoveInfo)
{
    if (pMoveInfo)
    {
        m_tMoveInfo = (*pMoveInfo);
    }
}

void EnemySlowDownState::slowDownEndCallback(cocos2d::CCNode *pSender)
{
//    Enemy *_enemy = (Enemy*)pSender;
//    _enemy->enemySlowDown();
//    if(_enemy->getPreviousState())
//    {
//        _enemy->GetFSMSystem()->ChangeState(_enemy->getPreviousState());
//    }
//    else
//    {
//        EnemyMoveState *_instance = EnemyMoveState::getInstance();
//        _enemy->GetFSMSystem()->ChangeState(_instance);
//    }
//    CCArmature *arm = (CCArmature*)this;
//    _enemy->removeChild(arm, true);
//    DATAPOOL->removeEffectCache(XingXingRes);
}

void EnemySlowDownState::setTime(float time)
{
    m_fTime = time;
}