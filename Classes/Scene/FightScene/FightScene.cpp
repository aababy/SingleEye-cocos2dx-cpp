//
//  FightScene.cpp
//  SingleEye
//
//  Created by qinfen on 13-10-30.
//
//

#include "FightScene.h"

#pragma mark - FightScene
USING_NS_CC_EXT;
FightScene::~FightScene()
{

}

bool FightScene::init()
{
    if (!CCScene::init()) {
        return false;
    }
    
    FightLayer *_layer = FightLayer::create();
    addChild(_layer,0,1);
    
    
    PauseLayer *_pauseLayer = PauseLayer::create();
    addChild(_pauseLayer,2,2);
    _pauseLayer->setVisible(false);
    
    ResultLayer *_resultLayer = ResultLayer::create();
    addChild(_resultLayer,2,3);
    _resultLayer->setVisible(false);
    
    return true;
}

void FightScene::setGameSpeHero(int *hero)
{
    FightLayer *_layer = (FightLayer*)getChildByTag(1);
    _layer->setSpeHero(hero);
}

void FightScene::setGameLevel(int chapter, int level)
{
    ((FightLayer*)getChildByTag(1))->setLevel(chapter, level);
}

void FightScene::setGameProps(int *props)
{
    FightLayer *_layer = (FightLayer*)getChildByTag(1);
    _layer->setProps(props);
}

void FightScene::setGameLans(int *lans)
{
    FightLayer *_layer = (FightLayer*)getChildByTag(1);
    _layer->setLansquenet(lans);
}

void FightScene::setGameLansTwo(int *lans)
{
    FightLayer *_layer = (FightLayer*)getChildByTag(1);
    _layer->setLansquenetTwo(lans);
}

void FightScene::setGameType(Game_Type type)
{
    FightLayer *_layer = (FightLayer*)getChildByTag(1);
    _layer->setGameType(type);
}

void FightScene::setBossHp(int hp)
{
    FightLayer *_layer = (FightLayer*)getChildByTag(1);
    _layer->setBossHp(hp);
}

void FightScene::onEnter()
{
    DATAPOOL->loadFightSceneResource();
    CCScene::onEnter();
}

void FightScene::onExit()
{
    removeAllChildrenWithCleanup(true);
    cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->purge();
    CCScene::onExit();
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
}

#pragma mark - FightLayer
#pragma mark - public init
FightLayer::FightLayer():
m_pBullet(NULL),
m_pLevelInfo(NULL),
m_pLevelData(NULL),
m_nCurrentWave(0),
m_nGameState(0),
m_pEnemy(NULL),
m_pBarrier(NULL),
m_nCurrentLevel(1),
m_nCurrentChapter(1),
m_nHaveSpeHero(NULL),
m_fGamePercent(0),
m_bIsUseLianNu(false),
m_bIsFireLianNu(false),
m_pInk(NULL),
m_eType(Game_Type_Level),
m_pPeel(NULL),
m_pIceWall(NULL),
m_nFireCount(0),
m_nHitCount(0),
m_nBarrierTotalHp(0),
m_nBarrierCurrentHp(0),
m_nBarrierTotalHp_Second(0),
m_nBarrierCurrentHp_Second(0),
m_pAward(NULL),
m_pShiBei(NULL),
m_nBestTime(0),
m_nTime(0),
m_bUseHongZhaJi(false),
m_bUseKuangBao(false),
m_bUseShuaXin(false),
m_bLockHero(false),
m_pFog(NULL),
m_LansIndex(0),
m_touchPropIndex(0),
m_nGameGainGem(0),
m_nGameUseGem(0),
m_bIsComplateLoading(false),
m_bDelayComplate(false),
m_bcreateEnemy(false),
m_bDanGongFire_Teach(true),
m_bDanGongPowerFire_Teach(true),
m_bUseHongZhaji_Teach(true),
m_bUseNewHero_Teach(true),
m_bStartingFire_Teach(false),
m_bStartingPowerFire_Teach(false),
m_bStartingUseNewHero_Teach(false),
m_bStartingHongZhaJi_Teach(false),
m_bDaoShuComplate(false),
m_nGainGold(0),
m_bEnemyInfo(false),
m_bShowTeachLayer(false),
//m_nLoadingPicNums(0),
//m_nLoadingPicNum(0),
m_bHitComb(false),
m_bShowResultLock(false),
m_bCanPullCatapult(true),
m_bIsSnatch(false),
m_pQiangbaoGuai(NULL),
m_bCompleteWave(false),
m_bStartQiangBao_Teach(false),
m_nBossType(0),
m_nXuLiDeltaTime(0),
m_bEnemyInfoShow(false),
m_bTwoBoss(false),
m_bFirstWave(true),
m_nCombo(0),
m_nEnergy(0),
m_bEnergy(false),
m_bPartner(false),
m_bShowPartner(false),
m_nTornadoCount(0),
m_nHuoQiuCount(0),
m_nBossHp(0),
m_nBossHurtHp(0),
m_nMuBeiCount(0),
m_fLoadPercent(0.0),
m_bLastWave(false),
m_nTotalWave(0),
m_bcreatePartner(false)
{
    CCLOG("create fight!");
}

FightLayer::~FightLayer()
{
    CCLOG("~FightLayer");
}

bool FightLayer::init()
{
    if (!CCLayer::init()) {
        return false;
    }
    setTouchEnabled(true);
    m_nHaveSpeHero = new int[Max_Game_Hero];
    for(int i = 0;i<Max_Game_Hero;++i)
    {
        m_nHaveSpeHero[i] = 0;
    }
    
    m_nUsePropType =  new int[Max_Game_Prop];
    for (int i = 0; i<Max_Game_Prop; ++i)
    {
        m_nUsePropType[i] = 0;
    }
    
    m_nUseLansTypeOne = new int[Max_Game_Lans];
    for (int i = 0; i<Max_Game_Lans; ++i)
    {
        m_nUseLansTypeOne[i] = 0;
    }
    
    m_nUseLansTypeTwo = new int [Max_Game_Lans];
    for (int i = 0; i<Max_Game_Lans; ++i)
    {
        m_nUseLansTypeTwo[i] = 0;
    }
    
    m_fPower = 0.0;
    
    m_powerTime.mCurTime = 0;
    m_powerTime.mTotalTime = Power_Time;
    
    m_fireDeltaTime.mCurTime = 0;
    m_fireDeltaTime.mTotalTime = Fire_Delta_Time;
    
    m_lianNuDeltaTime.mCurTime = 0;
    m_lianNuDeltaTime.mTotalTime = LianNu_DeltaTime;
    
    m_lianNuUseTime.mCurTime = 0;
    m_lianNuUseTime.mTotalTime = 0;
    
    m_explosionNum.mCurTime = 0;
    m_explosionNum.mTotalTime = Explosion_Num;
    
    m_waveTime.mCurTime = 0;
    m_waveTime.mTotalTime = Wave_Time;
    
    m_speakTime.mCurTime = 0;
    m_speakTime.mTotalTime = SPEAK_TIME;
    
    m_pEnemy = CCArray::create();
    m_pEnemy->retain();
    m_pBarrier = CCArray::create();
    m_pBarrier->retain();
    
    m_pBullet = CCArray::create();
    m_pBullet->retain();
    
    m_pHero = CCArray::create();
    m_pHero->retain();
    
    m_pInk = CCArray::create();
    m_pInk->retain();
    
    m_pPeel = CCArray::create();
    m_pPeel->retain();
    
    m_pAward = CCArray::create();
    m_pAward->retain();
    
    m_pShiBei = CCArray::create();
    m_pShiBei->retain();
    
    m_pIceWall = CCArray::create();
    m_pIceWall->retain();
    
    m_pFog = CCArray::create();
    m_pFog->retain();
    
    m_pMuBei = CCArray::create();
    m_pMuBei->retain();
    return true;
}
                               

void FightLayer::draw()
{
    if(!m_bIsComplateLoading)return;

    CCSprite *pTray = (CCSprite*)getChildByTag(Fight_Sprites_Catapult_Tray);
    CCSprite *pFrame = (CCSprite*)getChildByTag(Fight_Sprites_Catapult_Frame);
    float lineWidth = 5.0 * CC_CONTENT_SCALE_FACTOR();
    glLineWidth( lineWidth);
	ccDrawColor4B(0,0,0,255);
    int i = pTray->getContentSize().width/2;
    int j = (int)HelpMethod::getAngleDX(-m_fAngle, i);
    int k = (int)HelpMethod::getAngleDY(-m_fAngle, i);
    float x1 = 0;
    float y1 = 0;
    float x2 = 0;
    float y2 = 0;
    if(m_bIsUseLianNu)
    {
        int frameWidth = pFrame->getContentSize().width/2;
        x1 = pFrame->getPositionX() - HelpMethod::getAngleDX(m_fAngle, frameWidth);
        y1 = pFrame->getPositionY() - HelpMethod::getAngleDY(m_fAngle, frameWidth);
        x2 = pFrame->getPositionX() + HelpMethod::getAngleDX(m_fAngle, frameWidth);
        y2 = pFrame->getPositionY() + HelpMethod::getAngleDY(m_fAngle, frameWidth);
    }
    else
    {
        x1 = m_fCatapult_Org_X - pFrame->getContentSize().width/2 + 20;
        y1 = m_fCatapult_Org_Y + 25.0f;
        x2 = m_fCatapult_Org_X + pFrame->getContentSize().width/2 - 18;
        y2 = m_fCatapult_Org_Y  + 25.0f;
    }
    ccDrawLine(ccp(x1,y1), ccp(pTray->getPositionX() - j+2,k + pTray->getPositionY()+2));
    
    
    
    ccDrawLine(ccp(x2,y2), ccp(j + pTray->getPositionX()-2,pTray->getPositionY() - k+2));
    
    Hero* bullet = (Hero*)getChildByTag(Fight_Sprites_Bullet);
    if (bullet&&bullet->isPowerState()&&bullet->getType() == Hero_Type_BoLuo)
    {
        CCArmature *zhunxin = (CCArmature*)getChildByTag(Fight_Sprites_ZhunXin);
        if(!zhunxin)
        {
            DATAPOOL->addEffectCache(ZhunXinRes);
            zhunxin = CCArmature::create("zhunxin");
            zhunxin->getAnimation()->play("zhunxin");
            addChild(zhunxin,Fight_Zorder_Bullet,Fight_Sprites_ZhunXin);
        }
        int delta = (Min_Angle - Max_Angle)/5;
        float angle = 0;
        angle = Min_Angle + m_fAngle;
        int index = angle/delta;
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        float dx = size.width/5;
        if(index!=5)
        {
            zhunxin->setPosition(ccp(dx*index+dx/2, Barrier_Y+Atk_Barrier_DY));
            explosionPosX =dx*index+dx/2;
        }
    }
}

void FightLayer::update(float dt)
{
    if(!m_bIsComplateLoading)return;
    if(!m_bDaoShuComplate)return;
    if(m_bShowPartner)return;
    
    checkTeachLayer(dt);
    
    CTeachLayer *teachLayer = NULL;
    teachLayer = (CTeachLayer*)getChildByTag(Fight_Sprites_TeachLayer);
    if(teachLayer)
    {
        m_bShowTeachLayer = true;
        return;
    }
    
    checkCollision(dt);
    checkPeel(dt);
    checkResult(dt);
    checkNextWave(dt);
    checkPull(dt);
    checkTrans();
    updateLianNuDelta(dt);
    updateEnemyZorder();
    checkState();
    checkLansSkill();
    checkSpeak(dt);
}

void FightLayer::onEnter()
{
	CCLog("OnEnter!");
    CCLayer::onEnter();
    CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
    MsgSystem::Instance()->RegisterObjAndHandler(this, MSGSYS_HANLE_MSG(FightLayer::handleMsg));
    //禁用Busy提示
    xNetEngine->setBusyEnabled(false);
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    //background
    CCSprite* pBackground = CCSprite::create(Loading_BG);
    pBackground->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
    addChild(pBackground, 0,Fight_Sprites_Load_BG);
    
    //loading边框
    CCSprite *pProgressFrame = CCSprite::createWithSpriteFrameName(Loading_Progress_Frame);
    pProgressFrame->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
    addChild(pProgressFrame, 1,Fight_Sprites_Load_Progress);
    
    //loading条
    CCProgressTimer *pProgressBar = CCProgressTimer::create(CCSprite::createWithSpriteFrameName(Loading_Progress_Bar));
    pProgressBar->setType(kCCProgressTimerTypeBar);
    pProgressBar->setVisible(true);
    pProgressBar->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
    pProgressBar->setMidpoint(ccp(0, 0));
    pProgressBar->setBarChangeRate(ccp(1, 0));
    pProgressBar->setPercentage(0);
    addChild(pProgressBar, 2, Fight_Sprites_Load_ProgressBar);
    
    //load word
    CCSprite *pLoadWord = CCSprite::createWithSpriteFrameName(Loading_Text);
    pLoadWord->setPosition(ccp(origin.x + visibleSize.width/2,
                               origin.y + visibleSize.height/2 + pLoadWord->getContentSize().height));
    addChild(pLoadWord, 2, Fight_Sprites_Load_Txt);
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    int index = arc4random()%5;
    index++;
    char buf[20] = {0};
    sprintf(buf, "tips_txt%d.png",index);
    CCSprite *tips = CCSprite::createWithSpriteFrameName(buf);
    tips->setPosition(ccp(size.width/2, pProgressFrame->getPositionY() - pProgressFrame->getContentSize().height/2 - tips->getContentSize().height/2 - 10));
    addChild(tips,3,Fight_Sprites_Tips);
    

    CCActionInterval*  actionUp = CCJumpBy::create(2, ccp(0,0), 50, 4);
    pLoadWord->runAction( CCRepeatForever::create(actionUp));
    schedule(schedule_selector(FightLayer::loadingUpdate), 1/60.0);
    
    levelStatistics();
}


void FightLayer::levelStatistics()
{
    if (m_eType == Game_Type_Level) {
        
        int iLevel = (m_nCurrentChapter - 1)*10 + m_nCurrentLevel;
        
        if (iLevel < 0) {
            iLevel = 0;
        }
        
        char temp[10];
        xSocial->statByKey(SLevel, ccitoa(iLevel, temp));
    }
}


#pragma mark -
void FightLayer::loadingUpdate(float dt)
{
    if(m_fLoadPercent>100.0f)
    {
    	CCLog("Loading Complete!");
        unschedule(schedule_selector(FightLayer::loadingUpdate));
        loadingComplateCallBack(NULL);
    }
    else if(10.0==m_fLoadPercent)
    {
        DATAPOOL->addEffectCache(XiaoShiRes);
        DATAPOOL->addEffectCache(ChuxianRes);
        DATAPOOL->addEffectCache(RanShaoRes);
        DATAPOOL->addEffectCache(BaoZhaRes);
        DATAPOOL->addEffectCache(AppleRes);
    }
    else if(20.0==m_fLoadPercent)
    {
        if(m_eType == Game_Type_Endless)
        {
            for (int i = First_Hero_ID+1; i<First_Hero_ID+EnemyTotalNum; i++)
            {
                if(DATAPOOL->findHeroUnlock(i))
                {
                    Game_Hero_Data *_data = DATAPOOL->getHeroData(i);
                    char buf[30] = {0};
                    sprintf(buf, "Sprites/%s.ExportJson",_data->mSourceName);
                    DATAPOOL->addEffectCache(buf);
                }
            }
        }
        else
        {
            for (int i = 0; i<Max_Game_Hero; i++)
            {
                if(m_nHaveSpeHero[i]==0)continue;
                Game_Hero_Data *_data = DATAPOOL->getHeroData(m_nHaveSpeHero[i]);
                char buf[30] = {0};
                sprintf(buf, "Sprites/%s.ExportJson",_data->mSourceName);
                DATAPOOL->addEffectCache(buf);
            }
        }

    }
    else if(30.0==m_fLoadPercent)
    {
        if(m_eType==Game_Type_Level&&m_nCurrentChapter==Charpter_1)
        {
            if(m_nCurrentLevel==4)
            {
                Game_Hero_Data *_data = DATAPOOL->getHeroData(Hero_Type_Lizi);
                char buf[30] = {0};
                sprintf(buf, "Sprites/%s.ExportJson",_data->mSourceName);
                DATAPOOL->addEffectCache(buf);
            }
            else if(m_nCurrentLevel==9)
            {
                Game_Hero_Data *_data = DATAPOOL->getHeroData(Hero_Type_ShiLiu);
                char buf[30] = {0};
                sprintf(buf, "Sprites/%s.ExportJson",_data->mSourceName);
                DATAPOOL->addEffectCache(buf);
            }
        }
    }
    else if(40.0==m_fLoadPercent)
    {
        if(m_eType==Game_Type_PVP||(m_eType!=Game_Type_Boss&&m_nCurrentChapter==Charpter_1))
        {
            DATAPOOL->addEffectCache(GeBuLinRes);
            DATAPOOL->addEffectCache(JuXingGeBuLinRes);
            DATAPOOL->addEffectCache(FengXiYiRes);
            DATAPOOL->addEffectCache(RuanNIGuaiRes);
            DATAPOOL->addEffectCache(ABossRes);
            DATAPOOL->addEffectCache(BBossRes);
        }
    }
    else if(60.0==m_fLoadPercent)
    {
        if(m_eType==Game_Type_PVP||(m_eType!=Game_Type_Boss&&m_nCurrentChapter==Charpter_2))
        {
            DATAPOOL->addEffectCache(GeBuLinRes);
            DATAPOOL->addEffectCache(JuXingGeBuLinRes);
            DATAPOOL->addEffectCache(RuanNIGuaiRes);
            DATAPOOL->addEffectCache(FengXiYiRes);
            
            DATAPOOL->addEffectCache(XueYuGangXieRes);
            DATAPOOL->addEffectCache(ShuiJingZhangyuRes);
            DATAPOOL->addEffectCache(ChangMaoXueGuaiRes);
            DATAPOOL->addEffectCache(BingBaoHeTunRes);
            DATAPOOL->addEffectCache(CBossRes);
            DATAPOOL->addEffectCache(DBossRes);
        }
    }
    else if(70.0==m_fLoadPercent)
    {
        if(m_eType==Game_Type_PVP||(m_eType!=Game_Type_Boss&&m_nCurrentChapter==Charpter_3))
        {
            DATAPOOL->addEffectCache(GeBuLinRes);
            DATAPOOL->addEffectCache(JuXingGeBuLinRes);
            DATAPOOL->addEffectCache(FengXiYiRes);
            
            DATAPOOL->addEffectCache(ShaMoJiaChongRes);
            DATAPOOL->addEffectCache(HuangMoGeBuLinRes);
            DATAPOOL->addEffectCache(ShaMoFeiLongRes);
            DATAPOOL->addEffectCache(ShaBaoJuRenRes);
            DATAPOOL->addEffectCache(EBossRes);
            DATAPOOL->addEffectCache(FBossRes);
        }
    }
    else if(80.0==m_fLoadPercent)
    {
        if(m_eType==Game_Type_PVP||(m_eType!=Game_Type_Boss&&m_nCurrentChapter==Charpter_4))
        {
            DATAPOOL->addEffectCache(GeBuLinRes);
            DATAPOOL->addEffectCache(JuXingGeBuLinRes);
            DATAPOOL->addEffectCache(FengXiYiRes);
            DATAPOOL->addEffectCache(RuanNIGuaiRes);
            DATAPOOL->addEffectCache(ShaMoFeiLongRes);
            
            DATAPOOL->addEffectCache(XiaoGuiRes);
            DATAPOOL->addEffectCache(DiYuMoMengRes);
            DATAPOOL->addEffectCache(DiYuFengQuanRes);
            DATAPOOL->addEffectCache(LingHunShouGeZheRes);
            DATAPOOL->addEffectCache(GBossRes);
            DATAPOOL->addEffectCache(HBossRes);
        }
    }
    else if(90.0==m_fLoadPercent)
    {
        if(m_eType==Game_Type_PVP||(m_eType!=Game_Type_Boss&&m_nCurrentChapter==Charpter_5))
        {
            DATAPOOL->addEffectCache(GeBuLinRes);
            DATAPOOL->addEffectCache(HuangMoGeBuLinRes);
            DATAPOOL->addEffectCache(XiaoGuiRes);
            DATAPOOL->addEffectCache(FengXiYiRes);
            DATAPOOL->addEffectCache(JuXingGeBuLinRes);
            DATAPOOL->addEffectCache(RuanNIGuaiRes);
            DATAPOOL->addEffectCache(ShaMoFeiLongRes);
            DATAPOOL->addEffectCache(ChangMaoXueGuaiRes);
            DATAPOOL->addEffectCache(DiYuFengQuanRes);
            
            DATAPOOL->addEffectCache(JiaoTangKuiLeiRes);
            DATAPOOL->addEffectCache(YouEMoRes);
            DATAPOOL->addEffectCache(TangXinMoRes);
            DATAPOOL->addEffectCache(HeiTangEMoRes);
            DATAPOOL->addEffectCache(IBossRes);
        }

    }

    CCProgressTimer *m_progressBar = (CCProgressTimer *)(getChildByTag(Fight_Sprites_Load_ProgressBar));
    if (m_progressBar)
    {
        float percentage = (m_fLoadPercent > 100.0f)?(100.0f):(m_fLoadPercent);
        m_progressBar->setPercentage(percentage);
    }
    
    m_fLoadPercent += 1.0f;
}

void FightLayer::createWave(float dt)
{
    unschedule(schedule_selector(FightLayer::createWave));
    m_bDelayComplate = false;
    m_eModeInfo.mLogicState = Fight_LogicState_Create;
    schedule(schedule_selector(FightLayer::waveUpdate));
}

void FightLayer::onExit()
{
    //清楚英雄资源
    DATAPOOL->removeEffectCache(AppleRes);
    if(m_eType == Game_Type_Endless)
    {
        for (int i = First_Hero_ID+1; i<First_Hero_ID+HeroTotalNum; i++)
        {
            if(DATAPOOL->findHeroUnlock(i))
            {
                Game_Hero_Data *_data = DATAPOOL->getHeroData(i);
                char buf[30] = {0};
                sprintf(buf, "Sprites/%s.ExportJson",_data->mSourceName);
                DATAPOOL->removeEffectCache(buf);
            }
        }
    }
    else
    {
        for (int i = 0; i<Max_Game_Hero; i++)
        {
            if(m_nHaveSpeHero[i]==0)continue;
            Game_Hero_Data *_data = DATAPOOL->getHeroData(m_nHaveSpeHero[i]);
            char buf[30] = {0};
            sprintf(buf, "Sprites/%s.ExportJson",_data->mSourceName);
            DATAPOOL->removeEffectCache(buf);
        }
    }
    
    if(m_eType==Game_Type_Level&&m_nCurrentChapter==1)
    {
        if(m_nCurrentLevel==4)
        {
            Game_Hero_Data *_data = DATAPOOL->getHeroData(Hero_Type_Lizi);
            char buf[30] = {0};
            sprintf(buf, "Sprites/%s.ExportJson",_data->mSourceName);
            DATAPOOL->removeEffectCache(buf);
        }
        else if(m_nCurrentLevel==9)
        {
            Game_Hero_Data *_data = DATAPOOL->getHeroData(Hero_Type_ShiLiu);
            char buf[30] = {0};
            sprintf(buf, "Sprites/%s.ExportJson",_data->mSourceName);
            DATAPOOL->removeEffectCache(buf);
        }
    }
    
    CC_SAFE_DELETE(m_pLevelInfo);
    
    CCLOG("m_pEnemy retain count:%d",m_pEnemy->retainCount());
    m_pEnemy->removeAllObjects();
    CCLOG("m_pEnemy retain count:%d",m_pEnemy->retainCount());
    CC_SAFE_RELEASE_NULL(m_pEnemy);
    
    HelpMethod::checkRetainCount(m_pBarrier);
    m_pBarrier->removeAllObjects();
    CC_SAFE_RELEASE_NULL(m_pBarrier);
    
    HelpMethod::checkRetainCount(m_pBullet);
    m_pBullet->removeAllObjects();
    CC_SAFE_RELEASE_NULL(m_pBullet);
    
    HelpMethod::checkRetainCount(m_pHero);
    m_pHero->removeAllObjects();
    CC_SAFE_RELEASE_NULL(m_pHero);
    
    HelpMethod::checkRetainCount(m_pInk);
    m_pInk->removeAllObjects();
    CC_SAFE_RELEASE_NULL(m_pInk);
    
    HelpMethod::checkRetainCount(m_pPeel);
    m_pPeel->removeAllObjects();
    CC_SAFE_RELEASE_NULL(m_pPeel);
    
    HelpMethod::checkRetainCount(m_pAward);
    m_pAward->removeAllObjects();
    CC_SAFE_RELEASE_NULL(m_pAward);
    
    HelpMethod::checkRetainCount(m_pShiBei);
    m_pShiBei->removeAllObjects();
    CC_SAFE_RELEASE_NULL(m_pShiBei);
    
    HelpMethod::checkRetainCount(m_pIceWall);
    m_pIceWall->removeAllObjects();
    CC_SAFE_RELEASE_NULL(m_pIceWall);
    
    HelpMethod::checkRetainCount(m_pFog);
    m_pFog->removeAllObjects();
    CC_SAFE_RELEASE_NULL(m_pFog);
    
    HelpMethod::checkRetainCount(m_pMuBei);
    m_pMuBei->removeAllObjects();
    CC_SAFE_RELEASE_NULL(m_pMuBei);
    
    
    
    CC_SAFE_DELETE_ARRAY(m_nHaveSpeHero);
    CC_SAFE_DELETE_ARRAY(m_nUsePropType);
    if(m_pQiangbaoGuai)
    {
        m_pQiangbaoGuai->removeFromParentAndCleanup(true);
        m_pQiangbaoGuai = NULL;
        DATAPOOL->removeEffectCache(QiangBaoGuaiRes);
    }
    
    if(m_pLevelData)
    {
        for (int i = 0; i<m_pLevelData->nWaveCount; i++)
        {
            CCLOG("i:%d",i);
            CC_SAFE_DELETE_ARRAY(m_pLevelData->mArrayWave[i]->mArrayElement);
        }
        CC_SAFE_DELETE_ARRAY(m_pLevelData->mArrayWave);
        CC_SAFE_DELETE(m_pLevelData);
    }
    cleanArmatureDataCache();
    
    CCArmatureDataManager::sharedArmatureDataManager()->getAnimationDatas()->removeAllObjects();
    CCArmatureDataManager::sharedArmatureDataManager()->getArmatureDatas()->removeAllObjects();
    CCArmatureDataManager::sharedArmatureDataManager()->purge();
    
    SceneReader::sharedSceneReader()->purgeSceneReader();
    GUIReader::shareReader()->purgeGUIReader();
	ActionManager::purgeActionManager();
    DATAPOOL->resetHeroReference();
    //启用Busy提示
    xNetEngine->setBusyEnabled(true);
    MsgSystem::Instance()->ClearObjHandler(this);
    CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
    CCDirector::sharedDirector()->purgeCachedData();
    CCLayer::onExit();
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    
    xData->tryPlayBgMuisc(BACKMUSIC_TYPE_HOME);
}

void FightLayer::removeEnemyFromArray(cocos2d::CCObject *object)
{
    if (m_pEnemy)
    {
        m_pEnemy->removeObject(object);
    }
}

void FightLayer::addEnemy(CCObject *object,bool isBoss)
{
    if(m_pEnemy)
    {
        if(isBoss&&!m_bTwoBoss)
        {
            m_bTwoBoss = true;
        }
        m_pEnemy->addObject(object);
    }
}

void FightLayer::setLevel(int chapter, int level)
{
    m_nCurrentChapter = chapter;
    m_nCurrentLevel = level;
}

void FightLayer::setSpeHero(int *hero)
{
    for (int i = 0; i<Max_Game_Hero; ++i)
    {
        CCLOG("%d",hero[i]);
       m_nHaveSpeHero[i] =  hero[i];
    }
}

void FightLayer::setProps(int *props)
{
    for (int i = 0; i<Max_Game_Prop; ++i)
    {
        m_nUsePropType[i] = props[i];
    }
}

void FightLayer::setLansquenet(int *lans)
{
    for (int i = 0; i<Max_Game_Lans; ++i)
    {
        m_nUseLansTypeOne[i] = lans[i];
    }
}

void FightLayer::setLansquenetTwo(int *lans)
{
    for (int i = 0; i<Max_Game_Lans; ++i)
    {
        m_nUseLansTypeTwo[i] = lans[i];
    }
}

void FightLayer::animationEvent(cocos2d::extension::CCArmature *armature, cocos2d::extension::MovementEventType movementType, const char *movementID)
{
    string id = movementID;
    if(movementType == cocos2d::extension::COMPLETE)
    {
        if (id.compare("baozha")==0)
        {
            armature->removeFromParentAndCleanup(true);
        }
        else if(id.compare("du_b")==0)
        {
            armature->removeFromParentAndCleanup(true);
            DATAPOOL->removeEffectCache(DuBRes);
        }
        else if(id.compare("du_a")==0)
        {
            armature->removeFromParentAndCleanup(true);
            DATAPOOL->removeEffectCache(DuARes);
        }
        else if(id.compare("chuxian")==0)
        {
            FightLayer *layer = (FightLayer*)armature->getParent();
            Enemy *enemy = (Enemy*)this;
            if(enemy)
            {
                EnemyMoveState *_enemyMoveState = EnemyMoveState::getInstance();
                enemy->GetFSMSystem()->ChangeState(_enemyMoveState);
                layer->addChild(enemy,Fight_Zorder_Enemy);
                layer->addEnemy(enemy,false);
                if(layer->getGameType() != Game_Type_Boss)
                    layer->setCreateEnemy(false);
            }
            
            armature->removeFromParentAndCleanup(true);
            layer = NULL;
            enemy = NULL;
        }
        else if(id.compare("huohua")==0)
        {
            armature->removeFromParentAndCleanup(true);
            DATAPOOL->removeEffectCache(HuoHuaRes);
        }
        else if(id.compare("bingzha")==0)
        {
            armature->removeFromParentAndCleanup(true);
            DATAPOOL->removeEffectCache(BingZhaRes);
        }
        else if(id.compare("hudun_a")==0)
        {
            armature->removeFromParentAndCleanup(true);
            DATAPOOL->removeEffectCache(HuDunARes);
        }
        else if(id.compare("xiangjiao_a")==0)
        {
            createPeel(armature->getPosition(), 2);
            armature->removeFromParentAndCleanup(true);
            DATAPOOL->removeEffectCache(XiangJiaoARes);
        }
        else if(id.compare("boluobaozha")==0)
        {
            armature->removeFromParentAndCleanup(true);
            DATAPOOL->removeEffectCache(BoLuoBaoZhaRes);
        }
        else if(id.compare("shibei_a")==0)
        {
            m_pShiBei->addObject(armature);
        }
        else if(id.compare("shibei_b")==0)
        {
            armature->removeFromParentAndCleanup(true);
            DATAPOOL->removeEffectCache(ShiBeiRes);
        }
        else if(id.compare("bingzhu_a")==0)
        {
            m_pIceWall->addObject(armature);
        }
        else if(id.compare("bingzhu_b")==0)
        {
            armature->removeFromParentAndCleanup(true);
//            m_pIceWall->removeObject(armature);
            DATAPOOL->removeEffectCache(BingDRes);
        }
        else if(id.compare("luoshi")==0)
        {
            float _y = armature->getPositionY();
            CCSize size = CCDirector::sharedDirector()->getWinSize();
            for (int i=m_pEnemy->count()-1;i>=0;i--)
            {
                Enemy *enemy = (Enemy*)m_pEnemy->objectAtIndex(i);
                if(enemy&&CCRectMake(0, _y - 200, size.width, 300).containsPoint(enemy->getPosition()))
                {
                    EnemyHurtState *_instance = EnemyHurtState::getInstance();
                    enemy->GetFSMSystem()->ChangeState(_instance);
                    int add = m_eType==Game_Type_Boss?xData->getAttackAdd():0;
                    int atk =  DATAPOOL->getEnemyData(Enemy_Type_gBoss)->mAtk2*2 + (DATAPOOL->getEnemyData(Enemy_Type_gBoss)->mAtkAdd)*(DATAPOOL->getEnemyRecord(Enemy_Type_gBoss)->m_nLevel);
                    enemy->minusBlood(atk,false,add);
                }
            }
            
            armature->removeFromParentAndCleanup(true);
            DATAPOOL->removeEffectCache(LuoShiRes);
        }
        else if(id.compare("daoshu")==0)
        {
            armature->removeFromParentAndCleanup(true);
            DATAPOOL->removeEffectCache(DaoShuRes);
            m_bDaoShuComplate = true;
            CCSprite  *spr = CCSprite::createWithSpriteFrameName(GuaiShouChuMo);
            CCSize size = CCDirector::sharedDirector()->getWinSize();
            spr->setPosition(ccp(size.width/2,size.height/2));
            addChild(spr,Fight_Zorder_Top);
            spr->setOpacity(0);
            CCSequence *seq = CCSequence::create(CCFadeIn::create(0.1),
                                                 CCDelayTime::create(1.5),
                                                 CCFadeOut::create(0.1),
                                                 CCCallFuncN::create(this,callfuncN_selector(FightLayer::removeCallBack_Sprite)),
                                                 NULL);
            spr->runAction(seq);
            
            CCMenu *menuPause = (CCMenu*)getChildByTag(Fight_Sprites_Btn_Pause);
            menuPause->setVisible(true);
        }
    }
    else if (movementType == cocos2d::extension::LOOP_COMPLETE)
    {
        if(id.compare("bing_c")==0)
        {
            armature->removeFromParentAndCleanup(true);
            DATAPOOL->removeEffectCache(BingCRes);
        }
        else if(id.compare("huifu")==0)
        {
            armature->removeFromParentAndCleanup(true);
            DATAPOOL->removeEffectCache(HuiFuRes);
        }
        else if(id.compare("xiaoshi_mb")==0)
        {
            armature->removeFromParentAndCleanup(true);
            if(m_pMuBei->count()<=0)
            {
                DATAPOOL->removeEffectCache(MuBeiRes);
            }
        }
    }
}

