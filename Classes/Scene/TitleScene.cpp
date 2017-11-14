#include "TitleScene.h"
//#include "native/CCNative.h"
USING_NS_CC;
USING_NS_CC_EXT;

#pragma mark - CTitleLayer

CTitleLayer::CTitleLayer()
{
//    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(title_scene_plist, title_scene_image);
//     DATAPOOL->addEffectCache(title_scene_startBtn);
}
CTitleLayer::~CTitleLayer()
{
//    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(title_scene_plist);
//    DATAPOOL->removeEffectCache(title_scene_startBtn);
}
bool CTitleLayer::init()
{

	if(!CCLayer::init())
    {
        return false;
    }

    return true;
}
void CTitleLayer::onEnter()
{
    CCLayer::onEnter();
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(title_scene_plist, title_scene_image);
    DATAPOOL->addEffectCache(title_scene_startBtn);
    
    //////////////////////////////////////////////////////////
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    //bg
    CCSprite *pBg = CCSprite::createWithSpriteFrameName(background_1);
    pBg->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
    addChild(pBg);
    
//    //title
//    CCSprite *pTitle = CCSprite::createWithSpriteFrameName(title_1);
//    pTitle->setPosition(CCPoint(winSize.width/2, winSize.height - pTitle->getContentSize().height/2));
//    addChild(pTitle);
//    
//    //title2
//    CCSprite *pTitle2 = CCSprite::createWithSpriteFrameName(title_2);
//    pTitle2->setPosition(CCPoint(winSize.width/2, winSize.height - pTitle->getContentSize().height - pTitle2->getContentSize().height/2 + 80));
//    addChild(pTitle2);
    
    //start button
    //        ButtonSprite* pStartButton = ButtonSprite::createButtonSprite(startBtn_01_a);
    //        pStartButton->setPosition(ccp(origin.x + visibleSize.width/2 ,
    //                                 origin.y + pStartButton->getContentSize().height));
    //        pStartButton->setOnClickListener(click_selector(CTitleLayer::CBMenuStart));
    //        addChild(pStartButton, 1);
    
    //        CButtonAnimation *pStartBtn = CButtonAnimation::create();
    //        pStartBtn->setPosition(ccp(winSize.width/2, pStartBtn->getContentSize().height + 200));
    //        pStartBtn->setCallbackFun(this, callfunc_selector(CTitleLayer::CBMenuStart));
    //        addChild(pStartBtn, 1);
    
    m_pUILayer = UILayer::create();
    addChild(m_pUILayer, 3);
    UIButton *pStartaButton = UIButton::create();
    pStartaButton->addReleaseEvent(this, coco_releaseselector(CTitleLayer::CBMenuStart));
    CCArmature* pArmature = CCArmature::create("anniu");
    pArmature->setPosition(ccp(0, 0));
    pArmature->getAnimation()->playByIndex(0);
    pStartaButton->addRenderer(pArmature, 0);
    pStartaButton->setSizeType(SIZE_ABSOLUTE);
    pStartaButton->ignoreContentAdaptWithSize(false);
    pStartaButton->setSize(pArmature->getContentSize());
    pStartaButton->setPosition(ccp(winSize.width/2, winSize.height/2 - pStartaButton->getContentSize().height/2 - 300));
    pStartaButton->setTouchEnable(true);
    m_pUILayer->addWidget(pStartaButton);
    //////////////////////////////////////////////////////////
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(title_scene_menu_start);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(title_scene_back_music);
    
    this->setKeypadEnabled(true);
    
    xData->tryPlayBgMuisc(BACKMUSIC_TYPE_TITLE, true);
}

void CTitleLayer::onExit()
{
    CCLayer::onExit();
    HelpMethod::removeUnusedResourceAtExit(this);
    CCArmatureDataManager::purge();
}

void CTitleLayer::CBMenuStart(CCObject* pSender)
{
    CCLOG("entern CBMenuStart");
//    if (true) {
//        //2.4.6.21	购买宝石--首次请求(CMD_REQ_BUY_GEMSTONE) 56
//        (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_BUY_GEMSTONE(this, 1);
//        return;
//    }

    xData->playKeytone();
    
#if 1
    if (DATAPOOL->firsttime)
    {
        xSM->forward(ST_Story);
    }
    else
    {
        if (xData->checkSign()) {
            xSM->forward(ST_Home, 1, NULL);
//            xSM->forward(ST_Dungeon, 1, NULL);
        }
        else
        {
            xSM->forward(ST_Sign, 1, NULL);
            
        }
    }

#else//调试用
    xSM->forward(ST_Boss);
#endif
}
//void CTitleLayer::CBMenuSetting(CCObject* pSender)
//{
//    CCMenuItemImage *pSettingItem = (CCMenuItemImage *)pSender;
//    if (pSettingItem)
//    {
//        CCRotateBy* rotate = CCRotateBy::create(1, 360);
//        CCRepeatForever* repeatRotate = CCRepeatForever::create(rotate);
//        pSettingItem->runAction(repeatRotate);
//    }
//}

#pragma mark - CTitleScene

CTitleScene::CTitleScene(void)
{
	m_pMainLayer = NULL;
}

CTitleScene::~CTitleScene(void)
{
    removeAllChildrenWithCleanup(true);
}
bool CTitleScene::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!CCScene::init());
        
		m_pMainLayer = CTitleLayer::create();
		CC_BREAK_IF(!m_pMainLayer);
		addChild(m_pMainLayer);
    
		bRet = true;
        
	} while (0);
    
	return bRet;
}

