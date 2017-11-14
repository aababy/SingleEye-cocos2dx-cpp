//
//  DataPool.cpp
//  SingleEye
//
//  Created by wangwx on 13-12-5.
//
//

#include "DataPool.h"
#include "IOControl.h"

#define SIGN_FILE         "sign.dat"
#define GOLD_LIMIT          (999999)


CDataPool::CDataPool()
{
    m_bRequest = false;
    
    init();
    
    InitCheckData();
    
    CCTime::gettimeofdayCocos2d(&m_runTime, NULL);
}
CDataPool::~CDataPool()
{
    freeHeroRecord();
    freeEnemyRecord();
    freeOutfitRecord();
    freeLevelRecord();
    
    unloadHeroData();
    unloadEnemyData();
    unloadOutfitData();
    
    saveAndFreeMessage();
    
    FreeCheckData();
}

CDataPool * CDataPool::m_pInstance = NULL;
CDataPool * CDataPool::sharedDataPool()
{
#if 1   //by luoxp
    static CDataPool instance;
    return &instance;
#else
    if (NULL == m_pInstance)
    {
        m_pInstance = new CDataPool();
    }
    
    return m_pInstance;
#endif
}
void CDataPool::purgeDataPool()
{
    CC_SAFE_DELETE(m_pInstance);
}

void CDataPool::init()
{
    zoom = 0.0;
    m_nHeroReference = 0;
    m_bIsShowPingLunYet = false;
    m_bGamePause = false;
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    zoom = size.height/Device_Height_IP5;
    CCLOG("zoom:%f",zoom);
    
    readFileForVector(m_vSystem, MES_SYSTEM_NAME);
    MsgSystem::Instance()->RegisterObjAndHandler(this, MSGSYS_HANLE_MSG(CDataPool::handleMsg));
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(CDataPool::checkMessage), this, 20, false);
}

#pragma mark - 属性类数据

void CDataPool::loadHeroData()
{
    if (m_nHeroNum > 0)
    {
        return;
    }
    
    m_pHeroData = CfgDataManage::getInstance()->getTotalHeroData();
    m_nHeroNum = CFG_DATA_MGR->getHeroNum();
}
void CDataPool::unloadHeroData()
{
    for (int i = 0; i < m_nHeroNum; i++)
    {
        Game_Hero_Data *p = m_pHeroData[i];
        CC_SAFE_DELETE(p);
    }
    CC_SAFE_DELETE_ARRAY(m_pHeroData);
    
    m_nHeroNum = 0;
}
void CDataPool::loadEnemyData()
{
    if (m_nEnemyNum > 0)
    {
        return;
    }

    m_pEnemyData = CfgDataManage::getInstance()->getTotalEnemyData();
    m_nEnemyNum = CFG_DATA_MGR->getEnemyNum();
}
void CDataPool::unloadEnemyData()
{
    for (int i = 0; i < m_nEnemyNum; i++)
    {
        Enemy_Data *p = m_pEnemyData[i];
        CC_SAFE_DELETE(p);
    }
    CC_SAFE_DELETE_ARRAY(m_pEnemyData);
    
    m_nEnemyNum = 0;
}
bool CDataPool::isValidOutfitId(int outfitId)
{
    return(outfitId >= First_Outfit_ID
           && outfitId <= 7011);
}
void CDataPool::loadOutfitData()
{
    if (m_nOutfitNum > 0)
    {
        return;
    }
    m_pOutfitData = CFG_DATA_MGR->getTotalOutfitData();
    m_nOutfitNum = CFG_DATA_MGR->getOutfitNum();
}
void CDataPool::unloadOutfitData()
{
    for (int i = 0; i < m_nOutfitNum; i++)
    {
        COutfitData *p = m_pOutfitData[i];
        CC_SAFE_DELETE(p);
    }
    CC_SAFE_DELETE_ARRAY(m_pOutfitData);

    m_nOutfitNum = 0;
}
Enemy_Data* CDataPool::getEnemyData(int nID)
{
    Enemy_Data *pEnemy = NULL;
    int index = nID - First_Enemy_ID;
    if(index>=0)
        pEnemy = m_pEnemyData[index];
    return pEnemy;
}

Game_Hero_Data* CDataPool::getHeroData(int nID)
{
    Game_Hero_Data *pHero = NULL;
    int index = nID - First_Hero_ID;
    if(index>=0)
        pHero = m_pHeroData[index];
    return pHero;
}
COutfitData* CDataPool::getOutfitData(int nID)
{
    if (!m_pOutfitData)
    {
        return NULL;
    }
    COutfitData *p = NULL;
    int index = nID - First_Outfit_ID;
    if(index>=0 && index < m_nOutfitNum)
    {
        p = m_pOutfitData[index];
    }
    return p;
}

Prop_Data* CDataPool::getPropData(int nID)
{
    if (!m_pOutfitData)
    {
        return NULL;
    }
    Prop_Data *data = NULL;
    int index = nID - First_Outfit_ID;
    CCLOG("INDEX:%d",index);
    if(index>=0)
    {
        data = new Prop_Data;
        data->mID = m_pOutfitData[index]->m_nId;
        data->mHp = m_pOutfitData[index]->m_nHp;
        sprintf(data->m_sourceName, "%s",m_pOutfitData[index]->m_pSourceName->getCString());
    }
    return data;
}

#pragma mark - 读写存档

void CDataPool::loadArchive()
{
#if TYPE_OF_SAVE_METHOND

    ClearSaveRecord();
    ReadSaveRecord();

#else
    
    if(false == CCUserDefault::sharedUserDefault()->getBoolForKey(_isHaveArchiveFile))
    {
        //初始参数;
        //xUser->pUserInfo->gemstone = 10;
        return;
    }
    
    char buf[20] = {0};
    
    firsttime = CCUserDefault::sharedUserDefault()->getBoolForKey(_firsttime);
    gold = CCUserDefault::sharedUserDefault()->getIntegerForKey(_gold, 0);
    energy = CCUserDefault::sharedUserDefault()->getFloatForKey(_energy, 0.0);
    xUser->pUserInfo->gemstone = CCUserDefault::sharedUserDefault()->getIntegerForKey(_gem, 0);
    
    xUser->pUserInfo->strength = CCUserDefault::sharedUserDefault()->getIntegerForKey(_power, 0);
    //m_iShareCounts = CCUserDefault::sharedUserDefault()->getIntegerForKey(ShareCount, 0);
    m_iContinueSign = CCUserDefault::sharedUserDefault()->getIntegerForKey(ContinueSign, 0);
    m_iTotalScore = CCUserDefault::sharedUserDefault()->getIntegerForKey(TotalScore, 0);
    
    m_bRookieGift = CCUserDefault::sharedUserDefault()->getBoolForKey(_rookie,false);
    
//    if (firsttime == true) {
//        //默认初始值
//        m_iPower = 10;
//    }
    
    
    m_bOpenBackMusic = CCUserDefault::sharedUserDefault()->getBoolForKey(_openBackMusic);
    m_bOpenSoundEffect = CCUserDefault::sharedUserDefault()->getBoolForKey(_openSoundEffect);
    
    bestTime = CCUserDefault::sharedUserDefault()->getIntegerForKey(_bestTime, 0);
    
    m_bCompleteTeachLevelOne = CCUserDefault::sharedUserDefault()->getBoolForKey(_teachLevelOne, false);
    m_bCompleteTeachLevelTwo = CCUserDefault::sharedUserDefault()->getBoolForKey(_teachLevelTwo, false);
    
    //副本关卡
    currentChapter = CCUserDefault::sharedUserDefault()->getIntegerForKey(_chapter,0);
    for (int i = 0; i<MAX_CHAPTER_NUM; ++i)
    {
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "currentLevel_%d",i+1);
        m_nCurrentLevel[i] = CCUserDefault::sharedUserDefault()->getIntegerForKey(buf);
    }
    m_nMaxUnlockChapter = CCUserDefault::sharedUserDefault()->getIntegerForKey(_unlockChapter,0);
    for (int i = 0; i<MAX_CHAPTER_NUM; ++i)
    {
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "unlockLevel_%d",i+1);
        m_nMaxUnlockLevel[i] = CCUserDefault::sharedUserDefault()->getIntegerForKey(buf);
    }
    //score
//    for (LevelRecordContainer::const_iterator iter = m_levelRecordList.begin(); iter != m_levelRecordList.end(); iter++ )
//    {
//        int key = iter->first;
//        LevelRecord *pData = iter->second;
//        if (pData)
//        {
//            memset(buf, 0, sizeof(buf));
//            sprintf(buf, "levelLock_%d", key);
//            pData->m_nScore = CCUserDefault::sharedUserDefault()->getIntegerForKey(buf);
//        }
//    }
    for (int i = 0; i<MAX_CHAPTER_NUM*LEVEL_NUM_OF_CHAPTER; ++i)
    {
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "levelScore_%d", i);
        LevelRecord *pData = m_pLevelRecord[i];
        if (pData)
        {
            pData->m_nScore = CCUserDefault::sharedUserDefault()->getIntegerForKey(buf);
        }
    }
    
    //英雄
    {
        HeroRecord heroRcd;
        int nHeroId = 0;
        for (int i = 0; i < m_nHeroNum; ++i)
        {
            memset(&heroRcd, 0, sizeof(heroRcd));
            nHeroId = First_Hero_ID + i;

            memset(buf, 0, sizeof(buf));
            sprintf(buf, "hero_unlock_%d", nHeroId);
            heroRcd.m_bUnlock = CCUserDefault::sharedUserDefault()->getBoolForKey(buf);
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "hero_level_%d", nHeroId);
            heroRcd.m_nLevel = CCUserDefault::sharedUserDefault()->getIntegerForKey(buf, 0);
            
            setHeroRecord(nHeroId, heroRcd);
        }
    }
    
    //雇佣兵
    {
        EnemyRecord enemyRcd;
        int nEnemyId = 0;
        for (int i = 0; i < m_nEnemyNum; ++i)
        {
            memset(&enemyRcd, 0, sizeof(enemyRcd));
            nEnemyId = First_Enemy_ID + i;
            
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "pet_unlock_%d", nEnemyId);
            enemyRcd.m_bUnlock = CCUserDefault::sharedUserDefault()->getBoolForKey(buf);
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "pet_level_%d", nEnemyId);
            enemyRcd.m_nLevel = CCUserDefault::sharedUserDefault()->getIntegerForKey(buf, 0);
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "pet_hp_%d", nEnemyId);
            enemyRcd.m_nHpPercent = CCUserDefault::sharedUserDefault()->getIntegerForKey(buf, 0);
            
            setEnemyRecord(nEnemyId, enemyRcd);
        }
    }
    
    //装备
    {
        OutfitRecord rcd;
        int nId = 0;
        for (int i = 0; i < m_nOutfitNum; ++i)
        {
            memset(&rcd, 0, sizeof(rcd));
            nId = First_Outfit_ID + i;
            
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "outfit_num_%d",nId);
            rcd.m_nNum = CCUserDefault::sharedUserDefault()->getIntegerForKey(buf, 0);
            
            setOutfitRecord(nId, rcd);
        }
    }
    
    //部署
    for (int i = 0; i < PLAN_HERO_NUM; ++i)
    {
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "plan_hero_%d",i);
        m_nPlanHero[i] = CCUserDefault::sharedUserDefault()->getIntegerForKey(buf);
    }
    for (int i = 0; i < PLAN_OUTFIT_NUM; ++i)
    {
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "plan_outfit_%d",i);
        m_nPlanOutfit[i] = CCUserDefault::sharedUserDefault()->getIntegerForKey(buf);
    }
    for (int i = 0; i < PLAN_PET_NUM; ++i)
    {
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "plan_pet_1_%d",i);
        m_nPlanPet_1[i] = CCUserDefault::sharedUserDefault()->getIntegerForKey(buf);
       
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "plan_pet_2_%d",i);
        m_nPlanPet_2[i] = CCUserDefault::sharedUserDefault()->getIntegerForKey(buf);
    }
    //新手引导
    for(int i = 0; i < TEACH_STEP_LIMIT; i++)
    {
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "teach_step_%d",i);
        m_bTeachStepOver[i] = CCUserDefault::sharedUserDefault()->getBoolForKey(buf);
    }
    
