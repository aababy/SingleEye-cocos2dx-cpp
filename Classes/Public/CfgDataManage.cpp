//
//  CfgDataManage.cpp
//  SingleEye
//
//  Created by qinfen on 13-11-6.
//
//

#include "CfgDataManage.h"
#include "GameResouce.h"
#include "GameInclude.h"

static CfgDataManage *instance = NULL;
CfgDataManage* CfgDataManage::getInstance()
{
    if (!instance) {
        instance = new CfgDataManage();
    }
    return instance;
}

CfgDataManage::CfgDataManage():
m_pSkillDataList(NULL),
m_pBuffDataList(NULL),
m_pName1List(NULL),
m_pName2List(NULL),
m_pTxtlua(NULL),
m_pScript(NULL)
{

}

CfgDataManage::~CfgDataManage()
{
    ClearCfgData();
}

std::string CfgDataManage::getTxtByKey(const char *pKey,const char *pPath)
{
    std::string value;
    LuaManager *pLua = new LuaManager;
    LuaScript* pScript = NULL;

    if(pLua)
    {
        std::string mfilePath = CCFileUtils::sharedFileUtils()->fullPathForFilename(pPath);
        pScript = pLua->CreateScript(mfilePath.c_str());
    }
    if(pKey && pLua)
    {
        value = pLua->GetString((char *)pKey);
    }

    pLua->DestroyScript(pScript);
    CC_SAFE_DELETE(pLua);
    return value;
}

int CfgDataManage::getNumByKey(const char *pKey,const char *pPath)
{
    int value;
    LuaManager *pLua = new LuaManager;
    LuaScript* pScript = NULL;
    if(pLua)
    {
        std::string mfilePath = CCFileUtils::sharedFileUtils()->fullPathForFilename(pPath);
        pScript = pLua->CreateScript(mfilePath.c_str());
    }
    if(pKey && pLua)
    {
        value = pLua->GetInt((char *)pKey);
    }

    pLua->DestroyScript(pScript);
    CC_SAFE_DELETE(pLua);
    return value;
}
Fight_Level_Info* CfgDataManage::getLevelInfo(int nChapter ,int nLevel)
{
    Fight_Level_Info* levelInfo;
    LuaManager *pLua = new LuaManager;
    LuaScript* pScript = NULL;
    levelInfo = new Fight_Level_Info;
    if(pLua)
    {
        std::string mfilePath = CCFileUtils::sharedFileUtils()->fullPathForFilename(Level_Info_Path);
        pScript = pLua->CreateScript(mfilePath.c_str());
        char buf[20];
        memset(buf, 0, 20);
        sprintf(buf, "game_bg_%d",nChapter);
        CCLog("get background name! %d",nChapter);
        sprintf(levelInfo->cBackgroudName,"%s",pLua->GetString(buf).c_str());
        memset(buf,0, 20);
        sprintf(buf, "level%d_time",nChapter);
        CCLog("get Level Time!");
        levelInfo->fTime = (float)pLua->GetFloat(buf);
        memset(buf, 0, 20);
        CCLog("get complete!");
    }
    pLua->DestroyScript(pScript);
    CC_SAFE_DELETE(pLua);
    return levelInfo;
}

Fight_Level_Data* CfgDataManage::getLevelData(int nLevel)
{
    CCLOG("level:%d",nLevel);
    if(nLevel<=0)return NULL;
    Fight_Level_Data* levelData = NULL;
    LuaManager  *pLua = NULL;
    LuaScript   *pScript = NULL;
    pLua = new LuaManager;
    levelData = new Fight_Level_Data;
    if (pLua) {
        std::string mfilePath = CCFileUtils::sharedFileUtils()->fullPathForFilename(Level_Info_Path);
        pScript = pLua->CreateScript(mfilePath.c_str());
        char buf[20];
        memset(buf, 0, 20);
        sprintf(buf, "getWaveCount");
        int waveCount = pLua->GetFuncResult(buf,nLevel,-1);
        levelData->nWaveCount = waveCount;
        levelData->mArrayWave = new Fight_Wave_Data*[waveCount];
        for (int i = 0; i<waveCount; ++i)
        {
            CCLOG("wavecount:%d",i);
            memset(buf, 0, 20);
            sprintf(buf, "getWaveElementCount");
            int count = pLua->GetFuncResult(buf, nLevel,i+1,0,-1);
            Fight_Wave_Data *pWave = new Fight_Wave_Data;
            pWave->mElementCount = count;
            pWave->mArrayElement = new Fight_Wave_Element*[count];
            for (int j = 0; j<count; ++j)
            {
                CCLOG("count:%d",j);
                memset(buf, 0, 20);
                sprintf(buf, "getWaveElement");
                int type = pLua->GetFuncResult(buf, nLevel,i+1,j+1,-1);
                Fight_Wave_Element *pElement = new Fight_Wave_Element;
                pElement->mType = type;
                if (type!=0)
                {
                    memset(buf, 0, 20);
                    sprintf(buf, "getWaveElementPos");
                    int posIndex = pLua->GetFuncResult(buf, nLevel,i+1,j+1,-1);
                    memset(buf, 0, 20);
                    sprintf(buf, "getPosition");
                    int x = pLua->GetFuncResult(buf, posIndex,1,0,-1);
                    int y = pLua->GetFuncResult(buf, posIndex,2,0,-1);
                    pElement->mArrayPoint = ccp(x,y);
                }
                else
                {
                    memset(buf, 0, 20);
                    pElement->mArrayPoint = ccp(0,0);
                }

                pWave->mArrayElement[j] = pElement;
            }
            levelData->mArrayWave[i] = pWave;
        }

    }
    pLua->DestroyScript(pScript);
    CC_SAFE_DELETE(pLua);
    return levelData;
}

Enemy_Data** CfgDataManage::getTotalEnemyData()
{
    CCLOG("CfgDataManage::getTotalEnemyData!");
    Enemy_Data **pEnemyData = NULL;
    std::string tFullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(Enemy_Info_Path);
    unsigned long size = 0;
    char* file =  (char*)cocos2d::CCFileUtils::sharedFileUtils()->getFileData(tFullPath.c_str(), "r",&size);
    cs::CSJsonDictionary *dict = new cs::CSJsonDictionary();
    dict->initWithDescription(file);

    int _totalEnemy = dict->getItemCount();
    pEnemyData = new Enemy_Data*[_totalEnemy];
    CCLOG("_total:%d",_totalEnemy);
    for (int j = 0; j<_totalEnemy; ++j)
    {
        cs::CSJsonDictionary *sub = dict->getSubItemByIndex(j);
        Enemy_Data *pEnemy = new Enemy_Data;
        ParseJSON(sub, pEnemy);
        pEnemyData[j] = pEnemy;
        CC_SAFE_DELETE(sub);
    }

    CC_SAFE_DELETE(dict);
    CC_SAFE_DELETE_ARRAY(file);
    return pEnemyData;
}

