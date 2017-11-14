#include "PlanScene.h"
#include "FightScene.h"
#include "LevelScene.h"

USING_NS_CC;
USING_NS_CC_EXT;

#pragma mark - root

CPlanRootLayer::CPlanRootLayer()
{
   
}
CPlanRootLayer::~CPlanRootLayer(void)
{
    
}
bool CPlanRootLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    m_eSceneType = PLANSCENE_TYPE_NULL;
    
    return true;
}
void CPlanRootLayer::onEnter()
{
    CCLayer::onEnter();
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(plan_scene_body_plist, plan_scene_body_image);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(tavern_scene_body_plist, tavern_scene_body_image);
    
    DATAPOOL->checkPlanOutfit();
    MainInit();
    MainFresh();
    CCLOG("defense index=%d,%d", xData->m_nDefenseSetGoalIndex_1, xData->m_nDefenseSetGoalIndex_2);
    if ( -1 == xData->m_nDefenseSetGoalIndex_1 )
    {
        freshPetSwitch(2);
    }
    else
    {
        freshPetSwitch(1);
    }
    
    //teach layer
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    //引导开始第一关战斗
    if(false == DATAPOOL->m_bTeachStepOver[TEACH_STEP_ENTER_FIGHT1]
       && PLANSCENE_TYPE_PVE == m_eSceneType
       && 0 == DATAPOOL->currentChapter
       && 0 == xData->m_nCurrentLevel[0])
    {
        m_pTeachLayer = CTeachLayer::create();
        m_pTeachLayer->setContentSize(winSize);
        m_pTeachLayer->setFocusProp(ccp(winSize.width - MIN_SPACE_TO_SCREEN_EDGE - 80, MIN_SPACE_TO_SCREEN_EDGE + 40), 80);
        m_pTeachLayer->setPicInfo(CCPoint(winSize.width/2, winSize.height/4), teach_to_start_game);
        m_pTeachLayer->setCallbackFun(this, callfunc_selector(CPlanRootLayer::CBTeachEnd1));
        addChild(m_pTeachLayer, PLAN_ROOT_LAYER_ZORDER_TEACH, PLAN_ROOT_LAYER_TAG_TEACH);
    }
    //第二关战斗部署-引导进入英雄选择
    else if (DATAPOOL->m_bTeachStepOver[TEACH_STEP_ENTER_LEVEL2]
             && PLANSCENE_TYPE_PVE == m_eSceneType
        && false == DATAPOOL->m_bTeachStepOver[TEACH_STEP_ENTER_SELECT_HERO])
    {
        bool bNeedPlanHero = true;
        for (int i = 0; i < PLAN_HERO_NUM; ++i)
        {
            if (xData->getPlanHero(i) > 0)
            {
                bNeedPlanHero = false;
                break;
            }
        }
        
        if (bNeedPlanHero)
        {
            Layout *pJsonRoot = static_cast<Layout*>(m_pUILayer->getWidgetByTag(PLAN_ROOT_LAYER_TAG_JSON_ROOT));
            UIPanel *pPanel = static_cast<UIPanel*>(pJsonRoot->getChildByName("HeroPanel"));
            Layout *pItemPanel_0 = static_cast<Layout*>(pPanel->getChildByName("ItemPanel_0"));
            UIButton *pBtn0 = static_cast<UIButton*>(pItemPanel_0->getChildByName("Button"));
            m_pTeachLayer = CTeachLayer::create();
            m_pTeachLayer->setContentSize(winSize);
            if(pBtn0)
            {
                CCPoint pos = ccp(m_xOffset + pPanel->getPosition().x + pBtn0->getPosition().x + pBtn0->getContentSize().width/2 - 23, m_yOffset + pPanel->getPosition().y + pBtn0->getPosition().y + pBtn0->getContentSize().height/2);
                m_pTeachLayer->setFocusProp(pos, pBtn0->getContentSize().width/2 + 20);
            }
            m_pTeachLayer->setPicInfo(CCPoint(winSize.width/2, winSize.height/4), teach_enter_selectHero);
            m_pTeachLayer->setCallbackFun(this, callfunc_selector(CPlanRootLayer::CBTeachEnd2));
            addChild(m_pTeachLayer, PLAN_ROOT_LAYER_ZORDER_TEACH, PLAN_ROOT_LAYER_TAG_TEACH);
        }
        else
        {
            DATAPOOL->m_bTeachStepOver[TEACH_STEP_ENTER_SELECT_HERO] = true;
            xData->m_bTeachStepOver[TEACH_STEP_SELECT_HERO] = true;
        }

    }
    //第二关战斗部署-引导进入道具选择
    else if (DATAPOOL->m_bTeachStepOver[TEACH_STEP_ENTER_LEVEL2]
             && DATAPOOL->m_bTeachStepOver[TEACH_STEP_ENTER_SELECT_HERO]
             && DATAPOOL->m_bTeachStepOver[TEACH_STEP_SELECT_HERO]
             && (false == DATAPOOL->m_bTeachStepOver[TEACH_STEP_ENTER_SELECT_OUTFIT] || false == DATAPOOL->m_bTeachStepOver[TEACH_STEP_SELECT_OUTFIT])
             && PLANSCENE_TYPE_PVE == m_eSceneType)
    {
        bool bNeedPlanOutfit = true;
        for (int i = 0; i < PLAN_OUTFIT_NUM; ++i)
        {
            if (xData->getPlanOutfit(i) > 0 )
            {
                bNeedPlanOutfit = false;
                break;
            }
        }
        
        if (bNeedPlanOutfit)
        {
            Layout *pJsonRoot = static_cast<Layout*>(m_pUILayer->getWidgetByTag(PLAN_ROOT_LAYER_TAG_JSON_ROOT));
            UIPanel *pPanel = static_cast<UIPanel*>(pJsonRoot->getChildByName("DaoJuPanel"));
            Layout *pItemPanel_0 = static_cast<Layout*>(pPanel->getChildByName("ItemPanel_0"));
            UIButton *pBtn0 = static_cast<UIButton*>(pItemPanel_0->getChildByName("Button"));
            m_pTeachLayer = CTeachLayer::create();
            m_pTeachLayer->setContentSize(winSize);
            if(pBtn0)
            {
                CCPoint pos = ccp(pJsonRoot->getPosition().x + pPanel->getPosition().x + pBtn0->getPosition().x + pBtn0->getContentSize().width/2 + 20, pJsonRoot->getPosition().y + pPanel->getPosition().y + pBtn0->getPosition().y + pBtn0->getContentSize().height/2);
                m_pTeachLayer->setFocusProp(pos, pBtn0->getContentSize().width/2 + 20);
            }
            m_pTeachLayer->setPicInfo(CCPoint(winSize.width/2, winSize.height*3/4), teach_enter_selectOutfit);
            m_pTeachLayer->setCallbackFun(this, callfunc_selector(CPlanRootLayer::CBTeachEnd3));
            addChild(m_pTeachLayer, PLAN_ROOT_LAYER_ZORDER_TEACH, PLAN_ROOT_LAYER_TAG_TEACH);
        }
        else
        {
            DATAPOOL->m_bTeachStepOver[TEACH_STEP_ENTER_SELECT_OUTFIT] = true;
            DATAPOOL->m_bTeachStepOver[TEACH_STEP_SELECT_OUTFIT] = true;
        }

    }
    //引导开始第二关战斗
    else if(false == DATAPOOL->m_bTeachStepOver[TEACH_STEP_ENTER_FIGHT2]
    && PLANSCENE_TYPE_PVE == m_eSceneType
    && 1 == xData->m_nCurrentLevel[0]
    && 0 == DATAPOOL->currentChapter
    && DATAPOOL->m_bTeachStepOver[TEACH_STEP_ENTER_SELECT_HERO]
    && DATAPOOL->m_bTeachStepOver[TEACH_STEP_ENTER_SELECT_OUTFIT]
    && DATAPOOL->m_bTeachStepOver[TEACH_STEP_SELECT_OUTFIT])
    {
        m_pTeachLayer = CTeachLayer::create();
        m_pTeachLayer->setContentSize(winSize);
        m_pTeachLayer->setFocusProp(ccp(winSize.width - MIN_SPACE_TO_SCREEN_EDGE - 80, MIN_SPACE_TO_SCREEN_EDGE + 40), 80);
        m_pTeachLayer->setPicInfo(CCPoint(winSize.width/2, winSize.height/4), teach_to_start_game);
        m_pTeachLayer->setCallbackFun(this, callfunc_selector(CPlanRootLayer::CBTeachEnd4));
        addChild(m_pTeachLayer, PLAN_ROOT_LAYER_ZORDER_TEACH, PLAN_ROOT_LAYER_TAG_TEACH);
    }
    //第6关时引导进入宠物选择
    else if(false == DATAPOOL->m_bTeachStepOver[TEACH_STEP_ENTER_SELECT_PET]
            && PLANSCENE_TYPE_PVE == m_eSceneType
            && 0 == DATAPOOL->currentChapter
            && 5 == xData->m_nCurrentLevel[0])
    {
        bool bNeedPlanPet = true;
        for (int i = 0; i < PLAN_PET_NUM; ++i)
        {
            if (xData->getPlanPet(i, 1) > 0
                || xData->getPlanPet(i, 2) > 0 )
            {
                bNeedPlanPet = false;
                break;
            }
        }
        
        if (bNeedPlanPet)
        {
            Layout *pJsonRoot = static_cast<Layout*>(m_pUILayer->getWidgetByTag(PLAN_ROOT_LAYER_TAG_JSON_ROOT));
            UIPanel *pPanel = static_cast<UIPanel*>(pJsonRoot->getChildByName("GuYongBingPanel"));
            Layout *pItemPanel_0 = static_cast<Layout*>(pPanel->getChildByName("ItemPanel_0"));
            UIButton *pBtn0 = static_cast<UIButton*>(pItemPanel_0->getChildByName("Button"));
            m_pTeachLayer = CTeachLayer::create();
            m_pTeachLayer->setContentSize(winSize);
            if(pBtn0)
            {
                CCPoint pos = ccp(pJsonRoot->getPosition().x + pPanel->getPosition().x + pBtn0->getPosition().x + pBtn0->getContentSize().width/2 - 85, pJsonRoot->getPosition().y + pPanel->getPosition().y + pBtn0->getPosition().y + pBtn0->getContentSize().height/2);
                m_pTeachLayer->setFocusProp(pos, pBtn0->getContentSize().width/2 + 20);
            }
            m_pTeachLayer->setPicInfo(CCPoint(winSize.width/2, winSize.height*3/4), teach_enter_selectPet);
            m_pTeachLayer->setCallbackFun(this, callfunc_selector(CPlanRootLayer::CBTeachEnd5));
            addChild(m_pTeachLayer, PLAN_ROOT_LAYER_ZORDER_TEACH, PLAN_ROOT_LAYER_TAG_TEACH);
        }
        else
        {
            DATAPOOL->m_bTeachStepOver[TEACH_STEP_ENTER_SELECT_PET] = true;
            xData->m_bTeachStepOver[TEACH_STEP_SELECT_PET] = true;
        }

    }
}

