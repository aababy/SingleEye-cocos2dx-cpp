//酒馆
#include "GameInclude.h"

USING_NS_CC;
USING_NS_CC_EXT;

#pragma mark - CTavernLayer

CTavernLayer::CTavernLayer(void)
{
    m_bUIReady = false;
    m_eTavernState = TAVERN_STATE_NULL;
///    m_eTavernState = TAVERN_STATE_DIRECT;
}

CTavernLayer::~CTavernLayer(void)
{

}

bool CTavernLayer::init()
{
    return (CCLayer::init());
}

void CTavernLayer::onEnter()
{
    CCLayer::onEnter();
    
    //禁用Busy提示
    xNetEngine->setBusyEnabled(false);
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite* pSpriteBg = CCSprite::create(background_2);
    if(pSpriteBg)
    {
        pSpriteBg->setPosition(ccp(winSize.width/2, winSize.height/2));
        addChild(pSpriteBg, TAVERN_ZORDER_BACKGROUND);
    }
    
    CCLayerColor *pMask = CCLayerColor::create(ccc4(0,0,0,110));
    addChild(pMask, TAVERN_ZORDER_MASK);
    MsgSystem::Instance()->RegisterObjAndHandler(this, MSGSYS_HANLE_MSG(CTavernLayer::handleMsg));
    switch (m_eTavernState)
    {
        case TAVERN_STATE_NULL:
            LoadInit();
            break;
            
        case TAVERN_STATE_LOAD:
        {
            CCSprite *pLoadWord = (CCSprite *)(getChildByTag(TAVERN_SCENE_TAG_LOAD_WORD));
            if (pLoadWord)
            {
                CCActionInterval*  actionUp = CCJumpBy::create(2, ccp(0,0), 50, 4);
                pLoadWord->runAction( CCRepeatForever::create(actionUp));
            }
        }
            break;
            
        case TAVERN_STATE_UI:
            if (m_bUIReady)
            {
                freshOperationData();
                freshOperationUI(m_nCurPageTag, (ETAB_TYPE_HERO == m_eTabType)?(1):(2));
                freshArrowUI();
                freshMoneyProp();
            }
            break;
            
        case TAVERN_STATE_DIRECT://不要load bar直接进入
            if (!m_bUIReady)
            {
                UIInit(0);
                UIInit(1);
                UIInit(2);
                UIInit(3);
            }
            SetState(TAVERN_STATE_UI);
            break;
    }
    
    m_bLockOperationBtn = false;
}

void CTavernLayer::onExit()
{
    xNetEngine->setBusyEnabled(true);
    
    xData->saveArchive();
    
    CCLayer::onExit();
    HelpMethod::removeUnusedResourceAtExit(this);
}

void CTavernLayer::update(float delta)
{
    CCLayer *loadLayer = dynamic_cast<CCLayer*>(getChildByTag(TAVERN_SCENE_TAG_LOAD_LAYER));
    
    if (m_fLoadPercentage >= 110.0f)
    {
        unscheduleUpdate();
        //load结束
        if (loadLayer)
        {
            loadLayer->removeAllChildrenWithCleanup(true);
        }
        
        CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(tips_TxtPlist);
        
        SetState(TAVERN_STATE_UI);
        
        return;
    }
    else if(20.0f == m_fLoadPercentage)
    {
        if (!m_bUIReady)
        {
            UIInit(0);
        }
    }
    else if(50.0f == m_fLoadPercentage)
    {
        if (!m_bUIReady)
        {
            UIInit(1);
        }
    }
    else if(80.0f == m_fLoadPercentage)
    {
        if (!m_bUIReady)
        {
            UIInit(2);
        }
    }
    else if(90.0f == m_fLoadPercentage)
    {
        if (!m_bUIReady)
        {
            UIInit(3);
        }
    }
    
    if (loadLayer)
    {
        CCProgressTimer *m_progressBar = (CCProgressTimer *)(loadLayer->getChildByTag(TAVERN_SCENE_TAG_LOAD_BAR));
        if (m_progressBar)
        {
            float percentage = (m_fLoadPercentage > 100.0f)?(100.0f):(m_fLoadPercentage);
            m_progressBar->setPercentage(percentage);
        }
    }

    m_fLoadPercentage += 10.0f;
}
void CTavernLayer::LoadInit()
{
	do
	{
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        
        CCLayer *loadLayer = CCLayer::create();
        addChild(loadLayer, 100, TAVERN_SCENE_TAG_LOAD_LAYER);
        
        //bg
        CCSprite *pBg = CCSprite::create(background_2);
		pBg->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
		loadLayer->addChild(pBg, 100);
        
        //loading边框
        CCSprite *pProgressFrame = CCSprite::createWithSpriteFrameName(loading_1a);
        pProgressFrame->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
        loadLayer->addChild(pProgressFrame, 101);
        
        //loading条
        CCProgressTimer *pProgressBar = CCProgressTimer::create(CCSprite::createWithSpriteFrameName(loading_1b));
        pProgressBar->setType(kCCProgressTimerTypeBar);
        pProgressBar->setVisible(true);
        pProgressBar->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
        pProgressBar->setMidpoint(ccp(0, 0));
        pProgressBar->setBarChangeRate(ccp(1, 0));
        pProgressBar->setPercentage(0);
        loadLayer->addChild(pProgressBar, 102, TAVERN_SCENE_TAG_LOAD_BAR);
        
        //load word
        CCSprite *pLoadWord = CCSprite::createWithSpriteFrameName(loading_wd);
        pLoadWord->setPosition(ccp(origin.x + visibleSize.width/2,
                                   origin.y + visibleSize.height/2 + pLoadWord->getContentSize().height));
        loadLayer->addChild(pLoadWord, 103, TAVERN_SCENE_TAG_LOAD_WORD);
        CCActionInterval*  actionUp = CCJumpBy::create(2, ccp(0,0), 50, 4);
        pLoadWord->runAction( CCRepeatForever::create(actionUp));
        
        //tips
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(tips_TxtPlist, tips_TxtPic);
        CCSprite *tips = CCSprite::createWithSpriteFrameName(CCString::createWithFormat(tips_format_str, rand()%TIPS_PIC_NUM + 1)->getCString());
        tips->setPosition(ccp(winSize.width/2, pProgressFrame->getPositionY() - pProgressFrame->getContentSize().height/2 - tips->getContentSize().height/2 - 10));
        loadLayer->addChild(tips, 104);
        
        m_fLoadPercentage = 0.0f;
        //////////////////////////////////////////////////////////
        
        this->scheduleUpdate();
        
        SetState(TAVERN_STATE_LOAD);
        
	} while (0);

}

