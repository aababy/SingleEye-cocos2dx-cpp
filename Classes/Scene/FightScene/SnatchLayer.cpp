//
//  SnatchLayer.cpp
//  SingleEye
//
//  Created by qinfen on 14-1-10.
//
//

#include "SnatchLayer.h"
#define Cloud_Row (8)  //行
#define Cloud_Col (6)
#define ESCAPE_NUM (5)

//SnatchLayer::SnatchLayer()
//{
//    
//}

SnatchLayer::~SnatchLayer()
{
    
}

bool SnatchLayer::init()
{
    if(!CCLayer::init())
        return false;
    setTouchEnabled(true);
    
    m_pLevelData = NULL;
    m_pEnemy = CCArray::create();
    m_pEnemy->retain();
    m_pStreak = NULL;
    
    pauseNode = CCNode::create();
    addChild(pauseNode,1010);
    
    m_nCurrentWave = 0;
    m_nCloudNum = 0;
    m_nEscape = 0;
    m_bGameEnd = false;
    return true;
}

void SnatchLayer::onEnter()
{
    CCLayer::onEnter();
    CCLayerColor *layer = CCLayerColor::create(ccc4(0, 0, 0, 150));
    addChild(layer);
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();

    float  dx = size.width/Cloud_Col;
    float  dy = size.height/Cloud_Row;
    for ( int i = 0; i<Cloud_Col; ++i)
        for(int j = 0;j<Cloud_Row;++j)
        {
            float _orginal_x = 0;
            float _orginal_y = 0;
            if(i<Cloud_Col/2)  //左
            {
                _orginal_x = 0;
                _orginal_y = dy/2 + dy * j;
            }
            else
            {
                _orginal_x = size.width;
                _orginal_y = dy/2 + dy * j;
            }
            
            float _target_x = 0;
            float _target_y = 0;
            
            _target_x = dx/2 + dx * i;
            _target_y = dy/2 + dy * j;
            
            int index = arc4random()%10;
            if(index<4)
            {
                index = 1;
            }
            else if(index>=4&&index<8)
            {
                index = 2;
            }
            else   //减少3出现的概率
            {
                index = 3;
            }
            
            char buf[10] = {0};
            sprintf(buf, "yun_%d.png",index);
            CCSprite *cloud = CCSprite::createWithSpriteFrameName(buf);
            cloud->setPosition(ccp(_orginal_x, _orginal_y));
            
            int zorder = Snatch_Cloud_Zorder_1;
            zorder += arc4random()%5;
            
            addChild(cloud,zorder,CLOUD_TAG+m_nCloudNum);
            m_nCloudNum++;
            CCMoveTo *move = CCMoveTo::create(1, ccp(_target_x, _target_y));
            float delay = CCRANDOM_0_1();
            CCSequence *seq = CCSequence::create(CCDelayTime::create(delay),
                                                 move,
                                                 NULL);
            cloud->runAction(seq);
        }
    
    initUI();
    scheduleUpdate();
}

