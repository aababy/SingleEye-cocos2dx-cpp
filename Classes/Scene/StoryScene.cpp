#include "StoryScene.h"

USING_NS_CC;
USING_NS_CC_EXT;

#pragma mark - CStoryLayer

bool CStoryLayer::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    m_uStepCount = 0;

    return true;
	
}
void CStoryLayer::onEnter()
{
    do
	{
		CCLayer::onEnter();

        //////////////////////////////////////////////////////////
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        
        CCMenuItemImage *pNextItem = CCMenuItemImage::create(NULL, \
                                                             NULL, \
                                                             this, \
                                                             menu_selector(CStoryLayer::CBSkip));
        pNextItem->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(btn_next));
        pNextItem->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(btn_next2));
        pNextItem->setPosition(ccp(origin.x + visibleSize.width - pNextItem->getContentSize().width/2,
                                   origin.y + pNextItem->getContentSize().height/2));
        CCMenu* pMenu = CCMenu::create(pNextItem, NULL);
        pMenu->setPosition(CCPointZero);
        addChild(pMenu, STORY_SCENE_ZORDER_MENU);
        
        for (int i = 0; i < STORY_SCENE_STEP_NUM; i++)
        {
            DATAPOOL->addEffectCache(storyJsonArray[i]);
            
            CCArmature* pArmature = CCArmature::create(storyAniArray[i]);
            pArmature->setOpacity(0);
            addChild(pArmature, STORY_SCENE_ZORDER_STORY, STORY_SCENE_TAG_ANIMATION_BASE + i);
            //     CCLOG("pArmature=%d",pArmature);
        }
        
        setStep(m_uStepCount);
        
	} while (0);
    
}
void CStoryLayer::onExit()
{
    removeAllChildrenWithCleanup(true);
    for (int i = 0; i < STORY_SCENE_STEP_NUM; i++)
    {
        DATAPOOL->removeEffectCache(storyJsonArray[i]);
    }
    CCArmatureDataManager::sharedArmatureDataManager()->purge();
    unscheduleAllSelectors();
    CCDirector::sharedDirector()->purgeCachedData();
    
    CCLayer::onExit();
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}

