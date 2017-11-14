//
//  EnemyTransferState.h
//  SingleEye
//
//  Created by qinfen on 13-12-25.
//
//

#ifndef __SingleEye__EnemyTransferState__
#define __SingleEye__EnemyTransferState__
/**
 *  boss 传送门
 */
#include "GameInclude.h"

class EnemyTransferState : public State<Enemy>
{
public:
    EnemyTransferState(){};
    ~EnemyTransferState(){};
    
    static EnemyTransferState* getInstance();
    
    virtual void Enter(Enemy *pRole);
    virtual void Exit(Enemy *pRole);
    virtual void Update(Enemy *pRole){};
    virtual bool HandleMsg(Enemy *pRole, const Message &msg){return false;};
    
    void setMoveInfo(const Fight_Wave_Element *pMoveInfo);  //设置移动信息 目前只有坐标
private:
    Fight_Wave_Element m_tMoveInfo;  // 移动信息
    void createTransferAction(Enemy *pRole);
};


#endif /* defined(__SingleEye__EnemyTransferState__) */
