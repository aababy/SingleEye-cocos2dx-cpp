#include "SelectHeroScene.H"

USING_NS_CC;
USING_NS_CC_EXT;

#pragma mark - CSelectHeroLayer


CSelectHeroLayer::CSelectHeroLayer(void)
{
    
}
CSelectHeroLayer::~CSelectHeroLayer()
{
    
}
bool CSelectHeroLayer::init()
{
    return (CCLayer::init());
}
void CSelectHeroLayer::onEnter()
{
    CCLayer::onEnter();
    
    xNetEngine->setBusyEnabled(false);
    
    if ( !DATAPOOL->m_bTeachStepOver[TEACH_STEP_ENTER_SELECT_HERO] )
    {
        DATAPOOL->m_bTeachStepOver[TEACH_STEP_ENTER_SELECT_HERO] = true;
    }
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(plan_scene_body_plist, plan_scene_body_image);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(tavern_scene_body_plist, tavern_scene_body_image);
    
    m_nCurPageIndex = 0;
    m_nCurPageTag = 0;
    m_bLockOperationBtn = false;
    
    HeroInit();
    HeroFresh();
    
    freshChuzhanBtn();
    freshArrowUI();
    freshPageProp();
    freshLevelStarUI();
    
    freshOperationData();
    freshOperationUI(m_nCurPageTag);
    
    //teach layer
    HeroRecord *pRcd = xData->getHeroRecord(Hero_Type_TaoZi);
    if (false == DATAPOOL->m_bTeachStepOver[TEACH_STEP_SELECT_HERO]
        && pRcd
        && pRcd->m_bUnlock)
    {

        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        m_pTeachLayer = CTeachLayer::create();
        m_pTeachLayer->setContentSize(winSize);
        UIButton *pChuZhanBtn = dynamic_cast<UIButton*>(m_pUILayer->getWidgetByTag(PLAN_HERO_LAYER_TAG_CHUZHAN));
        if (pChuZhanBtn)
        {
            m_pTeachLayer->setFocusProp(pChuZhanBtn->getPosition(), pChuZhanBtn->getContentSize().width/2);
        }
        m_pTeachLayer->setPicInfo(CCPoint(winSize.width/2, winSize.height/4), teach_to_selectHero);
        m_pTeachLayer->setCallbackFun(this, callfunc_selector(CSelectHeroLayer::CBTeachEnd));
        addChild(m_pTeachLayer, PLAN_HERO_LAYER_ZORDER_TEACH, PLAN_HERO_LAYER_TAG_TEACH);
    }
}
void CSelectHeroLayer::onExit()
{
    xNetEngine->setBusyEnabled(true);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(plan_scene_body_plist);
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(tavern_scene_body_plist);
    
    CCLayer::onExit();
    HelpMethod::removeUnusedResourceAtExit(this);
}
void CSelectHeroLayer::HeroInit()
{
    do
    {
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        
        CCSprite *pBg = CCSprite::createWithSpriteFrameName(background_0);
        pBg->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
        addChild(pBg, PLAN_HERO_LAYER_ZORDER_BG);
        
        CCLayerColor *pMask = CCLayerColor::create(ccc4(0,0,0,110));
        addChild(pMask, PLAN_HERO_LAYER_ZORDER_MASK);
        
        //back menu
        CCMenuItemImage *pBackItem = CCMenuItemImage::create(NULL, \
                                                             NULL, \
                                                             this, \
                                                             menu_selector(CSelectHeroLayer::CBPopupHeroClose));
        pBackItem->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(btn_back));
        pBackItem->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(btn_back2));
        pBackItem->setPosition(ccp(MIN_SPACE_TO_SCREEN_EDGE + pBackItem->getContentSize().width/2, MIN_SPACE_TO_SCREEN_EDGE + pBackItem->getContentSize().height/2));
        
        CCMenu *pMenu = CCMenu::create(pBackItem, NULL);
        pMenu->setPosition(CCPointZero);
        addChild(pMenu, PLAN_HERO_LAYER_ZORDER_MENU);
        
        m_pUILayer = UILayer::create();
        addChild(m_pUILayer, PLAN_HERO_LAYER_ZORDER_TABLE);
        
        //top bg
        CCSprite *pTopBg = CCSprite::createWithSpriteFrameName(topbar_1);
        pTopBg->setPosition(ccp(winSize.width/2, winSize.height - pTopBg->getContentSize().height/2));
        addChild(pTopBg, PLAN_HERO_LAYER_ZORDER_TOPBG);
        //top name
        CCSprite *pTopName = CCSprite::createWithSpriteFrameName(plan_topbar_selhero);
        pTopName->setPosition(ccp(winSize.width/2 + 10, winSize.height  - pTopBg->getContentSize().height/2 - 12));
        addChild(pTopName, PLAN_HERO_LAYER_ZORDER_TOPNAME);
        
        //board
        UILayout *pRoot = dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile(tavern_scene_body_json));
        CC_BREAK_IF(!pRoot);
        pRoot->setPosition(ccp(winSize.width/2 - pRoot->getContentSize().width/2, winSize.height/2 - pRoot->getContentSize().height/2));
        pRoot->setTag(PLAN_HERO_LAYER_TAG_BOARD_JSON);
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
        pLeftBtn->setTag(PLAN_HERO_LAYER_TAG_LEFTARROW);
        pLeftBtn->addReleaseEvent(this, coco_releaseselector(CSelectHeroLayer::CBLeftArrow));
        pLeftBtn->setPressedActionEnabled(true);
        
        //right button
        UIButton *pRightBtn = dynamic_cast<UIButton*>(pRoot->getChildByName("rightArrow_Button"));
        CC_BREAK_IF(!pRightBtn);
        pRightBtn->setTag(PLAN_HERO_LAYER_TAG_RIGHTARROW);
        pRightBtn->addReleaseEvent(this, coco_releaseselector(CSelectHeroLayer::CBRightArrow));
        pRightBtn->setPressedActionEnabled(true);
        
        //operation button
        UIButton *pOperationBtn = dynamic_cast<UIButton*>(pRoot->getChildByName("operation_Button"));
        CC_BREAK_IF(!pOperationBtn);
        pOperationBtn->addReleaseEvent(this, coco_releaseselector(CSelectHeroLayer::CBToOperation));
        pOperationBtn->setPressedActionEnabled(false);
        
        // Create the page view
        m_pPageView = UIPageView::create();
        Layout* pJsonRootLayout = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(tavern_scene_page_json));
        Layout *pHeroPageLayout = dynamic_cast<Layout*>(pJsonRootLayout->getChildByName("page_Panel"));
        CCSize pageSize = pHeroPageLayout->getContentSize();
        m_pPageView->setSize(pageSize);
        m_pPageView->setPosition(ccp((winSize.width - pageSize.width) / 2,
                                     (winSize.height - pageSize.height) / 2) );
        m_pPageView->addPageTurningEvent(this, coco_PageView_PageTurning_selector(CSelectHeroLayer::CBPageViewTurningEvent));
        
        m_pUILayer->addWidget(m_pPageView);
        
        //chuzhan btn
        UIButton *pChuZhanBtn = UIButton::create();
        pChuZhanBtn->loadTextures(btn_chuzhan, btn_chuzhan2, btn_chuzhan2, UI_TEX_TYPE_PLIST);
        pChuZhanBtn->setPosition(ccp(winSize.width - MIN_SPACE_TO_SCREEN_EDGE - pChuZhanBtn->getContentSize().width/2,
                                     MIN_SPACE_TO_SCREEN_EDGE + pChuZhanBtn->getContentSize().height/2));
        pChuZhanBtn->setTag(PLAN_HERO_LAYER_TAG_CHUZHAN);
        pChuZhanBtn->setTouchEnable(true);
        pChuZhanBtn->addReleaseEvent(this, coco_releaseselector(CSelectHeroLayer::CBPopupHeroChuZhan));
        m_pUILayer->addWidget(pChuZhanBtn);
        
    }while(0);
    
}

