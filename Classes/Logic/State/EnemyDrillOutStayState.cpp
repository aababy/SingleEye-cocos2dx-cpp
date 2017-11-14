//
//  EnemyDrillOutStayState.cpp
//  SingleEye
//
//  Created by qinfen on 13-12-3.
//
//

#include "EnemyDrillOutStayState.h"

USING_NS_CC_EXT;

static EnemyDrillOutStayState *instance = NULL;

EnemyDrillOutStayState* EnemyDrillOutStayState::getInstance()
{
    if (!instance)
    {
        instance = new EnemyDrillOutStayState;
    }
    return instance;
}

void EnemyDrillOutStayState::Enter(Enemy *pRole)
{
    if(pRole->getState()== Enemy_State_Die)
        return;
//    pRole->setState(Enemy_State_DrillOut);
    pRole->stopUpdate();
    pRole->closeDlg();
    createDrillOutAction(pRole);
}

void EnemyDrillOutStayState::createDrillOutAction(Enemy *pRole)
{
    pRole->getSprite()->getAnimation()->play("zuanchu");
    pRole->getSprite()->getAnimation()->setMovementEventCallFunc(pRole,movementEvent_selector(Enemy::animationEvent));
    pRole->getSprite()->getAnimation()->setFrameEventCallFunc(pRole, frameEvent_selector(Enemy::frameEvent));
}

void EnemyDrillOutStayState::Exit(Enemy *pRole)
{
    
}

void EnemyDrillOutStayState::setMoveInfo(const Fight_Wave_Element *pMoveInfo)
{
    if (pMoveInfo)
    {
        m_tMoveInfo = (*pMoveInfo);
    }
}
