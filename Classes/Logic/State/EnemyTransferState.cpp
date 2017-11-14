//
//  EnemyTransferState.cpp
//  SingleEye
//
//  Created by qinfen on 13-12-25.
//
//

#include "EnemyTransferState.h"

static EnemyTransferState *instance = NULL;

EnemyTransferState* EnemyTransferState::getInstance()
{
    if (!instance)
    {
        instance = new EnemyTransferState;
    }
    return instance;
}

void EnemyTransferState::Enter(Enemy *pRole)
{
    if(pRole->getType() == Enemy_State_Atk||pRole->getState()== Enemy_State_Die)
        return;
    pRole->setState(Enemy_State_Atk);
    createTransferAction(pRole);
}

void EnemyTransferState::createTransferAction(Enemy *pRole)
{
    pRole->getSprite()->getAnimation()->play("gongji");
    pRole->getSprite()->getAnimation()->setFrameEventCallFunc(pRole, frameEvent_selector(Enemy::frameEvent));
}

void EnemyTransferState::Exit(Enemy *pRole)
{
    
}

void EnemyTransferState::setMoveInfo(const Fight_Wave_Element *pMoveInfo)
{
    if (pMoveInfo)
    {
        m_tMoveInfo = (*pMoveInfo);
    }
}