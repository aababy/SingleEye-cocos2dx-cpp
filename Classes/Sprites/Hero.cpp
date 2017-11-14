//
//  Hero.cpp
//  SingleEye
//
//  Created by qinfen on 13-11-12.
//
//

#include "Hero.h"
#include "HelpMethod.h"
enum Hero_Zorder
{
    Hero_Zorder_Burn = 0,
    Hero_Zorder_Hero,
    Hero_Zorder_Mask,
    Hero_Zorder_light,
    Hero_Zorder_Top,
};

enum Hero_Sprite
{
    Hero_Sprite_light = 100,
    Hero_Sprite_Burn,
    Hero_Sprite_Shiliu,
    Hero_Sprite_Effect,
};


Hero::Hero(Game_Hero_Data *pData):m_nState(0)
{
    m_pData = pData;
    sprintf(m_strHead,"%s",pData->mSourceName);
    CCLOG("%s",m_strHead);
    m_nType = pData->mType;
    m_bAtkSelf = false;
    int _atk = pData->mAtk2 + (pData->mAtkAdd)*(DATAPOOL->getHeroRecord(m_nType)->m_nLevel);
    int _skillAtk = _atk + pData->mSkillAtk + (pData->mSkillAtkAdd) * (DATAPOOL->getHeroRecord(m_nType)->m_nLevel);
    m_nAtk = _atk;
    m_nSkillAtk = _skillAtk;
    m_nDuration = pData->mDuration + (DATAPOOL->getHeroRecord(m_nType)->m_nLevel)*0.5;
    CCLOG("Atk:%d",m_nAtk);
    CCLOG("SkillAtk:%d",m_nSkillAtk);
    m_coolDown.mCurTime = 0;
    m_coolDown.mTotalTime = pData->mCoolDown/1000.0;
    m_nBuyGold = pData->mBuyGold;
    m_pSprite = cocos2d::extension::CCArmature::create(m_strHead);
    m_pSprite->getAnimation()->play("daiji");
    char buf[40];
    memset(buf, 0, 40);
    sprintf(buf, "Sprites/%s.png",m_strHead);
    m_pMask =  CCProgressTimer::create(CCSprite::create(buf));
    m_pMask->setType(kCCProgressTimerTypeBar);
    m_pMask->setMidpoint(ccp(0,0));
    m_pMask->setBarChangeRate(ccp(0,1));
    m_pMask->setAnchorPoint(ccp(0.5,0));
    addChild(m_pMask,Hero_Zorder_Mask);
    m_pMask->setVisible(false);
   
    addChild(m_pSprite,Hero_Zorder_Hero);
    m_bHitTarget = false;
    m_bAtkEnemy = true;
    m_bRemove = true;
    m_bHit = false;
    if(m_nType == Hero_Type_TaoZi||m_nType == Hero_Type_Lizi)
        m_bRemove = false;
    m_nReference = DATAPOOL->getHeroReference();
    m_bRemoveTime = false;
}

Hero::Hero()
{
    m_pSprite = extension::CCArmature::create();
    m_pData = NULL;
    m_bAtkSelf = false;
    addChild(m_pSprite);
     m_bAtkEnemy = true;
    m_bRemove = true;
    m_bHit = false;
    m_fAccelerate_Y = 0;
    m_fAccelerate_X = 0;
    if(m_nType == Hero_Type_TaoZi||m_nType == Hero_Type_Lizi)
        m_bRemove = false;
    m_nReference = DATAPOOL->getHeroReference();
    m_bRemoveTime = false;
}