void CStoryLayer::CBSkip(CCObject* pSender)
{
    xData->playKeytone();
    toNext();
}
void CStoryLayer::CBStepEnd(cocos2d::extension::CCArmature *armature, cocos2d::extension::MovementEventType movementType, const char *movementID)
{
    CCLOG("enter CBStepEnd,type=%d,step=%d,id=%s",movementType,m_uStepCount,movementID);
    
    if(LOOP_COMPLETE == movementType
       || COMPLETE == movementType)
    {
        string id = movementID;
        
        if (m_uStepCount >= STORY_SCENE_STEP_NUM - 1)
        {
            toNext();
            return;
        }
        else
        {
            ++m_uStepCount;
            setStep(m_uStepCount);
        }
    }
    

}
void CStoryLayer::CBPlayArmature()
{
    CCLOG("enter CBPlayArmature,step=%d",m_uStepCount);
    
    CCArmature *pArmature = dynamic_cast<CCArmature*>(getChildByTag(STORY_SCENE_TAG_ANIMATION_BASE + m_uStepCount));
    if (NULL != pArmature)
    {
        pArmature->getAnimation()->playByIndex(0);
        pArmature->getAnimation()->setMovementEventCallFunc(this,movementEvent_selector(CStoryLayer::CBStepEnd));
    }
}
void CStoryLayer::setStep(int step)
{
    CCLOG("enter setStep,step=%d",step);
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    CCFadeIn *fadeIn = CCFadeIn::create(0.8);
    CCSequence *seq = CCSequence::create(fadeIn,
                                         CCCallFunc::create(this, callfunc_selector(CStoryLayer::CBPlayArmature)),
                                         NULL);
    
    CCArmature *armatureArray[STORY_SCENE_STEP_NUM];
    for (int i = 0; i < STORY_SCENE_STEP_NUM; i++)
    {
        DATAPOOL->addEffectCache(storyJsonArray[i]);
        
        CCArmature* pArmature = dynamic_cast<CCArmature*>(getChildByTag(STORY_SCENE_TAG_ANIMATION_BASE + i));
        armatureArray[i] = pArmature;
    //    CCLOG("pArmature=%d",pArmature);
    }
    
    switch (step)
    {
        case 0:
        {
            armatureArray[0]->setPosition(ccp(winSize.width/2, winSize.height/2));
            armatureArray[0]->runAction(seq);
        }
            break;
            
        case 1:
        {
            armatureArray[0]->getAnimation()->stop();
            armatureArray[0]->setOpacity(0);
            
            armatureArray[1]->setPosition(ccp(winSize.width/2, winSize.height/2 + armatureArray[2]->boundingBox().size.height/2 + armatureArray[1]->boundingBox().size.height/2));
            armatureArray[1]->runAction((CCSequence*)seq->copy());
        }
            break;

        case 2:
        {
            armatureArray[1]->getAnimation()->stop();
            
            armatureArray[2]->setPosition(ccp(winSize.width/2, winSize.height/2));
            armatureArray[2]->runAction((CCSequence*)seq->copy());
        }
            break;
            
        case 3:
        {
            armatureArray[2]->getAnimation()->stop();
            
            armatureArray[3]->setPosition(ccp(winSize.width/2, winSize.height/2 - armatureArray[2]->boundingBox().size.height/2 - armatureArray[3]->boundingBox().size.height/2 + 80));
            armatureArray[3]->runAction((CCSequence*)seq->copy());
        }
            break;
            
        case 4:
        {
          armatureArray[3]->getAnimation()->stop();
            
            armatureArray[1]->setOpacity(0);
            armatureArray[2]->setOpacity(0);
            armatureArray[3]->setOpacity(0);
            
            armatureArray[4]->setPosition(ccp(winSize.width/2, winSize.height/2 + armatureArray[5]->boundingBox().size.height/2 + armatureArray[4]->boundingBox().size.height/2 - 20));
            armatureArray[4]->runAction((CCSequence*)seq->copy());
        }
            break;
            
        case 5:
        {
            armatureArray[4]->getAnimation()->stop();
            
            armatureArray[5]->setPosition(ccp(winSize.width/2, winSize.height/2 + armatureArray[5]->boundingBox().size.height/2 - 90));
            armatureArray[5]->runAction((CCSequence*)seq->copy());
        }
            break;
            
        case 6:
        {
            armatureArray[5]->getAnimation()->stop();
            
            armatureArray[6]->setPosition(ccp(winSize.width/2, winSize.height/2 - armatureArray[6]->boundingBox().size.height/2 - 70));
            armatureArray[6]->runAction((CCSequence*)seq->copy());
        }
            break;
            
        case 7:
        {
            armatureArray[6]->getAnimation()->stop();
            
            armatureArray[4]->setOpacity(0);
            armatureArray[5]->setOpacity(0);
            armatureArray[6]->setOpacity(0);
            
            armatureArray[7]->setPosition(ccp(winSize.width/2, winSize.height/2 + armatureArray[7]->boundingBox().size.height/2 - 100 ));
            armatureArray[7]->runAction((CCSequence*)seq->copy());
        }
            break;
            
        case 8:
        {
            armatureArray[7]->getAnimation()->stop();
            
            armatureArray[8]->setPosition(ccp(winSize.width/2, winSize.height/2 - armatureArray[8]->boundingBox().size.height/2 - 100));
            armatureArray[8]->runAction((CCSequence*)seq->copy());
        }
            break;
            
        case 9:
        {
            armatureArray[8]->getAnimation()->stop();
            armatureArray[7]->setOpacity(0);
            armatureArray[8]->setOpacity(0);
            
            armatureArray[9]->setPosition(ccp(winSize.width/2, winSize.height/2));
            armatureArray[9]->runAction((CCSequence*)seq->copy());
        }
            break;
    }
}

void CStoryLayer::toNext()
{
    DATAPOOL->setFirstTime(false);
    DATAPOOL->saveArchive();

    if (xData->checkSign()) {
        xSM->enableTransit();
        xSM->forward(ST_Home, 1, NULL);
    }
    else
    {
        xSM->forward(ST_Sign, 1, NULL);
        
    }
}

#pragma mark - CStoryScene

CStoryScene::CStoryScene()
{
    m_pMainLayer = NULL;
}
CStoryScene::~CStoryScene()
{

}

bool CStoryScene::init()
{
    bool bRst = false;
    
    do
    {
		CC_BREAK_IF(!CCScene::init());
        
		m_pMainLayer = CStoryLayer::create();
		CC_BREAK_IF(!m_pMainLayer);
		addChild(m_pMainLayer);
        
        bRst = true;
    } while (0);
    
    return bRst;
}