#endif
}
//测试用：手动设置数据
void CDataPool::manualArchive()
{
#if TYPE_OF_SAVE_METHOND
    
    DebugSaveRecord();
    
#else

    return;
    
#endif//
}

void CDataPool::saveArchive()
{
#if TYPE_OF_SAVE_METHOND
    
    WriteSaveRecord();
    
#else
    
    char buf[20] = {0};
    
    CCUserDefault::sharedUserDefault()->setBoolForKey(_firsttime, firsttime);
    CCUserDefault::sharedUserDefault()->setIntegerForKey(_gold,gold);
    CCUserDefault::sharedUserDefault()->setFloatForKey(_energy, energy);
    CCUserDefault::sharedUserDefault()->setIntegerForKey(_gem, xUser->pUserInfo->gemstone);
    
    CCUserDefault::sharedUserDefault()->setIntegerForKey(_power, xUser->pUserInfo->strength);
    //CCUserDefault::sharedUserDefault()->setIntegerForKey(ShareCount, m_iShareCounts);
    CCUserDefault::sharedUserDefault()->setIntegerForKey(ContinueSign, m_iContinueSign);
    CCUserDefault::sharedUserDefault()->setIntegerForKey(TotalScore, m_iTotalScore);
    
    //消息
    writeFileForVector(m_vSystem, MES_SYSTEM_NAME);
    
    CCUserDefault::sharedUserDefault()->setIntegerForKey(_bestTime, bestTime);
    CCUserDefault::sharedUserDefault()->setBoolForKey(_rookie, m_bRookieGift);
    
    
    CCUserDefault::sharedUserDefault()->setBoolForKey(_openBackMusic, m_bOpenBackMusic);
    CCUserDefault::sharedUserDefault()->setBoolForKey(_openSoundEffect, m_bOpenSoundEffect);
    
    CCUserDefault::sharedUserDefault()->setBoolForKey(_teachLevelOne, m_bCompleteTeachLevelOne);
    CCUserDefault::sharedUserDefault()->setBoolForKey(_teachLevelTwo, m_bCompleteTeachLevelTwo);
    
    //副本关卡
    CCUserDefault::sharedUserDefault()->setIntegerForKey(_chapter,currentChapter);
    for (int i = 0; i<MAX_CHAPTER_NUM; ++i)
    {
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "currentLevel_%d",i+1);
        CCUserDefault::sharedUserDefault()->setIntegerForKey(buf, m_nCurrentLevel[i]);
    }
    CCUserDefault::sharedUserDefault()->setIntegerForKey(_unlockChapter, m_nMaxUnlockChapter);
    for (int i = 0; i<MAX_CHAPTER_NUM; ++i)
    {
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "unlockLevel_%d",i+1);
        CCUserDefault::sharedUserDefault()->setIntegerForKey(buf, m_nMaxUnlockLevel[i]);
    }
    //score
    {
//        for (LevelRecordContainer::const_iterator iter = m_levelRecordList.begin(); iter != m_levelRecordList.end(); iter++ )
//        {
//            int key = iter->first;
//            LevelRecord *pData = iter->second;
//            if (pData)
//            {
//                memset(buf, 0, sizeof(buf));
//                sprintf(buf, "levelScore_%d", key);
//                CCUserDefault::sharedUserDefault()->setIntegerForKey(buf, pData->m_nScore);
//            }
//        }

        LevelRecord *pRcd = NULL;
        int nId = 0;
        for (int i = 0; i < MAX_CHAPTER_NUM*LEVEL_NUM_OF_CHAPTER; ++i)
        {
            pRcd = m_pLevelRecord[i];
            if(pRcd)
            {
                memset(buf, 0, sizeof(buf));
                sprintf(buf, "levelScore_%d", i);
                CCUserDefault::sharedUserDefault()->setIntegerForKey(buf, pRcd->m_nScore);
            }
        }
    }
    
    //英雄
    {
        HeroRecord *pRcd = NULL;
        int nId = 0;
        for (int i = 0; i < m_nHeroNum; ++i)
        {
            nId = First_Hero_ID + i;
            pRcd = getHeroRecord(nId);
            if(pRcd)
            {
                memset(buf, 0, sizeof(buf));
                sprintf(buf, "hero_unlock_%d", nId);
                CCUserDefault::sharedUserDefault()->setBoolForKey(buf, pRcd->m_bUnlock);
                memset(buf, 0, sizeof(buf));
                sprintf(buf, "hero_level_%d", nId);
                CCUserDefault::sharedUserDefault()->setIntegerForKey(buf, pRcd->m_nLevel);
            }
        }
    }
    
    //雇佣兵
    {
        EnemyRecord *pRcd = NULL;
        int nId = 0;
        for (int i = 0; i < m_nEnemyNum; ++i)
        {
            nId = First_Enemy_ID + i;
            pRcd = getEnemyRecord(nId);
            if(pRcd)
            {
                memset(buf, 0, sizeof(buf));
                sprintf(buf, "pet_unlock_%d", nId);
                CCUserDefault::sharedUserDefault()->setBoolForKey(buf, pRcd->m_bUnlock);
                memset(buf, 0, sizeof(buf));
                sprintf(buf, "pet_level_%d", nId);
                CCUserDefault::sharedUserDefault()->setIntegerForKey(buf, pRcd->m_nLevel);
                memset(buf, 0, sizeof(buf));
                sprintf(buf, "pet_hp_%d", nId);
                CCUserDefault::sharedUserDefault()->setIntegerForKey(buf, pRcd->m_nHpPercent);
            }
        }
    }
    
    //装备
    {
        OutfitRecord *pRcd = NULL;
        int nId = 0;
        for (int i = 0; i < m_nOutfitNum; ++i)
        {
            nId = First_Outfit_ID + i;
            pRcd = getOutfitRecord(nId);
            if(pRcd)
            {
                memset(buf, 0, sizeof(buf));
                sprintf(buf, "outfit_num_%d",nId);
                CCUserDefault::sharedUserDefault()->setIntegerForKey(buf, pRcd->m_nNum);
            }
        }
    }
    
    //部署
    for (int i = 0; i < PLAN_HERO_NUM; ++i)
    {
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "plan_hero_%d",i);
        CCUserDefault::sharedUserDefault()->setIntegerForKey(buf, m_nPlanHero[i]);
    }
    for (int i = 0; i < PLAN_OUTFIT_NUM; ++i)
    {
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "plan_outfit_%d",i);
        CCUserDefault::sharedUserDefault()->setIntegerForKey(buf, m_nPlanOutfit[i]);
    }
    for (int i = 0; i < PLAN_PET_NUM; ++i)
    {
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "plan_pet_1_%d",i);
        CCUserDefault::sharedUserDefault()->setIntegerForKey(buf, m_nPlanPet_1[i]);
        
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "plan_pet_2_%d",i);
        CCUserDefault::sharedUserDefault()->setIntegerForKey(buf, m_nPlanPet_2[i]);
    }
    //新手引导
    for(int i = 0; i < TEACH_STEP_LIMIT; i++)
    {
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "teach_step_%d",i);
        CCUserDefault::sharedUserDefault()->setBoolForKey(buf, m_bTeachStepOver[i]);
    }
    CCUserDefault::sharedUserDefault()->setBoolForKey(_isHaveArchiveFile, true);
    
    CCUserDefault::sharedUserDefault()->flush();
    
#endif//
}
//校验存档数据和内存数据是否一致，防修改器
void CDataPool::checkData()
{
    DATAPOOL->ReadCheckData();
    if(m_pCheckBakData)
    {
        int goldBak = m_pCheckBakData->gold;
        int gemBak = m_pCheckBakData->gem;
        CCLOG("checkData: bakGold=%d,curGold=%d",goldBak,gold);
        CCLOG("checkData: bakGem=%d,curGem=%d",gemBak, xUser->pUserInfo->gemstone);
        if (gold != goldBak
            || xUser->pUserInfo->gemstone != gemBak)
        {
            CCLOG("ERROR:exit(0)");
            exit(0);
        }
    }
}
/*
void CDataPool::InitSaveRecord()
{
    m_pSaveRcd = new CSaveRecord();
    ClearSaveRecord();
}
void CDataPool::FreeSaveRecord()
{
    ENGINE_SAFE_DEL(m_pSaveRcd);
}
*/

