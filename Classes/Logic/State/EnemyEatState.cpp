//
//  EnemyEatState.cpp
//  SingleEye
//
//  Created by qinfen on 14-1-7.
//
//

#include "EnemyEatState.h"

static EnemyEatState *instance = NULL;

EnemyEatState* EnemyEatState::getInstance()
{
    if (!instance)
    {
        instance = new EnemyEatState;
    }
    return instance;
}

void EnemyEatState::Enter(Enemy *pRole)
{
    if(pRole->getType() == Enemy_State_Eat||pRole->getState()== Enemy_State_Die)
    return;
    pRole->stopUpdate();
    pRole->stopSkillUpdate();
    pRole->setState(Enemy_State_Eat);
    createEatAction(pRole);
}

void EnemyEatState::createEatAction(Enemy *pRole)
{
    pRole->getSprite()->getAnimation()->play("yao");
    pRole->getSprite()->getAnimation()->setMovementEventCallFunc(pRole, movementEvent_selector(Enemy::animationEvent));
}

void EnemyEatState::Exit(Enemy *pRole)
{
    
}

void EnemyEatState::setMoveInfo(const Fight_Wave_Element *pMoveInfo)
{
    if (pMoveInfo)
    {
        m_tMoveInfo = (*pMoveInfo);
    }
}