void CTavernLayer::SetState(int goalState)
{
    if (goalState == m_eTavernState)
    {
        return;
    }
    
    switch (m_eTavernState)
    {
        case TAVERN_STATE_LOAD:
            break;
    }
    
    switch (goalState)
    {
        case TAVERN_STATE_NULL:
            LoadInit();
            break;
            
        case TAVERN_STATE_LOAD:
            break;
            
        case TAVERN_STATE_UI:
            if (m_bUIReady)
            {
                freshTabIndex();
                freshOperationData();
                freshOperationUI(m_nCurPageTag, (ETAB_TYPE_HERO == m_eTabType)?(1):(2));
                freshArrowUI();
                freshLevelStarUI();
            }
            break;
    }
    
    m_eTavernState = goalState;
}
void CTavernLayer::UIInit(int nStep)
{
    if (0 == nStep)
    {
        m_dicHero = CCDictionary::create();
        CC_SAFE_RETAIN(m_dicHero);
        m_dicPet = CCDictionary::create();
        CC_SAFE_RETAIN(m_dicPet);
        
        m_tabLayoutZOrder_1 = 0;
        m_tabLayoutZOrder_2 = 0;
        m_tabButtonZOrder_1 = 0;
        m_tabButtonZOrder_2 = 0;
    }
    
    do
    {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        
        if (0 == nStep)
        {
            m_pUILayer = UILayer::create();
            m_pUILayer->scheduleUpdate();
            addChild(m_pUILayer, TAVERN_ZORDER_JSON);

            //root
            UILayout *pRoot = dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile(tavern_scene_body_json));
            CC_BREAK_IF(!pRoot);
            pRoot->setPosition(ccp(winSize.width/2 - pRoot->getContentSize().width/2, winSize.height/2 - pRoot->getContentSize().height/2));
            pRoot->setTag(TAVERNSCENE_TAG_JSON_ROOT);
            m_pUILayer->addWidget(pRoot);

            //close button
            UIButton *pCloseBtn = dynamic_cast<UIButton*>(pRoot->getChildByName("close_Button"));
            CC_BREAK_IF(!pCloseBtn);
//            pCloseBtn->addReleaseEvent(this, coco_releaseselector(CTavernLayer::CBClose));
//            pCloseBtn->setPressedActionEnabled(true);
            pCloseBtn->disable();
            pCloseBtn->setVisible(false);
            
            CCMenuItemImage *pBackItem = CCMenuItemImage::create(NULL, \
                                                                 NULL, \
                                                                 this, \
                                                                 menu_selector(CTavernLayer::CBClose));
            pBackItem->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(btn_back));
            pBackItem->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(btn_back2));
            pBackItem->setPosition(ccp(MIN_SPACE_TO_SCREEN_EDGE + pBackItem->getContentSize().width/2, MIN_SPACE_TO_SCREEN_EDGE + pBackItem->getContentSize().height/2));
            CCMenu *pMenu = CCMenu::create(pBackItem, NULL);
            pMenu->setPosition(CCPointZero);
            addChild(pMenu, TAVERN_ZORDER_MENU);

            //tab button
            UIButton *pTabHeroBtn = dynamic_cast<UIButton*>(pRoot->getChildByName("tabHero_Button"));
            CC_BREAK_IF(!pTabHeroBtn);
            pTabHeroBtn->setTag(TAVERSCENE_TAG_TABHERO_BTN);
            pTabHeroBtn->addReleaseEvent(this, coco_releaseselector(CTavernLayer::CBTabSwitch));
            m_tabButtonZOrder_1 = pTabHeroBtn->getTag();
            UIButton *pTabPetBtn = dynamic_cast<UIButton*>(pRoot->getChildByName("tabPet_Button"));
            CC_BREAK_IF(!pTabPetBtn);
            pTabPetBtn->setTag(TAVERSCENE_TAG_TABPET_BTN);
            pTabPetBtn->addReleaseEvent(this, coco_releaseselector(CTavernLayer::CBTabSwitch));
            m_tabButtonZOrder_2 = pTabPetBtn->getTag();
            
            //left button
            UIButton *pLeftBtn = dynamic_cast<UIButton*>(pRoot->getChildByName("leftArrow_Button"));
            CC_BREAK_IF(!pLeftBtn);
            pLeftBtn->setTag(TAVERN_SCENE_TAG_LEFT_ARROW);
            pLeftBtn->addReleaseEvent(this, coco_releaseselector(CTavernLayer::CBLeft));
            pLeftBtn->setPressedActionEnabled(true);
            
            //right button
            UIButton *pRightBtn = dynamic_cast<UIButton*>(pRoot->getChildByName("rightArrow_Button"));
            CC_BREAK_IF(!pRightBtn);
            pRightBtn->setTag(TAVERN_SCENE_TAG_RIGHT_ARROW);
            pRightBtn->addReleaseEvent(this, coco_releaseselector(CTavernLayer::CBRight));
            pRightBtn->setPressedActionEnabled(true);

            //operation button
            UIButton *pOperationBtn = dynamic_cast<UIButton*>(pRoot->getChildByName("operation_Button"));
            CC_BREAK_IF(!pOperationBtn);
            pOperationBtn->addReleaseEvent(this, coco_releaseselector(CTavernLayer::CBToOperation));
            pOperationBtn->setPressedActionEnabled(false);

            //tab layout
            UILayout *pTabHeroLayout = dynamic_cast<UILayout*>(pRoot->getChildByName("tabHero_Panel"));
            CC_BREAK_IF(!pTabHeroLayout);
            pTabHeroLayout->setTag(TAVERNSCENE_TAG_TABHERO_LAYOUT);
            m_tabLayoutZOrder_1 = pTabHeroLayout->getZOrder();
            UILayout *pTabPetLayout = dynamic_cast<UILayout*>(pRoot->getChildByName("tabPet_Panel"));
            CC_BREAK_IF(!pTabPetLayout);
            pTabPetLayout->setTag(TAVERNSCENE_TAG_TABPET_LAYOUT);
            m_tabLayoutZOrder_2 = pTabPetLayout->getZOrder();
            
            CMoneyItem *pGoldCoin = new CMoneyItem(MONEY_ITEM_TYPE_GOLD);
            pGoldCoin->autorelease();
            addChild(pGoldCoin, TAVERN_ZORDER_MONEY_ITEM, TAVERN_SCENE_TAG_MONEYITEM_GOLD);
            
            CMoneyItem *pGem = new CMoneyItem(MONEY_ITEM_TYPE_GEM);
            pGem->autorelease();
            addChild(pGem, TAVERN_ZORDER_MONEY_ITEM, TAVERN_SCENE_TAG_MONEYITEM_GEM);
        }
    
        //pageView
        if (1 == nStep)
        {
            Layout* pRoot = dynamic_cast<Layout*>(m_pUILayer->getWidgetByTag(TAVERNSCENE_TAG_JSON_ROOT));
            CC_BREAK_IF(!pRoot);
            
            Layout *pTabHeroLayout = dynamic_cast<Layout*>(pRoot->getChildByTag(TAVERNSCENE_TAG_TABHERO_LAYOUT));
            CC_BREAK_IF(!pTabHeroLayout);
            
            UIPageView* pHeroPageView = createHeroPage();
            if (pHeroPageView)
            {
                pHeroPageView->setPosition(ccp(pTabHeroLayout->getContentSize().width/2 - pHeroPageView->getContentSize().width/2, pTabHeroLayout->getContentSize().height/2 - pHeroPageView->getContentSize().height/2));
                pTabHeroLayout->addChild(pHeroPageView);
            }
        }
        
        if (2 == nStep)
        {
            Layout* pRoot = dynamic_cast<Layout*>(m_pUILayer->getWidgetByTag(TAVERNSCENE_TAG_JSON_ROOT));
            CC_BREAK_IF(!pRoot);
            
            Layout *pTabPetLayout = dynamic_cast<Layout*>(pRoot->getChildByTag(TAVERNSCENE_TAG_TABPET_LAYOUT));
            CC_BREAK_IF(!pTabPetLayout);

            UIPageView* pPetPageView = createPetPage();
            if(pPetPageView)
            {
                pPetPageView->setPosition(ccp(pTabPetLayout->getContentSize().width/2 - pPetPageView->getContentSize().width/2, pTabPetLayout->getContentSize().height/2 - pPetPageView->getContentSize().height/2));
                pTabPetLayout->addChild(pPetPageView);
            }
            
        }
        
        if (3 == nStep)
        {
            Layout* pRoot = dynamic_cast<Layout*>(m_pUILayer->getWidgetByTag(TAVERNSCENE_TAG_JSON_ROOT));
            CC_BREAK_IF(!pRoot);
            
            Layout *pTabHeroLayout = dynamic_cast<Layout*>(pRoot->getChildByTag(TAVERNSCENE_TAG_TABHERO_LAYOUT));
            CC_BREAK_IF(!pTabHeroLayout);
            
            Layout *pTabPetLayout = dynamic_cast<Layout*>(pRoot->getChildByTag(TAVERNSCENE_TAG_TABPET_LAYOUT));
            CC_BREAK_IF(!pTabPetLayout);
            
            UIButton* pTabHeroBtn = dynamic_cast<UIButton*>(pRoot->getChildByTag(TAVERSCENE_TAG_TABHERO_BTN));
            CC_BREAK_IF(!pTabHeroBtn);
            
            UIButton* pTabPetBtn = dynamic_cast<UIButton*>(pRoot->getChildByTag(TAVERSCENE_TAG_TABPET_BTN));
            CC_BREAK_IF(!pTabPetBtn);
            
            //repertory
            CCObject* obj = NULL;
            CCARRAY_FOREACH(pTabHeroLayout->getChildren(), obj)
            {
                UIWidget* child = dynamic_cast<UIWidget*>(obj);
                m_dicHero->setObject(child, child->getName());
            }
            CCARRAY_FOREACH(pTabPetLayout->getChildren(), obj)
            {
                UIWidget* child = dynamic_cast<UIWidget*>(obj);
                m_dicPet->setObject(child, child->getName());
            }
            
            // initialize touch able and influence children
            m_nOperationIndex = TAVERN_OP_TYPE_NULL;
            m_nCurPageIndex = 0;
            m_nCurPageTag = 0;
            freshOperationData();
            
            m_eTabType = ETAB_TYPE_HERO;
            pTabHeroBtn->loadTextures(tavern_tab_hero, tavern_tab_hero, tavern_tab_hero, UI_TEX_TYPE_PLIST);
            pTabPetBtn->loadTextures(tavern_tab_pet_disable, tavern_tab_pet_disable, tavern_tab_pet_disable, UI_TEX_TYPE_PLIST);
            CCDictElement* element = NULL;
            pTabHeroLayout->setTouchEnabled(true);
            pTabHeroLayout->setVisible(true);
            CCDICT_FOREACH(m_dicHero, element)
            {
                UIWidget* child = dynamic_cast<UIWidget*>(element->getObject());
                child->setTouchEnabled(true);
            }
            pTabPetLayout->setTouchEnabled(false);
            pTabPetLayout->setVisible(false);
            CCDICT_FOREACH(m_dicPet, element)
            {
                UIWidget* child = dynamic_cast<UIWidget*>(element->getObject());
                child->setTouchEnabled(false);
            }
            
            m_bUIReady = true;
        }
        
    } while (0);
}

