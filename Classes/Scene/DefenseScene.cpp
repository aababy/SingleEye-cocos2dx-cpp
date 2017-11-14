#include "GameInclude.h"

USING_NS_CC;
USING_NS_CC_EXT;

#pragma mark - root

CDefenseRootLayer::CDefenseRootLayer(void)
{
    
}
CDefenseRootLayer::~CDefenseRootLayer(void)
{
    
}
bool CDefenseRootLayer::init()
{
    return (CCLayer::init());
}
void CDefenseRootLayer::onEnter()
{
    CCLayer::onEnter();
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(plan_scene_body_plist, plan_scene_body_image);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(tavern_scene_body_plist, tavern_scene_body_image);
    
    MainInit();
    MainFresh();
    
}
void CDefenseRootLayer::onExit()
{
    CCLayer::onExit();
    HelpMethod::removeUnusedResourceAtExit(this);
    CCArmatureDataManager::purge();
}
void CDefenseRootLayer::MainInit()
{
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite *pBg = CCSprite::createWithSpriteFrameName(background_0);
    pBg->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
    addChild(pBg);
    
    m_pUILayer = UILayer::create();
    addChild(m_pUILayer);
    
    m_pLayout = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(defense_scene_json));
    m_pLayout->setPosition(ccp(winSize.width/2 - m_pLayout->getContentSize().width/2, winSize.height/2 - m_pLayout->getContentSize().height/2));
    m_pUILayer->addWidget(m_pLayout);
    
    //防线1 select
    {
        UIPanel *pPanel = static_cast<UIPanel*>(m_pLayout->getChildByName("GuYongBingPanel"));
        
        Layout *pItemPanel_0 = static_cast<Layout*>(pPanel->getChildByName("ItemPanel_0"));
        UIButton *pBtn0 = static_cast<UIButton*>(pItemPanel_0->getChildByName("Button"));
        pBtn0->setTag(0);
        pBtn0->addTouchEventListener(this, toucheventselector(CDefenseRootLayer::CBSelectLine1));
        
        Layout *pItemPanel_1 = static_cast<Layout*>(pPanel->getChildByName("ItemPanel_1"));
        UIButton *pBtn1 = static_cast<UIButton*>(pItemPanel_1->getChildByName("Button"));
        pBtn1->setTag(1);
        pBtn1->addTouchEventListener(this, toucheventselector(CDefenseRootLayer::CBSelectLine1));
        
        Layout *pItemPanel_2 = static_cast<Layout*>(pPanel->getChildByName("ItemPanel_2"));
        UIButton *pBtn2 = static_cast<UIButton*>(pItemPanel_2->getChildByName("Button"));
        pBtn2->setTag(2);
        pBtn2->addTouchEventListener(this, toucheventselector(CDefenseRootLayer::CBSelectLine1));
        
        Layout *pItemPanel_3 = static_cast<Layout*>(pPanel->getChildByName("ItemPanel_3"));
        UIButton *pBtn3 = static_cast<UIButton*>(pItemPanel_3->getChildByName("Button"));
        pBtn3->addTouchEventListener(this, toucheventselector(CDefenseRootLayer::CBSelectLine1));
        pBtn3->setTag(3);
    }
    
    //防线2 select
    {
        UIPanel *pPanel = static_cast<UIPanel*>(m_pLayout->getChildByName("GuYongBingPanel_2"));
        
        Layout *pItemPanel_0 = static_cast<Layout*>(pPanel->getChildByName("ItemPanel_0"));
        UIButton *pBtn0 = static_cast<UIButton*>(pItemPanel_0->getChildByName("Button"));
        pBtn0->setTag(0);
       pBtn0->addTouchEventListener(this, toucheventselector(CDefenseRootLayer::CBSelectLine2));
        
        Layout *pItemPanel_1 = static_cast<Layout*>(pPanel->getChildByName("ItemPanel_1"));
        UIButton *pBtn1 = static_cast<UIButton*>(pItemPanel_1->getChildByName("Button"));
        pBtn1->setTag(1);
       pBtn1->addTouchEventListener(this, toucheventselector(CDefenseRootLayer::CBSelectLine2));
        
        Layout *pItemPanel_2 = static_cast<Layout*>(pPanel->getChildByName("ItemPanel_2"));
        UIButton *pBtn2 = static_cast<UIButton*>(pItemPanel_2->getChildByName("Button"));
        pBtn2->setTag(2);
      pBtn2->addTouchEventListener(this, toucheventselector(CDefenseRootLayer::CBSelectLine2));
        
        Layout *pItemPanel_3 = static_cast<Layout*>(pPanel->getChildByName("ItemPanel_3"));
        UIButton *pBtn3 = static_cast<UIButton*>(pItemPanel_3->getChildByName("Button"));
      pBtn3->addTouchEventListener(this, toucheventselector(CDefenseRootLayer::CBSelectLine2));
        pBtn3->setTag(3);
    }
 
    //UI
    CMoneyItem *pGoldCoin = new CMoneyItem(MONEY_ITEM_TYPE_GOLD);
    pGoldCoin->autorelease();