void CDataPool::ReadSaveRecord()
{
    byte *pBuf = NULL;
    int nLen = 0;
    
    do {
        CFIO::FileRead(SAVE_RECORD_FILE, &pBuf, nLen);
        CC_BREAK_IF(!pBuf);
        
        byte *pTmp = pBuf;
        
        string mstrflag = ReadUTFString(&pTmp);
        if(mstrflag.compare(DEFINED_SAVEDATA_DESFLAG) == 0)//数据已加密
        {
            m_DesVer = Read32(&pTmp);
            DesSystem *pDesSys = DesSystem::SharedSceneSystem();
            pDesSys->SetKey(pDesSys->GetDesPwdByVersion(m_DesVer,true));
            pDesSys->Run((char *)pTmp, (char *)pTmp, nLen - DEFINED_SAVEDATA_DESFLAG_LEN,
                                                DES_DECRYPT);
            pDesSys->SetKey(pDesSys->GetDesPwdByVersion(m_DesVer,false));
            pDesSys->Run((char *)pTmp, (char *)pTmp, nLen - DEFINED_SAVEDATA_DESFLAG_LEN,
                                                DES_DECRYPT);
        }
        
        int nCheckNum = Read32(&pTmp);
        CC_BREAK_IF(nCheckNum != MAGIC_NUM);
        
        firsttime = Read8(&pTmp);
        gold = Read32(&pTmp);
        string energyName = ReadUTFString(&pTmp);
        stringstream os;
        os << energyName;
        os >> energy;
        
        xUser->pUserInfo->gemstone = Read32(&pTmp);
        xUser->pUserInfo->strength = Read32(&pTmp);
        
        m_iShareCounts = Read32(&pTmp);
        m_iContinueSign = Read32(&pTmp);
        m_iTotalScore = Read32(&pTmp);
        xUser->totalScore = m_iTotalScore; //赋值
        
        m_bGetBossPrize = Read8(&pTmp);
        
        m_bRookieGift = Read8(&pTmp);
        
        m_bOpenBackMusic = Read8(&pTmp);
        m_bOpenSoundEffect = Read8(&pTmp);
        
        bestTime = Read32(&pTmp);
        
        m_bCompleteTeachLevelOne = Read8(&pTmp);
        m_bCompleteTeachLevelTwo = Read8(&pTmp);
        m_bPingLunYet = Read8(&pTmp);
//        
//        pTmp += sizeof(firsttime) + sizeof(gold);
//        string energyName = ReadUTFString(&pTmp);
//        stringstream os;
//        
//        pTmp += sizeof(xUser->pUserInfo->gemstone)
//        + sizeof(xUser->pUserInfo->strength)
//        + sizeof(m_iShareCounts)
//        + sizeof(m_iContinueSign)
//        + sizeof(m_iTotalScore)
//        + sizeof(xUser->totalScore)
//        + sizeof(m_bGetBossPrize)
//        + sizeof(m_bRookieGift)
//        + sizeof(m_bOpenBackMusic)
//        + sizeof(m_bOpenSoundEffect)
//        + sizeof(bestTime)
//        + sizeof(m_bCompleteTeachLevelOne)
//        + sizeof(m_bCompleteTeachLevelTwo)
//        + sizeof(m_bPingLunYet)
//        ;
        
        //副本关卡
        currentChapter = Read32(&pTmp);;
        for (int i = 0; i < MAX_CHAPTER_NUM; ++i)
        {
            m_nCurrentLevel[i] = Read32(&pTmp);
        }
        m_nMaxUnlockChapter = Read32(&pTmp);
        for (int i = 0; i<MAX_CHAPTER_NUM; ++i)
        {
            m_nMaxUnlockLevel[i] = Read32(&pTmp);
        }
        for (int i = 0; i < MAX_CHAPTER_NUM; i++)
        {
            for (int j = 0; j < LEVEL_NUM_OF_CHAPTER; j++)
            {
                LevelRecord rcd;
                rcd.m_nScore = Read32(&pTmp);
                setLevelRecord(i, j, rcd);
            }
        }
        
        //英雄
        {
            HeroRecord heroRcd;
            int nHeroId = 0;
            for (int i = 0; i < m_nHeroNum; ++i)
            {
                nHeroId = First_Hero_ID + i;
                memset(&heroRcd, 0, sizeof(heroRcd));
                heroRcd.m_bUnlock = Read8(&pTmp);;
                heroRcd.m_nLevel = Read32(&pTmp);
                
                setHeroRecord(nHeroId, heroRcd);
            }
        }
        
        //雇佣兵
        {
            EnemyRecord enemyRcd;
            int nEnemyId = 0;
            for (int i = 0; i < m_nEnemyNum; ++i)
            {
                nEnemyId = First_Enemy_ID + i;
                memset(&enemyRcd, 0, sizeof(enemyRcd));
                enemyRcd.m_bUnlock = Read8(&pTmp);
                enemyRcd.m_nLevel = Read32(&pTmp);
                enemyRcd.m_nHpPercent = Read32(&pTmp);
                
                setEnemyRecord(nEnemyId, enemyRcd);
            }
        }
        
        //装备
        {
            OutfitRecord rcd;
            int nId = 0;
            for (int i = 0; i < m_nOutfitNum; ++i)
            {
                nId = First_Outfit_ID + i;
                memset(&rcd, 0, sizeof(rcd));
                rcd.m_nNum = Read32(&pTmp);
                
                setOutfitRecord(nId, rcd);
            }
        }
        
        //部署
        for (int i = 0; i < PLAN_HERO_NUM; ++i)
        {
            m_nPlanHero[i] = Read32(&pTmp);
        }
        for (int i = 0; i < PLAN_OUTFIT_NUM; ++i)
        {
            m_nPlanOutfit[i] = Read32(&pTmp);
        }
        for (int i = 0; i < PLAN_PET_NUM; ++i)
        {
            m_nPlanPet_1[i] = Read32(&pTmp);
            m_nPlanPet_2[i] = Read32(&pTmp);
        }
        
        //新手引导
        for(int i = 0; i < TEACH_STEP_LIMIT; i++)
        {
            m_bTeachStepOver[i] = Read8(&pTmp);
        }
    
    } while (0);
    
    ENGINE_SAFE_DEL(pBuf);

}
void CDataPool::WriteSaveRecord()
{
#if 0
    if(!m_pSaveRcd)
    {
        return;
    }
    
    m_pSaveRcd->gemstone = xUser->pUserInfo->gemstone;
    m_pSaveRcd->strength = xUser->pUserInfo->strength;
    CFIO::FileWrite(SAVE_RECORD_FILE,(byte *)m_pSaveRcd,sizeof(CSaveRecord));
#endif
    
#if 1
    byte *pBuf = NULL;
    int nBufSize = 0;
    int nDataSize = 0;
    int nMagicNum = MAGIC_NUM;
    
    CCString *pEnergyStr = CCString::createWithFormat("%f", energy);
    std::string energyName = pEnergyStr->getCString();
 
    //size
    nDataSize += sizeof(nMagicNum);
    nDataSize += sizeof(firsttime);
    nDataSize += sizeof(gold);
    nDataSize += (2 + energyName.length());//sizeof(energy);
    nDataSize += sizeof(xUser->pUserInfo->gemstone);
    nDataSize += sizeof(xUser->pUserInfo->strength);
    nDataSize += sizeof(m_iShareCounts);
    nDataSize += sizeof(m_iContinueSign);
    nDataSize += sizeof(m_iTotalScore);
    nDataSize += sizeof(m_bGetBossPrize);    
    nDataSize += sizeof(m_bRookieGift);
    nDataSize += sizeof(m_bOpenBackMusic);
    nDataSize += sizeof(m_bOpenSoundEffect);
    nDataSize += sizeof(bestTime);
    nDataSize += sizeof(m_bCompleteTeachLevelOne);
    nDataSize += sizeof(m_bCompleteTeachLevelTwo);
    nDataSize += sizeof(m_bPingLunYet);
    nDataSize += sizeof(currentChapter);
    nDataSize += sizeof(m_nCurrentLevel[0]) * MAX_CHAPTER_NUM;//m_nCurrentLevel[i]
    nDataSize += sizeof(m_nMaxUnlockChapter);
    nDataSize += sizeof(m_nMaxUnlockLevel[0]) * MAX_CHAPTER_NUM;//m_nMaxUnlockLevel[i]
    nDataSize += sizeof(LevelRecord) * (MAX_CHAPTER_NUM*LEVEL_NUM_OF_CHAPTER);//m_pLevelRecord[i]
    nDataSize += sizeof(HeroRecord)*m_nHeroNum;
    nDataSize += sizeof(EnemyRecord)*m_nEnemyNum;
    nDataSize += sizeof(OutfitRecord)*m_nOutfitNum;
    nDataSize += sizeof(m_nPlanHero[0])*PLAN_HERO_NUM;//m_nPlanHero[i]
    nDataSize += sizeof(m_nPlanOutfit[0])*PLAN_OUTFIT_NUM;//m_nPlanOutfit[i]
    nDataSize += sizeof(m_nPlanPet_1[0])*PLAN_PET_NUM;//m_nPlanPet_1[i]
    nDataSize += sizeof(m_nPlanPet_2[0])*PLAN_PET_NUM;//m_nPlanPet_2[i]
    nDataSize += sizeof(m_bTeachStepOver[0])*TEACH_STEP_LIMIT;//m_bTeachStepOver[i]
    nDataSize += 512;  //待扩展用
    
    nBufSize = DEFINED_SAVEDATA_DESFLAG_LEN + nDataSize;
    
    //wait
    pBuf = new byte[nBufSize];
    memset(pBuf, 0, nBufSize);
    byte *pDesBuf = NULL;
    
    //copy
    {
        byte *pTmp = pBuf;
        
        WriteUTFString(&pTmp, DEFINED_SAVEDATA_DESFLAG);//加密标记
        Write32(&pTmp, m_DesVer);
        pDesBuf = pTmp;
        
        Write32(&pTmp, nMagicNum);
        
        Write8(&pTmp, firsttime);
        Write32(&pTmp, gold);
        WriteUTFString(&pTmp, energyName);
        
        Write32(&pTmp, xUser->pUserInfo->gemstone);
        Write32(&pTmp, xUser->pUserInfo->strength);
        Write32(&pTmp, m_iShareCounts);
        
        Write32(&pTmp, m_iContinueSign);
        Write32(&pTmp, xUser->totalScore);          //直接写入User中的数据.
        Write8(&pTmp, m_bGetBossPrize);
        Write8(&pTmp, m_bRookieGift);
        Write8(&pTmp, m_bOpenBackMusic);
        Write8(&pTmp, m_bOpenSoundEffect);
        Write32(&pTmp, bestTime);
        Write8(&pTmp, m_bCompleteTeachLevelOne);
        Write8(&pTmp, m_bCompleteTeachLevelTwo);
        Write8(&pTmp, m_bPingLunYet);
        
        //副本关卡
        Write32(&pTmp, currentChapter);
        for (int i = 0; i < MAX_CHAPTER_NUM; i++)
        {
            Write32(&pTmp, m_nCurrentLevel[i]);
        }
        
        Write32(&pTmp, m_nMaxUnlockChapter);
        for (int i = 0; i < MAX_CHAPTER_NUM; i++)
        {
            Write32(&pTmp, m_nMaxUnlockLevel[i]);
        }
        
        for (int i = 0; i < MAX_CHAPTER_NUM; ++i)
        {
            for (int j = 0; j < LEVEL_NUM_OF_CHAPTER; j++)
            {
                LevelRecord *pRcd = getLevelRecord(i, j);
                Write32(&pTmp, pRcd->m_nScore);
            }
        }
        
        //英雄
        for (int i = 0; i < m_nHeroNum; ++i)
        {
            HeroRecord *pRcd = m_pHeroRecord[i];
            
            Write8(&pTmp, pRcd->m_bUnlock);
            Write32(&pTmp, pRcd->m_nLevel);
        }
        
        //雇佣兵
        for (int i = 0; i < m_nEnemyNum; ++i)
        {
            EnemyRecord *pRcd = m_pEnemyRecord[i];
            Write8(&pTmp, pRcd->m_bUnlock);
            Write32(&pTmp, pRcd->m_nLevel);
            Write32(&pTmp, pRcd->m_nHpPercent);
        }
        
        //装备
        for (int i = 0; i < m_nOutfitNum; ++i)
        {
            OutfitRecord *pRcd = m_pOutfitRecord[i];
            Write32(&pTmp, pRcd->m_nNum);
        }
        
        //部署
        for (int i = 0; i < PLAN_HERO_NUM; ++i)
        {
            Write32(&pTmp, m_nPlanHero[i]);
        }
        for (int i = 0; i < PLAN_OUTFIT_NUM; ++i)
        {
            Write32(&pTmp, m_nPlanOutfit[i]);
        }
        for (int i = 0; i < PLAN_PET_NUM; ++i)
        {
            Write32(&pTmp, m_nPlanPet_1[i]);
            Write32(&pTmp, m_nPlanPet_2[i]);
        }
        
        //新手引导
        for(int i = 0; i < TEACH_STEP_LIMIT; i++)
        {
            Write8(&pTmp, m_bTeachStepOver[i]);
        }
    }
    //加密
    DesSystem *pDesSys = DesSystem::SharedSceneSystem();
    pDesSys->SetKey(pDesSys->GetDesPwdByVersion(m_DesVer,false));
    pDesSys->Run((char *)pDesBuf, (char *)pDesBuf, nDataSize,DES_ENCRYPT);
    pDesSys->SetKey(pDesSys->GetDesPwdByVersion(m_DesVer,true));
    pDesSys->Run((char *)pDesBuf, (char *)pDesBuf,nDataSize,DES_ENCRYPT);
    //write
    bool bRst = CFIO::FileWrite(SAVE_RECORD_FILE,(byte *)pBuf, nBufSize);
    CCLOG("@WriteSaveRecord() fileWriteRst=%d",bRst);
    //end
    ENGINE_SAFE_DEL(pBuf);
#endif
};
void CDataPool::ClearSaveRecord()
{
    m_DesVer = 0;
    
    //初始化所有存檔相關變量
    firsttime = true;
    m_bOpenBackMusic = true;
    m_bOpenSoundEffect = true;
    
    xUser->pUserInfo->gemstone = 0;
    xUser->pUserInfo->strength = 0;
    gold = 0;
    energy = 0;
    
    xUser->pUserInfo->gemstone = 0;
    xUser->pUserInfo->strength = 10;    //默认10个体力
    
    m_iShareCounts = 0;
    m_iContinueSign = 0;
    m_iTotalScore = 0;
    m_bGetBossPrize = false;    
    
    m_bRookieGift = false;
    
    m_bOpenBackMusic = true;
    m_bOpenSoundEffect = true;
    
    bestTime = 0;
    
    m_bCompleteTeachLevelOne = false;
    m_bCompleteTeachLevelTwo = false;
    m_bPingLunYet = false;
    
    currentChapter = 0;
    memset(m_nCurrentLevel, 0, sizeof(m_nCurrentLevel));
    m_nMaxUnlockChapter = 0;
    memset(m_nMaxUnlockLevel, 0, sizeof(m_nMaxUnlockLevel));
    for (int i = 0; i < MAX_CHAPTER_NUM; i++)
    {
        for (int j = 0; j < LEVEL_NUM_OF_CHAPTER; j++)
        {
            LevelRecord rcd;
            rcd.m_nScore = 0;
            setLevelRecord(i, j, rcd);
        }
    }
    
    {
        HeroRecord heroRcd;
        int nHeroId = 0;
        for (int i = 0; i < m_nHeroNum; ++i)
        {
            nHeroId = First_Hero_ID + i;
            memset(&heroRcd, 0, sizeof(heroRcd));
            heroRcd.m_bUnlock = (Hero_Type_PingGuo == nHeroId);//苹果默认解锁
            heroRcd.m_nLevel = 0;
            
            setHeroRecord(nHeroId, heroRcd);
        }
    }
    
    {
        EnemyRecord enemyRcd;
        int nEnemyId = 0;
        for (int i = 0; i < m_nEnemyNum; ++i)
        {
            nEnemyId = First_Enemy_ID + i;
            memset(&enemyRcd, 0, sizeof(enemyRcd));
            enemyRcd.m_bUnlock = false;
            enemyRcd.m_nLevel = 0;
            enemyRcd.m_nHpPercent = 100;
            
            setEnemyRecord(nEnemyId, enemyRcd);
        }
    }

    {
        OutfitRecord rcd;
        int nId = 0;
        for (int i = 0; i < m_nOutfitNum; ++i)
        {
            nId = First_Outfit_ID + i;
            memset(&rcd, 0, sizeof(rcd));
            rcd.m_nNum = 0;
            
            setOutfitRecord(nId, rcd);
        }
    }

    memset(m_nPlanHero, -1, sizeof(m_nPlanHero));
    memset(m_nPlanOutfit, -1, sizeof(m_nPlanOutfit));
    memset(m_nPlanPet_1, -1, sizeof(m_nPlanPet_1));
    memset(m_nPlanPet_2, -1, sizeof(m_nPlanPet_2));    

    memset(m_bTeachStepOver, 0, sizeof(m_bTeachStepOver));


}
void CDataPool::DebugSaveRecord()//调试数据
{
//    return;
    m_bOpenBackMusic = false;
    m_bOpenSoundEffect = false;
    xUser->pUserInfo->gemstone = 99999;
    for (int i = 0; i < TEACH_STEP_LIMIT; i++)
    {
        m_bTeachStepOver[i] = true;
    }
    m_nMaxUnlockChapter = 4;
    m_nMaxUnlockLevel[0]=9;
    m_nMaxUnlockLevel[1]=9;
    m_nMaxUnlockLevel[2]=9;
    m_nMaxUnlockLevel[3]=9;
    m_nMaxUnlockLevel[4]=9;
    
    freshLevelScore(0, 2, 3);
    freshLevelScore(0, 3, 2);
    freshLevelScore(0, 4, 1);
    freshLevelScore(0, 5, 2);
    freshLevelScore(0, 6, 3);
    freshLevelScore(0, 7, 1);
    
    return;
    
    firsttime = 3;
    gold = 10086;
    energy = 3.14159262;

    xUser->pUserInfo->strength = 126;
    m_iShareCounts = 711;
    
    m_iContinueSign = 8;
    m_iTotalScore = 321525;
    m_bRookieGift = 3;
//    Write8(&pTmp, m_bOpenBackMusic);
//    Write8(&pTmp, m_bOpenSoundEffect);
//    Write32(&pTmp, bestTime);
//    Write8(&pTmp, m_bCompleteTeachLevelOne);
//    Write8(&pTmp, m_bCompleteTeachLevelTwo);
    
    //副本关卡
    currentChapter = 3;
    for (int i = 0; i < MAX_CHAPTER_NUM; i++)
    {
        m_nCurrentLevel[i] = 3;
    }
    
    m_nMaxUnlockChapter = 4;
    for (int i = 0; i < MAX_CHAPTER_NUM; i++)
    {
        m_nMaxUnlockLevel[i] = 4;
    }
    
    for (int i = 0; i < MAX_CHAPTER_NUM; ++i)
    {
        for (int j = 0; j < LEVEL_NUM_OF_CHAPTER; j++)
        {
            LevelRecord *pRcd = getLevelRecord(i, j);
            pRcd->m_nScore = 813;
        }
    }
    //英雄
    {
        HeroRecord heroRcd;
        int nHeroId = 0;
        for (int i = 0; i < m_nHeroNum; ++i)
        {
            nHeroId = First_Hero_ID + i;
            memset(&heroRcd, 0, sizeof(heroRcd));
            heroRcd.m_bUnlock = 3;
            heroRcd.m_nLevel = 4;
            
            setHeroRecord(nHeroId, heroRcd);
        }
    }
    
    //雇佣兵
    {
        EnemyRecord enemyRcd;
        int nEnemyId = 0;
        for (int i = 0; i < m_nEnemyNum; ++i)
        {
            nEnemyId = First_Enemy_ID + i;
            memset(&enemyRcd, 0, sizeof(enemyRcd));
            enemyRcd.m_bUnlock = 3;
            enemyRcd.m_nLevel = 5;
            enemyRcd.m_nHpPercent = 50;
            
            setEnemyRecord(nEnemyId, enemyRcd);
        }
    }
    
    //装备
    {
        OutfitRecord rcd;
        int nId = 0;
        for (int i = 0; i < m_nOutfitNum; ++i)
        {
            nId = First_Outfit_ID + i;
            memset(&rcd, 0, sizeof(rcd));
            rcd.m_nNum = 8;
            
            setOutfitRecord(nId, rcd);
        }
    }
    
    //部署
    for (int i = 0; i < PLAN_HERO_NUM; ++i)
    {
        m_nPlanHero[i] = First_Hero_ID;
    }
    for (int i = 0; i < PLAN_OUTFIT_NUM; ++i)
    {
        m_nPlanOutfit[i] = First_Outfit_ID;
    }
    for (int i = 0; i < PLAN_PET_NUM; ++i)
    {
        m_nPlanPet_1[i] = First_Enemy_ID;
        m_nPlanPet_2[i] = First_Enemy_ID+1;
    }
    
    //新手引导
    for(int i = 0; i < TEACH_STEP_LIMIT; i++)
    {
        m_bTeachStepOver[i] = 3;
    }
    
    return;
    
    for (int i = 0; i < TEACH_STEP_LIMIT; i++)
    {
        m_bTeachStepOver[i] = true;
    }
    
    return;
    
    xUser->pUserInfo->gemstone = 2002;
    xUser->pUserInfo->strength = 2006;
    
    m_iShareCounts = 2007;
    m_iContinueSign = 2009;
    m_iTotalScore = 2014;
    
    m_bRookieGift = 3;
    
    m_bOpenBackMusic = 4;
    m_bOpenSoundEffect = 5;
    
    bestTime = 2013;
    
    m_bCompleteTeachLevelOne = 6;
    m_bCompleteTeachLevelTwo = 7;
}