void SnatchLayer::initUI()
{
    DATAPOOL->addEffectCache(QiangBaoTuBiaoRes);
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    m_nBeganTime = 3;
    m_nTime = 60;
    
    CCSprite *_goldIcon = CCSprite::createWithSpriteFrameName(Icon_Gold_Small);
    float _dx = 2.0f;
    float _x =  _goldIcon->getContentSize().width/2;
    float _y =  150;
    _goldIcon->setPosition(ccp(_x, _y));
    addChild(_goldIcon,100,Snatch_Sprties_Gold);
    
    m_nGold = 0;
    m_nGem = 0;
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
    
    _dx = 90.0f;
    float _dy = 10;
    _x = _goldIcon->getPositionX() + _dx;
    _y = _goldIcon->getPositionY() + _dy;
    _labelGold->setPosition(ccp(_x, _y));
    
    
    addChild(_labelGold,100,Snatch_Sprties_Gold_Num);
    
    
    CCSprite *_sprite = CCSprite::createWithSpriteFrameName(Icon_Time);
    _sprite->setPosition(ccp(size.width/2, size.height - _sprite->getContentSize().height/2 - 2));
    addChild(_sprite,100,Snatch_Sprites_Time_Icon);
    
    memset(buf, 0, 20);
    sprintf(buf, "%02d:%02d",m_nTime/60, m_nTime%60);
    CCLabelBMFont *_labelTime = NULL;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    _labelTime = CCLabelBMFont::create(buf,"num_time.fnt");
#else
    _labelTime = CCLabelBMFont::create(buf,"fonts/num_time.fnt");
#endif
    _labelTime->setPosition(ccp(_sprite->getPositionX(), _sprite->getPositionY()-5));
    addChild(_labelTime,100,Snatch_Sprites_Time);
    
    CCSprite *qiangbao_txt = CCSprite::createWithSpriteFrameName(QiangBao_Txt);
    qiangbao_txt->setPosition(ccp(size.width/2,size.height/2));
    addChild(qiangbao_txt,100);
    
    CCParticleSystem *emitter = NULL;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    emitter = CCParticleSystemQuad::create("fireworks.plist");
#else
    emitter = CCParticleSystemQuad::create("particle/fireworks.plist");
#endif
    emitter->setPosition(qiangbao_txt->getPosition());
    emitter->setAutoRemoveOnFinish(true);
    addChild(emitter,100);
    
    CCSequence *seq2 = CCSequence::create(CCDelayTime::create(3),
                                          CCFadeOut::create(0.5),
                                          NULL);
    qiangbao_txt->runAction(seq2);
    
    CCSequence *seq3 = CCSequence::create(CCDelayTime::create(3.5),
                                          CCCallFuncN::create(this, callfuncN_selector(SnatchLayer::beganTimeOut)),
                                          NULL);
    this->runAction(seq3);
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    m_pStreak = CCMotionStreak::create(.25, 3, 64, ccRED, "fire.png");
#else
    m_pStreak = CCMotionStreak::create(.25, 3, 64, ccRED, "particle/fire.png");
#endif
    addChild(m_pStreak,101);
    m_pStreak->setVisible(false);
    
    CCMenuItemSprite *itemPause = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(Game_Btn_Pause),
                                                           CCSprite::createWithSpriteFrameName(Game_Btn_Pause_Clicked),
                                                           this, menu_selector(SnatchLayer::menuPauseCallBack));
    CCMenu *menuPause = CCMenu::createWithItem(itemPause);
    menuPause->setPosition(ccp(size.width - itemPause->getContentSize().width/2, itemPause->getContentSize().height/2));
    
    
    addChild(menuPause,100,123);

    CCLayerColor *layer = CCLayerColor::create(ccc4(0, 0, 0, 150));
    pauseNode->addChild(layer,1);
    
    CCSprite *bg = CCSprite::createWithSpriteFrameName(Game_Result_BG);
    bg->setPosition(ccp(size.width/2, size.height/2));
    pauseNode->addChild(bg,1);
    
    CCMenuItemSprite *_item_btn_continue = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(Game_Pause_Btn_Continue),
                                                                    CCSprite::createWithSpriteFrameName(Game_Pause_Btn_Continue_Click),
                                                                    this, menu_selector(SnatchLayer::callbackContinue));
    
    
    
    
    CCMenu *_menuButton = CCMenu::create(_item_btn_continue,NULL);
    _menuButton->setPosition(ccp(size.width/2, size.height/2));
    pauseNode->addChild(_menuButton,1);
    _menuButton->setTouchPriority(-130);
    pauseNode->setVisible(false);

}

void SnatchLayer::callbackContinue(cocos2d::CCObject *pSender)
{
    pauseNode->setVisible(false);
    HelpMethod::resumeNodeActions(this);
    CCMenuItemSprite *item = (CCMenuItemSprite*)(((CCMenu*)getChildByTag(123))->getChildren()->objectAtIndex(0));
    if(item)item->setEnabled(true);
}

void SnatchLayer::menuPauseCallBack(cocos2d::CCObject *pSender)
{
    HelpMethod::pauseNodeActions(this);
    pauseNode->setVisible(true);
    CCMenuItemSprite *item = (CCMenuItemSprite*)pSender;
    if(item)item->setEnabled(false);
}

void SnatchLayer::onExit()
{
    m_pEnemy->removeAllObjects();
    CC_SAFE_RELEASE_NULL(m_pEnemy);
    pauseNode->removeFromParentAndCleanup(true);
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
    m_nCloudNum = 0;
    m_nCurrentWave = 0;
    unscheduleAllSelectors();
    stopAllActions();
    removeAllChildrenWithCleanup(true);
    DATAPOOL->removeEffectCache(QiangBaoTuBiaoRes);
    CCDirector::sharedDirector()->purgeCachedData();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}

void SnatchLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);
}

bool SnatchLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if(pauseNode->isVisible())
        return true;
    if(m_bGameEnd)return true;
    CCPoint location = pTouch->getLocation();
    m_pStreak->reset();
    m_pStreak->setPosition(location);
    m_pStreak->setVisible(true);
    return true;
}