Hero::Hero(CCArmature *arm,int atk)
{
    m_pMask = NULL;
    m_nState = Hero_State_Move;
    m_nType = Hero_Type_LianNu;
    m_coolDown.mCurTime = 0;
    m_coolDown.mTotalTime = 0;
    m_nSkillAtk = 0;
    m_nBuyGold = 0;
    m_fAccelerate_X = 0;
    m_fAccelerate_Y = 0;
    m_pSprite = arm;
    m_nAtk = atk;
    m_bAtkSelf = false;
    m_bHitTarget = false;
    addChild(m_pSprite);
     m_bAtkEnemy = true;
    m_bRemove = true;
    m_bHit = false;
    if(m_nType == Hero_Type_TaoZi||m_nType == Hero_Type_Lizi)
        m_bRemove = false;
    m_nReference = DATAPOOL->getHeroReference();
    m_bRemoveTime = false;
}

void Hero::onEnter()
{
    CCNode::onEnter();
}

void Hero::onExit()
{
    removeAllChildrenWithCleanup(true);
    CCNode::onExit();
}

Hero::~Hero()
{
    m_pData = NULL;
//    m_pSprite->removeFromParentAndCleanup(true);
}

void Hero::setState(int nState)
{
    m_nState = nState;
    switch (nState)
    {
        case Hero_State_Normal:
            playNormalAction();
            break;
        case Hero_State_Ready:
            playReadyAction();
            break;
        case Hero_State_CoolDown:
            playCoolDownAction();
            break;
        case Hero_State_Move:
            playMoveAction();
            break;
        case Hero_State_PowerMove:
            playPowerMoveAction();
            break;
        default:
            break;
    }
}

void Hero::setPowerState()
{
    if (m_nState==Hero_State_Power)
    {
        return;
    }
    m_nState = Hero_State_Power;
    switch (m_nType)
    {
        case Hero_Type_ShiLiu:
        {
//            scheduleOnce(schedule_selector(Hero::pitayaPowerStateUpdate), getDuration());
//            {
//                CCArmature *xuli = (CCArmature*)(m_pParent->getChildByTag(Hero_Sprite_light));
//                if(xuli)
//                {
//                    xuli->removeFromParentAndCleanup(true);
//                    CCArmature *burn = (CCArmature*)getChildByTag(Hero_Sprite_Burn);
//                    burn->removeFromParentAndCleanup(true);
//                }
//                DATAPOOL->addEffectCache(ShiLiuBRes);
//                CCArmature *shiliu = CCArmature::create("shiliu_b");
//                shiliu->setPosition(m_pSprite->getPosition());
//                shiliu->getAnimation()->play("shiliu_b");
//                addChild(shiliu,Hero_Zorder_light,Hero_Sprite_Shiliu);
//            }
        }
//            break;
        case Hero_Type_HuoLongGuo:
        {
//            scheduleOnce(schedule_selector(Hero::pitayaPowerStateUpdate), getDuration());
        }
        default:
        {
            {
                CCArmature *xuli = (CCArmature*)getChildByTag(Hero_Sprite_light);
                if(xuli)
                {
                    xuli->removeFromParentAndCleanup(true);
                }
                CCArmature *burn = (CCArmature*)getChildByTag(Hero_Sprite_Burn);
                if(burn)
                {
                    burn->getAnimation()->play("ranshao_b");
                    burn->getAnimation()->setMovementEventCallFunc(burn, movementEvent_selector(Hero::animationEvent));
                }
                else
                {
                    burn = CCArmature::create("ranshao");
                    burn->setPosition(m_pSprite->getPosition());
                    burn->getAnimation()->play("ranshao_b");
                    burn->getAnimation()->setMovementEventCallFunc(burn, movementEvent_selector(Hero::animationEvent));
                    addChild(burn,Fight_Zorder_Hero-1,Hero_Sprite_Burn);
                }
            }
        }
            break;
    }
}

void Hero::setRemoveTimeUpdate()
{
    if(m_bRemoveTime)return;
    m_bRemoveTime = true;
    scheduleOnce(schedule_selector(Hero::pitayaPowerStateUpdate), getDuration());
}

