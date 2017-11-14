#include "SlideMenu.h"

bool CSlideMenu::init()
{
    bool bRst = false;
    
    do
    {
        CC_BREAK_IF(!CCNode::init());
        
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        
        //board
        CCSprite *pBoard = CCSprite::createWithSpriteFrameName(set_ico_board);
        CC_BREAK_IF(!pBoard);
        addChild(pBoard, 1);
        
        //呼叫项
        CCMenuItemImage *pCallOut = CCMenuItemImage::create();
        CC_BREAK_IF(!pCallOut);
        pCallOut->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(set_ico_call_out));
        pCallOut->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(set_ico_call_out));
        CCMenuItemImage *pCallIn = CCMenuItemImage::create();
        CC_BREAK_IF(!pCallIn);
        pCallIn->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(set_ico_call_in));
        pCallIn->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(set_ico_call_in));
        CCMenuItemToggle *pCallItem = CCMenuItemToggle::createWithTarget(this,
                                                                         menu_selector(CSlideMenu::CBMenuCall),\
                                                                         pCallOut, \
                                                                         pCallIn, \
                                                                         NULL);
        CC_BREAK_IF(!pCallItem);
        pCallItem->setTag(SLIDEMENU_TAG_CALL_ITEM);
        
        //持续抖动
        CCActionInterval *actionBy = CCMoveBy::create(0.15f, CCPointMake(pCallItem->getPosition().x - pCallItem->getContentSize().width/4, pCallItem->getPosition().y));
        CCActionInterval *actionBack = actionBy->reverse();
        pCallItem->runAction(CCRepeatForever::create(CCSequence::create(actionBy, actionBack, NULL)));
        
        //set pos
        this->setContentSize(CCSize(pBoard->getContentSize().width + pCallItem->getContentSize().width, pBoard->getContentSize().height));
        pBoard->setPosition(ccp(getContentSize().width - pBoard->getContentSize().width/2, getContentSize().height/2));
        pCallItem->setPosition(ccp(pCallItem->getContentSize().width/2 + 10 , getContentSize().height/2));
        
        //设置
        CCMenuItemImage *pSheZhi = CCMenuItemImage::create(NULL, \
                                                           NULL, \
                                                           this, \
                                                           menu_selector(CSlideMenu::CBSheZhi));
        CC_BREAK_IF(!pSheZhi);
        pSheZhi->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("setting_shezhi.png"));
        pSheZhi->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("setting_shezhi.png"));
        
        //通知
        pTongZhi = CCMenuItemImage::create(NULL, \
                                           NULL, \
                                           this, \
                                           menu_selector(CSlideMenu::CBTongZhi));
        CC_BREAK_IF(!pTongZhi);
        pTongZhi->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("setting_tongzhi.png"));
        pTongZhi->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("setting_tongzhi.png"));
        
        //活动
        CCMenuItemImage *pHuoDong = CCMenuItemImage::create(NULL, \
                                                            NULL, \
                                                            this, \
                                                            menu_selector(CSlideMenu::CBHuoDong));
        CC_BREAK_IF(!pHuoDong);
        pHuoDong->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("setting_huodong.png"));
        pHuoDong->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("setting_huodong.png"));
        
        //关于
        CCMenuItemImage *pAbout = CCMenuItemImage::create(NULL, \
                                                            NULL, \
                                                            this, \
                                                            menu_selector(CSlideMenu::CBAbout));
        CC_BREAK_IF(!pAbout);
        pAbout->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("setting_about.png"));
        pAbout->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("setting_about.png"));
        
        //set pos
        int nItemNum = 4;
        float spaceBetweenItem = (pBoard->getContentSize().width - pSheZhi->getContentSize().width - pTongZhi->getContentSize().width - pHuoDong->getContentSize().width - pAbout->getContentSize().width)/(nItemNum+1);
        if (spaceBetweenItem < 0)
        {
            spaceBetweenItem = 0;
        }
        float tempPosX = pCallItem->getContentSize().width + spaceBetweenItem + pSheZhi->getContentSize().width/2;
        pSheZhi->setPosition(ccp(tempPosX, getContentSize().height/2));
        tempPosX += pSheZhi->getContentSize().width/2 + spaceBetweenItem + pTongZhi->getContentSize().width/2;
        pTongZhi->setPosition(ccp(tempPosX, getContentSize().height/2));
        tempPosX += pTongZhi->getContentSize().width/2 + spaceBetweenItem + pHuoDong->getContentSize().width/2;
        pHuoDong->setPosition(ccp(tempPosX, getContentSize().height/2));
        tempPosX += pHuoDong->getContentSize().width/2 + spaceBetweenItem + pAbout->getContentSize().width/2;
        pAbout->setPosition(ccp(tempPosX, getContentSize().height/2));
        
        //menu
        CCMenu* pMenu = CCMenu::create(pCallItem, pSheZhi, pTongZhi, pHuoDong, pAbout, NULL);
        pMenu->setPosition(CCPointZero);
        pMenu->setTag(SLIDEMENU_TAG_MENU);
        addChild(pMenu, 2);
        
        //data
        m_contentSizeWithCloseState = pCallItem->getContentSize();
        m_fSlideArea = pBoard->getContentSize().width;
        m_pBirthPosition = getPosition();
        m_eSlideState = SLIDEMENU_STATE_CLOSE;
        
        bRst = true;
    } while (0);
    
    return bRst;
}
void CSlideMenu::onEnter()
{
    CCNode::onEnter();
    newMessage(NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CSlideMenu::newMessage), NC_NewMessage, NULL);    
}