void SnatchLayer::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if(pauseNode->isVisible())
        return;
    if(m_bGameEnd)return;
    CCPoint location = pTouch->getLocation();
    m_pStreak->setPosition(location);
    for (int i=m_pEnemy->count()-1; i>=0; i--)
    {
        Enemy *enemy = (Enemy*)m_pEnemy->objectAtIndex(i);
        if(enemy&&enemy->checkCollision(location))
        {
            enemy->minusBlood(HongZhaJi_Atk,false);
            EnemyHurtState *_instance = EnemyHurtState::getInstance();
            enemy->GetFSMSystem()->ChangeState(_instance);
        }
    }
}

void SnatchLayer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if(pauseNode->isVisible())
        return;
    if(m_bGameEnd)return;
    m_pStreak->setVisible(false);
    CCPoint location = pTouch->getLocation();
    for (int i=m_pEnemy->count()-1; i>=0; i--)
    {
        Enemy *enemy = (Enemy*)m_pEnemy->objectAtIndex(i);
        if(enemy&&enemy->checkCollision(location))
        {
            enemy->minusBlood(HongZhaJi_Atk,false);
            EnemyHurtState *_instance = EnemyHurtState::getInstance();
            enemy->GetFSMSystem()->ChangeState(_instance);
        }
    }
}

void SnatchLayer::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if(pauseNode->isVisible())
        return;
    if(m_bGameEnd)return;
    m_pStreak->setVisible(false);
    CCPoint location = pTouch->getLocation();
    for (int i=m_pEnemy->count()-1; i>=0; i--)
    {
        Enemy *enemy = (Enemy*)m_pEnemy->objectAtIndex(i);
        if(enemy&&enemy->checkCollision(location))
        {
            enemy->minusBlood(HongZhaJi_Atk,false);
            EnemyHurtState *_instance = EnemyHurtState::getInstance();
            enemy->GetFSMSystem()->ChangeState(_instance);
        }
    }
}

void SnatchLayer::waveUpdate(float dt)
{
    if(!m_pLevelData)
        m_pLevelData = CfgDataManage::getInstance()->getLevelData(49);
    if (!m_pLevelData) {
        return;
    }
    
    m_nCurrentWave++;
    int _wave = m_nCurrentWave;
    if (m_nCurrentWave<=m_pLevelData->nWaveCount)  //当前的波数小于等于总波数
    {
        for (int i = 0; i<m_pLevelData->mArrayWave[_wave-1]->mElementCount; ++i)
        {
            int _type = First_Enemy_ID;
            Enemy_Data* enemyData = DATAPOOL->getEnemyData(_type);
            Enemy *enemy = NULL;
            
            enemy = new Enemy(enemyData,this);
            enemy->autorelease();
            enemyData = NULL;
            enemy->addPoints(m_pLevelData->mArrayWave[_wave-1]->mArrayElement[i]);
            m_pEnemy->addObject(enemy);
            addChild(enemy,20);
            EnemyMoveState *_instance = EnemyMoveState::getInstance();
            enemy->GetFSMSystem()->ChangeState(_instance);
        }
    }
    else
    {
        m_nCurrentWave = 0;
    }
}

void SnatchLayer::removeEnemyFromArray(CCObject *enemy)
{
    if (m_pEnemy)
    {
        m_pEnemy->removeObject(enemy);
    }
}

void SnatchLayer::beganTimeOut(cocos2d::CCNode *psender)
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    DATAPOOL->addEffectCache(DaoShuRes);
    CCArmature *arm = CCArmature::create("daoshu");
    arm->getAnimation()->play("daoshu");
    arm->setPosition(ccp(size.width/2,size.height/2));
    arm->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(SnatchLayer::animationEvent));
    addChild(arm,101);
}

void SnatchLayer::timeUpdate(float dt)
{
    m_nTime--;
    if(m_nTime>=1)
    {
        CCLabelBMFont *_labelTime = (CCLabelBMFont*)getChildByTag(Snatch_Sprites_Time);
        if(_labelTime)
        {
            char buf[6] = {0};
            sprintf(buf, "%02d:%02d",m_nTime/60, m_nTime%60);
            _labelTime->setString(buf);
        }
    }
    else
    {        
        cancelSnatch();
    }
}

