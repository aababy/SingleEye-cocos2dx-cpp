
#include "BuyGoldScene.h"

USING_NS_CC;
USING_NS_CC_EXT;

CBuyGoldTableViewCell::CBuyGoldTableViewCell(int num, int price)
: m_nNum(num), m_nPrice(price)
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
        pItemIco->loadTexture("money_ico06.png", UI_TEX_TYPE_PLIST);
        
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

#pragma mark - CBuyGoldLayer

CBuyGoldLayer::CBuyGoldLayer()
{
    
}
CBuyGoldLayer::~CBuyGoldLayer()
{
    
}
bool CBuyGoldLayer::init()
{
    return (CCLayer::init());
}
void CBuyGoldLayer::onEnter()
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
        addChild(pBg, BUYGEMSCENE_ZORDER_BACKGROUND);
        
        //top bg
        CCSprite *pTopBg = CCSprite::createWithSpriteFrameName(topbar_2);
        pTopBg->setPosition(ccp(winSize.width/2, winSize.height - pTopBg->getContentSize().height/2));
        addChild(pTopBg, BUYGEMSCENE_ZORDER_BACKGROUND);
        
        //top prop
        CMoneyItem *pGold = new CMoneyItem(MONEY_ITEM_TYPE_GOLD, false);
        pGold->autorelease();
        ///      pGold->setPosition(125, winSize.height - 50 - pGoldCoin->getContentSize().height);
        addChild(pGold, BUYGEMSCENE_ZORDER_PROP, BUYGOLD_TAG_TOPPROP_GOLD);
        CMoneyItem *pMoneyItem = new CMoneyItem(MONEY_ITEM_TYPE_GEM, false);
        pMoneyItem->autorelease();
        ///      pMoneyItem->setPosition(ccp(winSize.width*3/4, winSize.height - 50 - pMoneyItem->getContentSize().height));
        addChild(pMoneyItem, BUYGEMSCENE_ZORDER_PROP, BUYGOLD_TAG_TOPPROP_GEM);
        
        //table
        CCSprite *pBoard = CCSprite::createWithSpriteFrameName(money_item_board);
        m_nTableWidth = pBoard->getContentSize().width;
        m_nTableHeight = winSize.height*2/3;
        
        //back btn
        CCMenuItemImage *pBackItem = CCMenuItemImage::create(NULL, \
                                                             NULL, \
                                                             this, \
                                                             menu_selector(CBuyGemLayer::CBBack));
        pBackItem->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(btn_back));
        pBackItem->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(btn_back2));
        pBackItem->setPosition(ccp(MIN_SPACE_TO_SCREEN_EDGE + pBackItem->getContentSize().width/2, MIN_SPACE_TO_SCREEN_EDGE + pBackItem->getContentSize().height/2));
        CCMenu *pMenu = CCMenu::create(pBackItem, NULL);
        pMenu->setPosition(CCPointZero);
        addChild(pMenu, BUYGEMSCENE_ZORDER_MENU);
        
        //table
        CCTableView* pTableView = CCTableView::create(this, CCSizeMake(m_nTableWidth, m_nTableHeight));
        pTableView->setDirection(kCCScrollViewDirectionVertical);
        pTableView->setPosition(ccp(winSize.width/2 - m_nTableWidth/2, winSize.height/2 - m_nTableHeight/2));
        pTableView->setDelegate(this);
        pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
        addChild(pTableView, BUYGEMSCENE_ZORDER_TABLE, BUYGOLD_TAG_TABLE);
        pTableView->reloadData();
        
    } while (0);
    
}

void CBuyGoldLayer::onExit()
{
 //   removeChildByTag(BUYGOLD_TAG_TABLE);
    this->removeAllChildrenWithCleanup(true);
    
    CCLayer::onExit();
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}
void CBuyGoldLayer::CBBack(CCObject *pSender)
{
    xData->playKeytone();
    xSM->back();
}
void CBuyGoldLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    xData->playKeytone();
    
//    CCLOG("cell touched at index: %i", cell->getIdx());
    CBuyGoldTableViewCell *pCell = (CBuyGoldTableViewCell *)cell;
    
    if (pCell)
    {
        if (DATAPOOL->changeBagGemNum(-pCell->m_nPrice))
        {
            DATAPOOL->changeBagGoldNum(pCell->m_nNum);
            freshPropUI();
            
            xDialog->show(xStr("buy_success"));
            
            DATAPOOL->saveArchive();
        }
        else
        {
            xDialog->showYesNo(xStr("lack_of_gem"), dialogeventselector(CDataPool::CBLackOfGem), this);
        }
        
    }
}

CCSize CBuyGoldLayer::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
    CCSprite *pBoard = CCSprite::createWithSpriteFrameName(money_item_board);
    if(pBoard)
    {
        return CCSizeMake(pBoard->getContentSize().width, pBoard->getContentSize().height + 10);
    }
    return CCSizeMake(500, 120);
}
//todo 修改成读表动态载入
CCTableViewCell* CBuyGoldLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    int nNum = 0;
    int nPrice = 0;
    switch (idx)
    {
        case 0:
            nNum = 1000;
            nPrice = 5;
            break;
        case 1:
            nNum = 2500;
            nPrice = 10;
            break;
        case 2:
            nNum = 6666;
            nPrice = 20;
            break;
        case 3:
            nNum = 25000;
            nPrice = 50;
            break;
        case 4:
            nNum = 100000;
            nPrice = 100;
            break;
        default:
            break;
    }
    CCTableViewCell *pCell = new CBuyGoldTableViewCell(nNum, nPrice);
    pCell->autorelease();
    //   CCLOG("outfit=%d,%d", idx, m_nBagCell[idx]);
    return pCell;
}

unsigned int CBuyGoldLayer::numberOfCellsInTableView(CCTableView *table)
{
    return BUYGOLD_CELL_NUM;
}

void CBuyGoldLayer::freshPropUI()
{
    CMoneyItem *pGold = (CMoneyItem *)(getChildByTag(BUYGOLD_TAG_TOPPROP_GOLD));
    if (pGold)
    {
        pGold->setNum(DATAPOOL->getGoldCoin());
    }
    CMoneyItem *pGem = (CMoneyItem *)(getChildByTag(BUYGOLD_TAG_TOPPROP_GEM));
    if (pGem)
    {
        pGem->setNum(xUser->pUserInfo->gemstone);
    }
}

#pragma mark - CBuyGoldScene

CBuyGoldScene::CBuyGoldScene()
{
    CCScene::init();
}
CBuyGoldScene::~CBuyGoldScene()
{
    
}

bool CBuyGoldScene::init()
{
    bool bRst = false;
    
    do
    {
        CC_BREAK_IF(!CCScene::init());
        
        m_pMainLayer = CBuyGoldLayer::create();
        CC_BREAK_IF(!m_pMainLayer);
        addChild(m_pMainLayer);
        bRst = true;
        
    } while (0);
    
    return bRst;
}