void Hero::setXuli(CCObject *psender)
{
    if (m_nState==Hero_State_Xuli||m_nState == Hero_State_Power)
    {
        return;
    }
    FightLayer *_layer = (FightLayer*)psender;
    _layer->setXuliSoundID(DATAPOOL->playGameEffect(Xuli));
    m_nState = Hero_State_Xuli;
    
    CCArmature *xuli = (CCArmature*)getChildByTag(Hero_Sprite_light);
    if(!xuli)
    {
        xuli = CCArmature::create("ranshao");
        xuli->setPosition(m_pSprite->getPosition());
        xuli->getAnimation()->play("xuli");
        addChild(xuli,Hero_Zorder_light,Hero_Sprite_light);
        CCArmature *burn = CCArmature::create("ranshao");
        burn->setPosition(m_pSprite->getPosition());
        burn->getAnimation()->play("ranshao_a");
        addChild(burn,Fight_Zorder_Hero-1,Hero_Sprite_Burn);
    }
}

void Hero::cancelPowerState()
{
    CCArmature *xuli = (CCArmature*)getChildByTag(Hero_Sprite_light);
    if(!xuli)
    {
        xuli->removeFromParentAndCleanup(true);
        CCArmature *burn = (CCArmature*)getChildByTag(Hero_Sprite_Burn);
        if(burn)
            burn->removeFromParentAndCleanup(true);
    }
}

void Hero::cancelXuli()
{
    if (m_nState==Hero_State_Ready)
    {
        return;
    }
    m_nState = Hero_State_Ready;
    
    CCArmature *xuli = (CCArmature*)getChildByTag(Hero_Sprite_light);
    if(xuli)
        xuli->removeFromParentAndCleanup(true);
    
    CCArmature *burn = (CCArmature*)getChildByTag(Hero_Sprite_Burn);
    if(burn)
        burn->removeFromParentAndCleanup(true);
//    CCParticleSystem *xuli = (CCParticleSystemQuad*)getChildByTag(Hero_Sprite_light);
//    if(xuli)
//        xuli->removeFromParentAndCleanup(true);
//    
//    CCParticleSystem *burn = (CCParticleSystemQuad*)getChildByTag(Hero_Sprite_Burn);
//    if(burn)
//        burn->removeFromParentAndCleanup(true);
}
void Hero::playNormalAction()
{
    char buf[20];
    memset(buf, 0, 20);
    sprintf(buf, "daiji");
    m_pSprite->getAnimation()->play(buf);
}

void Hero::playReadyAction()
{
    char buf[20];
    memset(buf, 0, 20);
    sprintf(buf, "jiazi");
    m_pSprite->getAnimation()->play(buf);
}

void Hero::playMoveAction()
{
    char buf[20];
    memset(buf, 0, 20);
    if(m_pMask)
    {
        sprintf(buf, "gongji");      //英雄滚动攻击动画
    }
    else
    {
        sprintf(buf, "baojihuoqiu");       //道具 连弩子弹攻击动画
    }
    m_pSprite->getAnimation()->play(buf);
}

void Hero::playCoolDownAction()
{
    m_pMask->setPercentage(100);
    m_pSprite->getAnimation()->playByIndex(0);
    m_pSprite->getAnimation()->stop();
    m_pMask->setVisible(true);
    schedule(schedule_selector(Hero::coolDownUpdate), 0.1);
}