void FightLayer::playBarrierUnderAtkAni()
{
    int index = Fight_Sprites_Barrier1;
    for (int i = index; i<index+Max_Game_Lans; ++i)
    {
        Lansquenet *_sprite = (Lansquenet*)getChildByTag(i);
        if (_sprite)
        {
            CCAction *_act = _sprite->getSprite()->getActionByTag(1);
            if (!_act||_act->isDone())
            {
                CCActionInterval  *_action1 = CCTintBy::create(0.1f, -155, 0, 0);
                CCActionInterval  *_actionBack = _action1->reverse();
                CCSequence *_seq = CCSequence::create(_action1,_actionBack,NULL);
                _seq->setTag(1);
                _sprite->getSprite()->runAction(_seq);
                
            }
        }
    }
}

void FightLayer::minusBarrierHp(int atk)
{
    if(First_Barrier==m_nCurrentBarrier)
    {
        m_nBarrierCurrentHp-=atk;
        if(m_nBarrierCurrentHp<0)
            m_nBarrierCurrentHp=0;
    }
    else if(Second_Barrier==m_nCurrentBarrier)
    {
        m_nBarrierCurrentHp_Second-=atk;
        if(m_nBarrierCurrentHp_Second<0)
            m_nBarrierCurrentHp_Second=0;
    }
    updateHpBar();
}

void FightLayer::addBarrierHp(int atk)
{
    if(First_Barrier==m_nCurrentBarrier)
    {
        m_nBarrierCurrentHp+=atk;
        if(m_nBarrierCurrentHp>m_nBarrierTotalHp)
            m_nBarrierCurrentHp = m_nBarrierTotalHp;
    }
    else if(Second_Barrier==m_nCurrentBarrier)
    {
        m_nBarrierCurrentHp_Second+=atk;
        if(m_nBarrierCurrentHp_Second>m_nBarrierTotalHp_Second)
            m_nBarrierCurrentHp_Second = m_nBarrierTotalHp_Second;
    }
    
    updateHpBar();
}

void FightLayer::changeGoldNum(int gold)
{
    m_nGainGold+=gold;
    DATAPOOL->changeBagGoldNumForGame(gold);
    CCLabelBMFont *_labelGold = (CCLabelBMFont*)getChildByTag(Fight_Sprites_Gold_Num);
    if (_labelGold)
    {
        char buf[10];
        memset(buf, 0, 10);
        sprintf(buf, "%07d",DATAPOOL->gold);
        _labelGold->setString(buf);
    }
}

void FightLayer::changeGemNum(int gem)
{
    if(gem<0)
    {
        m_nGameUseGem += (-gem);
    }
    else
    {
        m_nGameGainGem += gem;
    }
    
    DATAPOOL->changeBagGemNumForGame(gem);
    CCLabelBMFont *_labelGem = (CCLabelBMFont*)getChildByTag(Fight_Sprites_Gem_Num);
    if (_labelGem)
    {
        char buf[10];
        memset(buf, 0, 10);
        sprintf(buf, "%07d",xUser->pUserInfo->gemstone);
        _labelGem->setString(buf);
    }
}

void FightLayer::addInk(cocos2d::CCObject *object)
{
    if(!DATAPOOL->m_bTeachStepOver[TEACH_STEP_INK1]||
       !DATAPOOL->m_bTeachStepOver[TEACH_STEP_INK2]||
       !DATAPOOL->m_bTeachStepOver[TEACH_STEP_INK3])
    {
        CCArray *animFrames = CCArray::createWithCapacity(2);
        CCSpriteFrame* frame1 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName( teach_shouzhi1 );
        animFrames->addObject(frame1);
        CCSpriteFrame* frame2 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName( teach_shouzhi2 );
        animFrames->addObject(frame2);
        CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames, 0.3f);
        CCSprite *pSprite = CCSprite::createWithSpriteFrameName(teach_shouzhi1);
        CCPoint pos = ((CCNode*)object)->getPosition();
        pSprite->setPosition(ccp(pos.x, pos.y-100));
        addChild(pSprite,Fight_Zorder_Top+1);
        pSprite->runAction(CCRepeatForever::create(CCAnimate::create(animation)));
        CCSequence *seq = CCSequence::create(CCDelayTime::create(1.5),
                                             CCCallFuncN::create(this, callfuncN_selector(FightLayer::removeCallBack_Sprite)),
                                             NULL);
        pSprite->runAction(seq);
        
        if(!DATAPOOL->m_bTeachStepOver[TEACH_STEP_INK1])
        {
            DATAPOOL->m_bTeachStepOver[TEACH_STEP_INK1] = true;
        }
        else if(DATAPOOL->m_bTeachStepOver[TEACH_STEP_INK1]&&!DATAPOOL->m_bTeachStepOver[TEACH_STEP_INK2])
        {
            DATAPOOL->m_bTeachStepOver[TEACH_STEP_INK2] = true;
        }
        else if(DATAPOOL->m_bTeachStepOver[TEACH_STEP_INK2]&&!DATAPOOL->m_bTeachStepOver[TEACH_STEP_INK3])
        {
            DATAPOOL->m_bTeachStepOver[TEACH_STEP_INK3] = true;
        }
    }
    m_pInk->addObject(object);
}

void FightLayer::setGameType(Game_Type type)
{
    m_eType = type;
}

void FightLayer::explosionUpdate(float dt)
{
    if(m_explosionNum.mCurTime<=m_explosionNum.mTotalTime)
    {
        m_explosionNum.mCurTime++;
        float _x = explosionPosX;
        float _y = Barrier_Y+Atk_Barrier_DY+m_explosionNum.mCurTime*90;
        createEffect(BoLuoBaoZhaRes, "boluobaozha", ccp(_x, _y), Fight_Zorder_Bullet);
        if(m_pQiangbaoGuai)
        {
            if(CCRectMake(_x - 200,_y-200, 400,400).containsPoint(m_pQiangbaoGuai->getPosition()))
            {
                if(!m_bHitComb)
                {
                    m_nHitCount++;
                    m_bHitComb = true;
                }
                m_pQiangbaoGuai->stopAllActions();
                m_pQiangbaoGuai->removeFromParentAndCleanup(true);
                m_pQiangbaoGuai = NULL;
                DATAPOOL->removeEffectCache(QiangBaoGuaiRes);
                
                CCSequence *seq = CCSequence::create(CCDelayTime::create(1),
                                                     CCCallFuncN::create(this, callfuncN_selector(FightLayer::showQiangBaoCallBack)),
                                                     NULL);
                this->runAction(seq);
            }
        }
        
        for (int i=m_pEnemy->count()-1; i>=0; i--)
        {
            Enemy *enemy = (Enemy*)m_pEnemy->objectAtIndex(i);
            if(CCRectMake(_x - 200,_y-200, 400,400).containsPoint(enemy->getPosition()))
            {
                if(!m_bHitComb)
                {
                    m_nHitCount++;
                    m_bHitComb = true;
                }
                m_nCombo++;
                updateCombo(enemy->getPosition());
                int add = m_eType==Game_Type_Boss?xData->getAttackAdd():0;
                int atk = DATAPOOL->getHeroData(Hero_Type_BoLuo)->mAtk2 + (DATAPOOL->getHeroData(Hero_Type_BoLuo)->mAtkAdd)*(DATAPOOL->getHeroRecord(Hero_Type_BoLuo)->m_nLevel);
                enemy->minusBlood(atk,false,add);
                EnemyHurtState *_instance = EnemyHurtState::getInstance();
                enemy->GetFSMSystem()->ChangeState(_instance);
            }
        }
        DATAPOOL->playGameEffect(Item_Explode);
    }
    else
    {
        unschedule(schedule_selector(FightLayer::explosionUpdate));
        m_explosionNum.mCurTime = 0;
    }
    
}

void FightLayer::createOneEnemy(int nType, cocos2d::CCPoint pos)
{
    switch (nType) {
        case Enemy_Type_GeBuLin:
        {
            Enemy_Data *_data = DATAPOOL->getEnemyData(Enemy_Type_GeBuLin);
            Enemy *_enemy = new Enemy(_data,this,true);
            _enemy->autorelease();
            _enemy->addPoint(pos);
            addChild(_enemy,Fight_Zorder_Enemy);
            m_pEnemy->addObject(_enemy);
            EnemyMoveState *_instance = EnemyMoveState::getInstance();
            _enemy->GetFSMSystem()->ChangeState(_instance);
        }
            break;
        default:
            break;
    }
}

void FightLayer::createTransfer(Enemy *enemy)
{
    CCArmature *_trans = (CCArmature*)getChildByTag(Fight_Sprites_Trans1);
    if(_trans)
        return;
    DATAPOOL->addEffectCache(HeiDongRes);
    CCArmature *_trans1 = CCArmature::create("heidong");
    _trans1->getAnimation()->play("heidong");
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    float _x = arc4random()%200;
    _x -= 100;
    _x += size.width/4;
    float _y = arc4random()%300;
    _y -= 150;
    _y += size.height/2;
    CCSequence *seq = CCSequence::create(CCDelayTime::create(HOLE_TIME),
                                         CCCallFuncN::create(this, callfuncN_selector(FightLayer::removeArmatureWithClean)),
                                         NULL);

    _trans1->setPosition(ccp(_x, _y));
    addChild(_trans1,Fight_Zorder_Up,Fight_Sprites_Trans1);
    _trans1->runAction(seq);
    
    _x = arc4random()%200;
    _x -=  100;
    _x += size.width - size.width/4;
    
    _y = arc4random()%300;
    _y -= 150;
    _y += size.height/2;
    
    CCArmature *_trans2 = CCArmature::create("heidong");
    _trans2->getAnimation()->play("heidong");
    _trans2->setPosition(ccp(480, _y));
    addChild(_trans2,Fight_Zorder_Up,Fight_Sprites_Trans2);
}

void FightLayer::createShip(Enemy *enemy)
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    int width = size.width/Ship_Num;
    float x = width/2;
    for (int i=0; i<Ship_Num; ++i)
    {
        DATAPOOL->addEffectCache(ChuanRes);
        CCArmature *ship = CCArmature::create("chuan");
        ship->setPosition(ccp(x+width*i, 700));
        ship->getAnimation()->play("chuan_a");
        addChild(ship,Fight_Zorder_Bullet);
        CCMoveTo *moveTo = CCMoveTo::create(2, ccp(ship->getPositionX(), Barrier_Y));
        CCSequence *seq = CCSequence::create(moveTo,
                                             CCCallFuncND::create(this, callfuncND_selector(FightLayer::remoteAtkCallBack_Armature), enemy),
                                             NULL);
        CCSequence *seq1 = CCSequence::create(moveTo,
                                              CCCallFuncN::create(this, callfuncN_selector(FightLayer::removeAtkCallBack_Armature)),
                                              NULL);
        if(i==0)
            ship->runAction(seq);
        else
            ship->runAction(seq1);
    }
}

void FightLayer::createTornado(Enemy *enemy)
{
    DATAPOOL->addEffectCache(LongJuanFengRes);
    m_nTornadoCount++;
    CCArmature *arm = CCArmature::create("longjuanfeng");
    arm->setPosition(ccp(enemy->getPositionX(), enemy->getPositionY() - 20));
    arm->getAnimation()->play("longjuanfeng");
    addChild(arm,Fight_Zorder_Bullet);
    CCMoveTo *moveTo = CCMoveTo::create(1.5f, ccp(arm->getPositionX(), Barrier_Y));
    CCSequence *seq = CCSequence::create(moveTo,
                                         CCCallFuncND::create(this, callfuncND_selector(FightLayer::remoteAtkCallBack_Armature_Tornado), enemy),
                                         NULL);
    arm->runAction(seq);
}

void FightLayer::createIceBerg(Enemy *enemy)
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    createEffect(BingCRes, "bing_c", ccp(size.width/2, Barrier_Y+Atk_Barrier_DY), Fight_Zorder_Enemy);
    minusBarrierHp(enemy->getSkillAtk());
    playBarrierUnderAtkAni();
}

void FightLayer::createLockHero(Enemy *enemy)
{
    int indexHero = Fight_Sprites_Hero1;
    for (int i = indexHero; i<Max_Game_Hero+indexHero; ++i)
    {
        CCNode* _hero = getChildByTag(i);
        if (_hero)
        {
            m_bLockHero = true;
            CCSprite *_bingfeng = CCSprite::createWithSpriteFrameName(BingFengRes);
            _bingfeng->setPosition(_hero->getPosition());
            addChild(_bingfeng,Fight_Zorder_Prop);
            
            CCSequence *seq = CCSequence::create(CCDelayTime::create(5),
                                                 CCCallFuncN::create(this, callfuncN_selector(FightLayer::lockHeroCallBack)),
                                                 NULL);
            _bingfeng->runAction(seq);
        }
    }
}

void FightLayer::createStoneWalling(Enemy *enemy)
{
    DATAPOOL->addEffectCache(ShiBeiRes);
    CCArmature *shibei = CCArmature::create("shibei");
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    float _x = arc4random()%400;
    _x -=200;
    _x +=size.width/2;
    float _y = enemy->getPositionY();
    _y-=arc4random()%50;
    shibei->setPosition(ccp(_x, _y));
    shibei->getAnimation()->play("shibei_a");
    shibei->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(FightLayer::animationEvent));
    addChild(shibei,Fight_Zorder_Enemy);
    
    if(!DATAPOOL->m_bTeachStepOver[TEACH_STEP_SHIBEI1]||
       !DATAPOOL->m_bTeachStepOver[TEACH_STEP_SHIBEI2]||
       !DATAPOOL->m_bTeachStepOver[TEACH_STEP_SHIBEI3])
    {
        CCArray *animFrames = CCArray::createWithCapacity(2);
        CCSpriteFrame* frame1 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName( teach_shouzhi1 );
        animFrames->addObject(frame1);
        CCSpriteFrame* frame2 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName( teach_shouzhi2 );
        animFrames->addObject(frame2);
        CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames, 0.3f);
        CCSprite *pSprite = CCSprite::createWithSpriteFrameName(teach_shouzhi1);
        CCPoint pos = ccp(_x, _y);
        pSprite->setPosition(ccp(pos.x, pos.y-100));
        addChild(pSprite,Fight_Zorder_Top+1);
        pSprite->runAction(CCRepeatForever::create(CCAnimate::create(animation)));
        CCSequence *seq = CCSequence::create(CCDelayTime::create(1.5),
                                             CCCallFuncN::create(this, callfuncN_selector(FightLayer::removeCallBack_Sprite)),
                                             NULL);
        pSprite->runAction(seq);
        
        if(!DATAPOOL->m_bTeachStepOver[TEACH_STEP_SHIBEI1])
        {
            DATAPOOL->m_bTeachStepOver[TEACH_STEP_SHIBEI1] = true;
        }
        else if(DATAPOOL->m_bTeachStepOver[TEACH_STEP_SHIBEI1]&&!DATAPOOL->m_bTeachStepOver[TEACH_STEP_SHIBEI2])
        {
            DATAPOOL->m_bTeachStepOver[TEACH_STEP_SHIBEI2] = true;
        }
        else if(DATAPOOL->m_bTeachStepOver[TEACH_STEP_SHIBEI2]&&!DATAPOOL->m_bTeachStepOver[TEACH_STEP_SHIBEI3])
        {
            DATAPOOL->m_bTeachStepOver[TEACH_STEP_SHIBEI3] = true;
        }
    }
}

void FightLayer::createIceWall(Enemy *enemy)
{
    DATAPOOL->addEffectCache(BingDRes);
    CCArmature *shibei = CCArmature::create("bing_d");
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    float _x = arc4random()%400;
    _x -=200;
    _x +=size.width/2;
    float _y = enemy->getPositionY();
    _y-=arc4random()%50;
    shibei->setPosition(ccp(_x, _y));
    shibei->getAnimation()->play("bingzhu_a");
    shibei->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(FightLayer::animationEvent));
    addChild(shibei,Fight_Zorder_Enemy);
}

void FightLayer::createSelf(Enemy *enemy)
{
    Enemy *boss = new Enemy(enemy->getData(),this,true);
    boss->autorelease();
    boss->setPosition(ccp(enemy->getPositionX()+50, enemy->getPositionY()));
    addChild(boss,Fight_Zorder_Enemy);
    addEnemy(boss, true);
    boss->setUseSkill(true);
    EnemyStayState *_instance = EnemyStayState::getInstance();
    boss->GetFSMSystem()->ChangeState(_instance);
}

void FightLayer::createFog(Enemy *enemy)
{
    DATAPOOL->addEffectCache(ShaBaoRes);
    CCArmature *shabao = CCArmature::create("shabao");
    CCSize size  = CCDirector::sharedDirector()->getWinSize();
    float _x = arc4random()%(int)size.width;
    float _y = arc4random()%(int)(size.height - Barrier_Y);
    shabao->setPosition(ccp(_x, _y+Barrier_Y));
    shabao->getAnimation()->play("shabao");
    addChild(shabao,Fight_Zorder_Up);
    m_pFog->addObject(shabao);
    if(m_pFog->count()>=Max_Fog)
    {
        for (int i=0; i<m_pEnemy->count(); i++)
        {
            Enemy *enemy = (Enemy*)m_pEnemy->objectAtIndex(i);
            if(enemy->getType() == Enemy_Type_iBoss)
            {
                enemy->setUseSkill(true);
            }
        }
    }
}

void FightLayer::createShipLans(Lansquenet *lans)
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    DATAPOOL->addEffectCache(ChuanRes);
    CCArmature *ship = CCArmature::create("chuan");
    ship->setPosition(lans->getPosition());
    ship->getAnimation()->play("chuan_b");
    addChild(ship,Fight_Zorder_Bullet,Fight_Sprites_Ship);
    CCMoveTo *moveTo = CCMoveTo::create(2, ccp(ship->getPositionX(), size.height));
    CCSequence *seq = CCSequence::create(moveTo,
                                         CCCallFuncND::create(this, callfuncND_selector(FightLayer::remoteAtkCallBack_ArmatureLans), lans),
                                         NULL);
    ship->runAction(seq);
}

void FightLayer::createTornadoLans(Lansquenet *lans)
{
    char buf[20]={0};
    int index = arc4random()%3;
    index++;
    sprintf(buf, "yinfu_%d.png",index);
    CCSprite *yinfu = CCSprite::createWithSpriteFrameName(buf);
    yinfu->setPosition(ccp(lans->getPositionX(), lans->getPositionY()+200));
    yinfu->setScale(0.3);
    addChild(yinfu,Fight_Zorder_Bullet,Fight_Sprites_Tornado);
    CCSequence *seq = CCSequence::create(CCSpawn::create(CCScaleTo::create(0.1, 0.4f),CCMoveBy::create(0.1, ccp(9, 9)),CCFadeTo::create(0.1, 220) ,NULL),
                                         CCSpawn::create(CCScaleTo::create(0.1, 0.5f),CCMoveBy::create(0.1, ccp(-5, 17)),CCFadeTo::create(0.1, 200) ,NULL),
                                         CCSpawn::create(CCScaleTo::create(0.1, 0.6f),CCMoveBy::create(0.1, ccp(-7, 12)),CCFadeTo::create(0.1, 180) ,NULL),
                                         CCSpawn::create(CCScaleTo::create(0.1, 0.7f),CCMoveBy::create(0.1, ccp(-8, 14)),CCFadeTo::create(0.1, 150) ,NULL),
                                         CCSpawn::create(CCScaleTo::create(0.1, 0.8f),CCMoveBy::create(0.1, ccp(5, 15)),CCFadeTo::create(0.1, 100) ,NULL),
                                         CCSpawn::create(CCScaleTo::create(0.1, 0.9f),CCMoveBy::create(0.1, ccp(9, 12)),CCFadeTo::create(0.1, 70) ,NULL),
                                         CCSpawn::create(CCScaleTo::create(0.1, 1),CCMoveBy::create(0.1, ccp(10, 18)),CCFadeTo::create(0.1, 20),NULL),
                                         CCSpawn::create(CCScaleTo::create(0.01, 0.3f),CCMoveBy::create(0.01, ccp(-13, -97)),CCFadeTo::create(0.01, 255),NULL),
                                         NULL);
    yinfu->runAction(CCRepeatForever::create(seq));
    float time = YinFuTime + 0.5*DATAPOOL->getEnemyRecord(Enemy_Type_cBoss)->m_nLevel;
    CCSequence *seq2 = CCSequence::create(CCDelayTime::create(time),
                                          CCCallFuncN::create(this, callfuncN_selector(FightLayer::removeCallBack_Sprite)),
                                          NULL);
    yinfu->runAction(seq2);
}

void FightLayer::createIceWallLans(Lansquenet *lans)
{
    CCSprite *ice = NULL;
    ice = (CCSprite*)getChildByTag(Fight_Sprites_Ice);
    if(!ice)
    {
        CCSize size = CCDirector::sharedDirector()->getWinSize();
       ice = CCSprite::createWithSpriteFrameName(BingRes);
        float _x = arc4random()%100;
        _x-=50;
        _x += size.width/2;
        float _y = arc4random()%150;
        _y -= 75;
        _y += size.height/2;
        ice->setPosition(ccp(_x, _y));
        ice->setOpacity(0);
        addChild(ice,Fight_Zorder_BG,Fight_Sprites_Ice);
        float time = 5 + 0.5*DATAPOOL->getEnemyRecord(Enemy_Type_dBoss)->m_nLevel;
        CCSequence *seq = CCSequence::create(CCFadeIn::create(1),
                                             CCDelayTime::create(time),
                                             CCCallFuncN::create(this, callfuncN_selector(FightLayer::removeCallBack_Sprite)),
                                             NULL);
        ice->runAction(seq);
    }
    
}

void FightLayer::createEnemyBack(Lansquenet *lans)
{
    CCSprite *chongjibo = CCSprite::createWithSpriteFrameName(ChongJiBoRes);
    chongjibo->setPosition(ccp(lans->getPositionX(), lans->getPositionY()+100));
    chongjibo->setScaleX(0.56f);
    chongjibo->setScaleY(0.20f);
    addChild(chongjibo,Fight_Zorder_Bullet,Fight_Sprites_ChongJiBo);
    CCSequence *seq = CCSequence::create(CCScaleTo::create(0.4, 1),
                                         CCCallFuncN::create(this, callfuncN_selector(FightLayer::removeCallBack_Sprite)),
                                         NULL);
    chongjibo->runAction(seq);
}

void FightLayer::createHole(Lansquenet *lans)
{
    CCArmature *arm = NULL;
    arm = (CCArmature*)getChildByTag(Fight_Sprites_Hole);
    if(!arm)
    {
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        DATAPOOL->addEffectCache(HeiDongRes);
        arm = CCArmature::create("heidong");
        arm->getAnimation()->play("heidong");
        addChild(arm,Fight_Zorder_BG,Fight_Sprites_Hole);
        float _y = arc4random()%200;
        _y -= 100;
        _y += size.height/2;
        float _x = arc4random()%100;
        _x -= 50;
        _x += size.width/2;
        arm->setPosition(ccp(_x, _y));
        float time = 5 + 0.5*DATAPOOL->getEnemyRecord(Enemy_Type_eBoss)->m_nLevel;
        CCSequence *seq = CCSequence::create(CCFadeIn::create(1),
                                             CCDelayTime::create(time),
                                             CCCallFuncND::create(this, callfuncND_selector(FightLayer::remoteAtkCallBack_ArmatureLans), lans),
                                             NULL);
        arm->runAction(seq);
    }
}

void FightLayer::createFireBall(Lansquenet *lans)
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    DATAPOOL->addEffectCache(HuoQiuRes);
    CCArmature *ship = CCArmature::create("huoqiu");
    ship->setPosition(lans->getPosition());
    ship->getAnimation()->play("huoqiu");
    addChild(ship,Fight_Zorder_Bullet,Fight_Sprites_FireBall);
    CCMoveTo *moveTo = CCMoveTo::create(2, ccp(ship->getPositionX(), size.height));
    CCSequence *seq = CCSequence::create(moveTo,
                                         CCCallFuncND::create(this, callfuncND_selector(FightLayer::remoteAtkCallBack_ArmatureLans), lans),
                                         NULL);
    ship->runAction(seq);
}

void FightLayer::createFireFall(Lansquenet *lans)
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    DATAPOOL->addEffectCache(LuoShiRes);
    CCArmature* arm = CCArmature::create("luoshi");
    
    float _y = arc4random()%200;
    _y += Barrier_Y + Atk_Barrier_DY + 50;
    arm->setPosition(ccp(size.width/2, _y));
    arm->getAnimation()->play("luoshi");
    arm->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(FightLayer::animationEvent));
    addChild(arm,Fight_Zorder_Bullet,Fight_Sprites_FireFall);
}

void FightLayer::enemyDie(bool isboss)
{
    if(!m_bDanGongFire_Teach)
    {
        DATAPOOL->m_bTeachStepOver[TEACH_STEP_USE_DANGONG1] = true;
        m_bDanGongFire_Teach = true;
    }
    if ((m_pLevelData&&m_nCurrentWave>=m_pLevelData->nWaveCount)||isboss)  //当前的波数大于等于总波数
    {
        if(m_eType == Game_Type_Endless)
        {
            m_nCurrentWave=0;
            m_nCurrentLevel = arc4random()%7 + 3;
            m_nCurrentLevel++;
            if(m_pLevelData)
            {
                for (int i = 0; i<m_pLevelData->nWaveCount; i++)
                {
                    CCLOG("i:%d",i);
                    CC_SAFE_DELETE_ARRAY(m_pLevelData->mArrayWave[i]->mArrayElement);
                }
                CC_SAFE_DELETE_ARRAY(m_pLevelData->mArrayWave);
                CC_SAFE_DELETE(m_pLevelData);
            }
        }
        else if((m_eType == Game_Type_Level&&m_pEnemy->count()<=0&&(!m_bcreateEnemy&&m_bCompleteWave))||(isboss&&m_eType == Game_Type_Level))   //游戏胜利
        {
            if(isboss&&m_bTwoBoss)
            {
                m_bTwoBoss = false;
                return;
            }
            m_bCanPullCatapult = false;
            bTouchedTray = false;
            CCSequence *seq = CCSequence::create(CCDelayTime::create(ShowResultDelay),
                                                 CCCallFuncN::create(this, callfuncN_selector(FightLayer::showWinResultCallBack)),
                                                 NULL);
            this->runAction(seq);
        }
        else if(m_eType == Game_Type_PVP||(isboss&&m_eType == Game_Type_PVP))
        {
            if(isboss&&m_bTwoBoss)
            {
                m_bTwoBoss = false;
                return;
            }
            if((m_LansIndex+1)>=UserData::Instance()->enemy_mercemaryNum)  //数据已经刷完
            {
                if((m_pEnemy->count()<=0&&m_bLastWave&&!m_bcreateEnemy)||isboss)
                {
                    
                    m_bCanPullCatapult = false;
                    bTouchedTray = false;
                    CCSequence *seq = CCSequence::create(CCDelayTime::create(ShowResultDelay),
                                                         CCCallFuncN::create(this, callfuncN_selector(FightLayer::showWinResultCallBack)),
                                                         NULL);
                    this->runAction(seq);
                }
            }
            else
            {
                m_nCurrentWave = 0;
                m_LansIndex++;
                if(m_pLevelData)
                {
                    for (int i = 0; i<m_pLevelData->nWaveCount; i++)
                    {
                        CCLOG("i:%d",i);
                        CC_SAFE_DELETE_ARRAY(m_pLevelData->mArrayWave[i]->mArrayElement);
                    }
                    CC_SAFE_DELETE_ARRAY(m_pLevelData->mArrayWave);
                    CC_SAFE_DELETE(m_pLevelData);
                }
            }
        }
    }
    
    if(m_eType == Game_Type_Boss&&m_nCurrentWave>0&&isboss)
    {
        if(isboss&&m_bTwoBoss)
        {
            m_bTwoBoss = false;
            return;
        }
        bTouchedTray = false;
        m_bCanPullCatapult = false;
//        m_fPower = 60;
//        resetTrayPos();
        CCSequence *seq = CCSequence::create(CCDelayTime::create(ShowResultDelay),
                                             CCCallFuncN::create(this, callfuncN_selector(FightLayer::showWinResultCallBack)),
                                             NULL);
        this->runAction(seq);
    }
    
    
}

void FightLayer::awardGold(cocos2d::CCPoint pos)
{
    int id = DATAPOOL->getEnemyAwardID();
    CCArmature *award = NULL;
    switch (id)
    {
        case 0:
            break;
        case 8001: //金币id
        {
            DATAPOOL->addEffectCache(JinBiRes);
            award = CCArmature::create("jinbi");
            award->getAnimation()->play("jinbi");
            award->setTag(Award_Tag_Gold);
        }
            break;
        case 8002:  //钻石
        {
            if(m_eType != Game_Type_PVP)
                return;
            DATAPOOL->addEffectCache(ZuanShiRes);
            award = CCArmature::create("zuanshi");
            award->getAnimation()->play("zuanshi");
            award->setTag(Award_Tag_Gem);
        }
            break;
        case Prop_Type_ZhaDan:
        {
            DATAPOOL->addEffectCache(LiPinRes);
            award = CCArmature::create("lipin");
            award->getAnimation()->play("lipin_a");
            award->setTag(Award_Tag_ZhaDan);
        }
            break;
        case Prop_Type_BingJing:
        {
            DATAPOOL->addEffectCache(LiPinRes);
            award = CCArmature::create("lipin");
            award->getAnimation()->play("lipin_a");
            award->setTag(Award_Tag_BingJing);
        }
            break;
        case Prop_Type_LianNu:
        {
            DATAPOOL->addEffectCache(LiPinRes);
            award = CCArmature::create("lipin");
            award->getAnimation()->play("lipin_a");
            award->setTag(Award_Tag_LianNu);
        }
            break;
    }
    
    if(award!=NULL)
    {
        award->setPosition(pos);
        addChild(award,Fight_Zorder_Up);
        CCSequence *seq = CCSequence::create(CCDelayTime::create(Award_Time),
                                             CCCallFuncN::create(this, callfuncN_selector(FightLayer::removeAwardCallBack)),
                                             NULL);
        award->runAction(seq);
        m_pAward->addObject(award);
    }
}

bool FightLayer::isDisableQuitBtn()
{
    if(m_eType!=Game_Type_Level)return false;
    
    if(m_nCurrentChapter==1&&m_nCurrentLevel==1&&!(DATAPOOL->m_bCompleteTeachLevelOne))
    {
        return true;
    }
    
    if(m_nCurrentChapter==1&&m_nCurrentLevel==2&&!(DATAPOOL->m_bCompleteTeachLevelTwo))
    {
        return true;
    }
    return false;
}

bool FightLayer::isMaxInk()
{
    if(m_pInk->count()>=MAX_INK)
        return true;
    return false;
}

bool FightLayer::isMaxShiBei()
{
    if(m_pShiBei->count()>=Max_ShiBei)
        return  true;
    return false;
}

void FightLayer::cancelKuangBao()
{
    if(m_bPartner&&!m_bEnergy)
    {
        m_bUseKuangBao = false;
    }
}