void CPlanRootLayer::onExit()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(plan_scene_body_plist);
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(tavern_scene_body_plist);
    
    CCLayer::onExit();
    HelpMethod::removeUnusedResourceAtExit(this);
}

void CPlanRootLayer::MainInit()
{
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    bool bPVP = (PLANSCENE_TYPE_PVP == m_eSceneType);
    
    CCSprite *pBg = CCSprite::create(background_2);
    pBg->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
    addChild(pBg);
    
    m_pUILayer = UILayer::create();
    addChild(m_pUILayer);
    
    Layout *pJsonRoot = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile((bPVP)?(plan_scene_main_pvp_json):(plan_scene_main_pve_json)));
    pJsonRoot->setTag(PLAN_ROOT_LAYER_TAG_JSON_ROOT);
    pJsonRoot->setPosition(ccp(winSize.width/2 - pJsonRoot->getContentSize().width/2, winSize.height/2 - pJsonRoot->getContentSize().height/2));
    m_pUILayer->addWidget(pJsonRoot);
    m_xOffset = winSize.width/2 - pJsonRoot->getContentSize().width/2;
    m_yOffset = winSize.height/2 - pJsonRoot->getContentSize().height/2;
    
    UIButton *pSwitchPetBtn = dynamic_cast<UIButton*>(pJsonRoot->getChildByName("switch_Button"));
    pSwitchPetBtn->setTag(PLAN_ROOT_LAYER_TAG_SWITCH_BTN);
    pSwitchPetBtn->addReleaseEvent(this, coco_releaseselector(CPlanRootLayer::CBSwitchPet));
    
    //hero select
    {
        UIPanel *pPanel = static_cast<UIPanel*>(pJsonRoot->getChildByName("HeroPanel"));
        
        Layout *pItemPanel_0 = static_cast<Layout*>(pPanel->getChildByName("ItemPanel_0"));
        UIButton *pBtn0 = static_cast<UIButton*>(pItemPanel_0->getChildByName("Button"));
        pBtn0->setTag(PLAN_ROOT_LAYER_TAG_HERO_BTN_0);
        pBtn0->addReleaseEvent(this, coco_releaseselector(CPlanRootLayer::CBSelectHero));
        
        Layout *pItemPanel_1 = static_cast<Layout*>(pPanel->getChildByName("ItemPanel_1"));
        UIButton *pBtn1 = static_cast<UIButton*>(pItemPanel_1->getChildByName("Button"));
        pBtn1->setTag(PLAN_ROOT_LAYER_TAG_HERO_BTN_1);
        pBtn1->addReleaseEvent(this, coco_releaseselector(CPlanRootLayer::CBSelectHero));
        
        Layout *pItemPanel_2 = static_cast<Layout*>(pPanel->getChildByName("ItemPanel_2"));
        UIButton *pBtn2 = static_cast<UIButton*>(pItemPanel_2->getChildByName("Button"));
        pBtn2->setTag(PLAN_ROOT_LAYER_TAG_HERO_BTN_2);
        pBtn2->addReleaseEvent(this, coco_releaseselector(CPlanRootLayer::CBSelectHero));
    }
    
    //道具 select
    if(!bPVP)
    {
        UIPanel *pPanel = static_cast<UIPanel*>(pJsonRoot->getChildByName("DaoJuPanel"));
        
        Layout *pItemPanel_0 = static_cast<Layout*>(pPanel->getChildByName("ItemPanel_0"));
        UIButton *pBtn0 = static_cast<UIButton*>(pItemPanel_0->getChildByName("Button"));
        pBtn0->setTag(PLAN_ROOT_LAYER_TAG_OUTFIT_BTN_0);
        pBtn0->addReleaseEvent(this, coco_releaseselector(CPlanRootLayer::CBSelectOutfit));
        
        Layout *pItemPanel_1 = static_cast<Layout*>(pPanel->getChildByName("ItemPanel_1"));
        UIButton *pBtn1 = static_cast<UIButton*>(pItemPanel_1->getChildByName("Button"));
        pBtn1->setTag(PLAN_ROOT_LAYER_TAG_OUTFIT_BTN_1);
        pBtn1->addReleaseEvent(this, coco_releaseselector(CPlanRootLayer::CBSelectOutfit));
    }
    
    //雇佣兵 select
    {
        UIPanel *pPetPanel_1 = static_cast<UIPanel*>(pJsonRoot->getChildByName("GuYongBingPanel"));
        pPetPanel_1->setTag(PLAN_ROOT_LAYER_TAG_PET_PANEL_1);
        
        Layout *pItemPanel_0 = static_cast<Layout*>(pPetPanel_1->getChildByName("ItemPanel_0"));
        UIButton *pBtn0 = static_cast<UIButton*>(pItemPanel_0->getChildByName("Button"));
        pBtn0->setTag(PLAN_ROOT_LAYER_TAG_PET_BTN_0);
        pBtn0->addReleaseEvent(this, coco_releaseselector(CPlanRootLayer::CBSelectPet));
        
        Layout *pItemPanel_1 = static_cast<Layout*>(pPetPanel_1->getChildByName("ItemPanel_1"));
        UIButton *pBtn1 = static_cast<UIButton*>(pItemPanel_1->getChildByName("Button"));
        pBtn1->setTag(PLAN_ROOT_LAYER_TAG_PET_BTN_1);
        pBtn1->addReleaseEvent(this, coco_releaseselector(CPlanRootLayer::CBSelectPet));
        
        Layout *pItemPanel_2 = static_cast<Layout*>(pPetPanel_1->getChildByName("ItemPanel_2"));
        UIButton *pBtn2 = static_cast<UIButton*>(pItemPanel_2->getChildByName("Button"));
        pBtn2->setTag(PLAN_ROOT_LAYER_TAG_PET_BTN_2);
        pBtn2->addReleaseEvent(this, coco_releaseselector(CPlanRootLayer::CBSelectPet));
        
        Layout *pItemPanel_3 = static_cast<Layout*>(pPetPanel_1->getChildByName("ItemPanel_3"));
        UIButton *pBtn3 = static_cast<UIButton*>(pItemPanel_3->getChildByName("Button"));
        pBtn3->addReleaseEvent(this, coco_releaseselector(CPlanRootLayer::CBSelectPet));
        pBtn3->setTag(PLAN_ROOT_LAYER_TAG_PET_BTN_3);
        
        m_middlePos = pPetPanel_1->getPosition();
    }
    
    //雇佣兵 select 2
    {
        UIPanel *pPetPanel_2 = static_cast<UIPanel*>(pJsonRoot->getChildByName("GuYongBingPanel_2"));
        pPetPanel_2->setVisible(true);
        pPetPanel_2->setTag(PLAN_ROOT_LAYER_TAG_PET_PANEL_2);
        
        Layout *pItemPanel_0 = static_cast<Layout*>(pPetPanel_2->getChildByName("ItemPanel_0"));
        UIButton *pBtn0 = static_cast<UIButton*>(pItemPanel_0->getChildByName("Button"));
        pBtn0->setTag(PLAN_ROOT_LAYER_TAG_PET_BTN_4);
        pBtn0->addReleaseEvent(this, coco_releaseselector(CPlanRootLayer::CBSelectPet2));
        
        Layout *pItemPanel_1 = static_cast<Layout*>(pPetPanel_2->getChildByName("ItemPanel_1"));
        UIButton *pBtn1 = static_cast<UIButton*>(pItemPanel_1->getChildByName("Button"));
        pBtn1->setTag(PLAN_ROOT_LAYER_TAG_PET_BTN_5);
        pBtn1->addReleaseEvent(this, coco_releaseselector(CPlanRootLayer::CBSelectPet2));
        
        Layout *pItemPanel_2 = static_cast<Layout*>(pPetPanel_2->getChildByName("ItemPanel_2"));
        UIButton *pBtn2 = static_cast<UIButton*>(pItemPanel_2->getChildByName("Button"));
        pBtn2->setTag(PLAN_ROOT_LAYER_TAG_PET_BTN_6);
        pBtn2->addReleaseEvent(this, coco_releaseselector(CPlanRootLayer::CBSelectPet2));
        
        Layout *pItemPanel_3 = static_cast<Layout*>(pPetPanel_2->getChildByName("ItemPanel_3"));
        UIButton *pBtn3 = static_cast<UIButton*>(pItemPanel_3->getChildByName("Button"));
        pBtn3->addReleaseEvent(this, coco_releaseselector(CPlanRootLayer::CBSelectPet2));
        pBtn3->setTag(PLAN_ROOT_LAYER_TAG_PET_BTN_7);
        
        m_rightPos = pPetPanel_2->getPosition();
    }
    
    m_leftPos = ccp(-m_rightPos.x, m_rightPos.y);
