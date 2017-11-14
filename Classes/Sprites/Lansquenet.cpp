//
//  Lansquenet.cpp
//  SingleEye
//
//  Created by qinfen on 13-12-11.
//
//

#include "Lansquenet.h"

Lansquenet::Lansquenet(Enemy_Data *pData,CCLayer *layer,int level):
m_pSprite(NULL),
m_nType(pData->mType),
m_nHp(pData->mHp),
m_nDef(20),
m_sprBarrier(NULL),
m_bUse(true)
{
    m_pParentLayer =(FightLayer*)layer;
    int _atk = pData->mAtk2*2 + (pData->mAtkAdd)*level;
    m_nAtk = _atk;
    sprintf(m_strHead,"%s_g",pData->mSourceName);
    CCLOG("name:%s",m_strHead);
    m_pSprite = cocos2d::extension::CCArmature::create(m_strHead);
    m_pSprite->getAnimation()->play("daiji");
    m_timeCD.mCurTime = 0;
    m_timeCD.mTotalTime = (pData->mCD/1000)*2-0.2;
    if(m_timeCD.mTotalTime<=1)
        m_timeCD.mTotalTime = 1;
    schedule(schedule_selector(Lansquenet::skillUpdate), 0.1f,kCCRepeatForever,3);
    addChild(m_pSprite);
    char name_normal[30] = {0};
    sprintf(name_normal, "Lans%d_Skill_Normal.png",m_nType);
    char name_disable[30] = {0};
    sprintf(name_disable, "Lans%d_Skill_Disable.png",m_nType);
    m_mSkillMenu = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(name_normal),
                                            CCSprite::createWithSpriteFrameName(name_normal),
                                            CCSprite::createWithSpriteFrameName(name_disable),
                                            this, menu_selector(Lansquenet::skillCallBack));
    
    CCMenu *menu = CCMenu::createWithItem(m_mSkillMenu);
    addChild(menu);
    menu->setPosition(ccp(0, 50));
    m_mSkillMenu->setEnabled(false);
    
    
    m_pMark = CCProgressTimer::create(CCSprite::createWithSpriteFrameName(Lans_Skill_Disable_Mark));
    m_pMark->setPosition(menu->getPosition());
    m_pMark->setType(kCCProgressTimerTypeRadial);
    m_pMark->setReverseProgress(true);
    m_pMark->setPercentage(100);
    addChild(m_pMark,1);
    
    memset(name_normal, 0, 30);
    sprintf(name_normal, "glint%d_1.png",m_nType);
    m_pGlint = CCSprite::createWithSpriteFrameName(name_normal);
    m_pGlint->setPosition(menu->getPosition());
    addChild(m_pGlint,1);
    m_pGlint->setVisible(false);
}

Lansquenet::Lansquenet(const char *name , CCLayer *layer):
m_pSprite(NULL),
m_nType(0),
m_nHp(Barrier_HP),
m_nAtk(0),
m_nDef(10),
m_sprBarrier(NULL),
m_bUse(true),
m_mSkillMenu(NULL),
m_pMark(NULL),
m_pGlint(NULL)
{
    m_pParentLayer =(FightLayer*)layer;
    m_sprBarrier = CCSprite::createWithSpriteFrameName(name);
    m_sprBarrier->setAnchorPoint(ccp(0.5, 0));
    m_timeCD.mCurTime = 0;
    m_timeCD.mTotalTime = 0;
    addChild(m_sprBarrier);
}

void Lansquenet::onEnter()
{
    CCNode::onEnter();
}

void Lansquenet::onExit()
{
    removeAllChildrenWithCleanup(true);
    CCNode::onExit();
}
Lansquenet::~Lansquenet()
{
    m_pParentLayer = NULL;
}

bool Lansquenet::CreateFsmSys()
{
    m_pFSMSystem = new FSMSystem<Lansquenet>(this);
    if (m_pFSMSystem)
    {
        scheduleUpdate();
        return true;
    }
    
    return false;
}

void Lansquenet::update(float delta)
{
    if (m_pFSMSystem) {
        m_pFSMSystem->Update(delta);
    }
}

