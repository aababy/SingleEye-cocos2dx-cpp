//
//  EnemyPoisonState.cpp
//  SingleEye
//
//  Created by qinfen on 13-11-29.
//
//

#include "EnemyPoisonState.h"

USING_NS_CC_EXT;

static EnemyPoisonState *instance = NULL;

EnemyPoisonState* EnemyPoisonState::getInstance()
{
    if (!instance)
    {
        instance = new EnemyPoisonState;
    }
    return instance;
}

void EnemyPoisonState::Enter(Enemy *pRole)
{
    if(pRole->getState()== Enemy_State_Die)
        return;
    pRole->setState(Enemy_State_Stay);
    createPoisonAction(pRole);
}

void EnemyPoisonState::createPoisonAction(Enemy *pRole)
{
    CCActionInterval *_action = CCTintBy::create(0.5f, -155, 0, 0);
    CCActionInterval *_actionBack = _action->reverse();
    pRole->getSprite()->runAction(CCSequence::create(_action,
                                                     CCDelayTime::create(PosionTime-1),
                                                     _actionBack,
                                                     NULL
                                  ));
}

void EnemyPoisonState::Exit(Enemy *pRole)
{
    
}

void EnemyPoisonState::setMoveInfo(const Fight_Wave_Element *pMoveInfo)
{
    if (pMoveInfo)
    {
        m_tMoveInfo = (*pMoveInfo);
    }
}