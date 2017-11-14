#include "HuodongScene.h"

USING_NS_CC;
USING_NS_CC_EXT;
CHuodongLayer::CHuodongLayer()
{

}
CHuodongLayer::~CHuodongLayer()
{

}
bool CHuodongLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    return true;
}

void CHuodongLayer::onEnter()
{
    CCLayer::onEnter();
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(huodong_scene_body_plist, huodong_scene_body_image);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    //bg
    CCSprite *pBg = CCSprite::create(background_2);
    pBg->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
    addChild(pBg, HUODONGSCENE_ZORDER_BACKGROUND);
    
    //top bg
    CCSprite *pTopBg = CCSprite::createWithSpriteFrameName(topbar_1);
    pTopBg->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height - pTopBg->getContentSize().height/2));
    addChild(pTopBg, HUODONGSCENE_ZORDER_BACKGROUND);
    
    //top name
    CCSprite *pTopName = CCSprite::createWithSpriteFrameName(huodong_scene_topbar_name);
    pTopName->setPosition(ccp(winSize.width/2 + 10, winSize.height  - pTopBg->getContentSize().height/2 - 12));
    addChild(pTopName, HUODONGSCENE_ZORDER_BACKGROUND);
    
    //每日签到
    CCMenuItemImage *pQianDaoItem = CCMenuItemImage::create(NULL, \
                                                            NULL, \
                                                            this, \
                                                            menu_selector(CHuodongLayer::CBQianDao));
    CC_RETURN_IF(!pQianDaoItem)
    pQianDaoItem->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(huodong_qiandao));
    pQianDaoItem->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(huodong_qiandao));
    
    //boss战斗
    CCMenuItemImage *pBossZhanItem = CCMenuItemImage::create(NULL, \
                                                             NULL, \
                                                             this, \
                                                             menu_selector(CHuodongLayer::CBBossZhan));
    CC_RETURN_IF(!pBossZhanItem);
    pBossZhanItem->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(huodong_boss));
    pBossZhanItem->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(huodong_boss));
    //分享有礼
    CCMenuItemImage *pShareItem = CCMenuItemImage::create(NULL, \
                                                          NULL, \
                                                          this, \
                                                          menu_selector(CHuodongLayer::CBShare));
    CC_RETURN_IF(!pShareItem);
    pShareItem->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(huodong_share));
    pShareItem->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(huodong_share));
    
    //center menu
    float inspace = 1.3;
    float totalHeight = pQianDaoItem->getContentSize().height*inspace + pBossZhanItem->getContentSize().height*inspace +pShareItem->getContentSize().height;
    float fTmpY = winSize.height/2 - totalHeight/2;
    pQianDaoItem->setPosition(ccp(winSize.width/2, fTmpY + pQianDaoItem->getContentSize().height/2));
    fTmpY += pQianDaoItem->getContentSize().height*inspace;
    pBossZhanItem->setPosition(ccp(winSize.width/2, fTmpY + pBossZhanItem->getContentSize().height/2));
    fTmpY += pBossZhanItem->getContentSize().height*inspace;
    pShareItem->setPosition(ccp(winSize.width/2, fTmpY + pShareItem->getContentSize().height/2));
    
    CCMenu *pCenterMenu = CCMenu::create(pQianDaoItem, pBossZhanItem, pShareItem, NULL);
    pCenterMenu->setPosition(CCPointZero);
    addChild(pCenterMenu, HUODONGSCENE_ZORDER_MENU);
    
    //back btn
    CCMenuItemImage *pBackItem = CCMenuItemImage::create(NULL, \
                                                         NULL, \
                                                         this, \
                                                         menu_selector(CHuodongLayer::CBBack));
    pBackItem->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(btn_back));
    pBackItem->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(btn_back2));
    pBackItem->setPosition(ccp(MIN_SPACE_TO_SCREEN_EDGE + pBackItem->getContentSize().width/2, MIN_SPACE_TO_SCREEN_EDGE + pBackItem->getContentSize().height/2));
    CCMenu *pMenu = CCMenu::create(pBackItem, NULL);
    pMenu->setPosition(CCPointZero);
    addChild(pMenu, BUYGEMSCENE_ZORDER_MENU);
    

    this->setKeypadEnabled(true);
}

void CHuodongLayer::onExit()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(huodong_scene_body_plist);
    CCLayer::onExit();
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}


void CHuodongLayer::CBQianDao(CCObject *pSender)
{
    xData->playKeytone();
    
    xSM->forward(ST_Sign);
}
void CHuodongLayer::CBBossZhan(CCObject *pSender)
{
    xData->playKeytone();
    
    xSM->forward(ST_Boss);
}
void CHuodongLayer::CBShare(CCObject *pSender)
{
    xData->playKeytone();
    
    xSM->forward(ST_Share);
}
void CHuodongLayer::CBBack(CCObject *pSender)
{
    xData->playKeytone();
    xSM->back();
}

CHuodongScene::CHuodongScene(void)
{
	m_pMainLayer = NULL;
}

CHuodongScene::~CHuodongScene(void)
{
}

bool CHuodongScene::init()
{
    bool bRst = false;
    
    do
    {
        CC_BREAK_IF(!CCScene::init());
        
        m_pMainLayer = CHuodongLayer::create();
        CC_BREAK_IF(!m_pMainLayer);
        addChild(m_pMainLayer);
        bRst = true;
        
    } while (0);
    
    return bRst;
}


