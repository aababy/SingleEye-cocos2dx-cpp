#include "ShopScene.H"

USING_NS_CC;
USING_NS_CC_EXT;

#pragma mark - CShopLayer

CShopLayer::CShopLayer()
{
    
}
CShopLayer::~CShopLayer()
{

}

bool CShopLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    return true;
}
void CShopLayer::onEnter()
{
    CCLayer::onEnter();
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(shop_scene_body_plist, shop_scene_body_image);

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    //bg
    CCSprite *pBg = CCSprite::createWithSpriteFrameName(background_0);
    pBg->setPosition(ccp(winSize.width/2, winSize.height/2));
    addChild(pBg, SHOPSCENE_ZORDER_BACKGROUND);
    
    //top bg
    CCSprite *pTopBg = CCSprite::createWithSpriteFrameName(topbar_2);
    pTopBg->setPosition(ccp(winSize.width/2, winSize.height - pTopBg->getContentSize().height/2));
    addChild(pTopBg, SHOPSCENE_ZORDER_BACKGROUND);
    
    //back item
    CCMenuItemImage *pBackItem = CCMenuItemImage::create(NULL, \
                                                         NULL, \
                                                         this, \
                                                         menu_selector(CShopLayer::CBBack));
    pBackItem->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(btn_back));
    pBackItem->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(btn_back2));
    pBackItem->setPosition(ccp(MIN_SPACE_TO_SCREEN_EDGE + pBackItem->getContentSize().width/2, MIN_SPACE_TO_SCREEN_EDGE + pBackItem->getContentSize().height/2));
    //buy gem item
//    CCMenuItemImage *pBuyGemItem = CCMenuItemImage::create(NULL, \
//                                                           NULL, \
//                                                           this, \
//                                                           menu_selector(CShopLayer::CBBuyGem));
//    pBuyGemItem->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(shop_buy_gem));
//    pBuyGemItem->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(shop_buy_gem));
//    pBuyGemItem->setPosition(ccp(winSize.width - pBuyGemItem->getContentSize().width/2, pBuyGemItem->getContentSize().height/2));
    
    //menu
    CCMenu *pMenu = CCMenu::create(pBackItem, /* pBuyGemItem, */NULL);
    pMenu->setPosition(CCPointZero);
    addChild(pMenu, SHOPSCENE_ZORDER_MENU);
    
    //table
    m_pUILayer = UILayer::create();
    addChild(m_pUILayer, SHOPSCENE_ZORDER_TABLE);
    
    // shop Layout from json
    Layout *shop_root = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(shop_scene_json));
    m_offsetX = winSize.width/2 - shop_root->getContentSize().width/2;
    m_offsetY = 0;
    shop_root->setPosition(ccp(m_offsetX, m_offsetY));
    shop_root->setTag(SHOPSCENE_TAG_TABLE);
    m_pUILayer->addWidget(shop_root);

    
    // shop scrollview
    UIScrollView* shop_scrollview = static_cast<UIScrollView*>(shop_root->getChildByName("shop_ScrollView"));
    shop_scrollview->setInnerContainerSize(CCSize(720, 410));
    // shop scrollview children
    //石栅栏和铁栅栏合并
