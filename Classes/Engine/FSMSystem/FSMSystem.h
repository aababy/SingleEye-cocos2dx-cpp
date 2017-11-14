//
//  FSMSystem.h
//  coslua
//
//  Created by chency on 13-5-23.
//
//

#ifndef coslua_FSMSystem_h
#define coslua_FSMSystem_h

#include "State.h"

class Message;
template <class obj_type>
class FSMSystem
{

public:

    FSMSystem(obj_type *_obj):m_pOwner(_obj),m_pFSMState(NULL){}

    virtual ~FSMSystem(){}

    void SetCurrentState(State<obj_type> *_obj){m_pFSMState = _obj;}
    State<obj_type> *GetCurrentState() {return m_pFSMState;}
    
    // 更新
    void  Update(float dt)
    {
        if (m_pFSMState)
            m_pFSMState->Update(m_pOwner);
    }

    // 更改状态
    void  ChangeState(State<obj_type>* pNewState)
    {
        if(m_pFSMState) m_pFSMState->Exit(m_pOwner);
        m_pFSMState = pNewState;
        m_pFSMState->Enter(m_pOwner);
    }

    // 处理消息
    bool HandleMsg(const Message &msg)
    {
        if(m_pFSMState)
            return m_pFSMState->HandleMsg(m_pOwner, msg);
        return false;
    };
private:
    // 拥有者
    obj_type *m_pOwner;
    // 当前状态
    State<obj_type> *m_pFSMState;


    

    
};


#endif
