#include "AboutScene.h"

USING_NS_CC;
USING_NS_CC_EXT;

#pragma mark - CAboutLayer

bool CAboutLayer::init()
{
    return (CCLayer::init());
}
void CAboutLayer::onEnter()
{
    CCLayer::onEnter();
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(title_scene_plist, title_scene_image);
    
    //////////////////////////////////////////////////////////
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    //bg
    CCSprite *pBg = CCSprite::create(background_2);
    pBg->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
    addChild(pBg);
    
    //mask
    CCLayerColor *pMask = CCLayerColor::create(ccc4(0,0,0,110));
    addChild(pMask);
    
    //content
    CCSprite *pContent = CCSprite::create(about_content);
    pContent->setPosition(CCPoint(winSize.width/2, winSize.height - pContent->getContentSize().height/2));
    addChild(pContent);
    
    //back item
    CCMenuItemImage *pBackItem = CCMenuItemImage::create(NULL, \
                                                         NULL, \
                                                         this, \
                                                         menu_selector(CAboutLayer::CBBack));
    pBackItem->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(btn_back));
    pBackItem->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(btn_back2));
    pBackItem->setPosition(ccp(MIN_SPACE_TO_SCREEN_EDGE + pBackItem->getContentSize().width/2, MIN_SPACE_TO_SCREEN_EDGE + pBackItem->getContentSize().height/2));
    //menu
    CCMenu *pMenu = CCMenu::create(pBackItem, NULL);
    pMenu->setPosition(CCPointZero);
    addChild(pMenu);

}

void CAboutLayer::onExit()
{
    CCLayer::onExit();
    HelpMethod::removeUnusedResourceAtExit(this);
}

void CAboutLayer::CBBack(CCObject* pSender)
{
    xData->playKeytone();
    xSM->back();
}

#pragma mark - CAboutScene

CAboutScene::CAboutScene(void)
{
	m_pMainLayer = NULL;
}

CAboutScene::~CAboutScene(void)
{
    
}
bool CAboutScene::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!CCScene::init());
        
		m_pMainLayer = CAboutLayer::create();
		CC_BREAK_IF(!m_pMainLayer);
		addChild(m_pMainLayer);
    
		bRet = true;
        
	} while (0);
    
	return bRet;
}