void CfgDataManage::ParseJSON(cs::CSJsonDictionary *dict, Enemy_Data *data)
{
    data->mType = dict->getItemIntValue("ID",First_Enemy_ID);
    sprintf(data->mSourceName, "%s",dict->getItemStringValue("SourceName"));
    data->mRange = dict->getItemIntValue("type", 0);
    data->mCD = dict->getItemIntValue("CD", 0);
    data->mHp = dict->getItemIntValue("HP", 0);
    data->mHpAdd = dict->getItemIntValue("HpAdd", 0);
    data->mAtk = dict->getItemIntValue("Atk", 0);
    data->mAtk2 = dict->getItemIntValue("Atk2", 0);
    data->mAtkAdd = dict->getItemIntValue("AtkAdd", 0);
    data->mSkillAtk = dict->getItemIntValue("SkillAtk", 0);
    data->mSkillAtkAdd = dict->getItemIntValue("SkillAtkAdd", 0);
    data->mDuration = dict->getItemIntValue("Duration", 0);
    data->mMagicID = dict->getItemIntValue("MagicID", 0);
    data->mStar = dict->getItemFloatValue("Star", 0);
    data->mVel = dict->getItemFloatValue("Vel", 0);
    data->mDistance = dict->getItemIntValue("AtkDistance", 0);
    data->mAtkFrequency = dict->getItemIntValue("AtkFrequency", 0);
    data->mBuy = dict->getItemIntValue("Buy", 0);
    data->mBuyGold = dict->getItemIntValue("BuyGold", 0);
    data->mBuyGem = dict->getItemIntValue("BuyGem", 0);
    data->mRecruitGold = dict->getItemIntValue("RecruitGold", 0);
    data->mRecruitGem = dict->getItemIntValue("RecruitGem", 0);
    data->mUpGold = dict->getItemIntValue("UpGold", 0);
    data->mUpGem =  dict->getItemIntValue("UpGem", 0);
    data->mGold = dict->getItemIntValue("Gold", 0);
    data->mFrequency = dict->getItemFloatValue("Frequency", 0);
}

Game_Hero_Data** CfgDataManage::getTotalHeroData()
{
    Game_Hero_Data** pHeroData = NULL;
    std::string tFullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(Hero_Info_Path);
    unsigned long size = 0;
    char* file =  (char*)cocos2d::CCFileUtils::sharedFileUtils()->getFileData(tFullPath.c_str(), "r",&size);
    cs::CSJsonDictionary *dict = new cs::CSJsonDictionary();
    dict->initWithDescription(file);

    int _totalHero = dict->getItemCount();
    pHeroData = new Game_Hero_Data*[_totalHero];
    CCLOG("_total:%d",_totalHero);
    for (int j = 0; j<_totalHero; ++j)
    {
        cs::CSJsonDictionary *sub = dict->getSubItemByIndex(j);
        Game_Hero_Data *pHero = new Game_Hero_Data;
        ParseJSON_Hero(sub, pHero);
        pHeroData[j] = pHero;
        CC_SAFE_DELETE(sub);
    }
    CC_SAFE_DELETE(dict);
    CC_SAFE_DELETE_ARRAY(file);
    return pHeroData;
}

void CfgDataManage::ParseJSON_Hero(cs::CSJsonDictionary *dict, Game_Hero_Data *data)
{
    data->mType = dict->getItemIntValue("ID", First_Hero_ID);
    sprintf(data->mSourceName, "%s",dict->getItemStringValue("SourceName"));
    CCLOG("%s",data->mSourceName);
    data->mCoolDown = dict->getItemIntValue("CD", 0);
    data->mAtk = dict->getItemIntValue("Atk", 0);
    data->mAtk2 = dict->getItemIntValue("Atk2", 0);
    data->mAtkAdd = dict->getItemIntValue("AtkAdd", 0);
    data->mSkillAtk = dict->getItemIntValue("SkillAtk", 0);
    data->mSkillAtkAdd = dict->getItemIntValue("SkillAtkAdd", 0);
    data->mDuration = dict->getItemFloatValue("Duration", 0);
    data->mStar = dict->getItemFloatValue("Star", 0);
    data->mBuyGold = dict->getItemIntValue("BuyGold", 0);
    data->mBuyGem = dict->getItemIntValue("BuyGem", 0);
    data->mRecruitGold = dict->getItemIntValue("RecruitGold", 0);
    data->mRecruitGem = dict->getItemIntValue("RecruitGem", 0);
    data->mUpGold = dict->getItemIntValue("UpGold", 0);
    data->mUpGem = dict->getItemIntValue("UpGem", 0);
    data->mBuy = dict->getItemIntValue("Buy", 0);
    data->mMagicID = dict->getItemIntValue("MagicID", 0);
    data->mUnLockLevel = dict->getItemIntValue("unlocklevel",0);
    //    CCLOG("data->mUnLockLevel:%d",data->mUnLockLevel);
}
COutfitData** CfgDataManage::getTotalOutfitData()
{
    COutfitData** pData = NULL;
    std::string tFullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(Outfit_Info_Path);
    unsigned long size = 0;
    char* file =  (char*)cocos2d::CCFileUtils::sharedFileUtils()->getFileData(tFullPath.c_str(), "r",&size);
    cs::CSJsonDictionary *dict = new cs::CSJsonDictionary();
    dict->initWithDescription(file);

    int _total = dict->getItemCount();
    pData = new COutfitData*[_total];
    CCLOG("_total:%d",_total);
    for (int j = 0; j<_total; ++j)
    {
        cs::CSJsonDictionary *sub = dict->getSubItemByIndex(j);
        COutfitData *p = new COutfitData;
        ParseJSON_Outfit(sub, p);
        pData[j] = p;
        CC_SAFE_DELETE(sub);
    }

    CC_SAFE_DELETE(dict);
    CC_SAFE_DELETE_ARRAY(file);
    return pData;
}
void CfgDataManage::ParseJSON_Outfit(cs::CSJsonDictionary *dict, COutfitData *data)
{
    data->m_nId = dict->getItemIntValue("ID", First_Outfit_ID);
    data->m_nGoldPrice = dict->getItemIntValue("goldPrice", 0);
    data->m_nGemPrice = dict->getItemIntValue("gemPrice", 0);
    data->m_nHp = dict->getItemIntValue("Hp", 0);
    //todo
    switch (data->m_nId)
    {
        case 7007:
        case 7008:
        case 7009:
            data->m_nMaxNum = 1;
            break;

        default:
            data->m_nMaxNum = 999;
            break;
    }
    data->m_pName = CCString::createWithFormat("%s",dict->getItemStringValue("name"));
    data->m_pName->retain();
    data->m_pSourceName = CCString::createWithFormat("%s",dict->getItemStringValue("sourceName"));
    data->m_pSourceName->retain();
    data->m_pInfo = CCString::createWithFormat("%s",dict->getItemStringValue("info"));
    data->m_pInfo->retain();
    data->m_pUse = CCString::createWithFormat("%s",dict->getItemStringValue("use"));
    data->m_pUse->retain();
    //    CCLOG("%s",data->m_pName->getCString());
    //    CCLOG("%s",data->m_pSourceName->getCString());
    //    CCLOG("%s",data->m_pInfo->getCString());
    //    CCLOG("%s",data->m_pUse->getCString());
}
int CfgDataManage::getEnemyNum()
{
    CCLOG("CfgDataManage::getEnemyNum!");
    int num= 0;
    std::string tFullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(Enemy_Info_Path);
    unsigned long size = 0;
    char* file =  (char*)cocos2d::CCFileUtils::sharedFileUtils()->getFileData(tFullPath.c_str(), "r",&size);
    cs::CSJsonDictionary *dict = new cs::CSJsonDictionary();
    dict->initWithDescription(file);
    CC_SAFE_DELETE_ARRAY(file);
    num = dict->getItemCount();
    CC_SAFE_DELETE(dict);
    return num;
}

int CfgDataManage::getHeroNum()
{
    CCLOG("CfgDataManage::getHeroNum!");
    int num= 0;
    std::string tFullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(Hero_Info_Path);
    unsigned long size = 0;
    char* file =  (char*)cocos2d::CCFileUtils::sharedFileUtils()->getFileData(tFullPath.c_str(), "r",&size);
    cs::CSJsonDictionary *dict = new cs::CSJsonDictionary();
    dict->initWithDescription(file);
    CC_SAFE_DELETE_ARRAY(file);
    num = dict->getItemCount();
    CC_SAFE_DELETE(dict);

    return num;
}
int CfgDataManage::getOutfitNum()
{
    int num= 0;
    std::string tFullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(Outfit_Info_Path);
    unsigned long size = 0;
    char* file =  (char*)cocos2d::CCFileUtils::sharedFileUtils()->getFileData(tFullPath.c_str(), "r",&size);
    cs::CSJsonDictionary *dict = new cs::CSJsonDictionary();
    dict->initWithDescription(file);
    CC_SAFE_DELETE_ARRAY(file);
    num = dict->getItemCount();
    CC_SAFE_DELETE(dict);
    return num;
}