#define OUTFIT_ARRAY_NUM  (10)
    int outfitIdArray[OUTFIT_ARRAY_NUM] = {0};
    int t = OUTFIT_ARRAY_NUM - 1;
    outfitIdArray[t--] = Prop_Type_ZhaDan;
    outfitIdArray[t--] = Prop_Type_BingJing;
    outfitIdArray[t--] = Prop_Type_ShuangFeng;
    outfitIdArray[t--] = Prop_Type_HongZhaJi;
    outfitIdArray[t--] = Prop_Type_KuangBaoYao;
    outfitIdArray[t--] = Prop_Type_HuiFuYao;
    outfitIdArray[t--] = Prop_Type_ShiZhaLan;
    outfitIdArray[t--] = Prop_Type_TieDanGong;
    outfitIdArray[t--] = Prop_Type_LianNu;
    outfitIdArray[t--] = Prop_Type_ShuaXinQiu;

    for (int i = 0; i < shop_scrollview->getChildren()->count(); ++i)
    {
        int outfitId = outfitIdArray[i];
        do
        {
            COutfitData *pOutfitData = DATAPOOL->getOutfitData(outfitId);
            CC_BREAK_IF(!pOutfitData);
            
            Layout* cell_layout = static_cast<Layout*>(shop_scrollview->getChildren()->objectAtIndex(i));
            CC_BREAK_IF(!cell_layout);
            cell_layout->setTag(outfitId);
        //用release有时候会点不中    cell_layout->addReleaseEvent(this, coco_releaseselector(CShopLayer::CBTouchCell));
            cell_layout->addPushDownEvent(this, coco_pushselector(CShopLayer::CBTouchCell));
            UIImageView *ico = static_cast<UIImageView*>(cell_layout->getChildByName("ico_ImageView"));
            CC_BREAK_IF(!ico);
            if (DATAPOOL->isValidOutfitId(outfitId))
            {
                ico->setTexture(pOutfitData->m_pSourceName->getCString(), UI_TEX_TYPE_PLIST);
                ico->setVisible(true);
            }
            else
            {
                ico->setVisible(false);
                ico->setTexture(shop_item_null_board, UI_TEX_TYPE_PLIST);
            }
            UIImageView *priceImage = static_cast<UIImageView*>(cell_layout->getChildByName("price_ImageView"));
            CC_BREAK_IF(!priceImage);
            UILabelAtlas *priceLabel = static_cast<UILabelAtlas*>(cell_layout->getChildByName("price_LabelAtlas"));
            CC_BREAK_IF(!priceLabel);
            if (pOutfitData->m_nGoldPrice > 0)
            {
                priceImage->setTexture(shop_price_board_gold, UI_TEX_TYPE_PLIST);
                char priceBuf[24] = {0};
                sprintf(priceBuf, "%d", pOutfitData->m_nGoldPrice);
                priceLabel->setStringValue(priceBuf);
            }
            else if(pOutfitData->m_nGemPrice > 0)
            {
                priceImage->setTexture(shop_price_board_gem, UI_TEX_TYPE_PLIST);
                char priceBuf[24] = {0};
                sprintf(priceBuf, "%d", pOutfitData->m_nGemPrice);
                priceLabel->setStringValue(priceBuf);
            }
            
            if (Prop_Type_HuiFuYao == outfitId)
            {
                m_selectPos = ico->getPosition();
            }
            
        } while (0);
    }
#undef OUTFIT_ARRAY_NUM
    
    //buy button
    UIButton *pBuyBtn = static_cast<UIButton*>(shop_root->getChildByName("buy_Button"));
    if(pBuyBtn)
    {
        pBuyBtn->setTag(SHOPSCENE_TAG_BUY_BUTTON);
        pBuyBtn->addReleaseEvent(this, coco_releaseselector(CShopLayer::CBBuyOutfit));
        pBuyBtn->disable();
    }