#pragma mark - 属性快捷获取

int CDataPool::getLevelScoreStar(int nChapter, int nLevel)
{
    LevelRecord *pRcd = getLevelRecord(nChapter, nLevel);
    if (pRcd)
    {
        return judgeLevelScoreStar(pRcd->m_nScore);
    }
    
    return 0;
}

//满分100
//0~10 0星 10~36一星 36~72二星，72+三星
int CDataPool::judgeLevelScoreStar(int nScore)
{
    if (nScore <= 10)
    {
        return 0;
    }
    else if(nScore <= 36)
    {
        return 1;
    }
    else if(nScore <= 72)
    {
        return 2;
    }
    else
    {
        return 3;
    }
}

int CDataPool::findLockHero(int level)
{
    int res = 0;  //未找到返回0
    if(m_pHeroData)
    {
        CCLOG("findLockHero");
        CCLOG("m_nHeroNum:%d",m_nHeroNum);
        for (int i=0; i<m_nHeroNum; i++)
        {
            if(m_pHeroData[i]->mUnLockLevel == level)
                res = m_pHeroData[i]->mType;
        }
    }
    return  res;
}
bool CDataPool::findHeroUnlock(int nId)
{
    HeroRecord *pData = DATAPOOL->getHeroRecord(nId);
    
    if (pData)
    {
        return pData->m_bUnlock;
    }
    
    return false;
}
bool CDataPool::findPetUnlock(int nId)
{
    EnemyRecord *pData = DATAPOOL->getEnemyRecord(nId);
    
    if (pData)
    {
        return pData->m_bUnlock;
    }
    
    return false;
}