void FightLayer::addMuBei(CCPoint pos)
{
    m_nMuBeiCount++;
    if(m_pMuBei->count()<=0)
    {
        DATAPOOL->addEffectCache(MuBeiRes);
    }
    CCArmature *mubei = CCArmature::create("mubei");
    mubei->getAnimation()->play("chuxian");
    mubei->setPosition(pos);
    addChild(mubei,Fight_Zorder_Enemy);
    m_pMuBei->addObject(mubei);
}

bool FightLayer::isMaxMuBei()
{
    bool res = false;
    if(m_nMuBeiCount>=MAX_MUBEI)
    {
        res = true;
    }
    
    return res;
}

#pragma mark - public ccTouches
void FightLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    if(!m_bIsComplateLoading)return;
    if(!m_bDaoShuComplate)return;
    CTeachLayer *teachLayer = NULL;
    teachLayer = (CTeachLayer*)getChildByTag(Fight_Sprites_TeachLayer);
    if(teachLayer)
        return;
    CCSetIterator it = pTouches->begin();
    cocos2d::CCTouch* touch = (cocos2d::CCTouch*)(*it);
    CCPoint location = touch->getLocation();
    touchInk(location);
    
    touchShiBei(location);
    touchFog(location);
    touchAward(location);
    
    if(m_bIsUseLianNu&&location.y>Barrier_Y)   //连弩
    {
        rotateLianNu(location);
        fireBulletLianNu(m_fAngle);
        schedule(schedule_selector(FightLayer::beganLianNuTime), 0.3);
    }
    
    selectHero(location);
    usePropBegan(location);
}

void FightLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    if(!m_bIsComplateLoading)return;
    if(!m_bDaoShuComplate)return;
    CTeachLayer *teachLayer = NULL;
    teachLayer = (CTeachLayer*)getChildByTag(Fight_Sprites_TeachLayer);
    if(teachLayer)
        return;
    CCSetIterator it = pTouches->begin();
    CCTouch* touch = (CCTouch*)(*it);
    CCPoint location = touch->getLocation();
    CCSprite *zhadan = (CCSprite*)getChildByTag(Fight_Sprites_ZhaDan);
    CCSprite *bingjing = (CCSprite*)getChildByTag(Fight_Sprites_BingJing);
    
    if(m_bIsUseLianNu&&location.y>Barrier_Y)
    {
        rotateLianNu(location);
    }
    
    if (bTouchedTray&&m_bCanPullCatapult)
    {
        if (zhadan==NULL&&bingjing==NULL)
        {
            pullCatapult(location);
        }
        
    }
    else
    {
        if(zhadan==NULL&&bingjing==NULL)
        {
         touchCatapult(location);
        }
    }
    
    if (zhadan)
    {
        zhadan->setPosition(ccp(location.x, location.y+100));
        CCLayerColor *layer = (CCLayerColor*)getChildByTag(Fight_Sprites_ZhaDan_Area);
        layer->setPosition(ccp(location.x-layer->getContentSize().width/2, location.y-layer->getContentSize().height/2));
    }
    else if(bingjing)
    {
        bingjing->setPosition(ccp(location.x, location.y+100));
        CCLayerColor *layer = (CCLayerColor*)getChildByTag(Fight_Sprites_ZhaDan_Area);
        layer->setPosition(ccp(location.x-layer->getContentSize().width/2, location.y-layer->getContentSize().height/2));
    }
    else
    {
        if(bTouchedTray)return;
        for (int i = m_pInk->count()-1; i>=0; i--)      //墨水
        {
            CCSprite *ink = (CCSprite*)m_pInk->objectAtIndex(i);
            if (ink->boundingBox().containsPoint(location))
            {
                ink->removeFromParentAndCleanup(true);
                m_pInk->removeObject(ink);
                break;
            }
        }
    }
    
}
void FightLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
    if(!m_bIsComplateLoading)return;
    if(!m_bDaoShuComplate)return;
    CTeachLayer *teachLayer = NULL;
    teachLayer = (CTeachLayer*)getChildByTag(Fight_Sprites_TeachLayer);
    if(teachLayer)
        return;
    CCSetIterator it = pTouches->begin();
    CCTouch* touch = (CCTouch*)(*it);
    CCPoint location = touch->getLocation();
    
    touchEnd(location);
}
void FightLayer::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
    if(!m_bIsComplateLoading)return;
    if(!m_bDaoShuComplate)return;
    CTeachLayer *teachLayer = NULL;
    teachLayer = (CTeachLayer*)getChildByTag(Fight_Sprites_TeachLayer);
    if(teachLayer)
        return;
    CCSetIterator it = pTouches->begin();
    CCTouch* touch = (CCTouch*)(*it);
    CCPoint location = touch->getLocation();
    
    touchEnd(location);
}
#pragma mark - private msg
bool FightLayer::handleMsg(const Message &msg)
{
    switch (msg.m_nMsgType) {
        default:
            break;
    }
    
    return true;
}

#pragma mark - private init
bool FightLayer::initLevelResInfo()
{
    if(m_pLevelInfo!=NULL)
    {
        CC_SAFE_DELETE(m_pLevelInfo);
        m_pLevelInfo = NULL;
    }
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCLog("getLevelInfo!");
    m_pLevelInfo = CfgDataManage::getInstance()->getLevelInfo(m_nCurrentChapter,m_nCurrentLevel);
    CCLog("getLevelInfoComplete!");
    bTouchedTray = false;
    m_fAngle = 0.0f;
    m_fCatapult_Org_X = CCDirector::sharedDirector()->getWinSize().width/2;
    m_fCatapult_Org_Y = 200.0f;
    m_fBullet_Org_Y = m_fCatapult_Org_Y - 30;
    
    m_nMaxOffset_x = Max_Length * cosf(Min_Angle * (PI/180));
    
    char buf[30] = {0};
    sprintf(buf, "GameBg/%s",m_pLevelInfo->cBackgroudName);
    CCSprite *bg = CCSprite::create(buf);
    bg->setPosition(ccp(size.width/2, size.height/2));
    addChild(bg,Fight_Zorder_BG,Fight_Sprites_BG);
    
    m_nTime = 0;
    if(m_eType == Game_Type_Endless || m_eType == Game_Type_Boss)
    {
    
        this->schedule(schedule_selector(FightLayer::timeUpdate), 1.0);
        
        CCSprite *_sprite = CCSprite::createWithSpriteFrameName(Icon_Time);
        _sprite->setPosition(ccp(size.width/2, size.height - _sprite->getContentSize().height/2 - 2));
        addChild(_sprite,Fight_Zorder_Top);
        
        if(m_eType==Game_Type_Boss)
        {
            m_nTime = 180;
        }
        
        memset(buf, 0, 30);
        sprintf(buf, "%02d:%02d",m_nTime/60, m_nTime%60);
        CCLabelBMFont *_labelTime = NULL;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        _labelTime = CCLabelBMFont::create(buf,"num_time.fnt");
#else
        _labelTime = CCLabelBMFont::create(buf,"fonts/num_time.fnt");
#endif
        _labelTime->setPosition(ccp(_sprite->getPositionX(), _sprite->getPositionY()-5));
        addChild(_labelTime,Fight_Zorder_Top,Fight_Sprites_Time);
    
    
        if(m_eType == Game_Type_Endless)
        {
            m_nBestTime = DATAPOOL->bestTime;
            
            CCSprite *_sprite1 = CCSprite::createWithSpriteFrameName(Best_Record);
            _sprite1->setPosition(ccp(_sprite1->getContentSize().width/2 + 2, size.height-_sprite1->getContentSize().height/2 - 2));
            addChild(_sprite1,Fight_Zorder_Top);
            
            memset(buf, 0, 30);
            sprintf(buf, "%02d:%02d",(int)m_nBestTime/60, (int)m_nBestTime%60);
            CCLabelBMFont *_labelBestTime = NULL;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
            _labelBestTime = CCLabelBMFont::create(buf,"num.fnt");
#else
            _labelBestTime = CCLabelBMFont::create(buf,"fonts/num.fnt");
#endif
            _labelBestTime->setPosition(ccp(_sprite1->getPositionX()+_sprite1->getContentSize().width/2 +40 ,_sprite1->getPositionY()+8));
            _labelBestTime->setScale(0.2f);
            addChild(_labelBestTime,Fight_Zorder_Top,Fight_Sprites_BestTime);
        }
    }
    
    return true;
}

bool FightLayer::initPublicInfo()
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();

    CCMenuItemSprite *itemPause = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(Game_Btn_Pause),
                                                         CCSprite::createWithSpriteFrameName(Game_Btn_Pause_Clicked),
                                                         this, menu_selector(FightLayer::menuPauseCallBack));
    CCMenu *menuPause = CCMenu::createWithItem(itemPause);
    menuPause->setPosition(ccp(size.width - itemPause->getContentSize().width/2, itemPause->getContentSize().height/2));
    
    addChild(menuPause,Fight_Zorder_Up,Fight_Sprites_Btn_Pause);
    menuPause->setVisible(false);
    
    CCSprite *_goldIcon = CCSprite::createWithSpriteFrameName(Icon_Gold_Small);
    CCSprite *_gemIcon = CCSprite::createWithSpriteFrameName(Icon_Gem_Small);
    float _dx = 2.0f;
    float _x =  size.width - 170;
    float _y =  size.height - _goldIcon->getContentSize().height/2 - _dx;
    _goldIcon->setPosition(ccp(_x, _y));
    addChild(_goldIcon,Fight_Zorder_Top,Fight_Sprites_Gold);
    
    _x = size.width - 170;
    _y = _y -_goldIcon->getContentSize().height/2 - _gemIcon->getContentSize().height/2 - _dx;
    _gemIcon->setPosition(ccp(_x, _y));
    addChild(_gemIcon,Fight_Zorder_Top,Fight_Sprites_Gem);
    
    m_nGold = DATAPOOL->gold;
    m_nGem = xUser->pUserInfo->gemstone;
    char buf[20];
    memset(buf, 0, 20);
    sprintf(buf, "%07d",m_nGold);
    CCLabelBMFont *_labelGold = NULL;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    _labelGold = CCLabelBMFont::create(buf, "num1.fnt");
#else
    _labelGold = CCLabelBMFont::create(buf, "fonts/num1.fnt");
#endif
    _labelGold->setScale(0.3f);
    memset(buf, 0, 20);
    sprintf(buf, "%07d",m_nGem);
    CCLabelBMFont *_labelGem = NULL;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    _labelGem = CCLabelBMFont::create(buf, "num1.fnt");
#else
    _labelGem = CCLabelBMFont::create(buf, "fonts/num1.fnt");
#endif
    _labelGem->setScale(0.3f);
    
    _dx = 90.0f;
    float _dy = 10;
    _x = _goldIcon->getPositionX() + _dx;
    _y = _goldIcon->getPositionY() + _dy;
    _labelGold->setPosition(ccp(_x, _y));
    
    
    _x = _gemIcon->getPositionX() + _dx;
    _y = _gemIcon->getPositionY() + _dy;
    _labelGem->setPosition(ccp(_x, _y));
    addChild(_labelGold,Fight_Zorder_Top,Fight_Sprites_Gold_Num);
    addChild(_labelGem,Fight_Zorder_Top,Fight_Sprites_Gem_Num);
    
    
    //energy边框
    CCSprite *pProgressFrame = CCSprite::createWithSpriteFrameName(NuQi_Frame);
    pProgressFrame->setPosition(ccp(size.width - pProgressFrame->getContentSize().width/2 - 20, _labelGem->getPositionY() - pProgressFrame->getContentSize().height/2 - 50));
    addChild(pProgressFrame, 1,Fight_Sprites_Energy_Frame);
    
    //energy条
    CCProgressTimer *pProgressBar = CCProgressTimer::create(CCSprite::createWithSpriteFrameName(NuQi_Bar));
    pProgressBar->setType(kCCProgressTimerTypeBar);
    pProgressBar->setVisible(true);
    pProgressBar->setPosition(pProgressFrame->getPosition());
    pProgressBar->setMidpoint(ccp(0, 0));
    pProgressBar->setBarChangeRate(ccp(0, 1));
    pProgressBar->setPercentage(0);
    addChild(pProgressBar, 2, Fight_Sprites_Energy_Bar);
    m_nEnergy = DATAPOOL->energy;
    pProgressBar->setPercentage(( m_nEnergy/(float)Max_Energy) *100 );
    
    if(m_eType==Game_Type_Level)
    {
        if(m_nCurrentChapter==Charpter_2)
        {
            CCParticleSystem *emitter = CCParticleSnow::create();
            emitter->setGravity(ccp(0,-10));
            emitter->setSpeed(120);
            emitter->setSpeedVar(30);
            ccColor4F startColor = emitter->getStartColor();
            startColor.r = 0.9f;
            startColor.g = 0.9f;
            startColor.b = 0.9f;
            emitter->setStartColor(startColor);
            ccColor4F startColorVar = emitter->getStartColorVar();
            startColorVar.b = 0.1f;
            emitter->setStartColorVar(startColorVar);
            emitter->setEmissionRate(emitter->getTotalParticles()/emitter->getLife());
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
            emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage("snow.png"));
#else
            emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage("particle/snow.png"));
#endif
            emitter->setPosition(ccp(size.width/2,size.height));
            emitter->setAutoRemoveOnFinish(true);
            addChild(emitter,Fight_Zorder_Top+1);
        }
        else if(m_nCurrentChapter==Charpter_4)
        {
            CCParticleSystem *emitter = CCParticleSnow::create();
            emitter->setGravity(ccp(0,-10));
            emitter->setSpeed(120);
            emitter->setSpeedVar(30);
            ccColor4F startColor = emitter->getStartColor();
            startColor.r = 0.9f;
            startColor.g = 0.5f;
            startColor.b = 0.7f;
            emitter->setStartColor(startColor);
            ccColor4F startColorVar = emitter->getStartColorVar();
            startColorVar.b = 0.1f;
            emitter->setStartColorVar(startColorVar);
            emitter->setEmissionRate(emitter->getTotalParticles()/emitter->getLife());
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
            emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage("snow.png"));
#else
            emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage("particle/snow.png"));
#endif
            emitter->setPosition(ccp(size.width/2,size.height));
            emitter->setAutoRemoveOnFinish(true);
            addChild(emitter,Fight_Zorder_Top+1);
        }
        
        
        CCSprite *_progress_frame = CCSprite::createWithSpriteFrameName(Progress_Frame);
        _progress_frame->setPosition(ccp(_progress_frame->getContentSize().width/2+35, size.height - _progress_frame->getContentSize().height/2 - 25));
        addChild(_progress_frame,Fight_Zorder_Top);
        
        CCProgressTimer *_progress_bar = CCProgressTimer::create(CCSprite::createWithSpriteFrameName(Progress_Bar));
        _progress_bar->setType(kCCProgressTimerTypeBar);
        _progress_bar->setMidpoint(ccp(0, 0));
        _progress_bar->setBarChangeRate(ccp(1, 0));
        _progress_bar->setPosition(_progress_frame->getPosition());
        _progress_bar->setPercentage(m_fGamePercent*100);
        addChild(_progress_bar,Fight_Zorder_Top,Fight_Sprites_Progress);
        
        CCSprite *_progress_icon = CCSprite::createWithSpriteFrameName(Progress_Icon);
        _x = _progress_bar->getPositionX() - _progress_bar->getContentSize().width/2;
        _x += _progress_bar->getContentSize().width * _progress_bar->getPercentage()/100;
        _progress_icon->setPosition(ccp(_x, _progress_bar->getPositionY()));
        addChild(_progress_icon,Fight_Zorder_Top,Fight_Sprites_Progress_Icon);
    }
    else  if(m_eType == Game_Type_PVP)
    {
        CCSprite *enemyIconFrame = CCSprite::createWithSpriteFrameName("enemyicon_frame.png");
        enemyIconFrame->setPosition(ccp(enemyIconFrame->getContentSize().width/2 + 5,size.height - enemyIconFrame->getContentSize().height/2 - 15));
        addChild(enemyIconFrame,Fight_Zorder_Top);
        
        char buf[20] = {0};
        int index = xData->getPvPId();
        sprintf(buf, "enemyicon%d.png",index);
        CCSprite *enemyIcon = CCSprite::createWithSpriteFrameName(buf);
        enemyIcon->setPosition(enemyIconFrame->getPosition());
        addChild(enemyIcon, Fight_Zorder_Top,Fight_Sprites_EnemyIcon);
        
        CCLabelTTF *labelTTF = CCLabelTTF::create("Name", "Arial", 40);
        memset(buf, 0, 20);
        sprintf(buf, "%s",UserData::Instance()->enemy_userName.c_str());
        CCLOG("%s",UserData::Instance()->enemy_userName.c_str());
        CCLOG("%d",UserData::Instance()->enemy_userId);
        labelTTF->setString(UserData::Instance()->enemy_userName.c_str());
        labelTTF->setPosition(ccp(enemyIcon->getPositionX()+enemyIcon->getContentSize().width/2+40,enemyIcon->getPositionY()));
        labelTTF->setAnchorPoint(ccp(0, 0.5));
        labelTTF->setColor(ccc3(255, 0, 0));
        addChild(labelTTF,Fight_Zorder_Top,Fight_Sprites_EnemyName);
    }
    return true;
}

bool FightLayer::initHeroInfo()
{
    if(m_eType == Game_Type_Endless)
    {
        for (int i = 0; i< 3; ++i)
        {
            int num = 1;
            int index = 1;
            do
            {
                num = arc4random()%21+1;
                if(num>11)  //提高苹果出现的概率
                    num-=10;
                else
                    num = 1;
                index = num;
                index+=(First_Hero_ID-1);
            }
            while (!(DATAPOOL->findHeroUnlock(index)));
            Game_Hero_Data *_data = DATAPOOL->getHeroData(index);
            Hero *_hero = new Hero(_data);
            _hero->autorelease();
            int _dy;
            if(i==1)
            {
                _dy = -50;
            }
            else
            {
                _dy = -100;
            }
            _hero->setPosition(ccp(50 + 100*i, m_fCatapult_Org_Y + _dy));
            addChild(_hero,Fight_Zorder_Hero);
            m_pHero->addObject(_hero);
            _data = NULL;
        }
        createNewBullet();
        return true;
    }
    
    for (int i = 0; i< 3; ++i)
    {
        Game_Hero_Data *_data = DATAPOOL->getHeroData(First_Hero_ID);
        Hero *_hero = new Hero(_data);
        _hero->autorelease();
        int _dy;
        if(i==1)
        {
            _dy = -50;
        }
        else
        {
            _dy = -100;
        }
        _hero->setPosition(ccp(50 + 100*i, m_fCatapult_Org_Y + _dy));
        addChild(_hero,Fight_Zorder_Hero);
        m_pHero->addObject(_hero);
        _data = NULL;
    }
    
    for (int j = 0; j<Max_Game_Hero; ++j)
    {
        int tag = Fight_Sprites_Hero1 + j;
        if(m_nHaveSpeHero[j]==0)continue;
        Game_Hero_Data *_data = DATAPOOL->getHeroData(m_nHaveSpeHero[j]);
        Hero *_hero = new Hero(_data);
        _hero->autorelease();
        int _dy;
        if(j==1)
        {
            _dy = -50;
        }
        else
        {
            _dy = -100;
        }
        _hero->setPosition(ccp(m_fCatapult_Org_X+ 80 +100*j, m_fCatapult_Org_Y + _dy));
        addChild(_hero,Fight_Zorder_Hero,tag);
        _data = NULL;
    }
    
    createNewBullet();
    
    return true;
}

bool FightLayer::initPropInfo()
{
    CCSprite *_prop1 = CCSprite::createWithSpriteFrameName(Prop_Panzi);
    CCSprite *_prop2 = CCSprite::createWithSpriteFrameName(Prop_Panzi);
    CCSprite *_prop3 = CCSprite::createWithSpriteFrameName(Prop_Panzi_Random);
    
    float _dx = 5.0f;
    float _x =  _prop1->getContentSize().width/2 + _dx;
    float _y =  _prop1->getContentSize().height/2 + _dx;
    _prop1->setPosition(ccp(_x, _y));
    _prop2->setPosition(ccp(_x + _prop1->getContentSize().width + _dx, _y));
    _prop3->setPosition(ccp(_prop2->getPositionX() + _prop1->getContentSize().width + _dx, _y));
    addChild(_prop1,Fight_Zorder_Hero,Fight_Sprites_Prop_Tray1);
    addChild(_prop2,Fight_Zorder_Hero,Fight_Sprites_Prop_Tray2);
    addChild(_prop3,Fight_Zorder_Hero,Fight_Sprites_Prop_Tray3);
    
    int index = Fight_Sprites_Prop_Tray1;
    int index_prop = Fight_Sprites_Prop1;
    int index_prop_num = Fight_Sprites_PropNum1;
    for (int i=0; i<Max_Game_Prop-1; ++i)
    {
        CCLOG("%d",m_nUsePropType[i]);
        if(m_nUsePropType[i]!=0)
        {
            CCSprite *_prop  = NULL;
            if(UNLOCKTAG==m_nUsePropType[i])
            {
                _prop = CCSprite::createWithSpriteFrameName(LockRes);
            }
            else
            {
                Prop_Data *_data = DATAPOOL->getPropData(m_nUsePropType[i]);
                _prop = CCSprite::createWithSpriteFrameName(_data->m_sourceName);
                CC_SAFE_DELETE(_data);
                char buf[10];
                memset(buf, 0, 10);
                sprintf(buf, "X%d",DATAPOOL->getOutfitRecord(m_nUsePropType[i])->m_nNum);
                CCLabelBMFont *numProp = NULL;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
                numProp = CCLabelBMFont::create(buf, "num_yellow.fnt");
#else
                numProp = CCLabelBMFont::create(buf, "fonts/num_yellow.fnt");
#endif
                numProp->setPosition(ccp(getChildByTag(index)->getPositionX(), getChildByTag(index)->getPositionY()-10));
                addChild(numProp,Fight_Zorder_Catapult,index_prop_num);
            }
            _prop->setAnchorPoint(ccp(0.5, 0));
            _prop->setPosition(getChildByTag(index)->getPosition());
            addChild(_prop,Fight_Zorder_Prop,index_prop);
        }
        index++;
        index_prop++;
        index_prop_num++;
    }
    
    CCSprite *_prop = CCSprite::createWithSpriteFrameName(Icon_Prop_Random);
    if(xUser->pUserInfo->gemstone<=0)
    {
        _prop->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(Icon_Prop_Random2));
    }
    _prop->setAnchorPoint(ccp(0.5, 0));
    _prop->setPosition(ccp(getChildByTag(Fight_Sprites_Prop_Tray3)->getPositionX(),getChildByTag(Fight_Sprites_Prop_Tray3)->getPositionY()-20));
    addChild(_prop,Fight_Zorder_Prop,index_prop);
    
    CCSprite *pFrame =  NULL;
    CCSprite *pTray =  NULL;
    
    if(DATAPOOL->isHaveTieDanGong())
    {
        pFrame = CCSprite::createWithSpriteFrameName(CatapultFrameTie);
        pTray = CCSprite::createWithSpriteFrameName(CatapultTray);
        m_nXuLiDeltaTime = Power_Time_Prop;
    }
    else
    {
        pFrame = CCSprite::createWithSpriteFrameName(CatapultFrame);
        pTray = CCSprite::createWithSpriteFrameName(CatapultTray);
    }
    
    if (!pFrame||!pTray) {
        return false;
    }
    pFrame->setAnchorPoint(ccp(0.5f, 0.5f));
    pFrame->setPosition(ccp(m_fCatapult_Org_X, m_fCatapult_Org_Y));
    
    pTray->setAnchorPoint(ccp(0.5f, 0.5f));
    pTray->setPosition(ccp(m_fCatapult_Org_X, m_fCatapult_Org_Y - 20));
    m_nTrayWidth = pTray->getContentSize().width;
    addChild(pFrame,Fight_Zorder_Catapult,Fight_Sprites_Catapult_Frame);
    addChild(pTray,Fight_Zorder_Down,Fight_Sprites_Catapult_Tray);
    return true;
}

bool FightLayer::initLansInfo()
{
    int index = Fight_Sprites_Barrier1;
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    int width = size.width/Max_Game_Lans;
    m_nCurrentBarrier = First_Barrier;
    for (int i = 0; i<Max_Game_Lans; ++i)
    {
        if(m_nUseLansTypeOne[i]!=0)  //使用雇佣兵
        {
            Enemy_Data *data = DATAPOOL->getEnemyData(m_nUseLansTypeOne[i]);
            char buf[30] = {0};
            int level = DATAPOOL->getEnemyRecord(m_nUseLansTypeOne[i])->m_nLevel;
            sprintf(buf, "Sprites/%s_g.ExportJson",data->mSourceName);
            DATAPOOL->addEffectCache(buf);
            Lansquenet *lans = new Lansquenet(data,this,level);
            lans->autorelease();
            lans->setPosition(ccp(width/2+width*i, Barrier_Y));
            addChild(lans,Fight_Zorder_Barrier,index);
            m_nBarrierTotalHp += data->mHp;
            data = NULL;
            m_pBarrier->addObject(lans);
        }
        else   //未使用 使用普通栅栏
        {
            int id = DATAPOOL->getZhanLanFromBag();
            CCLOG("id:%d",id);
            Lansquenet *lans = NULL;
            if(id==0)
            {
                lans = new Lansquenet(ZhaLanRes,this);
                lans->autorelease();
                m_nBarrierTotalHp += Barrier_HP;
            }
            else  //买的栅栏道具
            {
                if(id == Prop_Type_ShiZhaLan)
                {
                    lans = new Lansquenet(ShiZhaLanRes,this);
                    lans->autorelease();
                }
                else   //铁栅栏
                {
                    lans = new Lansquenet(TieZhaLanRes,this);
                    lans->autorelease();
                }
                CCLOG("barrierHP:%d",DATAPOOL->getPropData(id)->mHp);
                m_nBarrierTotalHp += DATAPOOL->getPropData(id)->mHp;
            }
            lans->setPosition(ccp(width/2+width*i, Barrier_Y));
            addChild(lans,Fight_Zorder_Barrier,index);
            
            m_pBarrier->addObject(lans);
        }
        
        if(m_nUseLansTypeTwo[i]!=0)  //使用雇佣兵
        {
            Enemy_Data *data = DATAPOOL->getEnemyData(m_nUseLansTypeTwo[i]);
            char buf[30] = {0};
            sprintf(buf, "Sprites/%s_g.ExportJson",data->mSourceName);
            DATAPOOL->addEffectCache(buf);
            data = NULL;
        }
        
        
        index++;
    }
    
    m_nBarrierCurrentHp = m_nBarrierTotalHp;
    CCLOG("m_nBarrierTotalHp:%d",m_nBarrierTotalHp);
    CCSprite *progressHp = CCSprite::createWithSpriteFrameName(Hp_Barrier_Frame);
    progressHp->setPosition(ccp(size.width/2, Barrier_Y + Atk_Barrier_DY));
    addChild(progressHp,Fight_Zorder_Up,Fight_Sprites_LansHp_Progress);
    
    
    CCProgressTimer *barHp = CCProgressTimer::create(CCSprite::createWithSpriteFrameName(Hp_Barrier_Bar));
    barHp->setPosition(ccp(size.width/2, Barrier_Y + Atk_Barrier_DY));
    barHp->setType(kCCProgressTimerTypeBar);
    barHp->setMidpoint(ccp(0, 0));
    barHp->setBarChangeRate(ccp(1, 0));
    addChild(barHp,Fight_Zorder_Up,Fight_Sprites_LansHp_Progress_Bar);
    
    float percent = (float)m_nBarrierCurrentHp / (float)m_nBarrierTotalHp;
    barHp->setPercentage(percent*100);
    
    CCSequence *seq = CCSequence::create(CCDelayTime::create(5),
                                         CCFadeOut::create(2),
                                         NULL);
    
    progressHp->runAction(seq);
    barHp->runAction((CCSequence*)(seq->copy()));
    
    return true;
}

#pragma mark - private logic
void FightLayer::pullCatapult(CCPoint point)
{
    m_fAngle = HelpMethod::getAngle(point.x - m_fCatapult_Org_X, point.y-m_fCatapult_Org_Y);
    if (((m_fAngle<180.0f)&&(m_fAngle>=70.0f)))
    {
        m_fAngle = Min_Angle;
    }
    float f = m_fPower = ccpDistance(ccp(m_fCatapult_Org_X,m_fCatapult_Org_Y), point);
    if (f>Max_Length) {
        f = Max_Length;
    }
 
    if((m_fAngle<=-70.0f)&&(m_fAngle>-180.0f))
        m_fAngle = Max_Angle;
    if (f<30.0f) {
       // m_fAngle = (450.0f + m_fAngle)/3.0f;
        m_fAngle = 0.0f;
    }
    float x = m_fCatapult_Org_X - (int)HelpMethod::getAngleDX(m_fAngle- 90, f);
    float y = m_fCatapult_Org_Y -(int)HelpMethod::getAngleDY(m_fAngle - 90, f);
    
    CCSprite *_tray =(CCSprite*)getChildByTag(Fight_Sprites_Catapult_Tray);
    _tray->setRotation(m_fAngle);
    _tray->setPosition(ccp(x,y));
    
    Hero *_bullet = (Hero*)getChildByTag(Fight_Sprites_Bullet);
    if(_bullet)
    {
        _bullet->setRotation(m_fAngle);
        _bullet->setPosition(ccp(x, y));
    }
    else
    {
        resetTrayPos();
    }
    
}

void FightLayer::resetTrayPos()
{
    bTouchedTray = false;
    float _angle = m_fAngle;
    CCSprite *_tray = (CCSprite*)getChildByTag(Fight_Sprites_Catapult_Tray);
    _tray->setPosition(ccp(m_fCatapult_Org_X,m_fCatapult_Org_Y-20));
    m_fAngle = 0;
    _tray->setRotation(m_fAngle);
    
    CCActionInterval* _move = CCMoveBy::create(0.2f, ccp(0,10));
    CCActionInterval* _move_back = _move->reverse();
    CCActionInterval* _move_ease_in = CCEaseElasticOut::create((CCActionInterval*)(_move->copy()->autorelease()));
    CCSequence* _seq = CCSequence::create(
                                          _move_ease_in,
                                          _move_back,
                                          NULL
                                          );
    CCSequence* _seq1 = CCSequence::create(
                                          _move_ease_in,
                                          _move_back,
                                          CCCallFunc::create(this, callfunc_selector(FightLayer::catapultResetCallBack)),
                                          NULL
                                          );
    
    Hero *_bullet = (Hero*)getChildByTag(Fight_Sprites_Bullet);
    if(_bullet)
    {
        _bullet->setRotation(m_fAngle);
        _bullet->setPosition(ccp(m_fCatapult_Org_X,m_fBullet_Org_Y));
        
        if (m_fPower<70.0f||m_bStartQiangBao_Teach)   //弹弓拉的力度不够 不发射
        {
            _bullet->runAction(_seq);
            _tray->runAction((CCSequence*)_seq->copy());
        }
        else
        {
            _tray->runAction(_seq1);
            setCoolDown(_bullet);
            fireBullet(_bullet, _angle);
            _bullet->removeFromParentAndCleanup(true);
        }
        CocosDenshion::SimpleAudioEngine::sharedEngine()->stopEffect(m_nXuliSoundID);
    }
    else
    {
        m_powerTime.mCurTime = 0;
        _tray->runAction(_seq1);
    }
   m_fPower = 0;
}

void FightLayer::fireBullet(Hero *pHero , float pAngle)
{
    if(!m_bCanPullCatapult)return;
    m_nFireCount++;
    m_bHitComb = false;
    Hero *_hero = pHero;
    
    Game_Hero_Data *_data = _hero->getData();
    Hero *_bullet = new Hero(_data);
    _bullet->autorelease();
    
    int type =  _bullet->getType();
    char buf[30];
    memset(buf, 0, 30);
    sprintf(buf, "Sound/hero/%d_gongji.ogg",type);
    DATAPOOL->playGameEffect(buf);
    
    _bullet->setPosition(ccp(m_fCatapult_Org_X, m_fCatapult_Org_Y));
    if(_hero->isPowerState())
    {
        _bullet->setState(Hero_State_PowerMove);
        CCArmature *zhunxin = (CCArmature*)getChildByTag(Fight_Sprites_ZhunXin);
        if(_bullet->getType()==Hero_Type_BoLuo&&zhunxin)
        {
            zhunxin->removeFromParentAndCleanup(true);
            DATAPOOL->removeEffectCache(ZhunXinRes);
        }
        if(m_bPartner&&!m_bEnergy)
        {
            if(_bullet->getType()!=Hero_Type_ShiLiu||!bTouchedTray)
            {
                m_bUseKuangBao =false;
                m_bPartner = false;
            }
        }
    }
    else
    {
        _bullet->setState(Hero_State_Move);
    }
    _bullet->setAngle(pAngle);
    addChild(_bullet);
    m_pBullet->addObject(_bullet);
    _hero = NULL;
    _data = NULL;
}

