#include "DungeonScene.h"
#include "cocos-ext.h"
#include "HomeScene.h"
#include "LevelScene.h"

USING_NS_CC_EXT;
USING_NS_CC;

#pragma mark - CDungeonLayer

CDungeonLayer::CDungeonLayer()
{

}
CDungeonLayer::~CDungeonLayer()
{

}

bool CDungeonLayer::init()
{
    return (CCLayer::init());
}
void CDungeonLayer::onEnter()
{
    CCLayer::onEnter();
    
    CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(dungeon_island_plist, dungeon_island_image);
    if (!xData->fubenTutorialsEnd())
    {
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(fuben_info_plist, fuben_info_image);
    }
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    IslandInit();
    
    //back menu
    CCMenuItemImage *pBackItem = CCMenuItemImage::create(NULL, \
                                                         NULL, \
                                                         this, \
                                                         menu_selector(CDungeonLayer::CBBack));
    pBackItem->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(btn_back));
    pBackItem->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(btn_back2));
    pBackItem->setPosition(ccp(MIN_SPACE_TO_SCREEN_EDGE + pBackItem->getContentSize().width/2, MIN_SPACE_TO_SCREEN_EDGE + pBackItem->getContentSize().height/2));
    CCMenu *pMenu = CCMenu::create(pBackItem, NULL);
    pMenu->setPosition(CCPointZero);
    addChild(pMenu, DUNGEON_ZORDER_MENU);
    
    //teach layer
    if (false == DATAPOOL->m_bTeachStepOver[TEACH_STEP_ENTER_FUBEN1])
    {
        UIButton *pBtn = dynamic_cast<UIButton*>(m_pUILayer->getWidgetByTag(ISLAND_CHECKBOX_TAG_BASE+1));
        
        m_pTeachLayer = CTeachLayer::create();
        m_pTeachLayer->setContentSize(winSize);
        if(pBtn)
        {
            m_pTeachLayer->setFocusProp(ccp(m_offsetX + pBtn->getPosition().x, m_offsetY + pBtn->getPosition().y), 180);
        }
        m_pTeachLayer->setPicInfo(CCPoint(winSize.width/2, winSize.height/2 - 150), teach_enter_fuben1);
        m_pTeachLayer->setCallbackFun(this, callfunc_selector(CDungeonLayer::CBTeachEnd1));
        addChild(m_pTeachLayer, DUNGEON_ZORDER_TEACH, ISLAND_TAG_TEACH);
    }
    else if (1 == xData->currentChapter
             && false == xData->m_bTeachStepOver[TEACH_STEP_ENTER_FUBEN2])
    {
        AddIntroduceMaskBg(1);
        
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        CCSprite *pInfo = CCSprite::createWithSpriteFrameName(teach_enter_fuben2);
        m_pTeachLayer = CTeachLayer::create();
        m_pTeachLayer->setContentSize(winSize);
        m_pTeachLayer->setFocusProp(ccp(winSize.width/2, winSize.height/2), winSize.height/2);
        m_pTeachLayer->setPicInfo(ccp(winSize.width/2, pInfo->getContentSize().height/2 + 100), teach_enter_fuben2);
        m_pTeachLayer->setCallbackFun(this, callfunc_selector(CDungeonLayer::CBTeachEnd2));
        m_pTeachLayer->setAnimation(false);
        addChild(m_pTeachLayer, DUNGEON_ZORDER_TEACH, ISLAND_TAG_TEACH);
    }
    else if (2 == xData->currentChapter
             && false == xData->m_bTeachStepOver[TEACH_STEP_ENTER_FUBEN3])
    {
        AddIntroduceMaskBg(2);
        
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        CCSprite *pInfo = CCSprite::createWithSpriteFrameName(teach_enter_fuben3);
        m_pTeachLayer = CTeachLayer::create();
        m_pTeachLayer->setContentSize(winSize);
        m_pTeachLayer->setFocusProp(ccp(winSize.width/2, winSize.height/2), winSize.height/2);
        m_pTeachLayer->setPicInfo(ccp(winSize.width/2, pInfo->getContentSize().height/2 + 100), teach_enter_fuben3);
        m_pTeachLayer->setCallbackFun(this, callfunc_selector(CDungeonLayer::CBTeachEnd3));
        m_pTeachLayer->setAnimation(false);
        addChild(m_pTeachLayer, DUNGEON_ZORDER_TEACH, ISLAND_TAG_TEACH);
    }
    else if (3 == xData->currentChapter
             && false == xData->m_bTeachStepOver[TEACH_STEP_ENTER_FUBEN4])
    {
        AddIntroduceMaskBg(3);
        
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        CCSprite *pInfo = CCSprite::createWithSpriteFrameName(teach_enter_fuben4);
        m_pTeachLayer = CTeachLayer::create();
        m_pTeachLayer->setContentSize(winSize);
        m_pTeachLayer->setFocusProp(ccp(winSize.width/2, winSize.height/2), winSize.height/2);
        m_pTeachLayer->setPicInfo(ccp(winSize.width/2, winSize.height - pInfo->getContentSize().height/2), teach_enter_fuben4);
        m_pTeachLayer->setCallbackFun(this, callfunc_selector(CDungeonLayer::CBTeachEnd4));
        m_pTeachLayer->setAnimation(false);
        addChild(m_pTeachLayer, DUNGEON_ZORDER_TEACH, ISLAND_TAG_TEACH);
    }
    else if (4 == xData->currentChapter
             && false == xData->m_bTeachStepOver[TEACH_STEP_ENTER_FUBEN5])
    {
        AddIntroduceMaskBg(4);
        
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        CCSprite *pInfo = CCSprite::createWithSpriteFrameName(teach_enter_fuben5);
        m_pTeachLayer = CTeachLayer::create();
        m_pTeachLayer->setContentSize(winSize);
        m_pTeachLayer->setFocusProp(ccp(winSize.width/2, winSize.height/2), winSize.height/2);
        m_pTeachLayer->setPicInfo(ccp(winSize.width/2, winSize.height - pInfo->getContentSize().height/2), teach_enter_fuben5);
        m_pTeachLayer->setCallbackFun(this, callfunc_selector(CDungeonLayer::CBTeachEnd5));
        m_pTeachLayer->setAnimation(false);
        addChild(m_pTeachLayer, DUNGEON_ZORDER_TEACH, ISLAND_TAG_TEACH);
    }

    m_bSwitching = false;
}