int CDataPool::findHeroLevel(int nId)
{
    HeroRecord *pData = DATAPOOL->getHeroRecord(nId);
    
    if (pData)
    {
        return pData->m_nLevel;
    }
    
    return 0;
}
int CDataPool::findPetLevel(int nId)
{
    EnemyRecord *pData = DATAPOOL->getEnemyRecord(nId);
    
    if (pData)
    {
        return pData->m_nLevel;
    }
    
    return 0;
}

int CDataPool::getPlanHero(int index)
{
    if (index >= PLAN_HERO_NUM)
    {
        return -2;
    }
    
    return m_nPlanHero[index];
}
int CDataPool::getPlanOutfit(int index)
{
    if (index >= PLAN_OUTFIT_NUM)
    {
        return -2;
    }
    
    return m_nPlanOutfit[index];
}
//检查已部署道具是否已用完，用完则设置为空
void CDataPool::checkPlanOutfit()
{
    int nId = 0;
    for (int i = 0; i < PLAN_OUTFIT_NUM; i++)
    {
        nId = m_nPlanOutfit[i];
        if (nId <= 0)
        {
            continue;
        }
        OutfitRecord *pRcd = getOutfitRecord(nId);
        if (pRcd && pRcd->m_nNum <=0 )
        {
            m_nPlanOutfit[i] = 0;
        }
    }
}
int CDataPool::getPlanPet(int index, int sub)
{
    if (index >= PLAN_PET_NUM)
    {
        return -2;
    }
    
    if (1 == sub)
    {
        return m_nPlanPet_1[index];
        
    }
    else if(2 == sub)
    {
        return m_nPlanPet_2[index];
    }
    
    return -2;

}

#pragma mark - 部署

void CDataPool::unlockPlanHero(int index)
{
    if (index >= PLAN_HERO_NUM)
    {
        return;
    }
    if (-1 == m_nPlanHero[index])
    {
        m_nPlanHero[index] = 0;
    }
}
void CDataPool::unlockPlanOutfit(int index)
{
    if (index >= PLAN_OUTFIT_NUM)
    {
        return;
    }
    if (-1 == m_nPlanOutfit[index])
    {
        m_nPlanOutfit[index] = 0;
    }
}
void CDataPool::unlockPlanPet(int index, int sub)
{
    if (index >= PLAN_PET_NUM)
    {
        return;
    }
    
    if (1 == sub)
    {
        if (-1 == m_nPlanPet_1[index])
        {
            m_nPlanPet_1[index] = 0;
        }
    }
    else if(2 == sub)
    {
        if (-1 == m_nPlanPet_2[index])
        {
            m_nPlanPet_2[index] = 0;
        }
    }
}
void CDataPool::setPlanHero(int index, int value)
{
    if (index >= PLAN_HERO_NUM)
    {
        return;
    }
    unsetPlanHero(value);
    m_nPlanHero[index] = value;
}
void CDataPool::setPlanOutfit(int index, int value)
{
    if (index >= PLAN_OUTFIT_NUM)
    {
        return;
    }
    unsetPlanOutfit(value);
    m_nPlanOutfit[index] = value;
}
void CDataPool::setPlanPet(int index, int value, int sub)
{
    if (index >= PLAN_PET_NUM)
    {
        return;
    }
    
    if (1 == sub)
    {
        unsetPlanPet(value);
        m_nPlanPet_1[index] = value;
    }
    else if(2 == sub)
    {
        unsetPlanPet(value);
        m_nPlanPet_2[index] = value;
    }
}
void CDataPool::unsetPlanHero(int value)
{
    if (value <= 0)
    {
        return;
    }
    for (int i = 0; i < PLAN_HERO_NUM; ++i)
    {
        if (value == m_nPlanHero[i])
        {
            m_nPlanHero[i] = 0;
        }
    }
}
void CDataPool::unsetPlanOutfit(int value)
{
    if (value <= 0)
    {
        return;
    }
    for (int i = 0; i < PLAN_OUTFIT_NUM; ++i)
    {
        if (value == m_nPlanOutfit[i])
        {
            m_nPlanOutfit[i] = 0;
        }
    }
}
void CDataPool::unsetPlanPet(int value)
{
    if (value <= 0)
    {
        return;
    }
    for (int i = 0; i < PLAN_PET_NUM; ++i)
    {
        if (value == m_nPlanPet_1[i])
        {
            m_nPlanPet_1[i] = 0;
        }
        else if(value == m_nPlanPet_2[i])
        {
            m_nPlanPet_2[i] = 0;
        }
    }
}

bool CDataPool::isHeroBeenPlan(int nId)
{
    for (int i = 0; i < PLAN_HERO_NUM; i++)
    {
        if (m_nPlanHero[i] == nId)
        {
            return true;
        }
    }
    
    return false;
}
bool CDataPool::isOutfitBeenPlan(int nId)
{
    for (int i = 0; i < PLAN_OUTFIT_NUM; i++)
    {
        if (m_nPlanOutfit[i] == nId)
        {
            return true;
        }
    }
    
    return false;
}
bool CDataPool::isPetBeenPlan(int nId)
{
    for (int i = 0; i < PLAN_PET_NUM; i++)
    {
        if (m_nPlanPet_1[i] == nId || m_nPlanPet_2[i] == nId)
        {
            return true;
        }
    }
    
    return false;
}
    
#pragma mark - 背包记录
int CDataPool::findBagOutfitNum(int outfitId)
{
    if (!isValidOutfitId(outfitId))
    {
        return 0;
    }
    OutfitRecord *pData = getOutfitRecord(outfitId);
    if (pData)
    {
        return pData->m_nNum;
    }
    return 0;
}
void CDataPool::changeBagOutfitNum(int outfitId, int nAdd)
{
    if (!isValidOutfitId(outfitId))
    {
        return;
    }
    OutfitRecord *pData = getOutfitRecord(outfitId);
    if (pData)
    {
        int num = pData->m_nNum;
        OutfitRecord rcd;
        memcpy(&rcd, pData, sizeof(OutfitRecord));
        rcd.m_nNum += nAdd;
        if (rcd.m_nNum < 0)
        {
            rcd.m_nNum = 0;
        }
        setOutfitRecord(outfitId, rcd);
    }
}


bool CDataPool::changeBagGoldNum(int nValue)
{
    return changeBagGoldNum(nValue, true);
}


bool CDataPool::changeBagGoldNumForGame(int nValue)
{    
    return changeBagGoldNum(nValue, false);
}


bool CDataPool::changeBagGoldNum(int nValue, bool bAlert)
{
    int bak = gold;
    
    gold += nValue;

    if (gold < 0)
    {
        gold = bak;
        return false;
    }
    else if (gold > GOLD_LIMIT)
    {
        gold = GOLD_LIMIT;
        
        if (bAlert) {
            xDialog->show(xStr("gold_limit"));
        }
        return false;
    }    
    else
    {
        return true;
    }
}


bool CDataPool::changeBagGemNum(int nValue)
{
    return changeBagGemNum(nValue, true);
}


bool CDataPool::changeBagGemNumForGame(int nValue)
{    
    return changeBagGemNum(nValue, false);
}


bool CDataPool::changeBagGemNum(int nValue, bool bAlert)
{
    int backup = xUser->pUserInfo->gemstone;
    
    xUser->pUserInfo->gemstone += nValue;
    
    if (xUser->pUserInfo->gemstone < 0)
    {
        xUser->pUserInfo->gemstone = backup;
        return false;
    }
    else if (xUser->pUserInfo->gemstone > GOLD_LIMIT)
    {
        xUser->pUserInfo->gemstone = GOLD_LIMIT;
        
        if (bAlert) {
            xDialog->show(xStr("gem_limit"));  
        }                      
        
        return false;
    }
    else
    {
        return true;
    }
}


void CDataPool::changePower(int nValue)
{
    xUser->pUserInfo->strength += nValue;
    
    if (xUser->pUserInfo->strength < 0)
    {
        xUser->pUserInfo->strength = 0;
    }
}