//    CCLOG("==============================");
//    CCLOG("pos left=(%f,%f)", m_leftPos.x, m_leftPos.y);
//    CCLOG("pos middle=(%f,%f)", m_middlePos.x, m_middlePos.y);
//    CCLOG("pos right=(%f,%f)", m_rightPos.x, m_rightPos.y);
    
    //UI
    CMoneyItem *pGoldCoin = new CMoneyItem(MONEY_ITEM_TYPE_GOLD);
    pGoldCoin->autorelease();
 ///   pGoldCoin->setPosition(125, winSize.height - 50 - pGoldCoin->getContentSize().height);
    addChild(pGoldCoin, PLAN_ROOT_LAYER_ZORDER_UI, PLAN_ROOT_LAYER_TAG_GOLD);
    
    CMoneyItem *pGem = new CMoneyItem(MONEY_ITEM_TYPE_GEM);
    pGem->autorelease();
///    pGem->setPosition(winSize.width-260, winSize.height - 50 - pGem->getContentSize().height);
    addChild(pGem, PLAN_ROOT_LAYER_ZORDER_UI, PLAN_ROOT_LAYER_TAG_GEM);
    
    CCMenuItemImage *pBackItem = CCMenuItemImage::create(NULL, \
                                                          NULL, \
                                                          this, \
                                                          menu_selector(CPlanRootLayer::CBBack));
    pBackItem->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(btn_back));
    pBackItem->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(btn_back2));
    pBackItem->setPosition(ccp(MIN_SPACE_TO_SCREEN_EDGE + pBackItem->getContentSize().width/2, MIN_SPACE_TO_SCREEN_EDGE + pBackItem->getContentSize().height/2));
    pBackItem->setTag(PLAN_ROOT_LAYER_TAG_MENU_ITEM_BACK);
    
    CCMenuItemImage *pNextItem = CCMenuItemImage::create(NULL, \
                                                         NULL, \
                                                         this, \
                                                         menu_selector(CPlanRootLayer::CBNext));
    pNextItem->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(btn_next));
    pNextItem->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(btn_next2));
    pNextItem->setPosition(ccp(winSize.width - MIN_SPACE_TO_SCREEN_EDGE - pNextItem->getContentSize().width/2,
                               MIN_SPACE_TO_SCREEN_EDGE + pNextItem->getContentSize().height/2));
    
    CCMenuItemImage *pShopItem = CCMenuItemImage::create(NULL, \
                                                         NULL, \
                                                         this, \
                                                         menu_selector(CPlanRootLayer::CBToShop));
    pShopItem->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(top_ico_shop));
    pShopItem->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(top_ico_shop));
    pShopItem->setPosition(ccp(winSize.width - MIN_SPACE_TO_SCREEN_EDGE - pShopItem->getContentSize().width/2,
                               winSize.height - MIN_SPACE_TO_SCREEN_EDGE - pShopItem->getContentSize().height/2));
    
    CCMenu* pMenu = CCMenu::create(pBackItem, pNextItem, pShopItem, NULL);
    pMenu->setPosition(CCPointZero);
    addChild(pMenu, PLAN_ROOT_LAYER_ZORDER_MENU, PLAN_ROOT_LAYER_TAG_MENU);
    
    m_eState = PLAN_ROOT_LAYER_STATE_PET_1;
    
}

