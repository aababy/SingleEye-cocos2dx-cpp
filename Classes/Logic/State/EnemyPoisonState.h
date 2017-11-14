//
//  EnemyPoisonState.h
//  SingleEye
//
//  Created by qinfen on 13-11-29.
//
//

#ifndef __SingleEye__EnemyPoisonState__
#define __SingleEye__EnemyPoisonState__

#include "GameInclude.h"

class EnemyPoisonState : public State<Enemy>
{
public:
    EnemyPoisonState(){};
    ~EnemyPoisonState(){};
    
    static EnemyPoisonState* getInstance();
    
    virtual void Enter(Enemy *pRole);
    virtual void Exit(Enemy *pRole);
    virtual void Update(Enemy *pRole){};
    virtual bool HandleMsg(Enemy *pRole, const Message &msg){return false;};
    
    void setMoveInfo(const Fight_Wave_Element *pMoveInfo);  //设置移动信息 目前只有坐标
private:
    
    Fight_Wave_Element m_tMoveInfo;  // 移动信息
    void createPoisonAction(Enemy *pRole);
};


#endif /* defined(__SingleEye__EnemyPoisonState__) */