//    //替换成滚动文字
//    UITextArea *pInfo = static_cast<UITextArea*>(shop_root->getChildByName("info_TextArea"));
//    if(pInfo)
//    {
//        pInfo->setVisible(false);
//        ScrollText *pScrollInfo = ScrollText::create(pInfo->getStringValue(), pInfo->getContentSize(), ccp(pInfo->getPosition().x - pInfo->getContentSize().width/2, pInfo->getPosition().y - pInfo->getContentSize().height/2));
//        pScrollInfo->setWidgetZOrder(pInfo->getWidgetZOrder());
//        pScrollInfo->setName("info_ScrollText");
//        shop_root->addChild(pScrollInfo);
//    }
    
    //top prop
    CMoneyItem *pGold = new CMoneyItem(MONEY_ITEM_TYPE_GOLD, true);
    pGold->autorelease();
    ///      pGold->setPosition(ccp(winSize.width/4, winSize.height - 50 - pGold->getContentSize().height));
    addChild(pGold, SHOPSCENE_ZORDER_PROP, SHOPSCENE_TAG_TOPPROP_GOLD);
    CMoneyItem *pGem = new CMoneyItem(MONEY_ITEM_TYPE_GEM, true);
    pGem->autorelease();
    ///       pGem->setPosition(ccp(winSize.width*3/4, winSize.height - 50 - pGem->getContentSize().height));
    addChild(pGem, SHOPSCENE_ZORDER_PROP, SHOPSCENE_TAG_TOPPROP_GEM);
    
    setCurOutfitId(0);
    setPreOutfitId(0);
    setNullCurOutfit();
    m_bAct = false;
    
    //teach layer
    if (false == DATAPOOL->m_bTeachStepOver[TEACH_STEP_SHOP_SELECT_OUTFIT])
    {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        m_pTeachLayer = CTeachLayer::create();
        m_pTeachLayer->setContentSize(winSize);
        m_pTeachLayer->setFocusProp(ccp(50, 200), 90);
        m_pTeachLayer->setPicInfo(CCPoint(winSize.width/2, winSize.height*3/4), teach_shop_select_outfit);
        m_pTeachLayer->setCallbackFun(this, callfunc_selector(CShopLayer::CBTeachEnd1));
        addChild(m_pTeachLayer, SHOPSCENE_ZORDER_TEACH, SHOPSCENE_TAG_TEACH);
    }
    
    freshGold();
    freshGem();
    checkToChangeNewOutfit();
    freshCurOutfit();
}

void CShopLayer::onExit()
{
    removeAllChildrenWithCleanup(true);
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(shop_scene_body_plist);
    CCLayer::onExit();
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}

void CShopLayer::CBTeachEnd1()
{
    setCurOutfitId(7006);//选择生命药剂
    freshCurOutfit();
    
    DATAPOOL->m_bTeachStepOver[TEACH_STEP_SHOP_SELECT_OUTFIT] = true;
    this->removeChildByTag(SHOPSCENE_TAG_TEACH, true);
    
    if (false == DATAPOOL->m_bTeachStepOver[TEACH_STEP_SHOP_BUY_OUTFIT])
    {
        Layout *shop_root = static_cast<Layout*>(m_pUILayer->getWidgetByTag(SHOPSCENE_TAG_TABLE));
        UIButton *pBuyBtn = dynamic_cast<UIButton*>(shop_root->getChildByName("buy_Button"));
        
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        m_pTeachLayer = CTeachLayer::create();
        m_pTeachLayer->setContentSize(winSize);
        if (pBuyBtn)
        {
            m_pTeachLayer->setFocusProp(ccp(m_offsetX + pBuyBtn->getPosition().x, m_offsetY + pBuyBtn->getPosition().y),pBuyBtn->getContentSize().width/2);
        }
        m_pTeachLayer->setPicInfo(CCPoint(winSize.width/2, winSize.height/4), teach_shop_buy_outfit);
        m_pTeachLayer->setCallbackFun(this, callfunc_selector(CShopLayer::CBTeachEnd2));
        addChild(m_pTeachLayer, SHOPSCENE_ZORDER_TEACH, SHOPSCENE_TAG_TEACH);
    }
}
void CShopLayer::CBTeachEnd2()
{
//不强制购买    toBuyOutfit(m_nCurId);
    
    DATAPOOL->m_bTeachStepOver[TEACH_STEP_SHOP_BUY_OUTFIT] = true;
    DATAPOOL->saveArchive();
    this->removeChildByTag(SHOPSCENE_TAG_TEACH, true);
}
void CShopLayer::CBTouchCell(CCObject *pSender)
{
    xData->playKeytone();
    
//    if(m_bAct)
//    {
//        return;
//    }
    
    Layout* cell_layout = static_cast<Layout*>(pSender);
    int tag = cell_layout->getTag();
    if (DATAPOOL->isValidOutfitId(tag))
    {
        setCurOutfitId(tag);
        
        freshCurOutfit();
        
        //
        Layout *shop_root = static_cast<Layout*>(m_pUILayer->getWidgetByTag(SHOPSCENE_TAG_TABLE));
        m_actPos = ccp(cell_layout->getPosition().x + m_offsetX + cell_layout->getContentSize().width/2,cell_layout->getPosition().y + m_offsetY + cell_layout->getContentSize().height);
  //      CCLOG("d[w_x]b ^^^^^ curpos=%f,%f",m_curOutfitPos.x, m_curOutfitPos.y);
    }
    else
    {
        setNullCurOutfit();
    }
    CCLOG("touch cell,%d", tag);
}
void CShopLayer::CBBack(CCObject* pSender)
{
    xData->playKeytone();
    xData->saveArchive();
    xSM->back();
}
void CShopLayer::CBBuyGem(CCObject *pSender)
{
    xSM->forward(ST_BuyGem);
}
void CShopLayer::CBBuyOutfit(CCObject *pSender)
{
    xData->playKeytone();
    
//    if(m_bAct)
//    {
//        return;
//    }
    
    toBuyOutfit(m_nCurId);
}

