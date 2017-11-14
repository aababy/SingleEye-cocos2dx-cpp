//
//  SceneManager.cpp
//  SingleEye
//
//  Created by gwsoft on 1/21/14.
//
//
#include "SceneManager.h"


SceneManager* SceneManager::getInstance()
{
    static SceneManager instance;
    return &instance;
}


SceneManager::SceneManager()
{
    CCNode::init();
    
    m_bTransEnabled = false;
    m_pOldScene = NULL;
}


SceneManager::~SceneManager()
{
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(SceneManager::switchToScene), this);
    m_vData.clear();
}


void SceneManager::switchToTrans(SceneData &data)
{
    CCScene *pScene = TransitScene::scene(data);
    //CCTransitionCrossFade *fade = CCTransitionCrossFade::create(0.f, pScene);        
    CCDirector::sharedDirector()->replaceScene(pScene);         
}


void SceneManager::switchToLoading(SceneData &data)
{
    CCScene* pRunScene = CCDirector::sharedDirector()->getRunningScene();    
    
    Block* block = Block::create();
    block->setOpacity(128);
    pRunScene->addChild(block, kDialogZOrder + 1);
    
    CCSprite *loading = CCSprite::createWithSpriteFrameName("zrz.png");
    CCSize size = CCDirector::sharedDirector()->getWinSize();    
    loading->setPosition(ccp(size.width/2, size.height/2));        
    pRunScene->addChild(loading, kDialogZOrder + 1);
    
    //设置个Timer下一帧再切换
    m_eBackup = data;
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(SceneManager::switchToScene), this, 0.f, 0, 0.f, false);  
}


void SceneManager::switchToScene(float dt)
{    
    backFromTrans(m_eBackup);
}


