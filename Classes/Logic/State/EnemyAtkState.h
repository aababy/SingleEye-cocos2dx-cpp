//
//  EnemyAtkState.h
//  SingleEye
//
//  Created by qinfen on 13-11-15.
//
//

#ifndef __SingleEye__EnemyAtkState__
#define __SingleEye__EnemyAtkState__

#include "GameInclude.h"


class EnemyAtkState : public State<Enemy>
{
public:
    EnemyAtkState(){};
    ~EnemyAtkState(){};
    
    static EnemyAtkState* getInstance();
    
    virtual void Enter(Enemy *pRole);
    virtual void Exit(Enemy *pRole);
    virtual void Update(Enemy *pRole){};
    virtual bool HandleMsg(Enemy *pRole, const Message &msg){return false;};
    
    void setMoveInfo(const Fight_Wave_Element *pMoveInfo);  //设置移动信息 目前只有坐标
    
private:
    Fight_Wave_Element m_tMoveInfo;  // 移动信息
    void createAtkAction(Enemy *pRole);
};


#endif /* defined(__SingleEye__EnemyAtkState__) */