void CSelectHeroLayer::InitHeroPage(Layout* pLayout, int nId)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    do
    {
        CC_BREAK_IF(!pLayout);
        
        Game_Hero_Data *pHeroData = DATAPOOL->getHeroData(nId);
        CC_BREAK_IF(!pHeroData);
        HeroRecord *pHeroRcd = DATAPOOL->getHeroRecord(nId);
        CC_BREAK_IF(!pHeroRcd);

        //头像
        UIImageView *pHeadImagView = static_cast<UIImageView*>(pLayout->getChildByName("HeadImageView"));
        if(pHeadImagView)
        {
            if (pHeroRcd->m_bUnlock)
            {
                pHeadImagView->setTexture(CCString::createWithFormat(juese_format_head, nId)->getCString(), UI_TEX_TYPE_PLIST);
            }
            else
            {
                pHeadImagView->setTexture(CCString::createWithFormat(juese_format_head_gray, nId)->getCString(), UI_TEX_TYPE_PLIST);
            }
        }
        //名称
        UIImageView *pNameImagView = static_cast<UIImageView*>(pLayout->getChildByName("NameImageView"));
        if(pNameImagView)
        {
            pNameImagView->setTexture(CCString::createWithFormat(juese_format_name, nId)->getCString(), UI_TEX_TYPE_PLIST);
        }
        //已出战标识
        UIImageView *pYiChuZhanImagView = static_cast<UIImageView*>(pLayout->getChildByName("yichuzhan_ImageView"));
        if(pYiChuZhanImagView)
        {
            pYiChuZhanImagView->setVisible(DATAPOOL->isHeroBeenPlan(nId));
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
            int nAttack = pHeroData->mStar;
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
            pJiNengText->setText(xStr(CCString::createWithFormat("magicId_%d", pHeroData->mMagicID)->getCString()));
        }
        //解锁条件
        UILabel *pTiaoJianText = dynamic_cast<UILabel*>(pLayout->getChildByName("tiaojian_Label"));
        if (pTiaoJianText)
        {
            pTiaoJianText->setText(xStr(CCString::createWithFormat("tiaojian_%d", pHeroData->mBuy)->getCString()));
        }
        
        //set id
        pLayout->setTag(nId);
        
    } while (0);
    
}
void CSelectHeroLayer::HeroFresh()
{
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCSize pageSize = CCSizeMake(visibleSize.width, visibleSize.height);
    
    //根据英雄仓库增加page
    int heroNum = DATAPOOL->m_nHeroNum;
    if (heroNum > 0)
    {
        m_pPageView->removeAllPages();
    }
    
    //按照解锁的顺序来，苹果不加入
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
#define HERO_ARRAY_NUM  (10)
    int heroIdArray[HERO_ARRAY_NUM] = {0};
    int t = 0;
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
    bool bUnlock = false;
    
    Layout* pLayoutModel = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(tavern_scene_page_json));
    for (int i = 0; i < HERO_ARRAY_NUM; ++i)
    {
        heroId = heroIdArray[i];
        Layout *pNewPage = dynamic_cast<Layout *>(pLayoutModel->clone());
        if(pNewPage)
        {
            InitHeroPage(pNewPage, heroId);
            m_pPageView->addPage(pNewPage);
        }
    }