int CShopLayer::getCurOutfitId()
{
    return m_nCurId;
}
void CShopLayer::setCurOutfitId(int goalId)
{
    setPreOutfitId(m_nCurId);
    m_nCurId = goalId;
}
int CShopLayer::getPreOutfitId()
{
    return m_nPreId;
}
void CShopLayer::setPreOutfitId(int goalId)
{
    m_nPreId = goalId;
}
void CShopLayer::freshCurOutfit()
{
    int tag = m_nCurId;
    
    do
    {
        CC_BREAK_IF(tag <= 0);
        Layout *shop_root = static_cast<Layout*>(m_pUILayer->getWidgetByTag(SHOPSCENE_TAG_TABLE));
        CC_BREAK_IF(!shop_root);
        COutfitData *pOutfitData = DATAPOOL->getOutfitData(tag);
        CC_BREAK_IF(!pOutfitData);
        UIImageView *pIco = static_cast<UIImageView*>(shop_root->getChildByName("buy_ImageView"));
        CC_BREAK_IF(!pIco);
        UILabelAtlas *pNum = static_cast<UILabelAtlas*>(shop_root->getChildByName("buy_LabelAtlas"));
        CC_BREAK_IF(!pNum);
        
        int num = DATAPOOL->findBagOutfitNum(tag);
        pNum->setStringValue(CCString::createWithFormat("%d", num)->getCString());
        pIco->setTexture(pOutfitData->m_pSourceName->getCString(), UI_TEX_TYPE_PLIST);
        pIco->setVisible(true);
        
        UITextArea *pInfo = static_cast<UITextArea*>(shop_root->getChildByName("info_TextArea"));
        CC_BREAK_IF(!pInfo);
        pInfo->setText(pOutfitData->m_pUse->getCString());
//        ScrollText *pScrollInfo = static_cast<ScrollText*>(shop_root->getChildByName("info_ScrollText"));
//        CC_BREAK_IF(!pScrollInfo);
//        pScrollInfo->setText(pOutfitData->m_pUse->getCString());
        
        UIButton *pBuyBtn = dynamic_cast<UIButton*>(m_pUILayer->getWidgetByTag(SHOPSCENE_TAG_BUY_BUTTON));
        CC_BREAK_IF(!pBuyBtn);
        if (pOutfitData->m_nMaxNum > 0
            && num >= pOutfitData->m_nMaxNum)
        {
       //     CCLOG("shopscene disable");
            pBuyBtn->disable();
        }
        else
        {
            pBuyBtn->active();
        }
        
        //设置选中高亮效果
        UIScrollView *shop_scrollview = dynamic_cast<UIScrollView*>(shop_root->getChildByName("shop_ScrollView"));
        CC_BREAK_IF(!shop_scrollview);
        if(m_nPreId > 0)
        {
            Layout* pPreCellLayout = static_cast<Layout*>(shop_scrollview->getChildByTag(m_nPreId));
            CC_BREAK_IF(!pPreCellLayout);
            pPreCellLayout->setBackGroundImage("shop_board2.png", UI_TEX_TYPE_PLIST);
        }
        if(m_nCurId > 0)
        {
            Layout* pCurCellLayout = static_cast<Layout*>(shop_scrollview->getChildByTag(m_nCurId));
            CC_BREAK_IF(!pCurCellLayout);
            pCurCellLayout->setBackGroundImage("shop_board1.png", UI_TEX_TYPE_PLIST);
        }
//        for (int i = 0; i < shop_scrollview->getChildren()->count(); ++i)
//        {
//            Layout* cell_layout = static_cast<Layout*>(shop_scrollview->getChildren()->objectAtIndex(i));
//            if (cell_layout)
//            {
//                int outfitId = cell_layout->getTag();
//                if (outfitId == m_nCurId )
//                {
//                    cell_layout->setBackGroundImage("shop_board1.png", UI_TEX_TYPE_PLIST);
//                }
//                else
//                {
//                    cell_layout->setBackGroundImage("shop_board2.png", UI_TEX_TYPE_PLIST);
//                }
//            }
//        }

    } while (0);
}

