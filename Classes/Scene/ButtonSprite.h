//点击后带放大action的button

#ifndef __ButtonSprite__
#define __ButtonSprite__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

typedef  void  (CCObject::*ButtonClick_Func)();
typedef  void  (CCObject::*ButtonClick_FuncN)(CCObject* obj);
typedef  void  (CCObject::*ButtonClick_FuncND)(CCObject* obj,void* param);


#define click_selector(_SELECTOR) (ButtonClick_Func)(&_SELECTOR)
#define click_selectorN(_SELECTOR) (ButtonClick_FuncN)(&_SELECTOR)
#define click_selectorND(_SELECTOR) (ButtonClick_FuncND)(&_SELECTOR)

class ButtonSprite : public CCSprite,public CCTouchDelegate
{
    // touch 事件优先级
    int m_touchPriority;
    
    bool m_isDown;
    
    bool m_bSwallowsTouches;
    
    // click handle
    ButtonClick_Func m_BtnClick_Func;
    
    //    int param;
public:
    ButtonSprite();
    virtual ~ButtonSprite();
    
    // 调整按钮的默认优先级,设置为最高优先级
    static ButtonSprite* createButtonSprite(const char* fileName, bool bSwallowsTouches = true, int tPriority = kCCMenuHandlerPriority);
    
    static ButtonSprite* createButtonSpriteWithSpriteFrame(CCSpriteFrame *pSpriteFrame , bool bSwallowsTouches = true, int tPriority = kCCMenuHandlerPriority);
    
    /* 触摸相关 */
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    // optional
    
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    
    void clicked();
    
    void setOnClickListener(ButtonClick_Func tBtnClickFunc);
};




#endif /* defined(__ButtonSprite__) */