void SnatchLayer::addAward(bool isLipin,CCPoint pos)
{
    if(isLipin)
    {
        if(arc4random()%1000<995)
        {
            DATAPOOL->addEffectCache(JinBiRes);
            CCSprite *gold = (CCSprite*)getChildByTag(Snatch_Sprties_Gold);
            if(gold)
            {
                CCArmature *jinbi = CCArmature::create("jinbi");
                jinbi->getAnimation()->play("jinbi");
                jinbi->setPosition(pos);
                addChild(jinbi,100);
                CCMoveTo *move = CCMoveTo::create(0.5, ccp(gold->getPositionX(), gold->getPositionY()));
                CCSequence *seq  = CCSequence::create(move,
                                                      CCCallFuncN::create(this, callfuncN_selector(SnatchLayer::removeAniCallBack)),
                                                      NULL);
                jinbi->runAction(seq);
                int full = 11;
                int num = arc4random()%100;
                int count = 0;
                if(num<70)
                {
                    count = full - (num/10);
                }
                else
                    count = 5;
                m_nGold += count;

            }
            updateGoldGemNum();
        }
    }
    else
    {
        DATAPOOL->addEffectCache(BaoZhaRes);
        DATAPOOL->playGameEffect(Item_Explode);
        CCArmature *arm = CCArmature::create("baozha");
        arm->getAnimation()->play("baozha");
        arm->setPosition(pos);
        addChild(arm,100);
        arm->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(SnatchLayer::animationEvent));
    
        cancelSnatch();
    }
}

void SnatchLayer::removeAniCallBack(cocos2d::CCNode *pSender)
{
    CCArmature *arm = (CCArmature*)pSender;
    if(arm)
    {
        this->removeChild(arm,true);
    }
}

void SnatchLayer::updateGoldGemNum()
{
    CCLabelBMFont *gold = (CCLabelBMFont*)getChildByTag(Snatch_Sprties_Gold_Num);
    if(gold)
    {
        char buf[20] = {0};
        sprintf(buf, "%07d",m_nGold);
        gold->setString(buf);
    }
    
    CCLabelBMFont *gem = (CCLabelBMFont*)getChildByTag(Snatch_Sprties_Gem_Num);
    if(gem)
    {
        char buf[20] = {0};
        sprintf(buf, "%07d",m_nGem);
        gem->setString(buf);
    }
}

void SnatchLayer::cancelSnatch()
{
    setTouchEnabled(false);
    m_bGameEnd = true;
    for (int i = m_pEnemy->count()-1; i>=0; i--)
    {
        Enemy *enemy =(Enemy*)m_pEnemy->objectAtIndex(i);
        if(enemy)
        {
            enemy->setVisible(false);
        }
    }
    FightLayer* layer = (FightLayer*)getParent();
    if(layer)
    {
        layer->changeGoldNum(m_nGold);
        layer->changeGoldNum(m_nGem);
    }
    CCSprite *goldIcon = (CCSprite*)getChildByTag(Snatch_Sprties_Gold);
    if(goldIcon)
    {
        goldIcon->removeFromParentAndCleanup(true);
    }
    
    CCSprite *gemIcon = (CCSprite*)getChildByTag(Snatch_Sprties_Gem);
    if(gemIcon)
    {
        gemIcon->removeFromParentAndCleanup(true);
    }
    
    CCLabelBMFont *gold = (CCLabelBMFont*)getChildByTag(Snatch_Sprties_Gold_Num);
    if(gold)
    {
        gold->removeFromParentAndCleanup(true);
    }
    
    CCLabelBMFont *gem = (CCLabelBMFont*)getChildByTag(Snatch_Sprties_Gem_Num);
    if(gem)
    {
        gem->removeFromParentAndCleanup(true);
    }
    
    CCLabelBMFont *_labelTime = (CCLabelBMFont*)getChildByTag(Snatch_Sprites_Time);
    if(_labelTime)
    {
        _labelTime->removeFromParentAndCleanup(true);
    }
    
    CCSprite *timeIcon = (CCSprite*)getChildByTag(Snatch_Sprites_Time_Icon);
    if(timeIcon)
    {
        timeIcon->removeFromParentAndCleanup(true);
    }
    
    unschedule(schedule_selector(SnatchLayer::timeUpdate));
    unschedule(schedule_selector(SnatchLayer::waveUpdate));
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    for (int i=CLOUD_TAG; i<CLOUD_TAG+m_nCloudNum; i++)
    {
        CCSprite *cloud = (CCSprite*)getChildByTag(i);
        if(cloud)
        {
            if(i<CLOUD_TAG+m_nCloudNum/2)
            {
                CCMoveBy *move = CCMoveBy::create(0.5, ccp(-size.width, 0));
                CCSequence *seq = CCSequence::create(CCDelayTime::create(0.5),
                                                     move,
                                                     NULL);
                cloud->runAction(seq);
            }
            else
            {
                CCMoveBy *move = CCMoveBy::create(0.5, ccp(size.width, 0));
                if(i!=CLOUD_TAG+m_nCloudNum-1)
                {
                    CCSequence *seq = CCSequence::create(CCDelayTime::create(0.5),
                                                         move,
                                                         NULL);
                    cloud->runAction(seq);
                }
                else
                {
                    CCSequence *seq = CCSequence::create(CCDelayTime::create(0.5),
                                                         move,
                                                         CCCallFuncN::create(this, callfuncN_selector(SnatchLayer::cancleSnatchCallBack)),
                                                         NULL);
                    
                    cloud->runAction(seq);
                }
            }
        }
    }
}

