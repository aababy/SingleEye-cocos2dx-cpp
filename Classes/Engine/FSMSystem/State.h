//
//  State.h
//  coslua
//
//  Created by chency on 13-5-23.
//
//

#ifndef coslua_State_h
#define coslua_State_h

class Message;
template <class obj_type>
class State
{
public:
    //State(){};
    virtual ~State(){};
    // 进入时调用
    virtual void Enter(obj_type*)=0;
    // 退出时调用
    virtual void Exit(obj_type*)=0;
    // 更新
    virtual void Update(obj_type*)=0;
    // 处理消息
    virtual bool HandleMsg(obj_type*, const Message&)=0;

};

#endif
