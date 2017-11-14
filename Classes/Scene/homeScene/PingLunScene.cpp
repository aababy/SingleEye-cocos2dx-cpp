#include "PingLunScene.H"

USING_NS_CC;
USING_NS_CC_EXT;

#pragma mark - CPingLunLayer

CPingLunLayer::CPingLunLayer()
{
    
}
CPingLunLayer::~CPingLunLayer()
{

}

bool CPingLunLayer::init()
{
    return (CCLayer::init());
}
void CPingLunLayer::onEnter()
{
    do {
        CCLayer::onEnter();
        
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("pinglunItem.plist", "pinglunItem.pvr.ccz");
        
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        
        //bg
        CCSprite *pBg = CCSprite::create(background_2);
        pBg->setPosition(ccp(winSize.width/2, winSize.height/2));
        addChild(pBg, PINGLUNSCENE_ZORDER_BG);
        
        //mask
        CCLayerColor *pMask = CCLayerColor::create(ccc4(0,0,0,110));
        addChild(pMask, PINGLUNSCENE_ZORDER_MASK);
        
        //content
        m_pUILayer = UILayer::create();
        addChild(m_pUILayer, PINGLUNSCENE_ZORDER_JSON);
        
        //Layout from json
        Layout *pJsonRoot = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile("HomeScene/PingLunScene.json"));
        CC_BREAK_IF(!pJsonRoot);
        pJsonRoot->setPosition(ccp(winSize.width/2 - pJsonRoot->getContentSize().width/2, winSize.height/2 - pJsonRoot->getContentSize().height/2));
        pJsonRoot->setTag(PINGLUNSCENE_TAG_JSON);
        m_pUILayer->addWidget(pJsonRoot);
        
        //yes btn
        UIButton *pYesBtn = dynamic_cast<UIButton*>(pJsonRoot->getChildByName("Button_Yes"));
        CC_BREAK_IF(!pYesBtn);
        pYesBtn->addReleaseEvent(this, coco_releaseselector(CPingLunLayer::CBYes));
        
        //no btn
        UIButton *pNoBtn = dynamic_cast<UIButton*>(pJsonRoot->getChildByName("Button_No"));
        CC_BREAK_IF(!pNoBtn);
        pNoBtn->addReleaseEvent(this, coco_releaseselector(CPingLunLayer::CBNo));
        
        //later btn
        UIButton *pLaterBtn = dynamic_cast<UIButton*>(pJsonRoot->getChildByName("Button_Later"));
        CC_BREAK_IF(!pLaterBtn);
        pLaterBtn->addReleaseEvent(this, coco_releaseselector(CPingLunLayer::CBNo));
        
        //back item
        CCMenuItemImage *pBackItem = CCMenuItemImage::create(NULL, \
                                                             NULL, \
                                                             this, \
                                                             menu_selector(CPingLunLayer::CBClose));
        pBackItem->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(btn_back));
        pBackItem->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(btn_back2));
        pBackItem->setPosition(ccp(MIN_SPACE_TO_SCREEN_EDGE + pBackItem->getContentSize().width/2, MIN_SPACE_TO_SCREEN_EDGE + pBackItem->getContentSize().height/2));
        CCMenu *pMenu = CCMenu::create(pBackItem, /* pBuyGemItem, */NULL);
        pMenu->setPosition(CCPointZero);
        addChild(pMenu, PINGLUNSCENE_ZORDER_MENU);
        
        xData->m_bIsShowPingLunYet = true;
    
    } while (0);
    
}

void CPingLunLayer::onExit()
{
    CCLayer::onExit();
    HelpMethod::removeUnusedResourceAtExit(this);
}

void CPingLunLayer::CBClose(CCObject* pSender)
{
    xData->playKeytone();
    xSM->back();
}
void CPingLunLayer::CBYes(CCObject* pSender)
{
    xData->playKeytone();
    
    xData->changeBagGemNum(PINGLUN_AWARD_GEM_NUM);
    xData->setPingLunRcd(true);
    xData->saveArchive();
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    CLocalCondition::toPingLun();
#endif
    xSM->back();
    
}
void CPingLunLayer::CBNo(CCObject* pSender)
{
    xData->playKeytone();
    CCTime::gettimeofdayCocos2d(&xData->m_runTime, NULL);
    xSM->back();
}

#pragma mark - CPingLunScene

CPingLunScene::CPingLunScene()
{
    m_pMainLayer = NULL;
}
CPingLunScene::~CPingLunScene()
{
    
}
bool CPingLunScene::init()
{
    bool bRst = false;
    
    do
    {
        CC_BREAK_IF(!CCScene::init());
        
        m_pMainLayer = CPingLunLayer::create();
		CC_BREAK_IF(!m_pMainLayer);
		addChild(m_pMainLayer);
        
        bRst = true;
        
    } while (0);

    return bRst;
}

