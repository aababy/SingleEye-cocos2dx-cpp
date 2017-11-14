//
//  EnemyMoveState.cpp
//  SingleEye
//
//  Created by qinfen on 13-11-15.
//
//

#include "EnemyMoveState.h"
USING_NS_CC_EXT;
static EnemyMoveState *instance = NULL;


EnemyMoveState* EnemyMoveState::getInstance()
{
    if (!instance)
    {
        instance = new EnemyMoveState;
    }
    return instance;
}

void EnemyMoveState::Enter(Enemy *pRole)
{
    if (pRole->getState() == Enemy_State_Move||pRole->getState()== Enemy_State_Die)
    {
        return;
    }
    
    createMoveAction(pRole);
    pRole->schedule(schedule_selector(Enemy::moveUpdate), pRole->getVel()/1000.0);
    pRole->stopEffectAnimation();
    pRole->setState(Enemy_State_Move);
    pRole->setCurrentState(this);
}

void EnemyMoveState::createMoveAction(Enemy *pRole)
{
  
    if(pRole->getType()==Enemy_Type_FengXiYi)
    {
        pRole->getSprite()->getAnimation()->play("daiji");
    }
    else if(pRole->getType() == Enemy_Type_iBoss)
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
    
    pRole->getSprite()->getAnimation()->setMovementEventCallFunc(pRole,movementEvent_selector(Enemy::animationEvent));
}

void EnemyMoveState::Exit(Enemy *pRole)
{
    
}

void EnemyMoveState::setMoveInfo(const Fight_Wave_Element *pMoveInfo)
{
    if (pMoveInfo)
    {
        m_tMoveInfo = (*pMoveInfo);
    }
}