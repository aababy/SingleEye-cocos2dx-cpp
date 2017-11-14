#include "SelectPetScene.H"

USING_NS_CC;
USING_NS_CC_EXT;


#pragma mark - pet

CSelectPetLayer::CSelectPetLayer(void)
{
    
}
CSelectPetLayer::~CSelectPetLayer(void)
{
}
bool CSelectPetLayer::init()
{
    return (CCLayer::init());
}
void CSelectPetLayer::onEnter()
{
    CCLayer::onEnter();
    
    xNetEngine->setBusyEnabled(false);
    
    if ( !DATAPOOL->m_bTeachStepOver[TEACH_STEP_ENTER_SELECT_PET] )
    {
        DATAPOOL->m_bTeachStepOver[TEACH_STEP_ENTER_SELECT_PET] = true;
    }
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(plan_scene_body_plist, plan_scene_body_image);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(tavern_scene_body_plist, tavern_scene_body_image);

    m_nCurPageIndex = 0;
    m_nCurPageTag = 0;
    m_bLockOperationBtn = false;
    
    PetInit();
    PetFresh();
    
    freshChuzhanBtn();
    freshArrowUI();
    freshPageProp();
    freshLevelStarUI();
    
    freshOperationData();
    freshOperationUI(m_nCurPageTag);
    
    if(false == DATAPOOL->m_bTeachStepOver[TEACH_STEP_SELECT_PET]
       && DATAPOOL->m_bTeachStepOver[TEACH_STEP_ENTER_SELECT_PET]
       && 0 == DATAPOOL->currentChapter
       && 5 == xData->m_nCurrentLevel[0])
    {
        Layout *pJsonRoot = static_cast<Layout*>(m_pUILayer->getWidgetByTag(PLAN_PET_LAYER_TAG_BOARD_JSON));
        UIButton *pOperationBtn = dynamic_cast<UIButton*>(pJsonRoot->getChildByName("operation_Button"));
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        m_pTeachLayer = CTeachLayer::create();
        m_pTeachLayer->setContentSize(winSize);
        if(pOperationBtn)
        {
            CCPoint pos = ccp(pJsonRoot->getPosition().x + pOperationBtn->getPosition().x + pOperationBtn->getContentSize().width/2 - 85, pJsonRoot->getPosition().y + pOperationBtn->getPosition().y + pOperationBtn->getContentSize().height/2);
            m_pTeachLayer->setFocusProp(pos, pOperationBtn->getContentSize().width/2 + 20);
        }
        m_pTeachLayer->setPicInfo(CCPoint(winSize.width/2, winSize.height*3/4), teach_to_upgrade_pet);
        m_pTeachLayer->setCallbackFun(this, callfunc_selector(CSelectPetLayer::CBTeachEnd));
        addChild(m_pTeachLayer, PLAN_PET_LAYER_ZORDER_TEACH, PLAN_PET_LAYER_TAG_TEACH);
    }
    
}
void CSelectPetLayer::onExit()
{
    xNetEngine->setBusyEnabled(true);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(plan_scene_body_plist);
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(tavern_scene_body_plist);
    
    CCLayer::onExit();
    HelpMethod::removeUnusedResourceAtExit(this);
}
void CSelectPetLayer::PetInit()
{
    do
    {
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        
        CCSprite *pBg = CCSprite::createWithSpriteFrameName(background_0);
        pBg->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
        addChild(pBg, PLAN_PET_LAYER_ZORDER_BG);
        
        CCLayerColor *pMask = CCLayerColor::create(ccc4(0,0,0,110));
        addChild(pMask, PLAN_PET_LAYER_ZORDER_MASK);
        
        //top bg
        CCSprite *pTopBg = CCSprite::createWithSpriteFrameName(topbar_1);
        pTopBg->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height - pTopBg->getContentSize().height/2));
        addChild(pTopBg, PLAN_PET_LAYER_ZORDER_TOPBG);
        
        //top name
        CCSprite *pTopName = CCSprite::createWithSpriteFrameName(plan_topbar_selpet);
        pTopName->setPosition(ccp(winSize.width/2 + 10, winSize.height  - pTopBg->getContentSize().height/2 - 12));
        addChild(pTopName, PLAN_PET_LAYER_ZORDER_TOPNAME);
        
        //back menu
        CCMenuItemImage *pBackItem = CCMenuItemImage::create(NULL, \
                                                             NULL, \
                                                             this, \
                                                             menu_selector(CSelectPetLayer::CBPetClose));
        pBackItem->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(btn_back));
        pBackItem->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(btn_back2));
        pBackItem->setPosition(ccp(MIN_SPACE_TO_SCREEN_EDGE + pBackItem->getContentSize().width/2, MIN_SPACE_TO_SCREEN_EDGE + pBackItem->getContentSize().height/2));
        CCMenu *pMenu = CCMenu::create(pBackItem, NULL);
        pMenu->setPosition(CCPointZero);
        addChild(pMenu, PLAN_PET_LAYER_ZORDER_MENU);
        
        m_pUILayer = UILayer::create();
        addChild(m_pUILayer, PLAN_PET_LAYER_ZORDER_JSON);
        
        //board
        UILayout *pRoot = dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile(tavern_scene_body_json));
        CC_BREAK_IF(!pRoot);
        pRoot->setPosition(ccp(winSize.width/2 - pRoot->getContentSize().width/2, winSize.height/2 - pRoot->getContentSize().height/2));
        pRoot->setTag(PLAN_PET_LAYER_TAG_BOARD_JSON);
        m_pUILayer->addWidget(pRoot);
        
        //close button
        UIButton *pCloseBtn = dynamic_cast<UIButton*>(pRoot->getChildByName("close_Button"));
        CC_BREAK_IF(!pCloseBtn);
        pCloseBtn->disable();
        pCloseBtn->setVisible(false);
        
        //tab button
        UIButton *pTabHeroBtn = dynamic_cast<UIButton*>(pRoot->getChildByName("tabHero_Button"));
        CC_BREAK_IF(!pTabHeroBtn);
        pTabHeroBtn->disable();
        pTabHeroBtn->setVisible(false);
        UIButton *pTabPetBtn = dynamic_cast<UIButton*>(pRoot->getChildByName("tabPet_Button"));
        CC_BREAK_IF(!pTabPetBtn);
        pTabPetBtn->disable();
        pTabPetBtn->setVisible(false);
        
        //left button
        UIButton *pLeftBtn = dynamic_cast<UIButton*>(pRoot->getChildByName("leftArrow_Button"));
        CC_BREAK_IF(!pLeftBtn);
        pLeftBtn->setTag(PLAN_PET_LAYER_TAG_LEFTARROW);
        pLeftBtn->addReleaseEvent(this, coco_releaseselector(CSelectPetLayer::CBLeftArrow));
        pLeftBtn->setPressedActionEnabled(true);
        
        //right button
        UIButton *pRightBtn = dynamic_cast<UIButton*>(pRoot->getChildByName("rightArrow_Button"));
        CC_BREAK_IF(!pRightBtn);
        pRightBtn->setTag(PLAN_PET_LAYER_TAG_RIGHTARROW);
        pRightBtn->addReleaseEvent(this, coco_releaseselector(CSelectPetLayer::CBRightArrow));
        pRightBtn->setPressedActionEnabled(true);
        
        //operation button
        UIButton *pOperationBtn = dynamic_cast<UIButton*>(pRoot->getChildByName("operation_Button"));
        CC_BREAK_IF(!pOperationBtn);
        pOperationBtn->addReleaseEvent(this, coco_releaseselector(CSelectPetLayer::CBToOperation));
        pOperationBtn->setPressedActionEnabled(false);
        
        // Create the page view
        m_pPageView = UIPageView::create();
        Layout* pJsonRootLayout = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(tavern_scene_page_json));
        Layout *pPageLayout = dynamic_cast<Layout*>(pJsonRootLayout->getChildByName("page_Panel"));
        CCSize pageSize = pPageLayout->getContentSize();
        m_pPageView->setSize(pageSize);
        m_pPageView->setPosition(ccp((winSize.width - pageSize.width) / 2,
                                     (winSize.height - pageSize.height) / 2) );
        m_pPageView->addPageTurningEvent(this, coco_PageView_PageTurning_selector(CSelectPetLayer::CBPageViewTurningEvent));
        m_pUILayer->addWidget(m_pPageView);
        
        //chuzhan btn
        UIButton *pChuZhanBtn = UIButton::create();
        pChuZhanBtn->loadTextures(btn_chuzhan, btn_chuzhan2, btn_chuzhan2, UI_TEX_TYPE_PLIST);
        pChuZhanBtn->setPosition(ccp(winSize.width - MIN_SPACE_TO_SCREEN_EDGE - pChuZhanBtn->getContentSize().width/2,
                                     MIN_SPACE_TO_SCREEN_EDGE + pChuZhanBtn->getContentSize().height/2));
        pChuZhanBtn->setTag(PLAN_PET_LAYER_TAG_CHUZHAN);
        pChuZhanBtn->setTouchEnable(true);
        pChuZhanBtn->addReleaseEvent(this, coco_releaseselector(CSelectPetLayer::CBPetChuZhan));
        m_pUILayer->addWidget(pChuZhanBtn);
        
    }while(0);
}
void CSelectPetLayer::InitPetPage(Layout* pLayout, int nId)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    do
    {
        CC_BREAK_IF(!pLayout);
        
        Enemy_Data *pPetData = DATAPOOL->getEnemyData(nId);
        CC_BREAK_IF(!pPetData);
        EnemyRecord *pPetRcd = DATAPOOL->getEnemyRecord(nId);
        CC_BREAK_IF(!pPetRcd);
        
        //头像
        UIImageView *pHeadImagView = static_cast<UIImageView*>(pLayout->getChildByName("HeadImageView"));
        if(pHeadImagView)
        {
            if (pPetRcd->m_bUnlock)
            {
                pHeadImagView->setTexture(CCString::createWithFormat(juese_format_head, nId)->getCString(), UI_TEX_TYPE_PLIST);
            }
            else
            {
                pHeadImagView->setTexture(CCString::createWithFormat(juese_format_head_gray, nId)->getCString(), UI_TEX_TYPE_PLIST);
            }
        }
        //已出战标识
        UIImageView *pYiChuZhanImagView = static_cast<UIImageView*>(pLayout->getChildByName("yichuzhan_ImageView"));
        if(pYiChuZhanImagView)
        {
            pYiChuZhanImagView->setVisible(DATAPOOL->isPetBeenPlan(nId));
        }
        //名称
        UIImageView *pNameImagView = static_cast<UIImageView*>(pLayout->getChildByName("NameImageView"));
        if(pNameImagView)
        {
            pNameImagView->setTexture(CCString::createWithFormat(juese_format_name, nId)->getCString(), UI_TEX_TYPE_PLIST);
        }
        //等级
        UIPanel *pLevelStarPanel = static_cast<UIPanel *>(pLayout->getChildByName("StarPanel_1"));
        if(pLevelStarPanel)
        {
            int nLevel = DATAPOOL->findHeroLevel(nId);
            for (int m = 0; m < nLevel; m++)
            {
                UICheckBox *pCheckBox = static_cast<UICheckBox *>(pLevelStarPanel->getChildByName(CCString::createWithFormat("CheckBox_%d", (m+1))->getCString()));
                pCheckBox->setSelectedState(true);
            }
        }
        //伤害
        UIPanel *pAttackStarPanel = static_cast<UIPanel *>(pLayout->getChildByName("StarPanel_2"));
        if(pAttackStarPanel)
        {
            int nAttack = pPetData->mStar;
            for (int n = 0; n < nAttack; n++)
            {
                UICheckBox *pCheckBox = static_cast<UICheckBox *>(pAttackStarPanel->getChildByName(CCString::createWithFormat("CheckBox_%d", (n+1))->getCString()));
                pCheckBox->setSelectedState(true);
            }
        }
        //技能
        UILabel *pJiNengText = dynamic_cast<UILabel*>(pLayout->getChildByName("jineng_Label"));
        if (pJiNengText)
        {
            pJiNengText->setText(xStr(CCString::createWithFormat("magicId_%d", pPetData->mMagicID)->getCString()));
        }
        //解锁条件
        UILabel *pTiaoJianText = dynamic_cast<UILabel*>(pLayout->getChildByName("tiaojian_Label"));
        if (pTiaoJianText)
        {
            pTiaoJianText->setText(xStr(CCString::createWithFormat("tiaojian_%d", pPetData->mBuy)->getCString()));
        }
        //hp bar
        UILoadingBar *pHpBar = dynamic_cast<UILoadingBar*>(pLayout->getChildByName("hp_LoadingBar"));
        if (pHpBar)
        {
            //小于10的时候进度条显示错误 所以最小值显示为10
            pHpBar->setPercent((pPetRcd->m_nHpPercent>10)?(pPetRcd->m_nHpPercent):(10));
        }
        UIButton *pHpBtn = dynamic_cast<UIButton*>(pLayout->getChildByName("hpbar_Button"));
        if (pHpBtn)
        {
            pHpBtn->setVisible(false);
            pHpBtn->disable();
        }
        
        //set id
        pLayout->setTag(nId);
        
    } while (0);
    
}
void CSelectPetLayer::PetFresh()
{
    int petNum = CFG_DATA_MGR->getEnemyNum();
    if (petNum > 0)
    {
        m_pPageView->removeAllPages();
    }
    bool bUnlock = false;
    Layout* pLayoutModel = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(tavern_scene_page_json));
    for (int i = 0; i < petNum; ++i)
    {
        int nId = i + First_Enemy_ID;
        
        if (!DATAPOOL->isPet(nId))
        {
            continue;
        }
        
        Layout* pNewLayout = static_cast<Layout*>(pLayoutModel->clone());
        if (pNewLayout)
        {
            InitPetPage(pNewLayout, nId);
            m_pPageView->addPage(pNewLayout);
        }
    }
}
void CSelectPetLayer::CBPageViewTurningEvent(CCObject *pSender)
{
    freshPageProp();
    freshChuzhanBtn();
    freshArrowUI();
    freshLevelStarUI();
    freshOperationData();
    freshOperationUI(m_nCurPageTag);
}
void CSelectPetLayer::CBLeftArrow(CCObject* pSender)
{
    if (m_pPageView)
    {
        int nCurPage = m_pPageView->getPage();
        CCArray *pPageArray = m_pPageView->getPages();
        int nPageNum = pPageArray->count();
        if (nCurPage > 0 && nPageNum > 0)
        {
            m_pPageView->scrollToPage(nCurPage - 1);
             xData->playKeytone();
        }
    }
}
void CSelectPetLayer::CBRightArrow(CCObject* pSender)
{
    if (m_pPageView)
    {
        int nCurPage = m_pPageView->getPage();
        CCArray *pPageArray = m_pPageView->getPages();
        int nPageNum = pPageArray->count();
        if (nCurPage < (nPageNum - 1) && nPageNum > 0 )
        {
            m_pPageView->scrollToPage(nCurPage + 1);
             xData->playKeytone();
        }
    }
}
void CSelectPetLayer::CBWaitOperation(float dt)
{
    m_bLockOperationBtn = false;
}
void CSelectPetLayer::CBToOperation(CCObject* pSender)
{
    if (m_bLockOperationBtn)
    {
        return;
    }
    m_bLockOperationBtn = true;
    
    xData->playKeytone();
    
    int nPetId = m_nCurPageTag;
    
    switch (m_nOperationIndex)
    {
        case TAVERN_OP_TYPE_UNLOCK://解锁
            unlockPet(nPetId);
            break;
        case TAVERN_OP_TYPE_TRAIN://训练
            trainPet(nPetId);
            break;
    }
    
    this->scheduleOnce(schedule_selector(CSelectPetLayer::CBWaitOperation), 0.8f);
    
}
void CSelectPetLayer::CBPetChuZhan(CCObject* pSender)
{
    int nCurPageIndex = m_pPageView->getCurPageIndex();
    UILayout* pCurPage = m_pPageView->getPage(nCurPageIndex);
    int nTag = pCurPage->getTag();
    //    CCMessageBox((CCString::createWithFormat("page = %d", nTag))->getCString(), "page change");
    
    if (DATAPOOL->m_nDefenseSetGoalIndex_1 >= 0)//第一防线
    {
   //     CCLOG("chuzhan pet %d,%d",nTag,DATAPOOL->m_nDefenseSetGoalIndex_1);
        DATAPOOL->setPlanPet(DATAPOOL->m_nDefenseSetGoalIndex_1, nTag, 1);
    }
    else//第二防线
    {
   //     CCLOG("chuzhan pet %d,%d",nTag,DATAPOOL->m_nDefenseSetGoalIndex_2);
        DATAPOOL->setPlanPet(DATAPOOL->m_nDefenseSetGoalIndex_2, nTag, 2);
    }
    
    if ( !DATAPOOL->m_bTeachStepOver[TEACH_STEP_SELECT_PET] )
    {
        DATAPOOL->m_bTeachStepOver[TEACH_STEP_SELECT_PET] = true;
    }
    
    xData->playKeytone();
    xSM->back();
}