void FightLayer::fireBulletLianNu(float pAngle)
{
    m_nFireCount++;
    m_bHitComb = false;
    DATAPOOL->addEffectCache(BaoJiHuoQiuRes);
    CCArmature *arm = CCArmature::create("baojihuoqiu");
    Hero *hero = new Hero(arm,10);
    hero->autorelease();
    DATAPOOL->playGameEffect("Sound/hero/1001_gong.ogg");
    addChild(hero);
    hero->setState(Hero_State_Move);
    hero->setAngle(pAngle);
    hero->setPosition(ccp(m_fCatapult_Org_X, m_fCatapult_Org_Y));
    m_pBullet->addObject(hero);
}

void FightLayer::checkState()
{
    if (m_eModeInfo.bInPause||m_eModeInfo.mLogicState==Fight_LogicState_None||!m_bDelayComplate)return;
    
    bool bClear = true;
    
    if (m_eModeInfo.mLogicState == Fight_LogicState_Enter)  //进场
    {

    }
    else if (m_eModeInfo.mLogicState == Fight_LogicState_Create)
    {
        createEnemy();
    }
    else if (m_eModeInfo.mLogicState == Fight_LogicState_Ready)
    {
        bClear = false;
    }
    else if(m_eModeInfo.mLogicState == Fight_LogicState_ACT)
    {

    }
}

bool FightLayer::createEnemy()
{
    if(m_eType == Game_Type_Boss)
    {
        if(m_nCurrentWave<1)
        {
            m_nCurrentWave++;
            
            DATAPOOL->addEffectCache(GeBuLinRes);
            
            int _type = xUser->bossId;
            m_nBossType = _type;
            Enemy_Data* enemyData = DATAPOOL->getEnemyData(_type);
            Enemy *enemy = NULL;
            char buf[40] = {0};
            sprintf(buf, "Sprites/%s.ExportJson",enemyData->mSourceName);
            DATAPOOL->addEffectCache(buf);
            enemy = new Enemy(enemyData,this,xUser->allHp,xUser->leftHp,xData->getDefenseAdd());
            enemy->autorelease();
            enemyData = NULL;
            CCSize size = CCDirector::sharedDirector()->getWinSize();
            enemy->addPoint(ccp(size.width/2, size.height/2+Max_Jump_Y_Center-50));
            
            {
                CCArmature *arm = CCArmature::create("chuxian");
                arm->setPosition(enemy->getPosition());
                arm->getAnimation()->play("chuxian");
                arm->getAnimation()->setMovementEventCallFunc(enemy, movementEvent_selector(FightLayer::animationEvent));
                addChild(arm,Fight_Zorder_Enemy);
            }
            m_eModeInfo.mLogicState = Fight_LogicState_Ready;
        }
        return true;
    }

    if(!getLevelData(getLevelNum()))
        return false;
    if(m_nCurrentWave<m_pLevelData->nWaveCount)
    {
        int elementCount = m_pLevelData->mArrayWave[m_nCurrentWave]->mElementCount;
        CCLOG("mElementCount:%d",elementCount);
        if((m_pEnemy->count()+elementCount)>Max_Enemy&&m_pEnemy->count()!=0)
        {
            m_eModeInfo.mLogicState = Fight_LogicState_Ready;
            return false;
        }
    }
    CCLOG("createEnemy!");
    m_nCurrentWave++;
    int _wave = m_nCurrentWave;
    if (m_nCurrentWave<=m_pLevelData->nWaveCount)  //当前的波数小于等于总波数
    {
        if(m_eType == Game_Type_PVP&&
           m_LansIndex>=UserData::Instance()->enemy_mercemaryNum&&
           m_nCurrentWave==m_pLevelData->nWaveCount)m_bLastWave = true;
        if(_wave<m_pLevelData->nWaveCount&&m_pLevelData->mArrayWave[_wave]->mElementCount>=EnemyInfo_Num&&!m_bEnemyInfo)  //提示大波怪兽来临
        {
            m_bEnemyInfo = true;
        }
        
        if(DATAPOOL->m_bTeachStepOver[TEACH_STEP_SELECT_HERO_G]&&!m_bUseNewHero_Teach)
        {
            m_bUseNewHero_Teach = true;
        }
        m_fGamePercent = (float)m_nCurrentWave/m_nTotalWave;
        if(m_fGamePercent>=1)m_fGamePercent=1;
        
        for (int i = 0; i<m_pLevelData->mArrayWave[_wave-1]->mElementCount; ++i)
        {
            int _type = m_pLevelData->mArrayWave[_wave-1]->mArrayElement[i]->mType;
            _type+=First_Enemy_ID-1;
            Enemy_Data* enemyData = DATAPOOL->getEnemyData(_type);
            Enemy *enemy = NULL;
            if(m_eType == Game_Type_PVP)
            {
                CCLOG("enemy_mercemaryNum:%d",UserData::Instance()->enemy_mercemaryNum);
                m_fGamePercent = (float)m_LansIndex/UserData::Instance()->enemy_mercemaryNum;
                if(UserData::Instance()->enemy_mercemaryNum==0)
                {
                    //小怪
                    enemy = new Enemy(enemyData,this,PVP_Coefficient_One,0);
                    enemy->autorelease();
                }
                else
                {
                    CCLOG("lansIndex : %d   mercemaryId : %d",m_LansIndex,UserData::Instance()->enemy_listMercemary[m_LansIndex]->mercemaryId);
                    if(enemyData->mType == UserData::Instance()->enemy_listMercemary[m_LansIndex]->mercemaryId)
                    {
                        int _level = UserData::Instance()->enemy_listMercemary[m_LansIndex]->grade;
                        enemy = new Enemy(enemyData,this,PVP_Coefficient_Two,_level);
                        enemy->autorelease();
                    }
                    else  //小怪
                    {
                        enemy = new Enemy(enemyData,this,PVP_Coefficient_Two,0);
                        enemy->autorelease();
                    }
                }
            }
            else if(m_eType == Game_Type_Endless)
            {
                enemy = new Enemy(enemyData,this,false);
                enemy->autorelease();
            }
            else   //pve
            {
                enemy = new Enemy(enemyData,this,true);
                enemy->autorelease();
                //抢宝怪
                {
                    int num = arc4random()%10000;
                    if(!m_bIsSnatch&&num>=9500&&m_eType == Game_Type_Level&&(m_nCurrentLevel!=1&&m_nCurrentLevel!=2))
                    {
                        m_bIsSnatch = true;
                        createQiangBaoGuai();
                    }
                }
                
                
                if(m_nCurrentWave==m_pLevelData->nWaveCount)
                {
                    m_bCompleteWave = true;
                }
                
            }
            enemyData = NULL;
            enemy->addPoints(m_pLevelData->mArrayWave[_wave-1]->mArrayElement[i]);
            {
                CCArmature *arm = CCArmature::create("chuxian");
                arm->setPosition(enemy->getPosition());
                arm->getAnimation()->play("chuxian");
                arm->getAnimation()->setMovementEventCallFunc(enemy, movementEvent_selector(FightLayer::animationEvent));
                addChild(arm,Fight_Zorder_Enemy);
            }
        }
        
        
        updateProgress();
        
        m_bDelayComplate = false;
        m_eModeInfo.mLogicState = Fight_LogicState_Ready;
    }
    return true;
}

bool FightLayer::getLevelData(int nLevel)
{
    CCLOG("Level:%d",nLevel);
    if(!m_pLevelData)
    m_pLevelData = CfgDataManage::getInstance()->getLevelData(nLevel);
    if (!m_pLevelData) {
        return false;
    }
    getTotalWave();
    return true;
}

void FightLayer::createNewBullet()
{
    Hero *_newbullet = (Hero*)getChildByTag(Fight_Sprites_Bullet);
    if(_newbullet)
        return;
    
    DATAPOOL->playGameEffect(Change);
    Game_Hero_Data *_data = NULL;
    if(m_eType == Game_Type_Endless)
    {
        int num = 1;
        int index = 1;
        do
        {
             num = arc4random()%21+1;
            if(num>11)  //提高苹果出现的概率
                num-=10;
            else
                num = 1;
            index = num;
            index+=(First_Hero_ID-1);
        }
        while (!(DATAPOOL->findHeroUnlock(index)));
        
        _data = DATAPOOL->getHeroData(index);// CfgDataManage::getInstance()->getGameHeroData(index);   //
    }
    else
    {
        _data = DATAPOOL->getHeroData(First_Hero_ID); //CfgDataManage::getInstance()->getGameHeroData(1);   //获得普通英雄的数据
    }
    
    Hero *_hero = new Hero(_data);
    _hero->autorelease();
    _data = NULL;
    Hero *_bullet =(Hero*)m_pHero->lastObject();
    if (!_bullet)
    {
        return;
    }
    m_pHero->removeObject(_bullet);
    CCPoint _point = _bullet->getPosition();
    
    int _tmp = 1;
    for (int i = m_pHero->count()-1; i>=0; i--)
    {
        Hero *_tmpHero =(Hero*)m_pHero->objectAtIndex(i);
        CCPoint _tmpPoint = _tmpHero->getPosition();
        CCJumpTo *_tmpJumpTo = CCJumpTo::create(0.1f, _point, 2, 2);
        CCSequence *_tmpSeq = CCSequence::create(CCDelayTime::create(_tmp * 0.1f),_tmpJumpTo,NULL);
        _tmpHero->runAction(_tmpSeq);
        _point = _tmpPoint;
        _tmp++;
    }
    
    _hero->setPosition(ccp(-20, 200));
    addChild(_hero,Fight_Zorder_Hero);
    m_pHero->insertObject(_hero, 0);
    CCJumpTo *_newJumpTo = CCJumpTo::create(0.1f, _point, 2, 2);
    CCSequence *_newSeq = CCSequence::create(CCDelayTime::create(_tmp * 0.1f),_newJumpTo,NULL);
    _hero->runAction(_newSeq);

    
    
    if(m_bIsUseLianNu)
    {
        _bullet->removeFromParentAndCleanup(true);
    }
    else
    {
        _bullet->setTag(Fight_Sprites_MoveToCatapult);
        CCJumpTo *_jumpTo = CCJumpTo::create(0.1f, ccp(m_fCatapult_Org_X, m_fBullet_Org_Y), 2, 1);
        CCSequence *_seq = CCSequence::create(
                                              _jumpTo,
                                              CCCallFuncO::create(this, callfuncO_selector(FightLayer::bulletReadyCallBack),_bullet),
                                              NULL
                                              );
        _bullet->runAction(_seq);
    }
    
}

void FightLayer::setCoolDown(Hero *pHero)
{
    if(m_bUseShuaXin)
        return;
    if(m_bPartner)
        return;
    
    int index = Fight_Sprites_Hero1;
    for (int i = index; i<=Fight_Sprites_Hero3; ++i)
    {
        CCNode* _hero = getChildByTag(i);
        if (_hero && ((Hero*)_hero)->getType() == pHero->getType())
        {
            ((Hero*)_hero)->coolDown();
        }
    }
}

void FightLayer::touchCatapult(CCPoint &point)
{
    CCSprite *pTray = (CCSprite*)getChildByTag(Fight_Sprites_Catapult_Tray);
    Hero *_bullet = (Hero*)getChildByTag(Fight_Sprites_Bullet);
    CCPoint _point = pTray->boundingBox().origin;
    CCSize _size = pTray->boundingBox().size;
    CCRect _touchRect = CCRectMake(_point.x-10, _point.y-20, _size.width+20, _size.height+40);   //增大弹弓拉动接触面积
    if ((_touchRect.containsPoint(point))&&_bullet)
    {
        bTouchedTray = true;
    }
    else
    {
        bTouchedTray = false;
    }
}

int FightLayer::getLevelNum()
{
    int level = 1;
    if(m_eType == Game_Type_PVP)
    {
        if(UserData::Instance()->enemy_mercemaryNum == 0)
        {
            level = PVP_Defult_Level;
        }
        else
        {
            if(m_LansIndex>=UserData::Instance()->enemy_mercemaryNum)
                return 0;
            CCLOG("boss num:%d",UserData::Instance()->enemy_mercemaryNum);
            int id =  UserData::Instance()->enemy_listMercemary[m_LansIndex]->mercemaryId;
            int index = id - First_Enemy_ID+1;
            switch (index)
            {
                case 5:
                    level = 5;
                    break;
                case 6:
                    level = 10;
                    break;
                case 11:
                    level = 15;
                    break;
                case 12:
                    level = 20;
                    break;
                case 17:
                    level = 25;
                    break;
                case 18:
                    level = 30;
                    break;
                case 23:
                    level = 35;
                    break;
                case 24:
                    level = 40;
                    break;
                case 29:
                    level = 45;
                    break;
                case 30:
                    level = 50;
                    break;
                default:
                    level = PVP_Defult_Level;
                    break;
            }
        }
    }
    else
    {
        level = (m_nCurrentChapter-1)*10+m_nCurrentLevel;
    }
    return level;
}

void FightLayer::updateProgress()
{
    if(m_eType != Game_Type_Level)
        return;
    CCProgressTimer *_progress_bar = (CCProgressTimer*)getChildByTag(Fight_Sprites_Progress);
    _progress_bar->setPercentage(m_fGamePercent*100);
    
    CCSprite *_progress_icon = (CCSprite*)getChildByTag(Fight_Sprites_Progress_Icon);
    float  _x = _progress_bar->getPositionX() - _progress_bar->getContentSize().width/2;
    _x += _progress_bar->getContentSize().width * _progress_bar->getPercentage()/100;
    _progress_icon->setPosition(ccp(_x, _progress_bar->getPositionY()));
}

void FightLayer::updateEnemyZorder()
{
    if(m_pEnemy->count()>1||m_pMuBei->count()>1||m_pShiBei->count()>1||m_pIceWall->count()>1||m_pQiangbaoGuai)
    {
        CCArray *_array = getChildren();
        for (int i=0; i<_array->count(); ++i)
            for(int j = i+1;j<_array->count();++j)
            {
                int _ZOrder1 = ((CCNode*)_array->objectAtIndex(i))->getZOrder();
                int _ZOrder2 = ((CCNode*)_array->objectAtIndex(j))->getZOrder();
                if (_ZOrder1 == Fight_Zorder_Enemy&&_ZOrder2 == Fight_Zorder_Enemy)
                {
                    CCNode *_enemy1 = (CCNode*)_array->objectAtIndex(i);
                    CCNode *_enemy2 = (CCNode*)_array->objectAtIndex(j);
                    if (_enemy1->getPositionY()<_enemy2->getPositionY())
                    {
                        _array->exchangeObjectAtIndex(i, j);
                    }
                }
            }
    }
    
}

void FightLayer::updateLianNu(float dt)
{
    m_lianNuUseTime.mCurTime+=dt;
    if(m_lianNuUseTime.mCurTime>=m_lianNuUseTime.mTotalTime)
    {
        m_lianNuUseTime.mCurTime = 0;
        m_lianNuUseTime.mTotalTime = 0;
        unschedule(schedule_selector(FightLayer::updateLianNu));
        CCSprite *catapult = (CCSprite*)getChildByTag(Fight_Sprites_Catapult_Frame);
        CCSprite *tray = (CCSprite*)getChildByTag(Fight_Sprites_Catapult_Tray);
        if(catapult)
        {
            m_bIsUseLianNu = false;
            if(DATAPOOL->isHaveTieDanGong())
            {
                catapult->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CatapultFrameTie));
                tray->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CatapultTray));
            }
            else
            {
                catapult->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CatapultFrame));
                tray->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CatapultTray));
            }
            m_fAngle = 0;
            catapult->setRotation(m_fAngle);
            tray->setRotation(m_fAngle);
            resetTrayPos();
        }
    }
}

bool FightLayer::areaAttack(cocos2d::CCPoint point, cocos2d::CCObject *object, float radius)
{
    bool result = false;
    Enemy *enemy = (Enemy*)object;
    float _x = enemy->getPositionX();
    float _y = enemy->getPositionY() + enemy->getSprite()->getContentSize().height/2;
    if(ccpDistance(point, ccp(_x,_y))<=radius)
    {
        result = true;
    }
    return result;
}

void FightLayer::updateLianNuDelta(float dt)
{
    if (m_bIsUseLianNu&&m_bIsFireLianNu)
    {
        m_lianNuDeltaTime.mCurTime+=dt;
        if(m_lianNuDeltaTime.mCurTime>=m_lianNuDeltaTime.mTotalTime)
        {
            m_lianNuDeltaTime.mCurTime = 0;
            fireBulletLianNu(m_fAngle);
        }
    }
}

void FightLayer::beganLianNuTime(float dt)
{
    unschedule(schedule_selector(FightLayer::beganLianNuTime));
    m_bIsFireLianNu = true;
}

void FightLayer::rotateLianNu(cocos2d::CCPoint point)
{
    m_fAngle = HelpMethod::getAngle(m_fCatapult_Org_X-point.x, m_fCatapult_Org_Y-point.y);
    if (((m_fAngle<180.0f)&&(m_fAngle>=70.0f)))
    {
        m_fAngle = 70.0f;
    }
    
    float f = Max_Length/2;
    if((m_fAngle<=-70.0f)&&(m_fAngle>-180.0f))
        m_fAngle = -70.0f;
    if (f<30.0f) {
        // m_fAngle = (450.0f + m_fAngle)/3.0f;
        m_fAngle = 0.0f;
    }
    float x = m_fCatapult_Org_X - (int)HelpMethod::getAngleDX(m_fAngle- 90, f);
    float y = m_fCatapult_Org_Y -(int)HelpMethod::getAngleDY(m_fAngle - 90, f);
    
    CCSprite *liannuFrame = (CCSprite*)getChildByTag(Fight_Sprites_Catapult_Frame);
    CCSprite *liannuTray = (CCSprite*)getChildByTag(Fight_Sprites_Catapult_Tray);
    liannuFrame->setRotation(m_fAngle);
    liannuTray->setRotation(m_fAngle);
    liannuTray->setPosition(ccp(x, y));
}

void FightLayer::createEffect(const char *fileName,const char *aniName,CCPoint pos,int zOrder,CCObject *target ,bool loadFile )
{
    if(loadFile)
       DATAPOOL->addEffectCache(fileName);
    CCArmature *arm = CCArmature::create(aniName);
    arm->setPosition(pos);
    arm->getAnimation()->play(aniName);
    if(target==NULL)
        arm->getAnimation()->setMovementEventCallFunc(arm, movementEvent_selector(FightLayer::animationEvent));
    else
        arm->getAnimation()->setMovementEventCallFunc(target, movementEvent_selector(FightLayer::animationEvent));
    addChild(arm,zOrder);
}

void FightLayer::createPeel(cocos2d::CCPoint pos, int num)
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    for (int i=0; i<num; ++i)
    {
        CCSprite *peel = CCSprite::createWithSpriteFrameName(PeelRes);
        peel->setScale(0.3f);
        peel->setPosition(pos);
        float width = peel->getContentSize().width;
        float height = peel->getContentSize().height;
        float dx = 0,dy = 0;
        do
        {
            dx = arc4random()%200;
            dy = arc4random()%200;
            dx -=100;
            dy -=100;
        } while ((pos.x+dx<width/2||pos.x+dx>size.width-width/2)||
                 (pos.y+dy>size.height-height/2||pos.y+dy<Barrier_Y+height/2));
        CCScaleTo *scaleTo = CCScaleTo::create(0.3f, 1);
        CCRotateBy *rotateBy = CCRotateBy::create(0.3f, 1440);
        CCMoveBy *moveBy = CCMoveBy::create(0.3f, ccp(dx, dy));
        CCSpawn *spawn = CCSpawn::create(scaleTo,
                                         moveBy,
                                         rotateBy,
                                         NULL);
        CCSequence *seq = CCSequence::create(spawn,
                                             CCCallFuncN::create(this, callfuncN_selector(FightLayer::peelCallBack)),
                                             NULL);
        addChild(peel,Fight_Zorder_Top);
        peel->runAction(seq);
    }
}

void FightLayer::removeBullet(Hero *object)
{
    this->removeChild(object, true);
    m_pBullet->removeObject(object);
}

void FightLayer::checkHeroAtk(Hero *_bullet, Enemy *_enemy, int _type,int index)
{
    if(_enemy->getRebound()&&!(_bullet->isPowerState())&&!_bullet->isLianNuBullet()) //黑糖恶魔 反弹普通伤害
    {
        _bullet->reverseY();
        _bullet->setAtkEnemy(true);
        _bullet->setAtkSelf(true);
        _bullet->setRemove(false);
        _enemy->speakSkill();
        m_nCombo = 0;
        return;
    }
    
    m_nCombo++;
    updateCombo(_enemy->getPosition());
    int add = m_eType==Game_Type_Boss?xData->getAttackAdd():0;
    switch (_type)   //攻击技能
    {
        case Hero_Type_LianNu:
        {
            _enemy->minusBlood(_bullet->getAtk(),true,0);
            EnemyHurtState *_instance = EnemyHurtState::getInstance();
            _enemy->GetFSMSystem()->ChangeState(_instance);
        }
            break;
        case Hero_Type_PingGuo:  //苹果
        {
            if(!(_bullet->isPowerState()))  //普通攻击
            {
                _enemy->minusBlood(_bullet->getAtk(),false,add);
                EnemyHurtState *_instance = EnemyHurtState::getInstance();
                _enemy->GetFSMSystem()->ChangeState(_instance);
            }
            else   //蓄力攻击
            {
                _enemy->minusBlood(_bullet->getPowerAtk(),true,add);
                EnemyHurtState *_instance = EnemyHurtState::getInstance();
                _enemy->GetFSMSystem()->ChangeState(_instance);
                CCSize size = CCDirector::sharedDirector()->getWinSize();
                if(_enemy->getPositionY()+50<size.height/2+Max_Jump_Y_Center-50)
                {
                    CCMoveBy *moveby = CCMoveBy::create(1, ccp(0, 50));
                    CCSequence *seq = CCSequence::create(moveby,
                                                         CCCallFuncN::create(this, callfuncN_selector(FightLayer::removeParticleCallBack)),
                                                         NULL);
                    _enemy->runAction(seq);
                    CCParticleSmoke *m_emitter = NULL;
                    m_emitter = (CCParticleSmoke*)_enemy->getChildByTag(Effect_Particle);
                    if(!m_emitter)
                    {
                        m_emitter = CCParticleSmoke::create();
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
                        m_emitter->setTexture(CCTextureCache::sharedTextureCache()->addImage("fire.png"));
#else
                        m_emitter->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle/fire.png"));
#endif

                        m_emitter->setPosition(_enemy->getSprite()->getPosition());
                        m_emitter->setLife(1);
                        m_emitter->setTotalParticles(20);
                        m_emitter->setStartSize(30);
                        m_emitter->setStartSizeVar(5);
                        m_emitter->setEndSize(5);
                        m_emitter->setEndSizeVar(0);
                        m_emitter->setAutoRemoveOnFinish(true);
                        _enemy->addChild(m_emitter,0,Effect_Particle);
                    }
                }
            }
        }
            break;
        case Hero_Type_HuoLongGuo:
        {
            if(!(_bullet->isPowerState()))
            {
                _enemy->minusBlood(_bullet->getAtk(),false,add);
                EnemyHurtState *_instance = EnemyHurtState::getInstance();
                _enemy->GetFSMSystem()->ChangeState(_instance);
            }
            else
            {
                _enemy->minusBlood(_bullet->getPowerAtk(),true,add);
                EnemyHurtState *_instance = EnemyHurtState::getInstance();
                _enemy->GetFSMSystem()->ChangeState(_instance);
            }
        }
            break;
        case Hero_Type_ShiLiu:
        {
            if(!(_bullet->isPowerState()))
            {
                _enemy->minusBlood(_bullet->getAtk(),false,add);
                EnemyHurtState *_instance = EnemyHurtState::getInstance();
                _enemy->GetFSMSystem()->ChangeState(_instance);
            }
            else
            {
                _enemy->minusBlood(_bullet->getPowerAtk(),true,add);
                EnemyHurtState *_instance = EnemyHurtState::getInstance();
                _enemy->GetFSMSystem()->ChangeState(_instance);
            }
        }
            break;
        case Hero_Type_BoLuo:
        {
            if(!(_bullet->isPowerState()))
            {
                _enemy->minusBlood(_bullet->getAtk(),false,add);
                EnemyStopState *_instance = EnemyStopState::getInstance();
                _instance->setTime(_bullet->getDuration(),Stop_Type_Bondage);
                _enemy->GetFSMSystem()->ChangeState(_instance);
            }
            else
            {
                _enemy->minusBlood(_bullet->getPowerAtk(),true,add);
            }
        }
            break;
        case Hero_Type_XiangJiao:
        {
            if(!(_bullet->isPowerState()))
            {
                _enemy->minusBlood(_bullet->getAtk(),false,add);
                EnemyHurtState *_instance = EnemyHurtState::getInstance();
                _enemy->GetFSMSystem()->ChangeState(_instance);
            }
            else
            {
                _enemy->minusBlood(_bullet->getPowerAtk(),true,add);
                EnemyHurtState *_instance = EnemyHurtState::getInstance();
                _enemy->GetFSMSystem()->ChangeState(_instance);
            }
        }
            break;
        case Hero_Type_Lizi:
        {
            if(!(_bullet->isPowerState()))
            {
                _enemy->minusBlood(_bullet->getAtk(),false,add);
                EnemyHurtState *_instance = EnemyHurtState::getInstance();
                _enemy->GetFSMSystem()->ChangeState(_instance);
            }
            else
            {
                _enemy->minusBlood(_bullet->getPowerAtk(),true,add);
                EnemyHurtState *_instance = EnemyHurtState::getInstance();
                _enemy->GetFSMSystem()->ChangeState(_instance);
                this->runAction(CCShake::create(0.5, 5));
            }
        }
            break;
        case Hero_Type_TaoZi:
        {
            if(!(_bullet->isPowerState()))
            {
                _enemy->minusBlood(_bullet->getAtk(),false,add);
                EnemyHurtState *_instance = EnemyHurtState::getInstance();
                _enemy->GetFSMSystem()->ChangeState(_instance);
            }
            else
            {
                _enemy->minusBlood(_bullet->getPowerAtk(),true,add);
                EnemyHurtState *_instance = EnemyHurtState::getInstance();
                _enemy->GetFSMSystem()->ChangeState(_instance);
            }
        }
            break;
        case Hero_Type_CaoMei:
        {
            if(!(_bullet->isPowerState()))
            {
                _enemy->minusBlood(_bullet->getAtk(),false,add);
                EnemyStopState *_instance = EnemyStopState::getInstance();
                _instance->setTime(_bullet->getDuration(),Stop_Type_Bondage);
                _enemy->GetFSMSystem()->ChangeState(_instance);
                _enemy->enemyBondage(_bullet->getDuration(),_bullet->getAddAtk());

            }
            else
            {
                _enemy->minusBlood(_bullet->getPowerAtk(),true,add);
                EnemyStopState *_instance = EnemyStopState::getInstance();
                _instance->setTime(_bullet->getDuration(),Stop_Type_Bondage);
                _enemy->GetFSMSystem()->ChangeState(_instance);
                _enemy->enemyBondage(_bullet->getDuration(),_bullet->getAddAtk());
                for (int m = m_pEnemy->count()-1; m>=0; m--)
                {
                    Enemy *enemy = (Enemy*)m_pEnemy->objectAtIndex(m);
                    if(enemy->getState()==Enemy_State_MuBei)continue;
                    if(m!=index&&areaAttack(_enemy->getPosition(),enemy, 180)&&!enemy->isInvincible())
                    {
                        EnemyStopState *_instance1 = EnemyStopState::getInstance();
                        _instance1->setTime(_bullet->getDuration(),Stop_Type_Bondage);
                        enemy->GetFSMSystem()->ChangeState(_instance1);
                        enemy->enemyBondage(_bullet->getDuration(),_bullet->getAddAtk());
                    }
                }
            }
        }
            break;
        case Hero_Type_LiuLian:   //榴莲
        {
            if(!(_bullet->isPowerState()))
            {
                _enemy->minusBlood(_bullet->getAtk(),false,add);
                _enemy->enemyPoison(_bullet->getDuration(),_bullet->getAddAtk());
                createEffect(DuARes,"du_a", _enemy->getPosition(), Fight_Zorder_Up);
                EnemyPoisonState *_instance = EnemyPoisonState::getInstance();
                _enemy->GetFSMSystem()->ChangeState(_instance);
            }
            else
            {
                _enemy->minusBlood(_bullet->getPowerAtk(),true,add);
                _enemy->enemyPoison(PosionTime-1,_bullet->getAddAtk()); //_bullet->getDuration()
                createEffect(DuBRes,"du_b", _enemy->getPosition(), Fight_Zorder_Up);
                EnemyPoisonState *_instance = EnemyPoisonState::getInstance();
                _enemy->GetFSMSystem()->ChangeState(_instance);
                for (int m = 0; m<m_pEnemy->count(); m++)
                {
                    Enemy *enemy = (Enemy*)m_pEnemy->objectAtIndex(m);
                    if(enemy->getState()==Enemy_State_MuBei)continue;
                    if(m!=index&&areaAttack(_enemy->getPosition(),enemy, 180)&&!enemy->isInvincible())
                    {
                        enemy->enemyPoison(_bullet->getDuration(),_bullet->getAddAtk());
                        EnemyPoisonState *_instance1 = EnemyPoisonState::getInstance();
                        enemy->GetFSMSystem()->ChangeState(_instance1);
                    }
                }
            }
        }
            break;
        case Hero_Type_MangGuo:
        {
            if(!(_bullet->isPowerState()))
            {
                _enemy->minusBlood(_bullet->getAtk(),false,add);
                EnemySlowDownState *_instance = EnemySlowDownState::getInstance();
                _instance->setTime(_bullet->getDuration());
                _enemy->GetFSMSystem()->ChangeState(_instance);
            }
            else
            {
                _enemy->minusBlood(_bullet->getPowerAtk(),true,add);
                EnemySlowDownState *_instance = EnemySlowDownState::getInstance();
                _instance->setTime(_bullet->getDuration());
                _enemy->GetFSMSystem()->ChangeState(_instance);
                for (int m = 0; m < m_pEnemy->count(); m++)
                {
                    Enemy *enemy = (Enemy*)m_pEnemy->objectAtIndex(m);
                    if(enemy->getState()==Enemy_State_MuBei)continue;
                    if(m!=index&&areaAttack(_enemy->getPosition(),enemy, 150)&&!enemy->isInvincible())
                    {
                        CCLOG("%d",_bullet->getPowerAtk());
                        enemy->minusBlood(_bullet->getPowerAtk(),true,add);
                        EnemySlowDownState *_instance1 = EnemySlowDownState::getInstance();
                        _instance1->setTime(_bullet->getDuration());
                        enemy->GetFSMSystem()->ChangeState(_instance1);
                    }
                }
            }
        }
            break;
        case Hero_Type_XiGua:
        {
            if(!(_bullet->isPowerState()))
            {
                _enemy->minusBlood(_bullet->getAtk(),false,add);
                createEffect(BingZhaRes,"bingzha", _enemy->getPosition(), Fight_Zorder_Bullet,_enemy);
                EnemyStopState *_instance = EnemyStopState::getInstance();
                _instance->setTime(_bullet->getDuration(),Stop_Type_Frozen);
                _enemy->GetFSMSystem()->ChangeState(_instance);
            }
            else
            {
                _enemy->minusBlood(_bullet->getPowerAtk(),true,add);
                createEffect(BingZhaRes,"bingzha", _enemy->getPosition(), Fight_Zorder_Bullet,_enemy);
                EnemyStopState *_instance = EnemyStopState::getInstance();
                _instance->setTime(_bullet->getDuration(),Stop_Type_Frozen);
                _enemy->GetFSMSystem()->ChangeState(_instance);
                for (int m = 0; m < m_pEnemy->count(); m++)
                {
                    Enemy *enemy = (Enemy*)m_pEnemy->objectAtIndex(m);
                    if(enemy->getState()==Enemy_State_MuBei)continue;
                    if(m!=index&&areaAttack(_enemy->getPosition(),enemy, 200)&&!enemy->isInvincible())
                    {
                        enemy->minusBlood(_bullet->getPowerAtk(),true,add);
                        createEffect(BingZhaRes,"bingzha", _enemy->getPosition(), Fight_Zorder_Bullet,enemy);
                        EnemyStopState *_instance = EnemyStopState::getInstance();
                        _instance->setTime(_bullet->getDuration(),Stop_Type_Frozen);
                        enemy->GetFSMSystem()->ChangeState(_instance);
                    }
                }
            }
        }
        break;
        default:
            break;
    }
    
    if(_enemy->getType()==Enemy_Type_ShaBaoJuRen&&!(_bullet->isPowerState()))
    {
        EnemySprintState *_instance = EnemySprintState::getInstance();
        _enemy->GetFSMSystem()->ChangeState(_instance);
    }
}

