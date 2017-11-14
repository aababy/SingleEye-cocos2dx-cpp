//
//  BuyPowerScene.cpp
//  SingleEye
//
//  Created by wangwx on 13-12-16.
//
//

#include "BuyPowerScene.h"

USING_NS_CC;
USING_NS_CC_EXT;

CBuyPowerTableViewCell::CBuyPowerTableViewCell(int num, int price)
: m_nPowerNum(num), m_nRmbPrice(price)
{
    do {
        m_pUILayer = UILayer::create();
        CC_BREAK_IF(!m_pUILayer);
        addChild(m_pUILayer);
        
        Layout *pJsonRoot = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(money_item_json));
        CC_BREAK_IF(!pJsonRoot);
        m_pUILayer->addWidget(pJsonRoot);
        
        UIImageView *pItemIco = dynamic_cast<UIImageView*>(pJsonRoot->getChildByName("ImageView_ItemIco"));
        CC_BREAK_IF(!pItemIco);
        pItemIco->loadTexture("money_ico08.png", UI_TEX_TYPE_PLIST);
        
        UILabelAtlas *pNum = dynamic_cast<UILabelAtlas*>(pJsonRoot->getChildByName("LabelAtlas_Num"));
        CC_BREAK_IF(!pNum);
        pNum->setStringValue(CCString::createWithFormat("%d", num)->getCString());
        
        UIImageView *pNumIco = dynamic_cast<UIImageView *>(pJsonRoot->getChildByName("ImageView_NumIco"));
        CC_BREAK_IF(!pNumIco);
        pNumIco->setPosition(ccp(pNum->getPosition().x - pNum->getContentSize().width/2 - pNumIco->getContentSize().width/2, pNum->getPosition().y));
        
        UILabelAtlas *pPrice = dynamic_cast<UILabelAtlas*>(pJsonRoot->getChildByName("LabelAtlas_Price"));
        CC_BREAK_IF(!pPrice);
        pPrice->setStringValue(CCString::createWithFormat("%d", price)->getCString());
        
        UIImageView *pPriceIco = dynamic_cast<UIImageView *>(pJsonRoot->getChildByName("ImageView_PriceIco"));
        CC_BREAK_IF(!pPriceIco);
        pPriceIco->loadTexture("money_ico03.png", UI_TEX_TYPE_PLIST);
        pPriceIco->setPosition(ccp(pPrice->getPosition().x - pPrice->getContentSize().width/2 - pPriceIco->getContentSize().width/2, pPrice->getPosition().y));
        
    } while (0);
}
//void CBuyPowerTableViewCell::draw()
//{
//    CCTableViewCell::draw();
//    
//    // draw bounding box
//    CCPoint pos = getPosition();
//    CCSize size = CCSizeMake(500, 120);
//    CCPoint vertices[4]={
//        ccp(pos.x+1, pos.y+1),
//        ccp(pos.x+size.width-1, pos.y+1),
//        ccp(pos.x+size.width-1, pos.y+size.height-1),
//        ccp(pos.x+1, pos.y+size.height-1),
//    };
//    ccDrawColor4B(0, 0, 255, 255);
//    ccDrawPoly(vertices, 4, true);
//}

#pragma mark - CBuyPowerLayer

CBuyPowerLayer::CBuyPowerLayer()
{
    
}
CBuyPowerLayer::~CBuyPowerLayer()
{
    
}
bool CBuyPowerLayer::init()
{
    return (CCLayer::init());
}
void CBuyPowerLayer::onEnter()
{
    do
    {
        CCLayer::onEnter();
        
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(money_item_plist, money_item_image);
        
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
        
        //bg
        CCSprite *pBg = CCSprite::createWithSpriteFrameName(background_0);
        pBg->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
        addChild(pBg, BUYPOWERSCENE_ZORDER_BACKGROUND);
        
        //top bg
        CCSprite *pTopBg = CCSprite::createWithSpriteFrameName(topbar_2);
        pTopBg->setPosition(ccp(winSize.width/2, winSize.height - pTopBg->getContentSize().height/2));
        addChild(pTopBg, BUYPOWERSCENE_ZORDER_BACKGROUND);
        
        //top prop
        CMoneyItem *pGold = new CMoneyItem(MONEY_ITEM_TYPE_POWER, false);
        pGold->autorelease();
        ///      pGold->setPosition(ccp(winSize.width/4, winSize.height - 50 - pGold->getContentSize().height));
        addChild(pGold, BUYPOWERSCENE_ZORDER_PROP, BUYPOWER_TAG_TOPPROP_GOLD);
        CMoneyItem *pPower = new CMoneyItem(MONEY_ITEM_TYPE_GEM, false);
        pPower->autorelease();
        ///       pPower->setPosition(ccp(winSize.width*3/4, winSize.height - 50 - pPower->getContentSize().height));
        addChild(pPower, BUYPOWERSCENE_ZORDER_PROP, BUYPOWER_TAG_TOPPROP_POWER);
        
        //table
        CCSprite *pBoard = CCSprite::createWithSpriteFrameName(money_item_board);
        m_nTableWidth = pBoard->getContentSize().width;
        m_nTableHeight = winSize.height*2/3;
        
        //back btn
        CCMenuItemImage *pBackItem = CCMenuItemImage::create(NULL, \
                                                             NULL, \
                                                             this, \
                                                             menu_selector(CBuyPowerLayer::CBBack));
        pBackItem->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(btn_back));
        pBackItem->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(btn_back2));
        pBackItem->setPosition(ccp(MIN_SPACE_TO_SCREEN_EDGE + pBackItem->getContentSize().width/2, MIN_SPACE_TO_SCREEN_EDGE + pBackItem->getContentSize().height/2));
        CCMenu *pMenu = CCMenu::create(pBackItem, NULL);
        pMenu->setPosition(CCPointZero);
        addChild(pMenu, BUYPOWERSCENE_ZORDER_MENU);
        
        //table
        CCTableView* pTableView = CCTableView::create(this, CCSizeMake(m_nTableWidth, m_nTableHeight));
        pTableView->setDirection(kCCScrollViewDirectionVertical);
        pTableView->setPosition(ccp(winSize.width/2 - m_nTableWidth/2, winSize.height/2 - m_nTableHeight/2));
        pTableView->setDelegate(this);
        pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
        addChild(pTableView, BUYPOWERSCENE_ZORDER_TABLE, BUYPOWER_TAG_TABLE);
        pTableView->reloadData();
    } while (0);

    
    MsgSystem::Instance()->RegisterObjAndHandler(this, MSGSYS_HANLE_MSG(CBuyPowerLayer::handleMsg));
}