int CfgDataManage::getEnemyAwardID()
{
    int res = 0;
    std::string tFullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(Enemy_Award_Path);
    unsigned long size = 0;
    char* file =  (char*)cocos2d::CCFileUtils::sharedFileUtils()->getFileData(tFullPath.c_str(), "r",&size);
    cs::CSJsonDictionary *dict = new cs::CSJsonDictionary();
    dict->initWithDescription(file);
    CC_SAFE_DELETE_ARRAY(file);
    int idCount = dict->getItemCount();
    int itemCount = 0;
    cs::CSJsonDictionary *sub = dict->getSubItemByIndex(0);
    itemCount = sub->getItemCount();
    CCLOG("idCount:%d itemCount:%d",idCount,itemCount);
    int indexID = arc4random()%idCount;
    itemCount--;
    int indexItem = arc4random()%itemCount;
    indexItem++;  // 1 - 10;
    char buf[10];
    memset(buf, 0, 10);
    sprintf(buf, "item%d",indexItem);
    res = dict->getSubItemByIndex(indexID)->getItemIntValue(buf, 0);
    return res;
}

void CfgDataManage::SetLoadData(int mtype)
{
    LuaManager *plua = new LuaManager;
    CC_RETURN_IF(!plua);
    LuaScript* pScript = NULL;
    std::string mfilePath = CCFileUtils::sharedFileUtils()->fullPathForFilename(g_sScriptCfg);
    pScript = plua->CreateScript(mfilePath.c_str());

    if(mtype & LOAD_ROLEPARA)
        SetRoleDataList(plua);
    if(mtype & LOAD_SKILLPARA)
        SetSkillDataList(plua);
    if(mtype & LOAD_BUFFPARA)
        SetBuffDataList(plua);
    if(mtype & LOAD_ANIINFO)
        SetAniList(plua);
    if(mtype & LOAD_NSINFO)
        SetNSList(plua);
    if(mtype & LOAD_NAMEINFO)
        SetLoginNameList(plua);
    if(mtype & LOAD_PROPINFO)
        SetPropDataList(plua);
    if(mtype & LOAD_EQUTINFO)
        SetEquipDataList(plua);
    if(mtype & LOAD_PROPERTYINFO)
        SetPropertyList(plua);
    if(mtype & LOAD_COPYINFO)
        SetCopyList(plua);

    plua->DestroyScript(pScript);
    CC_SAFE_DELETE(plua);
}

void CfgDataManage::ClearCfgData()
{
    SetRoleDataList(NULL);
    SetSkillDataList(NULL);
    SetBuffDataList(NULL);
    SetAniList(NULL);
    SetNSList(NULL);
    SetLoginNameList(NULL);
    SetPropDataList(NULL);
    SetEquipDataList(NULL);
    SetPropertyList(NULL);
    SetCopyList(NULL);
    m_pTxtlua->DestroyScript(m_pScript);
    CC_SAFE_DELETE(m_pTxtlua);
}

//FIGHT_ROLEPARA_INFO *CfgDataManage::GetRoleParmInfoById(const int mroleid)
//{
//    if(m_pRoleDataList && m_pRoleDataList->pParmInfo)
//    {
//        for(int i=0; i<m_pRoleDataList->mTotalNum; i++)
//        {
//            if(m_pRoleDataList->pParmInfo[i].mRoleId == mroleid ||
//               m_pRoleDataList->pParmInfo[i].mRoleId1 == mroleid)
//                return &m_pRoleDataList->pParmInfo[i];
//        }
//    }
//    return NULL;
//}

FIGHT_SKILLPARA_INFO *CfgDataManage::GetSkillParmInfoById(const int mskillid)
{
    if(m_pSkillDataList && m_pSkillDataList->pParmInfo && mskillid>0)
    {
        for(int i=0; i<m_pSkillDataList->mTotalNum; i++)
        {
            if(m_pSkillDataList->pParmInfo[i].mSkillId == mskillid)
                return &m_pSkillDataList->pParmInfo[i];
        }
    }
    return NULL;
}

FIGHT_BUFFPARA_INFO *CfgDataManage::GetBuffParmInfoById(const int mbuffid)
{
    if(m_pBuffDataList && m_pBuffDataList->pParmInfo && mbuffid>0)
    {
        for(int i=0; i<m_pBuffDataList->mTotalNum; i++)
        {
            if(m_pBuffDataList->pParmInfo[i].mBuffId == mbuffid)
                return &m_pBuffDataList->pParmInfo[i];
        }
    }
    return NULL;
}

//FIGHT_ANI_INFO *CfgDataManage::GetAniInfoById(const int mid)
//{
//    if(m_pAniList && m_pAniList->pAniInfo && mid>=FIGHT_SKILLVIEW_SRPHYACT && mid<FIGHT_SKILLVIEW_NUM)
//    {
//        return &m_pAniList->pAniInfo[mid];
//    }
//    return NULL;
//}
//
//NS_PIC_INFO *CfgDataManage::GetNSInfoById(const int mnsid)
//{
//    if(m_pNSList && m_pNSList->pPicInfo)
//    {
//        for(int i=0; i<m_pNSList->mNum; i++)
//        {
//            if(m_pNSList->pPicInfo[i].mNsID == mnsid)
//                return &m_pNSList->pPicInfo[i];
//        }
//    }
//    return NULL;
//}

//PROP_INFO *CfgDataManage::GetPropInfoById(const int propid)
//{
//    if(m_pPropList && m_pPropList->pPropInfo)
//    {
//        for(int i=0; i<m_pPropList->mNum; i++)
//        {
//            if(m_pPropList->pPropInfo[i].mPropID == propid)
//                return &m_pPropList->pPropInfo[i];
//        }
//    }
//   return NULL;
//}
//
//EQUIP_INFO *CfgDataManage::GetEquipInfoById(const int equipid)
//{
//    if(m_pEquipList && m_pEquipList->pEquipInfo)
//    {
//        for(int i=0; i<m_pEquipList->mNum; i++)
//        {
//            if(m_pEquipList->pEquipInfo[i].mEquipID == equipid)
//                return &m_pEquipList->pEquipInfo[i];
//        }
//    }
//    return NULL;
//}

//PROPERTY_INFO *CfgDataManage::GetPropertyInfoById(const int propertyid)
//{
//    if(m_pPropertyList && m_pPropertyList->pPropertyInfo)
//    {
//        for(int i=0; i<m_pPropertyList->mNum; i++)
//        {
//            if(m_pPropertyList->pPropertyInfo[i].mPropertyID == propertyid)
//                return &m_pPropertyList->pPropertyInfo[i];
//        }
//    }
//    return NULL;
//}

//COPY_INFO *CfgDataManage::GetCopyInfoById(const int copyid)
//{
//    if(m_pCopyList && m_pCopyList->pCopyInfo)
//    {
//        for(int i=0; i<m_pCopyList->mNum; i++)
//        {
//            if(m_pCopyList->pCopyInfo[i].mCopyID == copyid)
//                return &m_pCopyList->pCopyInfo[i];
//        }
//    }
//    return NULL;
//}

std::string CfgDataManage::GetTxtNameByKey(const char *pkey)
{
    std::string mvalueStr;
    if(!m_pTxtlua)
    {
        m_pTxtlua = new LuaManager;
        if(m_pTxtlua)
        {
            std::string mfilePath = CCFileUtils::sharedFileUtils()->fullPathForFilename(g_sScriptTxtCfg);
            m_pScript = m_pTxtlua->CreateScript(mfilePath.c_str());
        }
    }
    if(pkey && m_pTxtlua)
    {
        mvalueStr = m_pTxtlua->GetString((char *)pkey);
    }
    return mvalueStr;
}