void CSelectPetLayer::CBPetClose(CCObject* pSender)
{
    xData->playKeytone();
    xSM->back();
}
void CSelectPetLayer::CBTeachEnd()
{
    DATAPOOL->m_bTeachStepOver[TEACH_STEP_SELECT_PET] = true;
    DATAPOOL->saveArchive();
    
    this->removeChildByTag(PLAN_PET_LAYER_TAG_TEACH, true);
    
}

//刷新显示，是否已经出战
void CSelectPetLayer::freshChuzhanBtn()
{
    UIButton *pChuZhanBtn = (UIButton *)dynamic_cast<UIButton*>(m_pUILayer->getWidgetByTag(PLAN_HERO_LAYER_TAG_CHUZHAN));
    if (pChuZhanBtn)
    {
        int nCurPageIndex = m_pPageView->getCurPageIndex();
        UILayout* pCurPage = m_pPageView->getPage(nCurPageIndex);
        if(pCurPage)
        {
            int nId = pCurPage->getTag();
            EnemyRecord *pPetRcd = DATAPOOL->getEnemyRecord(nId);
            
            if (pPetRcd && pPetRcd->m_bUnlock)
            {
//                if (DATAPOOL->isPetBeenPlan(nId))
//                {
//                    pChuZhanBtn->disable();
//                    pChuZhanBtn->setVisible(false);
//                }
//                else
                {
                    pChuZhanBtn->active();
                    pChuZhanBtn->setVisible(true);
                }
            }
            else
            {
                pChuZhanBtn->disable();
                pChuZhanBtn->setVisible(false);
            }
            
            //已出战标识
            UIImageView *pYiChuZhanImagView = static_cast<UIImageView*>(pCurPage->getChildByName("yichuzhan_ImageView"));
            if(pYiChuZhanImagView)
            {
                pYiChuZhanImagView->setVisible(DATAPOOL->isPetBeenPlan(nId));
            }
        }
        else
        {
            pChuZhanBtn->disable();
            pChuZhanBtn->setVisible(false);
        }
    }
}

