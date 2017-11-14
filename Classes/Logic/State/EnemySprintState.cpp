//
//  EnemySprintState.cpp
//  SingleEye
//
//  Created by qinfen on 13-12-10.
//
//

#include "EnemySprintState.h"

USING_NS_CC_EXT;
#define Sprint_Distance 100

static EnemySprintState *instance = NULL;

EnemySprintState* EnemySprintState::getInstance()
{
    if (!instance)
    {
        instance = new EnemySprintState;
    }
    return instance;
}

void EnemySprintState::Enter(Enemy *pRole)
{
    if(pRole->getState() ==  Enemy_State_Sprint||pRole->getState()== Enemy_State_Die)
        return;
    if (pRole->getPositionY()-Sprint_Distance <=Atk_Barrier_DY+Barrier_Y)
    {
        pRole->setUseCDSkill(false);
        return;
    }
    pRole->setState(Enemy_State_Sprint);
    pRole->stopUpdate();
    pRole->stopSkillUpdate();
    if (pRole->getPositionY()-Sprint_Distance <=Atk_Barrier_DY+Barrier_Y)
    {
        pRole->setUseCDSkill(false);
        return;
    }
    createSprintAction(pRole);
}

void EnemySprintState::createSprintAction(Enemy *pRole)
{
    pRole->getSprite()->getAnimation()->play("chongci");   ////
    pRole->getSprite()->getAnimation()->setMovementEventCallFunc(pRole,movementEvent_selector(Enemy::animationEvent));
    DATAPOOL->playGameEffect(Sound_ChongCi);
    CCMoveBy *moveBy = CCMoveBy::create(0.5f, ccp(0, -Sprint_Distance));
    CCSequence *_seq = CCSequence::create(CCDelayTime::create(0.2),
                                          moveBy,
                                          CCCallFuncN::create(pRole,callfuncN_selector(EnemySprintState::sprintCallBack)),
                                          NULL);
    pRole->runAction(_seq);
}

void EnemySprintState::Exit(Enemy *pRole)
{
    
}

void EnemySprintState::setMoveInfo(const Fight_Wave_Element *pMoveInfo)
{
    if (pMoveInfo)
    {
        m_tMoveInfo = (*pMoveInfo);
    }
}

void EnemySprintState::sprintCallBack(CCObject *pSender)
{
    Enemy *enemy = (Enemy*)pSender;
    EnemyMoveState *_instance = EnemyMoveState::getInstance();
    enemy->GetFSMSystem()->ChangeState(_instance);
}