void FightLayer::checkSpriteCollision(Hero **_bullet)
{
    //检测抢宝怪击中
    if(checkQiangBaoGuaiCollision(_bullet))return;
    
    
    for (int j = m_pEnemy->count()-1; j>=0; j--)
    {
        Enemy *_enemy = (Enemy*)m_pEnemy->objectAtIndex(j);
        if ((*_bullet)&&_enemy->checkCollision(*_bullet))
        {
            if(collisionMuBei(*_bullet,_enemy)||collisionShiBei(*_bullet,_enemy))continue;
            int _type = (*_bullet)->getType();
            (*_bullet)->setHit();
            if(_enemy->isInvincible()&&!(*_bullet)->isLianNuBullet()&&!(*_bullet)->isReverseY())   //检查是不是处于无敌状态
            {
                if(_enemy->isHitInInvincible())
                {
                    if(_enemy->getRebound())
                    {
                        (*_bullet)->reverseY();
                        (*_bullet)->setAtkEnemy(true);
                        (*_bullet)->setAtkSelf(true);
                        (*_bullet)->setRemove(false);
                        _enemy->speakSkill();
                        m_nCombo= 0;
                    }
                    
                    if(!m_bHitComb)
                    {
                        m_nHitCount++;
                        m_bHitComb = true;
                    }
                    
                    {    //击中音效和特效
                        DATAPOOL->playGameEffect(Jizhong);
                        if(_type==Hero_Type_XiangJiao&&(*_bullet)->isPowerState())
                        {
                            createEffect(XiangJiaoARes,"xiangjiao_a", (*_bullet)->getPosition(), Fight_Zorder_Bullet,this);
                        }
                        else
                        {
                            createEffect(HuoHuaRes,"huohua", HelpMethod::getCenterPoint((*_bullet), _enemy), Fight_Zorder_Bullet);
                        }
                        EnemyShieldState *_instance = EnemyShieldState::getInstance();
                        _enemy->GetFSMSystem()->ChangeState(_instance);
                    }
                    
                    if((*_bullet)->getRemove())
                    {
                        CCLOG("remove bullet1!");
                        removeBullet((*_bullet));
                        return;
                    }
                    else
                    {
                        if(_type!=Hero_Type_TaoZi&&_type!=Hero_Type_Lizi)
                            (*_bullet)->setRemove(true);
                    }
                    
                    if(_type==Hero_Type_Lizi)
                    {
                        (*_bullet)->reverseX();
                    }
                }
                continue;
            }
            
            if(_enemy->isCanEat()&&!(*_bullet)->isLianNuBullet())
            {
                EnemyEatState *_instance = EnemyEatState::getInstance();
                _enemy->GetFSMSystem()->ChangeState(_instance);
                removeBullet((*_bullet));
                return;
            }
            
            if((_enemy->getType() == Enemy_Type_JuXingGeBuLin||_enemy->getType() == Enemy_Type_LingHunShouGeZhe)&&
               !(*_bullet)->isPowerState()&&
               !(*_bullet)->isLianNuBullet())  //吸收普通伤害
            {
                //todo
                CCSprite *_miss = CCSprite::createWithSpriteFrameName(Text_Miss);
                _miss->setPosition(ccp(_enemy->getSprite()->getPositionX(), _enemy->getSprite()->getPositionY()+_enemy->getSprite()->getContentSize().height-20));
                _enemy->addChild(_miss);
                CCSequence *seq = CCSequence::create(CCSpawn::create(CCMoveBy::create(0.2, ccp(0, 20)),
                                                                     CCScaleTo::create(0.2, 1.5),
                                                                     NULL),
                                                     CCFadeOut::create(0.1),
                                                     CCCallFuncN::create(this, callfuncN_selector(Enemy::removeMissBaojiCallBack)),
                                                     NULL);
                _miss->runAction(seq);
                _enemy->speakSkill();
                m_nCombo = 0;
            }
            else if(_enemy->getState()==Enemy_State_MuBei&&!(*_bullet)->isLianNuBullet())
            {
                CCSprite *_dlg = CCSprite::createWithSpriteFrameName("Dlg_Passive_4_1.png");
                CCSequence *seq1 = CCSequence::create(CCDelayTime::create(1),
                                                      CCFadeIn::create(0.1),
                                                      CCDelayTime::create(3),
                                                      CCCallFuncN::create(this, callfuncN_selector(FightLayer::removeCallBack_Sprite)),
                                                      NULL);
                _dlg->setPosition(ccp(_enemy->getSprite()->getPositionX(), _enemy->getSprite()->getPositionY()+_enemy->getSprite()->getContentSize().height));
                _enemy->addChild(_dlg,Effect_Dlg);
                _dlg->runAction(seq1);
            }
            else
            {
                checkHeroAtk(*_bullet, _enemy, _type,j);
            }
            
            if(!m_bHitComb)
            {
                m_nHitCount++;
                m_bHitComb = true;
            }
            
            {    //击中音效和特效
                DATAPOOL->playGameEffect(Jizhong);
                if(_type==Hero_Type_XiangJiao&&(*_bullet)->isPowerState())
                {
                    createEffect(XiangJiaoARes,"xiangjiao_a", (*_bullet)->getPosition(), Fight_Zorder_Bullet,this);
                }
                else if(_type!=Hero_Type_XiGua)
                {
                    createEffect(HuoHuaRes,"huohua", HelpMethod::getCenterPoint((*_bullet), _enemy), Fight_Zorder_Bullet);
                }
            }
            if((*_bullet)->getRemove())
            {
                CCLOG("remove bullet2!");
                removeBullet((*_bullet));
                return;
            }
            else
            {
                if(_type!=Hero_Type_TaoZi&&_type!=Hero_Type_Lizi)
                    (*_bullet)->setRemove(true);
            }
            
            if(_type==Hero_Type_Lizi)
            {
                (*_bullet)->reverseX();
            }
        }
    }
}

bool FightLayer::checkPeelCollision(cocos2d::CCSprite *peel)
{
    for (int i=m_pEnemy->count()-1; i>=0; i--)
    {
        Enemy *_enemy = (Enemy*)m_pEnemy->objectAtIndex(i);
        if(_enemy->getState()==Enemy_State_MuBei)continue;
        if(_enemy&&!_enemy->isBoss()&&peel->boundingBox().containsPoint(_enemy->getPosition()))
        {
            _enemy->setState(Enemy_State_Die);
            _enemy->stopUpdate();
            _enemy->stopEffectAnimation();
            m_pEnemy->removeObject(_enemy);
            if(_enemy->getType()==Enemy_Type_XiaoGui)
                _enemy->removeJiaCheng();
            changeGoldNum(_enemy->getGold());
            this->stopAllActions();
            this->setZOrder(Fight_Zorder_Up);
            if(_enemy->isBoss())
            {
                char buf[30] = {0};
                sprintf(buf, "Sound/%d_die.ogg",_enemy->getType());
                DATAPOOL->playGameEffect(buf);
            }
            else
            {
                DATAPOOL->playGameEffect(Die1);
            }
            _enemy->getSprite()->getAnimation()->stop();
            float angle = arc4random()%360;
            _enemy->setAccelerateX(HelpMethod::getAngleAccelerateY(angle, Enemy_Accelerate));
            _enemy->setAccelerateY(HelpMethod::getAngleAccelerateX(angle, Enemy_Accelerate));
            _enemy->startDisapperUpdate();
            CCFadeOut *fadeOut = CCFadeOut::create(1);
            CCRotateBy *rotateBy = CCRotateBy::create(0.3, 360);
            CCSequence *seq = CCSequence::create(fadeOut,
                                                 CCCallFuncN::create(this, callfuncN_selector(FightLayer::disapperCallBack)),
                                                 NULL);
            CCRepeatForever *repeat = CCRepeatForever::create(rotateBy);
            _enemy->getSprite()->setAnchorPoint(ccp(0.5, 0.5));
            _enemy->runAction(seq);
            _enemy->runAction(repeat);
            return true;
        }
    }
    return false;
}

void FightLayer::readData()
{
	CCLog("readData");
    initPublicInfo();
    initLevelResInfo();
    initHeroInfo();
    initPropInfo();
    initLansInfo();
    m_eModeInfo.mLogicState = Fight_LogicState_Enter;
    m_eModeInfo.bInPause = false;
    
    char buf[30];
    memset(buf, 0, 30);
    sprintf(buf, "Sound/game_bg_sound%d.ogg",m_nCurrentChapter);
    DATAPOOL->playGameBgSound(buf, true);
    schedule(schedule_selector(FightLayer::createWave), 2);
    DATAPOOL->addEffectCache(DaoShuRes);
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCArmature *arm = CCArmature::create("daoshu");
    arm->getAnimation()->play("daoshu");
    arm->setPosition(ccp(size.width/2,size.height/2));
    arm->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(FightLayer::animationEvent));
    addChild(arm,Fight_Zorder_Top);
    
    scheduleUpdate();
}

void FightLayer::refreshData(float cleanGem)
{
    m_nCurrentWave = 0;
    m_fGamePercent =0;
    m_bIsUseLianNu = false;
    m_bIsFireLianNu = false;
    m_nFireCount = 0;
    m_nHitCount = 0;
    m_nBarrierTotalHp = 0;
    m_nBarrierCurrentHp = 0;
    m_pEnemy->removeAllObjects();
    m_pBarrier->removeAllObjects();
    m_pBullet->removeAllObjects();
    m_pHero->removeAllObjects();
    m_pInk->removeAllObjects();
    m_pPeel->removeAllObjects();
    m_pAward->removeAllObjects();
    m_pShiBei->removeAllObjects();
    m_pIceWall->removeAllObjects();
    m_pFog->removeAllObjects();
    m_pMuBei->removeAllObjects();
    m_nBestTime = 0;
    m_nTime = 0;
    m_bUseHongZhaJi =false;
    m_bUseKuangBao = false;
    m_bUseShuaXin = false;
    m_bLockHero = false;
    m_touchPropIndex = 0;
    m_bIsComplateLoading = true;
    m_bDelayComplate = false;
    m_bcreateEnemy = false;
    m_bDaoShuComplate = false;
    m_nGainGold = 0;
    m_bEnemyInfo = false;
    m_bShowTeachLayer = false;
    m_bHitComb = false;
    m_bShowResultLock = false;
    m_bCanPullCatapult = true;
    m_bIsSnatch = false;
    m_bCompleteWave = false;
    m_bStartQiangBao_Teach = false;
    m_nBossType = 0;
    m_nXuLiDeltaTime = 0;
    m_bEnemyInfoShow = false;
    m_bTwoBoss = false;
    m_bFirstWave = true;
    m_nCombo = 0;
    m_nEnergy = 0;
    m_bEnergy = false;
    m_bPartner = false;
    m_bShowPartner = false;
    m_fPower = 0.0;
    m_nTornadoCount = 0;
    m_nHuoQiuCount = 0;
    m_nBossHurtHp = 0;
    m_nMuBeiCount = 0;
    m_bLastWave = false;
    m_nTotalWave = 0;
    m_bcreatePartner = false;
    
    m_powerTime.mCurTime = 0;
    m_powerTime.mTotalTime = Power_Time;
    
    m_fireDeltaTime.mCurTime = 0;
    m_fireDeltaTime.mTotalTime = Fire_Delta_Time;
    
    m_lianNuDeltaTime.mCurTime = 0;
    m_lianNuDeltaTime.mTotalTime = LianNu_DeltaTime;
    
    m_lianNuUseTime.mCurTime = 0;
    m_lianNuUseTime.mTotalTime = 0;
    
    m_explosionNum.mCurTime = 0;
    m_explosionNum.mTotalTime = Explosion_Num;
    
    m_waveTime.mCurTime = 0;
    m_waveTime.mTotalTime = Wave_Time;
    
    m_speakTime.mCurTime = 0;
    m_speakTime.mTotalTime = SPEAK_TIME;
    
    unscheduleUpdate();
    this->stopAllActions();
    this->unscheduleAllSelectors();
    
    if(m_pLevelData)
    {
        for (int i = 0; i<m_pLevelData->nWaveCount; i++)
        {
            CCLOG("i:%d",i);
            CC_SAFE_DELETE_ARRAY(m_pLevelData->mArrayWave[i]->mArrayElement);
        }
        CC_SAFE_DELETE_ARRAY(m_pLevelData->mArrayWave);
        CC_SAFE_DELETE(m_pLevelData);
    }
    
    if(m_pQiangbaoGuai)
    {
        m_pQiangbaoGuai->removeFromParentAndCleanup(true);
        m_pQiangbaoGuai = NULL;
    }
    
    
    removeAllChildren();
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}

void FightLayer::checkTrans()
{
    CCArmature *trans1 = (CCArmature*)getChildByTag(Fight_Sprites_Trans1);
    if(trans1)
    {
        CCArmature *trans2 = (CCArmature*)getChildByTag(Fight_Sprites_Trans2);
        for (int i = m_pBullet->count()-1; i>=0; i--)
        {
            Hero *_bullet =(Hero*)m_pBullet->objectAtIndex(i);
            if (!_bullet->atkSelf()&&trans1->boundingBox().containsPoint(_bullet->getPosition()))
            {
                _bullet->setPosition(trans2->getPosition());
                _bullet->reverseY();
                _bullet->setAtkSelf(true);
                _bullet->setAtkEnemy(false);
                return;
            }
            else if(!_bullet->atkSelf()&&trans2->boundingBox().containsPoint(_bullet->getPosition()))
            {
                _bullet->setPosition(trans1->getPosition());
                _bullet->reverseY();
                _bullet->setAtkSelf(true);
                _bullet->setAtkEnemy(false);
                return;
            }

        }
    }
}

void FightLayer::checkBarrierCollision(Hero *_bullet)
{
    if(!_bullet)return;
    if(!_bullet->atkSelf())
        return;
    for (int i=m_pBarrier->count()-1; i>=0; i--)
    {
        Lansquenet *lans = (Lansquenet*)m_pBarrier->objectAtIndex(i);
        if(lans&&lans->checkCollision(_bullet))
        {
            int atk = _bullet->isPowerState()?_bullet->getPowerAtk():_bullet->getAtk();
            minusBarrierHp(atk);
            playBarrierUnderAtkAni();
            removeBullet(_bullet);
            return;
        }
    }
}

void FightLayer::updateHpBar()
{
    CCSprite *progressHp = (CCSprite*)getChildByTag(Fight_Sprites_LansHp_Progress);
    CCProgressTimer *barHp = (CCProgressTimer*)getChildByTag(Fight_Sprites_LansHp_Progress_Bar);
    if(progressHp&&barHp)
    {
        progressHp->stopAllActions();
        barHp->stopAllActions();
        if(barHp->getOpacity()<=100)
        {
            CCSequence *seq = CCSequence::create(CCFadeIn::create(0.2),
                                                 CCDelayTime::create(5),
                                                 CCFadeOut::create(0.2),
                                                 NULL);
            
            progressHp->runAction(seq);
            barHp->runAction((CCSequence*)seq->copy());
        }
        else
        {
            barHp->setOpacity(255);
            progressHp->setOpacity(255);
            
            CCSequence *seq = CCSequence::create(CCDelayTime::create(5),
                                                 CCFadeOut::create(0.2),
                                                 NULL);
            barHp->runAction(seq);
            progressHp->runAction((CCSequence*)seq->copy());
        }
        float percent = 0;
        if(First_Barrier==m_nCurrentBarrier)
        {
             percent = (float)m_nBarrierCurrentHp / (float)m_nBarrierTotalHp;
        }
        else if(Second_Barrier==m_nCurrentBarrier)
        {
            percent = (float)m_nBarrierCurrentHp_Second / (float)m_nBarrierTotalHp_Second;
        }
       
        CCLOG("m_nBarrierCurrentHp:%d",m_nBarrierCurrentHp);
        CCLOG("m_nBarrierTotalHp:%d",m_nBarrierTotalHp);
        CCLOG("percent:%f",percent);
//        CCProgressTo *proTo = CCProgressTo::create(0.3, percent*100);
//        barHp->runAction(proTo);
        barHp->setPercentage(percent*100);
    }

}

void FightLayer::updatePropNum()
{
    int index_prop_num = Fight_Sprites_PropNum1;
    int index_prop = Fight_Sprites_Prop1;
    for (int i = 0; i<Max_Game_Prop-1; ++i)
    {
        CCLabelBMFont *label = (CCLabelBMFont*)getChildByTag(index_prop_num);
        switch (m_nUsePropType[i])
        {
            case Prop_Type_ZhaDan:
            {
                char buf[10];
                memset(buf, 0, 10);
                sprintf(buf, "X%d",DATAPOOL->getOutfitRecord(Prop_Type_ZhaDan)->m_nNum);
                label->setString(buf);
            }
                break;
            case Prop_Type_BingJing:
            {
                char buf[10];
                memset(buf, 0, 10);
                sprintf(buf, "X%d",DATAPOOL->getOutfitRecord(Prop_Type_BingJing)->m_nNum);
                label->setString(buf);
            }
                break;
            case Prop_Type_LianNu:
            {
                char buf[10];
                memset(buf, 0, 10);
                sprintf(buf, "X%d",DATAPOOL->getOutfitRecord(Prop_Type_LianNu)->m_nNum);
                label->setString(buf);
            }
                break;
            case Prop_Type_ShuangFeng:
            {
                char buf[10];
                memset(buf, 0, 10);
                sprintf(buf, "X%d",DATAPOOL->getOutfitRecord(Prop_Type_ShuangFeng)->m_nNum);
                label->setString(buf);
            }
                break;
            case Prop_Type_HongZhaJi:
            {
                char buf[10];
                memset(buf, 0, 10);
                sprintf(buf, "X%d",DATAPOOL->getOutfitRecord(Prop_Type_HongZhaJi)->m_nNum);
                label->setString(buf);
            }
                break;
            case Prop_Type_KuangBaoYao:
            {
                char buf[10];
                memset(buf, 0, 10);
                sprintf(buf, "X%d",DATAPOOL->getOutfitRecord(Prop_Type_KuangBaoYao)->m_nNum);
                label->setString(buf);
            }
                break;
            case Prop_Type_HuiFuYao:
            {
                char buf[10];
                memset(buf, 0, 10);
                sprintf(buf, "X%d",DATAPOOL->getOutfitRecord(Prop_Type_HuiFuYao)->m_nNum);
                label->setString(buf);
            }
                break;
            case Prop_Type_ShuaXinQiu:
            {
                char buf[10];
                memset(buf, 0, 10);
                sprintf(buf, "X%d",DATAPOOL->getOutfitRecord(Prop_Type_ShuaXinQiu)->m_nNum);
                label->setString(buf);
            }
                break;
            default:
                break;
        }
        if(m_nUsePropType[i]!=-1&&m_nUsePropType[i]!=0)
        {
            if(DATAPOOL->getOutfitRecord(m_nUsePropType[i])->m_nNum<=0) //用完从道具盘中移除
            {
                label->removeFromParentAndCleanup(true);
                m_nUsePropType[i]=0;
                CCSprite* prop = (CCSprite*)getChildByTag(index_prop);
                if(prop)
                {
                    prop->removeFromParentAndCleanup(true);
                }
            }
        }
        index_prop++;
        index_prop_num++;
    }
}

int FightLayer::getAward(int tag)
{
    int count = 0;
    switch (tag) {
        case Award_Tag_Gold:
        {
            int full = 11;
            int num = arc4random()%100;
//            int count = 0;
            if(num<70)
            {
               count = full - (num/10);
            }
            else
                count = 5;
            CCLOG("count:%d",count);
            changeGoldNum(count);
            DATAPOOL->playGameEffect(Award_Gold);
            DATAPOOL->removeEffectCache(JinBiRes);
        }
            break;
        case Award_Tag_Gem:
        {
            int full = 5;
            int num = arc4random()%100;
//            int count = 0;
            if(num<50)
                count = full - (num/10);
            else
                count = 1;
//            m_nGameGainGem+=count;
            changeGemNum(count);
            CCLOG("count:%d",count);
            DATAPOOL->playGameEffect(Award_Gem);
            DATAPOOL->removeEffectCache(ZuanShiRes);
        }
            break;
        case Award_Tag_ZhaDan:
        {
            int num = arc4random()%100;
//            int count = 0;
            if(num<30)
            {
                if(num/10==0)
                    count = 2;
                else
                    count = 1;
            }
            else
                count = 1;
            CCLOG("count:%d",count);
           OutfitRecord *rcd = DATAPOOL->getOutfitRecord(Prop_Type_ZhaDan);
            if(count>0)
            {
                rcd->m_nNum+=count;
                if(rcd->m_nNum>999)rcd->m_nNum=999;
                DATAPOOL->setOutfitRecord(Prop_Type_ZhaDan, *rcd);
            }
            DATAPOOL->removeEffectCache(LiPinRes);
        }
            break;
        case Award_Tag_BingJing:
        {
            int num = arc4random()%100;
//            int count = 0;
            if(num<30)
            {
                if(num/10==0)
                    count = 2;
                else
                    count = 1;
            }
            else
                count = 1;
            CCLOG("count:%d",count);
            OutfitRecord *rcd = DATAPOOL->getOutfitRecord(Prop_Type_BingJing);
            if(count>0)
            {
                rcd->m_nNum+=count;
                if(rcd->m_nNum>999)rcd->m_nNum=999;
                DATAPOOL->setOutfitRecord(Prop_Type_BingJing, *rcd);
            }
            DATAPOOL->removeEffectCache(LiPinRes);
        }
            break;
        case Award_Tag_LianNu:
        {
            int num = arc4random()%100;
//            int count = 0;
            if(num<30)
            {
                if(num/10==0)
                    count = 2;
                else
                    count = 1;
            }
            else
                count = 1;
            CCLOG("count:%d",count);
            OutfitRecord *rcd = DATAPOOL->getOutfitRecord(Prop_Type_LianNu);
            if(count>0)
            {
                rcd->m_nNum+=count;
                if(rcd->m_nNum>999)rcd->m_nNum=999;
                DATAPOOL->setOutfitRecord(Prop_Type_LianNu, *rcd);
            }
            DATAPOOL->removeEffectCache(LiPinRes);
        }
            break;
        default:
            break;
    }
    updatePropNum();
    return count;
}

void FightLayer::checkShiBei()   //冰墙也用此函数
{
    for(int i = m_pBullet->count()-1;i>=0;i--)
    {
        Hero *bullet = (Hero*)m_pBullet->objectAtIndex(i);
        for(int j = m_pShiBei->count()-1;j>=0;j--)
        {
            CCArmature *shibei = (CCArmature*)m_pShiBei->objectAtIndex(j);
            
            if(shibei&&!(bullet->atkSelf())&&shibei->boundingBox().containsPoint(bullet->getPosition()))
            {
                bullet->reverseY();
                bullet->setAtkSelf(true);
                bullet->setAtkEnemy(false);
                break;
            }
        }
    }
    //冰墙
    for(int i = m_pBullet->count()-1;i>=0;i--)
    {
        Hero *bullet = (Hero*)m_pBullet->objectAtIndex(i);
        for(int j = m_pIceWall->count()-1;j>=0;j--)
        {
            CCArmature *ice = (CCArmature*)m_pIceWall->objectAtIndex(j);
            
            if(ice&&ice->boundingBox().containsPoint(bullet->getPosition()))
            {
                DATAPOOL->playGameEffect(Jizhong);
                createEffect(HuoHuaRes,"huohua", HelpMethod::getCenterPoint(bullet, ice), Fight_Zorder_Bullet);
                removeBullet(bullet);
                m_nCombo= 0;
                ice->getAnimation()->play("bingzhu_b");
                m_pIceWall->removeObject(ice);
                break;
            }
        }
    }

}

void FightLayer::checkMuBei()
{
    for(int i = m_pBullet->count()-1;i>=0;i--)
    {
        Hero *bullet = (Hero*)m_pBullet->objectAtIndex(i);
        for(int j = m_pMuBei->count()-1;j>=0;j--)
        {
            CCArmature *mubei = (CCArmature*)m_pMuBei->objectAtIndex(j);
            
            if(mubei&&mubei->boundingBox().containsPoint(bullet->getPosition())&&!bullet->isLianNuBullet())
            {
                CCSprite *_miss = CCSprite::createWithSpriteFrameName(Text_Miss);
                _miss->setPosition(ccp(mubei->getPositionX(), mubei->getPositionY()+mubei->getContentSize().height-20));
                mubei->addChild(_miss);
                CCSequence *seq = CCSequence::create(CCSpawn::create(CCMoveBy::create(0.2, ccp(0, 20)),
                                                                     CCScaleTo::create(0.2, 1.5),
                                                                     NULL),
                                                     CCFadeOut::create(0.1),
                                                     CCCallFuncN::create(this, callfuncN_selector(Enemy::removeMissBaojiCallBack)),
                                                     NULL);
                _miss->runAction(seq);
                
                CCSprite *_dlg = CCSprite::createWithSpriteFrameName("Dlg_Passive_4_1.png");
                _dlg->setOpacity(0);
                CCSequence *seq1 = CCSequence::create(CCDelayTime::create(1),
                                                     CCFadeIn::create(0.1),
                                                     CCDelayTime::create(3),
                                                    CCCallFuncN::create(this, callfuncN_selector(FightLayer::removeCallBack_Sprite)),
                                                     NULL);
//                 _dlg->setPosition(ccp(mubei->getPositionX(), mubei->getPositionY()+mubei->getContentSize().height));
//                addChild(_dlg,Fight_Zorder_Top);
                _dlg->setPosition(ccp(mubei->getContentSize().width/2, mubei->getContentSize().height));
                mubei->addChild(_dlg);
                _dlg->runAction(seq1);
                createEffect(HuoHuaRes,"huohua", HelpMethod::getCenterPoint(bullet,mubei), Fight_Zorder_Bullet);
                m_nHitCount++;
                removeBullet(bullet);
                return;
            }
            else if(mubei&&mubei->boundingBox().containsPoint(bullet->getPosition())&&bullet->isLianNuBullet())
            {
                createEffect(HuoHuaRes,"huohua", HelpMethod::getCenterPoint(bullet,mubei), Fight_Zorder_Bullet);
                m_nHitCount++;
                removeBullet(bullet);
                mubei->getAnimation()->play("xiaoshi_mb");
                mubei->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(FightLayer::animationEvent));
                m_pMuBei->removeObjectAtIndex(j);
                m_nMuBeiCount--;
                return;
            }
        }
    }
}

