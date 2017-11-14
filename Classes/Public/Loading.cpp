//
//  Loading.cpp
//  SingleEye
//
//  Created by gwsoft on 1/16/14.
//
//

#include "Loading.h"

#define kLoadingHandlerPriority  (kCCMenuHandlerPriority - 2)
#define kLoadingZOrder           (128 + 1)


Loading * Loading::getInstance()
{
    static Loading instance;
    
    return &instance;
}


Loading::Loading()
{
    m_bBlocked = true;    
    bEnabled = true;
    
    init();
}


void Loading::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kLoadingHandlerPriority, true);
}


void Loading::setBlockedState(bool bBlocked)
{            
    if (bBlocked != m_bBlocked) {
        m_bBlocked = bBlocked;
                        
        unschedule(schedule_selector(Loading::scheduleSetBlockedState));
        scheduleOnce(schedule_selector(Loading::scheduleSetBlockedState), 0.f);
    }
}


bool Loading::init()
{
    CCLayer::init();
    
    setTouchEnabled(true);

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    DATAPOOL->addEffectCache(pvpLoadingRes);
    m_aLoading = CCArmature::create("loading");
    m_aLoading->setPosition(ccp(winSize.width/2, winSize.height/2));
    addChild(m_aLoading, kLoadingZOrder);
    
    return true;
}


void Loading::onExit()
{
    CCLayer::onExit();
    
    unscheduleAllSelectors();
}


bool Loading::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}


void Loading::start(bool bBlocked)
{
    start();
    setBlockedState(bBlocked);
}


void Loading::start()
{
    if (getParent() != NULL || bEnabled == false) {
        return;
    }
    
    CCScene *pRunScene = CCDirector::sharedDirector()->getRunningScene();
    
    if (pRunScene) {
        m_aLoading->setVisible(true);
        m_aLoading->getAnimation()->play("Animation1");
        
        pRunScene->addChild(this, kLoadingZOrder);
    }
    
    setBlockedState(true); 
}


void Loading::stop()
{
    if (getParent() == NULL) {
        return;
    }
    
    if (m_aLoading) {
        m_aLoading->getAnimation()->stop();
        m_aLoading->stopAllActions();
        m_aLoading->setVisible(false);
        
        CCScene *pRunScene = CCDirector::sharedDirector()->getRunningScene();
        pRunScene->removeChild(this);

    }
}


void Loading::setEnabled(bool bEnabled)
{
    Loading *load = Loading::getInstance();
    load->bEnabled = bEnabled;
    
    if (bEnabled == false) {
        stop();
    }
}


void Loading::scheduleSetBlockedState(float dt)
{
    CCTargetedTouchHandler* handler = (CCTargetedTouchHandler*)CCDirector::sharedDirector()->getTouchDispatcher()->findHandler(this);
    
    if (handler) {            
        handler->setSwallowsTouches(m_bBlocked);
    }
    else
    {
        CCAssert(false, "error");
    }    
}
