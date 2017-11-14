#include "ButtonSprite.h"

ButtonSprite::ButtonSprite():m_isDown(false)
{
    m_touchPriority = 0;
    m_BtnClick_Func = 0;
    m_bSwallowsTouches = true;
}
ButtonSprite::~ButtonSprite(){};


void ButtonSprite::onEnter()
{
    CCSprite::onEnter();
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,m_touchPriority, m_bSwallowsTouches);
};
void ButtonSprite::onExit()
{
    CCSprite::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
};


bool ButtonSprite::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
//    CCLOG("buttonSprite touchBegan");
    CCPoint touchPoint = getParent()->convertTouchToNodeSpace(pTouch);
    if (boundingBox().containsPoint(touchPoint) && !m_isDown) {
        m_isDown = true;
        CCLOG("ButtonSprite touchBegan down");
        return true;
    }
    
    return false;
};
// optional

void ButtonSprite::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
//    CCLOG("buttonSprite touchMove");
    CCPoint touchPoint = getParent()->convertTouchToNodeSpace(pTouch);
    
    float deltaX = pTouch->getDelta().x;
    
    deltaX = deltaX > 0 ? deltaX : -deltaX;
    
    if (!boundingBox().containsPoint(touchPoint)  || !m_isDown || deltaX > 15)
    {
        m_isDown = false;
        return;
    }
    
};
void ButtonSprite::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
//    CCLOG("buttonSprite touchEnd");
    CCPoint touchPoint = getParent()->convertTouchToNodeSpace(pTouch);
    if (boundingBox().containsPoint(touchPoint) && m_isDown) {
        /* 相当于产生了click事件 */
        if (m_BtnClick_Func)
        {
            //来个点击效果
            float curScaleX = this->getScaleX();//防止之前scale的值被修改丢失
            float curScaleY = this->getScaleY();
            CCActionInterval* sAction = CCScaleTo::create(0.1, curScaleX*1.2, curScaleY*1.2);
            CCActionInterval* sAction2 = CCScaleTo::create(0.1, curScaleX*1.0, curScaleX*1.0);
            CCCallFunc*  callFun = CCCallFunc::create(this,callfunc_selector(ButtonSprite::clicked));
            runAction(CCSequence::create(sAction,sAction2,callFun,NULL));
            
        }
        m_isDown = false;
    }
};


void ButtonSprite::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){
    m_isDown = false;
};

ButtonSprite* ButtonSprite::createButtonSprite(const char* fileName, bool bSwallowsTouches, int tPriority)
{
    ButtonSprite* button  = new ButtonSprite();
    button->m_touchPriority = tPriority;
    button->m_bSwallowsTouches = bSwallowsTouches;
    if (button && button->initWithFile(fileName))
    {
        button->autorelease();
        return button;
    }
    CC_SAFE_DELETE(button);
    return button;
}


ButtonSprite* ButtonSprite::createButtonSpriteWithSpriteFrame(CCSpriteFrame *pSpriteFrame, bool bSwallowsTouches, int tPriority)
{
    ButtonSprite *pobSprite = new ButtonSprite();
    pobSprite->m_touchPriority = tPriority;
    pobSprite->m_bSwallowsTouches = bSwallowsTouches;
    if (pSpriteFrame && pobSprite && pobSprite->initWithSpriteFrame(pSpriteFrame))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

//
// update
void ButtonSprite::setOnClickListener(ButtonClick_Func tBtnClickFunc)
{
    m_BtnClick_Func = tBtnClickFunc;
}

void ButtonSprite::clicked()
{
    (this->*m_BtnClick_Func)();
}