void CSelectPetLayer::freshArrowUI()
{
    do {
        //left button
        UIButton *pLeftBtn = dynamic_cast<UIButton*>(m_pUILayer->getWidgetByTag(PLAN_PET_LAYER_TAG_LEFTARROW));
        CC_BREAK_IF(!pLeftBtn);
        
        //right button
        UIButton *pRightBtn = dynamic_cast<UIButton*>(m_pUILayer->getWidgetByTag(PLAN_PET_LAYER_TAG_RIGHTARROW));
        CC_BREAK_IF(!pRightBtn);
        
        //pageview
        CC_BREAK_IF(!m_pPageView);
        
        int nCurPage = m_pPageView->getPage();
        CCArray *pPageArray = m_pPageView->getPages();
        int nPageNum = pPageArray->count();
        if (0 == nPageNum)
        {
            pLeftBtn->setVisible(false);
            pRightBtn->setVisible(false);
        }
        else
        {
            pLeftBtn->setVisible(0 != nCurPage);
            pRightBtn->setVisible(nCurPage != (nPageNum-1) );
        }
        
    } while (0);
}
void CSelectPetLayer::freshOperationData()
{
    do
    {
        ETavernOperationType nOpIndex = m_nOperationIndex;
        
        int nPetId = m_nCurPageTag;
        EnemyRecord *pPetRcd = DATAPOOL->getEnemyRecord(nPetId);
        if (pPetRcd)
        {
            if (pPetRcd->m_bUnlock)
            {
                nOpIndex = TAVERN_OP_TYPE_TRAIN;
            }
            else
            {
                nOpIndex = TAVERN_OP_TYPE_UNLOCK;
            }
        }
        if (nOpIndex != m_nOperationIndex)
        {
            m_nOperationIndex = nOpIndex;
        }
        freshOperationUI(nPetId);
        
    } while (0);
}