UIPageView *CTavernLayer::createHeroPage()
{
    UIPageView* pHeroPageView = UIPageView::create();
    if (NULL == pHeroPageView)
    {
        return NULL;
    }
    pHeroPageView->setTag(TAVERNSCENE_TAG_TABHERO_PAGEVIEW);

    //按照解锁的顺序来
    //苹果
    //水蜜桃
    //榴莲
    //火龙果
    //芒果
    //香蕉
    //草莓
    //鸭梨
    //西瓜
    //石榴
    //菠萝
#define HERO_ARRAY_NUM  (11)
    int heroIdArray[HERO_ARRAY_NUM] = {0};
    int t = 0;
    heroIdArray[t++] = Hero_Type_PingGuo;
    heroIdArray[t++] = Hero_Type_TaoZi;
    heroIdArray[t++] = Hero_Type_LiuLian;
    heroIdArray[t++] = Hero_Type_HuoLongGuo;
    heroIdArray[t++] = Hero_Type_MangGuo;
    heroIdArray[t++] = Hero_Type_XiangJiao;
    heroIdArray[t++] = Hero_Type_CaoMei;
    heroIdArray[t++] = Hero_Type_Lizi;
    heroIdArray[t++] = Hero_Type_XiGua;
    heroIdArray[t++] = Hero_Type_ShiLiu;
    heroIdArray[t++] = Hero_Type_BoLuo;
    
    int heroId = First_Hero_ID;
    Layout* pLayoutModel = dynamic_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(tavern_scene_page_json));
    for (int i = 0; i < HERO_ARRAY_NUM; ++i)
    {
        heroId = heroIdArray[i];
  ///      Layout* pLayout = dynamic_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(tavern_scene_page_json));
        Layout *pLayout = dynamic_cast<Layout *>(pLayoutModel->clone());
        if(0 == i)
        {
            Layout *pSubLayout = dynamic_cast<Layout*>(pLayout->getChildByName("page_Panel"));
            pHeroPageView->setSize(pSubLayout->getSize());
        }
        
        pLayout->setSize(pHeroPageView->getSize());
        pLayout->setTag(heroId);
        initHeroPage(pLayout, heroId);
        pHeroPageView->addPage(pLayout);
    }
