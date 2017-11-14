//
//  EnemyInkJetState.h
//  SingleEye
//
//  Created by qinfen on 13-12-16.
//
//

#ifndef __SingleEye__EnemyInkJetState__
#define __SingleEye__EnemyInkJetState__
/**
 *  章鱼喷墨
 */
#include "GameInclude.h"
class EnemyInkJetState : public State<Enemy>
{
public:
    EnemyInkJetState(){};
    ~EnemyInkJetState(){};
    
    static EnemyInkJetState* getInstance();
    
    virtual void Enter(Enemy *pRole);
    virtual void Exit(Enemy *pRole);
    virtual void Update(Enemy *pRole){};
    virtual bool HandleMsg(Enemy *pRole, const Message &msg){return false;};
    
    void setMoveInfo(const Fight_Wave_Element *pMoveInfo);  //设置移动信息 目前只有坐标
private:
    Fight_Wave_Element m_tMoveInfo;  // 移动信息
    void createInkJetAction(Enemy *pRole);
};

#endif /* defined(__SingleEye__EnemyInkJetState__) */
