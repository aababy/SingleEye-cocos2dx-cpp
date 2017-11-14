//
//  EnemyMoveState.h
//  SingleEye
//
//  Created by qinfen on 13-11-15.
//
//

#ifndef __SingleEye__EnemyMoveState__
#define __SingleEye__EnemyMoveState__

#include "GameInclude.h"
USING_NS_CC;
class Enemy;
class EnemyMoveState : public State<Enemy>
{
public:
    EnemyMoveState(){};
    ~EnemyMoveState(){};
    
    static EnemyMoveState* getInstance();
    
    virtual void Enter(Enemy *pRole);
    virtual void Exit(Enemy *pRole);
    virtual void Update(Enemy *pRole){};
    virtual bool HandleMsg(Enemy *pRole, const Message &msg){return false;};
    
    void setMoveInfo(const Fight_Wave_Element *pMoveInfo);  //设置移动信息 目前只有坐标
    
private:
    Fight_Wave_Element m_tMoveInfo;  // 移动信息
    void createMoveAction(Enemy *pRole);
};

#endif /* defined(__SingleEye__EnemyMoveState__) */
