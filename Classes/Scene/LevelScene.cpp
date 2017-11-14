#include "LevelScene.h"
#include "cocos-ext.h"
#include "DungeonScene.h"
#include "PlanScene.h"

USING_NS_CC;
USING_NS_CC_EXT;

#pragma mark - CLevelLayer

CLevelLayer::CLevelLayer()
{

}

CLevelLayer::~CLevelLayer()
{

}
bool CLevelLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
//    setTouchEnabled(true);
    return true;
}
void CLevelLayer::onEnter()
{
    CCLayer::onEnter();
    CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
    DATAPOOL->addEffectCache(level_cup_focus_animation);
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    //map layer
    m_pUILayer = UILayer::create();
    addChild(m_pUILayer, LEVELSCENE_ZORDER_MAP);
    
    //focus
    m_pArmature = CCArmature::create("dianji");
    if (m_pArmature)
    {
        m_pArmature->getAnimation()->play("dianji");
        m_pArmature->setPosition(ccp(winSize.width/2, winSize.height/2));
        addChild(m_pArmature, LEVELSCENE_ZORDER_FOCUS);
    }
    
    //back menu
    CCMenuItemImage *pBackItem = CCMenuItemImage::create(NULL, \
                                                         NULL, \
                                                         this, \
                                                         menu_selector(CLevelLayer::CBBack));
    pBackItem->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(btn_back));
    pBackItem->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(btn_back2));
    pBackItem->setPosition(ccp(MIN_SPACE_TO_SCREEN_EDGE + pBackItem->getContentSize().width/2, MIN_SPACE_TO_SCREEN_EDGE + pBackItem->getContentSize().height/2));
    CCMenu *pMenu = CCMenu::create(pBackItem, NULL);
    pMenu->setPosition(CCPointZero);
    addChild(pMenu, LEVELSCENE_ZORDER_MENU);

    //map
    MapInit();
    
    //teach layer
    if(0 == DATAPOOL->currentChapter)
    {
        LevelRecord *pRcd1 = DATAPOOL->getLevelRecord(0, 0);
        LevelRecord *pRcd2 = DATAPOOL->getLevelRecord(0, 1);
        if (NULL == pRcd1 || NULL == pRcd2)
        {
            return;
        }
        
        //进入第一关
        if (0 == pRcd1->m_nScore
            && false == DATAPOOL->m_bTeachStepOver[TEACH_STEP_ENTER_LEVEL1])
        {
            UIButton *pBtn1 = dynamic_cast<UIButton*>(m_pUILayer->getWidgetByTag(LEVEL_BUTTON_TAG_BASE + 0));
            m_pTeachLayer = CTeachLayer::create();
            m_pTeachLayer->setContentSize(winSize);
            if(pBtn1)
            {
                CCPoint pos1 = ccp(pBtn1->getPosition().x + m_offsetX, pBtn1->getPosition().y + m_offsetY);
                m_pTeachLayer->setFocusProp(pos1, 80);
            }
            m_pTeachLayer->setPicInfo(CCPoint(winSize.width/2, winSize.height/2), teach_enter_level);
            m_pTeachLayer->setCallbackFun(this, callfunc_selector(CLevelLayer::CBTeachEnd1));
            addChild(m_pTeachLayer, LEVELSCENE_ZORDER_TEACH, LEVEL_LAYER_TAG_TEACH);
        }
        //进入第二关
        else if (pRcd1->m_nScore > 0
                 && 0 == pRcd2->m_nScore
                 && DATAPOOL->m_bTeachStepOver[TEACH_STEP_ENTER_LEVEL1]
                 && false == DATAPOOL->m_bTeachStepOver[TEACH_STEP_ENTER_LEVEL2])
        {
            UIButton *pBtn2 = dynamic_cast<UIButton*>(m_pUILayer->getWidgetByTag(LEVEL_BUTTON_TAG_BASE + 1));
            m_pTeachLayer = CTeachLayer::create();
            m_pTeachLayer->setContentSize(winSize);
            if(pBtn2)
            {
                CCPoint pos2 = ccp(pBtn2->getPosition().x + m_offsetX, pBtn2->getPosition().y + m_offsetY);
                m_pTeachLayer->setFocusProp(pos2, 80);
            }
            m_pTeachLayer->setPicInfo(CCPoint(winSize.width/2, winSize.height/4), teach_enter_level);
            m_pTeachLayer->setCallbackFun(this, callfunc_selector(CLevelLayer::CBTeachEnd2));
            addChild(m_pTeachLayer, LEVELSCENE_ZORDER_TEACH, LEVEL_LAYER_TAG_TEACH);
        }
    }
    
}