void CPlanRootLayer::MainFresh()
{

    Layout *pJsonRoot = dynamic_cast<Layout*>(m_pUILayer->getWidgetByTag(PLAN_ROOT_LAYER_TAG_JSON_ROOT));
    
    //hero select
    {
        UIPanel *pPanel = static_cast<UIPanel*>(pJsonRoot->getChildByName("HeroPanel"));
        
        Layout *pItemPanel_0 = static_cast<Layout*>(pPanel->getChildByName("ItemPanel_0"));
        UIImageView *pImg0 = static_cast<UIImageView*>(pItemPanel_0->getChildByName("ImageView"));
        setHeadItem(0, 0, pImg0);
        
        Layout *pItemPanel_1 = static_cast<Layout*>(pPanel->getChildByName("ItemPanel_1"));
        UIImageView *pImg1 = static_cast<UIImageView*>(pItemPanel_1->getChildByName("ImageView"));
        setHeadItem(0, 1, pImg1);
        
        Layout *pItemPanel_2 = static_cast<Layout*>(pPanel->getChildByName("ItemPanel_2"));
        UIImageView *pImg2 = static_cast<UIImageView*>(pItemPanel_2->getChildByName("ImageView"));
        setHeadItem(0, 2, pImg2);
    }
    
    //道具 select
    bool bPVP = (PLANSCENE_TYPE_PVP == m_eSceneType);
    if (!bPVP)
    {
        UIPanel *pPanel = static_cast<UIPanel*>(pJsonRoot->getChildByName("DaoJuPanel"));
        
        Layout *pItemPanel_0 = static_cast<Layout*>(pPanel->getChildByName("ItemPanel_0"));
        UIImageView *pImg0 = static_cast<UIImageView*>(pItemPanel_0->getChildByName("ImageView"));
        setHeadItem(1, 0, pImg0);
        
        Layout *pItemPanel_1 = static_cast<Layout*>(pPanel->getChildByName("ItemPanel_1"));
        UIImageView *pImg1 = static_cast<UIImageView*>(pItemPanel_1->getChildByName("ImageView"));
        setHeadItem(1, 1, pImg1);
    }
    
    //雇佣兵 select
    {
        UIPanel *pPanel = static_cast<UIPanel*>(pJsonRoot->getChildByName("GuYongBingPanel"));
        
        Layout *pItemPanel_0 = static_cast<Layout*>(pPanel->getChildByName("ItemPanel_0"));
        UIImageView *pImg0 = static_cast<UIImageView*>(pItemPanel_0->getChildByName("ImageView"));
        setHeadItem(2, 0, pImg0);
        
        Layout *pItemPanel_1 = static_cast<Layout*>(pPanel->getChildByName("ItemPanel_1"));
        UIImageView *pImg1 = static_cast<UIImageView*>(pItemPanel_1->getChildByName("ImageView"));
        setHeadItem(2, 1, pImg1);
        
        Layout *pItemPanel_2 = static_cast<Layout*>(pPanel->getChildByName("ItemPanel_2"));
        UIImageView *pImg2 = static_cast<UIImageView*>(pItemPanel_2->getChildByName("ImageView"));
        setHeadItem(2, 2, pImg2);
        
        Layout *pItemPanel_3 = static_cast<Layout*>(pPanel->getChildByName("ItemPanel_3"));
        UIImageView *pImg3 = static_cast<UIImageView*>(pItemPanel_3->getChildByName("ImageView"));
        setHeadItem(2, 3, pImg3);
    }
    
    //雇佣兵 select 2
    {
        UIPanel *pPetPanel_2 = static_cast<UIPanel*>(pJsonRoot->getChildByName("GuYongBingPanel_2"));
        
        Layout *pItemPanel_0 = static_cast<Layout*>(pPetPanel_2->getChildByName("ItemPanel_0"));
        UIButton *pBtn0 = static_cast<UIButton*>(pItemPanel_0->getChildByName("Button"));
        UIImageView *pImg0 = static_cast<UIImageView*>(pItemPanel_0->getChildByName("ImageView"));
        setHeadItem(3, 0, pImg0);
        
        Layout *pItemPanel_1 = static_cast<Layout*>(pPetPanel_2->getChildByName("ItemPanel_1"));
        UIButton *pBtn1 = static_cast<UIButton*>(pItemPanel_1->getChildByName("Button"));
        UIImageView *pImg1 = static_cast<UIImageView*>(pItemPanel_1->getChildByName("ImageView"));
        setHeadItem(3, 1, pImg1);
        
        Layout *pItemPanel_2 = static_cast<Layout*>(pPetPanel_2->getChildByName("ItemPanel_2"));
        UIButton *pBtn2 = static_cast<UIButton*>(pItemPanel_2->getChildByName("Button"));
        UIImageView *pImg2 = static_cast<UIImageView*>(pItemPanel_2->getChildByName("ImageView"));
        setHeadItem(3, 2, pImg2);
        
        Layout *pItemPanel_3 = static_cast<Layout*>(pPetPanel_2->getChildByName("ItemPanel_3"));
        UIButton *pBtn3 = static_cast<UIButton*>(pItemPanel_3->getChildByName("Button"));
        UIImageView *pImg3 = static_cast<UIImageView*>(pItemPanel_3->getChildByName("ImageView"));
        setHeadItem(3, 3, pImg3);
        
        pBtn0->setTouchEnable(true);
        pBtn1->setTouchEnable(true);
        pBtn2->setTouchEnable(true);
        pBtn3->setTouchEnable(true);
        
    }
    
    //UI
    CMoneyItem *pGold = (CMoneyItem *)(getChildByTag(PLAN_ROOT_LAYER_TAG_GOLD));
    if (pGold)
    {
        pGold->setNum(DATAPOOL->getGoldCoin());
    }
    CMoneyItem *pGem = (CMoneyItem *)(getChildByTag(PLAN_ROOT_LAYER_TAG_GEM));
    if (pGem)
    {
        pGem->setNum(xUser->pUserInfo->gemstone);
    }
    
    if (PLANSCENE_TYPE_PVP == m_eSceneType)
    {
        CCMenu *pMenu = dynamic_cast<CCMenu*>(getChildByTag(PLAN_ROOT_LAYER_TAG_MENU));
        CCMenuItemImage *pBackItem = dynamic_cast<CCMenuItemImage*>(pMenu->getChildByTag(PLAN_ROOT_LAYER_TAG_MENU_ITEM_BACK));
        if (pBackItem)
        {
            pBackItem->setVisible(false);
        }
    }
    
}
//test
void CPlanRootLayer::addPetArmature(UIWidget *pImg, int nId)
{
    if (NULL == pImg || nId < First_Enemy_ID)
    {
        return;
    }
    Enemy_Data *pData = xData->getEnemyData(nId);
    xData->addEffectCache(CCString::createWithFormat("Sprites/%s.ExportJson", pData->mSourceName)->getCString());
    CCArmature *pNewArmature = CCArmature::create(pData->mSourceName);
    CCNode *pOldArmature = pImg->getRenderer();
    if (pOldArmature)
    {
        pImg->removeRenderer(pOldArmature, true);
    }
    pImg->addRenderer(pNewArmature, 80);
    pNewArmature->getAnimation()->playByIndex(0);
    pNewArmature->setPosition(ccp(0, -70));
}
void CPlanRootLayer::setHeadItem(int type, int n, UIImageView *pImg)
{
    int nIndex = 0;
    CCSize oldSize = pImg->getContentSize();
    CCPoint oldPos = pImg->getPosition();
    switch(type)
    {
        case 0://英雄
            nIndex = DATAPOOL->getPlanHero(n);

            if (nIndex < 0)	
            {
                pImg->setTexture(plan_item_lock, UI_TEX_TYPE_PLIST);
                pImg->setVisible(true);
                
                CCSize newSize = pImg->getContentSize();
                CCPoint newPos = pImg->getPosition();
                CCPoint judgePos = ccp(oldPos.x, oldPos.y - oldSize.height/2 + newSize.height/2);
                pImg->setPosition(judgePos);
            }
            else if(0 == nIndex)
            {
                pImg->setVisible(false);
            }
            else
            {
                pImg->setTexture(CCString::createWithFormat(juese_format_head, nIndex)->getCString(), UI_TEX_TYPE_PLIST);
                pImg->setVisible(true);
                //调整坐标 使head的底部中点和layout的中心点重合,即底对齐
                CCSize newSize = pImg->getContentSize();
                CCPoint newPos = pImg->getPosition();
                CCPoint judgePos = ccp(oldPos.x, oldPos.y - oldSize.height/2 + newSize.height/2);
                pImg->setPosition(judgePos);
            }
            break;
        case 1://道具
            nIndex = DATAPOOL->getPlanOutfit(n);
            if (nIndex < 0)
            {
                pImg->setTexture(plan_item_lock, UI_TEX_TYPE_PLIST);
                pImg->setVisible(true);
                
                CCSize newSize = pImg->getContentSize();
                CCPoint newPos = pImg->getPosition();
                CCPoint judgePos = ccp(oldPos.x, oldPos.y - oldSize.height/2 + newSize.height/2);
                pImg->setPosition(judgePos);
            }
            else if(0 == nIndex)
            {
                pImg->setVisible(false);
            }
            else
            {
                COutfitData *pOutfitData = DATAPOOL->getOutfitData(nIndex);
                if (pOutfitData)
                {
                    pImg->setTexture(pOutfitData->m_pSourceName->getCString(), UI_TEX_TYPE_PLIST);
                    pImg->setVisible(true);
  
                    CCSize newSize = pImg->getContentSize();
                    CCPoint newPos = pImg->getPosition();
                    CCPoint judgePos = ccp(oldPos.x, oldPos.y - oldSize.height/2 + newSize.height/2);
                    pImg->setPosition(judgePos);
                }
            }
            break;
        case 2://雇佣兵
            nIndex = DATAPOOL->getPlanPet(n, 1);
            if (nIndex < 0)
            {
                pImg->setTexture(plan_item_lock, UI_TEX_TYPE_PLIST);
                pImg->setVisible(true);
                
                CCSize newSize = pImg->getContentSize();
                CCPoint newPos = pImg->getPosition();
                CCPoint judgePos = ccp(oldPos.x, oldPos.y - oldSize.height/2 + newSize.height/2);
                pImg->setPosition(judgePos);
            }
            else if(0 == nIndex)
            {
                pImg->setVisible(false);
            }
            else
            {
                pImg->setTexture(CCString::createWithFormat(juese_format_head, nIndex)->getCString(), UI_TEX_TYPE_PLIST);
                pImg->setVisible(true);
                
                CCSize newSize = pImg->getContentSize();
                CCPoint newPos = pImg->getPosition();
                CCPoint judgePos = ccp(oldPos.x, oldPos.y - oldSize.height/2 + newSize.height/2);
                pImg->setPosition(judgePos);
            }
            break;
        case 3://雇佣兵二
            nIndex = DATAPOOL->getPlanPet(n, 2);
            if (nIndex < 0)
            {
                pImg->setTexture(plan_item_lock, UI_TEX_TYPE_PLIST);
                pImg->setVisible(true);
                
                CCSize newSize = pImg->getContentSize();
                CCPoint newPos = pImg->getPosition();
                CCPoint judgePos = ccp(oldPos.x, oldPos.y - oldSize.height/2 + newSize.height/2);
                pImg->setPosition(judgePos);
            }
            else if(0 == nIndex)
            {
                pImg->setVisible(false);
            }
            else
            {
                pImg->setTexture(CCString::createWithFormat(juese_format_head, nIndex)->getCString(), UI_TEX_TYPE_PLIST);
                pImg->setVisible(true);
                
                CCSize newSize = pImg->getContentSize();
                CCPoint newPos = pImg->getPosition();
                CCPoint judgePos = ccp(oldPos.x, oldPos.y - oldSize.height/2 + newSize.height/2);
                pImg->setPosition(judgePos);
            }
            break;
        default:
            return;
        
    }

}
void CPlanRootLayer::switchToPet1()
{
    m_eState = PLAN_ROOT_LAYER_STATE_PET_1;
}
void CPlanRootLayer::switchToPet2()
{
    m_eState = PLAN_ROOT_LAYER_STATE_PET_2;
}
void CPlanRootLayer::freshPetSwitch(int goal)
{
    UIPanel *pPanel_1 = dynamic_cast<UIPanel*>(m_pUILayer->getWidgetByTag(PLAN_ROOT_LAYER_TAG_PET_PANEL_1));
    UIPanel *pPanel_2 = dynamic_cast<UIPanel*>(m_pUILayer->getWidgetByTag(PLAN_ROOT_LAYER_TAG_PET_PANEL_2));
    UIButton *pSwitchPetBtn = dynamic_cast<UIButton*>(m_pUILayer->getWidgetByTag(PLAN_ROOT_LAYER_TAG_SWITCH_BTN));
    float moveDuration = 0.6;
    
    if (NULL == pPanel_1
        || NULL == pPanel_2
        || NULL == pSwitchPetBtn )
    {
        return;
    }
    
    if(2 == goal)
    {
        m_eState = PLAN_ROOT_LAYER_STATE_PET_2;
        pSwitchPetBtn->setTextures(arrow_left_1, arrow_left_1, arrow_left_1, UI_TEX_TYPE_PLIST);
        pPanel_1->setPosition(m_leftPos);
        pPanel_2->setPosition(m_middlePos);
    }
    else if(1 == goal)
    {
        m_eState = PLAN_ROOT_LAYER_STATE_PET_1;
        pSwitchPetBtn->setTextures(arrow_right_1, arrow_right_1, arrow_right_1, UI_TEX_TYPE_PLIST);
        pPanel_1->setPosition(m_middlePos);
        pPanel_2->setPosition(m_rightPos);
    }
}
void CPlanRootLayer::CBSwitchPet(CCObject* pSender)
{
    UIPanel *pPanel_1 = dynamic_cast<UIPanel*>(m_pUILayer->getWidgetByTag(PLAN_ROOT_LAYER_TAG_PET_PANEL_1));
    UIPanel *pPanel_2 = dynamic_cast<UIPanel*>(m_pUILayer->getWidgetByTag(PLAN_ROOT_LAYER_TAG_PET_PANEL_2));
    UIButton *pSwitchPetBtn = dynamic_cast<UIButton*>(m_pUILayer->getWidgetByTag(PLAN_ROOT_LAYER_TAG_SWITCH_BTN));
    float moveDuration = 0.6;
    
    if (NULL == pPanel_1
        || NULL == pPanel_2
        || NULL == pSwitchPetBtn )
    {
        return;
    }
    
    if(PLAN_ROOT_LAYER_STATE_PET_1 == m_eState)
    {
        xData->playKeytone();
        
        m_eState = PLAN_ROOT_LAYER_STATE_PET_SWITCHING;
        pSwitchPetBtn->setTextures(arrow_left_1, arrow_left_1, arrow_left_1, UI_TEX_TYPE_PLIST);
        
        CCActionInterval* moveToLeft = CCMoveTo::create(moveDuration, m_leftPos);
        pPanel_1->runAction(CCSequence::create(moveToLeft,NULL));
        
        CCActionInterval* moveToMiddle = CCMoveTo::create(moveDuration, m_middlePos);
        CCCallFunc*  callFun = CCCallFunc::create(this,callfunc_selector(CPlanRootLayer::switchToPet2));
        pPanel_2->runAction(CCSequence::create(moveToMiddle,callFun,NULL));
    }
    else if(PLAN_ROOT_LAYER_STATE_PET_2 == m_eState)
    {
        xData->playKeytone();
        
        m_eState = PLAN_ROOT_LAYER_STATE_PET_SWITCHING;
        pSwitchPetBtn->setTextures(arrow_right_1, arrow_right_1, arrow_right_1, UI_TEX_TYPE_PLIST);
        
        CCActionInterval* moveToMiddle = CCMoveTo::create(moveDuration, m_middlePos);
        pPanel_1->runAction(CCSequence::create(moveToMiddle,NULL));
        
        CCActionInterval* moveToRight = CCMoveTo::create(moveDuration, m_rightPos);
        CCCallFunc*  callFun = CCCallFunc::create(this,callfunc_selector(CPlanRootLayer::switchToPet1));
        pPanel_2->runAction(CCSequence::create(moveToRight,callFun,NULL));
    }
    
}
void CPlanRootLayer::CBSelectHero(CCObject* pSender)
{
    if (PLAN_ROOT_LAYER_STATE_PET_SWITCHING == m_eState)
    {
        return;
    }

    xData->playKeytone();
    
    UIButton *pBtn = (UIButton*)(pSender);
    int tag = pBtn->getTag() - PLAN_ROOT_LAYER_TAG_HERO_BASE;
    int index = DATAPOOL->getPlanHero(tag);
    m_nHeroSetGoalIndex = tag;
    if (index >= 0)
    {
        xSM->forward(ST_SelectHero, m_nHeroSetGoalIndex, NULL);
    }
    else
    {
        xDialog->showYesNo(xStr("unlock_pos"), dialogeventselector(CPlanRootLayer::CBPopupBack), this);
        xDialog->setDialogID(1);
    }

}