void SceneManager::replaceScene(SceneData &data)
{
    bool bDirectReturn = false;
    
    CCScene *pScene = NULL;
    
    
    //判断是否有过渡界面
    if (m_bTransEnabled == true) {
        switchToLoading(data);
        return;
    }
        
    //从FightScene返回, 就是从游戏返回(目前是这样), 加载UIPublic
    if (m_eCurScene == ST_Fight) {
        loadUIPublic();
    }
    
    //这里将记录当前的Tag
    m_eCurScene = data.aTag;
    
    switch (data.aTag) {
        case ST_Load:
        {
            pScene = CLoadScene::create();
        }
            break;
        case ST_Title:
        {
            pScene = CTitleScene::create();
        }
            break;
        case ST_Login:
        {
            pScene = LoginScene::scene();
        }
            break;
        case ST_PVP:
        {
            pScene = PvPScene::scene();
        }
            break;
        case ST_PLan:
        {
            CPlanScene *pSce = CPlanScene::create();
            pSce->setType((EPlanSceneType)data.iData);
            pScene = (CCScene*)pSce;
        }
            break;
        case ST_Sign:
        {
            pScene = SignScene::scene();
        }
            break;
        case ST_Story:
        {
            pScene = CStoryScene::create();
        }
            break;
        case ST_Home:
        {
            if (data.iData == 0) {
                pScene = CHomeScene::create();
            } else if(data.iData == 1){
                data.iData = 0;
                pScene = CHomeScene::create();
                CCTransitionFade *animateScene = CCTransitionFade::create(0.5f, pScene);
                CCDirector::sharedDirector()->replaceScene(animateScene);                                
                bDirectReturn = true;
            }
            
        }
            break;
        case ST_Boss:
        {
            pScene = BossScene::scene();
        }
            break;
        case ST_Message:
        {
            pScene = MessageScene::scene();
        }
            break;
        case ST_Dungeon:
        {
            pScene = CDungeonScene::create();        
        }
            break;
        case ST_Shop:
        {
            pScene = CShopScene::create();
        }
            break;
        case ST_Huodong:
        {
            pScene = CHuodongScene::create();
        }
            break;
        case ST_Share:
        {
            pScene = ShareScene::scene();
        }
            break;
        case ST_Tavern:
        {
            pScene = CTavernScene::create();
        }
            break;
        case ST_Level:
        {
            pScene = CLevelScene::create();
        }
            break;
        case ST_Defense:
        {
            pScene = CDefenseScene::create();
        }
            break;
        case ST_BuyPower:
        {
            pScene = CBuyPowerScene::create();
        }
            break;
        case ST_BuyGem:
        {
            CBuyGemScene * scene = CBuyGemScene::create();
            CCDirector::sharedDirector()->replaceScene(scene);
            bDirectReturn = true;
        }
            break;
        case ST_BuyGold:
        {
            CBuyGoldScene * scene = CBuyGoldScene::create();
            CCDirector::sharedDirector()->replaceScene(scene);
            bDirectReturn = true;
        }
            break;
        case ST_Fight:
        {
            FightScene *pScene = FightScene::create();
            
            if (data.iData == Game_Type_Endless)
            {
                //pet
                int petDefenseBuf1[PLAN_PET_NUM];
                for(int i = 0; i < PLAN_PET_NUM; i++)
                {
                    int value = DATAPOOL->getPlanPet(i, 1);
                    //fightScene中,0表示空
                    petDefenseBuf1[i] = (value>0)?(value):(0);
                    CCLOG("defense id=%d",petDefenseBuf1[i]);
                }
                pScene->setGameLans(petDefenseBuf1);
                
                if(DATAPOOL->isUnlockDefense2())
                {
                    int petDefenseBuf2[PLAN_PET_NUM];
                    for(int i = 0; i < PLAN_PET_NUM; i++)
                    {
                        int value = DATAPOOL->getPlanPet(i, 2);
                        //fightScene中,0表示空
                        petDefenseBuf2[i] = (value>0)?(value):(0);
                        CCLOG("defense id=%d",petDefenseBuf2[i]);
                    }
                    pScene->setGameLansTwo(petDefenseBuf2);
                }
                
                //outfit
                int outfitPlanBuf[PLAN_OUTFIT_NUM + 1];//+1给随机道具留位置
                memset(outfitPlanBuf, -1, sizeof(outfitPlanBuf));
                outfitPlanBuf[PLAN_OUTFIT_NUM] = 0;
                pScene->setGameProps(outfitPlanBuf);
                
                //关卡数据
                int chapter = (rand() % (xData->m_nMaxUnlockChapter+1)) + 1;
                if(chapter>5)chapter = 5;
                int level =   (rand() % 7) + 4; //4-10 关   //   (rand() % LEVEL_NUM_OF_CHAPTER) + 1;
                pScene->setGameLevel(chapter, level);
                
                //type
                pScene->setGameType(Game_Type_Endless);
            }
            else if (data.iData == Game_Type_Level)
            {
                //英雄数据
                int heroPlanBuf[PLAN_HERO_NUM];
                for(int i = 0; i < PLAN_HERO_NUM; i++)
                {
                    int value = DATAPOOL->getPlanHero(i);
                    heroPlanBuf[i] = (value>0)?(value):(0);
                }
                pScene->setGameSpeHero(heroPlanBuf);
                //道具
                int outfitPlanBuf[PLAN_OUTFIT_NUM+1];
                for(int i = 0; i < PLAN_OUTFIT_NUM; i++)
                {
                    int value = DATAPOOL->getPlanOutfit(i);
                    outfitPlanBuf[i] = (value>-1)?(value):(-1);
                }
                outfitPlanBuf[PLAN_OUTFIT_NUM] = 0;
                pScene->setGameProps(outfitPlanBuf);
                
                //pet
                int petPlanBuf[PLAN_PET_NUM];
                for(int i = 0; i < PLAN_PET_NUM; i++)
                {
                    int value = DATAPOOL->getPlanPet(i, 1);
                    petPlanBuf[i] = (value>0)?(value):(0);
                }
                pScene->setGameLans(petPlanBuf);
                
                if(DATAPOOL->isUnlockDefense2())
                {
                    int petDefenseBuf2[PLAN_PET_NUM];
                    for(int i = 0; i < PLAN_PET_NUM; i++)
                    {
                        int value = DATAPOOL->getPlanPet(i, 2);
                        petDefenseBuf2[i] = (value>0)?(value):(0);
                        CCLOG("defense id=%d",petDefenseBuf2[i]);
                    }
                    pScene->setGameLansTwo(petDefenseBuf2);
                }
                
                //关卡数据
                pScene->setGameLevel(DATAPOOL->currentChapter+1, DATAPOOL->m_nCurrentLevel[DATAPOOL->currentChapter]+1);
                
                pScene->setGameType(Game_Type_Level);

            } else if(data.iData == Game_Type_PVP) {

                //英雄数据
                int heroPlanBuf[PLAN_HERO_NUM];
                for(int i = 0; i < PLAN_HERO_NUM; i++)
                {
                    int value = DATAPOOL->getPlanHero(i);
                    heroPlanBuf[i] = (value>0)?(value):(0);
                }
                pScene->setGameSpeHero(heroPlanBuf);
                
                //道具
                int outfitPlanBuf[PLAN_OUTFIT_NUM+1];
                memset(outfitPlanBuf, -1, sizeof(outfitPlanBuf));
                outfitPlanBuf[PLAN_OUTFIT_NUM] = 0;
                pScene->setGameProps(outfitPlanBuf);
                
                //pet
                int petDefenseBuf1[PLAN_PET_NUM];
                for(int i = 0; i < PLAN_PET_NUM; i++)
                {
                    int value = DATAPOOL->getPlanPet(i, 1);
                    //fightScene中,0表示空
                    petDefenseBuf1[i] = (value>0)?(value):(0);
                    CCLOG("defense id=%d",petDefenseBuf1[i]);
                }
                pScene->setGameLans(petDefenseBuf1);
                
                if(DATAPOOL->isUnlockDefense2())
                {
                    int petDefenseBuf2[PLAN_PET_NUM];
                    for(int i = 0; i < PLAN_PET_NUM; i++)
                    {
                        int value = DATAPOOL->getPlanPet(i, 2);
                        //fightScene中,0表示空
                        petDefenseBuf2[i] = (value>0)?(value):(0);
                        CCLOG("defense id=%d",petDefenseBuf2[i]);
                    }
                    pScene->setGameLansTwo(petDefenseBuf2);
                }
                
                //关卡数据
                pScene->setGameLevel(DATAPOOL->currentChapter+1, DATAPOOL->m_nCurrentLevel[DATAPOOL->currentChapter]+1);
                
                if (PLANSCENE_TYPE_PVP == data.iData)
                {
                    pScene->setGameType(Game_Type_PVP);
                }
                else
                {
                    pScene->setGameType(Game_Type_Level);
                }
            }
            else if(data.iData == Game_Type_Boss)
            {
                int heroPlanBuf[PLAN_HERO_NUM];
                for(int i = 0; i < PLAN_HERO_NUM; i++)
                {
                    int value = DATAPOOL->getPlanHero(i);
                    heroPlanBuf[i] = (value>0)?(value):(0);
                }
                pScene->setGameSpeHero(heroPlanBuf);
                //道具
                int outfitPlanBuf[PLAN_OUTFIT_NUM+1];
                for(int i = 0; i < PLAN_OUTFIT_NUM; i++)
                {
                    int value = DATAPOOL->getPlanOutfit(i);
                    outfitPlanBuf[i] = (value>-1)?(value):(-1);
                }
                outfitPlanBuf[PLAN_OUTFIT_NUM] = 0;
                pScene->setGameProps(outfitPlanBuf);
                
                //pet
                int petPlanBuf[PLAN_PET_NUM];
                for(int i = 0; i < PLAN_PET_NUM; i++)
                {
                    int value = DATAPOOL->getPlanPet(i, 1);
                    petPlanBuf[i] = (value>0)?(value):(0);
                }
                pScene->setGameLans(petPlanBuf);
                
                if(DATAPOOL->isUnlockDefense2())
                {
                    int petDefenseBuf2[PLAN_PET_NUM];
                    for(int i = 0; i < PLAN_PET_NUM; i++)
                    {
                        int value = DATAPOOL->getPlanPet(i, 2);
                        petDefenseBuf2[i] = (value>0)?(value):(0);
                        CCLOG("defense id=%d",petDefenseBuf2[i]);
                    }
                    pScene->setGameLansTwo(petDefenseBuf2);
                }
 
                //关卡
                pScene->setGameLevel(1, 1);
            
                pScene->setGameType(Game_Type_Boss);
                
            }
            
            CCDirector::sharedDirector()->replaceScene(pScene);
            bDirectReturn = true;
        }
            break;
        case ST_Setting:
        {
            pScene = CSettingScene::create();
        }
            break;
        case ST_SelectHero:
        {
            CSelectHeroScene *pScene = CSelectHeroScene::create();
            pScene->setGoalIndex(data.iData);
            CCDirector::sharedDirector()->replaceScene(pScene);
            bDirectReturn = true;
        }
            break;
        case ST_SelectPet:
        {
            pScene = CSelectPetScene::create();
        }
            break;
        case ST_SelectOutfit:
        {
            CSelectOutfitScene *pScene = CSelectOutfitScene::create();
            pScene->setGoalIndex(data.iData);
            CCDirector::sharedDirector()->replaceScene(pScene);
            bDirectReturn = true;
        }
            break;
            
        case ST_About:
        {
            pScene = CAboutScene::create();
        }
            break;
        case ST_PingLun:
        {
            pScene = CPingLunScene::create();
        }
            break;
        default:
            break;
    }
            
    
    if (bDirectReturn == false) {
        if (m_vData.size() != 0) {
            CCDirector::sharedDirector()->replaceScene(pScene);
        } else {
            //第1个场景
            CCDirector::sharedDirector()->runWithScene(pScene);
        }
    }    
}