int CfgDataManage::GetTxtSizeByKey(const char *pkey)
{
    if(!m_pTxtlua)
    {
        m_pTxtlua = new LuaManager;
        if(m_pTxtlua)
        {
            std::string mfilePath = CCFileUtils::sharedFileUtils()->fullPathForFilename(g_sScriptTxtCfg);
            m_pScript = m_pTxtlua->CreateScript(mfilePath.c_str());
        }
    }
    if(pkey && m_pTxtlua)
    {
        return m_pTxtlua->GetInt((char *)pkey);
    }
    return 0;
}

//const char* CfgDataManage::GetImageNameByID(int id, int mtype, bool bhead, bool bfull)
//{
//    if(mtype==DROP_TOOL || mtype==DROP_STONE || mtype==DROP_STRENGTH || mtype==DROP_GOLDEN || mtype==DROP_NONE)
//    {
//        if(mtype==DROP_STONE)
//            id = SPECIAL_PROPID_STONE;
//        else if(mtype==DROP_STRENGTH)
//            id = SPECIAL_PROPID_STREN;
//        else if(mtype==DROP_GOLDEN)
//            id = SPECIAL_PROPID_GLODEN;
//        PROP_INFO *pPropInfo = GetPropInfoById(id);
//        if(pPropInfo)
//            return pPropInfo->mSFInfo.mImgeName;
//    }
//
//    if(mtype==DROP_EQUP || mtype==DROP_NONE)
//    {
//        EQUIP_INFO *pEquipInfo = GetEquipInfoById(id);
//        if(pEquipInfo)
//            return pEquipInfo->mSFInfo.mImgeName;
//    }
//
//    if(mtype==DROP_CARD || mtype==DROP_NONE)
//    {
//        FIGHT_ROLEPARA_INFO *pRoleInfo = GetRoleParmInfoById(id);
//        if(pRoleInfo)
//            return bhead ? pRoleInfo->mResHead.mImgeName:pRoleInfo->mResSF.mImgeName;
//    }
//
//    if(mtype==DROP_NS|| mtype==DROP_NONE)
//    {
//        NS_PIC_INFO *pNsInfo = GetNSInfoById(id);
//        if(pNsInfo)
//        {
//            if(bhead)
//                return pNsInfo->mHeadSF.mImgeName;
//            else if(bfull)
//                return pNsInfo->mBodyFullSF.mImgeName;
//            return pNsInfo->mBodySF.mImgeName;
//        }
//    }
//
//    return NULL;
//}

//const char* CfgDataManage::GetNameByID(int id, int mtype, bool bhead, bool bfull)
//{
//    if(mtype==DROP_TOOL || mtype==DROP_STONE || mtype==DROP_STRENGTH || mtype==DROP_GOLDEN || mtype==DROP_NONE)
//    {
//        if(mtype==DROP_STONE)
//            id = SPECIAL_PROPID_STONE;
//        else if(mtype==DROP_STRENGTH)
//            id = SPECIAL_PROPID_STREN;
//        else if(mtype==DROP_GOLDEN)
//            id = SPECIAL_PROPID_GLODEN;
//        PROP_INFO *pPropInfo = GetPropInfoById(id);
//        if(pPropInfo)
//            return pPropInfo->mNameStr;
//    }
//
//    if(mtype==DROP_EQUP || mtype==DROP_NONE)
//    {
//        EQUIP_INFO *pEquipInfo = GetEquipInfoById(id);
//        if(pEquipInfo)
//            return pEquipInfo->mNameStr;
//    }
//
//    if(mtype==DROP_CARD || mtype==DROP_NONE)
//    {
//        FIGHT_ROLEPARA_INFO *pRoleInfo = GetRoleParmInfoById(id);
//        if(pRoleInfo)
//            return pRoleInfo->mNameStr;
//    }
//
//    if(mtype==DROP_NS|| mtype==DROP_NONE)
//    {
//        NS_PIC_INFO *pNsInfo = GetNSInfoById(id);
//        return pNsInfo->mNameStr;
//    }
//
//    return NULL;
//}


void CfgDataManage::SetRoleDataList(LuaManager *plua)
{
    //    if(m_pRoleDataList)
    //    {
    //        CC_SAFE_DELETE_ARRAY(m_pRoleDataList->pParmInfo);
    //        CC_SAFE_DELETE(m_pRoleDataList);
    //    }
    //    CC_RETURN_IF(!plua);
    //
    //    m_pRoleDataList = new FIGHT_ROLEDATA_LIST;
    //    CC_RETURN_IF(!m_pRoleDataList);
    //    memset(m_pRoleDataList, 0x00,sizeof(FIGHT_ROLEDATA_LIST));
    //
    //    char mTpstr[USING_TXT_LEN];
    //    sprintf(mTpstr,"%s","roldid_total_num");
    //    m_pRoleDataList->mTotalNum = plua->GetInt(mTpstr);
    //    if(m_pRoleDataList->mTotalNum > 0)
    //    {
    //        m_pRoleDataList->pParmInfo = new FIGHT_ROLEPARA_INFO[m_pRoleDataList->mTotalNum];
    //        if(m_pRoleDataList->pParmInfo)
    //        {
    //            memset(m_pRoleDataList->pParmInfo,0x00,sizeof(FIGHT_ROLEPARA_INFO)*m_pRoleDataList->mTotalNum);
    //            for(int i=0; i<m_pRoleDataList->mTotalNum; i++)
    //            {
    //                sprintf(mTpstr,"rold%d_id",i+1);
    //                m_pRoleDataList->pParmInfo[i].mRoleId = plua->GetInt(mTpstr);
    //                sprintf(mTpstr,"rold%d_id1",i+1);
    //                m_pRoleDataList->pParmInfo[i].mRoleId1 = plua->GetInt(mTpstr);
    //                sprintf(mTpstr,"rold%d_short",i+1);
    //                m_pRoleDataList->pParmInfo[i].mShortId = plua->GetInt(mTpstr);
    //                sprintf(mTpstr,"rold%d_remote",i+1);
    //                m_pRoleDataList->pParmInfo[i].mRemoteId = plua->GetInt(mTpstr);
    //                sprintf(mTpstr,"rold%d_name",i+1);
    //                sprintf(m_pRoleDataList->pParmInfo[i].mResSF.mImgeName,"%s",plua->GetString(mTpstr).c_str());
    //                sprintf(mTpstr,"rold%d_min",i+1);
    //                m_pRoleDataList->pParmInfo[i].mResSF.mIndexmin = plua->GetInt(mTpstr);
    //                sprintf(mTpstr,"rold%d_max",i+1);
    //                m_pRoleDataList->pParmInfo[i].mResSF.mIndexmax = plua->GetInt(mTpstr);
    //                sprintf(mTpstr,"rold%d_left",i+1);
    //                m_pRoleDataList->pParmInfo[i].mResSF.bDirleft = plua->GetBool(mTpstr);
    //                sprintf(mTpstr,"rold%d_head",i+1);
    //                sprintf(m_pRoleDataList->pParmInfo[i].mResHead.mImgeName,"%s",plua->GetString(mTpstr).c_str());
    //                sprintf(mTpstr,"rold%d_txt",i+1);
    //                sprintf(m_pRoleDataList->pParmInfo[i].mNameStr,"%s",plua->GetString(mTpstr).c_str());
    //            }
    //        }
    //    }
}

