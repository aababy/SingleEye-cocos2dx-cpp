//
//  HomeSceneControlLayer.h
//  SingleEye
//
//  Created by wangwx on 13-12-17.
//
//

#ifndef __SingleEye__HomeControlLayer__
#define __SingleEye__HomeControlLayer__

#include "GameInclude.h"
#include "HomeAster.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define HOME_ROTATION_STEP_BASE     (0.1)
#define HOME_ROTATION_STEP_MAX      (2.0)
#define HOME_ROTATION_MAX_RANGE     (30.0)
#define HOME_ROTATION_MIN_RANGE     (-30.0)

class CHomeAster;

class CHomeControlLayer : public CCNode, public CCTargetedTouchDelegate
{
public:
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    
    virtual void touchDelegateRetain();
    virtual void touchDelegateRelease();
    
    CREATE_FUNC(CHomeControlLayer);
    
    void rotationTo(float rotation);
    void rotationBy(float fStep);
    
private:
    bool containsTouchLocation(CCTouch* touch);
    
    float m_tScrollDistance;                    //保存最后一次滑动距离
    void deaccelerateScrolling(float dt);
    
public:
    bool m_bIscontrol;
    float m_fRotation;
    
    CHomeAster *m_pAster;
};

#endif /* defined(__SingleEye__HomeControlLayer__) */
