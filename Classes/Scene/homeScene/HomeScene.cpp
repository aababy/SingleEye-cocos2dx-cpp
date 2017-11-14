#include "HomeScene.h"

USING_NS_CC;
USING_NS_CC_EXT;

#pragma mark - CHomeLayer

CHomeLayer::CHomeLayer()
{

}
CHomeLayer::~CHomeLayer()
{

}

bool CHomeLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    return true;
}

void CHomeLayer::onEnter()
{
    CCLayer::onEnter();
    
    xNetEngine->setBusyEnabled(false);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(home_scene_plist, home_scene_image);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(home_scene_bg_plist, home_scene_bg_image);
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    //cloud
    CHomeCloudNode *pCloud = CHomeCloudNode::create();
    pCloud->setPosition(ccp(0, 0));
    addChild(pCloud, HOMESCENE_ZORDER_CLOUD);
    
    //control layer
    m_pControlLayer = CHomeControlLayer::create();
    m_pControlLayer->setContentSize(winSize);
    addChild(m_pControlLayer, HOMESCENE_ZORDER_ASTER);
    
    //ui layer
    m_pUILayer = CHomeUILayer::create();
    m_pUILayer->setContentSize(winSize);
    addChild(m_pUILayer, HOMESCENE_ZORDER_UI);
    
    //teach layer
    if (false == DATAPOOL->m_bTeachStepOver[TEACH_STEP_ENTER_MAOXIAN])
    {
        m_pControlLayer->rotationTo(HOME_ROTATION_MIN_RANGE);//旋转至最右侧
        
        //test
        CCPoint itemPos = m_pControlLayer->m_pAster->getItemPosition(2);
        CCPoint asterPos = m_pControlLayer->m_pAster->getPosition();
        CCPoint item2asterPos = m_pControlLayer->m_pAster->convertToWorldSpace(itemPos);
        CCPoint item2ControlLayerPos = ccp(asterPos.x + item2asterPos.x, asterPos.y + item2asterPos.y);
        CCLOG("itemPos=%f,%f",itemPos.x, itemPos.y);
        CCLOG("asterPos=%f,%f",asterPos.x, asterPos.y);
        CCLOG("item2asterPos=%f,%f",item2asterPos.x, item2asterPos.y);
        CCLOG("item2ControlLayerPos=%f,%f",item2ControlLayerPos.x, item2ControlLayerPos.y);
        CCLOG("near pos=%f,%f",winSize.width*3/4 - 30, winSize.height/2 - 100);
        
        m_pTeachLayer = CTeachLayer::create();
        m_pTeachLayer->setPosition(0, 0);
        m_pTeachLayer->setFocusProp(ccp(winSize.width*3/4 - 30, item2ControlLayerPos.y), 150);
        m_pTeachLayer->setPicInfo(CCPoint(winSize.width/2, winSize.height/2 + 300), teach_enter_maoxian);
        m_pTeachLayer->setCallbackFun(this, callfunc_selector(CHomeLayer::CBTeachEnd));
        addChild(m_pTeachLayer, HOMESCENE_ZORDER_TEACH);
    }
    
    
    //music
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(home_scene_back_music);
    
    //////////////////////////////////////////////////////////
    
    //backGround
//    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSprite* pSpriteBg = NULL;
    if (isNight())
    {
        pSpriteBg = CCSprite::createWithSpriteFrameName(home_scene_bg_night);
    }
    else
    {
        pSpriteBg = CCSprite::createWithSpriteFrameName(home_scene_bg_day);
    }
    if (pSpriteBg)
    {
        float bgPosY = winSize.height/2;
        if(pSpriteBg->getContentSize().height > winSize.height)
        {
            bgPosY += pSpriteBg->getContentSize().height/2 - winSize.height/2;
        }
        pSpriteBg->setPosition(ccp(winSize.width - pSpriteBg->getContentSize().width/2, bgPosY));
        addChild(pSpriteBg, HOMESCENE_ZORDER_BG, HOMESCENE_TAG_BG);
    }
    
    xData->tryPlayBgMuisc(BACKMUSIC_TYPE_HOME, true);
    MsgSystem::Instance()->RegisterObjAndHandler(this, MSGSYS_HANLE_MSG(CHomeLayer::handleMsg));

    if (xData->getRequest() == false) {
        xData->setRequest(true);
        xNet->DO_CMD_REQ_APP_SENDING_PODIUM(this, 0);
    }    
    
    syncWithServer();
    
    //提示评论
    if (!xData->getPingLunRcd()
        && !xData->m_bIsShowPingLunYet
        && xData->m_nMaxUnlockLevel[0] >= 5
        )
    {
        this->schedule(schedule_selector(CHomeLayer::checkPingLun), CHECK_TO_PINGLUN_FREQ);
        this->scheduleUpdate();
    }
}
void CHomeLayer::checkPingLun(float dt)
{
    if (HelpMethod::isTimeToPingLun())
    {
        xSM->forward(ST_PingLun);
    }
}

