//
//  EnemySprintState.h
//  SingleEye
//
//  Created by qinfen on 13-12-10.
//
//

#ifndef __SingleEye__EnemySprintState__
#define __SingleEye__EnemySprintState__
/**
 *  冲刺
 */
#include "GameInclude.h"

class EnemySprintState : public State<Enemy>
{
public:
    EnemySprintState(){};
    ~EnemySprintState(){};
    
    static EnemySprintState* getInstance();
    
    virtual void Enter(Enemy *pRole);
    virtual void Exit(Enemy *pRole);
    virtual void Update(Enemy *pRole){};
    virtual bool HandleMsg(Enemy *pRole, const Message &msg){return false;};
    
    void setMoveInfo(const Fight_Wave_Element *pMoveInfo);  //设置移动信息 目前只有坐标
private:
    Fight_Wave_Element m_tMoveInfo;  // 移动信息
    void createSprintAction(Enemy *pRole);
    void sprintCallBack(CCObject *pSender);
};

#endif /* defined(__SingleEye__EnemySprintState__) */
