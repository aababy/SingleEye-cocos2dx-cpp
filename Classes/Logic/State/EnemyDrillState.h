//
//  EnemyDrillState.h
//  SingleEye
//
//  Created by qinfen on 13-11-29.
//
//

#ifndef __SingleEye__EnemyDrillState__
#define __SingleEye__EnemyDrillState__
/**
 *  软泥怪 钻入状态
 */
#include "GameInclude.h"

class EnemyDrillState : public State<Enemy>
{
public:
    EnemyDrillState(){};
    ~EnemyDrillState(){};
    
    static EnemyDrillState* getInstance();
    
    virtual void Enter(Enemy *pRole);
    virtual void Exit(Enemy *pRole);
    virtual void Update(Enemy *pRole){};
    virtual bool HandleMsg(Enemy *pRole, const Message &msg){return false;};
    
    void setMoveInfo(const Fight_Wave_Element *pMoveInfo);  //设置移动信息 目前只有坐标
private:
    Fight_Wave_Element m_tMoveInfo;  // 移动信息
    void createDrillAction(Enemy *pRole);
};

#endif /* defined(__SingleEye__EnemyDrillState__) */