void FightLayer::usePropBegan(cocos2d::CCPoint location)
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    int indexProp = Fight_Sprites_Prop1;
    for (int i = 0;i<Max_Game_Prop; ++i)
    {
        CCNode* _prop = getChildByTag(indexProp);
        if (_prop&&((CCSprite*)_prop)->boundingBox().containsPoint(location))
        {
            int _type = m_nUsePropType[i];
            switch (_type)
            {
                case 0:
                {
                    if(xUser->pUserInfo->gemstone<=0)
                    {
                        DATAPOOL->playGameEffect(Sound_Click_Error);
                    }
                    else
                    {
                        DATAPOOL->playGameEffect(UseProp);
                    }
                }
                    break;
                case Prop_Type_ZhaDan:
                default:
                    DATAPOOL->playGameEffect(UseProp);
                    break;
            }
            m_touchPropIndex = i;
            if(_type == 0 && indexProp != Fight_Sprites_Prop3)
            {
                return;
            }
            
            if(UNLOCKTAG == _type) //未解锁
                return;
            
            if(indexProp != Fight_Sprites_Prop3&&DATAPOOL->getOutfitRecord(_type)->m_nNum<=0)
                return;
                        switch (_type)
            {
                case 0:  //随机道具
                {
                    if(xUser->pUserInfo->gemstone<=0)
                    {
                        ((CCSprite*)_prop)->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(Icon_Prop_Random2));
                        return;
                    }
                    
                    changeGemNum(-1);
                    int index = First_Outfit_ID;
                    do {
                        index = First_Outfit_ID;
                        int delta = arc4random()%11;
                        index+=delta;
                        if(m_eType == Game_Type_Endless&&index==Prop_Type_ShuaXinQiu)
                            index--;
                    } while (index==Prop_Type_ShiZhaLan||index==Prop_Type_TieZhaLan||index==Prop_Type_TieDanGong);
                    ((CCSprite*)_prop)->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(DATAPOOL->getPropData(index)->m_sourceName));
                    ((CCSprite*)_prop)->setPosition(ccp(getChildByTag(Fight_Sprites_Prop_Tray3)->getPositionX(),getChildByTag(Fight_Sprites_Prop_Tray3)->getPositionY()));
                    CCLOG("i:%d index:%d",i,index);
                    m_nUsePropType[i] = index;
                }
                    break;
                case Prop_Type_ZhaDan:  //炸弹
                {
                    Prop_Data* data = DATAPOOL->getPropData(Prop_Type_ZhaDan);
                    CCSprite *zhadan = CCSprite::createWithSpriteFrameName(data->m_sourceName);
                    zhadan->setAnchorPoint(ccp(0.5f, 0));
                    zhadan->setPosition(ccp(location.x, location.y+100));
                    addChild(zhadan,Fight_Zorder_Top,Fight_Sprites_ZhaDan);
                    CCLayerColor *layer = CCLayerColor::create(ccc4(255, 0, 0, 100), 300, 200);
                    layer->setPosition(ccp(location.x-layer->getContentSize().width/2, location.y-layer->getContentSize().height/2));
                    addChild(layer,Fight_Zorder_Top,Fight_Sprites_ZhaDan_Area);
                    CC_SAFE_DELETE(data);
                }
                    break;
                case Prop_Type_BingJing:  //冰晶
                {
                    Prop_Data* data = DATAPOOL->getPropData(Prop_Type_BingJing);
                    CCSprite *bingjing = CCSprite::createWithSpriteFrameName(data->m_sourceName);
                    bingjing->setAnchorPoint(ccp(0.5f, 0));
                    bingjing->setPosition(ccp(location.x, location.y+100));
                    addChild(bingjing,Fight_Zorder_Top,Fight_Sprites_BingJing);
                    CCLayerColor *layer = CCLayerColor::create(ccc4(255, 0, 0, 100), 300, 200);
                    layer->setPosition(ccp(location.x-layer->getContentSize().width/2, location.y-layer->getContentSize().height/2));
                    addChild(layer,Fight_Zorder_Top,Fight_Sprites_ZhaDan_Area);
                    CC_SAFE_DELETE(data);
                }
                    break;
                case Prop_Type_LianNu:   //连弩
                {
                    CCSprite *catapult = (CCSprite*)getChildByTag(Fight_Sprites_Catapult_Frame);
                    CCSprite *tray = (CCSprite*)getChildByTag(Fight_Sprites_Catapult_Tray);
                    if(catapult&&tray)
                    {
                        if(indexProp != Fight_Sprites_Prop3)
                        {
                            OutfitRecord *rcd = DATAPOOL->getOutfitRecord(Prop_Type_LianNu);
                            rcd->m_nNum--;
                            DATAPOOL->setOutfitRecord(Prop_Type_LianNu, *rcd);
                            updatePropNum();
                        }
                        else   //随机道具
                        {
                            m_nUsePropType[i] = 0;
                            ((CCSprite*)_prop)->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(Icon_Prop_Random));
                            ((CCSprite*)_prop)->setPosition(ccp(getChildByTag(Fight_Sprites_Prop_Tray3)->getPositionX(),getChildByTag(Fight_Sprites_Prop_Tray3)->getPositionY()-15));
                        }
                        
                        if(m_bIsUseLianNu)
                        {
                            m_lianNuUseTime.mTotalTime+=LianNu_Time;
                        }
                        else
                        {
                            m_bIsUseLianNu= true;
                            m_lianNuUseTime.mTotalTime =LianNu_Time;
                            catapult->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(LianNuFrame));
                            tray->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(LianNuTray));
                            Hero *_bullet = (Hero*)getChildByTag(Fight_Sprites_Bullet);
                            if(_bullet)
                            {
                                _bullet->removeFromParentAndCleanup(true);
//                                CC_SAFE_RELEASE_NULL(_bullet);
                            }
                            
                            Hero *_bullet2 = (Hero*)getChildByTag(Fight_Sprites_MoveToCatapult);
                            if(_bullet2)
                            {
                                _bullet2->removeFromParentAndCleanup(true);
//                                CC_SAFE_RELEASE_NULL(_bullet);
                            }
                            
                            schedule(schedule_selector(FightLayer::updateLianNu), 0.5f);
                        }
                        
                    }
                }
                    break;
                case Prop_Type_HuiFuYao:   //恢复药
                {
                    if(indexProp != Fight_Sprites_Prop3)
                    {
                        OutfitRecord *rcd = DATAPOOL->getOutfitRecord(Prop_Type_HuiFuYao);
                        rcd->m_nNum--;
                        DATAPOOL->setOutfitRecord(Prop_Type_HuiFuYao, *rcd);
                        updatePropNum();
                    }
                    else
                    {
                        m_nUsePropType[i] = 0;
                        ((CCSprite*)_prop)->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(Icon_Prop_Random));
                        ((CCSprite*)_prop)->setPosition(ccp(getChildByTag(Fight_Sprites_Prop_Tray3)->getPositionX(),getChildByTag(Fight_Sprites_Prop_Tray3)->getPositionY()-15));
                    }
                    
                    int hp = m_nBarrierTotalHp*0.3;
                    addBarrierHp(hp);
                    DATAPOOL->playGameEffect(Add_HP);
                    DATAPOOL->addEffectCache(HuiFuRes);
                    CCArmature *arm = CCArmature::create("huifu");
                    arm->getAnimation()->play("huifu");
                    arm->setPosition(ccp(size.width/2, Barrier_Y));
                    arm->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(FightLayer::animationEvent));
                    addChild(arm,Fight_Zorder_Up);
                }
                    break;
                case Prop_Type_ShuangFeng:   //霜封
                {
                    if(indexProp != Fight_Sprites_Prop3)
                    {
                        OutfitRecord *rcd = DATAPOOL->getOutfitRecord(Prop_Type_ShuangFeng);
                        rcd->m_nNum--;
                        DATAPOOL->setOutfitRecord(Prop_Type_ShuangFeng, *rcd);
                        updatePropNum();
                    }
                    else
                    {
                        m_nUsePropType[i] = 0;
                        ((CCSprite*)_prop)->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(Icon_Prop_Random));
                        ((CCSprite*)_prop)->setPosition(ccp(getChildByTag(Fight_Sprites_Prop_Tray3)->getPositionX(),getChildByTag(Fight_Sprites_Prop_Tray3)->getPositionY()-15));
                    }
                    
                    for (int i = m_pEnemy->count()-1; i>=0; i--)
                    {
                        Enemy *enemy = (Enemy*)m_pEnemy->objectAtIndex(i);
                        if(enemy->getState()==Enemy_State_MuBei)continue;
                        EnemyStopState *_instance = EnemyStopState::getInstance();
                        _instance->setTime(4, Stop_Type_Frozen);
                        enemy->GetFSMSystem()->ChangeState(_instance);
                    }
                }
                    break;
                case Prop_Type_HongZhaJi:   //轰炸机
                {
                    if(m_bUseHongZhaJi)
                    {
                        return;
                    }
                    else
                    {
                        m_bUseHongZhaJi = true;
                    }
                    
                    if(false == DATAPOOL->m_bTeachStepOver[TEACH_STEP_USE_HONGZHAJI])
                    {
                        DATAPOOL->m_bTeachStepOver[TEACH_STEP_USE_HONGZHAJI] = true;
                    }
                    
                    if(indexProp != Fight_Sprites_Prop3)
                    {
                        OutfitRecord *rcd = DATAPOOL->getOutfitRecord(Prop_Type_HongZhaJi);
                        rcd->m_nNum--;
                        DATAPOOL->setOutfitRecord(Prop_Type_HongZhaJi, *rcd);
                        updatePropNum();
                    }
                    else
                    {
                        m_nUsePropType[i] = 0;
                        ((CCSprite*)_prop)->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(Icon_Prop_Random));
                        ((CCSprite*)_prop)->setPosition(ccp(getChildByTag(Fight_Sprites_Prop_Tray3)->getPositionX(),getChildByTag(Fight_Sprites_Prop_Tray3)->getPositionY()-15));
                    }
                    
                    CCSprite *plane = CCSprite::createWithSpriteFrameName(HongZhaJiRes);
                    if(arc4random()%2)  //左
                    {
                        plane->setPosition(ccp(-50, size.height/2+100));
                        plane->setFlipX(true);
                        CCMoveTo *moveto = CCMoveTo::create(HongZhaJi_Time, ccp(size.width+50, size.height/2+100));
                        CCSequence *seq = CCSequence::create(moveto,
                                                             CCCallFuncN::create(this, callfuncN_selector(FightLayer::hongZhaJiCallBack)),
                                                             NULL);
                        plane->runAction(seq);
                        addChild(plane,Fight_Zorder_Top,Fight_Sprites_HongZhaJi);
                    }
                    else   //右
                    {
                        plane->setPosition(ccp(size.width+50, size.height/2+100));
                        CCMoveTo *moveto = CCMoveTo::create(HongZhaJi_Time, ccp(-50, size.height/2+100));
                        CCSequence *seq = CCSequence::create(moveto,
                                                             CCCallFuncN::create(this, callfuncN_selector(FightLayer::hongZhaJiCallBack)),
                                                             NULL);
                        plane->runAction(seq);
                        addChild(plane,Fight_Zorder_Top,Fight_Sprites_HongZhaJi);
                    }
                    schedule(schedule_selector(FightLayer::hongZhaJiUpdate), HongZha_DeltaTime);
                }
                    break;
                case Prop_Type_KuangBaoYao:   //狂爆弹
                {
                    if(m_bUseKuangBao)
                        return;
                    else
                        m_bUseKuangBao = true;
                    if(indexProp != Fight_Sprites_Prop3)
                    {
                        OutfitRecord *rcd = DATAPOOL->getOutfitRecord(Prop_Type_KuangBaoYao);
                        rcd->m_nNum--;
                        DATAPOOL->setOutfitRecord(Prop_Type_KuangBaoYao, *rcd);
                        updatePropNum();
                    }
                    else
                    {
                        m_nUsePropType[i] = 0;
                        ((CCSprite*)_prop)->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(Icon_Prop_Random));
                        ((CCSprite*)_prop)->setPosition(ccp(getChildByTag(Fight_Sprites_Prop_Tray3)->getPositionX(),getChildByTag(Fight_Sprites_Prop_Tray3)->getPositionY()-15));
                    }
                    
                    CCSequence *seq = CCSequence::create(CCDelayTime::create(6),
                                                         CCCallFuncN::create(this, callfuncN_selector(FightLayer::kuangBaoYaoCallBack)),
                                                         NULL);
                    this->runAction(seq);
                }
                    break;
                case Prop_Type_ShuaXinQiu:  //刷新球
                {
                    if(m_bUseShuaXin)
                        return;
                    else
                        m_bUseShuaXin = true;
                    if(indexProp != Fight_Sprites_Prop3)
                    {
                        OutfitRecord *rcd = DATAPOOL->getOutfitRecord(Prop_Type_ShuaXinQiu);
                        rcd->m_nNum--;
                        DATAPOOL->setOutfitRecord(Prop_Type_ShuaXinQiu, *rcd);
                        updatePropNum();
                        int index = Fight_Sprites_Hero1;
                        for (int i = index; i<=Fight_Sprites_Hero3; ++i)
                        {
                            CCNode* _hero = getChildByTag(i);
                            if (_hero)
                            {
                                ((Hero*)_hero)->cancelCoolDown();
                            }
                        }
                    }
                    else
                    {
                        m_nUsePropType[i] = 0;
                        ((CCSprite*)_prop)->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(Icon_Prop_Random));
                        ((CCSprite*)_prop)->setPosition(ccp(getChildByTag(Fight_Sprites_Prop_Tray3)->getPositionX(),getChildByTag(Fight_Sprites_Prop_Tray3)->getPositionY()-15));
                    }
                    
                    DATAPOOL->addEffectCache(HuiFuRes);
                    CCArmature *arm = CCArmature::create("huifu");
                    arm->getAnimation()->play("huifu");
                    arm->setPosition(ccp(m_fCatapult_Org_X+ 180, m_fCatapult_Org_Y-50));
                    addChild(arm,Fight_Zorder_Top,Fight_Sprites_ShuaXin);
                    
                    CCSequence *seq = CCSequence::create(CCDelayTime::create(4),
                                                         CCCallFuncN::create(this, callfuncN_selector(FightLayer::shuaXinCallBack)),
                                                         NULL);
                    this->runAction(seq);
                }
                    break;
                default:
                    break;
            }
            updatePropNum();
        }
        indexProp++;
    }
}

void FightLayer::selectHero(cocos2d::CCPoint location)
{
    if(m_bPartner)return;
    int indexHero = Fight_Sprites_Hero1;
    for (int i = indexHero; i<Max_Game_Hero+indexHero; ++i)
    {
        if(m_bIsUseLianNu||m_bLockHero)continue;
        CCNode* _hero = getChildByTag(i);
        if (_hero)
        {
            CCPoint _point = _hero->convertToNodeSpace(location);
            if (_hero&&!((Hero*)_hero)->isCoolDown()&&((Hero*)_hero)->getSprite()->boundingBox().containsPoint(_point))
            {
                Hero* _bullet = (Hero*)getChildByTag(Fight_Sprites_Bullet);
                int type = ((Hero*)_hero)->getType();
                char buf[20];
                memset(buf, 0, 20);
                sprintf(buf, "Sound/hero/%d.ogg",type);
                DATAPOOL->playGameEffect(buf);
                if (_bullet)
                {
                    this->removeChild(_bullet, true);
                }
                
                Hero *_movebullet = (Hero*)getChildByTag(Fight_Sprites_MoveToCatapult);
                if(_movebullet)
                {
                    _movebullet->removeFromParentAndCleanup(true);
                }
                Game_Hero_Data *_data = ((Hero*)_hero)->getData();
                Hero* _newbullet = new Hero(_data);
                _newbullet->autorelease();
                _newbullet->setPosition(m_fCatapult_Org_X,m_fBullet_Org_Y);
                _newbullet->setState(Hero_State_Ready);
                addChild(_newbullet,Fight_Zorder_Bullet,Fight_Sprites_Bullet);
            }
            
            if (((Hero*)_hero)->isCoolDown()&&((Hero*)_hero)->getSprite()->boundingBox().containsPoint(_point))
            {
                DATAPOOL->playGameEffect(Moveup);
            }
        }
    }

}

void FightLayer::useSecondLansquenet()
{
    for (int i = m_pBarrier->count()-1; i>=0; i--)
    {
        Lansquenet *lans = (Lansquenet*)m_pBarrier->objectAtIndex(i);
        lans->removeFromParentAndCleanup(true);
    }
    m_pBarrier->removeAllObjects();
    
    m_nCurrentBarrier = Second_Barrier;
    int index = Fight_Sprites_Barrier1;
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    int width = size.width/Max_Game_Lans;
    for (int i = 0; i<Max_Game_Lans; ++i)
    {
        if(m_nUseLansTypeTwo[i]!=0)  //使用雇佣兵
        {
            Enemy_Data *data = DATAPOOL->getEnemyData(m_nUseLansTypeTwo[i]);
            Lansquenet *lans = new Lansquenet(data,this);
            lans->autorelease();
            lans->setPosition(ccp(width/2+width*i, Barrier_Y));
            addChild(lans,Fight_Zorder_Barrier,index);
            m_nBarrierTotalHp_Second += data->mHp;
            data = NULL;
            m_pBarrier->addObject(lans);
        }
        else   //未使用 使用普通栅栏
        {
            int id = DATAPOOL->getZhanLanFromBag();
            CCLOG("id:%d",id);
            Lansquenet *lans = NULL;
            if(id==0)
            {
                lans = new Lansquenet(ZhaLanRes,this);
                lans->autorelease();
                m_nBarrierTotalHp_Second += Barrier_HP;
            }
            else  //买的栅栏道具
            {
                if(id == Prop_Type_ShiZhaLan)
                {
                    lans = new Lansquenet(ShiZhaLanRes,this);
                    lans->autorelease();
                }
                else   //铁栅栏
                {
                    lans = new Lansquenet(TieZhaLanRes,this);
                    lans->autorelease();
                }
                //                lans = new Lansquenet(DATAPOOL->getPropData(id)->m_sourceName,this);
                CCLOG("barrierHP:%d",DATAPOOL->getPropData(id)->mHp);
                m_nBarrierTotalHp_Second += DATAPOOL->getPropData(id)->mHp;
            }
            lans->setPosition(ccp(width/2+width*i, Barrier_Y));
            addChild(lans,Fight_Zorder_Barrier,index);
            
            m_pBarrier->addObject(lans);
        }
        index++;
    }
    m_nBarrierCurrentHp_Second = m_nBarrierTotalHp_Second;
    CCLOG("m_nBarrierTotalHp_Second:%d",m_nBarrierTotalHp_Second);
    CCSprite* progressHp = (CCSprite*)getChildByTag(Fight_Sprites_LansHp_Progress);
    CCProgressTimer *barHp = (CCProgressTimer*)getChildByTag(Fight_Sprites_LansHp_Progress_Bar);
    
    float percent = (float)m_nBarrierCurrentHp_Second / (float)m_nBarrierTotalHp_Second;
    barHp->setPercentage(percent*100);
    
    CCSequence *seq = CCSequence::create(CCDelayTime::create(5),
                                         CCFadeOut::create(2),
                                         NULL);
    
    progressHp->runAction(seq);
    barHp->runAction((CCSequence*)(seq->copy()));
}

void FightLayer::playLoseResult()
{
    unschedule(schedule_selector(FightLayer::waveUpdate));

    
//    int type = 1;
//    if(m_eType==Game_Type_PVP)
//    {
//        type = 0;
//    }
    m_bCanPullCatapult = false;
    CCSequence *seq = CCSequence::create(CCDelayTime::create(ShowResultDelay),
                                         CCCallFuncN::create(this, callfuncN_selector(FightLayer::showLoseResultCallBack)),
                                         NULL);
    this->runAction(seq);
}

void FightLayer::checkLansSkill()
{
    CCArmature *ship = (CCArmature*)getChildByTag(Fight_Sprites_Ship);
    if(ship)
    {
        for (int i=0; i<m_pEnemy->count(); i++)
        {
            Enemy *enemy = (Enemy*)m_pEnemy->objectAtIndex(i);
            if(enemy&&enemy->checkCollisionArm(ship,2))
            {
                EnemyStopState *_instance = EnemyStopState::getInstance();
                float time = 4 + DATAPOOL->getEnemyRecord(Enemy_Type_bBoss)->m_nLevel*0.5;
                _instance->setTime(time, Stop_Type_None);
                enemy->GetFSMSystem()->ChangeState(_instance);
                ship->stopAllActions();
                ship->removeFromParentAndCleanup(true);
                DATAPOOL->removeEffectCache(ChuanRes);
                break;
            }
        }
    }
    
    CCSprite *ice = (CCSprite*)getChildByTag(Fight_Sprites_Ice);
    if(ice)
    {
        for (int i=m_pEnemy->count()-1; i>=0; i--)
        {
            Enemy *enemy = (Enemy*)m_pEnemy->objectAtIndex(i);
            CCRect rect = ice->boundingBox();
            float x = rect.getMinX();
            float y = rect.getMinY();
            float width = rect.getMaxX() - x;
            float height = rect.getMaxY() - y;
            CCRect validRect = CCRectMake(x, y, width, height - 50); //缩小检测区域
            if(enemy&&validRect.containsPoint(enemy->getPosition())&&enemy->getState()==Enemy_State_Move)
            {
                EnemySlowDownState *_instance = EnemySlowDownState::getInstance();
                float time = 4 +  0.5*DATAPOOL->getEnemyRecord(Enemy_Type_dBoss)->m_nLevel;
                _instance->setTime(time);
                enemy->GetFSMSystem()->ChangeState(_instance);
            }
        }
    }
    
    CCArmature *hole = (CCArmature*)getChildByTag(Fight_Sprites_Hole);
    if(hole)
    {
        for (int i=m_pEnemy->count()-1; i>=0; i--)
        {
            Enemy *enemy = (Enemy*)m_pEnemy->objectAtIndex(i);
            if(enemy&&!enemy->isBoss()&&hole->boundingBox().containsPoint(enemy->getPosition()))
            {
                m_pEnemy->removeObject(enemy);
                CCSpawn *act = CCSpawn::create(CCScaleTo::create(1, 0.2),
                                               CCRepeatForever::create(CCRotateBy::create(0.5, 360)),
                                               NULL);
                CCSequence *seq = CCSequence::create(act,
                                                     CCCallFuncN::create(this, callfuncN_selector(FightLayer::removeEnemyCallBack)),
                                                     NULL);
                enemy->runAction(seq);
            }
        }
    }
    
    CCArmature *fireball = (CCArmature*)getChildByTag(Fight_Sprites_FireBall);
    if(fireball)
    {
        for (int i=0; i<m_pEnemy->count(); i++)
        {
            Enemy *enemy = (Enemy*)m_pEnemy->objectAtIndex(i);
            if(enemy&&enemy->checkCollisionArm(fireball))
            {
                EnemyHurtState *_instance = EnemyHurtState::getInstance();
                enemy->GetFSMSystem()->ChangeState(_instance);
                int atk = DATAPOOL->getEnemyData(Enemy_Type_fBoss)->mAtk2*2 + (DATAPOOL->getEnemyData(Enemy_Type_fBoss)->mAtkAdd)*(DATAPOOL->getEnemyRecord(Enemy_Type_fBoss)->m_nLevel);
                enemy->minusBlood(atk,false);
                fireball->stopAllActions();
                fireball->removeFromParentAndCleanup(true);
                DATAPOOL->removeEffectCache(HuoQiuRes);
                break;
            }
        }
    }
    
    CCSprite *yinfu = (CCSprite*)getChildByTag(Fight_Sprites_Tornado);
    if(yinfu)
    {
        for (int i = 0; i<m_pEnemy->count(); i++)
        {
            Enemy *_enemy = (Enemy*)m_pEnemy->objectAtIndex(i);
            if(_enemy&&CCRectMake(yinfu->getPositionX(), yinfu->getPositionY(), 300, 200).containsPoint(_enemy->getPosition()))
            {
                EnemyStopState *_instance = EnemyStopState::getInstance();
                float time = 5 + 0.5*DATAPOOL->getEnemyRecord(Enemy_Type_cBoss)->m_nLevel;
                _instance->setTime(time, Stop_Type_Sleep);
                _enemy->GetFSMSystem()->ChangeState(_instance);
                break;
            }
        }
    }
    
    CCSprite *chongjibo = (CCSprite*)getChildByTag(Fight_Sprites_ChongJiBo);
    if(chongjibo)
    {
        for (int i = 0; i<m_pEnemy->count(); i++)
        {
            Enemy *_enemy = (Enemy*)m_pEnemy->objectAtIndex(i);
            if(_enemy&&_enemy->checkCollision(chongjibo))
            {
                CCMoveBy *move = CCMoveBy::create(0.2, ccp(0, 20));
                _enemy->runAction(move);
            }
        }
    }
}

void FightLayer::updateGemNum()
{
    CCLabelBMFont *_labelGem = (CCLabelBMFont*)getChildByTag(Fight_Sprites_Gem_Num);
    if (_labelGem)
    {
        char buf[10];
        memset(buf, 0, 10);
        sprintf(buf, "%07d",xUser->pUserInfo->gemstone);
        _labelGem->setString(buf);
    }
}

