//
//  EnemyReplicateState.cpp
//  SingleEye
//
//  Created by qinfen on 13-12-17.
//
//

#include "EnemyReplicateState.h"

static EnemyReplicateState *instance = NULL;

EnemyReplicateState* EnemyReplicateState::getInstance()
{
    if (!instance)
    {
        instance = new EnemyReplicateState;
    }
    return instance;
}

void EnemyReplicateState::Enter(Enemy *pRole)
{
//    if(pRole->getType() == Enemy_State_Atk)
//        return;
//    pRole->setState(Enemy_State_Atk);
    if(pRole->getState()== Enemy_State_Die)
        return;
    pRole->stopUpdate();
    createReplicateAction(pRole);
}

void EnemyReplicateState::createReplicateAction(Enemy *pRole)
{
    pRole->getSprite()->getAnimation()->play("fenshen");
    
    pRole->runAction(
        CCRepeatForever::create(
                        CCSequence::create(
                        CCMoveBy::create(0.05f, ccp(20,0)),
                        CCMoveBy::create(0.05f, ccp(-20,0)),
                     NULL)));
    pRole->runAction(
                     CCSequence::create(
                     CCDelayTime::create(2),
                     CCCallFuncN::create(pRole, callfuncN_selector(Enemy::replicateCallBack)),
                                        NULL));
}

void EnemyReplicateState::Exit(Enemy *pRole)
{
    
}

void EnemyReplicateState::setMoveInfo(const Fight_Wave_Element *pMoveInfo)
{
    if (pMoveInfo)
    {
        m_tMoveInfo = (*pMoveInfo);
    }
}