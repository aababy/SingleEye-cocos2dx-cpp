//
//  EnemyInkJetState.cpp
//  SingleEye
//
//  Created by qinfen on 13-12-16.
//
//

#include "EnemyInkJetState.h"

static EnemyInkJetState *instance = NULL;

EnemyInkJetState* EnemyInkJetState::getInstance()
{
    if (!instance)
    {
        instance = new EnemyInkJetState;
    }
    return instance;
}

void EnemyInkJetState::Enter(Enemy *pRole)
{
  if(pRole->getType() == Enemy_State_Atk||pRole->getState()== Enemy_State_Die)
      return;
    pRole->setState(Enemy_State_Atk);
    createInkJetAction(pRole);
}

void EnemyInkJetState::createInkJetAction(Enemy *pRole)
{
    pRole->getSprite()->getAnimation()->play("penmo");
    pRole->getSprite()->getAnimation()->setMovementEventCallFunc(pRole,movementEvent_selector(Enemy::animationEvent));
    pRole->getSprite()->getAnimation()->setFrameEventCallFunc(pRole, frameEvent_selector(Enemy::frameEvent));
}

void EnemyInkJetState::Exit(Enemy *pRole)
{
    
}

void EnemyInkJetState::setMoveInfo(const Fight_Wave_Element *pMoveInfo)
{
    if (pMoveInfo)
    {
        m_tMoveInfo = (*pMoveInfo);
    }
}