void SnatchLayer::cancleSnatchCallBack(cocos2d::CCNode *pSender)
{
    FightLayer* layer = (FightLayer*)getParent();
    if(layer)
    {
        this->removeFromParentAndCleanup(true);
        HelpMethod::resumeNodeActions(layer);
    }
}

void SnatchLayer::animationEvent(cocos2d::extension::CCArmature *armature, cocos2d::extension::MovementEventType movementType, const char *movementID)
{
    string id = movementID;
    if(movementType == cocos2d::extension::COMPLETE)
    {
        if (id.compare("baozha")==0)
        {
            armature->removeFromParentAndCleanup(true);
            DATAPOOL->removeEffectCache(BaoZhaRes);
        }
        else if(id.compare("daoshu")==0)
        {
            armature->removeFromParentAndCleanup(true);
            DATAPOOL->removeEffectCache(DaoShuRes);
            

            
            if(false == DATAPOOL->m_bTeachStepOver[TEACH_STEP_QIANGBAO_SLIDE])   //提示抢宝规则
            {
                DATAPOOL->m_bTeachStepOver[TEACH_STEP_QIANGBAO_SLIDE] = true;
                CCSize size = CCDirector::sharedDirector()->getWinSize();
                for (int i = 0; i<3; i++)
                {
                    char buf[30] = {0};
                    sprintf(buf, "qiangbao_tips%d.png",i+1);
                    CCSprite *tips = CCSprite::createWithSpriteFrameName(buf);
                    tips->setPosition(ccp(size.width/2, size.height/2));
                    addChild(tips,101);
                    tips->setOpacity(0);
                    CCSequence *seq = CCSequence::create(CCDelayTime::create(2.3*i),
                                                         CCFadeIn::create(0.2),
                                                         CCDelayTime::create(2),
                                                         CCFadeOut::create(0.1),
                                                         NULL);
                    
                    CCSequence *seq2 = CCSequence::create(CCDelayTime::create(2.3*i),
                                                          CCFadeIn::create(0.2),
                                                          CCDelayTime::create(2),
                                                          CCFadeOut::create(0.1),
                                                          CCCallFunc::create(this, callfunc_selector(SnatchLayer::tipsCallBack)),
                                                          NULL);
                    if(i==2)
                    {
                        tips->runAction(seq2);
                    }
                    else
                    {
                        tips->runAction(seq);
                    }
                }
            }
            else
            {
                tipsCallBack();
            }
        }
    }
}

void SnatchLayer::tipsCallBack()
{
    waveUpdate(0);
    schedule(schedule_selector(SnatchLayer::waveUpdate), 3);
    schedule(schedule_selector(SnatchLayer::timeUpdate), 1);
}

void SnatchLayer::checkEnemyZorder()
{
    if(m_pEnemy->count()>1)
    {
        CCArray *_array = getChildren();
        for (int i=0; i<_array->count(); ++i)
            for(int j = i+1;j<_array->count();++j)
            {
                int _ZOrder1 = ((CCNode*)_array->objectAtIndex(i))->getZOrder();
                int _ZOrder2 = ((CCNode*)_array->objectAtIndex(j))->getZOrder();
                if (_ZOrder1 == 20&&_ZOrder2 == 20)
                {
                    Enemy *_enemy1 = (Enemy*)_array->objectAtIndex(i);
                    Enemy *_enemy2 = (Enemy*)_array->objectAtIndex(j);
                    if (_enemy1->getPositionY()<_enemy2->getPositionY())
                    {
                        _array->exchangeObjectAtIndex(i, j);
                    }
                }
            }
    }
}

void SnatchLayer::update(float dt)
{
    checkEnemyZorder();
}

void SnatchLayer::addEscape()
{
    m_nEscape++;
    if(m_nEscape>ESCAPE_NUM)
    {
        cancelSnatch();
    }
}