void CfgDataManage::SetSkillDataList(LuaManager *plua)
{
    //    if(m_pSkillDataList)
    //    {
    //        CC_SAFE_DELETE_ARRAY(m_pSkillDataList->pParmInfo);
    //        CC_SAFE_DELETE(m_pSkillDataList);
    //    }
    //    CC_RETURN_IF(!plua);
    //
    //    m_pSkillDataList = new FIGHT_SKILLDATA_LIST;
    //    CC_RETURN_IF(!m_pSkillDataList);
    //    memset(m_pSkillDataList, 0x00,sizeof(FIGHT_SKILLDATA_LIST));
    //
    //    char mTpstr[USING_TXT_LEN];
    //    sprintf(mTpstr,"%s","skill_total_num");
    //    m_pSkillDataList->mTotalNum = plua->GetInt(mTpstr);
    //    sprintf(mTpstr,"%s","skill_base");
    //    m_pSkillDataList->mBaseNum = plua->GetInt(mTpstr);
    //    if(m_pSkillDataList->mTotalNum > 0)
    //    {
    //        m_pSkillDataList->pParmInfo = new FIGHT_SKILLPARA_INFO[m_pSkillDataList->mTotalNum];
    //        if(m_pSkillDataList->pParmInfo)
    //        {
    //            for(int i=0; i<m_pSkillDataList->mTotalNum; i++)
    //            {
    //                sprintf(mTpstr,"skill%d_id",i+1);
    //                m_pSkillDataList->pParmInfo[i].mSkillId = plua->GetInt(mTpstr);
    //                sprintf(mTpstr,"skill%d_txt",i+1);
    //                m_pSkillDataList->pParmInfo[i].mTxtName = plua->GetString(mTpstr);
    //                sprintf(mTpstr,"skill%d_name",i+1);
    //                m_pSkillDataList->pParmInfo[i].mStrName = plua->GetString(mTpstr);
    //                sprintf(mTpstr,"skill%d_image",i+1);
    //                m_pSkillDataList->pParmInfo[i].mImage = plua->GetString(mTpstr);
    //                sprintf(mTpstr,"skill%d_viewid",i+1);
    //                m_pSkillDataList->pParmInfo[i].mViewId = plua->GetInt(mTpstr);
    //                sprintf(mTpstr,"skill%d_efetime",i+1);
    //                m_pSkillDataList->pParmInfo[i].mHurtTime = plua->GetInt(mTpstr);
    //                sprintf(mTpstr,"skill%d_szycenter",i+1);
    //                m_pSkillDataList->pParmInfo[i].bszycenter = plua->GetBool(mTpstr);
    //            }
    //        }
    //    }
}

void CfgDataManage::SetBuffDataList(LuaManager *plua)
{
    if(m_pBuffDataList)
    {
        CC_SAFE_DELETE_ARRAY(m_pBuffDataList->pParmInfo);
        CC_SAFE_DELETE(m_pBuffDataList);
    }
    CC_RETURN_IF(!plua);

    m_pBuffDataList = new FIGHT_BUFFDATA_LIST;
    CC_RETURN_IF(!m_pBuffDataList);
    memset(m_pBuffDataList, 0x00,sizeof(FIGHT_BUFFDATA_LIST));

    char mTpstr[USING_TXT_LEN];
    sprintf(mTpstr,"%s","buff_total_num");
    m_pBuffDataList->mTotalNum = plua->GetInt(mTpstr);
    if(m_pBuffDataList->mTotalNum > 0)
    {
        m_pBuffDataList->pParmInfo = new FIGHT_BUFFPARA_INFO[m_pBuffDataList->mTotalNum];
        if(m_pBuffDataList->pParmInfo)
        {
            for(int i=0; i<m_pBuffDataList->mTotalNum; i++)
            {
                sprintf(mTpstr,"buff%d_id",i+1);
                m_pBuffDataList->pParmInfo[i].mBuffId = plua->GetInt(mTpstr);
                sprintf(mTpstr,"buff%d_tipid",i+1);
                m_pBuffDataList->pParmInfo[i].mTipId = plua->GetInt(mTpstr);
                sprintf(mTpstr,"buff%d_viewid",i+1);
                m_pBuffDataList->pParmInfo[i].mViewId = plua->GetInt(mTpstr);
            }
        }
    }
}

