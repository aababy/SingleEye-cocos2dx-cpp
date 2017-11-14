//
//  EnemyDrillStayState.cpp
//  SingleEye
//
//  Created by qinfen on 13-12-3.
//
//

#include "EnemyDrillStayState.h"

USING_NS_CC_EXT;

static EnemyDrillStayState *instance = NULL;

EnemyDrillStayState* EnemyDrillStayState::getInstance()
{
    if (!instance)
    {
        instance = new EnemyDrillStayState;
    }
    return instance;
}

void EnemyDrillStayState::Enter(Enemy *pRole)
{
    if(pRole->getState()== Enemy_State_Die)
        return;
    pRole->setState(Enemy_State_DrillStay);
    pRole->stopUpdate();
    pRole->stopSkillUpdate();
    int type = pRole->getType();
    if (type==Enemy_Type_BingBaoHeTun || type == Enemy_Type_HuangMoGeBuLin)
    {
      pRole->schedule(schedule_selector(Enemy::moveUpdate), pRole->getVel()/1000.0);
    }
    createDrillStayAction(pRole);
    pRole->runAction(createDelayAction(pRole));
}

void EnemyDrillStayState::createDrillStayAction(Enemy *pRole)
{
    pRole->getSprite()->getAnimation()->play("zuanjindaiji");
}

void EnemyDrillStayState::Exit(Enemy *pRole)
{
    
}

void EnemyDrillStayState::setMoveInfo(const Fight_Wave_Element *pMoveInfo)
{
    if (pMoveInfo)
    {
        m_tMoveInfo = (*pMoveInfo);
    }
}

CCFiniteTimeAction* EnemyDrillStayState::createDelayAction(Enemy* pRole)
{
    float time =0;
    if(pRole->getType() == Enemy_Type_RuanNiGuai||pRole->getType() == Enemy_Type_XueYuGangXie)
    {
        time = 2.0;
    }
    else if(pRole->getType() == Enemy_Type_HuangMoGeBuLin || pRole->getType() == Enemy_Type_BingBaoHeTun)
    {
        time = 4.0;
    }
    else if(pRole->getType() == Enemy_Type_hBoss)
    {
        time = 6.0;
    }
    CCSequence *_seq = CCSequence::create(CCDelayTime::create(time),
                                          CCCallFuncND::create(
                                                               NULL,
                                                               callfuncND_selector(EnemyDrillStayState::drillStayCallBack),pRole),
                                          NULL);
    return _seq;
}

void EnemyDrillStayState::drillStayCallBack(cocos2d::CCNode *pSender, void *pData)
{
    Enemy *enemy = (Enemy*)pData;
    EnemyDrillOutStayState *_instance = EnemyDrillOutStayState::getInstance();
    enemy->GetFSMSystem()->ChangeState(_instance);
}
