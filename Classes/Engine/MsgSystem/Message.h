//
//  Message.h
//  coslua
//
//  Created by chency on 13-5-26.
//
//

#ifndef coslua_Message_h
#define coslua_Message_h

USING_NS_CC;

//客户端自定义消息,网络协议消息小于10000
typedef enum
{
    //网络消息    
    NET_ERROR = 10001, // 联网出错信息
    
    //系统消息
    SYS_TYPE = 20001,
    SYS_DLG_OK,//对话框OK消息
    SYS_DLG_CANCEL,//对话框Cancel消息
    SYS_KEYBOARD_SHOW,//虚拟键盘出现消息
    SYS_KEYBOARD_HIDE,//虚拟键盘消失消息
    SYS_BUSYTIP_CHANGE,//系统繁忙提示状态改变
    
    //自定义消息
    USER_TYPE = 30001,//用户消息
    ROLE_MOVE,
    ROLE_STOP,
    ROLE_MOVE_OR_STOP,

    //打斗消息
    FIGHT_ACT_RM,//远程普通攻击
    FIGHT_ACT_RM_SKIL,//远程技能攻击
    FIGHT_ACT_SD_PUSH,//近程普通攻击(挤压)
    FIGHT_ACT_SD,//近程技能攻击
    FIGHT_ACT_SD_CC,//近程技能攻击(冲刺)
    FIGHT_ACT_STAY,//技能攻击原地发招
    FIGHT_ACT_ZX,//技能攻击阵型中心点
    FIGHT_MOVE,//移动
    FIGHT_MOVE_CC,//移动冲刺
    FIGHT_MOVE_EXT,//移动放大(近程)
    FIGHT_MOVE_EXTAK,//移动放大(远程)
    FIGHT_STAY,//站立
    FIGHT_SKILL_NAME,//技能名称显示完
    FIGHT_CHECK_MISS,//检查闪避事件
    FIGHT_MISS,//闪避
    FIGHT_EFFECT,//受击
    FIGHT_FJ,//反击
    FIGHT_SZY,//神之语
    FIGHT_START,//进攻开始
    FIGHT_ACTOVER,//进攻结束
    FIGHT_ADDBUFF,//Buff添加
    FIGHT_RELIFE,//复活
    FIGHT_CHECKDEAD,//检测死亡
    FIGHT_OVER,//回合结束
    //
    MAKESURE,//确认消息
}MSGSYS_MSG_TYPE;

class Message
{
public:
    Message(CCObject *sender, CCObject *receiver, int msgType, float sendingTime, void *extraData = NULL):m_pSender(sender),
    m_pReceiver(receiver),
    m_nMsgType(msgType),
    m_fSendingTime(sendingTime),
    m_pExtraData(extraData){};
    Message(){};


    bool operator<(const Message &other) const
    {
//        bool b = (*this).m_pReceiver == other.m_pReceiver &&
//        (*this).m_pSender == other.m_pSender &&
//        (*this).m_pExtraData == other.m_pExtraData &&
//        abs((*this).m_fSendingTime - other.m_fSendingTime) < 10;
//        if(b) return false;
        //  set容器排序使用必须加=符号
        return (*this).m_fSendingTime <= other.m_fSendingTime;
    }

    // 消息发送者
    CCObject *m_pSender;

    // 消息接收者
    CCObject *m_pReceiver;

    // 消息类型
    int m_nMsgType;

    // 发送时间
    // 当消息是延迟消息时有用 及时消息忽略 单位：毫秒
    int64_t m_fSendingTime;

    // 附加数据
    // 可以根据消息类型自定义附加的传送数据
    // 默认为空
    void *m_pExtraData;

};
#endif