///    pGoldCoin->setPosition(125, winSize.height - 50 - pGoldCoin->getContentSize().height);
    addChild(pGoldCoin, DEFENSE_ROOT_LAYER_ZORDER_UI, DEFENSE_UI_TAG_GOLD);
    
    CMoneyItem *pGem = new CMoneyItem(MONEY_ITEM_TYPE_GEM);
    pGem->autorelease();
///    pGem->setPosition(winSize.width-260, winSize.height - 50 - pGem->getContentSize().height);
    addChild(pGem, DEFENSE_ROOT_LAYER_ZORDER_UI, DEFENSE_UI_TAG_GEM);
    
    CCMenuItemImage *pBackItem = CCMenuItemImage::create(NULL, \
                                                          NULL, \
                                                          this, \
                                                          menu_selector(CDefenseRootLayer::CBBack));
    pBackItem->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(btn_back));
    pBackItem->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(btn_back2));
    pBackItem->setPosition(ccp(MIN_SPACE_TO_SCREEN_EDGE + pBackItem->getContentSize().width/2, MIN_SPACE_TO_SCREEN_EDGE + pBackItem->getContentSize().height/2));
    
    CCMenuItemImage *pNextItem = CCMenuItemImage::create(NULL, \
                                                         NULL, \
                                                         this, \
                                                         menu_selector(CDefenseRootLayer::CBNext));
    pNextItem->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(btn_next));
    pNextItem->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(btn_next2));
    pNextItem->setPosition(ccp(winSize.width - MIN_SPACE_TO_SCREEN_EDGE - pNextItem->getContentSize().width/2,
                               MIN_SPACE_TO_SCREEN_EDGE + pNextItem->getContentSize().height/2));
    
    CCMenuItemImage *pShopItem = CCMenuItemImage::create(NULL, \
                                                         NULL, \
                                                         this, \
                                                         menu_selector(CDefenseRootLayer::CBToShop));
    pShopItem->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(top_ico_shop));
    pShopItem->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(top_ico_shop));
    pShopItem->setPosition(ccp(origin.x + visibleSize.width - pShopItem->getContentSize().width/2,
                               origin.y + visibleSize.height - pShopItem->getContentSize().height/2));
    
    CCMenu* pMenu = CCMenu::create(pBackItem, pNextItem, pShopItem, NULL);
    pMenu->setPosition(CCPointZero);
    addChild(pMenu, DEFENSE_ROOT_LAYER_ZORDER_MENU);

}