void CSelectPetLayer::freshOperationUI(int nId)
{
    do
    {
        int buyGem = 0;
        int buyGold = 0;
        int recruitGem = 0;
        int recruitGold = 0;
        int trainGem = 0;
        int trainGold = 0;
        int level = 0;
        
        Enemy_Data *pPetData = DATAPOOL->getEnemyData(nId);
        CC_BREAK_IF(!pPetData);
        buyGem = pPetData->mBuyGem;
        buyGold = pPetData->mBuyGold;
        recruitGem = pPetData->mRecruitGem;
        recruitGold = pPetData->mRecruitGold;
        trainGem = pPetData->mUpGem;
        trainGold = pPetData->mUpGold;
        EnemyRecord *pPetRcd = DATAPOOL->getEnemyRecord(nId);
        CC_BREAK_IF(!pPetRcd);
        level = pPetRcd->m_nLevel;
        
        Layout* pRoot = dynamic_cast<Layout*>(m_pUILayer->getWidgetByTag(PLAN_HERO_LAYER_TAG_BOARD_JSON));
        CC_BREAK_IF(!pRoot);
        
        UIButton *pOperationBtn = dynamic_cast<UIButton*>(pRoot->getChildByName("operation_Button"));
        CC_BREAK_IF(!pOperationBtn);
        
        UIImageView *pPriceIco = dynamic_cast<UIImageView*>(pRoot->getChildByName("priceIco_ImageView"));
        CC_BREAK_IF(!pPriceIco);
        
        UILabelAtlas *pPrice = dynamic_cast<UILabelAtlas*>(pRoot->getChildByName("price_LabelAtlas"));
        
        if (TAVERN_OP_TYPE_UNLOCK == m_nOperationIndex)
        {
            pOperationBtn->loadTextures(tavern_button_unlock, tavern_button_unlock, tavern_button_unlock, UI_TEX_TYPE_PLIST);
            if(buyGem > 0)
            {
                pPrice->setStringValue(CCString::createWithFormat("%d", buyGem)->getCString());
                pPriceIco->loadTexture("jguan_ico_gem.png", UI_TEX_TYPE_PLIST);
            }
            else
            {
                pPrice->setStringValue(CCString::createWithFormat("%d", buyGold)->getCString());
                pPriceIco->loadTexture("jguan_ico_gold.png", UI_TEX_TYPE_PLIST);
            }
            
        }
        else if (TAVERN_OP_TYPE_TRAIN == m_nOperationIndex)
        {
            pOperationBtn->loadTextures(tavern_button_train, tavern_button_train, tavern_button_train, UI_TEX_TYPE_PLIST);
    
            if( xData->isUseGoldToTrain(trainGold, level) )
            {
                pPrice->setStringValue(CCString::createWithFormat("%d", trainGold)->getCString());
                pPriceIco->loadTexture("jguan_ico_gold.png", UI_TEX_TYPE_PLIST);
            }
            else
            {
                pPrice->setStringValue(CCString::createWithFormat("%d", trainGem)->getCString());
                pPriceIco->loadTexture("jguan_ico_gem.png", UI_TEX_TYPE_PLIST);
            }
        }
    } while (0);
}
void CSelectPetLayer::freshLevelStarUI()
{
    do
    {
        CC_BREAK_IF(!m_pPageView);
        
        Layout* pPageLayout = m_pPageView->getPage(m_nCurPageIndex);
        if (pPageLayout)
        {
            //等级星级
            UIPanel *pPanel_1 = dynamic_cast<UIPanel*>(pPageLayout->getChildByName("StarPanel_1"));
            int nLevel = DATAPOOL->findPetLevel(m_nCurPageTag);
            for (int m = 0; m < nLevel; m++)
            {
                UICheckBox *pCheckBox = static_cast<UICheckBox *>(pPageLayout->getChildByName(CCString::createWithFormat("CheckBox_%d", (m+1))->getCString()));
                pCheckBox->setSelectedState(true);
            }
        }
        
    } while (0);
}
void CSelectPetLayer::freshPageProp()
{
    do
    {
        CC_BREAK_IF(!m_pPageView);
        
        m_nCurPageIndex = m_pPageView->getCurPageIndex();
        m_nCurPageTag = m_pPageView->getPage(m_nCurPageIndex)->getTag();
        
    } while (0);
    
}
void CSelectPetLayer::freshHeadUI()
{
    do
    {
        CC_BREAK_IF(!m_pPageView);
        
        Layout* pPageLayout = m_pPageView->getPage(m_nCurPageIndex);
        CC_BREAK_IF(!pPageLayout);
        int nId = m_nCurPageTag;
        EnemyRecord *pRcd = xData->getEnemyRecord(nId);
        CC_BREAK_IF(!pRcd);
        
        //头像
        UIImageView *pHeadImagView = static_cast<UIImageView*>(pPageLayout->getChildByName("HeadImageView"));
        CC_BREAK_IF(!pHeadImagView);
        
        if (pRcd->m_bUnlock)
        {
            pHeadImagView->setTexture(CCString::createWithFormat(juese_format_head, nId)->getCString(), UI_TEX_TYPE_PLIST);
        }
        else
        {
            pHeadImagView->setTexture(CCString::createWithFormat(juese_format_head_gray, nId)->getCString(), UI_TEX_TYPE_PLIST);
        }
        
    } while (0);
}


