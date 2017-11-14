//
//  EngineSystem.cpp
//  coslua
//
//  Created by yuanlu on 13-5-16.
//
//

#include "EngineSystem.h"
#include "MsgSystem.h"
#include "CCDrawHelp.h"
#include "Loading.h"
#include "Dialog.h"
#include "MTNotificationQueue.h"


typedef enum _EngineTag
{
    T_Dialog = 0,
    T_SystemBusy,
}EngineTag;


NS_ENGINE_BEGIN

EngineSystem* EngineSystem::SharedEngineSystem()
{
    static EngineSystem instance;
    return &instance;
}


EngineSystem::EngineSystem()
{
    CCNode::init();
    
    m_bShowNetEnabled = true;
    m_bNetMessageEnabled = true;
    
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(EngineSystem::onNotification), NC_Dialog, NULL);
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(EngineSystem::onNotification), NC_SystemBusy, NULL);
}


EngineSystem::~EngineSystem()
{
    CCDirector::sharedDirector()->getScheduler()->unscheduleUpdateForTarget(this);
    
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}


void EngineSystem::update(float delta)
{
    MTNotificationQueue::getInstance()->postNotifications(delta);    
    //这句必须有
    (MsgSystem::Instance())->SendDelayedMsg();
}


void EngineSystem::setBusyEnabled(bool bEnabled)
{
    m_bShowNetEnabled = bEnabled;
    
    m_bNetMessageEnabled = m_bShowNetEnabled;   //同样影响 Message
}


void EngineSystem::setNetMessageEnabled(bool bEnabled)
{
    m_bNetMessageEnabled = bEnabled;
}


void EngineSystem::onNotification(CCObject *obj)
{
    NData *data = (NData*)obj;
    
    switch (data->iTag) {
        case T_Dialog:
        {
            xDialog->show(data->str.c_str());
        }
            break;
        case T_SystemBusy:
        {
            if (m_bBusy) {
                xLoading->start();
            }
            else
            {
                xLoading->stop();
            }
        }
            break;
        default:
            break;
    }
    
    
    
    //删除该data; (仅当只删除1个元素时才能这样用)
    vector<NData*>::iterator iter = m_vData.begin();
    
    while(iter != m_vData.end())
    {
        if (*iter == data)
		{
            CC_SAFE_DELETE(data);
			m_vData.erase(iter);
            break;
		}
        else
        {
            iter++;
        }
    }
    
    /* erase用法备用
     for (vector<NData*>::iterator iter = m_vData.begin(); iter != m_vData.end();)
     {
     if (*iter == data)
     {
     CC_SAFE_DELETE(data);
     //当erase后 iter会返回+1的iter
     iter = m_vData.erase(iter);
     }
     else
     {
     iter++;
     }
     }
     */
    
    return;
}


#pragma mark -对话框
/**
 *	@brief	显示对话框
 *  左按键，右按键都是空，默认显示确定
 *	@param 	pContent 	显示内容
 *	@param 	pLBName 	左按键图片名称
 *	@param 	pRBName 	右按键图片名称
 *	@param 	mleve       对话框优先级
 *	@param 	bokexit 	是否退出应用 true：退出
 *	@param 	pObject 	按键消息接收者（按下左右软键）
 *
 *	@return	失败时返回false
 */
//这是网络线程的代码, 禁止在这里调用任何主线程绘图代码(包括CCNotificationCenter)
bool EngineSystem::ShowDialog(const char *pContent, const char *pLBName, const char *pRBName,
                              EG_DIALOG_LEVEL mleve, bool bokexit, CCObject *pObject)

{
    if (m_bShowNetEnabled || m_bNetMessageEnabled) {
        NData *data = new NData();
        data->iTag = T_Dialog;
        data->str = string(pContent);
        
        m_vData.push_back(data);
        
        MTNotificationQueue::getInstance()->postNotification(NC_Dialog, data);
    }
    
    return true;
}


//这是网络线程的代码, 禁止在这里调用任何主线程绘图代码(包括CCNotificationCenter)
bool EngineSystem::SetSysBusyTipState(bool bopen, int64_t mdtms)
{
    m_bBusy = bopen;
    
    if (m_bShowNetEnabled) {
        
        NData *data = new NData();
        data->iTag = T_SystemBusy;
        
        m_vData.push_back(data);
        
        MTNotificationQueue::getInstance()->postNotification(NC_SystemBusy, data);
    }
    
    return true;
}


NS_ENGINE_END
