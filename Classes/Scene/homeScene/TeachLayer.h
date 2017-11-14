//
//  TeachLayer.h
//  SingleEye
//
//  Created by wangwx on 14-1-19.
//
//

#ifndef __SingleEye__TeachLayer__
#define __SingleEye__TeachLayer__

#include "GameInclude.h"

USING_NS_CC;
USING_NS_CC_EXT;

typedef enum
{
    TEACH_LAYER_TAG_CLIPNODE = 1000,
    TEACH_LAYER_TAG_ANIMATION,
    TEACH_LAYER_TAG_INFO_PIC,
    TEACH_LAYER_TAG_LIMIT
}ETeachLayerTag;

typedef enum
{
    TEACH_LAYER_ZORDER_MASK = 0,
    TEACH_LAYER_ZORDER_INFO_BOARD,
    TEACH_LAYER_ZORDER_INFO_TEXT,
    TEACH_LAYER_ZORDER_HAND,
    TEACH_LAYER_ZORDER_LIMIT
}ETeachLayerZOrder;

class CTeachLayer : public CCLayer
{
public:
    CTeachLayer();
    ~CTeachLayer();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual void registerWithTouchDispatcher();
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    
    virtual void touchDelegateRetain();
    virtual void touchDelegateRelease();
    
    CREATE_FUNC(CTeachLayer);
    
    void setFocusProp(CCPoint pos, float r);
//    void setTextInfo(CCRect rc, const char* pFileName);
    void setPicInfo(CCPoint centerPos, const char* pPicPath, TextureResType texType = UI_TEX_TYPE_PLIST);
    void setCallbackFun(CCObject* target, SEL_CallFunc callfun);
    void setAnimation(bool bOpen);
    
    CCPoint getFocusPos(){return m_focusPos;};
    
protected:
    virtual void CBTouchGoalArea();
    void scaleActionEnd();
    void moveActionEnd();
    bool containsTouchLocation(CCTouch* touch);
    
    void initClipMask();
    void initAnimation();
    void initNoAnimation();
    
private:
    bool m_bAnimationOver;
    bool m_bOpenActionFun;//true  有遮罩聚焦和手指等动画
    
    CCPoint m_focusPos;
    float m_focusRadius;
    
    CCRect m_infoRect;
//    CCString *m_infoStr;
    
    CCObject *m_callbackListener;
    SEL_CallFunc m_callback;
};

#endif /* defined(__SingleEye__TeachLayer__) */