void Hero::playPowerMoveAction()
{
    if(m_pMask)
    {
        if(m_nType==Hero_Type_HuoLongGuo || m_nType ==  Hero_Type_ShiLiu)
        {
            m_pSprite->setVisible(false);
            
        }
        else
        {
           m_pSprite->getAnimation()->play("gongji");      //英雄滚动攻击动画
        }
    }
    else
    {
        m_pSprite->getAnimation()->play("ranshao_a");    //道具 连弩子弹攻击动画
    }
    CCLOG("type:%d",m_nType);
    switch (m_nType)
    {
        case Hero_Type_ShiLiu:
        {
            DATAPOOL->addEffectCache(ShiLiuARes);
            CCArmature *shiliu_a = CCArmature::create("shiliu_a");
            shiliu_a->setPosition(m_pSprite->getPosition());
            shiliu_a->getAnimation()->play("shiliu_a");
            addChild(shiliu_a,Hero_Zorder_Top,Hero_Sprite_Effect);
        }
            break;
        case Hero_Type_HuoLongGuo:
        {
            DATAPOOL->addEffectCache(HuoQiuRes);
            CCArmature *shiliu_a = CCArmature::create("huoqiu");
            shiliu_a->setPosition(m_pSprite->getPosition());
            shiliu_a->getAnimation()->play("huoqiu");
            addChild(shiliu_a,Hero_Zorder_Top,Hero_Sprite_Effect);
        }
            break;
        case Hero_Type_TaoZi:
        {
            DATAPOOL->addEffectCache(XuanFengRes);
            CCArmature *shiliu_a = CCArmature::create("xuanfeng");
            shiliu_a->setPosition(m_pSprite->getPosition());
            shiliu_a->getAnimation()->play("xuanfeng");
            addChild(shiliu_a,Hero_Zorder_Top,Hero_Sprite_Effect);
        }
            break;
        case Hero_Type_Lizi:
        {
            m_pSprite->setScale(1.5f);
            CCParticleSystem *m_emitter;
            m_emitter = CCParticleSmoke::create();
            m_emitter->retain();
            addChild(m_emitter);
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
            m_emitter->setTexture(CCTextureCache::sharedTextureCache()->addImage("fire.png"));
#else
            m_emitter->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle/fire.png"));
#endif
            m_emitter->setPosition(m_pSprite->getPosition());
            m_emitter->setAutoRemoveOnFinish(true);
        }
            break;
        default:
        {
            DATAPOOL->addEffectCache(BaoJiHuoQiuRes);
            CCArmature *shiliu_a = CCArmature::create("baojihuoqiu");
            shiliu_a->setPosition(m_pSprite->getPosition());
            shiliu_a->getAnimation()->play("baojihuoqiu");
            addChild(shiliu_a,Hero_Zorder_Top,Hero_Sprite_Effect);
        }
            break;
    }
    
}

Game_Hero_Data* Hero::getData()
{
//    Game_Hero_Data *_data = new Game_Hero_Data;
//    _data->mType = m_nType;
//    _data->mAtk = m_nAtk;
//    _data->mSkillAtk = m_nSkillAtk;
//    _data->mBuyGold = m_nBuyGold;
//    _data->mCoolDown = m_coolDown.mTotalTime;
//    sprintf(_data->mSourceName, "%s",m_strHead);
//    return _data;
    return m_pData;
}

void Hero::setAngle(float pAngle)
{
    m_pSprite->setRotation(pAngle);
    if (m_nState == Hero_State_PowerMove)
    {
        CCArmature *effect = (CCArmature*)getChildByTag(Hero_Sprite_Effect);
        if(effect)
        {
            effect->setRotation(pAngle);
        }
    }
    m_fAccelerate_Y = HelpMethod::getAngleAccelerateY(pAngle, Accelerate);
    m_fAccelerate_X = HelpMethod::getAngleAccelerateX(pAngle, Accelerate);
}

bool Hero::moveUpdate(float dt)
{
    float _posX = this->getPositionX();
    float _posY = this->getPositionY();
    
    _posX += m_fAccelerate_X;
    _posY += m_fAccelerate_Y;
    CCSize _size = CCDirector::sharedDirector()->getWinSize();
    if(m_nType==Hero_Type_TaoZi)
    {
        if(_posX > _size.width + this->getContentSize().width/2||_posX < -(this->getContentSize().width/2))
        {
            return false;
        }
    }
    else
    {
        if (_posX < this->getContentSize().width/2)
        {
            _posX = this->getContentSize().width/2;
            float _angle = m_pSprite->getRotation();
            _angle *= -1;
            setAngle(_angle);
        }
        else if (_posX > (_size.width - this->getContentSize().width/2))
        {
            _posX = _size.width - this->getContentSize().width/2;
            float _angle = m_pSprite->getRotation();
            _angle *= -1;
            setAngle(_angle);
        }
    }
    
    if (_posY > _size.height + this->getContentSize().height/2)
    {
        return false;
    }
    else if(_posY < Barrier_Y)
    {
        return false;
    }
    this->setPosition(ccp(_posX, _posY));
    if(m_nType == Hero_Type_BoLuo && m_nState == Hero_State_PowerMove&&_posY>=(Barrier_Y+Atk_Barrier_DY))
    {
        getParent()->schedule(schedule_selector(FightLayer::explosionUpdate), 0.1);
        getParent()->runAction(CCShake::create(0.3, 20));
        ((FightLayer*)getParent())->removeBullet(this);
    }
    return true;
}

