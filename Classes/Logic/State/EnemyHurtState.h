//
//  EnemyHurtState.h
//  SingleEye
//
//  Created by qinfen on 13-11-15.
//
//

#ifndef __SingleEye__EnemyHurtState__
#define __SingleEye__EnemyHurtState__

#include "GameInclude.h"
class Enemy;

class EnemyHurtState : public State<Enemy>
{
public:
    EnemyHurtState(){};
    ~EnemyHurtState(){};
    
    static EnemyHurtState* getInstance();
    
    virtual void Enter(Enemy *pRole);
    virtual void Exit(Enemy *pRole);
    virtual void Update(Enemy *pRole){};
    virtual bool HandleMsg(Enemy *pRole, const Message &msg){return false;};
    
    void setMoveInfo(const Fight_Wave_Element *pMoveInfo);  //设置移动信息 目前只有坐标
    void setHurt(int hurt);
private:
    
    Fight_Wave_Element m_tMoveInfo;  // 移动信息
    void createHurtAction(Enemy *pRole);
    int m_nAtk;   //攻击方的攻击力
};

#endif /* defined(__SingleEye__EnemyHurtState__) */
