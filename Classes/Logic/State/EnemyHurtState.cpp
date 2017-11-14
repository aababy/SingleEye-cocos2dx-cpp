//
//  EnemyHurtState.cpp
//  SingleEye
//
//  Created by qinfen on 13-11-15.
//
//

#include "EnemyHurtState.h"
USING_NS_CC_EXT;
static EnemyHurtState *instance = NULL;


EnemyHurtState* EnemyHurtState::getInstance()
{
    if (!instance)
    {
        instance = new EnemyHurtState;
    }
    return instance;
}

void EnemyHurtState::Enter(Enemy *pRole)
{
    if (pRole->getState() == Enemy_State_UnderAtk||
        pRole->getState()== Enemy_State_Die||
        pRole->getState()==Enemy_State_MuBei
        )
    {
        return;
    }
    
    if (pRole->isBoss()&&pRole->getState()==Enemy_State_Atk)return;  //boss攻击不被打断。
    
    pRole->setState(Enemy_State_UnderAtk);
    pRole->stopUpdate();
    if(pRole->getType()!=Enemy_Type_aBoss&&pRole->getType()!=Enemy_Type_bBoss&&
       pRole->getType()!=Enemy_Type_cBoss&&pRole->getType()!=Enemy_Type_dBoss)
    {
        pRole->stopAllActions();
    }
    createHurtAction(pRole);
}

void EnemyHurtState::createHurtAction(Enemy *pRole)
{
    int index = arc4random()%4;
    index++;
    char buf[30] = {0};
    if(pRole->isBoss())
    {
        sprintf(buf, "Sound/%d_hurt.ogg",pRole->getType());
    }
    else
    {
        sprintf(buf, "Sound/hurt%d.ogg",index);
    }
    DATAPOOL->playGameEffect(buf);
    pRole->getSprite()->getAnimation()->stop();
    if(pRole->getType() == Enemy_Type_iBoss)
    {
        pRole->getSprite()->getAnimation()->play("shoushang_1");
    }
    else if(pRole->getType() == Enemy_Type_jBoss)
    {
        pRole->getSprite()->getAnimation()->play("shoushang_2");
    }
    else
    {
        pRole->getSprite()->getAnimation()->play("shoushang");
    }
    pRole->getSprite()->getAnimation()->setMovementEventCallFunc(pRole,movementEvent_selector(Enemy::animationEvent));
}

void EnemyHurtState::Exit(Enemy *pRole)
{
    
}

void EnemyHurtState::setHurt(int hurt)
{
    m_nAtk = hurt;
}

void EnemyHurtState::setMoveInfo(const Fight_Wave_Element *pMoveInfo)
{
    if (pMoveInfo)
    {
        m_tMoveInfo = (*pMoveInfo);
    }
}
