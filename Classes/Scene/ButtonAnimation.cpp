//
//  ButtonAnimation.cpp
//  SingleEye
//
//  Created by wangwx on 14-1-20.
//
//

#include "ButtonAnimation.h"

CButtonAnimation::CButtonAnimation():m_isDown(false)
{
    m_touchPriority = 0;
    m_bSwallowsTouches = true;
    m_callback = 0;
}
CButtonAnimation::~CButtonAnimation()
{
    CCLOG("filename:%s",m_sResName.c_str());
    cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(m_sResName.c_str());
};

CButtonAnimation* CButtonAnimation::createButtonAnimation(const char* cacheName, const char *armatureName, bool bSwallowsTouches, int tPriority)
{
    CButtonAnimation* button  = new CButtonAnimation();
    button->m_touchPriority = tPriority;
    button->m_bSwallowsTouches = bSwallowsTouches;
    if (button && button->initContent(cacheName, armatureName))
    {
        button->autorelease();
        return button;
    }
    CC_SAFE_DELETE(button);
    return button;
}

bool CButtonAnimation::initContent(const char* cacheName, const char *armatureName)
{
    bool bRst = false;
    
    do
    {
        setAnchorPoint(ccp(0.5, 0.5));
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(cacheName);
        m_sResName = cacheName;
        CCArmature* pArmature = CCArmature::create(armatureName);
        CC_BREAK_IF(!pArmature);
        setContentSize(pArmature->getContentSize());
        pArmature->setPosition(ccp(getContentSize().width/2, getContentSize().height/2));
        pArmature->getAnimation()->playByIndex(0);
        addChild(pArmature);

        //for debug
        if(0)
        {
            CCLayerColor *pMask = CCLayerColor::create(ccc4(0,0,0,110));
            pMask->setContentSize(getContentSize());
            addChild(pMask);
        }
    
        bRst = true;
        
    } while (0);
    
    return bRst;
}

bool CButtonAnimation::init()
{
    bool bRst = false;
    
    do
    {
        CC_BREAK_IF(!CCNode::init());
        
        bRst = true;
        
    } while (0);
    
    return bRst;
}

void CButtonAnimation::onEnter()
{
    CCNode::onEnter();
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, m_touchPriority, m_bSwallowsTouches);
}

void CButtonAnimation::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    
    CCNode::onExit();
}

void CButtonAnimation::touchDelegateRetain()
{
    this->retain();
}

void CButtonAnimation::touchDelegateRelease()
{
    this->release();
}

bool CButtonAnimation::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
//    CCLOG("CButtonAnimation touchBegan");
//    CCSize c = this->getContentSize();
//    CCRect rect = boundingBox();
//    CCLOG("btn size=%f,%f", c.width, c.height);
//    CCLOG("btn rect=%f,%f,%f,%f", rect.getMinX(),rect.getMinY(), rect.getMaxX(), rect.getMaxY());
    
    CCPoint touchPoint = getParent()->convertTouchToNodeSpace(pTouch);
    if (boundingBox().containsPoint(touchPoint) && !m_isDown)
    {
        m_isDown = true;
        CCLOG("CButtonAnimation touchBegan down");
        return true;
    }
    
    return false;
};

void CButtonAnimation::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLOG("CButtonAnimation touchmove");
    
    CCPoint touchPoint = getParent()->convertTouchToNodeSpace(pTouch);
    
    float deltaX = pTouch->getDelta().x;
    
    deltaX = deltaX > 0 ? deltaX : -deltaX;
    
    if (!boundingBox().containsPoint(touchPoint)  || !m_isDown || deltaX > 15)
    {
        m_isDown = false;
        return;
    }
    
};

void CButtonAnimation::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLOG("CButtonAnimation touchEnd");
    
    CCPoint touchPoint = getParent()->convertTouchToNodeSpace(pTouch);
    if (boundingBox().containsPoint(touchPoint) && m_isDown)
    {
        /* 相当于产生了click事件 */
        CCLOG("CButtonAnimation click");
        if (m_callback && m_callbackListener)
        {
            //来个点击效果
            float curScaleX = this->getScaleX();//防止之前scale的值被修改丢失
            float curScaleY = this->getScaleY();
            CCActionInterval* sAction = CCScaleTo::create(0.1, curScaleX*1.2, curScaleY*1.2);
            CCActionInterval* sAction2 = CCScaleTo::create(0.1, curScaleX*1.0, curScaleX*1.0);
            CCCallFunc*  callFun = CCCallFunc::create(this,callfunc_selector(CButtonAnimation::clicked));
            runAction(CCSequence::create(sAction,sAction2,callFun,NULL));
        }
        
        m_isDown = false;
    }
};

void CButtonAnimation::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){
    m_isDown = false;
};

void CButtonAnimation::setCallbackFun(CCObject* target, SEL_CallFunc callfun)
{
    m_callbackListener = target;
    m_callback = callfun;
}

void CButtonAnimation::clicked()
{
   (m_callbackListener->*m_callback)();
}