void CDataPool::initEnemyRecord()
{
    m_pEnemyRecord = new EnemyRecord*[m_nEnemyNum];
    for (int i = 0; i < m_nEnemyNum; i++)
    {
        EnemyRecord *p = new EnemyRecord;
        if(p)
        {
            p->m_bUnlock = false;
            p->m_nLevel = 0;
            p->m_nHpPercent = 100;
        }
        
        m_pEnemyRecord[i] = p;
    }
//    m_pEnemyRecord[0]->m_bUnlock = true;
//    m_pEnemyRecord[0]->m_nLevel = 0;
}
void CDataPool::freeEnemyRecord()
{
    for (int i = 0; i < m_nEnemyNum; i++)
    {
        EnemyRecord *p = m_pEnemyRecord[i];
        CC_SAFE_DELETE(p);
    }
    CC_SAFE_DELETE_ARRAY(m_pEnemyRecord);
}
EnemyRecord *CDataPool::getEnemyRecord(int nID)
{
    if (!m_pEnemyData)
    {
        return NULL;
    }
    EnemyRecord *p = NULL;
    int index = nID - First_Enemy_ID;
    if(index>=0 && index < m_nEnemyNum)
    {
        p = m_pEnemyRecord[index];
    }
    return p;
}
void CDataPool::setEnemyRecord(int nID, EnemyRecord & rcd)
{
    if (!m_pEnemyRecord)
    {
        return;
    }
    EnemyRecord *p = NULL;
    int index = nID - First_Enemy_ID;
    if(index>=0 && index < m_nEnemyNum)
    {
        p = m_pEnemyRecord[index];
        p->m_bUnlock = rcd.m_bUnlock;
        p->m_nLevel = rcd.m_nLevel;
        p->m_nHpPercent = rcd.m_nHpPercent;
    }
}
void CDataPool::initHeroRecord()
{
    m_pHeroRecord = new HeroRecord*[m_nHeroNum];
    for (int i = 0; i < m_nHeroNum; i++)
    {
        HeroRecord *p = new HeroRecord;
        if(p)
        {
            p->m_bUnlock = false;
            p->m_nLevel = 0;
        }
        m_pHeroRecord[i] = p;
    }
    m_pHeroRecord[0]->m_bUnlock = true;
    m_pHeroRecord[0]->m_nLevel = 0;
}
void CDataPool::freeHeroRecord()
{
    for (int i = 0; i < m_nHeroNum; i++)
    {
        HeroRecord *p = m_pHeroRecord[i];
        CC_SAFE_DELETE(p);
    }
    CC_SAFE_DELETE_ARRAY(m_pHeroRecord);
}
HeroRecord *CDataPool::getHeroRecord(int nID)
{
    if (!m_pHeroRecord)
    {
        return NULL;
    }
    HeroRecord *p = NULL;
    int index = nID - First_Hero_ID;
    if(index>=0 && index < m_nHeroNum)
    {
        p = m_pHeroRecord[index];
    }
    return p;
}
void CDataPool::setHeroRecord(int nID, HeroRecord & rcd)
{
    if (!m_pHeroRecord)
    {
        return;
    }
    HeroRecord *p = NULL;
    int index = nID - First_Hero_ID;
    if(index>=0 && index < m_nHeroNum)
    {
        p = m_pHeroRecord[index];
        p->m_bUnlock = rcd.m_bUnlock;
        p->m_nLevel = rcd.m_nLevel;
    }
}
void CDataPool::initOutfitRecord()
{
    m_pOutfitRecord = new OutfitRecord*[m_nOutfitNum];
    for (int i = 0; i < m_nHeroNum; i++)
    {
        OutfitRecord *p = new OutfitRecord;
        if(p)
        {
            p->m_nNum = 0;
        }
        m_pOutfitRecord[i] = p;
    }
    m_pOutfitRecord[0]->m_nNum = 5;
}
void CDataPool::freeOutfitRecord()
{
    for (int i = 0; i < m_nOutfitNum; i++)
    {
        OutfitRecord *p = m_pOutfitRecord[i];
        CC_SAFE_DELETE(p);
    }
    CC_SAFE_DELETE_ARRAY(m_pOutfitRecord);
}
OutfitRecord *CDataPool::getOutfitRecord(int nID)
{
    if (NULL == m_pOutfitRecord)
    {
        return NULL;
    }
    OutfitRecord *p = NULL;
    int index = nID - First_Outfit_ID;
    if(index>=0 && index < m_nOutfitNum)
    {
        p = m_pOutfitRecord[index];
    }
    return p;
}
void CDataPool::setOutfitRecord(int nID, OutfitRecord & rcd)
{
    if (!m_pOutfitRecord)
    {
        return;
    }
    OutfitRecord *p = NULL;
    int index = nID - First_Outfit_ID;
    if(index>=0 && index < m_nOutfitNum)
    {
        p = m_pOutfitRecord[index];
        p->m_nNum = rcd.m_nNum;
    }
}
void CDataPool::initLevelRecord()
{
//    for (int i = 0; i < MAX_CHAPTER_NUM*LEVEL_NUM_OF_CHAPTER; i++)
//    {
//        LevelRecord *pData = new LevelRecord;
//        if (pData)
//        {
//            ///     pData->m_bUnlock = false;
//            pData->m_nScore = 0;
//
//            m_levelRecordList.insert(pair<int, LevelRecord*>(i, pData));
//        }
//    }
    
    m_pLevelRecord = new LevelRecord*[MAX_CHAPTER_NUM*LEVEL_NUM_OF_CHAPTER];
    for (int i = 0; i < MAX_CHAPTER_NUM*LEVEL_NUM_OF_CHAPTER; i++)
    {
        LevelRecord *p = new LevelRecord;
        if(p)
        {
            p->m_nScore = 0;
        }
        m_pLevelRecord[i] = p;
    }
    
}
void CDataPool::freeLevelRecord()
{
//    LevelRecordContainer::const_iterator it = m_levelRecordList.begin();
//    while (m_levelRecordList.end() != it)
//    {
//        LevelRecord *pData = it->second;
//        CC_SAFE_DELETE(pData);
//    }
    
    for (int i = 0; i < MAX_CHAPTER_NUM*LEVEL_NUM_OF_CHAPTER; i++)
    {
        LevelRecord *p = m_pLevelRecord[i];
        CC_SAFE_DELETE(p);
    }
    CC_SAFE_DELETE_ARRAY(m_pLevelRecord);
}
//nChapter, nLevel 都是0起计数
LevelRecord *CDataPool::getLevelRecord(int nChapter, int nLevel)
{
//    int key = nChapter*LEVEL_NUM_OF_CHAPTER + nLevel;
//    LevelRecordContainer::const_iterator it = m_levelRecordList.find(key);
//    
//    if (m_levelRecordList.end() != it)
//    {
//        return it->second;
//    }
//    
//    return NULL;
    
    if (NULL == m_pLevelRecord)
    {
        return NULL;
    }
    LevelRecord *p = NULL;
    int index = nChapter*LEVEL_NUM_OF_CHAPTER + nLevel;
    if(index>=0 && index < MAX_CHAPTER_NUM*LEVEL_NUM_OF_CHAPTER)
    {
        p = m_pLevelRecord[index];
    }
    return p;
}
//nChapter, nLevel 都是0起计数
void CDataPool::setLevelRecord(int nChapter, int nLevel, LevelRecord &rcd)
{
//    LevelRecord *pRcd = getLevelRecord(nChapter, nLevel);
//    if(pRcd)
//    {
//        ///    pRcd->m_bUnlock = rcd.m_bUnlock;
//        pRcd->m_nScore = rcd.m_nScore;
//    }
    
    LevelRecord *pRcd = getLevelRecord(nChapter, nLevel);
    if(pRcd)
    {
    ///    pRcd->m_bUnlock = rcd.m_bUnlock;
        pRcd->m_nScore = rcd.m_nScore;
    }
}
//刷新关卡得分
//nChapter, nLevel 都是0起计数
void CDataPool::freshLevelScore(int nChapter, int nLevel, int nScore)
{
    LevelRecord *pRcd = getLevelRecord(nChapter, nLevel);
    if(pRcd && pRcd->m_nScore < nScore)
    {
        pRcd->m_nScore = nScore;
    }
}
#pragma mark - 资源加载
void CDataPool::addEffectCache(const char *name)
{
    cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(name);
}

void CDataPool::removeEffectCache(const char *name)
{
    return;
    cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(name);
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}

#pragma mark - 辅助函数

//true 拥有铁弹弓道具
bool CDataPool::isHaveTieDanGong()
{
    OutfitRecord *pRcd = getOutfitRecord(Prop_Type_TieDanGong);
    if (pRcd)
    {
        return (pRcd->m_nNum > 0);
    }
    
    return false;
}
//获取栅栏类的道具id
//返回值：0表示无栅栏类道具 大于0表示最强栅栏的id
int CDataPool::getZhanLanFromBag()
{
    do
    {
        OutfitRecord *pTieZhaLan = getOutfitRecord(Prop_Type_TieZhaLan);
        CC_BREAK_IF(!pTieZhaLan);
        if (pTieZhaLan->m_nNum > 0)
        {
            return Prop_Type_TieZhaLan;
        }
        
        OutfitRecord *pShiZhaLan = getOutfitRecord(Prop_Type_ShiZhaLan);
        CC_BREAK_IF(!pShiZhaLan);
        if (pShiZhaLan->m_nNum > 0)
        {
            return Prop_Type_ShiZhaLan;
        }
        
    } while (0);
    
    return 0;
}

int CDataPool::getEnemyAwardID()
{
    int ID = 0;
    ID = CFG_DATA_MGR->getEnemyAwardID();
    return ID;
}
//nChapter level 是从1开始
//游戏胜利 传入副本和关卡数 判断是否解锁下一副本或关卡
void CDataPool::playedLevel(int nChapter, int level)
{
    bool bChange = false;
    
    do
    {
        int _chapter = nChapter-1;
        int _level = level-1;
        
        //解锁副本
        if (_level >= (LEVEL_NUM_OF_CHAPTER - 1))
        {
            if (_chapter == m_nMaxUnlockChapter
                && m_nMaxUnlockChapter < (MAX_CHAPTER_NUM - 1) )
            {
                ++m_nMaxUnlockChapter;
                currentChapter = m_nMaxUnlockChapter;
                
                bChange = true;
            }
            
            break;
        }
        
        //解锁关卡
        if(_level == m_nMaxUnlockLevel[_chapter]
           && m_nMaxUnlockLevel[_chapter] < (LEVEL_NUM_OF_CHAPTER - 1))
        {
            ++m_nMaxUnlockLevel[_chapter];
            m_nCurrentLevel[_chapter] = m_nMaxUnlockLevel[_chapter];
            
            bChange = true;
            
            break;
        }
        
    } while (0);
    
    if (bChange)
    {
        saveArchive();
    }
}

bool CDataPool::handleMsg(const Message &msg)
{
    CCLOG("NetTest::HandleMsg msgType= %d", msg.m_nMsgType);



    switch (msg.m_nMsgType)
    {



        case CMD_REQ_SYSTEM_MESSAGE:
        {
            //CCUserDefault::sharedUserDefault()->setStringForKey()
            
            //m_pPanelMatch->setEnabled(true);
            break;
        }
        case CMD_REQ_FIGHT_REPORT:    //2.4.6.16.1	获取排行榜玩家返回的信息(CMD_RES_RANKING) 	47
        {
            //createChallenge();
            break;
        }
        case CMD_RES_FIGHT:
        {
            
        }
            break;
        case CMD_RES_EXPEND:
        {
//            CCLOG("gemstone:%d",DATAPOOL->gem = UserData::Instance()->pUserInfo->gemstone);
//            DATAPOOL->gem = UserData::Instance()->pUserInfo->gemstone;
//            CCLOG("gem:%d",DATAPOOL->gem);
        }
            break;
        default:
            break;
    }
    
    return true;
}



void CDataPool::readFileForVector(vector <GMessage> &vec, const char * name)
{
    std::string path = CCFileUtils::sharedFileUtils()->getWritablePath() + name;
    
    if(CCFileUtils::sharedFileUtils()->isFileExist(path.c_str()))
    {
        FILE* f = fopen(path.c_str(), "rb");
        
        if(f == NULL)
        {
            //do stuff to create the empty data
            CCAssert(false, "********** fopen is fail.");
        }
        else
        {
            int count;
            fread(&count, sizeof(int), 1, f);
            for(int i = 0; i < count; i++)
            {
                GMessage p;
                
                fread(&p.iType, sizeof(int), 1, f);
                fread(&p.iData, sizeof(int), 1, f);
                fread(&p.iLen, sizeof(int), 1, f);
                
                
                p.msg = new char[p.iLen + 1];
                p.msg[p.iLen] = NULL;
                fread(p.msg, sizeof(char), p.iLen, f);
                
                vec.push_back(p);  //vector of my_struct
            }
            
            fclose(f);
        }
    }
}