void CSelectPetLayer::unlockPet(int nPetId)
{
    bool bChange = false;
    
    EnemyRecord *pPetRcd = DATAPOOL->getEnemyRecord(nPetId);
    Enemy_Data *pPetData = DATAPOOL->getEnemyData(nPetId);
    
    if (NULL == pPetRcd
        || NULL == pPetData)
    {
        CCLOG("@unlockPet() error:null data");
        return;
    }
    
    bool bUseGoldToUnlock = (0 == pPetData->mBuyGem);
    if (bUseGoldToUnlock)
    {
        if (DATAPOOL->changeBagGoldNum(-pPetData->mBuyGold))
        {
            pPetRcd->m_bUnlock = true;
            bChange = true;
        }
        else
        {
            xDialog->showYesNo(xStr("lack_of_gold"), dialogeventselector(CDataPool::CBLackOfGold), this);
        }
    }
    else
    {
        if (DATAPOOL->changeBagGemNum(-pPetData->mBuyGem))
        {
            pPetRcd->m_bUnlock = true;
            bChange = true;
        }
        else
        {
            xDialog->showYesNo(xStr("lack_of_gem"), dialogeventselector(CDataPool::CBLackOfGem), this);
        }
    }
    
    if (bChange)
    {
        freshOperationData();
        freshChuzhanBtn();
        freshHeadUI();
        successEffect();
    }
}