#undef HERO_ARRAY_NUM
    
    pHeroPageView->addPageTurningEvent(this, coco_PageView_PageTurning_selector(CTavernLayer::CBPageViewTurningEvent));
    
    return pHeroPageView;
}
void CTavernLayer::initHeroPage(Layout* pPageLayout, int nHeroId)
{
    Game_Hero_Data *pHeroData = DATAPOOL->getHeroData(nHeroId);
    HeroRecord *pHeroRcd = DATAPOOL->getHeroRecord(nHeroId);
    do
    {
        CC_BREAK_IF(!pPageLayout);
        CC_BREAK_IF(!pHeroData);
        CC_BREAK_IF(!pHeroRcd);
        
        Layout* pRoot = dynamic_cast<Layout*>(m_pUILayer->getWidgetByTag(TAVERNSCENE_TAG_JSON_ROOT));
        CC_BREAK_IF(!pRoot);
        
        Layout *pHeroLayout = dynamic_cast<Layout*>(pRoot->getChildByTag(TAVERNSCENE_TAG_TABHERO_LAYOUT));
        CC_BREAK_IF(!pHeroLayout);
        
        //名称
        UIImageView *pName = dynamic_cast<UIImageView*>(pPageLayout->getChildByName("NameImageView"));
        if (pName)
        {
            pName->loadTexture(CCString::createWithFormat(juese_format_name, nHeroId)->getCString(), UI_TEX_TYPE_PLIST);
        }
        
        //头像
        UIImageView *pHead = dynamic_cast<UIImageView*>(pPageLayout->getChildByName("HeadImageView"));
        if (pHead)
        {
            if (pHeroRcd->m_bUnlock)
            {
                pHead->loadTexture(CCString::createWithFormat(juese_format_head, nHeroId)->getCString(), UI_TEX_TYPE_PLIST);
            }
            else
            {
                pHead->loadTexture(CCString::createWithFormat(juese_format_head_gray, nHeroId)->getCString(), UI_TEX_TYPE_PLIST);
            }
        }
        
        //购买、招募的价格
        UILabelAtlas *pPriceLabel = dynamic_cast<UILabelAtlas*>(pHeroLayout->getChildByName("price_LabelAtlas"));
        if (pPriceLabel)
        {
            pPriceLabel->setStringValue(CCString::createWithFormat("%d", pHeroData->mBuyGold)->getCString());
        }
        
        //等级星级
        UIPanel *pPanel_1 = dynamic_cast<UIPanel*>(pPageLayout->getChildByName("StarPanel_1"));
        int nLevel = DATAPOOL->findHeroLevel(nHeroId);
        for (int m = 0; m < nLevel; m++)
        {
            UICheckBox *pCheckBox = static_cast<UICheckBox *>(pPanel_1->getChildByName(CCString::createWithFormat("CheckBox_%d", (m+1))->getCString()));
            pCheckBox->setSelectedState(true);
        }
        
        //伤害星级
        UIPanel *pPanel_2 = dynamic_cast<UIPanel*>(pPageLayout->getChildByName("StarPanel_2"));
        int level_2 = pHeroData->mStar;
        for (int n = 0; n < level_2; n++)
        {
            UICheckBox *pCheckBox = dynamic_cast<UICheckBox *>(pPanel_2->getChildByName(CCString::createWithFormat("CheckBox_%d", (n+1))->getCString()));
            pCheckBox->setSelectedState(true);
        }
        
        //技能
        {
            UILabel *pJiNengText = dynamic_cast<UILabel*>(pPageLayout->getChildByName("jineng_Label"));
            if (pJiNengText)
            {
                pJiNengText->setText(xStr(CCString::createWithFormat("magicId_%d", pHeroData->mMagicID)->getCString()));
            }
        }
        
        //解锁条件
        {
            UILabel *pTiaoJianText = dynamic_cast<UILabel*>(pPageLayout->getChildByName("tiaojian_Label"));
            if (pTiaoJianText)
            {
                pTiaoJianText->setText(xStr(CCString::createWithFormat("tiaojian_%d", pHeroData->mBuy)->getCString()));
            }
        }
        
    } while (0);
}

UIPageView *CTavernLayer::createPetPage()
{
    do
    {
        UIPageView* pPetPageView = UIPageView::create();
        CC_BREAK_IF(!pPetPageView);
        pPetPageView->setTag(TAVERNSCENE_TAG_TABPET_PAGEVIEW);
        
        //setSize
        {
            Layout* pLayout = dynamic_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(tavern_scene_page_json));
            CC_BREAK_IF(!pLayout);
            Layout *pLayout2 = dynamic_cast<Layout*>(pLayout->getChildByName("page_Panel"));
            CC_BREAK_IF(!pLayout2);
            pPetPageView->setSize(pLayout2->getSize());
            
        }
        
        int nPetNum = DATAPOOL->m_nEnemyNum;
        int nPetId = First_Enemy_ID;
        for (int i = 0; i < nPetNum; i++)
        {
            nPetId = First_Enemy_ID + i;
            if (DATAPOOL->isPet(nPetId))
            {
                Layout* pLayout = dynamic_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(tavern_scene_page_json));
                if (0 == i)
                {
                    Layout *pSubLayout = dynamic_cast<Layout*>(pLayout->getChildByName("page_Panel"));
                    pPetPageView->setSize(pSubLayout->getSize());
                }
                pLayout->setSize(pPetPageView->getSize());
                pLayout->setTag(nPetId);
                initPetPage(pLayout, nPetId);
                pPetPageView->addPage(pLayout);
            }
        }
        
        pPetPageView->addPageTurningEvent(this, coco_PageView_PageTurning_selector(CTavernLayer::CBPageViewTurningEvent));
        return pPetPageView;
    } while (0);
    
    return NULL;

}

void CTavernLayer::initPetPage(Layout* pPageLayout, int nPetId)
{
    Enemy_Data *pPetData = DATAPOOL->getEnemyData(nPetId);
    EnemyRecord *pPetRcd = DATAPOOL->getEnemyRecord(nPetId);
    do
    {
        CC_BREAK_IF(!pPageLayout);
        CC_BREAK_IF(!pPetData);
        CC_BREAK_IF(!pPetRcd);
        
        Layout* pRoot = dynamic_cast<Layout*>(m_pUILayer->getWidgetByTag(TAVERNSCENE_TAG_JSON_ROOT));
        CC_BREAK_IF(!pRoot);
        
        Layout *pHeroLayout = dynamic_cast<Layout*>(pRoot->getChildByTag(TAVERNSCENE_TAG_TABHERO_LAYOUT));
        CC_BREAK_IF(!pHeroLayout);
        
        //名称
        UIImageView *pName = dynamic_cast<UIImageView*>(pPageLayout->getChildByName("NameImageView"));
        if (pName)
        {
            pName->loadTexture(CCString::createWithFormat(juese_format_name, nPetId)->getCString(), UI_TEX_TYPE_PLIST);
        }
        
        //头像
        UIImageView *pHead = dynamic_cast<UIImageView*>(pPageLayout->getChildByName("HeadImageView"));
        if (pHead)
        {
            if (pPetRcd->m_bUnlock)
            {
                pHead->loadTexture(CCString::createWithFormat(juese_format_head, nPetId)->getCString(), UI_TEX_TYPE_PLIST);
            }
            else
            {
                pHead->loadTexture(CCString::createWithFormat(juese_format_head_gray, nPetId)->getCString(), UI_TEX_TYPE_PLIST);
            }
        }
        
        //购买、招募的价格
        UILabelAtlas *pPriceLabel = dynamic_cast<UILabelAtlas*>(pHeroLayout->getChildByName("price_LabelAtlas"));
        if (pPriceLabel)
        {
            pPriceLabel->setStringValue(CCString::createWithFormat("%d", pPetData->mBuyGold)->getCString());
        }
        
        //等级星级
        UIPanel *pPanel_1 = dynamic_cast<UIPanel*>(pPageLayout->getChildByName("StarPanel_1"));
        int nLevel = DATAPOOL->findPetLevel(nPetId);
        for (int m = 0; m < nLevel; m++)
        {
            UICheckBox *pCheckBox = static_cast<UICheckBox *>(pPanel_1->getChildByName(CCString::createWithFormat("CheckBox_%d", (m+1))->getCString()));
            pCheckBox->setSelectedState(true);
        }
        
        //伤害星级
        UIPanel *pPanel_2 = dynamic_cast<UIPanel*>(pPageLayout->getChildByName("StarPanel_2"));
        int level_2 = pPetData->mStar;
        for (int n = 0; n < level_2; n++)
        {
            UICheckBox *pCheckBox = dynamic_cast<UICheckBox *>(pPanel_2->getChildByName(CCString::createWithFormat("CheckBox_%d", (n+1))->getCString()));
            pCheckBox->setSelectedState(true);
        }
        
        //技能
        {
            UILabel *pJiNengText = dynamic_cast<UILabel*>(pPageLayout->getChildByName("jineng_Label"));
            if (pJiNengText)
            {
                pJiNengText->setText(xStr(CCString::createWithFormat("magicId_%d", pPetData->mMagicID)->getCString()));
            }
        }
        
        //解锁条件
        {
            UILabel *pTiaoJianText = dynamic_cast<UILabel*>(pPageLayout->getChildByName("tiaojian_Label"));
            if (pTiaoJianText)
            {
                pTiaoJianText->setText(xStr(CCString::createWithFormat("tiaojian_%d", pPetData->mBuy)->getCString()));
            }
        }
    } while (0);
}