void FightLayer::cleanArmatureDataCache()
{
    cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(ChuxianRes);
    cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(XiaoShiRes);
    cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(RanShaoRes);
    cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(BaoZhaRes);
    
    if(m_eType==Game_Type_PVP||(m_eType != Game_Type_Boss&&m_nCurrentChapter==Charpter_1))
    {
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(GeBuLinRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(JuXingGeBuLinRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(FengXiYiRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(RuanNIGuaiRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(ABossRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(BBossRes);
    }
    
    if(m_eType==Game_Type_PVP||(m_eType != Game_Type_Boss&&m_nCurrentChapter==Charpter_2))
    {
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(GeBuLinRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(RuanNIGuaiRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(JuXingGeBuLinRes);
        
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(XueYuGangXieRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(ShuiJingZhangyuRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(ChangMaoXueGuaiRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(BingBaoHeTunRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(CBossRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(DBossRes);
    }
    
    if(m_eType==Game_Type_PVP||(m_eType != Game_Type_Boss&&m_nCurrentChapter==Charpter_3))
    {
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(GeBuLinRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(JuXingGeBuLinRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(FengXiYiRes);
        
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(ShaMoJiaChongRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(HuangMoGeBuLinRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(ShaMoFeiLongRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(ShaBaoJuRenRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(EBossRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(FBossRes);
    }
    
    if(m_eType==Game_Type_PVP||(m_eType != Game_Type_Boss&&m_nCurrentChapter==Charpter_4))
    {
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(GeBuLinRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(JuXingGeBuLinRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(FengXiYiRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(RuanNIGuaiRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(ShaMoFeiLongRes);
        
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(XiaoGuiRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(DiYuMoMengRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(DiYuFengQuanRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(LingHunShouGeZheRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(GBossRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(HBossRes);
    }
    
    
    if(m_eType==Game_Type_PVP||(m_eType != Game_Type_Boss&&m_nCurrentChapter==Charpter_5))
    {
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(GeBuLinRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(HuangMoGeBuLinRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(XiaoGuiRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(FengXiYiRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(JuXingGeBuLinRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(RuanNIGuaiRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(ShaMoFeiLongRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(ChangMaoXueGuaiRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(DiYuFengQuanRes);
        
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(JiaoTangKuiLeiRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(YouEMoRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(TangXinMoRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(HeiTangEMoRes);
        cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(IBossRes);
    }
    
    if(m_eType == Game_Type_Boss&&m_nBossType!=0)
    {
        Enemy_Data* enemyData = DATAPOOL->getEnemyData(m_nBossType);
        char buf[40] = {0};
        sprintf(buf, "Sprites/%s.ExportJson",enemyData->mSourceName);
        DATAPOOL->removeEffectCache(buf);
        DATAPOOL->removeEffectCache(GeBuLinRes);
    }
    
    for (int i = 0; i<Max_Game_Lans; ++i)
    {
        if(m_nUseLansTypeOne[i]!=0)  //使用雇佣兵
        {
            Enemy_Data *data = DATAPOOL->getEnemyData(m_nUseLansTypeOne[i]);
            char buf[30] = {0};
            sprintf(buf, "Sprites/%s_g.ExportJson",data->mSourceName);
            DATAPOOL->removeEffectCache(buf);
        }
        
        if(m_nUseLansTypeTwo[i]!=0)
        {
            Enemy_Data *data = DATAPOOL->getEnemyData(m_nUseLansTypeTwo[i]);
            char buf[30] = {0};
            sprintf(buf, "Sprites/%s_g.ExportJson",data->mSourceName);
            DATAPOOL->removeEffectCache(buf);
        }
    }
}

void FightLayer::createQiangBaoGuai()
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    DATAPOOL->addEffectCache(QiangBaoGuaiRes);
    if(!m_pQiangbaoGuai)
    {
        if(false == DATAPOOL->m_bTeachStepOver[TEACH_STEP_QIANGBAO])
        {
            CCSequence *seq = CCSequence::create(CCDelayTime::create(1.5),
                                                CCCallFunc::create(this, callfunc_selector(FightLayer::startQiangBaoTeachCallBack)),
                                                NULL);
            this->runAction(seq);
        }
        m_pQiangbaoGuai = CCArmature::create("qiangbaoguai");
        m_pQiangbaoGuai->getAnimation()->play("qiangbaoguai");
        addChild(m_pQiangbaoGuai,Fight_Zorder_Enemy);
    
        int dy  = arc4random()%300 - 150;
        
        if(arc4random()%2)  //左
        {
            m_pQiangbaoGuai->setScaleX(-1);
            m_pQiangbaoGuai->setPosition(ccp(-50, size.height/2+dy));
            CCMoveTo *moveto = CCMoveTo::create(HongZhaJi_Time, ccp(size.width+50, size.height/2+dy));
            CCSequence *seq = CCSequence::create(moveto,
                                                 CCCallFuncN::create(this, callfuncN_selector(FightLayer::qiangbaoguaiCallBack)),
                                                 NULL);
            seq->setTag(101);
            m_pQiangbaoGuai->runAction(seq);
        }
        else   //右
        {
            m_pQiangbaoGuai->setPosition(ccp(size.width+50, size.height/2+dy));
            CCMoveTo *moveto = CCMoveTo::create(HongZhaJi_Time, ccp(-50, size.height/2+dy));
            CCSequence *seq = CCSequence::create(moveto,
                                                 CCCallFuncN::create(this, callfuncN_selector(FightLayer::qiangbaoguaiCallBack)),
                                                 NULL);
            seq->setTag(101);
            m_pQiangbaoGuai->runAction(seq);
        }
    }
}

bool FightLayer::checkQiangBaoGuaiCollision(Hero **_bullet)
{
    bool res = false;
    if((*_bullet)->getType()==Hero_Type_BoLuo&&(*_bullet)->isPowerState())return false;
    if(*_bullet&&m_pQiangbaoGuai)
    {
        CCPoint _point = (*_bullet)->getPosition();
        float width = (*_bullet)->getSprite()->getContentSize().width/2;
        float height = (*_bullet)->getSprite()->getContentSize().height/2;
//        CCLOG("width:%f",width);
//        CCLOG("height:%f",height);
        if (m_pQiangbaoGuai->boundingBox().intersectsRect(CCRectMake(_point.x, _point.y, width, height)))  //大小调试
        {
            res = true;
        }
        
        if(res)
        {
            int _type = (*_bullet)->getType();
            if(!m_bHitComb)
            {
                m_nHitCount++;
                m_bHitComb = true;
            }
            {    //击中音效和特效
                DATAPOOL->playGameEffect(Jizhong);
                if(_type==Hero_Type_XiangJiao&&(*_bullet)->isPowerState())
                {
                    createEffect(XiangJiaoARes,"xiangjiao_a", (*_bullet)->getPosition(), Fight_Zorder_Bullet,this);
                }
                else if(_type!=Hero_Type_XiGua)
                {
                    createEffect(HuoHuaRes,"huohua", HelpMethod::getCenterPoint((*_bullet), m_pQiangbaoGuai), Fight_Zorder_Bullet);
                }
            }
            if(_type!=Hero_Type_TaoZi&&_type!=Hero_Type_Lizi)
            {
                removeBullet(*_bullet);
//                CC_SAFE_RELEASE_NULL(*_bullet);
            }
            else if(_type==Hero_Type_Lizi)
            {
                (*_bullet)->reverseX();
            }
            
            m_pQiangbaoGuai->stopAllActions();
            m_pQiangbaoGuai->removeFromParentAndCleanup(true);
            m_pQiangbaoGuai = NULL;
            DATAPOOL->removeEffectCache(QiangBaoGuaiRes);
            
            CCSequence *seq = CCSequence::create(CCDelayTime::create(1),
                                                 CCCallFuncN::create(this, callfuncN_selector(FightLayer::showQiangBaoCallBack)),
                                                 NULL);
            this->runAction(seq);
        }
    }
    return res;
}

void FightLayer::checkSpeak(float dt)
{
    m_speakTime.mCurTime += dt;
    if(m_speakTime.mCurTime>=m_speakTime.mTotalTime)
    {
        m_speakTime.mCurTime = 0;
        enemySpeak();
    }
}

void FightLayer::enemySpeak()
{
    if(m_pEnemy->count()>Max_Enemy/2)
    {
        int count = arc4random()%2;
        count+=2;
        for (int i=0; i<count; i++)
        {
            int index = arc4random()%(m_pEnemy->count());
            Enemy *enemy = (Enemy*)m_pEnemy->objectAtIndex(index);
            enemy->speakInitiative();
        }
    }
}

void FightLayer::updateCombo(CCPoint pos)
{
    if(m_bEnergy)
    {
        m_nCombo=0;
        return;
    }
    if(m_nCombo>=3)
    {
        CCLOG("combo:%d",m_nCombo);
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        
        
        char buf[20] = {0};
        sprintf(buf, "X%d",m_nCombo);
        CCLabelBMFont *combNum = NULL;
        CCSprite *comboTxt = NULL;
        combNum = (CCLabelBMFont*)getChildByTag(Fight_Sprites_Combo_Txt);
        comboTxt = (CCSprite*)getChildByTag(Fight_Sprites_Combo_Num);
        if(!combNum&&!comboTxt)
        {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        	combNum= CCLabelBMFont::create(buf,  "num_combo.fnt");
#else
        	combNum= CCLabelBMFont::create(buf,  "fonts/num_combo.fnt");
#endif
            combNum->setScale(0.7f);
            combNum->setPosition(ccp(size.width-combNum->getContentSize().width/2 - 20, size.height/2 + 100));
            addChild(combNum,Fight_Zorder_Top,Fight_Sprites_Combo_Txt);
            
            comboTxt= CCSprite::createWithSpriteFrameName(ComBo_Txt);
            comboTxt->setPosition(ccp(combNum->getPositionX() - combNum->getContentSize().width/2 - comboTxt->getContentSize().width/2 - 2, combNum->getPositionY()));
            addChild(comboTxt,Fight_Zorder_Top,Fight_Sprites_Combo_Num);
            
            
            CCSequence *seq = CCSequence::create(CCDelayTime::create(1),
                                                 CCCallFuncN::create(this, callfuncN_selector(FightLayer::removeBMFontCallBack)),
                                                 NULL);
            
            CCSequence *seq1 = CCSequence::create(CCDelayTime::create(1),
                                                  CCCallFuncN::create(this, callfuncN_selector(FightLayer::removeCallBack_Sprite)),
                                                  NULL);
            seq->setTag(1);
            seq1->setTag(1);
            combNum->runAction(seq);
            comboTxt->runAction(seq1);
        }
        else if(combNum&&comboTxt)
        {
            CCAction *act = combNum->getActionByTag(1);
            CCAction *act1 = comboTxt->getActionByTag(1);
            if(act&&act1)
            {
                combNum->stopActionByTag(1);
                comboTxt->stopActionByTag(1);
                CCSequence *seq = CCSequence::create(CCDelayTime::create(1),
                                                     CCCallFuncN::create(this, callfuncN_selector(FightLayer::removeBMFontCallBack)),
                                                     NULL);
                
                CCSequence *seq1 = CCSequence::create(CCDelayTime::create(1),
                                                      CCCallFuncN::create(this, callfuncN_selector(FightLayer::removeCallBack_Sprite)),
                                                      NULL);
                seq->setTag(1);
                seq1->setTag(1);
                combNum->runAction(seq);
                comboTxt->runAction(seq1);
                combNum->setString(buf);
            }
        }
    
        updateEnergy(pos);
        
    }
}

void FightLayer::removeBMFontCallBack(cocos2d::CCNode *pSender)
{
    CCLabelBMFont *label = (CCLabelBMFont*)pSender;
    if(label)
        label->removeFromParentAndCleanup(true);
}

void FightLayer::removeEnergyParticleCallBack(cocos2d::CCNode *pSender)
{
    CCParticleSystem *emitter = (CCParticleSystem*)pSender;
    if(emitter)
        emitter->removeFromParentAndCleanup(true);
}

void FightLayer::updateEnergy(CCPoint pos)
{
    CCParticleSystem *emitter = NULL;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    emitter = CCParticleSystemQuad::create("soul.plist");
#else
    emitter = CCParticleSystemQuad::create("particle/soul.plist");
#endif

    emitter->setPosition(pos);
    emitter->setAutoRemoveOnFinish(true);
    addChild(emitter,Fight_Zorder_Top);
    CCSprite *frame = (CCSprite*)getChildByTag(Fight_Sprites_Energy_Frame);
    CCSequence *seq = CCSequence::create(CCMoveBy::create(0.2, ccp(0, 30)),
                                         CCMoveTo::create(0.4, ccp(frame->getPositionX(),frame->getPositionY()-frame->getContentSize().height/2)),
                                         CCCallFuncN::create(this, callfuncN_selector(FightLayer::removeEnergyParticleCallBack)),
                                         NULL);
    emitter->runAction(seq);
    DATAPOOL->playGameEffect(Sound_Energy);
    
    m_nEnergy+=(float)(m_nCombo-1)*ENERGY*(1+(float)m_nCombo/10);
    DATAPOOL->energy = m_nEnergy;
    if(m_nEnergy>=Max_Energy)
    {
        m_nEnergy = Max_Energy;
        m_bEnergy = true;
        CCSprite *energyFrame = (CCSprite*)getChildByTag(Fight_Sprites_Energy_Frame);
        CCParticleSystem *emitter = NULL;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        emitter = CCParticleSystemQuad::create("energy.plist");
#else
        emitter = CCParticleSystemQuad::create("particle/energy.plist");
#endif
        emitter->setPosition(ccp(energyFrame->getPositionX(),energyFrame->getPositionY()-energyFrame->getContentSize().height/2+20));
        emitter->setAutoRemoveOnFinish(true);
        addChild(emitter,0,Fight_Sprites_Energy_Particle);
        CCSequence *seq1 = CCSequence::create(CCDelayTime::create(8),
                                             CCCallFuncN::create(this, callfuncN_selector(FightLayer::removeEnergyParticleCleanStateCallBack)),
                                             NULL);
        emitter->runAction(seq1);
        starEnergyState();
        CCProgressTimer *bar = (CCProgressTimer*)getChildByTag(Fight_Sprites_Energy_Bar);
        bar->setPercentage(100);
        CCProgressFromTo *progressFromTo = CCProgressFromTo::create(8, 100, 0);
        bar->runAction(progressFromTo);
    }
    else
    {
        CCProgressTimer *bar = (CCProgressTimer*)getChildByTag(Fight_Sprites_Energy_Bar);
        bar->setPercentage(( m_nEnergy/(float)Max_Energy) *100 );
    }
}

void FightLayer::removeEnergyParticleCleanStateCallBack(cocos2d::CCNode *pSender)
{
    CCParticleSystem *emitter = (CCParticleSystem*)pSender;
    if(emitter)
    {
        emitter->removeFromParentAndCleanup(true);
    }
    m_bEnergy = false;
    m_bUseKuangBao = false;
    endEnergyState();
}

void FightLayer::starEnergyState()
{
    DATAPOOL->playGameEffect(Sound_Angry);
    CCSprite *energyFrame = (CCSprite*)getChildByTag(Fight_Sprites_Energy_Frame);
    CCSprite *energyTxt = CCSprite::createWithSpriteFrameName(NuQi_Txt);
    energyTxt->setPosition(ccp(energyFrame->getPositionX(), energyFrame->getPositionY()-energyFrame->getContentSize().height/2+30));
    addChild(energyTxt,Fight_Zorder_Top,Fight_Sprites_Energy_Txt);
    
    CCSequence *seq = CCSequence::create(CCScaleTo::create(0.1, 1.5),
                                         CCScaleTo::create(0.1, 1),
                                         CCBlink::create(0.3, 100),
                                         NULL);
    energyTxt->runAction(seq);
    
    m_bUseKuangBao = true;
    
}

void FightLayer::endEnergyState()
{
    m_nEnergy = 0;
    DATAPOOL->energy = m_nEnergy;
    CCProgressTimer *bar = (CCProgressTimer*)getChildByTag(Fight_Sprites_Energy_Bar);
    CCSprite *energyTxt = (CCSprite*)getChildByTag(Fight_Sprites_Energy_Txt);
    if(energyTxt)
        energyTxt->removeFromParentAndCleanup(true);
    bar->setPercentage(0);
}

void FightLayer::createPartnerAni()
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    CCParticleSystem *ball = NULL;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    ball = CCParticleSystemQuad::create("soul.plist");
#else
    ball = CCParticleSystemQuad::create("particle/soul.plist");
#endif
    ball->setPosition(ccp(size.width/2,size.height));
    ball->setAutoRemoveOnFinish(true);
    addChild(ball,Fight_Zorder_Top);
    CCSequence *seq = CCSequence::create(CCMoveTo::create(0.3, ccp(m_fCatapult_Org_X, m_fCatapult_Org_Y)),
                                         CCCallFuncN::create(this, callfuncN_selector(FightLayer::partnerShowCallBack)),
                                         NULL);
    ball->runAction(seq);
    
    CCSprite *txt = CCSprite::createWithSpriteFrameName(YXGL_Txt);
    txt->setPosition(ccp(size.width/2, size.height/2));
    txt->setScale(0.2);
    addChild(txt,Fight_Zorder_Top);
    CCSequence *seq1 = CCSequence::create(CCScaleTo::create(0.2, 1.2),
                                          CCDelayTime::create(1),
                                          CCCallFuncN::create(this, callfuncN_selector(FightLayer::removeCallBack_Sprite)),
                                          NULL);
    txt->runAction(seq1);
    
    CCSequence *seq2 = CCSequence::create(CCDelayTime::create(0.3),
                                          CCShake::create(0.2, 15),
                                          NULL);
    this->runAction(seq2);
}

void FightLayer::partnerShowCallBack(cocos2d::CCNode *pSender)
{
    CCParticleSystem *ball = (CCParticleSystem*)pSender;
    m_bShowPartner = false;
    if(ball)
        ball->removeFromParentAndCleanup(true);
    if(m_nCurrentLevel==4)
    {
        m_bUseKuangBao = true;
        m_bPartner = true;
        Hero* bullet = (Hero*)getChildByTag(Fight_Sprites_Bullet);
        if(bullet)bullet->removeFromParentAndCleanup(true);
        Game_Hero_Data *data = DATAPOOL->getHeroData(Hero_Type_Lizi);
        Hero *hero = new Hero(data);
        hero->autorelease();
        hero->setPosition(ccp(m_fCatapult_Org_X,m_fCatapult_Org_Y));
        addChild(hero);
        hero->setState(Hero_State_Ready);
        hero->setTag(Fight_Sprites_Bullet);
        hero->setZOrder(Fight_Zorder_Bullet);
        
        
    }
    else if (m_nCurrentLevel==9)
    {
        m_bUseKuangBao = true;
        m_bPartner = true;
        Hero* bullet = (Hero*)getChildByTag(Fight_Sprites_Bullet);
        if(bullet)bullet->removeFromParentAndCleanup(true);
        Game_Hero_Data *data = DATAPOOL->getHeroData(Hero_Type_ShiLiu);
        Hero *hero = new Hero(data);
        hero->autorelease();
        hero->setPosition(ccp(m_fCatapult_Org_X,m_fCatapult_Org_Y));
        addChild(hero);
        hero->setState(Hero_State_Ready);
        hero->setTag(Fight_Sprites_Bullet);
        hero->setZOrder(Fight_Zorder_Bullet);
        
    }
}

void FightLayer::checkCollision(float dt)
{
    checkMuBei();
    checkShiBei();
    if(m_pBullet->count()>0)
    {
        for (int i = m_pBullet->count()-1; i>=0; i--)
        {
            Hero *_bullet = (Hero*)(m_pBullet->objectAtIndex(i));
            if (!_bullet->moveUpdate(dt))
            {
                if(!m_bUseNewHero_Teach&&_bullet->getType()==Hero_Type_TaoZi)
                {
                    DATAPOOL->m_bTeachStepOver[TEACH_STEP_SELECT_HERO_G] = true;
                }
                if(!_bullet->getHit())
                    m_nCombo = 0;
                removeBullet(_bullet);
            }
            else    //未出界  检查是否碰撞
            {
                checkSpriteCollision(&_bullet);
                
            }
        }
    }
    for (int i = m_pBullet->count()-1; i>=0; i--)
    {
        Hero *_bullet = (Hero*)(m_pBullet->objectAtIndex(i));
        checkBarrierCollision(_bullet);
    }
}

bool FightLayer::collisionMuBei(Hero *bullet,Enemy *enemy)
{
    bool res = false;
    for(int j = m_pMuBei->count()-1;j>=0;j--)
    {
        CCArmature *mubei = (CCArmature*)m_pMuBei->objectAtIndex(j);
        CCRect rect = mubei->boundingBox();
        float x = rect.getMinX();
        float y = rect.getMinY();
        float width = rect.getMaxX() - x;
        float height = rect.getMaxY() - y;
        CCRect validRect = CCRectMake(x, y, width, height+30);
        if(mubei&&validRect.intersectsRect(enemy->boundingBox())&&mubei->getPositionY()-enemy->getPositionY()<30)
        {
            res = true;
            break;
        }
    }
    return res;
}

bool FightLayer::collisionShiBei(Hero *bullet,Enemy *enemy)
{
    bool res = false;
    for(int j = m_pShiBei->count()-1;j>=0;j--)
    {
        CCArmature *shibei = (CCArmature*)m_pShiBei->objectAtIndex(j);
        
        if(shibei&&shibei->boundingBox().intersectsRect(enemy->boundingBox())&&shibei->getPositionY()-enemy->getPositionY()<30)
        {
            res = true;
            break;
        }
    }
    return res;
}

void FightLayer::getTotalWave()
{
    if(!m_pLevelData)
        m_nTotalWave = 0;
    int wave_count = m_pLevelData->nWaveCount;
    for (int i=0; i<wave_count; i++){
        
        int element_count = m_pLevelData->mArrayWave[i]->mElementCount;
        for (int j = 0; j<element_count; j++){
            
            int type =   m_pLevelData->mArrayWave[i]->mArrayElement[j]->mType;
            if(isBoss(type+First_Enemy_ID-1))
            {
                m_nTotalWave = i+1;
                return;
            }
        }
        m_nTotalWave = i + 1;
    }
}

bool FightLayer::isBoss(int type)
{
    if(type == Enemy_Type_aBoss||
       type == Enemy_Type_bBoss||
       type == Enemy_Type_cBoss||
       type == Enemy_Type_dBoss||
       type == Enemy_Type_eBoss||
       type == Enemy_Type_fBoss||
       type == Enemy_Type_gBoss||
       type == Enemy_Type_hBoss||
       type == Enemy_Type_iBoss||
       type == Enemy_Type_jBoss)
        return true;
    
    return false;
}

void FightLayer::checkTeachLayer(float dt)
{
    //新手引导
        if(1==getLevelNum()&&m_eType==Game_Type_Level)
        {
            if(!m_bStartingFire_Teach&&false == DATAPOOL->m_bTeachStepOver[TEACH_STEP_USE_DANGONG1])
            {
                m_bDanGongFire_Teach = false;
                CCSize size = CCDirector::sharedDirector()->getWinSize();
                CTeachLayer *teachLayer = NULL;
                teachLayer = (CTeachLayer*)getChildByTag(Fight_Sprites_TeachLayer);
                if(!teachLayer)
                {
                    teachLayer = CTeachLayer::create();
                    teachLayer->setContentSize(size);
                    teachLayer->setFocusProp(ccp(size.width/2, Barrier_Y), 100);
                    teachLayer->setPicInfo(ccp(size.width/2, Barrier_Y+200), "teach_txt_3.png");
                    teachLayer->setCallbackFun(this, callfunc_selector(FightLayer::dangongFireCallBack));
                    addChild(teachLayer, Fight_Zorder_Top,Fight_Sprites_TeachLayer);
                }
            }
            
            if(m_bDanGongFire_Teach&&!m_bStartingPowerFire_Teach&&false == DATAPOOL->m_bTeachStepOver[TEACH_STEP_USE_DANGONG2])
            {
                m_bDanGongPowerFire_Teach = false;
                CCSize size = CCDirector::sharedDirector()->getWinSize();
                CTeachLayer *teachLayer = NULL;
                teachLayer = (CTeachLayer*)getChildByTag(Fight_Sprites_TeachLayer);
                if(!teachLayer)
                {
                    teachLayer = CTeachLayer::create();
                    teachLayer->setContentSize(size);
                    teachLayer->setFocusProp(ccp(size.width/2, Barrier_Y), 100);
                    teachLayer->setPicInfo(ccp(size.width/2, size.height/2+100), "teach_txt_4.png");
                    teachLayer->setCallbackFun(this, callfunc_selector(FightLayer::dangongPowerFireCallBack));
                    addChild(teachLayer, Fight_Zorder_Top,Fight_Sprites_TeachLayer);
                }
            }
        }
        
        if(2==getLevelNum()&&m_eType==Game_Type_Level)
        {
            int indexHero = Fight_Sprites_Hero1;
            CCNode *_hero = getChildByTag(indexHero);
            if(!m_bStartingUseNewHero_Teach&&false == DATAPOOL->m_bTeachStepOver[TEACH_STEP_SELECT_HERO_G]&&_hero)
            {
                m_bUseNewHero_Teach = false;
                CCSize size = CCDirector::sharedDirector()->getWinSize();
                CTeachLayer *teachLayer = NULL;
                teachLayer = (CTeachLayer*)getChildByTag(Fight_Sprites_TeachLayer);
                if(!teachLayer)
                {
                    teachLayer = CTeachLayer::create();
                    teachLayer->setContentSize(size);
                    teachLayer->setFocusProp(_hero->getPosition(), 80);
                    teachLayer->setPicInfo(ccp(size.width/2, m_fCatapult_Org_Y+200),"teach_txt_11.png");
                    teachLayer->setCallbackFun(this, callfunc_selector(FightLayer::newHeroCallBack));
                    addChild(teachLayer, Fight_Zorder_Top,Fight_Sprites_TeachLayer);
                }
            }
            
            int type =  m_nUsePropType[0];
            int enemyCount =  m_pEnemy->count();
            if(m_bUseNewHero_Teach&&!m_bStartingHongZhaJi_Teach&&false == DATAPOOL->m_bTeachStepOver[TEACH_STEP_USE_HONGZHAJI]&& type== Prop_Type_HongZhaJi&&enemyCount>5)
            {
                m_bUseHongZhaji_Teach = false;
                CCSize size = CCDirector::sharedDirector()->getWinSize();
                CTeachLayer *teachLayer = NULL;
                teachLayer = (CTeachLayer*)getChildByTag(Fight_Sprites_TeachLayer);
                if(!teachLayer)
                {
                    teachLayer = CTeachLayer::create();
                    teachLayer->setContentSize(size);
                    CCSprite *tray1 = (CCSprite*)getChildByTag(Fight_Sprites_Prop_Tray1);
                    teachLayer->setFocusProp(tray1->getPosition(), 70);
                    teachLayer->setPicInfo(ccp(size.width/2, tray1->getPositionY()+200), "teach_txt_10.png");
                    teachLayer->setCallbackFun(this, callfunc_selector(FightLayer::hongzhajiCallBack));
                    addChild(teachLayer, Fight_Zorder_Top,Fight_Sprites_TeachLayer);
                }
            }
            
        }
        
        if(m_eType == Game_Type_Level&&m_bStartQiangBao_Teach)
        {
            CCSize size = CCDirector::sharedDirector()->getWinSize();
            CTeachLayer *teachLayer = NULL;
            teachLayer = (CTeachLayer*)getChildByTag(Fight_Sprites_TeachLayer);
            if(!teachLayer)
            {
                m_pQiangbaoGuai->pauseSchedulerAndActions();
                teachLayer = CTeachLayer::create();
                teachLayer->setContentSize(size);
                teachLayer->setFocusProp(ccp(m_pQiangbaoGuai->getPositionX(), m_pQiangbaoGuai->getPositionY()+20), 100);
                teachLayer->setPicInfo(ccp(size.width/2, m_pQiangbaoGuai->getPositionY()+200), "teach_txt_21.png");
                teachLayer->setCallbackFun(this, callfunc_selector(FightLayer::qiangBaoCallBack));
                addChild(teachLayer, Fight_Zorder_Top,Fight_Sprites_TeachLayer);
            }
        }
}

void FightLayer::checkPeel(float dt)
{
    for (int i=m_pPeel->count()-1; i>=0; i--)
    {
        CCSprite *peel = (CCSprite*)m_pPeel->objectAtIndex(i);
        if(checkPeelCollision(peel))
        {
            peel->removeFromParentAndCleanup(true);
            m_pPeel->removeObject(peel);
            break;
        }
    }
}

void FightLayer::checkResult(float dt)
{
    if (m_nBarrierCurrentHp<=0&&m_nCurrentWave>=1)     //失败
    {
        if(DATAPOOL->isUnlockDefense2())   //解锁了第二防线
        {
            if(m_nCurrentBarrier!=Second_Barrier)
            {
                useSecondLansquenet();
            }
            else
            {
                if(m_nBarrierCurrentHp_Second<=0)
                {
                    playLoseResult();
                }
            }
        }
        else
        {
            playLoseResult();
        }
    }

}

void FightLayer::checkNextWave(float dt)
{
    if (m_eModeInfo.mLogicState != Fight_LogicState_Enter&&m_eModeInfo.mLogicState != Fight_LogicState_None)
    {
        if (m_pEnemy->count()<=0&&!m_bcreateEnemy)
        {
            if(m_eType != Game_Type_Level||!m_bCompleteWave)
            {
                m_bcreateEnemy = true;
                if(m_eType == Game_Type_PVP&&m_bLastWave)
                {
                    m_bcreateEnemy = false;
                }
                m_waveTime.mCurTime = 0;
                CCLOG("m_bDelayComplate:%s",m_bDelayComplate?"true":"false");
                m_eModeInfo.mLogicState = Fight_LogicState_Create;
                int delaytime = Wave_Delay_Time;
                if(m_bFirstWave)
                {
                    delaytime = 0.5;
                    m_bFirstWave = false;
                }
                CCSequence *seq = CCSequence::create(CCDelayTime::create(delaytime),
                                                     CCCallFuncN::create(this, callfuncN_selector(FightLayer::waveDelayCallBack)),
                                                     NULL);
                this->runAction(seq);
                if(m_bEnemyInfo&&!m_bEnemyInfoShow)
                {
                    m_bEnemyInfoShow = true;
                    CCSprite *info = CCSprite::createWithSpriteFrameName(DaBoGuaiShou);
                    CCSize size = CCDirector::sharedDirector()->getWinSize();
                    info->setPosition(ccp(size.width/2, size.height/2));
                    addChild(info);
                    CCSequence *seq = CCSequence::create(CCDelayTime::create(2),
                                                         CCScaleTo::create(0.5, 0.1),
                                                         CCCallFuncN::create(this, callfuncN_selector(FightLayer::removeCallBack_Sprite)),
                                                         NULL);
                    info->runAction(seq);
                }
            }
        }
    }
}

void FightLayer::checkPull(float dt)
{
    if (m_bUseKuangBao||m_fPower>90)
    {
        m_powerTime.mCurTime+=dt;
        if (m_bUseKuangBao||m_powerTime.mCurTime>=(m_powerTime.mTotalTime-m_nXuLiDeltaTime))
        {
            Hero *_bullet = (Hero*)getChildByTag(Fight_Sprites_Bullet);
            if (_bullet) {
                _bullet->setPowerState();
                if(!m_bDanGongPowerFire_Teach)
                {
                    m_bDanGongPowerFire_Teach = true;
                    DATAPOOL->m_bTeachStepOver[TEACH_STEP_USE_DANGONG2] = true;
                }
                
                int type = _bullet->getType();
                switch (type)
                {
                    case Hero_Type_HuoLongGuo:
                    {
                        if(m_bUseKuangBao&&m_fPower<90)break;
                        _bullet->setRemoveTimeUpdate();
                        m_fireDeltaTime.mCurTime +=dt;
                        if (m_fireDeltaTime.mCurTime>=m_fireDeltaTime.mTotalTime)
                        {
                            setCoolDown(_bullet);
                            m_fireDeltaTime.mCurTime = 0;
                            fireBullet(_bullet, m_fAngle);
                        }
                    }
                        break;
                    case Hero_Type_ShiLiu:
                    {
                        if(m_bUseKuangBao&&m_fPower<90)break;
                        if(m_bPartner&&m_fPower<90)break;
                        _bullet->setRemoveTimeUpdate();
                        m_fireDeltaTime.mCurTime +=dt;
                        if (m_fireDeltaTime.mCurTime>=m_fireDeltaTime.mTotalTime)
                        {
                            
                            setCoolDown(_bullet);
                            m_fireDeltaTime.mCurTime = 0;
                            int delta = 20;
                            if(m_fAngle<Max_Angle+20)
                            {
                                fireBullet(_bullet, m_fAngle);
                                fireBullet(_bullet, m_fAngle+delta);
                                fireBullet(_bullet, m_fAngle+delta*2);
                            }
                            else if(m_fAngle>Min_Angle-20)
                            {
                                fireBullet(_bullet, m_fAngle-delta*2);
                                fireBullet(_bullet, m_fAngle-delta);
                                fireBullet(_bullet, m_fAngle);
                            }
                            else
                            {
                                fireBullet(_bullet, m_fAngle-delta);
                                fireBullet(_bullet, m_fAngle);
                                fireBullet(_bullet, m_fAngle+delta);
                            }
                        }
                    }
                        break;
                    default:
                        break;
                }
            }
        }
        else
        {
            Hero *_bullet = (Hero*)getChildByTag(Fight_Sprites_Bullet);
            if (_bullet) {
                _bullet->setXuli(this);
            }
        }
    }
    else
    {
        m_powerTime.mCurTime = 0;
        Hero *_bullet = (Hero*)getChildByTag(Fight_Sprites_Bullet);
        if (_bullet) {
            CCArmature *zhunxin = (CCArmature*)getChildByTag(Fight_Sprites_ZhunXin);
            if(zhunxin&&_bullet->getType() == Hero_Type_BoLuo)
            {
                zhunxin->removeFromParentAndCleanup(true);
                DATAPOOL->removeEffectCache(ZhunXinRes);
            }
            _bullet->cancelXuli();
            CocosDenshion::SimpleAudioEngine::sharedEngine()->stopEffect(m_nXuliSoundID);
        }
    }
}

void FightLayer::touchAward(cocos2d::CCPoint location)
{
    for (int i = m_pAward->count()-1; i>=0; i--)
    {
        CCArmature *arm = (CCArmature*)m_pAward->objectAtIndex(i);
        if(arm->boundingBox().containsPoint(location))
        {
            int tag = arm->getTag();
            int count = getAward(tag);
            arm->stopAllActions();
            switch (tag)
            {
                case Award_Tag_Gold:
                {
                    CCPoint pos =  getChildByTag(Fight_Sprites_Gold)->getPosition();
                    CCMoveTo *moveTo = CCMoveTo::create(0.2, pos);
                    CCSequence *seq = CCSequence::create(moveTo,
                                                         CCCallFuncN::create(this, callfuncN_selector(FightLayer::awardActionCallBack)),
                                                         NULL);
                    arm->runAction(seq);
                }
                    break;
                case Award_Tag_Gem:
                {
                    CCPoint pos =  getChildByTag(Fight_Sprites_Gem)->getPosition();
                    CCMoveTo *moveTo = CCMoveTo::create(0.2, pos);
                    CCSequence *seq = CCSequence::create(moveTo,
                                                         CCCallFuncN::create(this, callfuncN_selector(FightLayer::awardActionCallBack)),
                                                         NULL);
                    arm->runAction(seq);
                }
                    break;
                case Award_Tag_BingJing:
                {
                    if(count<=0)
                    {
                        arm->removeFromParentAndCleanup(true);
                        m_pAward->removeObject(arm);
                        return;
                    }
                    Prop_Data* data =   DATAPOOL->getPropData(Prop_Type_BingJing);
                    CCSprite *spr = CCSprite::createWithSpriteFrameName(data->m_sourceName);
                    CC_SAFE_DELETE(data);
                    if(spr)
                    {
                        spr->setPosition(arm->getPosition());
                        addChild(spr,Fight_Zorder_Top);
                        
                        char buf[20] = {0};
                        sprintf(buf, "+%d",count);
                        CCLabelBMFont *num = NULL;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
                        num = CCLabelBMFont::create(buf, "num_combo.fnt");
#else
                        num = CCLabelBMFont::create(buf, "fonts/num_combo.fnt");
#endif
                        num->setPosition(ccp(spr->getPositionX()+spr->getContentSize().width/2+num->getContentSize().width/2 +2, spr->getPositionY()));
                        addChild(num,Fight_Zorder_Top);
                        
                        CCSequence *seq = CCSequence::create(CCMoveBy::create(0.3, ccp(0, 50)),
                                                             CCCallFuncN::create(this, callfuncN_selector(FightLayer::removeCallBack_Sprite)),
                                                             NULL);
                        CCSequence *seq1 = CCSequence::create(CCMoveBy::create(0.3, ccp(0, 50)),
                                                              CCCallFuncN::create(this, callfuncN_selector(FightLayer::removeBMFontCallBack)),
                                                              NULL);
                        spr->runAction(seq);
                        num->runAction(seq1);
                    }
                    arm->removeFromParentAndCleanup(true);
                    m_pAward->removeObject(arm);
                }
                    break;
                case Award_Tag_LianNu:
                {
                    if(count<=0)
                    {
                        arm->removeFromParentAndCleanup(true);
                        m_pAward->removeObject(arm);
                        return;
                    }
                    Prop_Data* data =   DATAPOOL->getPropData(Prop_Type_LianNu);
                    CCSprite *spr = CCSprite::createWithSpriteFrameName(data->m_sourceName);
                    CC_SAFE_DELETE(data);
                    if(spr)
                    {
                        spr->setPosition(arm->getPosition());
                        addChild(spr,Fight_Zorder_Top);
                        
                        char buf[20] = {0};
                        sprintf(buf, "+%d",count);
                        CCLabelBMFont *num = NULL;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
                        num = CCLabelBMFont::create(buf, "num_combo.fnt");
#else
                        num = CCLabelBMFont::create(buf, "fonts/num_combo.fnt");
#endif
                        num->setPosition(ccp(spr->getPositionX()+spr->getContentSize().width/2+num->getContentSize().width/2 +2, spr->getPositionY()));
                        addChild(num,Fight_Zorder_Top);
                        
                        CCSequence *seq = CCSequence::create(CCMoveBy::create(0.3, ccp(0, 50)),
                                                             CCCallFuncN::create(this, callfuncN_selector(FightLayer::removeCallBack_Sprite)),
                                                             NULL);
                        CCSequence *seq1 = CCSequence::create(CCMoveBy::create(0.3, ccp(0, 50)),
                                                              CCCallFuncN::create(this, callfuncN_selector(FightLayer::removeBMFontCallBack)),
                                                              NULL);
                        spr->runAction(seq);
                        num->runAction(seq1);
                    }
                    arm->removeFromParentAndCleanup(true);
                    m_pAward->removeObject(arm);
                }
                    break;
                case Award_Tag_ZhaDan:
                {
                    if(count<=0)
                    {
                        arm->removeFromParentAndCleanup(true);
                        m_pAward->removeObject(arm);
                        return;
                    }
                    Prop_Data* data =   DATAPOOL->getPropData(Prop_Type_ZhaDan);
                    CCSprite *spr = CCSprite::createWithSpriteFrameName(data->m_sourceName);
                    CC_SAFE_DELETE(data);
                    if(spr)
                    {
                        spr->setPosition(arm->getPosition());
                        addChild(spr,Fight_Zorder_Top);
                        
                        char buf[20] = {0};
                        sprintf(buf, "+%d",count);
                        CCLabelBMFont *num = NULL;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
                        num = CCLabelBMFont::create(buf, "num_combo.fnt");
#else
                        num = CCLabelBMFont::create(buf, "fonts/num_combo.fnt");
#endif
                        num->setPosition(ccp(spr->getPositionX()+spr->getContentSize().width/2+num->getContentSize().width/2 +2, spr->getPositionY()));
                        addChild(num,Fight_Zorder_Top);
                        
                        CCSequence *seq = CCSequence::create(CCMoveBy::create(0.3, ccp(0, 50)),
                                                             CCCallFuncN::create(this, callfuncN_selector(FightLayer::removeCallBack_Sprite)),
                                                             NULL);
                        CCSequence *seq1 = CCSequence::create(CCMoveBy::create(0.3, ccp(0, 50)),
                                                              CCCallFuncN::create(this, callfuncN_selector(FightLayer::removeBMFontCallBack)),
                                                              NULL);
                        spr->runAction(seq);
                        num->runAction(seq1);
                    }
                    arm->removeFromParentAndCleanup(true);
                    m_pAward->removeObject(arm);
                }
                    break;
                default:
                {
                    
                    arm->removeFromParentAndCleanup(true);
                    m_pAward->removeObject(arm);
                }
                    break;
            }
            return;
        }
    }
}

void FightLayer::touchFog(cocos2d::CCPoint location)
{
    for (int i = m_pFog->count()-1; i>=0; i--)      //雾
    {
        CCArmature *fog = (CCArmature*)m_pFog->objectAtIndex(i);
        if (fog->boundingBox().containsPoint(location))
        {
            fog->removeFromParentAndCleanup(true);
            m_pFog->removeObject(fog);
            DATAPOOL->removeEffectCache(ShaBaoRes);
            if(m_pFog->count()<Max_Fog)
            {
                for (int i=0; i<m_pEnemy->count(); i++)
                {
                    Enemy *enemy = (Enemy*)m_pEnemy->objectAtIndex(i);
                    if(enemy->getType() == Enemy_Type_iBoss)
                    {
                        enemy->setUseSkill(false);
                    }
                }
            }
            break;
        }
    }
}

void FightLayer::touchInk(cocos2d::CCPoint location)
{
    for (int i = m_pInk->count()-1; i>=0; i--)      //墨水
    {
        CCSprite *ink = (CCSprite*)m_pInk->objectAtIndex(i);
        if (ink->boundingBox().containsPoint(location))
        {
            ink->removeFromParentAndCleanup(true);
            m_pInk->removeObject(ink);
            break;
        }
    }
}

void FightLayer::touchShiBei(cocos2d::CCPoint location)
{
    for (int i = m_pShiBei->count()-1; i>=0; i--)  //石碑
    {
        CCArmature *arm = (CCArmature*)m_pShiBei->objectAtIndex(i);
        if(arm->boundingBox().containsPoint(location))
        {
            m_pShiBei->removeObject(arm);
            arm->getAnimation()->play("shibei_b");
            if(m_pShiBei->count()<Max_ShiBei)
            {
                for (int i=0; i<m_pEnemy->count(); i++)
                {
                    Enemy *enemy = (Enemy*)m_pEnemy->objectAtIndex(i);
                    if(enemy->getType() == Enemy_Type_eBoss)
                    {
                        enemy->setUseSkill(false);
                    }
                }
            }
            break;
        }
    }

}

void FightLayer::touchEnd(CCPoint location)
{
    if(m_bIsUseLianNu)
    {
        m_bIsFireLianNu = false;
        m_lianNuDeltaTime.mCurTime = 0;
        unschedule(schedule_selector(FightLayer::beganLianNuTime));
    }
    
    if (bTouchedTray&&m_bCanPullCatapult)
    {
        resetTrayPos();
    }
    
    CCSprite *zhadan = (CCSprite*)getChildByTag(Fight_Sprites_ZhaDan);
    if (zhadan)
    {
        CCLayerColor *layer = (CCLayerColor*)getChildByTag(Fight_Sprites_ZhaDan_Area);
        
        if (location.y>Barrier_Y)
        {
            if(m_touchPropIndex==2&&m_nUsePropType[m_touchPropIndex]==Prop_Type_ZhaDan)
            {
                m_nUsePropType[m_touchPropIndex] = 0;
                CCSprite *_prop = (CCSprite*)getChildByTag(Fight_Sprites_Prop3);
                _prop->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(Icon_Prop_Random));
                _prop->setPosition(ccp(getChildByTag(Fight_Sprites_Prop_Tray3)->getPositionX(),getChildByTag(Fight_Sprites_Prop_Tray3)->getPositionY()-15));
            }
            else
            {
                OutfitRecord *rcd = DATAPOOL->getOutfitRecord(Prop_Type_ZhaDan);
                rcd->m_nNum--;
                DATAPOOL->setOutfitRecord(Prop_Type_ZhaDan, *rcd);
                updatePropNum();
            }
            DATAPOOL->playGameEffect(Item_Explode);
            float _x  = layer->getPositionX();
            float _y = layer->getPositionY();
            createEffect(BaoZhaRes, "baozha", ccp(_x+layer->getContentSize().width/2, _y+layer->getContentSize().height/2), Fight_Zorder_Top);
            for (int i=m_pEnemy->count()-1; i>=0; i--)
            {
                Enemy *enemy =(Enemy*)m_pEnemy->objectAtIndex(i);
                if (enemy->checkCollisionLayer(layer)&&enemy->getState()!=Enemy_State_MuBei) {
                    EnemyHurtState *instance = EnemyHurtState::getInstance();
                    enemy->dieWithBoom();
                    enemy->minusBlood(ZhaDanShangHai,false);
                    enemy->GetFSMSystem()->ChangeState(instance);
                }
                else if(enemy->checkCollisionLayer(layer)&&enemy->getState()==Enemy_State_MuBei)
                {
                    m_pEnemy->removeObjectAtIndex(i);
                    enemy->removeFromParentAndCleanup(true);
                    m_nMuBeiCount--;
                }
            }
            for (int i = m_pMuBei->count()-1; i>=0; i--)
            {
                CCArmature *mubei = (CCArmature*)m_pMuBei->objectAtIndex(i);
                if(mubei&&layer->boundingBox().intersectsRect(mubei->cocos2d::CCNode::boundingBox()))
                {
                    mubei->getAnimation()->play("xiaoshi_mb");
                    mubei->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(FightLayer::animationEvent));
                    m_pMuBei->removeObjectAtIndex(i);
                    m_nMuBeiCount--;
                }
            }
        }
        
        this->removeChild(zhadan, true);
        this->removeChild(layer,true);
    }
    
    CCSprite *bingjing = (CCSprite*)getChildByTag(Fight_Sprites_BingJing);
    if (bingjing)
    {
        CCLayerColor *layer = (CCLayerColor*)getChildByTag(Fight_Sprites_ZhaDan_Area);
        if (location.y>Barrier_Y)
        {
            if(m_touchPropIndex==2&&m_nUsePropType[m_touchPropIndex]==Prop_Type_BingJing)
            {
                m_nUsePropType[m_touchPropIndex] = 0;
                CCSprite *_prop = (CCSprite*)getChildByTag(Fight_Sprites_Prop3);
                _prop->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(Icon_Prop_Random));
                _prop->setPosition(ccp(getChildByTag(Fight_Sprites_Prop_Tray3)->getPositionX(),getChildByTag(Fight_Sprites_Prop_Tray3)->getPositionY()-15));
            }
            else
            {
                DATAPOOL->playGameEffect(Item_Frozen);
                OutfitRecord *rcd = DATAPOOL->getOutfitRecord(Prop_Type_BingJing);
                rcd->m_nNum--;
                DATAPOOL->setOutfitRecord(Prop_Type_BingJing, *rcd);
                updatePropNum();
            }
            
            for (int i=m_pEnemy->count()-1; i>=0; i--)
            {
                Enemy *enemy =(Enemy*)m_pEnemy->objectAtIndex(i);
                if (enemy->checkCollisionLayer(layer)&&enemy->getState()!=Enemy_State_MuBei) {
                    EnemyStopState *instance = EnemyStopState::getInstance();
                    instance->setTime(3,Stop_Type_Frozen);
                    enemy->minusBlood(BingJingShangHai,false);
                    enemy->GetFSMSystem()->ChangeState(instance);
                }
            }
        }
        this->removeChild(bingjing, true);
        this->removeChild(layer,true);
    }
    
    m_touchPropIndex = 0;
}

#pragma mark - private callbackfunc
void FightLayer::menuPauseCallBack(cocos2d::CCObject *pSender)
{
    DATAPOOL->m_bGamePause = true;
    DATAPOOL->playGameEffect(Sound_Btn_Clicked);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseAllEffects();
    HelpMethod::pauseNodeActions(this);
    PauseLayer *_pause = (PauseLayer*)(this->getParent()->getChildByTag(2));
    _pause->setZOrder(100);
    _pause->setVisible(true);
    _pause->setTouchEnabled(true);
}

void FightLayer::catapultResetCallBack()
{
    int rand = arc4random()%100;
    if(!m_bPartner&&!m_bcreatePartner&&rand>20&&m_eType == Game_Type_Level&&m_bDaoShuComplate)
    {
        if(m_nCurrentChapter==1&&m_nCurrentLevel==4)
        {
            m_bShowPartner = true;
            m_bcreatePartner = true;
            createPartnerAni();
            return;
        }
        else if(m_nCurrentChapter==1&&m_nCurrentLevel==9)
        {
            m_bShowPartner = true;
            m_bcreatePartner = true;
            createPartnerAni();
            return;
        }
    }
    createNewBullet();
}

void FightLayer::bulletReadyCallBack(CCObject* pSender)
{
    Hero *_bullet =(Hero*)pSender;
    _bullet->setState(Hero_State_Ready);
    _bullet->setTag(Fight_Sprites_Bullet);
    _bullet->setZOrder(Fight_Zorder_Bullet);
}

void FightLayer::bingkuaiRemoveCallBack(cocos2d::CCObject *pSender)
{
    CCSprite *spr = (CCSprite*)pSender;
    this->removeChild(spr, true);
}

void FightLayer::peelCallBack(cocos2d::CCObject *pSender)
{
    CCSprite *peel = (CCSprite*)pSender;
    peel->setZOrder(Fight_Zorder_BG);
    m_pPeel->addObject(peel);
}

void FightLayer::icebergCallBack(cocos2d::CCObject *pSender, void *data)
{
    Enemy *enemy = (Enemy*)pSender;
    CCPoint* pos = (CCPoint*)(data);
    createEffect(BingCRes, "bing_c", ccp(pos->x, pos->y), Fight_Zorder_Up);
    if(pos->y == Barrier_Y + Atk_Barrier_DY)
    {
        minusBarrierHp(enemy->getAtk());
        playBarrierUnderAtkAni();
    }
    CC_SAFE_DELETE(pos);
}

void FightLayer::removeAwardCallBack(cocos2d::CCNode *pSender)
{
    CCArmature *arm = (CCArmature*)pSender;
    if(arm)
    {
        char buf[30];
        memset(buf, 0, 30);
        CCArmatureData *armData = arm->getArmatureData();
        if(armData)
        {
          sprintf(buf, "Sprites/%s.ExportJson",armData->name.c_str());
          DATAPOOL->removeEffectCache(buf);
        }
        if(m_pAward)
            m_pAward->removeObject(arm);
        this->removeChild(arm,true);
    }
}

void FightLayer::hongZhaJiCallBack(cocos2d::CCNode *pSender)
{
    m_bUseHongZhaJi = false;
    unschedule(schedule_selector(FightLayer::hongZhaJiUpdate));
    CCSprite *plane = (CCSprite*)getChildByTag(Fight_Sprites_HongZhaJi);
    if(plane)
        plane->removeFromParentAndCleanup(true);
}

void FightLayer::kuangBaoYaoCallBack(cocos2d::CCNode *pSender)
{
    if(!m_bEnergy)
        m_bUseKuangBao = false;
}

void FightLayer::shuaXinCallBack(cocos2d::CCNode *pSender)
{
    m_bUseShuaXin = false;
    CCArmature *arm = (CCArmature*)getChildByTag(Fight_Sprites_ShuaXin);
    if(arm)
    {
        arm->removeFromParentAndCleanup(true);
        DATAPOOL->removeEffectCache(HuiFuRes);
    }
}

void FightLayer::lockHeroCallBack(cocos2d::CCNode *pSender)
{
    CCSprite *_bingfeng = (CCSprite*)pSender;
    if(_bingfeng)
    {
        _bingfeng->removeFromParentAndCleanup(true);
    }
    m_bLockHero = false;
}

void FightLayer::awardActionCallBack(cocos2d::CCNode *pSender)
{
    CCArmature *arm = (CCArmature*)pSender;
    if(arm)
    {
        arm->removeFromParentAndCleanup(true);
        m_pAward->removeObject(arm);
    }
}

void FightLayer::loadingComplateCallBack(cocos2d::CCNode *pSender)
{
    CCLog("loadingComplateCallBack");
    CCSprite *bg = (CCSprite*)getChildByTag(Fight_Sprites_Load_BG);
    if(bg)
    {
        bg->removeFromParentAndCleanup(true);
    }
    
    CCSprite *progress = (CCSprite*)getChildByTag(Fight_Sprites_Load_Progress);
    if(progress)
    {
        progress->removeFromParentAndCleanup(true);
    }
    
    CCProgressTimer *progressBar = (CCProgressTimer*)getChildByTag(Fight_Sprites_Load_ProgressBar);
    if(progressBar)
    {
        progressBar->removeFromParentAndCleanup(true);
    }
    
    CCSprite *txt = (CCSprite*)getChildByTag(Fight_Sprites_Load_Txt);
    if(txt)
    {
        txt->removeFromParentAndCleanup(true);
    }
    
    CCSprite *tips = (CCSprite*)getChildByTag(Fight_Sprites_Tips);
    if(tips)
    {
        tips->removeFromParentAndCleanup(true);
//        CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(tips_TxtPlist);
    }
    m_bIsComplateLoading = true;
    readData();
}

void FightLayer::waveDelayCallBack(cocos2d::CCNode *pSender)
{
    m_waveTime.mCurTime = 0;
    m_bDelayComplate = true;
}

void FightLayer::disapperCallBack(CCNode *pSender)
{
    Enemy *enemy = (Enemy*)pSender;
    enemy->removeFromParentAndCleanup(true);
    enemyDie(enemy->isBoss());
}

void FightLayer::dangongFireCallBack()
{
    CTeachLayer *teachLayer = NULL;
    teachLayer = (CTeachLayer*)getChildByTag(Fight_Sprites_TeachLayer);
    if(teachLayer)
    {
        m_bShowTeachLayer = false;
        teachLayer->removeFromParentAndCleanup(true);
    }
    m_bStartingFire_Teach = true;
        
}

void FightLayer::dangongPowerFireCallBack()
{
    CTeachLayer *teachLayer = NULL;
    teachLayer = (CTeachLayer*)getChildByTag(Fight_Sprites_TeachLayer);
    if(teachLayer)
    {
        m_bShowTeachLayer = false;
        teachLayer->removeFromParentAndCleanup(true);
        Hero *_bullet = (Hero*)getChildByTag(Fight_Sprites_Bullet);
        if(_bullet)
        {
            _bullet->setPowerState();
            if(!m_bDanGongPowerFire_Teach)
            {
                m_bDanGongPowerFire_Teach = true;
                DATAPOOL->m_bTeachStepOver[TEACH_STEP_USE_DANGONG2] = true;
            }
        }
    }
    m_bStartingPowerFire_Teach = true;
}

void FightLayer::hongzhajiCallBack()
{
    CTeachLayer *teachLayer = NULL;
    teachLayer = (CTeachLayer*)getChildByTag(Fight_Sprites_TeachLayer);
    if(teachLayer)
    {
        m_bShowTeachLayer = false;
        CCPoint pos = teachLayer->getFocusPos();
        usePropBegan(pos);
        teachLayer->removeFromParentAndCleanup(true);
        if(!m_bUseHongZhaji_Teach)
        {
            DATAPOOL->m_bTeachStepOver[TEACH_STEP_USE_HONGZHAJI] = true;
            m_bUseHongZhaji_Teach = true;
        }
        
    }
    m_bStartingHongZhaJi_Teach = true;
}

void FightLayer::newHeroCallBack()
{
    CTeachLayer *teachLayer = NULL;
    teachLayer = (CTeachLayer*)getChildByTag(Fight_Sprites_TeachLayer);
    if(teachLayer)
    {
        m_bShowTeachLayer = false;
        teachLayer->removeFromParentAndCleanup(true);
        selectHero(teachLayer->getFocusPos());
    }
    m_bStartingUseNewHero_Teach= true;
}

void FightLayer::qiangBaoCallBack()
{
    CTeachLayer *teachLayer = NULL;
    teachLayer = (CTeachLayer*)getChildByTag(Fight_Sprites_TeachLayer);
    if(teachLayer)
    {
        m_pQiangbaoGuai->resumeSchedulerAndActions();
        m_bShowTeachLayer = false;
        teachLayer->removeFromParentAndCleanup(true);
        DATAPOOL->m_bTeachStepOver[TEACH_STEP_QIANGBAO] = true;
    }
    resetTrayPos();
    m_bStartQiangBao_Teach = false;
}

void FightLayer::showWinResultCallBack(cocos2d::CCNode *pSender)
{
    CCMenu *menuPause = (CCMenu*)getChildByTag(Fight_Sprites_Btn_Pause);
    menuPause->setVisible(false);
    CCSprite *goldIcon = (CCSprite*)getChildByTag(Fight_Sprites_Gold);
    goldIcon->setVisible(false);
    CCSprite *gemIcon = (CCSprite*)getChildByTag(Fight_Sprites_Gem);
    gemIcon->setVisible(false);
    CCLabelBMFont *goldNum = (CCLabelBMFont*)getChildByTag(Fight_Sprites_Gold_Num);
    goldNum->setVisible(false);
    CCLabelBMFont *gemNum = (CCLabelBMFont*)getChildByTag(Fight_Sprites_Gem_Num);
    gemNum->setVisible(false);
    CCLOG("SHOW WIN RESULT!");
    if(m_bShowResultLock)return;
    m_bShowResultLock = true;
    int type = 0;
    HelpMethod::pauseNodeActions(this);
    ResultLayer *_layer = (ResultLayer*)(getParent()->getChildByTag(3));
    _layer->setVisible(true);
    _layer->setTouchEnabled(true);
    _layer->setZOrder(100);
    float hit = 0.0;
    if(m_nFireCount != 0)
        hit = m_nHitCount>m_nFireCount?1:(float)m_nHitCount/(float)m_nFireCount;
    float damage = (float)(m_nBarrierCurrentHp+m_nBarrierCurrentHp_Second)/(float)(m_nBarrierTotalHp+m_nBarrierTotalHp_Second);
    if(hit==0)hit=1;
    float score = damage*0.9 + hit*0.1;
    
    if(m_eType == Game_Type_Level)
    {
        type = 1;
        DATAPOOL->playedLevel(m_nCurrentChapter, m_nCurrentLevel);
        CCLOG("score:%f",score);
        DATAPOOL->freshLevelScore(m_nCurrentChapter-1, m_nCurrentLevel-1, score*100);
        if(!(DATAPOOL->m_bCompleteTeachLevelOne)&&m_nCurrentLevel==1)
        {
            DATAPOOL->m_bCompleteTeachLevelOne = true;
        }
        if(!(DATAPOOL->m_bCompleteTeachLevelTwo)&&m_nCurrentLevel==2)
        {
            DATAPOOL->m_bCompleteTeachLevelTwo = true;
        }
    }
    else if(m_eType == Game_Type_Boss)
    {
        type = 2;
    }
    int star = DATAPOOL->judgeLevelScoreStar(score*100);
     CCLOG("m_nGameGainGem:%d m_nGameUseGem:%d",m_nGameGainGem,m_nGameUseGem);
    int level = (m_nCurrentChapter-1)*10 + m_nCurrentLevel;
    if(m_nBossHurtHp>xUser->leftHp)m_nBossHurtHp = xUser->leftHp;
    _layer->showWin(star, hit*100, damage*100,m_nGainGold,level,type,m_nGameGainGem,m_nGameUseGem,m_nBossHurtHp);
}

void FightLayer::showLoseResultCallBack(cocos2d::CCNode *pSender)
{
    CCMenu *menuPause = (CCMenu*)getChildByTag(Fight_Sprites_Btn_Pause);
    menuPause->setVisible(false);
    if(m_bShowResultLock)return;
    m_bShowResultLock = true;
    HelpMethod::pauseNodeActions(this);
    ResultLayer *_layer = (ResultLayer*)(getParent()->getChildByTag(3));
    _layer->setVisible(true);
    _layer->setTouchEnabled(true);
    _layer->setZOrder(100);
    float hit = 0.0;
    if(m_nFireCount != 0)
        hit = m_nHitCount>m_nFireCount?1:(float)m_nHitCount/(float)m_nFireCount;
    float damage = (float)(m_nBarrierCurrentHp+ m_nBarrierCurrentHp_Second) / (float)(m_nBarrierTotalHp_Second+m_nBarrierTotalHp);
    CCLOG("%f %f",hit,damage);
    CCLOG("%d", m_eType);
    int type = 1;
    if(m_eType==Game_Type_PVP)
    {
        type = 0;
    }
    else if(m_eType == Game_Type_Boss)
    {
        type = 2;
    }
    CCLOG("m_nGameGainGem:%d m_nGameUseGem:%d",m_nGameGainGem,m_nGameUseGem);
    if(m_nBossHurtHp > xUser->leftHp) m_nBossHurtHp = xUser->leftHp;
    _layer->showLose(0, hit*100, damage*100,m_nGainGold,m_nGameGainGem,m_nGameUseGem,type,m_nBossHurtHp);
}

void FightLayer::qiangbaoguaiCallBack(cocos2d::CCNode *pSender)
{
    m_pQiangbaoGuai->removeFromParentAndCleanup(true);
    DATAPOOL->removeEffectCache(QiangBaoGuaiRes);
    m_pQiangbaoGuai = NULL;
}

void FightLayer::showQiangBaoCallBack(cocos2d::CCNode *pSender)
{
    HelpMethod::pauseNodeActions(this);
    SnatchLayer *layer = SnatchLayer::create();
    addChild(layer,Fight_Zorder_Top+2);
}

void FightLayer::startQiangBaoTeachCallBack()
{
    m_bStartQiangBao_Teach = true;
}

//void FightLayer::loadingCallBack(float dt)
//{
//    CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
//    m_nLoadingPicNum++;
//    int percent = (((float)m_nLoadingPicNum/m_nLoadingPicNums)*100.0f);
//    
//    CCProgressTimer *progress = (CCProgressTimer*)getChildByTag(Fight_Sprites_Load_ProgressBar);
//    if(progress)
//    {
//        progress->setPercentage(percent);
//    }
//    
//    if(m_nLoadingPicNum>=m_nLoadingPicNums)
//    {
//        loadingComplateCallBack(NULL);
//    }
//}

void FightLayer::removeParticleCallBack(cocos2d::CCNode *pSender)
{
    Enemy *_enemy = (Enemy*)pSender;
    if(_enemy)
    {
        CCParticleSmoke *_emitter = (CCParticleSmoke*)_enemy->getChildByTag(Effect_Particle);
        if(_emitter)
            _emitter->removeFromParentAndCleanup(true);
    }
}

#pragma mark - Scheduler
void FightLayer::timeUpdate(float dt)
{
    if(!m_bDaoShuComplate)return;
    if(m_eType == Game_Type_Boss)
    {
        m_nTime--;
        if (m_nTime>=0)
        {
            char buf[10];
            memset(buf, 0, 10);
            sprintf(buf, "%02d:%02d",m_nTime/60,(int)m_nTime%60);
            CCLabelBMFont *_time = (CCLabelBMFont*)getChildByTag(Fight_Sprites_Time);
            _time->setString(buf);
            if(m_nTime<=10)
            {
                CCLOG("time is up!");
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(Time);
            }
        }
        else
        {
            unschedule(schedule_selector(FightLayer::timeUpdate));
            playLoseResult();
        }
    }
    else if(m_eType == Game_Type_Endless)
    {
        m_nTime++;
        char buf[10] = {0};
        sprintf(buf, "%02d:%02d",m_nTime/60,m_nTime%60);
        CCLabelBMFont *_time = (CCLabelBMFont*)getChildByTag(Fight_Sprites_Time);
        _time->setString(buf);
        
        if(m_nTime>m_nBestTime)
        {
            m_nBestTime = m_nTime;
            DATAPOOL->bestTime = m_nBestTime;
            memset(buf, 0, 10);
            sprintf(buf, "%02d:%02d",m_nBestTime/60,(int)m_nBestTime%60);
            CCLabelBMFont *_time1 = (CCLabelBMFont*)getChildByTag(Fight_Sprites_BestTime);
            _time1->setString(buf);
        }
    }
}

void FightLayer::waveUpdate(float dt)
{
    if (m_pLevelData&&m_nCurrentWave>m_pLevelData->nWaveCount)  //当前的波数大于等于总波数
    {
        if(m_eType == Game_Type_Level)
        {
            unschedule(schedule_selector(FightLayer::waveUpdate));
            return;
        }
    }
    
    if(isShowTeachLayer())return;
    m_waveTime.mCurTime+=dt;
    if(m_waveTime.mCurTime>m_waveTime.mTotalTime)
    {
        m_waveTime.mCurTime = 0;
        m_eModeInfo.mLogicState = Fight_LogicState_Create;
        int delaytime = Wave_Delay_Time;
        if(m_bFirstWave)
        {
            delaytime = 0.5;
            m_bFirstWave = false;
        }
        CCSequence *seq = CCSequence::create(CCDelayTime::create(delaytime),
                                             CCCallFuncN::create(this, callfuncN_selector(FightLayer::waveDelayCallBack)),
                                             NULL);
        this->runAction(seq);
        if(m_bEnemyInfo&&!m_bEnemyInfoShow)
        {
            m_bEnemyInfoShow = true;
            CCSprite *info = CCSprite::createWithSpriteFrameName(DaBoGuaiShou);
            CCSize size = CCDirector::sharedDirector()->getWinSize();
            info->setPosition(ccp(size.width/2, size.height/2));
            addChild(info);
            CCSequence *seq = CCSequence::create(CCDelayTime::create(2),
                                                 CCScaleTo::create(0.5, 0.1),
                                                 CCCallFuncN::create(this, callfuncN_selector(FightLayer::removeCallBack_Sprite)),
                                                 NULL);
            info->runAction(seq);
        }
        
    }
}

void FightLayer::hongZhaJiUpdate(float dt)
{
    CCSprite *plane = (CCSprite*)getChildByTag(Fight_Sprites_HongZhaJi);
    if(plane)
    {
        float _x = plane->getPositionX();
        _x += arc4random()%20;
        _x -=10;
        float _y = plane->getPositionY();
        createEffect(BaoZhaRes, "baozha",ccp(_x, _y-100),Fight_Zorder_Up,this,true);
        for (int i=m_pEnemy->count()-1; i>=0; i--)
        {
            Enemy *enemy = (Enemy*)m_pEnemy->objectAtIndex(i);
            if(CCRectMake(_x - 250,_y-250, 500,500).containsPoint(enemy->getPosition())&&enemy->getState()!=Enemy_State_MuBei&&enemy->getState()!=Enemy_State_Invisibility)
            {
                enemy->dieWithBoom();
                enemy->minusBlood(HongZhaJi_Atk,false);
                EnemyHurtState *_instance = EnemyHurtState::getInstance();
                enemy->GetFSMSystem()->ChangeState(_instance);
            }
            else if(CCRectMake(_x - 250,_y-250, 500,500).containsPoint(enemy->getPosition())&&enemy->getState()==Enemy_State_MuBei)
            {
                m_pEnemy->removeObjectAtIndex(i);
                enemy->removeFromParentAndCleanup(true);
                m_nMuBeiCount--;
            }
        }
        
        for (int i = m_pMuBei->count()-1; i>=0; i--)
        {
            CCArmature *mubei = (CCArmature*)m_pMuBei->objectAtIndex(i);
            if(mubei&&CCRectMake(_x - 250,_y-250, 500,500).containsPoint(mubei->getPosition()))
            {
                mubei->getAnimation()->play("xiaoshi_mb");
                mubei->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(FightLayer::animationEvent));
                m_pMuBei->removeObjectAtIndex(i);
                m_nMuBeiCount--;
            }
        }
        DATAPOOL->playGameEffect(Item_Explode);
    }
}

void FightLayer::gameRestart(float cleanGem)
{
    DATAPOOL->m_bGamePause = false;
    refreshData(cleanGem);
    readData();
}

void FightLayer::gameQuit(bool sendMsg)
{
    if(m_bEnergy)
        DATAPOOL->energy = 0;
    DATAPOOL->m_bGamePause = false;
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
    if(sendMsg)
    {
        if(m_eType!=Game_Type_Boss)
        {
            (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_AF_UPDATE_USERINFO(this, 0, xUser->pUserInfo->gemstone, m_nGameGainGem,m_nGameUseGem,0,xUser->totalScore);
        }
        else
        {
            if(m_nBossHurtHp!=0)
            {
                (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_BOSS_UPDATE_FIGHT(this, 0, xUser->pUserInfo->gemstone,m_nGameGainGem,m_nGameUseGem, m_nBossHurtHp);
                xData->m_bGetBossPrize = true;
            }
        }
    }
    xSM->enableTransit();
    
    switch (m_eType) {
        case Game_Type_Level:
        {
            CCLOG("level:%d",m_nCurrentLevel);
            if(m_nCurrentLevel%10==0)
            {
                xSM->back(ST_Dungeon);
            }
            else
            {
                xSM->back(ST_Level);
            }
        }
            break;
        case Game_Type_Endless:
            xSM->back(ST_Home);
            break;
        case Game_Type_PVP:
            xSM->back(ST_PVP);
            break;
        case Game_Type_Boss:
            xSM->back(ST_Boss);
            break;
        default:
            xSM->back(ST_Home);
            break;
    }
}


void FightLayer::remoteAtkCallBack_Sprite(cocos2d::CCNode *pSender, void *data)
{
    Enemy* _enemy = (Enemy*)data;
    CCSprite *spr = (CCSprite*)pSender;
    if(spr)
    {
        CCLOG("pSender is sprite!");
        minusBarrierHp(_enemy->getAtk());
        playBarrierUnderAtkAni();
        this->removeChild(spr, true);
    }
    
}

void FightLayer::remoteAtkCallBack_Armature(cocos2d::CCNode *pSender, void *data)
{
    Enemy* _enemy = (Enemy*)data;
    CCArmature *arm = (CCArmature*)pSender;
    if(arm)
    {
        CCLOG("pSender is armature!");
        minusBarrierHp(_enemy->getSkillAtk());
        playBarrierUnderAtkAni();
        CCArmatureData *armdata = arm->getArmatureData();
        if(armdata)
        {
            char buf[40]= {0};
            sprintf(buf, "Sprites/%s.ExportJson",armdata->name.c_str());
            DATAPOOL->removeEffectCache(buf);
            this->removeChild(arm,true);
        }
    }
}

void FightLayer::remoteAtkCallBack_Armature_Tornado(cocos2d::CCNode *pSender, void *data)
{
    Enemy* _enemy = (Enemy*)data;
    CCArmature *arm = (CCArmature*)pSender;
    if(arm)
    {
        CCLOG("pSender is armature!");
        minusBarrierHp(_enemy->getSkillAtk());
        playBarrierUnderAtkAni();
        CCArmatureData *armdata = arm->getArmatureData();
        this->removeChild(arm,true);
        if(armdata&&m_nTornadoCount==1)
        {
            char buf[40]= {0};
            sprintf(buf, "Sprites/%s.ExportJson",armdata->name.c_str());
            DATAPOOL->removeEffectCache(buf);
        }
        m_nTornadoCount--;
    }
}

void FightLayer::remoteAtkCallBack_Armature_HuoQiu(cocos2d::CCNode *pSender, void *data)
{
    Enemy* _enemy = (Enemy*)data;
    CCArmature *arm = (CCArmature*)pSender;
    if(arm)
    {
        CCLOG("pSender is armature!");
        minusBarrierHp(_enemy->getSkillAtk());
        playBarrierUnderAtkAni();
        CCArmatureData *armdata = arm->getArmatureData();
        this->removeChild(arm,true);
        if(armdata&&m_nHuoQiuCount==1)
        {
            char buf[40]= {0};
            sprintf(buf, "Sprites/%s.ExportJson",armdata->name.c_str());
            DATAPOOL->removeEffectCache(buf);
        }
        m_nHuoQiuCount--;
    }
}

void FightLayer::removeAtkCallBack_Armature(CCNode *pSender)
{
    CCArmature *arm = (CCArmature*)pSender;
    if(arm)
    {
        this->removeChild(arm,true);
    }
}

void FightLayer::removeArmatureWithClean(cocos2d::CCNode *pSender)
{
    CCArmature *trans1 = (CCArmature*)getChildByTag(Fight_Sprites_Trans1);
    CCArmature *trans2 = (CCArmature*)getChildByTag(Fight_Sprites_Trans2);
    trans1->removeFromParentAndCleanup(true);
    trans2->removeFromParentAndCleanup(true);
    DATAPOOL->removeEffectCache(HeiDongRes);
    
    for(int j = 0;j<m_pEnemy->count();j++)
    {
        Enemy *_enemy = (Enemy*)m_pEnemy->objectAtIndex(j);
        if(_enemy->isBoss())
        {
            _enemy->setUseSkill(false);
        }
    }
}

void FightLayer::remoteAtkCallBack_ArmatureLans(cocos2d::CCNode *pSender, void *data)
{
    Lansquenet *lans = (Lansquenet*)data;
    CCArmature *arm = (CCArmature*)pSender;
    if(arm)
    {
        for (int i=m_pEnemy->count()-1; i>=0; i--)
        {
            Enemy *enemy = (Enemy*)m_pEnemy->objectAtIndex(i);
            if(enemy&&enemy->checkCollisionArm(lans))
            {
                enemy->minusBlood(lans->getAtk(),false);
                EnemyHurtState *_instance = EnemyHurtState::getInstance();
                enemy->GetFSMSystem()->ChangeState(_instance);
            }
        }
        CCArmatureData *armdata = arm->getArmatureData();
        if(armdata)
        {
            char buf[40] = {0};
            sprintf(buf, "Sprites/%s.ExportJson",armdata->name.c_str());
            this->removeChild(arm,true);
            DATAPOOL->removeEffectCache(buf);
        }
    }
}

void FightLayer::removeCallBack_Sprite(cocos2d::CCNode *pSender)
{
    CCSprite *spr = (CCSprite*)pSender;
    if(spr)
    {
        spr->removeFromParentAndCleanup(true);
    }
}

void FightLayer::removeEnemyCallBack(cocos2d::CCNode *pSender)
{
    Enemy *enemy = (Enemy*)pSender;
    if(enemy)
    {
        changeGoldNum(enemy->getGold());
        enemy->removeFromParentAndCleanup(true);
//        CC_SAFE_RELEASE_NULL(enemy);
    }
}

#pragma mark - PauseLayer
PauseLayer::~PauseLayer()
{
    
}

bool PauseLayer::init()
{
    if (!CCLayer::init()) {
        return false;
    }
    setTouchEnabled(false);
    return true;
}

void PauseLayer::onEnter()
{
    CCLayer::onEnter();
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCLayerColor *layer = NULL;
    layer = (CCLayerColor*)getChildByTag(101);
    if(!layer)
    {
        layer = CCLayerColor::create(ccc4(0, 0, 0, 150));
        addChild(layer,1,101);
        
        CCSprite *bg = CCSprite::createWithSpriteFrameName(Game_Result_BG);
        bg->setPosition(ccp(size.width/2, size.height/2));
        addChild(bg,1);
        
        CCMenuItemSprite *_item_btn_continue = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(Game_Pause_Btn_Continue),
                                                                        CCSprite::createWithSpriteFrameName(Game_Pause_Btn_Continue_Click),
                                                                        this, menu_selector(PauseLayer::callbackContinue));
        
        CCMenuItemSprite *_item_btn_restart = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(Game_Pause_Btn_restart),
                                                                       CCSprite::createWithSpriteFrameName(Game_Pause_Btn_restart_Click),
                                                                       this,menu_selector(PauseLayer::callbackRestart));

        
        m_itemExit = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(Game_Pause_Btn_Exit),
                                              CCSprite::createWithSpriteFrameName(Game_Pause_Btn_Exit_Click),
                                              CCSprite::createWithSpriteFrameName(Game_Pause_Btn_Exit_Click),
                                              this,menu_selector(PauseLayer::callbackExit));
        
        
        CCMenu *_menuButton = CCMenu::create(_item_btn_continue,_item_btn_restart,m_itemExit,NULL);
        _menuButton->alignItemsVerticallyWithPadding(30);
        _menuButton->setPosition(ccp(size.width/2, size.height/2));
        addChild(_menuButton,1);
        _menuButton->setTouchPriority(-130);
    }
}

void PauseLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);
}

bool PauseLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}

void PauseLayer::callbackContinue(CCObject *pSender)
{
    DATAPOOL->m_bGamePause = false;
    DATAPOOL->playGameEffect(Sound_Btn_Clicked);
    if(xData->m_bOpenBackMusic)
    {
        SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    }
    if(xData->m_bOpenSoundEffect)
    {
        SimpleAudioEngine::sharedEngine()->resumeAllEffects();
    }
    FightScene* scene = (FightScene*)getParent();
    FightLayer *layer = (FightLayer*)(scene->getChildByTag(1));
    HelpMethod::resumeNodeActions(layer);
    this->setVisible(false);
    this->setTouchEnabled(false);
#ifdef FAST_CLEARANCE
    CCSequence *seq = CCSequence::create(CCDelayTime::create(ShowResultDelay),
                                         CCCallFuncN::create(layer, callfuncN_selector(FightLayer::showWinResultCallBack)),
                                         NULL);
    this->runAction(seq);
#endif
}

void PauseLayer::callbackRestart(CCObject *pSender)
{
    DATAPOOL->m_bGamePause = false;
    DATAPOOL->playGameEffect(Sound_Btn_Clicked);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    FightScene* scene = (FightScene*)getParent();
    FightLayer *layer = (FightLayer*)(scene->getChildByTag(1));
    HelpMethod::resumeNodeActions(layer);
    this->setVisible(false);
    this->setTouchEnabled(false);
#ifdef FAST_CLEARANCE
    CCSequence *seq = CCSequence::create(CCDelayTime::create(ShowResultDelay),
                                         CCCallFuncN::create(layer, callfuncN_selector(FightLayer::showLoseResultCallBack)),
                                         NULL);
    this->runAction(seq);
#else
    layer->gameRestart(true);
#endif
    
}

void PauseLayer::callbackExit(CCObject *pSender)
{
    DATAPOOL->m_bGamePause = false;
    DATAPOOL->playGameEffect(Sound_Btn_Clicked);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
    FightScene* scene = (FightScene*)getParent();
    FightLayer *layer = (FightLayer*)(scene->getChildByTag(1));
    layer->gameQuit(true);
}