void CfgDataManage::SetAniList(LuaManager *plua)
{
    //    if(m_pAniList)
    //    {
    //        CC_SAFE_DELETE_ARRAY(m_pAniList->pAniInfo);
    //        CC_SAFE_DELETE(m_pAniList);
    //    }
    //    CC_RETURN_IF(!plua);
    //
    //    m_pAniList = new FIGHT_ANI_LIST;
    //    CC_RETURN_IF(!m_pAniList);
    //    memset(m_pAniList, 0x00,sizeof(FIGHT_ANI_LIST));
    //
    //    m_pAniList->mNum = FIGHT_SKILLVIEW_NUM;
    //    m_pAniList->pAniInfo = new FIGHT_ANI_INFO[m_pAniList->mNum];
    //    CC_RETURN_IF(!m_pAniList->pAniInfo);
    //    memset(m_pAniList->pAniInfo,0x00,sizeof(FIGHT_ANI_INFO)*m_pAniList->mNum);
    //    /*-------卡牌普通攻击-------*/
    //    //近程物理攻击(劈砍)
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_SRPHYACT].mSFList[FIGHT_ANITYPE_ACT],&g_tTpPhySRPKAtk,sizeof(GMRES_SF));
    //    //近程物理攻击(刺击)
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_SRPHYACT1].mSFList[FIGHT_ANITYPE_ACT],&g_tTpPhySRCCAtk,sizeof(GMRES_SF));
    //    //远程物理攻击
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_REPHYACT].mSFList[FIGHT_ANITYPE_MOVE],&g_tTpPhyRENorAtk,sizeof(GMRES_SF));
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_REPHYACT].mSFList[FIGHT_ANITYPE_EFE],&g_tTpPhyRENorEft,sizeof(GMRES_SF));
    //    //远程魔法攻击（地）
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_REMAGACT1].mSFList[FIGHT_ANITYPE_MOVE],&g_tTpYGF,sizeof(GMRES_SF));
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_REMAGACT1].mSFList[FIGHT_ANITYPE_EFE],&g_tTpYGE,sizeof(GMRES_SF));
    //    //远程魔法攻击（水）
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_REMAGACT2].mSFList[FIGHT_ANITYPE_MOVE],&g_tTpYGF,sizeof(GMRES_SF));
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_REMAGACT2].mSFList[FIGHT_ANITYPE_EFE],&g_tTpYGE,sizeof(GMRES_SF));
    //    //远程魔法攻击（风）
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_REMAGACT3].mSFList[FIGHT_ANITYPE_MOVE],&g_tTpYGF,sizeof(GMRES_SF));
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_REMAGACT3].mSFList[FIGHT_ANITYPE_EFE],&g_tTpYGE,sizeof(GMRES_SF));
    //    //远程魔法攻击（火）
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_REMAGACT4].mSFList[FIGHT_ANITYPE_MOVE],&g_tTpYGF,sizeof(GMRES_SF));
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_REMAGACT4].mSFList[FIGHT_ANITYPE_EFE],&g_tTpYGE,sizeof(GMRES_SF));
    //    //远程魔法攻击（光）
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_REMAGACT5].mSFList[FIGHT_ANITYPE_MOVE],&g_tTpYGF,sizeof(GMRES_SF));
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_REMAGACT5].mSFList[FIGHT_ANITYPE_EFE],&g_tTpYGE,sizeof(GMRES_SF));
    //    //远程魔法攻击（暗）
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_REMAGACT6].mSFList[FIGHT_ANITYPE_MOVE],&g_tTpYGF,sizeof(GMRES_SF));
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_REMAGACT6].mSFList[FIGHT_ANITYPE_EFE],&g_tTpYGE,sizeof(GMRES_SF));
    //
    //    /*-------神之语技能-------*/
    //    //生命女神－诺恩斯
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_SMNS_RNS].mSFList[FIGHT_ANITYPE_ACT],&g_tTpSzyFire1,sizeof(GMRES_SF));
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_SMNS_RNS].mSFList[FIGHT_ANITYPE_EFE],&g_tTpSzySj,sizeof(GMRES_SF));
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_SMNS_RNS].mSFList[FIGHT_ANITYPE_GD],&g_tTpSzyLight,sizeof(GMRES_SF));
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_SMNS_RNS].mSFList[FIGHT_ANITYPE_WZ],&g_tTpSzyTile,sizeof(GMRES_SF));
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_SMNS_RNS].mSFList[FIGHT_ANITYPE_DOWN],&g_tTpSzyT1,sizeof(GMRES_SF));
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_SMNS_RNS].mSFList[FIGHT_ANITYPE_TS1],&g_tTpSzyT1,sizeof(GMRES_SF));
    //
    //    //六翼战天使－米迦勒
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_LYZTS_MJL].mSFList[FIGHT_ANITYPE_ACT],&g_tTpSzy32Map,sizeof(GMRES_SF));
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_LYZTS_MJL].mSFList[FIGHT_ANITYPE_EFE],&g_tTpSzy32Efe,sizeof(GMRES_SF));
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_LYZTS_MJL].mSFList[FIGHT_ANITYPE_WZ],&g_tTpSzy32Txt,sizeof(GMRES_SF));
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_LYZTS_MJL].mSFList[FIGHT_ANITYPE_DOWN],&g_tTpSzy31NS1,sizeof(GMRES_SF));
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_LYZTS_MJL].mSFList[FIGHT_ANITYPE_TS1],&g_tTpSzy31NS1,sizeof(GMRES_SF));
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_LYZTS_MJL].mSFList[FIGHT_ANITYPE_TS2],&g_tTpSzy31NS2,sizeof(GMRES_SF));
    //    m_pAniList->pAniInfo[FIGHT_SKILLVIEW_LYZTS_MJL].mfTime[FIGHT_ANITYPE_TS1] = 1.0f;
    //    m_pAniList->pAniInfo[FIGHT_SKILLVIEW_LYZTS_MJL].mfTime[FIGHT_ANITYPE_TS2] = 1.0f;
    //    m_pAniList->pAniInfo[FIGHT_SKILLVIEW_LYZTS_MJL].mPointOFF[FIGHT_ANITYPE_ACT] = CCPointMake(150, 250);
    //    m_pAniList->pAniInfo[FIGHT_SKILLVIEW_LYZTS_MJL].mPointOFF[FIGHT_ANITYPE_TS1] = CCPointMake(200, 0);
    //
    //    //冥府主宰－哈迪斯
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_MFZZ_HDS].mSFList[FIGHT_ANITYPE_EFE],&g_tTpSzyType2Efe,sizeof(GMRES_SF));
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_MFZZ_HDS].mSFList[FIGHT_ANITYPE_WZ],&g_tTpSzyType2Txt,sizeof(GMRES_SF));
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_MFZZ_HDS].mSFList[FIGHT_ANITYPE_DOWN],&g_tTpSzyType2NS1,sizeof(GMRES_SF));
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_MFZZ_HDS].mSFList[FIGHT_ANITYPE_TS1],&g_tTpSzyType2NS1,sizeof(GMRES_SF));
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_MFZZ_HDS].mSFList[FIGHT_ANITYPE_TS2],&g_tTpSzyType2NS2,sizeof(GMRES_SF));
    //    m_pAniList->pAniInfo[FIGHT_SKILLVIEW_MFZZ_HDS].mfTime[FIGHT_ANITYPE_TS1] = 1.0f;
    //    m_pAniList->pAniInfo[FIGHT_SKILLVIEW_MFZZ_HDS].mfTime[FIGHT_ANITYPE_TS2] = 0.3f;
    //    m_pAniList->pAniInfo[FIGHT_SKILLVIEW_MFZZ_HDS].mPointOFF[FIGHT_ANITYPE_TS1] = CCPointMake(-200, 0);
    //
    //    /*-------卡牌技能特效-------*/
    //    //远程弓箭类攻击1
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_YCGJGJ1].mSFList[FIGHT_ANITYPE_MOVE],&g_tTpPhyRENorAtk,sizeof(GMRES_SF));
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_YCGJGJ1].mSFList[FIGHT_ANITYPE_EFE],&g_tTpPhyRENorEft,sizeof(GMRES_SF));
    //    //远程弓箭类攻击2
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_YCGJGJ2].mSFList[FIGHT_ANITYPE_MOVE],&g_tTpYGF,sizeof(GMRES_SF));
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_YCGJGJ2].mSFList[FIGHT_ANITYPE_EFE],&g_tTpYGE,sizeof(GMRES_SF));
    //    //HP恢复技能1
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_HPHFJL1].mSFList[FIGHT_ANITYPE_ACT],&g_tTpHpSF,sizeof(GMRES_SF));
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_HPHFJL1].mSFList[FIGHT_ANITYPE_EFE],&g_tTpHpSJ,sizeof(GMRES_SF));
    //    //MP恢复技能1
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_MPHFJL1].mSFList[FIGHT_ANITYPE_ACT],&g_tTpSklMpAtk,sizeof(GMRES_SF));
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_MPHFJL1].mSFList[FIGHT_ANITYPE_EFE],&g_tTpSklMpEft,sizeof(GMRES_SF));
    //    //暗影系近程魔法攻击技能1
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_AYJCMF1].mSFList[FIGHT_ANITYPE_ACT],&g_tTpYGF,sizeof(GMRES_SF));
    //    //暗影系远程魔法攻击技能1
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_AYYCMF1].mSFList[FIGHT_ANITYPE_ACT],&g_tTpYGF,sizeof(GMRES_SF));
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_AYYCMF1].mSFList[FIGHT_ANITYPE_EFE],&g_tTpYGE,sizeof(GMRES_SF));
    //    //近程物理攻击技能1
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_JCWLJL1].mSFList[FIGHT_ANITYPE_ACT],&g_tTpYGF,sizeof(GMRES_SF));
    //    //近程物理攻击技能2
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_JCWLJL2].mSFList[FIGHT_ANITYPE_ACT],&g_tTpJCW2,sizeof(GMRES_SF));
    //    //光明系近程魔法攻击技能1
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_GMJCMF1].mSFList[FIGHT_ANITYPE_ACT],&g_tTpGMJCMagSF,sizeof(GMRES_SF));
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_GMJCMF1].mSFList[FIGHT_ANITYPE_EFE],&g_tTpGMJCMagSJ,sizeof(GMRES_SF));
    //    //天赋魔法远程攻击技能1
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_TFMFYC1].mSFList[FIGHT_ANITYPE_MOVE],&g_tTpYGF,sizeof(GMRES_SF));
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_TFMFYC1].mSFList[FIGHT_ANITYPE_EFE],&g_tTpYGE,sizeof(GMRES_SF));
    //    //近程长枪类攻击技能1
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_JCCQJL1].mSFList[FIGHT_ANITYPE_ACT],&g_tTpPhySRCCAtk,sizeof(GMRES_SF));
    //    //近程刀类攻击技能1
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_JCDJL1].mSFList[FIGHT_ANITYPE_ACT],&g_tTpSklDaoAtk,sizeof(GMRES_SF));
    //    //远程鞭类攻击技能1
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_YCBJL1].mSFList[FIGHT_ANITYPE_ACT],&g_tTpYGF,sizeof(GMRES_SF));
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_YCBJL1].mSFList[FIGHT_ANITYPE_EFE],&g_tTpYGE,sizeof(GMRES_SF));
    //    //暗影系近程魔法攻击技能2
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_AYJCMF2].mSFList[FIGHT_ANITYPE_ACT],&g_tTpYGF,sizeof(GMRES_SF));
    //    //冰系远程魔法攻击技能1
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_BXYCMF1].mSFList[FIGHT_ANITYPE_ACT],&g_tTpSklMpAtk,sizeof(GMRES_SF));
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_BXYCMF1].mSFList[FIGHT_ANITYPE_EFE],&g_tTpSklIce1Efe,sizeof(GMRES_SF));
    //    //冰系远程魔法攻击技能2
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_BXYCMF2].mSFList[FIGHT_ANITYPE_ACT],&g_tTpSklMpAtk,sizeof(GMRES_SF));
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_BXYCMF2].mSFList[FIGHT_ANITYPE_EFE],&g_tTpSklIce2Efe,sizeof(GMRES_SF));
    //    //暗影系近程魔法攻击技能3
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_AYJCMF3].mSFList[FIGHT_ANITYPE_ACT],&g_tTpBossAtk,sizeof(GMRES_SF));
    //    //自然系远程魔法技能1
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_ZRYCMF1].mSFList[FIGHT_ANITYPE_ACT],&g_tTpHpSF,sizeof(GMRES_SF));
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_ZRYCMF1].mSFList[FIGHT_ANITYPE_EFE],&g_tTpZRMagSJ,sizeof(GMRES_SF));
    //    //BOSS技能1（猛玛中心点全体攻击）
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_MAQUANT].mSFList[FIGHT_ANITYPE_MOVE],&g_tTpSkillRun,sizeof(GMRES_SF));
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_MAQUANT].mSFList[FIGHT_ANITYPE_ACT],&g_tTpSkillRunF,sizeof(GMRES_SF));
    //    m_pAniList->pAniInfo[FIGHT_SKILLVIEW_MAQUANT].mPointOFF[FIGHT_ANITYPE_ACT] = CCPointMake(-80, 0);
    //    //冰系中心全体攻击
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_BINQUANT].mSFList[FIGHT_ANITYPE_MOVE],&g_tTpSkillBQT,sizeof(GMRES_SF));
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_BINQUANT].mSFList[FIGHT_ANITYPE_EFE],&g_tTpSkillBQTF,sizeof(GMRES_SF));
    //    //莱格拉斯中心全体攻击
    //    memcpy(&m_pAniList->pAniInfo[FIGHT_SKILLVIEW_LGLS].mSFList[FIGHT_ANITYPE_MOVE],&g_tTpLGLSSF,sizeof(GMRES_SF));
    //    m_pAniList->pAniInfo[FIGHT_SKILLVIEW_LGLS].mPointOFF[FIGHT_ANITYPE_MOVE] = CCPointMake(-120, 0);
}

