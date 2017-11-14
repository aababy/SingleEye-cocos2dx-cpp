//
//  EnemyJumpState.h
//  SingleEye
//
//  Created by qinfen on 13-11-27.
//
//

#ifndef __SingleEye__EnemyJumpState__
#define __SingleEye__EnemyJumpState__

#include "GameInclude.h"

class EnemyJumpState : public State<Enemy>
{
public:
    EnemyJumpState(){};
    ~EnemyJumpState(){};
    
    static EnemyJumpState* getInstance();
    
    virtual void Enter(Enemy *pRole);
    virtual void Exit(Enemy *pRole);
    virtual void Update(Enemy *pRole){};
    virtual bool HandleMsg(Enemy *pRole, const Message &msg){return false;};
    
    void setMoveInfo(const Fight_Wave_Element *pMoveInfo);  //设置移动信息 目前只有坐标
private:
    
    Fight_Wave_Element m_tMoveInfo;  // 移动信息
    void createJumpAction(Enemy *pRole);
    void jumpCallBack(CCNode* pSender);
};
#endif /* defined(__SingleEye__EnemyJumpState__) */