void CPlanRootLayer::CBSelectOutfit(CCObject* pSender)
{
    if (PLAN_ROOT_LAYER_STATE_PET_SWITCHING == m_eState)
    {
        return;
    }
    
    xData->playKeytone();
    
    UIButton *pBtn = (UIButton*)(pSender);
    int tag = pBtn->getTag() - PLAN_ROOT_LAYER_TAG_OUTFIT_BASE;
    int index = DATAPOOL->getPlanOutfit(tag);
    m_nOutfitSetGoalIndex = tag;
    if (index >= 0)
    {
        xSM->forward(ST_SelectOutfit, m_nOutfitSetGoalIndex, NULL);
    }
    else
    {
        xDialog->showYesNo(xStr("unlock_pos"), dialogeventselector(CPlanRootLayer::CBPopupBack), this);
        xDialog->setDialogID(2);
    }
}
void CPlanRootLayer::CBSelectPet(CCObject* pSender)
{
    if (PLAN_ROOT_LAYER_STATE_PET_SWITCHING == m_eState)
    {
        return;
    }
    
    xData->playKeytone();
    
    UIButton *pBtn = (UIButton*)(pSender);
    int tag = pBtn->getTag() - PLAN_ROOT_LAYER_TAG_PET_BASE;
    int index = DATAPOOL->getPlanPet(tag, 1);
    DATAPOOL->m_nDefenseSetGoalIndex_1 = tag;
    DATAPOOL->m_nDefenseSetGoalIndex_2 = -1;
    if (index >= 0)
    {
        xSM->forward(ST_SelectPet);
    }
    else
    {
        xDialog->showYesNo(xStr("unlock_pos"), dialogeventselector(CPlanRootLayer::CBPopupBack), this);
        xDialog->setDialogID(3);
    }
}
void CPlanRootLayer::CBSelectPet2(CCObject* pSender)
{
    if (PLAN_ROOT_LAYER_STATE_PET_SWITCHING == m_eState)
    {
        return;
    }
    
    xData->playKeytone();
    
    UIButton *pBtn = (UIButton*)(pSender);
    int tag = pBtn->getTag() - PLAN_ROOT_LAYER_TAG_PET_2_BASE;
    int index = DATAPOOL->getPlanPet(tag, 2);
    DATAPOOL->m_nDefenseSetGoalIndex_1 = -1;
    DATAPOOL->m_nDefenseSetGoalIndex_2 = tag;
    if (index >= 0)
    {
        xSM->forward(ST_SelectPet);
    }
    else
    {
        xDialog->showYesNo(xStr("unlock_pos"), dialogeventselector(CPlanRootLayer::CBPopupBack), this);
        xDialog->setDialogID(5);
    }
}

