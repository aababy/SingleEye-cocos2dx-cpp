//
//  EnemyInvisibilityState.h
//  SingleEye
//
//  Created by qinfen on 13-12-10.
//
//

#ifndef __SingleEye__EnemyInvisibilityState__
#define __SingleEye__EnemyInvisibilityState__
/**
 *  隐身
 */
#include "GameInclude.h"

class EnemyInvisibilityState : public State<Enemy>
{
public:
    EnemyInvisibilityState(){};
    ~EnemyInvisibilityState(){};
    
    static EnemyInvisibilityState* getInstance();
    
    virtual void Enter(Enemy *pRole);
    virtual void Exit(Enemy *pRole);
    virtual void Update(Enemy *pRole){};
    virtual bool HandleMsg(Enemy *pRole, const Message &msg){return false;};
    
    void setMoveInfo(const Fight_Wave_Element *pMoveInfo);  //设置移动信息 目前只有坐标
private:
    Fight_Wave_Element m_tMoveInfo;  // 移动信息
    void createInvisibilityAction(Enemy *pRole);
    void invisibilityCallBack(CCObject *pSender);
    void fadeInCallBack(CCObject *pSender);
};


#endif /* defined(__SingleEye__EnemyInvisibilityState__) */
