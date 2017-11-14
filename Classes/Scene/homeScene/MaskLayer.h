//遮罩层 屏蔽触摸
#ifndef ___MASK_LAYER_H__
#define ___MASK_LAYER_H__

#include "GameInclude.h"

class CMaskLayer: public CCLayer
{
public:
    CMaskLayer();
    ~CMaskLayer();
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(CMaskLayer);
    
    virtual void registerWithTouchDispatcher(void);
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    void setCallbackFun(CCObject* target, SEL_CallFunc callfun);
    void CBToClose();
    
    void toShow();
    void toHide();
    
private:
    CCObject* m_callbackListener;
    SEL_CallFunc m_callback;
    
    bool m_bIsDown;
};

#endif // ___MASK_LAYER_H__