void Hero::coolDown()
{
    if(m_nState == Hero_State_CoolDown)
        return;
    setState(Hero_State_CoolDown);
}

void Hero::cancelCoolDown()
{
    m_pMask->setVisible(false);
    m_coolDown.mCurTime = 0.0;
    unschedule(schedule_selector(Hero::coolDownUpdate));
    setState(Hero_State_Normal);
}

void Hero::coolDownUpdate(float dt)
{
    m_coolDown.mCurTime+=dt;
    if (m_coolDown.mCurTime>=m_coolDown.mTotalTime)
    {
        m_pMask->setVisible(false);
        m_coolDown.mCurTime = 0.0;
        unschedule(schedule_selector(Hero::coolDownUpdate));
        setState(Hero_State_Normal);
    }
    else
    {
        float percent = m_coolDown.mCurTime/m_coolDown.mTotalTime;
        m_pMask->setPercentage((100-(percent*100)));
    }
}

bool Hero::isCoolDown()
{
    if (m_nState == Hero_State_CoolDown)
    {
        return true;
    }
    
    return false;
}

bool Hero::isPowerState()
{
    if (m_nState == Hero_State_Power || m_nState == Hero_State_PowerMove)
    {
        return true;
    }
    return false;
}

void Hero::animationEvent(cocos2d::extension::CCArmature *armature, cocos2d::extension::MovementEventType movementType, const char *movementID)
{
    string id = movementID;
    if(movementType == cocos2d::extension::COMPLETE)
    {
        if (id.compare("ranshao_b")==0)
        {
            armature->getAnimation()->play("ranshao_c");
        }
    }
    
}

void Hero::pitayaPowerStateUpdate(float dt)
{
    FightLayer *parent = (FightLayer*)getParent();
    parent->cancelKuangBao();
    parent->removeBullet(this);
    if(parent->getCatapultPower()<=0.0)
    {
        parent->resetTrayPos();
    }
    CCArmature *shiliu_b = (CCArmature*)getChildByTag(Hero_Sprite_Shiliu);
    if(shiliu_b)
    {
        shiliu_b->removeFromParentAndCleanup(true);
        DATAPOOL->removeEffectCache(ShiLiuBRes);
    }
    if(m_nType == Hero_Type_ShiLiu&&parent->getPartner())parent->setPartner(false);
    this->removeFromParentAndCleanup(true);
}

void Hero::reverseY()
{
    m_pSprite->setRotation(180);
    CCArmature *effect = (CCArmature*)getChildByTag(Hero_Sprite_Effect);
    if(effect)
    {
        effect->setRotation(180);
    }
    m_fAccelerate_Y*=-1;
}

bool Hero::isReverseY()
{
    if(m_fAccelerate_Y<0)
        return true;
    else
        return false;
}

int Hero::getAddAtk()
{
    return 1 + m_pData->mSkillAtkAdd * DATAPOOL->getHeroRecord(m_nType)->m_nLevel;  //0级为1点伤害
}

void Hero::setHit()
{
    m_bHit = true;
}

bool Hero::getHit()
{
    return m_bHit;
}

bool Hero::isLianNuBullet()
{
    bool res = false;
    if(m_nType == Hero_Type_LianNu)
        res = true;
    return res;
}