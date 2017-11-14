//
//  EnemyAtkState.cpp
//  SingleEye
//
//  Created by qinfen on 13-11-15.
//
//

#include "EnemyAtkState.h"

static EnemyAtkState *instance = NULL;
USING_NS_CC_EXT;

EnemyAtkState* EnemyAtkState::getInstance()
{
    if (!instance)
    {
        instance = new EnemyAtkState;
    }
    return instance;
}

void EnemyAtkState::Enter(Enemy *pRole)
{
    if (pRole->getState() == Enemy_State_Atk||pRole->getState()== Enemy_State_Die)
    {
        return;
    }
    
    pRole->setState(Enemy_State_Atk);
    pRole->stopUpdate();
    pRole->stopSkillUpdate();
    createAtkAction(pRole);
//    pRole->setCurrentState(this);
}

void EnemyAtkState::createAtkAction(Enemy *pRole)
{
    if(pRole->getType() == Enemy_Type_iBoss)
    {
        pRole->getSprite()->getAnimation()->play("gongji_1");
    }
    else if(pRole->getType() == Enemy_Type_jBoss)
    {
        pRole->getSprite()->getAnimation()->play("gongji_2");
    }
    else
    {
        pRole->getSprite()->getAnimation()->play("gongji");
    }
    
    pRole->getSprite()->getAnimation()->setMovementEventCallFunc(pRole,movementEvent_selector(Enemy::animationEvent));
    pRole->getSprite()->getAnimation()->setFrameEventCallFunc(pRole, frameEvent_selector(Enemy::frameEvent));
}

void EnemyAtkState::Exit(Enemy *pRole)
{
    
}

void EnemyAtkState::setMoveInfo(const Fight_Wave_Element *pMoveInfo)
{
    if (pMoveInfo)
    {
        m_tMoveInfo = (*pMoveInfo);
    }
}
