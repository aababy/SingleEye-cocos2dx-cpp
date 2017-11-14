//
//  BuyGemScene.cpp
//  SingleEye
//
//  Created by wangwx on 13-12-16.
//
//

#include "BuyGemScene.h"

USING_NS_CC;
USING_NS_CC_EXT;

CBuyGemTableViewCell::CBuyGemTableViewCell(int gemNum, int rmbPrice, ELocalId localId)
: m_nGemNum(gemNum), m_nRmbPrice(rmbPrice), m_eLocal(localId)
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
        pItemIco->loadTexture("money_ico07.png", UI_TEX_TYPE_PLIST);
        
        UILabelAtlas *pNum = dynamic_cast<UILabelAtlas*>(pJsonRoot->getChildByName("LabelAtlas_Num"));
        CC_BREAK_IF(!pNum);
        pNum->setStringValue(CCString::createWithFormat("%d", gemNum)->getCString());
        
        UIImageView *pNumIco = dynamic_cast<UIImageView *>(pJsonRoot->getChildByName("ImageView_NumIco"));
        CC_BREAK_IF(!pNumIco);
        pNumIco->setPosition(ccp(pNum->getPosition().x - pNum->getContentSize().width/2 - pNumIco->getContentSize().width/2, pNum->getPosition().y));
        
        UILabelAtlas *pPrice = dynamic_cast<UILabelAtlas*>(pJsonRoot->getChildByName("LabelAtlas_Price"));
        CC_BREAK_IF(!pPrice);
        pPrice->setVisible(false);
        
        UIImageView *pPriceIco = dynamic_cast<UIImageView *>(pJsonRoot->getChildByName("ImageView_PriceIco"));
        CC_BREAK_IF(!pPriceIco);        
        
        int priceNumId = 1;
        switch (rmbPrice) {
            case 6:       
                priceNumId = 1;
                break;
            case 18:                
                priceNumId = 2;
                break;
            case 30:                
                priceNumId = 3;
                break;
        }
        switch (localId)
        {
            case LOCAL_ID_TAIWAN:
                pPriceIco->loadTexture(CCString::createWithFormat("price_tw_%d.png", priceNumId)->getCString(), UI_TEX_TYPE_PLIST);
                break;
            case LOCAL_ID_XIANGGANG:
                pPriceIco->loadTexture(CCString::createWithFormat("price_hk_%d.png", priceNumId)->getCString(), UI_TEX_TYPE_PLIST);
                break;
            case LOCAL_ID_AOMEN:
                pPriceIco->loadTexture(CCString::createWithFormat("price_mop_%d.png", priceNumId)->getCString(), UI_TEX_TYPE_PLIST);
                break;
            case LOCAL_ID_XINJIAPO:
                pPriceIco->loadTexture(CCString::createWithFormat("price_sg_%d.png", priceNumId)->getCString(), UI_TEX_TYPE_PLIST);
                break;
            case LOCAL_ID_ZHONGGUO:
                pPriceIco->loadTexture(CCString::createWithFormat("price_cn_%d.png", priceNumId)->getCString(), UI_TEX_TYPE_PLIST);
                break;
            default:
                pPriceIco->loadTexture(CCString::createWithFormat("price_en_%d.png", priceNumId)->getCString(), UI_TEX_TYPE_PLIST);
                break;
        }

        pPriceIco->setPosition(ccp(pPriceIco->getPosition().x + pPriceIco->getContentSize().width * 0.4f, pPriceIco->getPosition().y));
        
    } while (0);
}
//void CBuyGemTableViewCell::draw()
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
//        c#include "BuyTiliScene.h"cp(pos.x+1, pos.y+size.height-1),
//    };
//    ccDrawColor4B(0, 0, 255, 255);
//    ccDrawPoly(vertices, 4, true);
//}

#pragma mark - CBuyGemLayer

