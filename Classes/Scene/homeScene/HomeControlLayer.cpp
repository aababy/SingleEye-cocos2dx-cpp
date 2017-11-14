#include "HomeControlLayer.h"

bool CHomeControlLayer::init()
{
    bool bRst = false;
    
    do
    {
        CC_BREAK_IF(!CCNode::init());
        
        bRst = true;
        
    } while (0);
    
    return bRst;
}

void CHomeControlLayer::onEnter()
{
    CCNode::onEnter();
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    m_pAster = CHomeAster::create();
    m_pAster->ignoreAnchorPointForPosition(false);
    m_pAster->setAnchorPoint(ccp(0.5, 0.5));
    m_pAster->setPosition(ccp(winSize.width/2, 0));
    m_pAster->setScale(0.8f);
    m_pAster->setRotation(m_fRotation);
    addChild(m_pAster, 1);
    
    m_fRotation = 0.0f;
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    
    m_bIscontrol = false;
}

void CHomeControlLayer::onExit()
{
    removeAllChildrenWithCleanup(true);
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    unschedule(schedule_selector(CHomeControlLayer::deaccelerateScrolling));
    CCNode::onExit();
}

void CHomeControlLayer::touchDelegateRetain()
{
    this->retain();
}

void CHomeControlLayer::touchDelegateRelease()
{
    this->release();
}

bool CHomeControlLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    if(! containsTouchLocation(touch))
    {
        return false;
    }
    else
    {
        m_bIscontrol = true;
    }
    return true;
}

void CHomeControlLayer::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    if(false == m_bIscontrol)
    {
        return;
    }
    
    CCPoint prevPoint = touch->getPreviousLocationInView();
    CCPoint currentPoint = touch->getLocationInView();
        
    m_tScrollDistance = currentPoint.x - prevPoint.x;

    if(currentPoint.x < prevPoint.x)
    {
        float fSpeed = prevPoint.x - currentPoint.x;
    //    CCLOG("d[w_x]b rotation speed=%f",fSpeed);
        float fStep = HOME_ROTATION_STEP_BASE * fSpeed;
        if (fStep > HOME_ROTATION_STEP_MAX)
        {
            fStep = HOME_ROTATION_STEP_MAX;
        }
        
        rotationTo(m_fRotation - fStep);        
    }
    else
    {
        float fSpeed = currentPoint.x - prevPoint.x;
   //     CCLOG("d[w_x]b rotation speed=%f",fSpeed);
        float fStep = HOME_ROTATION_STEP_BASE * fSpeed;
        if (fStep > HOME_ROTATION_STEP_MAX)
        {
            fStep = HOME_ROTATION_STEP_MAX;
        }

        rotationTo(m_fRotation + fStep);
    }
    
}
void CHomeControlLayer::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    if(m_bIscontrol)
    {
        m_bIscontrol = false;
    }
   
    //进入减速状态
    schedule(schedule_selector(CHomeControlLayer::deaccelerateScrolling));

}


void CHomeControlLayer::deaccelerateScrolling(float dt)
{
    //1. 滑动距离判定    
    float fSpeed = fabsf(m_tScrollDistance);
    float fStep = HOME_ROTATION_STEP_BASE * fSpeed;
    if (fStep > HOME_ROTATION_STEP_MAX)
    {
        fStep = HOME_ROTATION_STEP_MAX;
    }    
        
    if (m_tScrollDistance < 0) {
        rotationTo(m_fRotation - fStep);
    }
    else
    {
        rotationTo(m_fRotation + fStep);    
    }
    
    //2. 滑动距离减弱
    m_tScrollDistance *= 0.86f;
    
    //3. 如果滑动距离小于1, 就不再滑动
    if (fSpeed < 1.f || 
        m_fRotation == HOME_ROTATION_MAX_RANGE || m_fRotation == HOME_ROTATION_MIN_RANGE)
    {
        unschedule(schedule_selector(CHomeControlLayer::deaccelerateScrolling));
    }    
}


bool CHomeControlLayer::containsTouchLocation(CCTouch* touch)
{
    return true;//test
    CCRect contentRect = CCRectMake(-m_obContentSize.width / 2, -m_obContentSize.height / 2, m_obContentSize.width, m_obContentSize.height);
    return contentRect.containsPoint(convertTouchToNodeSpaceAR(touch));
}
void CHomeControlLayer::rotationTo(float rotation)
{
    m_fRotation = rotation;
    
    if (m_fRotation > HOME_ROTATION_MAX_RANGE)
    {
        m_fRotation = HOME_ROTATION_MAX_RANGE;
    }
    if (m_fRotation < HOME_ROTATION_MIN_RANGE)
    {
        m_fRotation = HOME_ROTATION_MIN_RANGE;
    }
    
    if(m_pAster)
    {
        m_pAster->setRotation(m_fRotation);
    }
}
void CHomeControlLayer::rotationBy(float fStep)
{
    m_fRotation += fStep;
    
    if (m_fRotation > HOME_ROTATION_MAX_RANGE)
    {
        m_fRotation = HOME_ROTATION_MAX_RANGE;
    }
    if (m_fRotation < HOME_ROTATION_MIN_RANGE)
    {
        m_fRotation = HOME_ROTATION_MIN_RANGE;
    }
    
    if(m_pAster)
    {
        CCActionInterval* sAction = CCRotateBy::create(0.6, fStep);
        m_pAster->runAction(CCSequence::create(sAction,NULL));
    }
}