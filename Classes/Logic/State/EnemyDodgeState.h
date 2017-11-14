//
//  EnemyDodgeState.h
//  SingleEye
//
//  Created by qinfen on 13-12-11.
//
//

#ifndef __SingleEye__EnemyDodgeState__
#define __SingleEye__EnemyDodgeState__
/**
 *  闪躲
 */
#include "GameInclude.h"
class EnemyDodgeState : public State<Enemy>
{
public:
    EnemyDodgeState(){};
    ~EnemyDodgeState(){};
    
    static EnemyDodgeState* getInstance();
    
    virtual void Enter(Enemy *pRole);
    virtual void Exit(Enemy *pRole);
    virtual void Update(Enemy *pRole){};
    virtual bool HandleMsg(Enemy *pRole, const Message &msg){return false;};
    
    void setMoveInfo(const Fight_Wave_Element *pMoveInfo);  //设置移动信息 目前只有坐标
    void setLeft(bool res){m_bLeft = res;};
private:
    Fight_Wave_Element m_tMoveInfo;  // 移动信息
    void createDodgeAction(Enemy *pRole);
    void DodgeCallBack(CCObject *pSender);
    bool m_bLeft;  //子弹的x加速度是否为正 从左边飞过来
};

#endif /* defined(__SingleEye__EnemyDodgeState__) */