#undef HERO_ARRAY_NUM
    
}
void CSelectHeroLayer::CBPageViewTurningEvent(CCObject *pSender)
{
    freshPageProp();
    freshChuzhanBtn();
    freshArrowUI();
    freshLevelStarUI();
    freshOperationData();
    freshOperationUI(m_nCurPageTag);
}
void CSelectHeroLayer::CBLeftArrow(CCObject* pSender)
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
void CSelectHeroLayer::CBRightArrow(CCObject* pSender)
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
void CSelectHeroLayer::CBWaitOperation(float dt)
{
    m_bLockOperationBtn = false;
}
void CSelectHeroLayer::CBToOperation(CCObject* pSender)
{
    if (m_bLockOperationBtn)
    {
        return;
    }
    m_bLockOperationBtn = true;
    
    xData->playKeytone();
    
    int nHeroId = m_nCurPageTag;
    
    switch (m_nOperationIndex)
    {
        case TAVERN_OP_TYPE_UNLOCK://解锁
            unlockHero(nHeroId);
            break;
        case TAVERN_OP_TYPE_TRAIN://训练
            trainHero(nHeroId);
            break;
    }
    
    this->scheduleOnce(schedule_selector(CSelectHeroLayer::CBWaitOperation), 0.8f);
    
}
void CSelectHeroLayer::CBPopupHeroChuZhan(CCObject* pSender)
{
    xData->playKeytone();
    
    int nCurPageIndex = m_pPageView->getCurPageIndex();
    UILayout* pCurPage = m_pPageView->getPage(nCurPageIndex);
    int nTag = pCurPage->getTag();
    //    CCMessageBox((CCString::createWithFormat("page = %d", nTag))->getCString(), "page change");
    DATAPOOL->setPlanHero(m_nGoalIndex, nTag);
    if ( !DATAPOOL->m_bTeachStepOver[TEACH_STEP_SELECT_HERO] )
    {
        DATAPOOL->m_bTeachStepOver[TEACH_STEP_SELECT_HERO] = true;
    }
    xSM->back();
}

void CSelectHeroLayer::CBPopupHeroClose(CCObject* pSender)
{
    xData->playKeytone();
    xSM->back();
}
void CSelectHeroLayer::CBTeachEnd()
{
    DATAPOOL->m_bTeachStepOver[TEACH_STEP_SELECT_HERO] = true;
    DATAPOOL->saveArchive();
    
    this->removeChildByTag(PLAN_HERO_LAYER_TAG_TEACH, true);
    
    int nId = 1007;//水蜜桃
    HeroRecord *pHeroRcd = DATAPOOL->getHeroRecord(nId);
    if (pHeroRcd && pHeroRcd->m_bUnlock)
    {
        DATAPOOL->setPlanHero(m_nGoalIndex, nId);
    }
    xSM->back();
}
//刷新显示，是否已经出战
void CSelectHeroLayer::freshChuzhanBtn()
{
    UIButton *pChuZhanBtn = (UIButton *)dynamic_cast<UIButton*>(m_pUILayer->getWidgetByTag(PLAN_HERO_LAYER_TAG_CHUZHAN));
    if (pChuZhanBtn && m_pPageView)
    {
        int nCurPageIndex = m_pPageView->getCurPageIndex();
        UILayout* pCurPage = m_pPageView->getPage(nCurPageIndex);
        if(pCurPage)
        {
            int nId = pCurPage->getTag();
            HeroRecord *pHeroRcd = DATAPOOL->getHeroRecord(nId);
            
            if (pHeroRcd && pHeroRcd->m_bUnlock)
            {
//                if (DATAPOOL->isHeroBeenPlan(nId))
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
                pYiChuZhanImagView->setVisible(DATAPOOL->isHeroBeenPlan(nId));
            }
        }
        else
        {
            pChuZhanBtn->disable();
            pChuZhanBtn->setVisible(false);
        }
    }
}
void CSelectHeroLayer::freshOperationData()
{
    do
    {
        ETavernOperationType nOpIndex = m_nOperationIndex;
        
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
        freshOperationUI(nHeroId);
        
    } while (0);
}

