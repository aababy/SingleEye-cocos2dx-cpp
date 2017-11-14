//
//  EnemyDrillOutStayStateState.h
//  SingleEye
//
//  Created by qinfen on 13-12-3.
//
//

#ifndef __SingleEye__EnemyDrillOutStayState__
#define __SingleEye__EnemyDrillOutStayState__
/**
 *  软泥怪 钻出状态
 */
#include "GameInclude.h"

class EnemyDrillOutStayState : public State<Enemy>
{
public:
    EnemyDrillOutStayState(){};
    ~EnemyDrillOutStayState(){};
    
    static EnemyDrillOutStayState* getInstance();
    
    virtual void Enter(Enemy *pRole);
    virtual void Exit(Enemy *pRole);
    virtual void Update(Enemy *pRole){};
    virtual bool HandleMsg(Enemy *pRole, const Message &msg){return false;};
    
    void setMoveInfo(const Fight_Wave_Element *pMoveInfo);  //设置移动信息 目前只有坐标
private:
    Fight_Wave_Element m_tMoveInfo;  // 移动信息
    void createDrillOutAction(Enemy *pRole);
};

#endif /* defined(__SingleEye__EnemyDrillOutStayState__) */