void CDataPool::writeFileForVector(vector <GMessage> &vec, const char * name)
{
    std::string path = CCFileUtils::sharedFileUtils()->getWritablePath() + name;
    FILE* f = fopen(path.c_str(), "wb");
    
    if(f == NULL)
    {
        //do stuff to create the empty data
        CCAssert(false, "********** fopen is fail.");
    }
    else
    {
        int size = vec.size(); //number of items to save
        fwrite(&size, sizeof(int), 1, f);
        for(unsigned int i = 0; i<vec.size(); i++)
        {
            fwrite(&vec.at(i).iType, sizeof(int), 1, f);
            fwrite(&vec.at(i).iData, sizeof(int), 1, f);
            fwrite(&vec.at(i).iLen, sizeof(int), 1, f);
            fwrite(vec.at(i).msg, sizeof(char), vec.at(i).iLen, f);
        }
        
        fclose(f);
    }
}


void CDataPool::readFileForSign(vector <GSign> &vec)
{
    std::string path = CCFileUtils::sharedFileUtils()->getWritablePath() + SIGN_FILE;
    
    if(CCFileUtils::sharedFileUtils()->isFileExist(path.c_str()))
    {
        FILE* f = fopen(path.c_str(), "rb");
        
        if(f == NULL)
        {
            //do stuff to create the empty data
            CCAssert(false, "********** fopen is fail.");
        }
        else
        {
            int count;
            fread(&count, sizeof(int), 1, f);
            for(int i = 0; i < count; i++)
            {
                GSign p;
                
                fread(&p.bSign, sizeof(bool), 1, f);
                fread(&p.iDay, sizeof(int), 1, f);
                
                vec.push_back(p);  //vector of my_struct
            }
            
            fclose(f);
        }
    }
}


void CDataPool::writeFileForSign(vector <GSign> &vec)
{
    std::string path = CCFileUtils::sharedFileUtils()->getWritablePath() + SIGN_FILE;
    FILE* f = fopen(path.c_str(), "wb");
    
    if(f == NULL)
    {
        //do stuff to create the empty data
        CCAssert(false, "********** fopen is fail.");
    }
    else
    {
        int size = vec.size(); //number of items to save
        fwrite(&size, sizeof(int), 1, f);
        for(unsigned int i = 0; i<vec.size(); i++)
        {
            fwrite(&vec.at(i).bSign, sizeof(bool), 1, f);
            fwrite(&vec.at(i).iDay, sizeof(int), 1, f);
        }
        
        fclose(f);
    }
}


void CDataPool::saveAndFreeMessage()
{
    writeFileForVector(m_vSystem, MES_SYSTEM_NAME);
    
    for (int i = 0; i < m_vSystem.size(); i++) {
        CC_SAFE_DELETE_ARRAY(m_vSystem.at(i).msg);
    }
}


void CDataPool::checkMessage(float dt)
{
    UserData *data = UserData::Instance();
    
    bool bNew = false;
    
    if (data->content_action.length() != 0) {
        
        GMessage tGMsg;
        tGMsg.bNew = true;
        tGMsg.iType = 0;
        tGMsg.iData = data->configId;
        tGMsg.iLen = data->content_action.length();
        tGMsg.msg = new char[tGMsg.iLen + 1];
        strcpy(tGMsg.msg, data->content_action.c_str());
        
        if (m_vSystem.size() > MESSAGE_LIMIT) {
            vector<GMessage>::iterator itr = m_vSystem.begin();
            while (m_vSystem.size() > MESSAGE_LIMIT)
            {
                m_vSystem.erase(itr);   //删除旧消息
            }
        }
        
        m_vSystem.push_back(tGMsg);
        data->content_action.clear();
        
        bNew = true;
    }
    
    
    if (data->content_fighting.length() != 0) {
        GMessage tGMsg;
        tGMsg.bNew = true;
        tGMsg.iType = 1;
        tGMsg.iData = data->attackUserId;
        tGMsg.iLen = data->content_fighting.length();
        tGMsg.msg = new char[tGMsg.iLen + 1];
        strcpy(tGMsg.msg, data->content_fighting.c_str());
        
        if (m_vSystem.size() > MESSAGE_LIMIT) {
            vector<GMessage>::iterator itr = m_vSystem.begin();
            while (m_vSystem.size() > MESSAGE_LIMIT)
            {
                m_vSystem.erase(itr);   //删除旧消息
            }
        }
        
        m_vSystem.push_back(tGMsg);
        data->content_fighting.clear();
        
        bNew = true;
    }
    
    
    if (data->bossFightRewardStr.length() != 0) {
        GMessage tGMsg;
        tGMsg.bNew = true;
        tGMsg.iType = 0;
        tGMsg.iLen = data->bossFightRewardStr.length();
        tGMsg.msg = new char[tGMsg.iLen + 1];
        strcpy(tGMsg.msg, data->bossFightRewardStr.c_str());
        
        if (m_vSystem.size() > MESSAGE_LIMIT) {
            vector<GMessage>::iterator itr = m_vSystem.begin();
            while (m_vSystem.size() > MESSAGE_LIMIT)
            {
                m_vSystem.erase(itr);   //删除旧消息
            }
        }
        
        m_vSystem.push_back(tGMsg);
        data->bossFightRewardStr.clear();
        
        bNew = true;
    }
    
    
    if (data->bossFightStr.length() != 0) {
        GMessage tGMsg;
        tGMsg.bNew = true;
        tGMsg.iType = 0;
        tGMsg.iLen = data->bossFightStr.length();
        tGMsg.msg = new char[tGMsg.iLen + 1];
        strcpy(tGMsg.msg, data->bossFightStr.c_str());
        
        if (m_vSystem.size() > MESSAGE_LIMIT) {
            vector<GMessage>::iterator itr = m_vSystem.begin();
            while (m_vSystem.size() > MESSAGE_LIMIT)
            {
                m_vSystem.erase(itr);   //删除旧消息
            }
        }
        
        m_vSystem.push_back(tGMsg);
        data->bossFightStr.clear();
        
        bNew = true;
    }
    
    
    if (m_iCountTime % 30 == 29) {
//    if (true) {
        m_iCountTime = 0;
        writeFileForVector(m_vSystem, MES_SYSTEM_NAME);
        //writeFileForVector(m_vFight, MES_FIGHT_NAME);
    }
    else{
        m_iCountTime++;
    }
    
    if(bNew)
    {
        CCNotificationCenter::sharedNotificationCenter()->postNotification(NC_NewMessage);
    }
}


int CDataPool::getNewMessageCount()
{
    int iCount = 0;
    
    for (int i = 0; i < m_vSystem.size(); i++) {
        if (m_vSystem.at(i).bNew == true) {
            CCLOG("********** %d", iCount);
            iCount++;
        }
    }
    
    return iCount;
}


void CDataPool::makeAllMessageToOld()
{
    for (int i = 0; i < m_vSystem.size(); i++) {
        m_vSystem.at(i).bNew = false;
    }
    
    CCLOG("********** m_vSystem.size() %d", m_vSystem.size());
}

void CDataPool::playGameBgSound(const char *name,bool bloop)
{
    if(m_bOpenBackMusic)
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(name,bloop);
    }
}

int CDataPool::playGameEffect(const char *name)
{
    if(m_bOpenSoundEffect)
        return  CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(name);
    else
        return 0;
}


void CDataPool::changeShareCounts(int iChange)
{
    CCAssert(false, "暂时不用了");
    
    m_iShareCounts += iChange;
    
    //分享一次保存一次
    saveArchive();
    //CCUserDefault::sharedUserDefault()->setIntegerForKey(ShareCount, m_iShareCounts);
}


void CDataPool::setAccpetSharePrize(int iLevel)
{
    char key[100];
    
    sprintf(key, "share_prize_%d", iLevel);
    CCUserDefault::sharedUserDefault()->setIntegerForKey(key, true);

}


void CDataPool::getAccpetSharePrize(bool bAcceptPrize[])
{
    char key[100];
    
    for (int i = 0; i < SHARE_NUM; ++i)
    {
        sprintf(key, "share_prize_%d",i);
        bAcceptPrize[i] = CCUserDefault::sharedUserDefault()->getIntegerForKey(key);
    }
}


string CDataPool::getDefaultShareTxt()
{
    string sShareTxt = xUGString("share_text");
    
    if (sShareTxt.empty()) {
        char buf[15];            
        
        if (xUser->totalScore != 0) {
            sShareTxt = string(xStr("share_default0")) + ccitoa(xUser->totalScore, buf) + xStr("share_default1");
        }
        else
        {
            sShareTxt = string(xStr("share_default_no_score"));
        }
    }
    
    return sShareTxt;
}

//是否是雇佣兵
bool CDataPool::isPet(int id)
{
    return (id == Enemy_Type_aBoss
            ||id == Enemy_Type_bBoss
            ||id == Enemy_Type_cBoss
            ||id == Enemy_Type_dBoss
            ||id == Enemy_Type_eBoss
            ||id == Enemy_Type_fBoss
            ||id == Enemy_Type_gBoss);
}
//true 使用金币升级 false 使用宝石升级
//如果gold大于0而且是首次升级,使用金币升级
bool CDataPool::isUseGoldToTrain(int trainGold, int level)
{
    return((trainGold > 0)
        && (0 == level));
}

bool CDataPool::checkSign()
{
#ifdef SIGN_DEBUG
    return false;
#endif    
    
    bool bRet = true;
    
    int m_iDay;
    int m_iWeekDay;
    
    HelpMethod::getWeekDay(&m_iWeekDay, &m_iDay);
    
    readFileForSign(m_vSign);
    
    if (!m_vSign.empty()){
        //检查今天
        if (m_vSign.at(m_iWeekDay).bSign == false || m_vSign.at(m_iWeekDay).iDay != m_iDay) {
            bRet = false;
        }
    }
    else
    {
        bRet = false;
    }
    
    return  bRet;
}