void CSelectHeroLayer::freshOperationUI(int nId)
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
void CSelectHeroLayer::freshLevelStarUI()
{
    do
    {
        CC_BREAK_IF(!m_pPageView);
        
        Layout* pPageLayout = m_pPageView->getPage(m_nCurPageIndex);
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
        
    } while (0);
}
void CSelectHeroLayer::freshHeadUI()
{
    do
    {
        CC_BREAK_IF(!m_pPageView);
        
        Layout* pPageLayout = m_pPageView->getPage(m_nCurPageIndex);
        CC_BREAK_IF(!pPageLayout);
        int nId = m_nCurPageTag;
        HeroRecord *pHeroRcd = xData->getHeroRecord(nId);
        CC_BREAK_IF(!pHeroRcd);
        
        //头像
        UIImageView *pHeadImagView = static_cast<UIImageView*>(pPageLayout->getChildByName("HeadImageView"));
        CC_BREAK_IF(!pHeadImagView);
        
        if (pHeroRcd->m_bUnlock)
        {
            pHeadImagView->setTexture(CCString::createWithFormat(juese_format_head, nId)->getCString(), UI_TEX_TYPE_PLIST);
        }
        else
        {
            pHeadImagView->setTexture(CCString::createWithFormat(juese_format_head_gray, nId)->getCString(), UI_TEX_TYPE_PLIST);
        }
        
    } while (0);
}
void CSelectHeroLayer::freshPageProp()
{
    do
    {
        CC_BREAK_IF(!m_pPageView);
        
        m_nCurPageIndex = m_pPageView->getCurPageIndex();
        m_nCurPageTag = m_pPageView->getPage(m_nCurPageIndex)->getTag();
        
    } while (0);
    
}
void CSelectHeroLayer::freshArrowUI()
{
    do {
        //left button
        UIButton *pLeftBtn = dynamic_cast<UIButton*>(m_pUILayer->getWidgetByTag(PLAN_HERO_LAYER_TAG_LEFTARROW));
        CC_BREAK_IF(!pLeftBtn);
        
        //right button
        UIButton *pRightBtn = dynamic_cast<UIButton*>(m_pUILayer->getWidgetByTag(PLAN_HERO_LAYER_TAG_RIGHTARROW));
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
void CSelectHeroLayer::unlockHero(int nHeroId)
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
        freshChuzhanBtn();
        freshHeadUI();
        successEffect();
    }
}
void CSelectHeroLayer::trainHero(int nHeroId)
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
        successEffect();
    }
}

void CSelectHeroLayer::successEffect()
{
    do {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        
        //particle
        CCParticleSystem *p = CCParticleSystemQuad::create(success_particle);
        CC_BREAK_IF(!p);
        p->setPosition(ccp(winSize.width/2, winSize.height/2 + 150));
        p->setAutoRemoveOnFinish(true);
        p->setScale(2.0f);
        addChild(p, PLAN_HERO_LAYER_ZORDER_SUCCESS_EFFECT);
    } while (0);
    
}

void CSelectHeroLayer::setGoalIndex(int index)
{
    m_nGoalIndex = index;
}

#pragma mark - CSelectHeroScene

CSelectHeroScene::CSelectHeroScene()
{
    m_pMainLayer = NULL;
}
CSelectHeroScene::~CSelectHeroScene()
{
    
}
bool CSelectHeroScene::init()
{
    bool bRst = false;
    
    do
    {
        CC_BREAK_IF(!CCScene::init());
        
        m_pMainLayer = CSelectHeroLayer::create();
		CC_BREAK_IF(!m_pMainLayer);
		addChild(m_pMainLayer);
        
        bRst = true;
        
    } while (0);

    return bRst;
}
void CSelectHeroScene::setGoalIndex(int index)
{
    m_pMainLayer->setGoalIndex(index);
}