void CLevelLayer::onExit()
{
    m_pUILayer->removeFromParent();
    
    SceneReader::sharedSceneReader()->purgeSceneReader();
    GUIReader::shareReader()->purgeGUIReader();
	ActionManager::purgeActionManager();
    CCArmatureDataManager::purge();
    
    CCLayer::onExit();
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}

void CLevelLayer::ToNext(int nLevelIndex)
{
    DATAPOOL->m_nCurrentLevel[DATAPOOL->currentChapter] = nLevelIndex;
    
    xSM->forward(ST_PLan, PLANSCENE_TYPE_PVE, NULL);
//    xSM->forward(ST_Fight, Game_Type_Level, NULL);
}

void CLevelLayer::CBBack(CCObject* pSender)
{
    xData->playKeytone();
    xSM->back();
}
void CLevelLayer::CBTeachEnd1()
{
    DATAPOOL->m_bTeachStepOver[TEACH_STEP_ENTER_LEVEL1] = true;
    DATAPOOL->saveArchive();
    
    this->removeChildByTag(LEVEL_LAYER_TAG_TEACH, true);
    
    ToNext(0);
}
void CLevelLayer::CBTeachEnd2()
{
    DATAPOOL->m_bTeachStepOver[TEACH_STEP_ENTER_LEVEL2] = true;
    DATAPOOL->saveArchive();
    
    this->removeChildByTag(LEVEL_LAYER_TAG_TEACH, true);
    
    ToNext(1);
}
void CLevelLayer::MapInit()
{
    int nChapter = DATAPOOL->currentChapter;
    if(nChapter >= MAX_CHAPTER_NUM)
    {
        nChapter = MAX_CHAPTER_NUM - 1;
    }
    int nUnlock = DATAPOOL->m_nMaxUnlockLevel[nChapter];
    int nLevel = DATAPOOL->m_nCurrentLevel[nChapter];
    CCLOG("curChapter=%d,nUnlockLevel=%d,level=%d",nChapter, nUnlock,nLevel);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    // map root from json
    Layout* pMapRoot = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(CCString::createWithFormat(level_map_json_format, nChapter+1)->getCString()));
    pMapRoot->setTag(LEVEL_JSON_ROOT_TAG);
    pMapRoot->setPosition(ccp(winSize.width/2 - pMapRoot->getContentSize().width/2, winSize.height/2 - pMapRoot->getContentSize().height/2));
    m_pUILayer->addWidget(pMapRoot);
    m_offsetX = winSize.width/2 - pMapRoot->getContentSize().width/2;
    m_offsetY = winSize.height/2 - pMapRoot->getContentSize().height/2;
    
    // build button on map root
    UILayout *pCupPanel = static_cast<UILayout*>(pMapRoot->getChildByName("Panel_cup"));
    for (int i = 0; i < pCupPanel->getChildren()->count(); ++i)
    {
        //依赖于编辑器中预设tag    pButton->setTag(LEVEL_BUTTON_TAG + i);
        UIButton *pButton = static_cast<UIButton*>(pCupPanel->getChildren()->objectAtIndex(i));
        pButton->addReleaseEvent(this, coco_releaseselector(CLevelLayer::buildTouchEvent));
        pButton->setPressedActionEnabled(true);
        
        int nIndex = pButton->getTag() - LEVEL_BUTTON_TAG_BASE;
        bool bUnlock = (nIndex <= nUnlock);
        bool bBossLevel = isBossLevel(nChapter, nIndex);
 //       CCLOG("level index=%d,unlock=%d,tag=%d",nIndex, nUnlock, pButton->getTag());
        if (bUnlock)
        {
            pButton->active();
            
            //奖杯等级
            int nScore = xData->getLevelScoreStar(nChapter, i);
       //     CCLOG("level score=%d",nScore);
            if(bBossLevel)
            {
                switch (nScore)
                {
                    case 0:
                        pButton->loadTextures(level_cup_boss, level_cup_boss, level_cup_boss, UI_TEX_TYPE_PLIST);
                        break;
                        
                    case 1:
                        pButton->loadTextures(level_cup_1a, level_cup_1a, level_cup_lock2, UI_TEX_TYPE_PLIST);
                        break;
                        
                    case 2:
                        pButton->loadTextures(level_cup_2a, level_cup_2a, level_cup_lock2, UI_TEX_TYPE_PLIST);
                        break;
                        
                    case 3:
                        pButton->loadTextures(level_cup_3a, level_cup_3a, level_cup_lock2, UI_TEX_TYPE_PLIST);
                        break;
                }
            }
            else
            {
                switch (nScore)
                {
                    case 0:
                        pButton->loadTextures(level_cup_normal, level_cup_normal, level_cup_normal, UI_TEX_TYPE_PLIST);
                        break;
                        
                    case 1:
                        pButton->loadTextures(level_cup_1, level_cup_1, level_cup_lock, UI_TEX_TYPE_PLIST);
                        break;
                        
                    case 2:
                        pButton->loadTextures(level_cup_2, level_cup_2, level_cup_lock, UI_TEX_TYPE_PLIST);
                        break;
                        
                    case 3:
                        pButton->loadTextures(level_cup_3, level_cup_3, level_cup_lock, UI_TEX_TYPE_PLIST);
                        break;
                }
            }
            
            //选中光效
            if(nIndex == nLevel && m_pArmature)
            {
                m_pArmature->setPosition(pMapRoot->convertToWorldSpace(pButton->getPosition()));
            }
        }
        else
        {
            pButton->disable();
            
            if(bBossLevel)
            {
                pButton->loadTextures(level_cup_lock2, level_cup_lock2, level_cup_lock2, UI_TEX_TYPE_PLIST);
            }
            else
            {
                pButton->loadTextures(level_cup_lock, level_cup_lock, level_cup_lock, UI_TEX_TYPE_PLIST);
            }
        }
    }
}


void CLevelLayer::buildTouchEvent(CCObject *pSender)
{
    xData->playKeytone();
    
    UIButton* pButton = dynamic_cast<UIButton*>(pSender);

    int index = pButton->getTag() - LEVEL_BUTTON_TAG_BASE;
    
    int nChapter = DATAPOOL->currentChapter;
    int nUnlock = DATAPOOL->m_nMaxUnlockLevel[nChapter];
    if (index <= nUnlock)
    {
        ToNext(index);
    }
}

//是否是boss关卡
bool CLevelLayer::isBossLevel(int chapter, int level)
{
    return (4 == level
            || 9 == level);
}

#pragma mark - CLevelScene

CLevelScene::CLevelScene()
{
    m_pMainLayer = NULL;
}
CLevelScene::~CLevelScene()
{
    
}
bool CLevelScene::init()
{
    bool bRst = false;
    
    do
    {
        CC_BREAK_IF(!CCScene::create());
        
        m_pMainLayer = CLevelLayer::create();
        CC_BREAK_IF(!m_pMainLayer);
        addChild(m_pMainLayer);
        
        bRst = true;
    } while (0);
    
    return bRst;
}