void CPlanRootLayer::CBPopupBack(int id, int iTag)
{
    switch (id)
    {
        case 1://解锁英雄的位置
        {
            if(0 == iTag)
            {
                int nPrice = 1;//todo:价格从配置文件读入
                if (DATAPOOL->changeBagGemNum(-nPrice))//扣除宝石,解锁
                {
                    DATAPOOL->setPlanHero(m_nHeroSetGoalIndex, 0);
                    MainFresh();
                }
                else//宝石不足
                {
                    xDialog->showYesNo(xStr("lack_of_gem"), dialogeventselector(CDataPool::CBLackOfGem), this);
                }
            }
        }
            break;
            
        case 2://解锁道具的位置
        {
            if(0 == iTag)
            {
                int nPrice = 1;//todo:价格从配置文件读入
                if (DATAPOOL->changeBagGemNum(-nPrice))//扣除宝石,解锁
                {
                    DATAPOOL->setPlanOutfit(m_nOutfitSetGoalIndex, 0);
                    MainFresh();
                }
                else//宝石不足
                {
                    xDialog->showYesNo(xStr("lack_of_gem"), dialogeventselector(CDataPool::CBLackOfGem), this);
                }
            }
        }
            break;
            
        case 3://解锁雇佣兵1的位置
        {
            if(0 == iTag)
            {
                int nPrice = 1;//todo:价格从配置文件读入
                if (DATAPOOL->changeBagGemNum(-nPrice))//扣除宝石,解锁
                {
                    DATAPOOL->setPlanPet(DATAPOOL->m_nDefenseSetGoalIndex_1, 0, 1);
                    MainFresh();
                }
                else
                {
                    xDialog->showYesNo(xStr("lack_of_gem"), dialogeventselector(CDataPool::CBLackOfGem), this);
                }
            }
        }
            break;
            
        case 5://解锁雇佣兵2的位置
        {
            if(0 == iTag)
            {
                int nPrice = 1;//todo:价格从配置文件读入
                if (DATAPOOL->changeBagGemNum(-nPrice))//扣除宝石,解锁
                {
                    DATAPOOL->setPlanPet(DATAPOOL->m_nDefenseSetGoalIndex_2, 0, 2);
                    MainFresh();
                }
                else
                {
                    xDialog->showYesNo(xStr("lack_of_gem"), dialogeventselector(CDataPool::CBLackOfGem), this);
                }
            }
        }
            break;
    }
}
void CPlanRootLayer::CBBack(CCObject* pSender)
{
    if (PLANSCENE_TYPE_PVP == m_eSceneType)
    {
        return;
    }
    xData->playKeytone();
    DATAPOOL->saveArchive();
    xSM->back();
}
void CPlanRootLayer::CBNext(CCObject* pSender)
{
    switch (m_eSceneType)
    {
        case PLANSCENE_TYPE_PVE:
            xSM->forward(ST_Fight, Game_Type_Level, NULL);
            break;
            
        case PLANSCENE_TYPE_PVP:
            xSM->forward(ST_Fight, Game_Type_PVP, NULL);
            break;
            
        case PLANSCENE_TYPE_BOSS:
            xSM->forward(ST_Fight, Game_Type_Boss, NULL);
            break;

    }
    
    xData->playKeytone();
    DATAPOOL->saveArchive();
}
void CPlanRootLayer::CBToShop(CCObject *pSender)
{
    xData->playKeytone();
    xSM->forward(ST_Shop);
}
void CPlanRootLayer::CBUnlockPlane2(CCObject *pSender)
{
    xDialog->showYesNo("花费1个宝石，解锁第二防线", dialogeventselector(CPlanRootLayer::CBPopupBack), this);
    xDialog->setDialogID(4);
}
void CPlanRootLayer::CBTeachEnd1()
{
    DATAPOOL->m_bTeachStepOver[TEACH_STEP_ENTER_FIGHT1] = true;
    DATAPOOL->saveArchive();
    
    this->removeChildByTag(PLAN_ROOT_LAYER_TAG_TEACH, true);
    
    xData->playKeytone();
    
    switch (m_eSceneType)
    {
        case PLANSCENE_TYPE_PVE:
            xSM->forward(ST_Fight, Game_Type_Level, NULL);
            break;
            
        case PLANSCENE_TYPE_PVP:
            xSM->forward(ST_Fight, Game_Type_PVP, NULL);
            break;
            
        case PLANSCENE_TYPE_BOSS:
            xSM->forward(ST_Fight, Game_Type_Boss, NULL);
            break;
    }

}
void CPlanRootLayer::CBTeachEnd2()
{
    DATAPOOL->m_bTeachStepOver[TEACH_STEP_ENTER_SELECT_HERO] = true;
    DATAPOOL->saveArchive();
    
    this->removeChildByTag(PLAN_ROOT_LAYER_TAG_TEACH, true);
    
    int index = DATAPOOL->getPlanHero(0);
    m_nHeroSetGoalIndex = 0;
    if (index >= 0)
    {
        xSM->forward(ST_SelectHero);
    }
    else
    {
        xDialog->showYesNo(xStr("unlock_pos"), dialogeventselector(CPlanRootLayer::CBPopupBack), this);
        xDialog->setDialogID(1);
    }
}
void CPlanRootLayer::CBTeachEnd3()
{
    DATAPOOL->m_bTeachStepOver[TEACH_STEP_ENTER_SELECT_OUTFIT] = true;
    DATAPOOL->saveArchive();
    
    this->removeChildByTag(PLAN_ROOT_LAYER_TAG_TEACH, true);
    
    int index = DATAPOOL->getPlanOutfit(0);
    m_nOutfitSetGoalIndex = 0;
    if (index >= 0)
    {
        xSM->forward(ST_SelectOutfit, m_nOutfitSetGoalIndex, NULL);
    }
    else
    {
        xDialog->showYesNo(xStr("unlock_pos"), dialogeventselector(CPlanRootLayer::CBPopupBack), this);
        xDialog->setDialogID(2);
    }
}
void CPlanRootLayer::CBTeachEnd4()
{
    DATAPOOL->m_bTeachStepOver[TEACH_STEP_ENTER_FIGHT2] = true;
    DATAPOOL->saveArchive();
    
    this->removeChildByTag(PLAN_ROOT_LAYER_TAG_TEACH, true);
    
    xData->playKeytone();
    
    switch (m_eSceneType)
    {
        case PLANSCENE_TYPE_PVE:
            xSM->forward(ST_Fight, Game_Type_Level, NULL);
            break;
            
        case PLANSCENE_TYPE_PVP:
            xSM->forward(ST_Fight, Game_Type_PVP, NULL);
            break;
            
        case PLANSCENE_TYPE_BOSS:
            xSM->forward(ST_Fight, Game_Type_Boss, NULL);
            break;
    }

}
void CPlanRootLayer::CBTeachEnd5()
{
    DATAPOOL->m_bTeachStepOver[TEACH_STEP_ENTER_SELECT_PET] = true;
    DATAPOOL->saveArchive();
    
    this->removeChildByTag(PLAN_ROOT_LAYER_TAG_TEACH, true);
    
    xData->playKeytone();
    
}

void CPlanRootLayer::setType(EPlanSceneType goalType)
{
    m_eSceneType = goalType;
}

#pragma mark - CPlanScene

CPlanScene::CPlanScene()
{
    CCScene::init();
}
CPlanScene::~CPlanScene()
{
    
}

bool CPlanScene::init()
{
    bool bRst = false;
    
    do
    {
        CC_BREAK_IF(!CCScene::init());
        
        m_pMainLayer = CPlanRootLayer::create();
        CC_BREAK_IF(!m_pMainLayer);
        addChild(m_pMainLayer);
        bRst = true;
        
    } while (0);
    
    return bRst;
}
void CPlanScene::setType(EPlanSceneType goalType)
{
    if (m_pMainLayer)
    {
        m_pMainLayer->setType(goalType);
    }
}