void CfgDataManage::SetNSList(LuaManager *plua)
{
    //    if(m_pNSList)
    //    {
    //        CC_SAFE_DELETE_ARRAY(m_pNSList->pPicInfo);
    //        CC_SAFE_DELETE(m_pNSList);
    //    }
    //    CC_RETURN_IF(!plua);
    //
    //    m_pNSList = new NS_PIC_LIST;
    //    CC_RETURN_IF(!m_pNSList);
    //    memset(m_pNSList, 0x00,sizeof(NS_PIC_LIST));
    //
    //    char mTpstr[USING_TXT_LEN];
    //    sprintf(mTpstr,"%s","ns_total_num");
    //    m_pNSList->mNum = plua->GetInt(mTpstr);
    //    if(m_pNSList->mNum > 0)
    //    {
    //        m_pNSList->pPicInfo = new NS_PIC_INFO[m_pNSList->mNum];
    //        if(m_pNSList->pPicInfo)
    //        {
    //            for(int i=0; i<m_pNSList->mNum; i++)
    //            {
    //                sprintf(mTpstr,"ns%d_id",i+1);
    //                m_pNSList->pPicInfo[i].mNsID = plua->GetInt(mTpstr);
    //                sprintf(mTpstr,"ns%d_nametxt",i+1);
    //                sprintf(m_pNSList->pPicInfo[i].mNameStr,"%s",plua->GetString(mTpstr).c_str());
    //                sprintf(mTpstr,"ns%d_background",i+1);
    //                sprintf(m_pNSList->pPicInfo[i].mBackSF.mImgeName,"%s",plua->GetString(mTpstr).c_str());
    //                sprintf(mTpstr,"ns%d_xoff",i+1);
    //                m_pNSList->pPicInfo[i].mPointOFF.x = plua->GetInt(mTpstr);
    //                sprintf(mTpstr,"ns%d_yoff",i+1);
    //                m_pNSList->pPicInfo[i].mPointOFF.y = plua->GetInt(mTpstr);
    //                sprintf(mTpstr,"ns%d_name",i+1);
    //                sprintf(m_pNSList->pPicInfo[i].mResSF.mImgeName,"%s",plua->GetString(mTpstr).c_str());
    //                sprintf(mTpstr,"ns%d_min",i+1);
    //                m_pNSList->pPicInfo[i].mResSF.mIndexmin = plua->GetInt(mTpstr);
    //                sprintf(mTpstr,"ns%d_max",i+1);
    //                m_pNSList->pPicInfo[i].mResSF.mIndexmax = plua->GetInt(mTpstr);
    //                sprintf(mTpstr,"ns%d_left",i+1);
    //                m_pNSList->pPicInfo[i].mResSF.bDirleft = plua->GetBool(mTpstr);
    //                sprintf(mTpstr,"ns%d_txt",i+1);
    //                sprintf(m_pNSList->pPicInfo[i].mNameSF.mImgeName,"%s",plua->GetString(mTpstr).c_str());
    //                sprintf(mTpstr,"ns%d_cont",i+1);
    //                m_pNSList->pPicInfo[i].mTxtCont = plua->GetString(mTpstr);
    //                sprintf(mTpstr,"ns%d_head",i+1);
    //                sprintf(m_pNSList->pPicInfo[i].mHeadSF.mImgeName,"%s",plua->GetString(mTpstr).c_str());
    //                sprintf(mTpstr,"ns%d_body",i+1);
    //                sprintf(m_pNSList->pPicInfo[i].mBodySF.mImgeName,"%s",plua->GetString(mTpstr).c_str());
    //                sprintf(mTpstr,"ns%d_bodyfull",i+1);
    //                sprintf(m_pNSList->pPicInfo[i].mBodyFullSF.mImgeName,"%s",plua->GetString(mTpstr).c_str());
    //            }
    //        }
    //    }
}

void CfgDataManage::GetLoginNameListInfo(LuaManager *plua,LOGIN_NMAE_LIST *pList, int mindex)
{
    CC_RETURN_IF(!plua || !pList);

    char mTpstr[USING_TXT_LEN];
    sprintf(mTpstr,"randname%d_total_num",mindex);
    pList->mNum = plua->GetInt(mTpstr);
    if(pList->mNum > 0)
    {
        pList->pNameInfo = new LOGIN_NAME_INFO[pList->mNum];
        if(pList->pNameInfo)
        {
            for(int i=0; i<pList->mNum; i++)
            {
                sprintf(mTpstr,"randname%d_id%d",mindex,i+1);
                sprintf(pList->pNameInfo[i].mNameStr,"%s",plua->GetString(mTpstr).c_str());
            }
        }
    }
}

void CfgDataManage::SetLoginNameList(LuaManager *plua)
{
    if(m_pName1List)
    {
        CC_SAFE_DELETE_ARRAY(m_pName1List->pNameInfo);
        CC_SAFE_DELETE(m_pName1List);
    }
    if(m_pName2List)
    {
        CC_SAFE_DELETE_ARRAY(m_pName2List->pNameInfo);
        CC_SAFE_DELETE(m_pName2List);
    }
    CC_RETURN_IF(!plua);

    m_pName1List = new LOGIN_NMAE_LIST;
    m_pName2List = new LOGIN_NMAE_LIST;
    CC_RETURN_IF(!m_pName1List || !m_pName2List);
    memset(m_pName1List, 0x00,sizeof(LOGIN_NMAE_LIST));
    memset(m_pName2List, 0x00,sizeof(LOGIN_NMAE_LIST));

    GetLoginNameListInfo(plua,m_pName1List,1);
    GetLoginNameListInfo(plua,m_pName2List,2);
}