void CDataPool::playKeytone()
{
    if(m_bOpenSoundEffect)
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(title_scene_menu_start);
    }
}
void CDataPool::tryPlayBgMuisc(EBackMusicType type, bool bForceChange)
{
    if ( !m_bOpenBackMusic )
    {
        return;
    }
    
    if (!bForceChange
        && CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
    {
        return;
    }
    
    switch (type)
    {
        case BACKMUSIC_TYPE_TITLE:
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(title_scene_back_music,true);
            break;
            
        case BACKMUSIC_TYPE_HOME:
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(home_scene_back_music,true);
            break;
    }

}

void CDataPool::loadFightSceneResource()
{
    CCTextureCache::sharedTextureCache()->removeAllTextures();
    CCDirector::sharedDirector()->purgeCachedData();
    CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("GameUI/Game_Play_UI.plist", "GameUI/Game_Play_UI.pvr.ccz");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("GameUI/Game_Result_UI.plist","GameUI/Game_Result_UI.pvr.ccz");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("GameUI/Game_Sprite.plist","GameUI/Game_Sprite.pvr.ccz");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(tips_TxtPlist, tips_TxtPic);
    CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
}

void CDataPool::removeFightSceneResource()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("GameUI/Game_Play_UI.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("GameUI/Game_Result_UI.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("GameUI/Game_Sprite.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(tips_TxtPlist);
    CCTextureCache::sharedTextureCache()->removeAllTextures();
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
}

void CDataPool::loadResource()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrames();
    CCTextureCache::sharedTextureCache()->removeAllTextures();
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    CCDirector::sharedDirector()->purgeCachedData();
    CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
    //公共资源 by luoxp 
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("PvPScene/+Dialog.plist", "PvPScene/+Dialog.pvr.ccz");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("+Public/+UI1.plist", "+Public/+UI1.pvr.ccz");
    
    //公共资源
    if(!gameTutorialsEnd())
    {
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(teach_shouzhi_plist, teach_shouzhi_ccz);
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(teach_text_plist, teach_text_ccz);
        CCSpriteFrameCache::sharedSpriteFrameCache()->retainSpriteFramesFromFile(teach_shouzhi_plist);
        CCSpriteFrameCache::sharedSpriteFrameCache()->retainSpriteFramesFromFile(teach_text_plist);
    }
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(ui_plist, ui_image);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(bg0_plist, bg0_image);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(bg1_plist, bg1_image);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(juese_plist, juese_image);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("GameUI/Props.plist","GameUI/Props.pvr.ccz");
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->retainSpriteFramesFromFile("GameUI/Props.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->retainSpriteFramesFromFile("+Public/+UI1.plist");    
    CCSpriteFrameCache::sharedSpriteFrameCache()->retainSpriteFramesFromFile("PvPScene/+Dialog.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->retainSpriteFramesFromFile(ui_plist);
    CCSpriteFrameCache::sharedSpriteFrameCache()->retainSpriteFramesFromFile(bg0_plist);
    CCSpriteFrameCache::sharedSpriteFrameCache()->retainSpriteFramesFromFile(bg1_plist);
    CCSpriteFrameCache::sharedSpriteFrameCache()->retainSpriteFramesFromFile(juese_plist);
    CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
}

void CDataPool::removeResource()
{
    //公共资源 by luoxp
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("PvPScene/+Dialog.plist");
    
    //公共资源
    if(!gameTutorialsEnd())
    {
        CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(teach_shouzhi_plist);
        CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(teach_text_plist);
    }
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(ui_plist);
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(bg0_plist);
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(bg1_plist);
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(juese_plist);
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("GameUI/Props.plist");
}

bool CDataPool::fightTutorialsEnd()
{
    if(m_bTeachStepOver[TEACH_STEP_USE_DANGONG1]          //弹弓普通用法
       &&m_bTeachStepOver[TEACH_STEP_USE_DANGONG2]        //弹弓蓄力用法
       &&m_bTeachStepOver[TEACH_STEP_USE_HONGZHAJI]       //道具轰炸机
       &&m_bTeachStepOver[TEACH_STEP_SELECT_HERO_G]       //选择英雄 游戏界面
       &&m_bTeachStepOver[TEACH_STEP_YAOJIANG]            //摇奖
       &&m_bTeachStepOver[TEACH_STEP_QIANGBAO]            //抢宝怪第一次出现 提示
       &&m_bTeachStepOver[TEACH_STEP_QIANGBAO_SLIDE]      //第一次进入抢宝界面
       &&m_bTeachStepOver[TEACH_STEP_USE_RAND_OUTFIT]     //随机道具用法
       )
        return true;
    
    return false;
}

bool CDataPool::fubenTutorialsEnd()
{
    return(m_bTeachStepOver[TEACH_STEP_ENTER_FUBEN1]
       &&m_bTeachStepOver[TEACH_STEP_ENTER_FUBEN2]
       &&m_bTeachStepOver[TEACH_STEP_ENTER_FUBEN3]
       &&m_bTeachStepOver[TEACH_STEP_ENTER_FUBEN4]
       &&m_bTeachStepOver[TEACH_STEP_ENTER_FUBEN5]
       );
}

bool CDataPool::gameTutorialsEnd()
{
    if(m_bTeachStepOver[TEACH_STEP_ENTER_MAOXIAN]   //进入冒险
       &&m_bTeachStepOver[TEACH_STEP_ENTER_FUBEN1]         //进入副本1
       &&m_bTeachStepOver[TEACH_STEP_ENTER_LEVEL1]        //进入关卡1
       &&m_bTeachStepOver[TEACH_STEP_ENTER_LEVEL2]        //进入关卡2
       &&m_bTeachStepOver[TEACH_STEP_ENTER_FIGHT1]          //进入第1关战斗
       &&m_bTeachStepOver[TEACH_STEP_ENTER_FIGHT2]          //进入第2关战斗
       &&m_bTeachStepOver[TEACH_STEP_ENTER_SELECT_HERO]   //进入英雄选择
       &&m_bTeachStepOver[TEACH_STEP_ENTER_SELECT_OUTFIT] //进入道具选择
       &&m_bTeachStepOver[TEACH_STEP_ENTER_SELECT_PET]
       &&m_bTeachStepOver[TEACH_STEP_SELECT_HERO]         //选择英雄 部署界面
       &&m_bTeachStepOver[TEACH_STEP_SELECT_OUTFIT]       //选择道具
       &&m_bTeachStepOver[TEACH_STEP_SELECT_PET]
       &&m_bTeachStepOver[TEACH_STEP_SHOP_SELECT_OUTFIT]          //商店-选择道具
       &&m_bTeachStepOver[TEACH_STEP_SHOP_BUY_OUTFIT]   //商店-购买道具
       &&m_bTeachStepOver[TEACH_STEP_USE_DANGONG1]          //弹弓普通用法
       &&m_bTeachStepOver[TEACH_STEP_USE_DANGONG2]        //弹弓蓄力用法
       &&m_bTeachStepOver[TEACH_STEP_USE_HONGZHAJI]       //道具轰炸机
       &&m_bTeachStepOver[TEACH_STEP_SELECT_HERO_G]       //选择英雄 游戏界面
       &&m_bTeachStepOver[TEACH_STEP_YAOJIANG]            //摇奖
       &&m_bTeachStepOver[TEACH_STEP_QIANGBAO]            //抢宝怪第一次出现 提示
       &&m_bTeachStepOver[TEACH_STEP_QIANGBAO_SLIDE]      //第一次进入抢宝界面
       &&m_bTeachStepOver[TEACH_STEP_USE_RAND_OUTFIT]     //随机道具用法
       &&m_bTeachStepOver[TEACH_STEP_INK1]
       &&m_bTeachStepOver[TEACH_STEP_INK2]
       &&m_bTeachStepOver[TEACH_STEP_INK3]
       &&m_bTeachStepOver[TEACH_STEP_SHIBEI1]
       &&m_bTeachStepOver[TEACH_STEP_SHIBEI2]
       &&m_bTeachStepOver[TEACH_STEP_SHIBEI3]
       )
        return  true;
    
    
    return false;
}

int CDataPool::getHeroReference()
{
    m_nHeroReference++;
    return m_nHeroReference;
}

void CDataPool::resetHeroReference()
{
    m_nHeroReference = 0;
}

bool CDataPool::isUnlockDefense2()
{
    return (this->getPlanPet(0, 2) >= 0)
    || (this->getPlanPet(1, 2) >= 0)
    || (this->getPlanPet(2, 2) >= 0)
    || (this->getPlanPet(3, 2) >= 0);
}
void CDataPool::CBLackOfGem(int id, int iTag)
{
    if (0 == iTag)
    {
        xSM->forward(ST_BuyGem);
    }
}
void CDataPool::CBLackOfGold(int id, int iTag)
{
    if (0 == iTag)
    {
        xSM->forward(ST_BuyGold);
    }
}

void CDataPool::waitSyncPetData()
{
    //5. 佣兵数长度 (没有本地数据)
    int nEnemyNum = DATAPOOL->m_nEnemyNum;
    int nPetNum = 0;
    for (int i = 0; i < nEnemyNum; i++)
    {
        int nEnemyId = First_Enemy_ID + i;
        if (DATAPOOL->isPet(nEnemyId))
        {
            EnemyRecord *pRcd = xData->getEnemyRecord(nEnemyId);
            if (pRcd && pRcd->m_bUnlock)
            {
                ++(nPetNum);
            }
        }
    }
    //6. 玩家佣兵数量列表
    if (nPetNum <= 0)
    {
        return;
    }
    DEL_ARRAY(xUser->pUserInfo->listMercemary, xUserInfo->mercemaryNum);
    xUserInfo->mercemaryNum = nPetNum;
    xUserInfo->listMercemary = new Mercemary*[nPetNum];
    
//        for (int i = 0, j = 0; i < nEnemyNum && j < nPetNum; i++)
//        {
//            nEnemyId = First_Enemy_ID + i;
//            if (DATAPOOL->isPet(nEnemyId))
//            {
//                EnemyRecord *pRcd = xData->getEnemyRecord(nEnemyId);
//                if (pRcd && pRcd->m_bUnlock)
//                {
//                    Mercemary *p = new Mercemary();
//                    p->mercemaryId = nEnemyId;
//                    p->grade = pRcd->m_nLevel;
//                    xUserInfo->listMercemary[j++] = p;
//                }
//            }
//        }
    
    int nListCount = 0;
    for (int nLine = 1; nLine <= 2; nLine++)
    {
        for (int nIndex = 0; nIndex < PLAN_PET_NUM; nIndex++)
        {
            int nEnemyId = xData->getPlanPet(nIndex, nLine);
            if (DATAPOOL->isPet(nEnemyId))
            {
                EnemyRecord *pRcd = xData->getEnemyRecord(nEnemyId);
                if (pRcd && pRcd->m_bUnlock)
                {
                    Mercemary *p = new Mercemary();
                    p->mercemaryId = nEnemyId;
                    p->grade = pRcd->m_nLevel;
                    xUserInfo->listMercemary[nListCount++] = p;
                }
            }
        }
    }

    if (nListCount < PLAN_PET_NUM * 2)
    {
        for (int i = 0, j = nListCount; i < nEnemyNum && j < nPetNum; i++)
        {
            int nEnemyId = First_Enemy_ID + i;
            if ( xData->isPet(nEnemyId) && !DATAPOOL->isPetBeenPlan(nEnemyId) )
            {
                EnemyRecord *pRcd = xData->getEnemyRecord(nEnemyId);
                if (pRcd && pRcd->m_bUnlock)
                {
                    Mercemary *p = new Mercemary();
                    p->mercemaryId = nEnemyId;
                    p->grade = pRcd->m_nLevel;
                    xUserInfo->listMercemary[j++] = p;
                }
            }
        }
    }

}
