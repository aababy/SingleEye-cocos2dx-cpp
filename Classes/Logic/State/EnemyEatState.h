//
//  EnemyEatState.h
//  SingleEye
//
//  Created by qinfen on 14-1-7.
//
//
/**
 *  吃子弹
 */
#ifndef __SingleEye__EnemyEatState__
#define __SingleEye__EnemyEatState__

#include "GameInclude.h"

class EnemyEatState : public State<Enemy>
{
public:
    EnemyEatState(){};
    ~EnemyEatState(){};
    
    static EnemyEatState* getInstance();
    
    virtual void Enter(Enemy *pRole);
    virtual void Exit(Enemy *pRole);
    virtual void Update(Enemy *pRole){};
    virtual bool HandleMsg(Enemy *pRole, const Message &msg){return false;};
    
    void setMoveInfo(const Fight_Wave_Element *pMoveInfo);  //设置移动信息 目前只有坐标
private:
    Fight_Wave_Element m_tMoveInfo;  // 移动信息
    void createEatAction(Enemy *pRole);
};

#endif /* defined(__SingleEye__EnemyEatState__) */