void CfgDataManage::SetPropDataList(LuaManager *plua)
{
    //    if(m_pPropList)
    //    {
    //        CC_SAFE_DELETE_ARRAY(m_pPropList->pPropInfo);
    //        CC_SAFE_DELETE(m_pPropList);
    //    }
    //    CC_RETURN_IF(!plua);
    //
    //    m_pPropList = new PROP_LIST;
    //    CC_RETURN_IF(!m_pPropList);
    //    memset(m_pPropList, 0x00,sizeof(PROP_LIST));
    //
    //    char mTpstr[USING_TXT_LEN];
    //    sprintf(mTpstr,"%s","prop_total_num");
    //    m_pPropList->mNum = plua->GetInt(mTpstr);
    //    if(m_pPropList->mNum > 0)
    //    {
    //        m_pPropList->pPropInfo = new PROP_INFO[m_pPropList->mNum];
    //        if(m_pPropList->pPropInfo)
    //        {
    //            for(int i=0; i<m_pPropList->mNum; i++)
    //            {
    //                sprintf(mTpstr,"prop%d_id",i+1);
    //                m_pPropList->pPropInfo[i].mPropID = plua->GetInt(mTpstr);
    //                sprintf(mTpstr,"prop%d_name",i+1);
    //                sprintf(m_pPropList->pPropInfo[i].mSFInfo.mImgeName,"%s",plua->GetString(mTpstr).c_str());
    //                sprintf(mTpstr,"prop%d_txt",i+1);
    //                sprintf(m_pPropList->pPropInfo[i].mNameStr,"%s",plua->GetString(mTpstr).c_str());
    //            }
    //        }
    //    }
}

void CfgDataManage::SetEquipDataList(LuaManager *plua)
{
    //    if(m_pEquipList)
    //    {
    //        CC_SAFE_DELETE_ARRAY(m_pEquipList->pEquipInfo);
    //        CC_SAFE_DELETE(m_pEquipList);
    //    }
    //    CC_RETURN_IF(!plua);
    //
    //    m_pEquipList = new EQUIP_LIST;
    //    CC_RETURN_IF(!m_pEquipList);
    //    memset(m_pEquipList, 0x00,sizeof(EQUIP_LIST));
    //
    //    char mTpstr[USING_TXT_LEN];
    //    sprintf(mTpstr,"%s","equip_total_num");
    //    m_pEquipList->mNum = plua->GetInt(mTpstr);
    //    if(m_pEquipList->mNum > 0)
    //    {
    //        m_pEquipList->pEquipInfo = new EQUIP_INFO[m_pEquipList->mNum];
    //        if(m_pEquipList->pEquipInfo)
    //        {
    //            for(int i=0; i<m_pEquipList->mNum; i++)
    //            {
    //                sprintf(mTpstr,"equip%d_id",i+1);
    //                m_pEquipList->pEquipInfo[i].mEquipID = plua->GetInt(mTpstr);
    //                sprintf(mTpstr,"equip%d_name",i+1);
    //                sprintf(m_pEquipList->pEquipInfo[i].mSFInfo.mImgeName,"%s",plua->GetString(mTpstr).c_str());
    //                sprintf(mTpstr,"equip%d_txt",i+1);
    //                sprintf(m_pEquipList->pEquipInfo[i].mNameStr,"%s",plua->GetString(mTpstr).c_str());
    //            }
    //        }
    //    }
}

void CfgDataManage::SetPropertyList(LuaManager *plua)
{
    //    if(m_pPropertyList)
    //    {
    //        CC_SAFE_DELETE_ARRAY(m_pPropertyList->pPropertyInfo);
    //        CC_SAFE_DELETE(m_pPropertyList);
    //    }
    //    CC_RETURN_IF(!plua);
    //
    //    m_pPropertyList = new PROPERTY_LIST;
    //    CC_RETURN_IF(!m_pPropertyList);
    //    memset(m_pPropertyList, 0x00,sizeof(PROPERTY_LIST));
    //
    //    char mTpstr[USING_TXT_LEN];
    //    sprintf(mTpstr,"%s","property_total_num");
    //    m_pPropertyList->mNum = plua->GetInt(mTpstr);
    //    if(m_pPropertyList->mNum > 0)
    //    {
    //        m_pPropertyList->pPropertyInfo = new PROPERTY_INFO[m_pPropertyList->mNum];
    //        if(m_pPropertyList->pPropertyInfo)
    //        {
    //            for(int i=0; i<m_pPropertyList->mNum; i++)
    //            {
    //                sprintf(mTpstr,"property%d_id",i+1);
    //                m_pPropertyList->pPropertyInfo[i].mPropertyID = plua->GetInt(mTpstr);
    //                sprintf(mTpstr,"property%d_name",i+1);
    //                sprintf(m_pPropertyList->pPropertyInfo[i].mSFInfo.mImgeName,"%s",plua->GetString(mTpstr).c_str());
    //                sprintf(mTpstr,"property%d_txt",i+1);
    //                sprintf(m_pPropertyList->pPropertyInfo[i].mNameStr,"%s",plua->GetString(mTpstr).c_str());
    //                sprintf(mTpstr,"property%d_type",i+1);
    //                m_pPropertyList->pPropertyInfo[i].btype = plua->GetInt(mTpstr);
    //            }
    //        }
    //    }
}

void CfgDataManage::SetCopyList(LuaManager *plua)
{
    //    if(m_pCopyList)
    //    {
    //        CC_SAFE_DELETE_ARRAY(m_pCopyList->pCopyInfo);
    //        CC_SAFE_DELETE(m_pCopyList);
    //    }
    //    CC_RETURN_IF(!plua);
    //    
    //    m_pCopyList = new COPY_LIST;
    //    CC_RETURN_IF(!m_pCopyList);
    //    memset(m_pCopyList, 0x00,sizeof(COPY_LIST));
    //    
    //    char mTpstr[USING_TXT_LEN];
    //    sprintf(mTpstr,"%s","copy_total_num");
    //    m_pCopyList->mNum = plua->GetInt(mTpstr);
    //    if(m_pCopyList->mNum > 0)
    //    {
    //        m_pCopyList->pCopyInfo = new COPY_INFO[m_pCopyList->mNum];
    //        if(m_pCopyList->pCopyInfo)
    //        {
    //            for(int i=0; i<m_pCopyList->mNum; i++)
    //            {
    //                sprintf(mTpstr,"copy%d_id",i+1);
    //                m_pCopyList->pCopyInfo[i].mCopyID = plua->GetInt(mTpstr);
    //                sprintf(mTpstr,"copy%d_name",i+1);
    //                sprintf(m_pCopyList->pCopyInfo[i].mNameStr,"%s",plua->GetString(mTpstr).c_str());
    //                sprintf(mTpstr,"copy%d_posX",i+1);
    //                m_pCopyList->pCopyInfo[i].mPoint.x = plua->GetInt(mTpstr);
    //                sprintf(mTpstr,"copy%d_posY",i+1);
    //                m_pCopyList->pCopyInfo[i].mPoint.y = plua->GetInt(mTpstr);
    //                sprintf(mTpstr,"copy%d_namePic1",i+1);
    //                sprintf(m_pCopyList->pCopyInfo[i].mNameTxt1,"%s",plua->GetString(mTpstr).c_str());
    //                sprintf(mTpstr,"copy%d_namePic2",i+1);
    //                sprintf(m_pCopyList->pCopyInfo[i].mNameTxt2,"%s",plua->GetString(mTpstr).c_str());
    //                sprintf(mTpstr,"copy%d_nameIcon1",i+1);
    //                sprintf(m_pCopyList->pCopyInfo[i].mNameImage1,"%s",plua->GetString(mTpstr).c_str());
    //                sprintf(mTpstr,"copy%d_nameIcon2",i+1);
    //                sprintf(m_pCopyList->pCopyInfo[i].mNameImage2,"%s",plua->GetString(mTpstr).c_str());
    //            }
    //        }
    //    }
}
