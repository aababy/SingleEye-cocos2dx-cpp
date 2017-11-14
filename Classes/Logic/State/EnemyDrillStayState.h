//
//  EnemyDrillStayState.h
//  SingleEye
//
//  Created by qinfen on 13-12-3.
//
//

#ifndef __SingleEye__EnemyDrillStayState__
#define __SingleEye__EnemyDrillStayState__
#include "GameInclude.h"
/**
 *  软泥怪 钻入待机状态
 */
class EnemyDrillStayState : public State<Enemy>
{
public:
    EnemyDrillStayState(){};
    ~EnemyDrillStayState(){};
    
    static EnemyDrillStayState* getInstance();
    
    virtual void Enter(Enemy *pRole);
    virtual void Exit(Enemy *pRole);
    virtual void Update(Enemy *pRole){};
    virtual bool HandleMsg(Enemy *pRole, const Message &msg){return false;};
    
    void setMoveInfo(const Fight_Wave_Element *pMoveInfo);  //设置移动信息 目前只有坐标
private:
    void drillStayCallBack(CCNode* pSender , void* pData);
    Fight_Wave_Element m_tMoveInfo;  // 移动信息
    void createDrillStayAction(Enemy *pRole);
    CCFiniteTimeAction* createDelayAction(Enemy *pRole);
};


#endif /* defined(__SingleEye__EnemyDrillStayState__) */