void CHomeLayer::CBTeachEnd()
{
//    CCLOG("teach end");
    DATAPOOL->m_bTeachStepOver[TEACH_STEP_ENTER_MAOXIAN] = true;
    DATAPOOL->saveArchive();
    
    xSM->forward(ST_Dungeon);
}

void CHomeLayer::onExit()
{
    xNetEngine->setBusyEnabled(true);
    
    removeAllChildrenWithCleanup(true);

    unscheduleAllSelectors();
    
    MsgSystem::Instance()->ClearObjHandler(this);
    CCDirector::sharedDirector()->purgeCachedData();
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(home_scene_plist);
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(home_scene_bg_plist);
    
    CCLayer::onExit();
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    
    xData->tryPlayBgMuisc(BACKMUSIC_TYPE_HOME, true);
}
void CHomeLayer::syncWithServer()
{   
#ifdef SIGN_DEBUG    
    return;
#endif    
//    int idx = 0;
//    
//    //同步前赋值
/////    xUser->pUserInfo->gemstone  = xData->gem;                       //1. 宝石
/////    xUser->pUserInfo->strength  = xData->getPower();                //2. 体力
//
//    for (int i = 0; i < xData->m_nHeroNum; i++)                     //4. 玩家英雄数量列表
//    {
//        if (xData->findHeroUnlock(i)) {
//            //有问题, 再讨论.
//            break;            
//            xUser->pUserInfo->listHero[idx]->heroId = First_Hero_ID + i;
//            xUser->pUserInfo->listHero[idx]->grade = xData->m_pHeroRecord[i]->m_nLevel;
//            idx++;
//        }
//    }  
//    
//    xUser->pUserInfo->heroNum   = idx;                              //3. 英雄数长度
    xUser->pUserInfo->heroNum = 0;
    //5. 佣兵数长度 (没有本地数据)
    //6. 玩家佣兵数量列表
    xData->waitSyncPetData();
    
    //xUser->pUserInfo->shareTimes = xData->getShareCounts();         //7. 分享次数, 不做了
    xNet->DO_CMD_REQ_UPDATE_USERINFO(this);
}
//return true 夜晚 return false 白天
bool CHomeLayer::isNight()
{
    struct tm *tm;
    time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    time(&timep);
#else
    struct cc_timeval now;
    CCTime::gettimeofdayCocos2d(&now, NULL);
    timep = now.tv_sec;
#endif
    
    tm = localtime(&timep);
//    int year = tm->tm_year + 1900;
//    int month = tm->tm_mon + 1;
//    int day = tm->tm_mday;
    int hour=tm->tm_hour;
//    int min=tm->tm_min;
//    int second=tm->tm_sec;
    CCLOG("d[w_x]b hour=%d",hour);
    
    //真实时间周期
//    return ( hour >= 20 || hour <= 8 );
    
    //昼夜周期2个小时
    return ((hour % 4 - 2) < 0);
}

bool CHomeLayer::handleMsg(const Message &msg)
{
    switch (msg.m_nMsgType) {
        case CMD_RES_UPDATE_USERINFO:
        {
            //同步服务器数据
     ///       xData->setBagGemNum(xUser->pUserInfo->gemstone);            //1. 宝石
     ///       xData->setPower(xUser->pUserInfo->strength);                //2. 体力
            
#ifdef SIGN_DEBUG              
            xData->setContinueSign(xUser->pUserInfo->signInNum);        //连续签到数
#endif            
            //xData->setShareCounts(xUser->pUserInfo->shareTimes);        //分享次数, 不做了
            
            //雇佣兵信息
#if COCOS2D_DEBUG == 1
            CCLOG("pet num=%d",xUserInfo->mercemaryNum);
            for (int i = 0; i < xUserInfo->mercemaryNum; i++)
            {
                CCLOG("pet_%d,level=%d",xUserInfo->listMercemary[i]->mercemaryId,xUserInfo->listMercemary[i]->grade);
            }
#endif//COCOS2D_DEBUG

            //todo 其它同步暂时未做.
            
            break;
        }
        case CMD_RES_APP_SENDING_PODIUM:
        {            
            xUSString("share_text", xUser->message.c_str());
        }
            break;
        default:
            break;
    }
    
    return true;
}


#pragma mark - CHomeScene

CHomeScene::CHomeScene(void)
{
	m_pMainLayer = NULL;
}

CHomeScene::~CHomeScene(void)
{
    removeAllChildrenWithCleanup(true);
}

// on "init" you need to initialize your instance
bool CHomeScene::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!CCScene::init());
        
		m_pMainLayer = CHomeLayer::create();
		CC_BREAK_IF(!m_pMainLayer);
		addChild(m_pMainLayer);
        
        
		bRet = true;
        
	} while (0);
    
	return bRet;
}

