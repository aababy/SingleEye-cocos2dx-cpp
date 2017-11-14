//
//  EnemyShieldState.h
//  SingleEye
//
//  Created by qinfen on 13-12-17.
//
//

#ifndef __SingleEye__EnemyShieldState__
#define __SingleEye__EnemyShieldState__
/**
 *  护盾
 */
#include "GameInclude.h"


class EnemyShieldState : public State<Enemy>
{
public:
    EnemyShieldState(){};
    ~EnemyShieldState(){};
    
    static EnemyShieldState* getInstance();
    
    virtual void Enter(Enemy *pRole);
    virtual void Exit(Enemy *pRole);
    virtual void Update(Enemy *pRole){};
    virtual bool HandleMsg(Enemy *pRole, const Message &msg){return false;};
    
    void setMoveInfo(const Fight_Wave_Element *pMoveInfo);  //设置移动信息 目前只有坐标
private:
    Fight_Wave_Element m_tMoveInfo;  // 移动信息
    void createShieldAction(Enemy *pRole);
};


#endif /* defined(__SingleEye__EnemyShieldState__) */
