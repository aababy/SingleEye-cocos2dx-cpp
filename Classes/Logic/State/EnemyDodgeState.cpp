//
//  EnemyDodgeState.cpp
//  SingleEye
//
//  Created by qinfen on 13-12-11.
//
//
#define DodgeDistance 150

#include "EnemyDodgeState.h"

static EnemyDodgeState *instance = NULL;

EnemyDodgeState* EnemyDodgeState::getInstance()
{
    if (!instance)
    {
        instance = new EnemyDodgeState;
    }
    return instance;
}

void EnemyDodgeState::Enter(Enemy *pRole)
{
    if (pRole->getState() == Enemy_State_Dodge||pRole->getState() == Enemy_State_Stop ||
        pRole->getState() == Enemy_State_SlowDown||pRole->getState()== Enemy_State_Die)
    {
        return;
    }
    pRole->setState(Enemy_State_Dodge);
    CCAction *action =  pRole->getActionByTag(1);
    if (action)
    {
        if (action->isDone())
        {
            pRole->stopUpdate();
            createDodgeAction(pRole);
        }
    }
    else
    {
        pRole->stopUpdate();
        createDodgeAction(pRole);
    }
    
    
}

void EnemyDodgeState::createDodgeAction(Enemy *pRole)
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    float dx = DodgeDistance;
    if(!m_bLeft)
    {
        dx *= -1;
    }
    float spritewidth = pRole->getSprite()->getContentSize().width;
    if (pRole->getPositionX()+dx>size.width - spritewidth/2||pRole->getPositionX()+dx<spritewidth/2)
    {
        dx*=-1;
    }
    CCMoveBy *moveBy = CCMoveBy::create(0.2, ccp(dx, 0));
    CCActionInterval *moveByBack = moveBy->reverse();
    CCSequence *seq = CCSequence::create(moveBy,
                                         moveByBack,
                                         CCCallFuncN::create(pRole, callfuncN_selector(EnemyDodgeState::DodgeCallBack)),
                                         NULL);
    seq->setTag(1);
    pRole->runAction(seq);
}

void EnemyDodgeState::Exit(Enemy *pRole)
{
    
}

void EnemyDodgeState::setMoveInfo(const Fight_Wave_Element *pMoveInfo)
{
    if (pMoveInfo)
    {
        m_tMoveInfo = (*pMoveInfo);
    }
}

void EnemyDodgeState::DodgeCallBack(CCObject *pSender)
{
    Enemy *enemy = (Enemy*)pSender;
//    if(enemy->getPositionY()<=enemy->getAtkPos())
//    {
//        enemy->setPosition(ccp(enemy->getPositionX(), enemy->getAtkPos()));
//    }
//    else
//    {
//        EnemyMoveState *_instance = EnemyMoveState::getInstance();
//        enemy->GetFSMSystem()->ChangeState(_instance);
//    }
    EnemyStayState *_instance = EnemyStayState::getInstance();
    enemy->GetFSMSystem()->ChangeState(_instance);
}






