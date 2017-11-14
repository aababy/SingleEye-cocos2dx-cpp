//
//  EnemySlowDownState.h
//  SingleEye
//
//  Created by qinfen on 13-11-19.
//
//

#ifndef __SingleEye__EnemySlowDownState__
#define __SingleEye__EnemySlowDownState__

#include "GameInclude.h"


class EnemySlowDownState : public State<Enemy>
{
public:
    EnemySlowDownState(){};
    ~EnemySlowDownState(){};
    
    static EnemySlowDownState* getInstance();
    
    virtual void Enter(Enemy *pRole);
    virtual void Exit(Enemy *pRole);
    virtual void Update(Enemy *pRole){};
    virtual bool HandleMsg(Enemy *pRole, const Message &msg){return false;};
    
    void setMoveInfo(const Fight_Wave_Element *pMoveInfo);  //设置移动信息 目前只有坐标
    void setTime(float time); //设置减速时间
private:
    void slowDownEndCallback(CCNode* pSender);
    
    Fight_Wave_Element m_tMoveInfo;  // 移动信息
    void createMoveAction(Enemy *pRole);
    void createDelayAction(Enemy *pRole);
    float m_fTime;
};

#endif /* defined(__SingleEye__EnemySlowDownState__) */