void CDungeonLayer::onExit()
{
    this->removeAllChildrenWithCleanup(true);
    GUIReader::shareReader()->purgeGUIReader();
    DictionaryHelper::shareHelper()->purgeDictionaryHelper();
    ActionManager::shareManager()->purgeActionManager();
    CCDirector::sharedDirector()->purgeCachedData();
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(dungeon_island_plist);
    CCLayer::onExit();
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}

void CDungeonLayer::IslandInit()
{
    do
    {
        m_pUILayer = UILayer::create();
        addChild(m_pUILayer, DUNGEON_ZORDER_BG);
        
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();

        // root from json
        Layout *pIslandRoot = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(dungeon_island_json));
        CC_BREAK_IF(!pIslandRoot);
        pIslandRoot->setTag(ISLAND_ROOT_TAG);
        pIslandRoot->setPosition(ccp(winSize.width/2 - pIslandRoot->getContentSize().width/2, winSize.height/2 - pIslandRoot->getContentSize().height/2));
        m_pUILayer->addWidget(pIslandRoot);
        m_offsetX = winSize.width/2 - pIslandRoot->getContentSize().width/2;
        m_offsetY = winSize.height/2 - pIslandRoot->getContentSize().height/2;
        
        // box
        // 这里依赖编辑器中设定的tag依次为ISLAND_CHECKBOX_TAG_BASE+1、ISLAND_CHECKBOX_TAG_BASE+2、...ISLAND_CHECKBOX_TAG_BASE+5
        Layout* box_layout = static_cast<Layout*>(pIslandRoot->getChildByName("box_Panel"));
        CC_BREAK_IF(!box_layout);
        int nChapterNum = DATAPOOL->m_nMaxUnlockChapter;
        int nChapterIndex = DATAPOOL->currentChapter;
        for (int i = 0; i < box_layout->getChildren()->count(); ++i)
        {
            UIButton *pBtn = dynamic_cast<UIButton*>(box_layout->getChildren()->objectAtIndex(i));
            pBtn->addReleaseEvent(this, coco_releaseselector(CDungeonLayer::IslandTouchEvent));
            (i <= nChapterNum)?(pBtn->active()):(pBtn->disable());
        }
    } while (0);
    
}

void CDungeonLayer::AddIntroduceMaskBg(int goalIndex)
{
    if (goalIndex >= MAX_CHAPTER_NUM)
    {
        return;
    }
    int index = goalIndex;
    m_nCurIndex = index;
    CCLayerColor *pMask = CCLayerColor::create(ccc4(0,0,0,110));
    addChild(pMask, DUNGEON_ZORDER_MASK, ISLAND_TAG_MASK);
    
    UIButton *pBtn = dynamic_cast<UIButton*>(m_pUILayer->getWidgetByTag(ISLAND_CHECKBOX_TAG_BASE + index + 1));
    CCSprite *pSingleIsland = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("fubenditu_%d.png", index+1)->getCString());
    pSingleIsland->setPosition(ccp(m_offsetX + pBtn->getPosition().x, m_offsetY + pBtn->getPosition().y));
    addChild(pSingleIsland, DUNGEON_ZORDER_SINGLE_ISLAND, ISLAND_TAG_SINGLE_ISLAND);
}