bool Lansquenet::checkCollision(Hero *bullet)
{
    CCLOG("retain count:%d",bullet->retainCount());
    if(bullet&&bullet->retainCount()==0)return false;
    CCPoint _point = convertToNodeSpace(bullet->getPosition());
    float width = bullet->getContentSize().width;
    float height = bullet->getContentSize().height;
    if (m_pSprite&&m_pSprite->boundingBox().intersectsRect(CCRectMake(_point.x, _point.y, width, height)))
    {
        return true;
    }
    
    if(m_sprBarrier&&m_sprBarrier->boundingBox().intersectsRect(CCRectMake(_point.x, _point.y, width, height)))
    {
        return true;
    }
    return  false;
}

CCNode* Lansquenet::getSprite()
{
    if(m_pSprite)
        return m_pSprite;
    if(m_sprBarrier)
        return m_sprBarrier;
    
    return NULL;
}

void Lansquenet::skillUpdate(float dt)
{
    if(m_pParentLayer->isShowTeachLayer())return;
    if(m_timeCD.mTotalTime == 0)
        return;
    if(!m_bUse)return;
    
    m_timeCD.mCurTime+=dt;
    float percent = m_timeCD.mCurTime / m_timeCD.mTotalTime;
    m_pMark->setPercentage(100 - percent*100);
    if(m_timeCD.mCurTime>=m_timeCD.mTotalTime)
    {
        m_timeCD.mCurTime = 0;
        m_bUse = false;
        m_pMark->setVisible(false);
        m_mSkillMenu->setEnabled(true);
        m_pGlint->setVisible(true);
        
        char buf[20] = {0};
        CCArray *aniFrames = CCArray::create();
        for (int i = 1; i<5; i++)
        {
            memset(buf, 0, 20);
            sprintf(buf, "glint%d_%d.png",m_nType,i);
            CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(buf);
            aniFrames->addObject(frame);
        }
        
        CCAnimation *animation = CCAnimation::createWithSpriteFrames(aniFrames,0.15);
        CCAnimate *animate = CCAnimate::create(animation);
        m_pGlint->runAction(CCRepeatForever::create(animate));
    }
}

void Lansquenet::skillCallBack(cocos2d::CCObject *pSender)
{
    m_bUse = true;
    m_mSkillMenu->setEnabled(false);
    m_pMark->setVisible(true);
    m_pGlint->setVisible(false);
    m_pGlint->stopAllActions();
    
    DATAPOOL->addEffectCache(ShiFangRes);
    CCArmature *arm = CCArmature::create("shifang");
    arm->getAnimation()->play("shifang");
    arm->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(Lansquenet::animationEvent));
    arm->setPosition(m_pGlint->getPosition());
    addChild(arm,1);
    
    m_pSprite->getAnimation()->play("gongji");
    m_pSprite->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(Lansquenet::animationEvent));
    
    switch (m_nType)
    {
        case Enemy_Type_aBoss:
        {
            m_pParentLayer->createEnemyBack(this);
        }
            break;
        case Enemy_Type_bBoss:
        {
            m_pParentLayer->createShipLans(this);
        }
            break;
        case Enemy_Type_cBoss:
        {
            m_pParentLayer->createTornadoLans(this);
        }
            break;
        case Enemy_Type_dBoss:
        {
            m_pParentLayer->createIceWallLans(this);
        }
            break;
        case Enemy_Type_eBoss:
        {
            m_pParentLayer->createHole(this);
        }
            break;
        case Enemy_Type_fBoss:
        {
            m_pParentLayer->createFireBall(this);
        }
            break;
        case Enemy_Type_gBoss:
        {
            m_pParentLayer->createFireFall(this);
        }
            break;
        default:
            break;
    }
}

void Lansquenet::animationEvent(cocos2d::extension::CCArmature *armature, cocos2d::extension::MovementEventType movementType, const char *movementID)
{
    string id = movementID;
    if(movementType == cocos2d::extension::COMPLETE)
    {
        if (id.compare("shifang")==0)
        {
            armature->removeFromParentAndCleanup(true);
            DATAPOOL->removeEffectCache(ShiFangRes);
        }
    }
    else if(movementType == cocos2d::extension::LOOP_COMPLETE)
    {
        if(id.compare("gongji")==0)
        {
            armature->getAnimation()->play("daiji");
        }
    }
}