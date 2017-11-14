//
//  EnemyDrillState.cpp
//  SingleEye
//
//  Created by qinfen on 13-11-29.
//
//

#include "EnemyDrillState.h"
USING_NS_CC_EXT;

static EnemyDrillState *instance = NULL;

EnemyDrillState* EnemyDrillState::getInstance()
{
    if (!instance)
    {
        instance = new EnemyDrillState;
    }
    return instance;
}

void EnemyDrillState::Enter(Enemy *pRole)
{
    if(pRole->getState()== Enemy_State_Die)
        return;
    pRole->stopUpdate();
    createDrillAction(pRole);
}

void EnemyDrillState::createDrillAction(Enemy *pRole)
{
    pRole->getSprite()->getAnimation()->play("zuanjin");
    pRole->getSprite()->getAnimation()->setMovementEventCallFunc(pRole,movementEvent_selector(Enemy::animationEvent));
}

void EnemyDrillState::Exit(Enemy *pRole)
{
    
}

void EnemyDrillState::setMoveInfo(const Fight_Wave_Element *pMoveInfo)
{
    if (pMoveInfo)
    {
        m_tMoveInfo = (*pMoveInfo);
    }
}






