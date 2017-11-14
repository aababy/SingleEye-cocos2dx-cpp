//
//  EnemyStopState.h
//  SingleEye
//
//  Created by qinfen on 13-11-29.
//
//

#ifndef __SingleEye__EnemyStopState__
#define __SingleEye__EnemyStopState__

#include "GameInclude.h"

class EnemyStopState : public State<Enemy>
{
public:
    EnemyStopState(){};
    ~EnemyStopState(){};
    
    static EnemyStopState* getInstance();
    
    virtual void Enter(Enemy *pRole);
    virtual void Exit(Enemy *pRole);
    virtual void Update(Enemy *pRole){};
    virtual bool HandleMsg(Enemy *pRole, const Message &msg){return false;};
    
    void setMoveInfo(const Fight_Wave_Element *pMoveInfo);  //设置移动信息 目前只有坐标
    void setTime(float time  ,Stop_Type type); //设置减速时间
private:
    void stopEndCallback(CCNode* pSender , void* pData);
    
    Fight_Wave_Element m_tMoveInfo;  // 移动信息
    void createStopAction(Enemy *pRole);
    void createDelayAction(Enemy *pRole);
    float m_fTime;
    Stop_Type m_eType;
};

#endif /* defined(__SingleEye__EnemyStopState__) */