void CSelectPetLayer::trainPet(int nPetId)
{
    bool bChange = false;
    EnemyRecord *pPetRcd = DATAPOOL->getEnemyRecord(nPetId);
    Enemy_Data *pPetData = DATAPOOL->getEnemyData(nPetId);
    
    if (NULL == pPetRcd
        || NULL == pPetData)
    {
        CCLOG("@trainPet() error:null data");
        return;
    }
    if(pPetRcd->m_nLevel >= 5)
    {
        xDialog->show(xStr("max_grade_yet"));
        return;
    }
    
    if (xData->isUseGoldToTrain(pPetData->mUpGold, pPetRcd->m_nLevel))
    {
        if (DATAPOOL->changeBagGoldNum(-pPetData->mUpGold))
        {
            CCLOG("d[w_x]b net upgrade gem=%d,petid=%d,petlevel=%d",xUser->pUserInfo->gemstone, nPetId, pPetRcd->m_nLevel);
            (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_UPGRADE(this, xUser->pUserInfo->gemstone, 2, nPetId, pPetRcd->m_nLevel, nPetId, pPetRcd->m_nLevel+1);
            ++(pPetRcd->m_nLevel);
            bChange = true;
        }
        else
        {
            xDialog->showYesNo(xStr("lack_of_gold"), dialogeventselector(CDataPool::CBLackOfGold), this);
        }
    }
    else
    {
        if (DATAPOOL->changeBagGemNum(-pPetData->mUpGem))
        {
            CCLOG("d[w_x]b net upgrade gem=%d,petid=%d,petlevel=%d",xUser->pUserInfo->gemstone, nPetId, pPetRcd->m_nLevel);
            (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_UPGRADE(this, xUser->pUserInfo->gemstone, 2, nPetId, pPetRcd->m_nLevel, nPetId, pPetRcd->m_nLevel+1);
            ++(pPetRcd->m_nLevel);
            bChange = true;
        }
        else
        {
            xDialog->showYesNo(xStr("lack_of_gem"), dialogeventselector(CDataPool::CBLackOfGem), this);
        }
    }
    
    if (bChange)
    {
        freshOperationData();
        freshLevelStarUI();
        successEffect();
    }
}

void CSelectPetLayer::successEffect()
{
    do {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        
        //particle
        CCParticleSystem *p = CCParticleSystemQuad::create(success_particle);
        CC_BREAK_IF(!p);
        p->setPosition(ccp(winSize.width/2, winSize.height/2 + 150));
        p->setAutoRemoveOnFinish(true);
        p->setScale(2.0f);
        addChild(p, PLAN_PET_LAYER_ZORDER_SUCCESS_EFFECT);
    } while (0);
    
}

void CSelectPetLayer::setGoalIndex(int index)
{
    m_nGoalIndex = index;
}

#pragma mark - CSelectPetScene

CSelectPetScene::CSelectPetScene()
{
    m_pMainLayer = NULL;
}
CSelectPetScene::~CSelectPetScene()
{
    
}
bool CSelectPetScene::init()
{
    bool bRst = false;
    
    do
    {
        CC_BREAK_IF(!CCScene::init());
        
        m_pMainLayer = CSelectPetLayer::create();
		CC_BREAK_IF(!m_pMainLayer);
		addChild(m_pMainLayer);
        
        bRst = true;
        
    } while (0);

    return bRst;
}
void CSelectPetScene::setGoalIndex(int index)
{
    m_pMainLayer->setGoalIndex(index);
}