void CBuyPowerLayer::onExit()
{
//    removeChildByTag(BUYPOWER_TAG_TABLE);
    removeAllChildrenWithCleanup(true);
    
    CCLayer::onExit();
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}


bool CBuyPowerLayer::handleMsg(const Message &msg)
{
    switch (msg.m_nMsgType)
    {
        case CMD_RES_BUY_STRENGTH:
        {
            //购买体力, 客户端自己计算
            xData->changeBagGemNum(m_iCost * -1);   //花费宝石
            xUserInfo->strength += m_iPower;        //增加体力
            CCLOG("##### %d", xUserInfo->strength);
            
            //更新数据
            DATAPOOL->saveArchive();
            
            //更新UI
            freshPower();
            
            xDialog->showYes(xStr("buy_success"), dialogeventselector(CBuyPowerLayer::onDialog), this);
            xDialog->setDialogID(10);
            
            break;
        }
        default:
            break;
    }
    
    return true;
}


void CBuyPowerLayer::CBBack(CCObject *pSender)
{
    xData->playKeytone();    
    xSM->back();
}

void CBuyPowerLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{    
    xData->playKeytone();
    
    CCLOG("cell touched at index: %i", cell->getIdx());
    CBuyPowerTableViewCell *pCell = (CBuyPowerTableViewCell *)cell;
    
    if (pCell)
    {
        m_iPower = 0;
        m_iCost = 0;
        
        getPowerNumber(cell->getIdx(), &m_iPower, &m_iCost);
        
        if (xUser->pUserInfo->gemstone < m_iCost) {
            xDialog->showYesNo(xStr("gem_tip"), dialogeventselector(CBuyPowerLayer::onDialog), this);
        }
        else
        {            
            if (xUserInfo->strength + m_iPower > 99) {
                xDialog->show(xStr("power_limit"));
            }
            else
            {
                xNet->DO_CMD_REQ_BUY_STRENGTH(this, xUser->pUserInfo->strength, m_iPower, m_iCost, xUserInfo->gemstone);
            }
        }
    }
}


void CBuyPowerLayer::onDialog(int id, int iTag)
{
    if (id == 10) {
        return;
    }
    
    switch (iTag) {
        case 0:
            //跳转商店界面
            xSM->forward(ST_BuyGem);
            break;
            
        default:
            break;
    }
}


CCSize CBuyPowerLayer::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
    CCSprite *pBoard = CCSprite::createWithSpriteFrameName(money_item_board);
    if(pBoard)
    {
        return CCSizeMake(pBoard->getContentSize().width, pBoard->getContentSize().height + 10);
    }
    return CCSizeMake(500, 120);
}

CCTableViewCell* CBuyPowerLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    int nPower = 0;
    int nRmb = 0;
    
    getPowerNumber(idx, &nPower, &nRmb);
    
    CCTableViewCell *pCell = new CBuyPowerTableViewCell(nPower, nRmb);
    pCell->autorelease();
    //   CCLOG("outfit=%d,%d", idx, m_nBagCell[idx]);
    return pCell;
}


void CBuyPowerLayer::getPowerNumber(int idx, int *nPower, int *nRmb)
{
    switch (idx)
    {
        case 0:
            *nPower = 1;
            *nRmb = 2;
            break;
        case 1:
            *nPower = 5;
            *nRmb = 4;
            break;
        case 2:
            *nPower = 10;
            *nRmb = 7;
            break;
        case 3:
            *nPower = 20;
            *nRmb = 10;
            break;
        default:
            break;
    }
}


unsigned int CBuyPowerLayer::numberOfCellsInTableView(CCTableView *table)
{
    return BUYPOWER_CELL_NUM;
}

void CBuyPowerLayer::freshPower()
{
    CMoneyItem *pPower = (CMoneyItem *)(getChildByTag(BUYPOWER_TAG_TOPPROP_GOLD));
    if (pPower)
    {
        pPower->setNum(xUser->pUserInfo->strength);
    }
    
    CMoneyItem *pGold = (CMoneyItem *)(getChildByTag(BUYPOWER_TAG_TOPPROP_POWER));
    if (pGold)
    {
        pGold->setNum(xUser->pUserInfo->gemstone);
    }
}

#pragma mark - CBuyPowerScene

CBuyPowerScene::CBuyPowerScene()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("PvPScene/Power/PowerScene.plist");
    CCScene::init();
}


CBuyPowerScene::~CBuyPowerScene()
{
    
}


bool CBuyPowerScene::init()
{
    bool bRst = false;
    
    do
    {
        CC_BREAK_IF(!CCScene::init());
        
        m_pMainLayer = CBuyPowerLayer::create();
        
        CC_BREAK_IF(!m_pMainLayer);
        addChild(m_pMainLayer);
        bRst = true;
        
    } while (0);
    
    return bRst;
}