//石栅栏购满之后变成铁栅栏
void CShopLayer::checkToChangeNewOutfit()
{
    bool bNeedFreshCurOutfit = false;
    int oldId = Prop_Type_ShiZhaLan;
    int newId = Prop_Type_TieZhaLan;
    
    do
    {
        Layout *shop_root = static_cast<Layout*>(m_pUILayer->getWidgetByTag(SHOPSCENE_TAG_TABLE));
        CC_BREAK_IF(!shop_root);
        
        UIScrollView *shop_scrollview = dynamic_cast<UIScrollView*>(shop_root->getChildByName("shop_ScrollView"));
        CC_BREAK_IF(!shop_scrollview);
        for (int i = 0; i < shop_scrollview->getChildren()->count(); ++i)
        {
            Layout* cell_layout = static_cast<Layout*>(shop_scrollview->getChildren()->objectAtIndex(i));
            if (!cell_layout)
            {
                continue;
            }

            int outfitId = cell_layout->getTag();
            
            if (oldId == outfitId)
            {
                int num = DATAPOOL->findBagOutfitNum(outfitId);
                COutfitData *pOutfitData = DATAPOOL->getOutfitData(outfitId);
                if (pOutfitData->m_nMaxNum > 0
                    && num >= pOutfitData->m_nMaxNum)
                {
                    cell_layout->setTag(newId);
                    if (oldId == m_nCurId )
                    {
                        m_nCurId = newId;
                        bNeedFreshCurOutfit = true;
                    }
                    UIImageView *ico = static_cast<UIImageView*>(cell_layout->getChildByName("ico_ImageView"));
                    COutfitData *pNewData = DATAPOOL->getOutfitData(newId);
                    if(ico && pNewData)
                    {
                        ico->setTexture(pNewData->m_pSourceName->getCString(), UI_TEX_TYPE_PLIST);
                        ico->setVisible(true);
                    }
                    
                    UIImageView *priceImage = static_cast<UIImageView*>(cell_layout->getChildByName("price_ImageView"));
                    CC_BREAK_IF(!priceImage);
                    UILabelAtlas *priceLabel = static_cast<UILabelAtlas*>(cell_layout->getChildByName("price_LabelAtlas"));
                    CC_BREAK_IF(!priceLabel);
                    if (pNewData->m_nGoldPrice > 0)
                    {
                        priceImage->setTexture(shop_price_board_gold, UI_TEX_TYPE_PLIST);
                        char priceBuf[24] = {0};
                        sprintf(priceBuf, "%d", pNewData->m_nGoldPrice);
                        priceLabel->setStringValue(priceBuf);
                    }
                    else if(pNewData->m_nGemPrice > 0)
                    {
                        priceImage->setTexture(shop_price_board_gem, UI_TEX_TYPE_PLIST);
                        char priceBuf[24] = {0};
                        sprintf(priceBuf, "%d", pNewData->m_nGemPrice);
                        priceLabel->setStringValue(priceBuf);
                    }
                }
            }

        }
        
    } while (0);
    
    if (bNeedFreshCurOutfit)
    {
        freshCurOutfit();
    }
}

