//
//  ButtonAnimation.h
//  SingleEye
//
//  Created by wangwx on 14-1-20.
//
//
//带精灵动画的按钮

#ifndef __SingleEye__ButtonAnimation__
#define __SingleEye__ButtonAnimation__

#include "GameInclude.h"

USING_NS_CC;
USING_NS_CC_EXT;

class CButtonAnimation : public CCNode, public CCTargetedTouchDelegate
{
public:
    CButtonAnimation();
    virtual ~CButtonAnimation();
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void touchDelegateRetain();
    virtual void touchDelegateRelease();
    
    CREATE_FUNC(CButtonAnimation);
    
    void setCallbackFun(CCObject* target, SEL_CallFunc callfun);
    
    static CButtonAnimation* createButtonAnimation(const char* cacheName, const char *armatureName, bool bSwallowsTouches = true, int tPriority = kCCMenuHandlerPriority);
    bool initContent(const char* cacheName, const char *armatureName);
    void clicked();
private:
    CCObject* m_callbackListener;
    SEL_CallFunc m_callback;
    bool m_isDown;
    
    // touch 事件优先级
    int m_touchPriority;
    bool m_bSwallowsTouches;
    
    std::string m_sResName;
};

#endif /* defined(__SingleEye__ButtonAnimation__) */
