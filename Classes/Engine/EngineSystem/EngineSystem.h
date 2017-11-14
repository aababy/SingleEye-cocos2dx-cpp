//
//  EngineSystem.h
//  coslua
//
//  Created by yuanlu on 13-5-16.
//
//

#ifndef __EngineSystem__
#define __EngineSystem__

#include <stddef.h>
#include <string.h>
#include <vector>
#include "EngineDefine.h"
#include "cocos2d.h"
#include "Message.h"

USING_NS_CC;
using namespace std;

NS_ENGINE_BEGIN

#define EG_CHILD_ZORDER  (10)//默认提示控件层
#define EG_TIPMSG_TIME  (2.0f)//默认文字提示显示时长
#define EG_TIPMSG_VDIS  (100)//默认文字提示垂直偏移
#define EG_TIPMSG_TXTCOLOR_SUC  (ccc3(0,255,0))//默认文字提示成功字体颜色
#define EG_TIPMSG_TXTCOLOR_FAL  (ccc3(255,0,0))//默认文字提示失败字体颜色
#define EG_TIPMSG_LINE_COLOR   (ccc3(0,0,0))//默认文字提示字体描边颜色
#define EG_TIPMSG_LINE_SIZE    (2.0f)//默认文字提示字体描边宽度
#define EG_TIPMSG_STARTID   (1000)//系统文字飘浮开始ID
#define EG_TIPMSG_MAXNUM    (10000)//系统文字飘浮最大个数
#define EG_TIPMSG_ENDID     (EG_TIPMSG_STARTID+EG_TIPMSG_MAXNUM)//系统文字飘浮结束ID

class CCDialog;
class SysTip;

#define xNetEngine        EngineSystem::SharedEngineSystem()

typedef enum _EG_DIALOG_LEVEL_
{
    DIALOG_LEVEL_NONE = -1,//无
    DIALOG_LEVEL_SCENE = 0,//场景
    DIALOG_LEVEL_NET,//网络
    DIALOG_LEVEL_SYS,//系统
    DIALOG_LEVEL_NUM,
}EG_DIALOG_LEVEL;


class NData: public CCObject
{
public:
    int iTag;
    bool bData;
    string str;
};


class EngineSystem: public CCNode
{
public:
    static EngineSystem* SharedEngineSystem();
    
    EngineSystem();
    ~EngineSystem();
    void update(float delta);
    void onNotification(CCObject *obj);
    void setBusyEnabled(bool bEnabled);
    void setNetMessageEnabled(bool bEnabled);
    
    
    //自动显示网络消息对话框
    bool ShowDialog(const char *pContent, const char *pLBName=NULL, const char *pRBName=NULL,
                    EG_DIALOG_LEVEL mleve=DIALOG_LEVEL_SCENE, bool bokexit=false, CCObject *pObject=NULL);//显示对话框
    
    //自动显示网络繁忙状态
    bool SetSysBusyTipState(bool bopen, int64_t mdtms=0);//是否提示系统繁忙
    
private:
    bool                    m_bShowNetEnabled;
    bool                    m_bNetMessageEnabled;
    bool                    m_bBusy;
    vector<NData*>          m_vData;
    vector<CCString*>       m_vTag;
};

NS_ENGINE_END

#endif