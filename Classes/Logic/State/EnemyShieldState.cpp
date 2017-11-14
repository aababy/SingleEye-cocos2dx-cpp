//
//  EnemyShieldState.cpp
//  SingleEye
//
//  Created by qinfen on 13-12-17.
//
//

#include "EnemyShieldState.h"

static EnemyShieldState *instance = NULL;

EnemyShieldState* EnemyShieldState::getInstance()
{
    if (!instance)
    {
        instance = new EnemyShieldState;
    }
    return instance;
}

void EnemyShieldState::Enter(Enemy *pRole)
{
    if(pRole->getType() == Enemy_State_Shield||pRole->getState()== Enemy_State_Die)
        return;
    pRole->setState(Enemy_State_Shield);
    createShieldAction(pRole);
}

void EnemyShieldState::createShieldAction(Enemy *pRole)
{
    pRole->playShieldAnimation();
}

void EnemyShieldState::Exit(Enemy *pRole)
{
    
}

void EnemyShieldState::setMoveInfo(const Fight_Wave_Element *pMoveInfo)
{
    if (pMoveInfo)
    {
        m_tMoveInfo = (*pMoveInfo);
    }
}