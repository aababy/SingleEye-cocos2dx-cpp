//
//  Block.cpp
//  SingleEye
//
//  Created by luoxp on 3/13/14.
//
//

#include "Block.h"


Block* Block::getInstance()
{
    static Block instance;
    return &instance;
}


Block* Block::create()
{
    return create(NULL, NULL);
}


Block* Block::create(SEL_BlockEvent onBlock, CCObject *target)
{
    Block *pRet = new Block();
    if (pRet)
    {
        pRet->autorelease();             
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}


Block::Block()
{
    m_onBlock = NULL;
    m_target = NULL;
    
    init();
}


bool Block::init()
{
    if (CCLayerColor::init()) {                           
        setTouchEnabled(true);
    }
    
    return true;
}


void Block::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, (kCCMenuHandlerPriority - 1), true);
}


bool Block::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    bool bRet = true;
    
    if (m_onBlock) {
        bRet = (m_target->*m_onBlock)(pTouch, pEvent, TOUCH_BEGAN);
    }
    
    return bRet;
}


void Block::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_onBlock) {
        (m_target->*m_onBlock)(pTouch, pEvent, TOUCH_ENDED);
    }
}


void Block::setOnBlock(SEL_BlockEvent onBlock, CCObject *target)
{
    m_onBlock = onBlock;
    m_target = target;
}


void Block::block(GLubyte opacity)
{
    setOpacity(opacity);
    block();
}


void Block::block()
{
    CCScene *pRunScene = CCDirector::sharedDirector()->getRunningScene();
    
    if (pRunScene && getParent() == NULL) {
        pRunScene->addChild(this, 128);
    }
}


void Block::block(SEL_BlockEvent onBlock, CCObject *target)
{
    setOnBlock(onBlock, target);
    block();
}


void Block::unblock()
{
    m_onBlock = NULL;
    
    CCScene *pRunScene = CCDirector::sharedDirector()->getRunningScene();
    
    if (pRunScene) {
        pRunScene->removeChild(this);
    }
}

