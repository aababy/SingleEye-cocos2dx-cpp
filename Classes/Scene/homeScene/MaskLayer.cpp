#include "MaskLayer.h"

CMaskLayer::CMaskLayer():
m_bIsDown(false)
, m_callbackListener(NULL)
, m_callback(NULL)
{
    
}

CMaskLayer::~CMaskLayer()
{

}

bool CMaskLayer::init()
{
    bool bRef = false;
    
    do
    {
        CC_BREAK_IF(!CCLayer::init());
        
        setTouchEnabled(true);
        
        bRef = true;
    }while(0);
    
    return bRef;
}
void CMaskLayer::onEnter()
{
    CCLayer::onEnter();
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
 
    //menu
    CSlideMenu *pSetting = CSlideMenu::create();
    pSetting->setPosition(ccp(winSize.width - pSetting->getContentSizeWithCloseState().width, 50));
    addChild(pSetting, 1, 3001);
    
    toHide();
}

void CMaskLayer::onExit()
{
    CCLayer::onExit();
}

void CMaskLayer::toShow()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    this->setPosition(CCPointZero);
    this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    
    CCLayerColor *pBack = CCLayerColor::create(ccc4(0, 0, 0, 125), winSize.width, winSize.height);
    addChild(pBack, 0, 3000);
}
void CMaskLayer::toHide()
{
    removeChildByTag(3000);
    this->setPosition(CCPointZero);
    this->setContentSize(CCSizeZero);
}

void CMaskLayer::registerWithTouchDispatcher()
{
    // -128 屏蔽下方触摸
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);

}
bool CMaskLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchPoint = getParent()->convertTouchToNodeSpace(pTouch);
    if (boundingBox().containsPoint(touchPoint) && !m_bIsDown)
    {
        m_bIsDown = true;
        return true;
    }
    
    return false;
};

void CMaskLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchPoint = getParent()->convertTouchToNodeSpace(pTouch);
    float deltaX = pTouch->getDelta().x;
    deltaX = deltaX > 0 ? deltaX : -deltaX;
    
    if (!boundingBox().containsPoint(touchPoint)  || !m_bIsDown || deltaX > 15)
    {
        m_bIsDown = false;
        return;
    }
    
};
void CMaskLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchPoint = getParent()->convertTouchToNodeSpace(pTouch);
    if (boundingBox().containsPoint(touchPoint) && m_bIsDown)
    {
        CBToClose();
        
        m_bIsDown = false;
    }
};
void CMaskLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    m_bIsDown = false;
};
void CMaskLayer::setCallbackFun(CCObject *target, SEL_CallFunc callfun)
{
    m_callbackListener = target;
    m_callback = callfun;    
}

void CMaskLayer::CBToClose()
{
    if (m_callback
        && m_callbackListener)
    {
        (m_callbackListener->*m_callback)();
    }
    
    CSlideMenu *pSlideMenu = dynamic_cast<CSlideMenu*>(this->getChildByTag(3001));
    if (pSlideMenu)
    {
        pSlideMenu->toHide();
    }
}

