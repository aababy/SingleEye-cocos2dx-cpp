//
//  EnemyStayState.h
//  SingleEye
//
//  Created by qinfen on 13-11-27.
//
//

#ifndef __SingleEye__EnemyStayState__
#define __SingleEye__EnemyStayState__

#include "GameInclude.h"

class EnemyStayState : public State<Enemy>
{
public:
    EnemyStayState(){};
    ~EnemyStayState(){};
    
    static EnemyStayState* getInstance();
    
    virtual void Enter(Enemy *pRole);
    virtual void Exit(Enemy *pRole);
    virtual void Update(Enemy *pRole){};
    virtual bool HandleMsg(Enemy *pRole, const Message &msg){return false;};
    
    void setMoveInfo(const Fight_Wave_Element *pMoveInfo);  //设置移动信息 目前只有坐标
private:
    
    Fight_Wave_Element m_tMoveInfo;  // 移动信息
    void createStayAction(Enemy *pRole);
};

#endif /* defined(__SingleEye__EnemyStayState__) */