void SceneManager::back()
{    
    if (isOldScene()) {
        return;
    }        
    
    //删除当前场景记录            
    m_vData.pop_back();
    CCAssert(m_vData.size() != 0, "error");

    SceneData data = m_vData.at(m_vData.size() - 1);
    replaceScene(data);
}


void SceneManager::back(SceneTag aTag)
{
    if (isOldScene()) {
        return;
    }
    
    CCAssert(m_vData.size() != 0, "error");
    
    int index = m_vData.size();

    while (index > 0) {
        SceneData data = m_vData.at(index - 1);
        
        if (data.aTag == aTag) {
            replaceScene(data);
            break;
        }
        else
        {
            //删除该记录
            index--;
            m_vData.pop_back();
            CCAssert(index != 0, "no this scene.");
        }
    }
}


void SceneManager::forward(SceneTag aTag)
{   
    forward(aTag, 0, NULL);
}


void SceneManager::forward(SceneTag aTag, int iData, void* pData)
{
    if (isOldScene()) {
        return;
    }
    
    //记录进入的Scene
    SceneData data;
    
    data.aTag = aTag;
    data.iData = iData;
    data.pData = pData;
        
    replaceScene(data);

    m_vData.push_back(data);
}


SceneTag SceneManager::getLastSceneTag()
{
    if (m_vData.size() > 1) {
        SceneData data = m_vData.at(m_vData.size() - 2);        
        return data.aTag;
    }
    else
    {
        return ST_None;
    }
}


