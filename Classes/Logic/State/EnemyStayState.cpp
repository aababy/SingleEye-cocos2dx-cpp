//
//  EnemyStayState.cpp
//  SingleEye
//
//  Created by qinfen on 13-11-27.
//
//

#include "EnemyStayState.h"
USING_NS_CC_EXT;

static EnemyStayState *instance = NULL;

EnemyStayState* EnemyStayState::getInstance()
{
    if (!instance)
    {
        instance = new EnemyStayState;
    }
    return instance;
}

void EnemyStayState::Enter(Enemy *pRole)
{
    if(pRole->getState()== Enemy_State_Die)
        return;
    pRole->setState(Enemy_State_Stay);
    if(pRole->getType()!=Enemy_Type_FengXiYi)
        pRole->stopUpdate();
    pRole->stopSkillUpdate();
    createStayAction(pRole);
    pRole->setCurrentState(this);
}

void EnemyStayState::createStayAction(Enemy *pRole)
{
    if(pRole->getType() == Enemy_Type_iBoss)
    {
        pRole->getSprite()->getAnimation()->play("daiji_1");
    }
    else if(pRole->getType() == Enemy_Type_jBoss)
    {
        pRole->getSprite()->getAnimation()->play("daiji_2");
    }
    else
    {
        pRole->getSprite()->getAnimation()->play("daiji");
    }
    pRole->getSprite()->getAnimation()->setMovementEventCallFunc(pRole,movementEvent_selector(Enemy::animationEvent));
}

void EnemyStayState::Exit(Enemy *pRole)
{
    
}

void EnemyStayState::setMoveInfo(const Fight_Wave_Element *pMoveInfo)
{
    if (pMoveInfo)
    {
        m_tMoveInfo = (*pMoveInfo);
    }
}