CBuyGemLayer::CBuyGemLayer()
{
    
}
CBuyGemLayer::~CBuyGemLayer()
{
    
}
bool CBuyGemLayer::init()
{
    return (CCLayer::init());
}
void CBuyGemLayer::onEnter()
{
    do
    {
        CCLayer::onEnter();
        
        //如果有未完成的交易验证发送验证请求
        if (UserData::Instance()->transactionCount > 0) {
            string s;
            (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_VALIDATE_GEMSTONE(NULL, s,UserData::Instance()->pUserInfo->gemstone);//刚进入游戏验证回调设置为空
        }
        
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(money_item_plist, money_item_image);
        
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
        
        //local
        m_eLocal = LOCAL_ID_EN;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        CLocalCondition *pLocal = new CLocalCondition();
        if(pLocal)
        {

            m_eLocal = pLocal->getLocalCondition();
            delete pLocal;
        }
#endif
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
        ///      pGold->setPosition(125, winSize.height - 50 - pGold->getContentSize().height);
        addChild(pGold, BUYGEMSCENE_ZORDER_PROP, BUYGEM_TAG_TOPPROP_GOLD);
        CMoneyItem *pGem = new CMoneyItem(MONEY_ITEM_TYPE_GEM, false);
        pGem->autorelease();
        ///      pGem->setPosition(winSize.width-260, winSize.height - 50 - pGem->getContentSize().height);
        addChild(pGem, BUYGEMSCENE_ZORDER_PROP, BUYGEM_TAG_TOPPROP_GEM);
        
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
        addChild(pTableView, BUYGEMSCENE_ZORDER_TABLE, BUYGEM_TAG_TABLE);
        pTableView->reloadData();
        MsgSystem::Instance()->RegisterObjAndHandler(this, MSGSYS_HANLE_MSG(CBuyGemLayer::handleMsg));
    } while (0);
}


void CBuyGemLayer::onExit()
{
//    removeChildByTag(BUYGEM_TAG_TABLE);
    this->removeAllChildrenWithCleanup(true);
    MsgSystem::Instance()->ClearObjHandler(this);
    CCLayer::onExit();
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}


bool CBuyGemLayer::handleMsg(const Message &msg)
{
    CCLOG("NetTest::HandleMsg msgType= %d", msg.m_nMsgType);
    switch (msg.m_nMsgType)
    {
        case CMD_RES_VALIDATE_GEMSTONE:
        {
            if (xUser->isSuccess) {
                xDialog->show(xStr("buy_success"));  //自定义消息
   ///             xData->setBagGemNum(xUser->pUserInfo->gemstone);
                xData->saveArchive();
                freshPropUI();
            }
            else
            {
                xDialog->show(xUser->message.c_str());
            }
            
            break;
        }
        default:
            break;
    }
    
    return true;
}


void CBuyGemLayer::CBBack(CCObject *pSender)
{
    xData->playKeytone();
    xSM->back();
}
void CBuyGemLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    xData->playKeytone();
    
    CCLOG("cell touched at index: %i", cell->getIdx());
    CBuyGemTableViewCell *pCell = (CBuyGemTableViewCell *)cell;
    
    if (pCell)
    {
        TargetPlatform platform = CCApplication::sharedApplication()->getTargetPlatform();
        
        if (platform == kTargetIphone || platform == kTargetIpad ) {
#if TARGET_IPHONE_SIMULATOR
            //模拟器实现不了, 等待网络模块..
#elif TARGET_OS_IPHONE
            xNet->DO_CMD_REQ_BUY_GEMSTONE(this, cell->getIdx() + 1, xUser->pUserInfo->gemstone);
#endif
        }                
    }
}

CCSize CBuyGemLayer::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
    CCSprite *pBoard = CCSprite::createWithSpriteFrameName(money_item_board);
    if(pBoard)
    {
        return CCSizeMake(pBoard->getContentSize().width, pBoard->getContentSize().height + 40);
    }
    
    return CCSizeMake(500, 120);
}

//todo 修改成读表动态载入
CCTableViewCell* CBuyGemLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    int nGem = 0;
    int nRmb = 0;
    switch (idx)
    {
        case 0:
            nGem = 24;
            nRmb = 6;
            break;
        case 1:
            nGem = 78;
            nRmb = 18;
            break;
        case 2:
            nGem = 136;
            nRmb = 30;
            break;
        default:
            return NULL;
    }
    CCTableViewCell *pCell = new CBuyGemTableViewCell(nGem, nRmb, m_eLocal);
    pCell->autorelease();
    //   CCLOG("outfit=%d,%d", idx, m_nBagCell[idx]);
    return pCell;
}

unsigned int CBuyGemLayer::numberOfCellsInTableView(CCTableView *table)
{
    return BUYGEM_CELL_NUM;
}

void CBuyGemLayer::freshPropUI()
{
    CMoneyItem *pGem = (CMoneyItem *)(getChildByTag(BUYGEM_TAG_TOPPROP_GEM));
    if (pGem)
    {
        pGem->setNum(xUser->pUserInfo->gemstone);
    }
    
    CMoneyItem *pGold = (CMoneyItem *)(getChildByTag(BUYGEM_TAG_TOPPROP_GOLD));
    if (pGold)
    {
        pGold->setNum(xData->getGoldCoin());
    }

}

#pragma mark - CBuyGemScene

CBuyGemScene::CBuyGemScene()
{
    CCScene::init();
}
CBuyGemScene::~CBuyGemScene()
{
    
}

bool CBuyGemScene::init()
{
    bool bRst = false;
    
    do
    {
        CC_BREAK_IF(!CCScene::init());
        
        m_pMainLayer = CBuyGemLayer::create();
        CC_BREAK_IF(!m_pMainLayer);
        addChild(m_pMainLayer);
        bRst = true;
        
    } while (0);
    
    return bRst;
}