SceneTag SceneManager::getRunningSceneTag()
{      
    return m_eCurScene;
//    if (m_vData.size() != 0) {
//        SceneData data = m_vData.at(m_vData.size() - 1);        
//        return data.aTag;
//    }
//    else
//    {
//        return ST_None;
//    }
}


void SceneManager::enableTransit()
{
    m_bTransEnabled = true;    
}


void SceneManager::loadUIPublic()
{
    DATAPOOL->removeFightSceneResource();
//    DATAPOOL->loadResource();
}


bool SceneManager::isOldScene()
{
    bool bRet = false;
    
    CCScene *pRunningScene = CCDirector::sharedDirector()->getRunningScene();
    
    if (pRunningScene != NULL) {
        if (m_pOldScene == pRunningScene) {
            bRet = true;
        }
        else
        {
            m_pOldScene = pRunningScene;
        }
    }
        
    return bRet;
}


void SceneManager::backFromTrans(SceneData &data)
{
    m_bTransEnabled = false;        
    m_pOldScene = CCDirector::sharedDirector()->getRunningScene();        
    replaceScene(data);
}


void SceneManager::dumpAll()
{
    for (int i = 0; i < m_vData.size(); i++) 
    {
        CCLOG("######### %d", m_vData.at(i).aTag);
    }
}

