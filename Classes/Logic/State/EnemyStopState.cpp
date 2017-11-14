//
//  EnemyStopState.cpp
//  SingleEye
//
//  Created by qinfen on 13-11-29.
//
//

#include "EnemyStopState.h"

static EnemyStopState *instance = NULL;


EnemyStopState* EnemyStopState::getInstance()
{
    if (!instance)
    {
        instance = new EnemyStopState;
    }
    return instance;
}

void EnemyStopState::Enter(Enemy *pRole)
{
    if(pRole->getState()== Enemy_State_Die)//||pRole->getState() == Enemy_State_SlowDown)
        return;
    if(pRole->getState() == Enemy_State_DrillStay&&pRole->getType()==Enemy_Type_BingBaoHeTun)
        return;
    if(pRole->getState() == Enemy_State_Stop)
    {
        if(m_eType == Stop_Type_Frozen)  //冰冻
        {
            pRole->playFrozenEffectAnimation();
        }
        else if(m_eType == Stop_Type_Bondage)
        {
            pRole->playBondageEffectAnimation();
        }
        else  //沉睡
        {
            pRole->playSleepEffectAnimation();
        }
        createDelayAction(pRole);
        return;
    }
    
    pRole->getSprite()->stopAllActions();
    pRole->stopAllActions();
    pRole->setState(Enemy_State_Stop);
    pRole->stopUpdate();
    pRole->stopEffectAnimation();
    pRole->stopSkillUpdate();
    createStopAction(pRole);
    pRole->setCurrentState(this);
    createDelayAction(pRole);
}

void EnemyStopState::createStopAction(Enemy *pRole)
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
    
    pRole->getSprite()->getAnimation()->setMovementEventCallFunc(pRole, movementEvent_selector(Enemy::animationEvent));
    if(m_eType == Stop_Type_Frozen)  //冰冻
    {
        pRole->playFrozenEffectAnimation();
    }
    else if(m_eType == Stop_Type_Bondage)
    {
        pRole->playBondageEffectAnimation();
    }
    else  //沉睡
    {
        pRole->playSleepEffectAnimation();
    }
}

void EnemyStopState::createDelayAction(Enemy *pRole)
{
//    CCSequence *_seq = CCSequence::create(
//                                          CCDelayTime::create(m_fTime),
//                                          CCCallFuncND::create(
//                                                               NULL,
//                                                               callfuncND_selector(EnemyStopState::stopEndCallback),pRole),
//                                          NULL);
//    return _seq;
    pRole->createStopDelayAction(m_fTime);
}

void EnemyStopState::Exit(Enemy *pRole)
{
    
}

void EnemyStopState::setMoveInfo(const Fight_Wave_Element *pMoveInfo)
{
    if (pMoveInfo)
    {
        m_tMoveInfo = (*pMoveInfo);
    }
}

void EnemyStopState::stopEndCallback(cocos2d::CCNode *pSender, void *pData)
{
//    Enemy *_enemy = (Enemy*)pData;
//    _enemy->enemyStop();
//    
//    if(_enemy->getPositionY()<=Atk_Barrier_DY+Barrier_Y)
//    {
//        EnemyStayState *_instance = EnemyStayState::getInstance();
//        _enemy->GetFSMSystem()->ChangeState(_instance);
//    }
//    else
//    {
//        EnemyMoveState *_instance = EnemyMoveState::getInstance();
//        _enemy->GetFSMSystem()->ChangeState(_instance);
//    }
}

void EnemyStopState::setTime(float time ,Stop_Type type)
{
    m_fTime = time;
    m_eType = type;
}