void CDefenseRootLayer::MainFresh()
{
    {
        UIPanel *pPanel = static_cast<UIPanel*>(m_pLayout->getChildByName("GuYongBingPanel"));
        
        Layout *pItemPanel_0 = static_cast<Layout*>(pPanel->getChildByName("ItemPanel_0"));
        UIImageView *pImg0 = static_cast<UIImageView*>(pItemPanel_0->getChildByName("ImageView"));
        setHeadItem(1, 0, pImg0);
        
        Layout *pItemPanel_1 = static_cast<Layout*>(pPanel->getChildByName("ItemPanel_1"));
        UIImageView *pImg1 = static_cast<UIImageView*>(pItemPanel_1->getChildByName("ImageView"));
        setHeadItem(1, 1, pImg1);
        
        Layout *pItemPanel_2 = static_cast<Layout*>(pPanel->getChildByName("ItemPanel_2"));
        UIImageView *pImg2 = static_cast<UIImageView*>(pItemPanel_2->getChildByName("ImageView"));
        setHeadItem(1, 2, pImg2);
        
        Layout *pItemPanel_3 = static_cast<Layout*>(pPanel->getChildByName("ItemPanel_3"));
        UIImageView *pImg3 = static_cast<UIImageView*>(pItemPanel_3->getChildByName("ImageView"));
        setHeadItem(1, 3, pImg3);
    }
    
    {
        UIPanel *pPanel = static_cast<UIPanel*>(m_pLayout->getChildByName("GuYongBingPanel_2"));
        
        Layout *pItemPanel_0 = static_cast<Layout*>(pPanel->getChildByName("ItemPanel_0"));
        UIButton *pBtn0 = static_cast<UIButton*>(pItemPanel_0->getChildByName("Button"));
        UIImageView *pImg0 = static_cast<UIImageView*>(pItemPanel_0->getChildByName("ImageView"));
        setHeadItem(2, 0, pImg0);
        
        Layout *pItemPanel_1 = static_cast<Layout*>(pPanel->getChildByName("ItemPanel_1"));
        UIButton *pBtn1 = static_cast<UIButton*>(pItemPanel_1->getChildByName("Button"));
        UIImageView *pImg1 = static_cast<UIImageView*>(pItemPanel_1->getChildByName("ImageView"));
        setHeadItem(2, 1, pImg1);
        
        Layout *pItemPanel_2 = static_cast<Layout*>(pPanel->getChildByName("ItemPanel_2"));
        UIButton *pBtn2 = static_cast<UIButton*>(pItemPanel_2->getChildByName("Button"));
        UIImageView *pImg2 = static_cast<UIImageView*>(pItemPanel_2->getChildByName("ImageView"));
        setHeadItem(2, 2, pImg2);
        
        Layout *pItemPanel_3 = static_cast<Layout*>(pPanel->getChildByName("ItemPanel_3"));
        UIButton *pBtn3 = static_cast<UIButton*>(pItemPanel_3->getChildByName("Button"));
        UIImageView *pImg3 = static_cast<UIImageView*>(pItemPanel_3->getChildByName("ImageView"));
        setHeadItem(2, 3, pImg3);
        
        pBtn0->setTouchEnable(true);
        pBtn1->setTouchEnable(true);
        pBtn2->setTouchEnable(true);
        pBtn3->setTouchEnable(true);
                
        Layout *pLockPanel = static_cast<Layout*>(m_pLayout->getChildByName("unlock_Panel"));
        UIButton *pLockBtn = static_cast<UIButton*>(m_pLayout->getChildByName("unlock_Button"));

        {
            pLockPanel->setVisible(false);
            pLockPanel->disable();
            pLockBtn->setVisible(false);
            pLockBtn->setTouchEnable(false);
        }
    }
    
    //UI
    CMoneyItem *pGold = (CMoneyItem *)(getChildByTag(DEFENSE_UI_TAG_GOLD));
    if (pGold)
    {
        pGold->setNum(DATAPOOL->getGoldCoin());
    }
    CMoneyItem *pGem = (CMoneyItem *)(getChildByTag(DEFENSE_UI_TAG_GEM));
    if (pGem)
    {
        pGem->setNum(xUser->pUserInfo->gemstone);
    }
}
void CDefenseRootLayer::setHeadItem(int type, int n, UIImageView *pImg)
{
    int nIndex = 0;
    CCSize oldSize = pImg->getContentSize();
    CCPoint oldPos = pImg->getPosition();
    
    switch(type)
    {
        case 1://防线1
            nIndex = DATAPOOL->getPlanPet(n, 1);
            break;
        case 2://防线2
            nIndex = DATAPOOL->getPlanPet(n, 2);
            break;
            
        default:
            return;
    }
    
    if (nIndex < 0)
    {
        pImg->setTexture(plan_item_lock, UI_TEX_TYPE_PLIST);
        pImg->setVisible(true);
        
        //调整坐标 使head的底部中点和layout的中心点重合,即底对齐
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

}

void CDefenseRootLayer::CBSelectLine1(CCObject* pSender, TouchEventType type)
{
    if (TOUCH_EVENT_ENDED != type)
    {
        return;
    }
    
    xData->playKeytone();
    
    UIButton *pBtn = (UIButton*)(pSender);
    int tag = pBtn->getTag();
    int index = DATAPOOL->getPlanPet(tag, 1);
    DATAPOOL->m_nDefenseSetGoalIndex_1 = tag;
    DATAPOOL->m_nDefenseSetGoalIndex_2 = -1;
    if (index >= 0)
    {
        xSM->forward(ST_SelectPet);
    }
    else
    {
        xDialog->showYesNo(xStr("unlock_pos"), dialogeventselector(CDefenseRootLayer::CBPopupBack), this);
        xDialog->setDialogID(1);
    }
}
void CDefenseRootLayer::CBSelectLine2(CCObject* pSender, TouchEventType type)
{
    if (TOUCH_EVENT_ENDED != type)
    {
        return;
    }
    
    xData->playKeytone();
    
    UIButton *pBtn = (UIButton*)(pSender);
    int tag = pBtn->getTag();
    int index = DATAPOOL->getPlanPet(tag, 2);
    DATAPOOL->m_nDefenseSetGoalIndex_1 = -1;
    DATAPOOL->m_nDefenseSetGoalIndex_2 = tag;
    if (index >= 0)
    {
        xSM->forward(ST_SelectPet);
    }
    else
    {
        xDialog->showYesNo(xStr("unlock_pos"), dialogeventselector(CDefenseRootLayer::CBPopupBack), this);
        xDialog->setDialogID(2);
    }
}

void CDefenseRootLayer::CBPopupBack(int id, int iTag)
{
    switch (id)
    {
        case 1://解锁防线1的位置
        {
            if(0 == iTag)
            {
                int nPrice = 1;//todo:价格从配置文件读入
                if (DATAPOOL->changeBagGemNum(-nPrice))//扣除宝石,解锁
                {
                    DATAPOOL->setPlanPet(DATAPOOL->m_nDefenseSetGoalIndex_1, 0, 1);
                    MainFresh();
                }
                else//宝石不足
                {
                    xDialog->showYesNo(xStr("lack_of_gem"), dialogeventselector(CDataPool::CBLackOfGem), this);
                }
            }
        }
            break;
            
        case 2://解锁防线2的位置
        {
            if(0 == iTag)
            {
                int nPrice = 1;//todo:价格从配置文件读入
                if (DATAPOOL->changeBagGemNum(-nPrice))//扣除宝石,解锁
                {
                    DATAPOOL->setPlanPet(DATAPOOL->m_nDefenseSetGoalIndex_2, 0, 2);
                    MainFresh();
                }
                else//宝石不足
                {
                    xDialog->showYesNo(xStr("lack_of_gem"), dialogeventselector(CDataPool::CBLackOfGem), this);
                }
            }
        }
            break;
    }
}

void CDefenseRootLayer::CBBack(CCObject* pSender)
{
    xData->playKeytone();
    DATAPOOL->saveArchive();
    xSM->back();
}
void CDefenseRootLayer::CBNext(CCObject* pSender)
{
    xData->playKeytone();
    DATAPOOL->saveArchive();
    xSM->forward(ST_Fight, Game_Type_Endless, NULL);
}
void CDefenseRootLayer::CBToShop(CCObject *pSender)
{
    xData->playKeytone();
    xSM->forward(ST_Shop);
}

#pragma mark - CDefenseScene

CDefenseScene::CDefenseScene()
{

}
CDefenseScene::~CDefenseScene()
{
    
}

bool CDefenseScene::init()
{
    bool bRst = false;
    
    do
    {
        CC_BREAK_IF(!CCScene::init());
        
        m_pMainLayer = CDefenseRootLayer::create();
        CC_BREAK_IF(!m_pMainLayer);
        addChild(m_pMainLayer);
        bRst = true;
        
    } while (0);
    
    return bRst;
}


