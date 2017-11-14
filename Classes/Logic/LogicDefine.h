//
//  LogicDefine.h
//  coslua
//
//  Created by yuanlu on 13-6-2.
//
//

#ifndef coslua_LogicDefine_h
#define coslua_LogicDefine_h
 
#include "cocos2d.h"
#include "../Engine/Engine.h"

typedef enum
{
    ATK_NOR_SR = 1, //普通近程攻击(闪烁攻击特效)
    ATK_NOR_RM,     //普通远程攻击(原地飞行特效)
    ATK_SKL_SR,     //技能近程攻击(跳跃挤压)
    ATK_SKL_RM,     //技能远程攻击(上升放大飞行特效)
    ATK_SKL_STAY,   //技能原地攻击(原地施放)
    ATK_SKL_ZXM,    //技能原地攻击(中心全体移动)
    ATK_SKL_ZX,     //技能原地攻击(中心全体)
    ATK_FJ_SR,      //反击攻击(近程)
    ATK_FJ_RM,      //反击攻击(远程)
}ATKWAY_MODE;//攻击类型

typedef enum _FIGHT_ANI_TYPE_
{
    FIGHT_ANITYPE_MOVE = 0,//移动
    FIGHT_ANITYPE_ACT,//攻击
    FIGHT_ANITYPE_EFE,//受击
    //神之语
    FIGHT_ANITYPE_GD,//光带
    FIGHT_ANITYPE_WZ,//文字
    FIGHT_ANITYPE_DOWN,//下落
    FIGHT_ANITYPE_TS1,//天使1
    FIGHT_ANITYPE_TS2,//天使2
    FIGHT_ANITYPE_NUM,
}FIGHT_ANI_TYPE;//动画类型

typedef struct _MOVE_DESINFO_
{
    float mDScal;   //缩放
    float mOrgScal; //原始缩放
    CCPoint mPoint; //坐标
}MOVE_DESINFO;//移动位置信息

//typedef struct _MOVE_INFO_
//{
//    bool bstay;     //是否返回等待状态(攻击结束)
//    bool bEnlager;  //是否放大处理
//    bool bCongChi;  //是否近程冲刺
//    bool bMtAct;    //是否上升点攻击(远攻)
//    float mMDtime;   //移动下降时长
//    float mDtime;   //延迟时间
//    float mOrgScal; //原始缩放
//    float mDScal;   //目标位置缩放
//    CCRole *pRole;  //角色
//    CCPoint mPoint; //目标坐标
//}MOVE_INFO;//移动信息

typedef struct _ROLEPARAM_INFO_
{
    int32_t mHp;//当前血量值
    int32_t mMp;//当前魔法值
    int32_t mHpTotal;//血量值
    int32_t mBuffId;//当前显示Buff
}ROLEPARAM_INFO;//角色当前属性

//typedef struct _CHANGE_INFO_
//{
//    CCRole *pRole;//角色
//    bool bNoSprt;//停止播放特效
//    byte mHurtTime;//伤害显示次数
//    ROLEPARAM_INFO *pParaInfo;//角色参数信息
//    CardChange *pChangeInfo;//改变内容
//}CHNAGE_INFO;//伤害信息

typedef struct _ACTZX_PARAM_INFO_
{
    bool bMove;//移动
}ACTZX_PARAM_INFO;//中心攻击信息

//typedef struct _ACTZX_INFO_
//{
//    CCRole *pRole;//角色
//    ACTZX_PARAM_INFO mparaminfo;//攻击信息
//}ACTZX_INFO;//技能中心攻击信息

#endif