void CShopLayer::setNullCurOutfit()
{
    do
    {
        Layout *shop_root = static_cast<Layout*>(m_pUILayer->getWidgetByTag(SHOPSCENE_TAG_TABLE));
        CC_BREAK_IF(!shop_root);
        UIImageView *pIco = static_cast<UIImageView*>(shop_root->getChildByName("buy_ImageView"));
        CC_BREAK_IF(!pIco);
        pIco->stopAction(false);
        pIco->setVisible(false);
        pIco->setTexture(shop_item_null_board, UI_TEX_TYPE_PLIST);
        UILabelAtlas *pNum = static_cast<UILabelAtlas*>(shop_root->getChildByName("buy_LabelAtlas"));
        CC_BREAK_IF(!pNum);
        pNum->setStringValue("0");
//        UITextArea *pInfo = static_cast<UITextArea*>(shop_root->getChildByName("info_TextArea"));
//        CC_BREAK_IF(!pInfo);
//        pInfo->setText(" ");
    } while (0);
}
void CShopLayer::freshGold()
{
    CMoneyItem *pGold = (CMoneyItem *)(getChildByTag(SHOPSCENE_TAG_TOPPROP_GOLD));
    if (pGold)
    {
        pGold->setNum(DATAPOOL->getGoldCoin());
    }
}
void CShopLayer::freshGem()
{
    CMoneyItem *pGem = (CMoneyItem *)(getChildByTag(SHOPSCENE_TAG_TOPPROP_GEM));
    if (pGem)
    {
        pGem->setNum(xUser->pUserInfo->gemstone);
    }
}