void CDungeonLayer::IslandTouchEvent(CCObject *pSender)
{
    UIButton* pSenderCheckBox = dynamic_cast<UIButton*>(pSender);
    if(NULL == pSenderCheckBox)
    {
        return;
    }
    
    int index = pSenderCheckBox->getTag() - ISLAND_CHECKBOX_TAG_BASE - 1;
    
    //点击效果
    float curScaleX = this->getScaleX();//防止之前scale的值被修改丢失
    float curScaleY = this->getScaleY();
    CCActionInterval* sAction = CCScaleTo::create(0.1, curScaleX*1.2, curScaleY*1.2);
    CCActionInterval* sAction2 = CCScaleTo::create(0.1, curScaleX*1.0, curScaleX*1.0);
    m_nCurIndex = index;
    CCCallFunc*  callFun = CCCallFunc::create(this,callfunc_selector(CDungeonLayer::clickedIsland));
    pSenderCheckBox->runAction(CCSequence::create(sAction,sAction2,callFun,NULL));
}
                                                  
void CDungeonLayer::clickedIsland()
{
    ToNext(m_nCurIndex);
}
void CDungeonLayer::CBBack(CCObject* pSender)
{
    xData->playKeytone();
    xSM->back();
}
void CDungeonLayer::CBTeachEnd1()
{
    xData->playKeytone();
    
    this->removeChildByTag(ISLAND_TAG_TEACH, true);
    DATAPOOL->m_bTeachStepOver[TEACH_STEP_ENTER_FUBEN1] = true;
    DATAPOOL->saveArchive();
    
    ToNext(0);
}
void CDungeonLayer::CBTeachEnd2()
{
    xData->playKeytone();
    
    this->removeChildByTag(ISLAND_TAG_MASK, true);
    this->removeChildByTag(ISLAND_TAG_SINGLE_ISLAND, true);
    this->removeChildByTag(ISLAND_TAG_TEACH, true);
    xData->m_bTeachStepOver[TEACH_STEP_ENTER_FUBEN2] = true;
    xData->saveArchive();
    
    ToNext(1);
}
void CDungeonLayer::CBTeachEnd3()
{
    xData->playKeytone();
    
    this->removeChildByTag(ISLAND_TAG_MASK, true);
    this->removeChildByTag(ISLAND_TAG_SINGLE_ISLAND, true);
    this->removeChildByTag(ISLAND_TAG_TEACH, true);
    xData->m_bTeachStepOver[TEACH_STEP_ENTER_FUBEN3] = true;
    xData->saveArchive();
    
    ToNext(2);
}
void CDungeonLayer::CBTeachEnd4()
{
    xData->playKeytone();
    
    this->removeChildByTag(ISLAND_TAG_MASK, true);
    this->removeChildByTag(ISLAND_TAG_SINGLE_ISLAND, true);
    this->removeChildByTag(ISLAND_TAG_TEACH, true);
    xData->m_bTeachStepOver[TEACH_STEP_ENTER_FUBEN4] = true;
    xData->saveArchive();
    
    ToNext(3);
}
void CDungeonLayer::CBTeachEnd5()
{
    xData->playKeytone();
    
    this->removeChildByTag(ISLAND_TAG_MASK, true);
    this->removeChildByTag(ISLAND_TAG_SINGLE_ISLAND, true);
    this->removeChildByTag(ISLAND_TAG_TEACH, true);
    xData->m_bTeachStepOver[TEACH_STEP_ENTER_FUBEN5] = true;
    xData->saveArchive();
    
    ToNext(4);
}
void CDungeonLayer::ToNext(int nChapterIndex)
{
    if(!m_bSwitching)
    {
        m_bSwitching = true;
        
        xData->playKeytone();
        
        DATAPOOL->currentChapter = nChapterIndex;
        
        xSM->forward(ST_Level);
    }
}

#pragma mark - CDungeonScene

CDungeonScene::CDungeonScene(void)
{
	m_pMainLayer = NULL;
}

CDungeonScene::~CDungeonScene(void)
{
}
bool CDungeonScene::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!CCScene::init());
        
		m_pMainLayer = CDungeonLayer::create();
		CC_BREAK_IF(!m_pMainLayer);
		addChild(m_pMainLayer);
        
		bRet = true;
        
	} while (0);
    
	return bRet;
}