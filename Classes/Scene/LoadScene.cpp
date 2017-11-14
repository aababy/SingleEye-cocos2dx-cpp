#include "LoadScene.h"

USING_NS_CC;
USING_NS_CC_EXT;

#pragma mark - CLoadLayer

CLoadLayer::CLoadLayer()
{

}
CLoadLayer::~CLoadLayer()
{

}

bool CLoadLayer::init()
{
	return (CCLayer::init());
}

void CLoadLayer::onEnter()
{
    do
	{
		CCLayer::onEnter();
        
        xNetEngine->setBusyEnabled(false);
        
        //////////////////////////////////////////////////////////
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
        
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(ui_plist, ui_image);
        SimpleAudioEngine::sharedEngine()->preloadEffect(load_scene_music);
        
        //background
		CCSprite* pBackground = CCSprite::create(background_2);
		pBackground->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
		addChild(pBackground, LOAD_SCENE_ZORDER_BG);
        
        //loading边框
        CCSprite *pProgressFrame = CCSprite::createWithSpriteFrameName(loading_1a);
        pProgressFrame->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
        addChild(pProgressFrame, LOAD_SCENE_ZORDER_PROCESS_FRAME);
        
        //loading条
        CCProgressTimer *pProgressBar = CCProgressTimer::create(CCSprite::createWithSpriteFrameName(loading_1b));
        pProgressBar->setType(kCCProgressTimerTypeBar);
        pProgressBar->setVisible(true);
        pProgressBar->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
        pProgressBar->setMidpoint(ccp(0, 0));
        pProgressBar->setBarChangeRate(ccp(1, 0));
        pProgressBar->setPercentage(0);
        addChild(pProgressBar, LOAD_SCENE_ZORDER_PROCESS_BAR, LOAD_SCENE_TAG_PROCESSBAR);
        
        //load word
        CCSprite *pLoadWord = CCSprite::createWithSpriteFrameName(loading_wd);
        CC_BREAK_IF(!pLoadWord);
        pLoadWord->setPosition(ccp(origin.x + visibleSize.width/2,
                                   origin.y + visibleSize.height/2 + pLoadWord->getContentSize().height));
        addChild(pLoadWord, LOAD_SCENE_ZORDER_WORD, LOAD_SCENE_TAG_WORD);
        CCActionInterval*  actionUp = CCJumpBy::create(2, ccp(0,0), 50, 4);
        pLoadWord->runAction( CCRepeatForever::create(actionUp));
        
        m_fPercentage = 0.0f;
        //////////////////////////////////////////////////////////
        
        this->scheduleUpdate();
        
	} while (0);

}
void CLoadLayer::onExit()
{
    CCLayer::onExit();
    
    xNetEngine->setBusyEnabled(true);
}

void CLoadLayer::toNext()
{
    xSM->forward(ST_Title);
}

void CLoadLayer::update(float delta)
{
    if (m_fPercentage >= 110.0f)
    {
        unscheduleUpdate();
        toNext();
    }
//    else if(10.0f == m_fPercentage)
//    {
//        DATAPOOL->loadEnemyData();
//        DATAPOOL->loadHeroData();
//        DATAPOOL->loadOutfitData();
//    }
//    else if(20.0f == m_fPercentage)
//    {
//        DATAPOOL->initEnemyRecord();
//        DATAPOOL->initHeroRecord();
//        DATAPOOL->initOutfitRecord();
//        DATAPOOL->initLevelRecord();
//    }
    else if(98.0f == m_fPercentage)
    {
//        xData->loadArchive();
//        xData->manualArchive();
        DATAPOOL->loadResource();
   }
    
    CCProgressTimer *m_progressBar = (CCProgressTimer *)(getChildByTag(LOAD_SCENE_TAG_PROCESSBAR));
    if (m_progressBar)
    {
        float percentage = (m_fPercentage > 100.0f)?(100.0f):(m_fPercentage);
        m_progressBar->setPercentage(percentage);
    }
    
    m_fPercentage += 1.0f;
}

#pragma mark - CLoadScene

CLoadScene::CLoadScene(void)
{
	m_pMainLayer = NULL;
}

CLoadScene::~CLoadScene(void)
{
}
bool CLoadScene::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!CCScene::init());
        
		m_pMainLayer = CLoadLayer::create();
		CC_BREAK_IF(!m_pMainLayer);
		addChild(m_pMainLayer);
        
		bRet = true;
        
	} while (0);
    
	return bRet;
}