void CSlideMenu::newMessage(CCObject *obj)
{
    int iNewMsgCount = CDataPool::sharedDataPool()->getNewMessageCount();
    
    if (iNewMsgCount != 0) {
        pTongZhi->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("zcdszhi_19.png"));
        pTongZhi->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("zcdszhi_19.png"));
    }
    else
    {
        pTongZhi->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("setting_tongzhi.png"));
        pTongZhi->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("setting_tongzhi.png"));
    } 
}



void CSlideMenu::onExit()
{
    CCNode::onExit();
    
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}
//关闭状态时的contentSize
const CCSize& CSlideMenu::getContentSizeWithCloseState()
{
    return m_contentSizeWithCloseState;
}
void CSlideMenu::toShow()
{
    CMaskLayer *pParentLayer = dynamic_cast<CMaskLayer*>(this->getParent());
    pParentLayer->toShow();
    
    m_eSlideState = SLIDEMENU_STATE_SLIDE;
    m_pBirthPosition = getPosition();
    
    CCMenu* pMenu = dynamic_cast<CCMenu*>(this->getChildByTag(SLIDEMENU_TAG_MENU));
    CCMenuItemToggle *pCallItem = dynamic_cast<CCMenuItemToggle *>(pMenu->getChildByTag(SLIDEMENU_TAG_CALL_ITEM));
    if (pCallItem)
    {
        pCallItem->setSelectedIndex(1);
    }
    
    CCActionInterval* sAction = CCMoveTo::create(0.1, ccp(m_pBirthPosition.x - m_fSlideArea, m_pBirthPosition.y));
    CCCallFunc*  callFun = CCCallFunc::create(this,callfunc_selector(CSlideMenu::openSlideOver));
    runAction(CCSequence::create(sAction,callFun,NULL));

}
void CSlideMenu::toHide()
{
    CMaskLayer *pParentLayer = dynamic_cast<CMaskLayer*>(this->getParent());
    pParentLayer->toHide();
    
    m_eSlideState = SLIDEMENU_STATE_SLIDE;
    
    CCMenu* pMenu = dynamic_cast<CCMenu*>(this->getChildByTag(SLIDEMENU_TAG_MENU));
    CCMenuItemToggle *pCallItem = dynamic_cast<CCMenuItemToggle *>(pMenu->getChildByTag(SLIDEMENU_TAG_CALL_ITEM));
    if (pCallItem)
    {
        pCallItem->setSelectedIndex(0);
    }
    
    CCActionInterval* sAction = CCMoveTo::create(0.1, m_pBirthPosition);
    CCCallFunc*  callFun = CCCallFunc::create(this,callfunc_selector(CSlideMenu::closeSlideOver));
    runAction(CCSequence::create(sAction,callFun,NULL));
}
void CSlideMenu::CBMenuCall(CCObject *pSender)
{
//    CCLOG("slide menu call");
    
    switch (m_eSlideState)
    {
        case SLIDEMENU_STATE_CLOSE:
        {
            this->toShow();
        }
            break;
            
        case SLIDEMENU_STATE_OPEN:
        {
            this->toHide();
        }
            break;
    }

}
void CSlideMenu::CBSheZhi(CCObject *pSender)
{
    if(SLIDEMENU_STATE_OPEN != m_eSlideState)
    {
        return;
    }
    xData->playKeytone();
    xSM->forward(ST_Setting);
}
void CSlideMenu::CBTongZhi(CCObject *pSender)
{
    if(SLIDEMENU_STATE_OPEN != m_eSlideState)
    {
        return;
    }
    xData->playKeytone();
    xSM->forward(ST_Message);
}
void CSlideMenu::CBHuoDong(CCObject *pSender)
{
    if(SLIDEMENU_STATE_OPEN != m_eSlideState)
    {
        return;
    }
    xData->playKeytone();
    xSM->forward(ST_Huodong);
}
void CSlideMenu::CBAbout(CCObject *pSender)
{
    if(SLIDEMENU_STATE_OPEN != m_eSlideState)
    {
        return;
    }
    xData->playKeytone();
    xSM->forward(ST_About);
    
}
void CSlideMenu::openSlideOver()
{
    m_eSlideState = SLIDEMENU_STATE_OPEN;
}
void CSlideMenu::closeSlideOver()
{
    m_eSlideState = SLIDEMENU_STATE_CLOSE;
}