void CShopLayer::toPopup(EShopScenePopupType type)
{
    switch (type)
    {
        case SHOPSCENE_POPUP_LACK_GOLD:
        {
            xDialog->showYesNo(xStr("lack_of_gold"), dialogeventselector(CDataPool::CBLackOfGold), this);
        }
            break;
            
        case SHOPSCENE_POPUP_LACK_GEM:
        {
            xDialog->showYesNo(xStr("lack_of_gem"), dialogeventselector(CDataPool::CBLackOfGem), this);
        }
            break;
    }
}
void CShopLayer::toBuyOutfit(int nId)
{
    do
    {
        //检查道具数量上限
        if (isReachLimit(nId)) {
            return;
        }                
        
        //扣除金币
        COutfitData *pData = DATAPOOL->getOutfitData(nId);
        CC_BREAK_IF(!pData);
        int goldPrice = pData->m_nGoldPrice;
        int gemPrice = pData->m_nGemPrice;
        bool paySuccessful = false;
        if (goldPrice > 0)
        {
            if (DATAPOOL->changeBagGoldNum(-goldPrice))
            {
                paySuccessful = true;
            }
            else
            {
                toPopup(SHOPSCENE_POPUP_LACK_GOLD);
            }
        }
        else if(gemPrice > 0)
        {
            if (DATAPOOL->changeBagGemNum(-gemPrice))
            {
                paySuccessful = true;
            }
            else
            {
                toPopup(SHOPSCENE_POPUP_LACK_GEM);
            }
        }
        
        if(paySuccessful)
        {
            //购买统计
            buyPropStatistics(nId);            
            
            DATAPOOL->changeBagOutfitNum(nId, 1);
            checkToChangeNewOutfit();
            freshCurOutfit();
            freshGem();
            freshGold();
            CCLOG("touch buy,%d",nId);
            
            //购买后，选中道具图标飘动至购买框消失
            {
                Layout *shop_root = static_cast<Layout*>(m_pUILayer->getWidgetByTag(SHOPSCENE_TAG_TABLE));
                CC_BREAK_IF(!shop_root);
                COutfitData *pData = DATAPOOL->getOutfitData(nId);
                CC_BREAK_IF(!pData);
                UIImageView *pIco = static_cast<UIImageView*>(shop_root->getChildByName("buy_ImageView"));
                CC_BREAK_IF(!pIco);
                
                CCPoint goalPos = ccp(pIco->getPosition().x + m_offsetX, pIco->getPosition().y + m_offsetY);
                
                m_bAct = true;
                
                CCSprite *pActIco = CCSprite::createWithSpriteFrameName(pData->m_pSourceName->getCString());
                pActIco->setPosition(m_actPos);
                m_pUILayer->addChild(pActIco, SHOPSCENE_ZORDER_FLY_ACT);
                CCActionInterval* moveToGoal = CCMoveTo::create(1.5, goalPos);
                CCCallFuncN*  callFun = CCCallFuncN::create(this,callfuncN_selector(CShopLayer::CBBuyActionEnd));
                pActIco->runAction(CCSequence::create(moveToGoal,callFun,NULL));
                
           //     CCLOG("d[w_x]b @@@@@@@@@@ m_curOutfitPos=%f,%f",pActIco->getPosition().x, pActIco->getPosition().y);
            }
        }
    } while (0);
}


bool CShopLayer::isReachLimit(int outfitId)
{
    OutfitRecord *pData = xData->getOutfitRecord(outfitId);
    
    if (pData->m_nNum >= 999) {                
        xDialog->show(xStr("prop_limit"));        
        return true;
    }
    else
    {
        return false;
    }
}


void CShopLayer::buyPropStatistics(int nId)
{
    string eventId;
    
    switch (nId) {
        case 7001:
            eventId = SBuyBomb;
            break;            
        case 7002:
            eventId = SBuyIce;
            break;            
        case 7003:
            eventId = SBuyFrost;
            break;            
        case 7004:
            eventId = SBuyPlane;
            break;            
        case 7005:
            eventId = SBuyFury;
            break;            
        case 7006:
            eventId = SBuyRecover;
            break;            
        case 7007:
            eventId = SBuyStoneF;
            break;    
        case 7008:
            eventId = SBuyIronF;
            break;            
        case 7009:
            eventId = SBuyIronBow;
            break;            
        case 7010:
            eventId = SBuyMetalBow;
            break;            
        case 7011:
            eventId = SBuyRefresh;
            break;             
        default:
            break;
    }

    xSocial->stat(eventId.c_str());
}


void CShopLayer::CBBuyActionEnd(CCObject *pSender)
{
    m_bAct = false;
    
    CCSprite *pActIco = (CCSprite *)(pSender);
    if(pActIco)
    {
        m_pUILayer->removeChild(pActIco, true);
    }
}

#pragma mark - CShopScene

CShopScene::CShopScene()
{
    m_pMainLayer = NULL;
}
CShopScene::~CShopScene()
{
    
}
bool CShopScene::init()
{
    bool bRst = false;
    
    do
    {
        CC_BREAK_IF(!CCScene::init());
        
        m_pMainLayer = CShopLayer::create();
		CC_BREAK_IF(!m_pMainLayer);
		addChild(m_pMainLayer);
        
        bRst = true;
        
    } while (0);

    return bRst;
}

