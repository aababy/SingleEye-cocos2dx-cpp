//
//  Block.h
//  SingleEye
//
//  Created by luoxp on 3/13/14.
//
//

#ifndef __SingleEye__Block__
#define __SingleEye__Block__

#include "cocos2d.h"

#define xBlock          Block::getInstance()

USING_NS_CC;


typedef enum
{
    TOUCH_BEGAN,
    TOUCH_MOVED,
    TOUCH_ENDED,
    TOUCH_CANCELED
}TouchType;


typedef bool (CCObject::*SEL_BlockEvent)(CCTouch *pTouch, CCEvent *pEvent, TouchType type);
#define blockeventselector(_SELECTOR) (SEL_BlockEvent)(&_SELECTOR)


//同时支持单件和非单件模式
class Block : public CCLayerColor
{        
public:
    static Block * getInstance();    
    static Block* create();
    static Block* create(SEL_BlockEvent onBlock, CCObject *target);
        
    void block();
    void block(GLubyte opacity);
    void unblock();    
    void block(SEL_BlockEvent onBlock, CCObject *target);    
    
private:    
    void setOnBlock(SEL_BlockEvent onBlock, CCObject *target);
    Block();
    virtual bool init();    
    
    // 需要重写触摸注册函数，重新给定触摸级别
    virtual void registerWithTouchDispatcher(void);
    // 重写触摸函数，永远返回 true ，屏蔽其它层，达到 “模态” 效果
    bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);   
    void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent); 
        
    SEL_BlockEvent      m_onBlock;
    CCObject*           m_target;
};


#endif /* defined(__SingleEye__Block__) */
