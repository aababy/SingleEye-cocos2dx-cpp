//
//  HomeUILayer.cpp
//  SingleEye
//
//  Created by wangwx on 13-12-17.
//
//

#include "HomeUILayer.h"

#pragma mark - CCHomeUILayer

bool CHomeUILayer::init()
{
    bool bRst = false;
    
    do
    {
        CC_BREAK_IF(!CCLayer::init());
        
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        
        CMoneyItem *pGoldCoin = new CMoneyItem(MONEY_ITEM_TYPE_GOLD);
        pGoldCoin->autorelease();
        addChild(pGoldCoin, HOME_UI_LAYER_ZORDER_GOLD, HOMEUI_LAYER_TAG_GOLD);
        
        CMoneyItem *pGem = new CMoneyItem(MONEY_ITEM_TYPE_GEM);
        pGem->autorelease();
        addChild(pGem, HOME_UI_LAYER_ZORDER_GEM, HOMEUI_LAYER_TAG_GEM);
        
        CMaskLayer *pMaskMenu = CMaskLayer::create();
        addChild(pMaskMenu, HOME_UI_LAYER_ZORDER_SLIDEMENU, HOMEUI_LAYER_TAG_SLIDEMENU);
        
        bRst = true;
    } while (0);
    
    return bRst;
    
}

void CHomeUILayer::onEnter()
{
    CCLayer::onEnter();
    
    CMoneyItem *pGold = (CMoneyItem *)(getChildByTag(HOMEUI_LAYER_TAG_GOLD));
    if (pGold)
    {
        pGold->setNum(DATAPOOL->getGoldCoin());
    }
    CMoneyItem *pGem = (CMoneyItem *)(getChildByTag(HOMEUI_LAYER_TAG_GEM));
    if (pGem)
    {
        pGem->setNum(xUser->pUserInfo->gemstone);
    }
}

void CHomeUILayer::onExit()
{
    removeAllChildrenWithCleanup(true);
    CCLayer::onExit();
}

