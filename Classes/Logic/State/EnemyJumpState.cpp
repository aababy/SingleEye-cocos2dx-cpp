//
//  EnemyJumpState.cpp
//  SingleEye
//
//  Created by qinfen on 13-11-27.
//
//

#include "EnemyJumpState.h"
USING_NS_CC_EXT;
#define Max_jump_Distance 150


static EnemyJumpState *instance = NULL;

EnemyJumpState* EnemyJumpState::getInstance()
{
    if (!instance)
    {
        instance = new EnemyJumpState;
    }
    return instance;
}

void EnemyJumpState::Enter(Enemy *pRole)
{
    if(pRole->getState()== Enemy_State_Die)
        return;
    pRole->setState(Enemy_State_Jump);
    pRole->stopUpdate();
    createJumpAction(pRole);
    pRole->setCurrentState(this);
}

void EnemyJumpState::createJumpAction(Enemy *pRole)
{
    pRole->setZOrder(Fight_Zorder_Up);
    pRole->getSprite()->getAnimation()->play("tiaoyue");
    pRole->getSprite()->getAnimation()->setMovementEventCallFunc(pRole,movementEvent_selector(Enemy::animationEvent));
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    float _width = pRole->getSprite()->getContentSize().width;
    float _x = 0.0;
    float _y = 0.0;
    float _current_x = pRole->getPositionX();
    float _current_y = pRole->getPositionY();
    float _dx = 0.0;
    float _dy = 0.0;
    do {
        _x = (arc4random()%(int)(size.width-_width)) ;
        _y = arc4random()%(int)(size.height - Barrier_Y - (size.height/2-Max_Jump_Y_Center));
        _x += _width/2;
        _y += Barrier_Y;
        _dx = _current_x - _x;
        _dy = _current_y - _y;
    } while (sqrtf(powf(_dx, 2.0)+powf(_dy, 2.0))>=Max_jump_Distance);
    
    CCMoveTo *_moveto = CCMoveTo::create(0.63f, ccp(_x,_y));
    CCSequence *_seq = CCSequence::create(CCDelayTime::create(0.12f),_moveto,
                                          CCCallFuncN::create(pRole, callfuncN_selector(EnemyJumpState::jumpCallBack)),
                                          NULL);
    pRole->runAction(_seq);
}



void EnemyJumpState::Exit(Enemy *pRole)
{
    
}

void EnemyJumpState::setMoveInfo(const Fight_Wave_Element *pMoveInfo)
{
    if (pMoveInfo)
    {
        m_tMoveInfo = (*pMoveInfo);
    }
}

void EnemyJumpState::jumpCallBack(cocos2d::CCNode *pSender)
{
    Enemy* enemy = (Enemy*)pSender;
    enemy->setZOrder(Fight_Zorder_Enemy);
}