void CTavernLayer::freshLevelStarUI()
{
    do
    {
        //root
        UILayout *pRoot = dynamic_cast<UILayout*>(m_pUILayer->getWidgetByTag(TAVERNSCENE_TAG_JSON_ROOT));
        CC_BREAK_IF(!pRoot);
        
        //tab layout
        UILayout *pTabHeroLayout = dynamic_cast<UILayout*>(pRoot->getChildByName("tabHero_Panel"));
        CC_BREAK_IF(!pTabHeroLayout);

        UILayout *pTabPetLayout = dynamic_cast<UILayout*>(pRoot->getChildByName("tabPet_Panel"));
        CC_BREAK_IF(!pTabPetLayout);

        //scroll view
        UIPageView *pHeroPageView = dynamic_cast<UIPageView*>(pTabHeroLayout->getChildByTag(TAVERNSCENE_TAG_TABHERO_PAGEVIEW));
        CC_BREAK_IF(!pHeroPageView);
        
        UIPageView *pPetPageView = dynamic_cast<UIPageView*>(pTabPetLayout->getChildByTag(TAVERNSCENE_TAG_TABPET_PAGEVIEW));
        CC_BREAK_IF(!pPetPageView);
        
        switch (m_eTabType)
        {
            case ETAB_TYPE_HERO:
            {
                Layout* pPageLayout = pHeroPageView->getPage(m_nCurPageIndex);
                if (pPageLayout)
                {
                    //等级星级
                    UIPanel *pPanel_1 = dynamic_cast<UIPanel*>(pPageLayout->getChildByName("StarPanel_1"));
                    int nLevel = DATAPOOL->findHeroLevel(m_nCurPageTag);
                    for (int m = 0; m < nLevel; m++)
                    {
                        UICheckBox *pCheckBox = static_cast<UICheckBox *>(pPageLayout->getChildByName(CCString::createWithFormat("CheckBox_%d", (m+1))->getCString()));
                        pCheckBox->setSelectedState(true);
                    }
                }
            }
                break;
            case ETAB_TYPE_PET:
            {
                Layout* pPageLayout = pPetPageView->getPage(m_nCurPageIndex);
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
            }
                break;
        }
        
    } while (0);
}
void CTavernLayer::freshTabIndex()
{
    do
    {
        //root
        UILayout *pRoot = dynamic_cast<UILayout*>(m_pUILayer->getWidgetByTag(TAVERNSCENE_TAG_JSON_ROOT));
        CC_BREAK_IF(!pRoot);
        
        //tab layout
        UILayout *pTabHeroLayout = dynamic_cast<UILayout*>(pRoot->getChildByName("tabHero_Panel"));
        CC_BREAK_IF(!pTabHeroLayout);
        
        UILayout *pTabPetLayout = dynamic_cast<UILayout*>(pRoot->getChildByName("tabPet_Panel"));
        CC_BREAK_IF(!pTabPetLayout);
        
        //scroll view
        UIPageView *pHeroPageView = dynamic_cast<UIPageView*>(pTabHeroLayout->getChildByTag(TAVERNSCENE_TAG_TABHERO_PAGEVIEW));
        CC_BREAK_IF(!pHeroPageView);
        
        UIPageView *pPetPageView = dynamic_cast<UIPageView*>(pTabPetLayout->getChildByTag(TAVERNSCENE_TAG_TABPET_PAGEVIEW));
        CC_BREAK_IF(!pPetPageView);
        
        switch (m_eTabType)
        {
            case ETAB_TYPE_HERO:
                m_nCurPageIndex = pHeroPageView->getCurPageIndex();
                m_nCurPageTag = pHeroPageView->getPage(m_nCurPageIndex)->getTag();
                break;
            case ETAB_TYPE_PET:
                m_nCurPageIndex = pPetPageView->getCurPageIndex();
                m_nCurPageTag = pPetPageView->getPage(m_nCurPageIndex)->getTag();
                break;
        }
        
    }while (0);
    
}
void CTavernLayer::freshOperationData()
{
    do
    {
        ETavernOperationType nOpIndex = m_nOperationIndex;
        
        switch (m_eTabType)
        {
            case ETAB_TYPE_HERO:
            {
                int nHeroId = m_nCurPageTag;
                HeroRecord *pHeroRcd = DATAPOOL->getHeroRecord(nHeroId);
                if (pHeroRcd)
                {
                    if (pHeroRcd->m_bUnlock)
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
                freshOperationUI(nHeroId, 1);
            }
                break;
            case ETAB_TYPE_PET:
            {
                int nPetId = m_nCurPageTag;
                EnemyRecord *pEnemyRcd = DATAPOOL->getEnemyRecord(nPetId);
                if (pEnemyRcd)
                {
                    if (false == pEnemyRcd->m_bUnlock)
                    {
                        nOpIndex = TAVERN_OP_TYPE_UNLOCK;
                    }
                    else
                    {
                        nOpIndex = TAVERN_OP_TYPE_TRAIN;
                    }
                }
                
                if (nOpIndex != m_nOperationIndex)
                {
                    m_nOperationIndex = nOpIndex;
                }
                freshOperationUI(nPetId, 2);
            }
                break;
        }
        
    } while (0);
}
//type : 1 hero 2 pet
void CTavernLayer::freshOperationUI(int nId, int nType)
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
        
        if (1 == nType)
        {
            Game_Hero_Data *pHeroData = DATAPOOL->getHeroData(nId);
            CC_BREAK_IF(!pHeroData);
            buyGem = pHeroData->mBuyGem;
            buyGold = pHeroData->mBuyGold;
            recruitGem = pHeroData->mRecruitGem;
            recruitGold = pHeroData->mRecruitGold;
            trainGem = pHeroData->mUpGem;
            trainGold = pHeroData->mUpGold;
            HeroRecord *pHeroRcd = DATAPOOL->getHeroRecord(nId);
            CC_BREAK_IF(!pHeroRcd);
            level = pHeroRcd->m_nLevel;
        }
        else if(2 == nType)
        {
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
        }
        else
        {
            break;
        }
        
        Layout* pRoot = dynamic_cast<Layout*>(m_pUILayer->getWidgetByTag(TAVERNSCENE_TAG_JSON_ROOT));
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
void CTavernLayer::freshArrowUI()
{
    do
    {
        //root
        UILayout *pRoot = dynamic_cast<UILayout*>(m_pUILayer->getWidgetByTag(TAVERNSCENE_TAG_JSON_ROOT));
        CC_BREAK_IF(!pRoot);
        
        //tab layout
        UILayout *pTabHeroLayout = dynamic_cast<UILayout*>(pRoot->getChildByName("tabHero_Panel"));
        CC_BREAK_IF(!pTabHeroLayout);
        
        UILayout *pTabPetLayout = dynamic_cast<UILayout*>(pRoot->getChildByName("tabPet_Panel"));
        CC_BREAK_IF(!pTabPetLayout);
        
        //left button
        UIButton *pLeftBtn = dynamic_cast<UIButton*>(pRoot->getChildByName("leftArrow_Button"));
        CC_BREAK_IF(!pLeftBtn);
        
        //right button
        UIButton *pRightBtn = dynamic_cast<UIButton*>(pRoot->getChildByName("rightArrow_Button"));
        CC_BREAK_IF(!pRightBtn);
        
        //scroll view
        UIPageView *pHeroPageView = dynamic_cast<UIPageView*>(pTabHeroLayout->getChildByTag(TAVERNSCENE_TAG_TABHERO_PAGEVIEW));
        CC_BREAK_IF(!pHeroPageView);
        
        UIPageView *pPetPageView = dynamic_cast<UIPageView*>(pTabPetLayout->getChildByTag(TAVERNSCENE_TAG_TABPET_PAGEVIEW));
        CC_BREAK_IF(!pPetPageView);
        
        switch (m_eTabType)
        {
            case ETAB_TYPE_HERO:
            {
                int nCurPage = pHeroPageView->getPage();
                CCArray *pPageArray = pHeroPageView->getPages();
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

            }
                break;
            case ETAB_TYPE_PET:
            {
                int nCurPage = pPetPageView->getPage();
                CCArray *pPageArray = pPetPageView->getPages();
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
            }
                break;
        }
        
    } while (0);
}
void CTavernLayer::freshMoneyProp()
{
    CMoneyItem *pGold = (CMoneyItem *)(getChildByTag(TAVERN_SCENE_TAG_MONEYITEM_GOLD));
    if (pGold)
    {
        pGold->setNum(DATAPOOL->gold);
    }
    CMoneyItem *pGem = (CMoneyItem *)(getChildByTag(TAVERN_SCENE_TAG_MONEYITEM_GEM));
    if (pGem)
    {
        pGem->setNum(xUser->pUserInfo->gemstone);
    }
}
void CTavernLayer::freshHeadUI()
{
    do
    {
        //root
        UILayout *pRoot = dynamic_cast<UILayout*>(m_pUILayer->getWidgetByTag(TAVERNSCENE_TAG_JSON_ROOT));
        CC_BREAK_IF(!pRoot);
        
        //tab layout
        UILayout *pTabHeroLayout = dynamic_cast<UILayout*>(pRoot->getChildByName("tabHero_Panel"));
        CC_BREAK_IF(!pTabHeroLayout);
        
        UILayout *pTabPetLayout = dynamic_cast<UILayout*>(pRoot->getChildByName("tabPet_Panel"));
        CC_BREAK_IF(!pTabPetLayout);
        
        //scroll view
        UIPageView *pHeroPageView = dynamic_cast<UIPageView*>(pTabHeroLayout->getChildByTag(TAVERNSCENE_TAG_TABHERO_PAGEVIEW));
        CC_BREAK_IF(!pHeroPageView);
        
        UIPageView *pPetPageView = dynamic_cast<UIPageView*>(pTabPetLayout->getChildByTag(TAVERNSCENE_TAG_TABPET_PAGEVIEW));
        CC_BREAK_IF(!pPetPageView);
        
        int nId = m_nCurPageTag;
        
        switch (m_eTabType)
        {
            case ETAB_TYPE_HERO:
            {
                UILayout *pPageLayout = pHeroPageView->getPage(pHeroPageView->getCurPageIndex());
                UIImageView *pHeadImagView = static_cast<UIImageView*>(pPageLayout->getChildByName("HeadImageView"));
                CC_BREAK_IF(!pHeadImagView);
                HeroRecord *pHeroRcd = xData->getHeroRecord(m_nCurPageTag);
                CC_BREAK_IF(!pHeroRcd);
                
                if (pHeroRcd->m_bUnlock)
                {
                    pHeadImagView->setTexture(CCString::createWithFormat(juese_format_head, nId)->getCString(), UI_TEX_TYPE_PLIST);
                }
                else
                {
                    pHeadImagView->setTexture(CCString::createWithFormat(juese_format_head_gray, nId)->getCString(), UI_TEX_TYPE_PLIST);
                }
            }
                break;
            case ETAB_TYPE_PET:
            {
                UILayout *pPageLayout = pPetPageView->getPage(pPetPageView->getCurPageIndex());
                UIImageView *pHeadImagView = static_cast<UIImageView*>(pPageLayout->getChildByName("HeadImageView"));
                CC_BREAK_IF(!pHeadImagView);
                EnemyRecord *pPetRcd = xData->getEnemyRecord(nId);
                CC_BREAK_IF(!pPetRcd);
                
                if (pPetRcd->m_bUnlock)
                {
                    pHeadImagView->setTexture(CCString::createWithFormat(juese_format_head, nId)->getCString(), UI_TEX_TYPE_PLIST);
                }
                else
                {
                    pHeadImagView->setTexture(CCString::createWithFormat(juese_format_head_gray, nId)->getCString(), UI_TEX_TYPE_PLIST);
                }
            }
                break;
        }
        //头像

        
    } while (0);
}

void CTavernLayer::CBClose(CCObject* pSender)
{
    //调试用
//    {
//        //招募
//        (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_RECRUIT(this, 9000, 1, 1006, 1);
//        //升级
//        (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_UPGRADE(this, 8000, 1, 1006, 2, 1006, 3);
//        
//        //招募
//        (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_RECRUIT(this, 9000, 2, 1106, 1);
//        //升级
//        (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_UPGRADE(this, 8000, 2, 1106, 1, 1106, 2);
//        
//        return;
//    }
    
    if(TAVERN_STATE_UI != m_eTavernState)
    {
        return;
    }
    xData->playKeytone();
    
    xSM->back();
}
void CTavernLayer::CBLeft(CCObject* pSender)
{
    if(TAVERN_STATE_UI != m_eTavernState)
    {
        return;
    }

    bool bValid = false;
    
    do
    {
        //root
        UILayout *pRoot = dynamic_cast<UILayout*>(m_pUILayer->getWidgetByTag(TAVERNSCENE_TAG_JSON_ROOT));
        CC_BREAK_IF(!pRoot);
        
        //tab layout
        UILayout *pTabHeroLayout = dynamic_cast<UILayout*>(pRoot->getChildByName("tabHero_Panel"));
        CC_BREAK_IF(!pTabHeroLayout);
        
        UILayout *pTabPetLayout = dynamic_cast<UILayout*>(pRoot->getChildByName("tabPet_Panel"));
        CC_BREAK_IF(!pTabPetLayout);
        
        switch (m_eTabType)
        {
            case ETAB_TYPE_HERO:
            {
                UIPageView *pHeroPageView = dynamic_cast<UIPageView*>(pTabHeroLayout->getChildByTag(TAVERNSCENE_TAG_TABHERO_PAGEVIEW));
                if (pHeroPageView)
                {
                    int nCurPage = pHeroPageView->getPage();
                    CCArray *pPageArray = pHeroPageView->getPages();
                    int nPageNum = pPageArray->count();
                    if (nCurPage > 0 && nPageNum > 0)
                    {
                        pHeroPageView->scrollToPage(nCurPage - 1);
                        bValid = true;
                    }
                }
            }
                break;
            case ETAB_TYPE_PET:
            {
                UIPageView *pPetPageView = dynamic_cast<UIPageView*>(pTabPetLayout->getChildByTag(TAVERNSCENE_TAG_TABPET_PAGEVIEW));
                if (pPetPageView)
                {
                    int nCurPage = pPetPageView->getPage();
                    CCArray *pPageArray = pPetPageView->getPages();
                    int nPageNum = pPageArray->count();
                    if (nCurPage > 0 && nPageNum > 0)
                    {
                        pPetPageView->scrollToPage(nCurPage - 1);
                        bValid = true;
                    }
                }
            }
                break;
        }
        
    } while (0);
    
    if (bValid)
    {
        xData->playKeytone();
    }
}
void CTavernLayer::CBRight(CCObject* pSender)
{
    if(TAVERN_STATE_UI != m_eTavernState)
    {
        return;
    }

    bool bValid = false;
    
    do
    {
        //root
        UILayout *pRoot = dynamic_cast<UILayout*>(m_pUILayer->getWidgetByTag(TAVERNSCENE_TAG_JSON_ROOT));
        CC_BREAK_IF(!pRoot);
        
        //tab layout
        UILayout *pTabHeroLayout = dynamic_cast<UILayout*>(pRoot->getChildByName("tabHero_Panel"));
        CC_BREAK_IF(!pTabHeroLayout);
        
        UILayout *pTabPetLayout = dynamic_cast<UILayout*>(pRoot->getChildByName("tabPet_Panel"));
        CC_BREAK_IF(!pTabPetLayout);
        
        switch (m_eTabType)
        {
            case ETAB_TYPE_HERO:
            {
                UIPageView *pHeroPageView = dynamic_cast<UIPageView*>(pTabHeroLayout->getChildByTag(TAVERNSCENE_TAG_TABHERO_PAGEVIEW));
                if (pHeroPageView)
                {
                    int nCurPage = pHeroPageView->getPage();
                    CCArray *pPageArray = pHeroPageView->getPages();
                    int nPageNum = pPageArray->count();
                    if (nCurPage < (nPageNum - 1) && nPageNum > 0)
                    {
                        pHeroPageView->scrollToPage(nCurPage + 1);
                        bValid = true;
                    }
                }
            }
                break;
            case ETAB_TYPE_PET:
            {
                UIPageView *pPetPageView = dynamic_cast<UIPageView*>(pTabPetLayout->getChildByTag(TAVERNSCENE_TAG_TABPET_PAGEVIEW));
                if (pPetPageView)
                {
                    int nCurPage = pPetPageView->getPage();
                    CCArray *pPageArray = pPetPageView->getPages();
                    int nPageNum = pPageArray->count();
                    if (nCurPage < (nPageNum - 1) && nPageNum > 0 )
                    {
                        pPetPageView->scrollToPage(nCurPage + 1);
                        bValid = true;
                    }
                }
            }
                break;
        }
        
    } while (0);
    
    if (bValid)
    {
        xData->playKeytone();
    }

}
void CTavernLayer::CBTabSwitch(CCObject* pSender)
{
    if(TAVERN_STATE_UI != m_eTavernState)
    {
        return;
    }
    xData->playKeytone();
    
    bool bSwitch = false;
    
    do
    {
        Layout* pRoot = dynamic_cast<Layout*>(m_pUILayer->getWidgetByTag(TAVERNSCENE_TAG_JSON_ROOT));
        CC_BREAK_IF(!pRoot);
        
        Layout *pHeroLayout = dynamic_cast<Layout*>(pRoot->getChildByTag(TAVERNSCENE_TAG_TABHERO_LAYOUT));
        CC_BREAK_IF(!pHeroLayout);
        Layout *pPetLayout = dynamic_cast<Layout*>(pRoot->getChildByTag(TAVERNSCENE_TAG_TABPET_LAYOUT));
        CC_BREAK_IF(!pPetLayout);
        
        UIButton* pTabHeroBtn = dynamic_cast<UIButton*>(pRoot->getChildByTag(TAVERSCENE_TAG_TABHERO_BTN));
        CC_BREAK_IF(!pTabHeroBtn);
        UIButton* pTabPetBtn = dynamic_cast<UIButton*>(pRoot->getChildByTag(TAVERSCENE_TAG_TABPET_BTN));
        CC_BREAK_IF(!pTabPetBtn);
        
        UIButton* pSenderButton = dynamic_cast<UIButton*>(pSender);
        CC_BREAK_IF(!pSenderButton);
        
        CCDictElement* element = NULL;
        switch (pSenderButton->getTag())
        {
            case TAVERSCENE_TAG_TABHERO_BTN:
                if(ETAB_TYPE_HERO != m_eTabType)
                {
                    m_eTabType = ETAB_TYPE_HERO;
                    bSwitch = true;

                    pTabHeroBtn->loadTextures(tavern_tab_hero, tavern_tab_hero, tavern_tab_hero, UI_TEX_TYPE_PLIST);
                    pTabPetBtn->loadTextures(tavern_tab_pet_disable, tavern_tab_pet_disable, tavern_tab_pet_disable, UI_TEX_TYPE_PLIST);
                    
                    pTabHeroBtn->setZOrder(m_tabButtonZOrder_1);
                    pTabPetBtn->setZOrder(m_tabButtonZOrder_2);
                    
                    pHeroLayout->setZOrder(m_tabLayoutZOrder_1);
                    pPetLayout->setZOrder(m_tabLayoutZOrder_2);
                    
                    pHeroLayout->setTouchEnable(true);
                    pHeroLayout->setVisible(true);
                    CCDICT_FOREACH(m_dicHero, element)
                    {
                        UIWidget* child = dynamic_cast<UIWidget*>(element->getObject());
                        child->setTouchEnabled(true);
                    }
                    
                    pPetLayout->setTouchEnable(false);
                    pPetLayout->setVisible(false);
                    CCDICT_FOREACH(m_dicPet, element)
                    {
                        UIWidget* child = dynamic_cast<UIWidget*>(element->getObject());
                        child->setTouchEnabled(false);
                    }
                }
                break;
            case TAVERSCENE_TAG_TABPET_BTN:
                if(ETAB_TYPE_PET != m_eTabType)
                {
                    m_eTabType = ETAB_TYPE_PET;
                    bSwitch = true;
                    
                    pTabHeroBtn->loadTextures(tavern_tab_hero_disable, tavern_tab_hero_disable, tavern_tab_hero_disable, UI_TEX_TYPE_PLIST);
                    pTabPetBtn->loadTextures(tavern_tab_pet, tavern_tab_pet, tavern_tab_pet, UI_TEX_TYPE_PLIST);
                    
                    pTabHeroBtn->setZOrder(m_tabButtonZOrder_2);
                    pTabPetBtn->setZOrder(m_tabButtonZOrder_1);
                    
                    pHeroLayout->setZOrder(m_tabLayoutZOrder_2);
                    pPetLayout->setZOrder(m_tabLayoutZOrder_1);
                    
                    pHeroLayout->setTouchEnable(false);
                    pHeroLayout->setVisible(false);
                    CCDICT_FOREACH(m_dicHero, element)
                    {
                        UIWidget* child = dynamic_cast<UIWidget*>(element->getObject());
                        child->setTouchEnabled(false);
                    }
                    
                    pPetLayout->setTouchEnable(true);
                    pPetLayout->setVisible(true);
                    CCDICT_FOREACH(m_dicPet, element)
                    {
                        UIWidget* child = dynamic_cast<UIWidget*>(element->getObject());
                        child->setTouchEnabled(true);
                    }

                }
                break;
            default:
                break;
        }

    } while (0);
    
    if (bSwitch)
    {
        freshTabIndex();
        freshOperationData();
        freshOperationUI(m_nCurPageTag, (ETAB_TYPE_HERO == m_eTabType)?(1):(2));
        freshArrowUI();
    }
}

void CTavernLayer::CBWaitOperation(float dt)
{
    m_bLockOperationBtn = false;
    CCLOG("@CTavernLayer::CBWaitOperation()");
}

void CTavernLayer::CBToOperation(CCObject* pSender)
{
    if(TAVERN_STATE_UI != m_eTavernState)
    {
        return;
    }
    
    if (m_bLockOperationBtn)
    {
        return;
    }
    
    m_bLockOperationBtn = true;
    
    xData->playKeytone();
    
    switch (m_eTabType)
    {
        case ETAB_TYPE_HERO:
            operationHero();
            break;
        case ETAB_TYPE_PET:
            operationPet();
            break;
    }
    
    freshMoneyProp();
    
    this->scheduleOnce(schedule_selector(CTavernLayer::CBWaitOperation), 0.8f);
    
}

void CTavernLayer::CBPageViewTurningEvent(CCObject *pSender)
{
//    CCLOG("@ CBPageViewTurningEvent()");
    
    if(m_bUIReady)
    {
        freshTabIndex();
        freshOperationData();
        freshOperationUI(m_nCurPageTag, (ETAB_TYPE_HERO == m_eTabType)?(1):(2));
        freshArrowUI();
    }
}

void CTavernLayer::CBHpButton(CCObject* pSender)
{
    if(TAVERN_STATE_UI != m_eTavernState)
    {
        return;
    }
    //TODO 消耗货币补充hp
//    CCLOG("HP BAR button");
}

void CTavernLayer::operationHero()
{
    int nHeroId = m_nCurPageTag;

    switch (m_nOperationIndex)
    {
        case TAVERN_OP_TYPE_UNLOCK://解锁
            unlockHero(nHeroId);
            break;
//        case TAVERN_OP_TYPE_RECRUIT://招募
//            recruitHero(nHeroId);
//            break;
        case TAVERN_OP_TYPE_TRAIN://训练
            trainHero(nHeroId);
            break;
    }
}

void CTavernLayer::operationPet()
{
    int nPetId = m_nCurPageTag;
    
    switch (m_nOperationIndex)
    {
        case TAVERN_OP_TYPE_UNLOCK://解锁
            unlockPet(nPetId);
            break;
//        case TAVERN_OP_TYPE_RECRUIT://招募
//            recruitPet(nPetId);
//            break;
        case TAVERN_OP_TYPE_TRAIN://训练
            trainPet(nPetId);
            break;
    }

}

void CTavernLayer::unlockHero(int nHeroId)
{
    bool bChange = false;
    
    HeroRecord *pHeroRcd = DATAPOOL->getHeroRecord(nHeroId);
    Game_Hero_Data *pHeroData = DATAPOOL->getHeroData(nHeroId);
    if (NULL == pHeroRcd
        || NULL == pHeroData)
    {
        CCLOG("@unlockHero() error:null data");
        return;
    }
    
    bool bUseGoldToUnlock = (0 == pHeroData->mBuyGem);
    if (bUseGoldToUnlock)
    {
        if (DATAPOOL->changeBagGoldNum(-pHeroData->mBuyGold))
        {
            pHeroRcd->m_bUnlock = true;
            bChange = true;
        }
        else
        {
            xDialog->showYesNo(xStr("lack_of_gold"), dialogeventselector(CDataPool::CBLackOfGold), this);
        }
    }
    else
    {
        if (DATAPOOL->changeBagGemNum(-pHeroData->mBuyGem))
        {
            pHeroRcd->m_bUnlock = true;
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
        freshMoneyProp();
        freshHeadUI();
        successEffect();
    }
}
void CTavernLayer::unlockPet(int nPetId)
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
        freshMoneyProp();
        freshHeadUI();
        successEffect();
    }
}
void CTavernLayer::trainHero(int nHeroId)
{
    bool bChange = false;
    HeroRecord *pHeroRcd = DATAPOOL->getHeroRecord(nHeroId);
    Game_Hero_Data *pHeroData = DATAPOOL->getHeroData(nHeroId);
    if (NULL == pHeroRcd
        || NULL == pHeroData)
    {
        CCLOG("@trainHero() error:null data");
        return;
    }
    
    if(pHeroRcd->m_nLevel >= 5)
    {
        xDialog->show(xStr("max_grade_yet"));
        return;
    }
    
    if (xData->isUseGoldToTrain(pHeroData->mUpGold, pHeroRcd->m_nLevel))
    {
        if (DATAPOOL->changeBagGoldNum(-pHeroData->mUpGold))
        {
            CCLOG("d[w_x]b net upgrade gem=%d,heroid=%d,petlevel=%d",xUser->pUserInfo->gemstone, nHeroId, pHeroRcd->m_nLevel);
            (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_UPGRADE(this, xUser->pUserInfo->gemstone, 1, nHeroId, pHeroRcd->m_nLevel, nHeroId, pHeroRcd->m_nLevel+1);
            ++(pHeroRcd->m_nLevel);
            bChange = true;
        }
        else
        {
            xDialog->showYesNo(xStr("lack_of_gold"), dialogeventselector(CDataPool::CBLackOfGold), this);
        }
    }
    else
    {
        if (DATAPOOL->changeBagGemNum(-pHeroData->mUpGem))
        {
            CCLOG("d[w_x]b net upgrade gem=%d,heroid=%d,level=%d",xUser->pUserInfo->gemstone, nHeroId, pHeroRcd->m_nLevel);
            (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_UPGRADE(this, xUser->pUserInfo->gemstone, 1, nHeroId, pHeroRcd->m_nLevel, nHeroId, pHeroRcd->m_nLevel+1);
            ++(pHeroRcd->m_nLevel);
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
        freshMoneyProp();
        
        successEffect();
    }
}
void CTavernLayer::trainPet(int nPetId)
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
        freshMoneyProp();
        
        successEffect();
    }
}
void CTavernLayer::successEffect()
{
    do {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        
        //particle
        CCParticleSystem *p = CCParticleSystemQuad::create(success_particle);
        CC_BREAK_IF(!p);
        p->setPosition(ccp(winSize.width/2, winSize.height/2 + 150));
        p->setAutoRemoveOnFinish(true);
        p->setScale(2.0f);
        addChild(p,TAVERN_ZORDER_SUCCESS_EFFECT);
    } while (0);
    
}

bool CTavernLayer::handleMsg(const Message &msg)
{
    switch (msg.m_nMsgType)
    {
        case CMD_RES_UPGRADE://升级
            CCLOG("@handleMsg() tavern handlemsg upgrade");
            CCLOG("@handleMsg() isSucess=%d,msg=%s,gem=%d",xUser->isSuccess,xUser->message.c_str(),xUser->pUserInfo->gemstone);
            CCLOG("@handleMsg() oldId=%d,oldLv=%d,newId=%d,newLv=%d",xUser->oldHeroOrMercemaryId,xUser->oldGrade,xUser->newHeroOrMercemaryId,xUser->newGrade);
            break;
    }
    
    return true;
}

#pragma mark - CTavernScene

bool CTavernScene::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!CCScene::init());
        
		m_pMainLayer = CTavernLayer::create();
		CC_BREAK_IF(!m_pMainLayer);
		addChild(m_pMainLayer);
        
		bRet = true;
        
	} while (0);
    
	return bRet;
}

