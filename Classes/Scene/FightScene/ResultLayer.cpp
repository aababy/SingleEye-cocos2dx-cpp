//
//  ResultLayer.cpp
//  SingleEye
//
//  Created by qinfen on 13-11-27.
//
//

#include "ResultLayer.h"
USING_NS_CC_EXT;
#pragma mark -ResultLayer
ResultLayer::~ResultLayer()
{
    
}

bool ResultLayer::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    setTouchEnabled(false);
    m_bIsShanke = false;
    m_bShankeSuccess = false;
    m_bUseShake = false;
    
    m_titleResult = NULL;
    m_txtHit = NULL;
    m_txtDamage = NULL;
    m_labelHit = NULL;
    m_labelDamage = NULL;
    m_iconAward = NULL;
    m_labelGold = NULL;
    m_iconBox = NULL;
    m_iconShare = NULL;
    m_bgResult = NULL;
    m_btnLeft = NULL;
    m_btnRight = NULL;
    
    m_bgResultSmall = NULL;
    m_txtJiangli = NULL;
    menuBox = NULL;
    m_bShakeTeach = true;
    m_bStartingShakeTeach = false;
    
    resultNode = CCNode::create();
    shakeNode = CCNode::create();
    unlockNode = CCNode::create();
    rookieNode = CCNode::create();
    addChild(resultNode);
    addChild(shakeNode);
    addChild(unlockNode);
    addChild(rookieNode);

    return true;
}

void ResultLayer::onEnter()
{
    CCLayer::onEnter();
    MsgSystem::Instance()->RegisterObjAndHandler(this, MSGSYS_HANLE_MSG(ResultLayer::handleMsg));
    m_bIsShanke = false;
    m_bShankeSuccess = false;
    m_bUseShake = false;
    m_bShowLockNode = false;
}

void ResultLayer::onExit()
{
    xNetEngine->setNetMessageEnabled(false);
    resultNode->removeAllChildren();
    shakeNode->removeAllChildren();
    unlockNode->removeAllChildren();
    rookieNode->removeAllChildren();
    removeAllChildrenWithCleanup(true);
    m_bShowLockNode = false;
    MsgSystem::Instance()->ClearObjHandler(this);
    CCLayer::onExit();
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
//    CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
}

void ResultLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);
}

bool ResultLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if(m_bIsShanke)
        quitCallBack(NULL);
        
    return true;
}

bool ResultLayer::handleMsg(const Message &msg)
{
    CCLOG("NetTest::HandleMsg msgType= %d", msg.m_nMsgType);
    switch (msg.m_nMsgType)
    {
        case CMD_REQ_SHAKE:
        {
            
        }
            break;
        case CMD_RES_SHAKE:
        {
            xLoading->stop();
            CCSprite *txt = (CCSprite*)shakeNode->getChildByTag(123);
            txt->setVisible(false);
            getResData();
        }
            break;
        case CMD_REQ_AF_UPDATE_USERINFO:
        {
            
        }
            break;
        case CMD_RES_AF_UPDATE_USERINFO:
        {
            getResDataUpdate();
        }
            break;
        case CMD_REQ_BOSS_FIGHT:
        {
            
        }
            break;
        case CMD_RES_BOSS_FIGHT:
        {
            
        }
            break;
        case CMD_RES_BOSS_UPDATE_FIGHT:
        {
            
        }
            break;
        case NET_ERROR:
        {
            m_bShankeSuccess = false;
            xLoading->stop();
        }
            break;
        case CMD_RES_APP_SENDING_PODIUM:
        {            
            xUSString("share_text", xUser->message.c_str());            
            //开始分享
            xShare->snsShare(DATAPOOL->getDefaultShareTxt().c_str(), Btn_Continue);
        }
            break;             
        default:
            break;
    }
    return true;
}

void ResultLayer::getResData()
{
    INT32 _goldCoin = UserData::Instance()->shake_goldCoin;
    INT32 _gemStone = UserData::Instance()->shake_gemstone;
    INT32 _returnType = (INT32)UserData::Instance()->shake_returnType;
    INT32 _returnID = UserData::Instance()->shake_reId;
    INT32 _returnGrade = UserData::Instance()->shake_reGrade;
    
    
    if(menuBox)
    {
        menuBox->setNormalImage(CCSprite::createWithSpriteFrameName(Box_Open));
        menuBox->setSelectedImage(CCSprite::createWithSpriteFrameName(Box_Open));
    }
    
    
    m_bShankeSuccess = true;
    char buf[5] = {0};
    if(0==_goldCoin&&0==_gemStone&&0==_returnType&&0==_returnID&&0==_returnGrade)  //无奖品
    {
        DATAPOOL->playGameEffect(Shake_Not_Award);
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        CCSprite *noreward = CCSprite::createWithSpriteFrameName(No_Reward);
        noreward->setPosition(ccp(size.width/2, size.height/2));
        noreward->setScale(0.2f);
        CCSpawn *spa = CCSpawn::create(CCScaleTo::create(0.3, 1),
                                       CCMoveBy::create(0.3, ccp(0, 100)),
                                       NULL);
        noreward->runAction(spa);
        shakeNode->addChild(noreward,2);
        CCLOG("不奖品!");
    }
    else
    {
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        int deltaY = 70;
        int index = 0;
        if(_goldCoin!=0)
        {
            index++;
            DATAPOOL->changeBagGoldNumForGame(_goldCoin);
            CCSprite *jinbi = CCSprite::createWithSpriteFrameName("jinbi.png");
            jinbi->setPosition(ccp(size.width/2, size.height/2));
            shakeNode->addChild(jinbi,2);
            
            CCMoveTo *move = CCMoveTo::create(0.5, ccp(size.width/2-70, size.height-200-deltaY*index));
            jinbi->runAction(move);
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
            CCLabelBMFont *jinbi_num = CCLabelBMFont::create(buf, "num_yellow.fnt");
#else
            CCLabelBMFont *jinbi_num = CCLabelBMFont::create(buf, "fonts/num_yellow.fnt");
#endif
            
            jinbi_num->setPosition(ccp(size.width/2+30, size.height-200-deltaY*index));
            sprintf(buf, "X%d",_goldCoin);
            jinbi_num->setString(buf);
            jinbi_num->setOpacity(0);
            shakeNode->addChild(jinbi_num,2);
            CCSequence *seq = CCSequence::create(CCDelayTime::create(0.5),
                                                 CCFadeIn::create(0.1),
                                                 NULL);
            jinbi_num->runAction(seq);
            CCLOG("奖励金币:%d",_goldCoin);
        }
        
        if(_gemStone!=0)
        {
            index++;
            CCSprite *baoshi = CCSprite::createWithSpriteFrameName("baoshi.png");
            baoshi->setPosition(ccp(size.width/2, size.height/2));
            shakeNode->addChild(baoshi,2);
            
            CCMoveTo *move = CCMoveTo::create(0.5, ccp(size.width/2-70, size.height-200-deltaY*index));
            baoshi->runAction(move);
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
            CCLabelBMFont *baoshi_num = CCLabelBMFont::create(buf, "num_yellow.fnt");
#else
            CCLabelBMFont *baoshi_num = CCLabelBMFont::create(buf, "fonts/num_yellow.fnt");
#endif
            
            baoshi_num->setPosition(ccp(size.width/2+30, size.height-200-deltaY*index));
            memset(buf, 0, 5);
            sprintf(buf, "X%d",_gemStone);
            baoshi_num->setString(buf);
            baoshi_num->setOpacity(0);
            shakeNode->addChild(baoshi_num,2);
            
            CCSequence *seq = CCSequence::create(CCDelayTime::create(0.5),
                                                 CCFadeIn::create(0.1),
                                                 NULL);
            baoshi_num->runAction(seq);
            CCLOG("奖励宝石:%d",_gemStone);
            xUser->pUserInfo->gemstone += _gemStone;
        }
        
        CCLOG("_returnType:%d",_returnType);
        if(_returnType == 3) //道具
        {
            index++;
            CCSprite *daoju = CCSprite::createWithSpriteFrameName(DATAPOOL->getPropData(_returnID)->m_sourceName);
            if(daoju)
            {
                daoju->setPosition(ccp(size.width/2, size.height/2));
                shakeNode->addChild(daoju,2);
                CCMoveTo *move = CCMoveTo::create(0.5, ccp(size.width/2-70, size.height-200-deltaY*index));
                daoju->runAction(move);
            }
            OutfitRecord *rcd = DATAPOOL->getOutfitRecord(_returnID);
            rcd->m_nNum+=1;
            DATAPOOL->setOutfitRecord(_returnID, *rcd);
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
            CCLabelBMFont *daoju_num = CCLabelBMFont::create(buf, "num_yellow.fnt");
#else
            CCLabelBMFont *daoju_num = CCLabelBMFont::create(buf, "fonts/num_yellow.fnt");
#endif
            
            daoju_num->setPosition(ccp(size.width/2+30, size.height-200-deltaY*index));
            daoju_num->setOpacity(0);
            daoju_num->setString("X1");
            shakeNode->addChild(daoju_num,2);
            
            CCSequence *seq = CCSequence::create(CCDelayTime::create(0.5),
                                                 CCFadeIn::create(0.1),
                                                 NULL);
            daoju_num->runAction(seq);
            CCLOG("奖励道具 ID:%d",_returnID);
        }
    }
///    DATAPOOL->gem = UserData::Instance()->pUserInfo->gemstone;
    DATAPOOL->playGameEffect(Shake_Award);
}

void ResultLayer::getResDataUpdate()
{
    if(UserData::Instance()->isSuccess)
    {
        INT32 _totalScore = 0;
        if(m_nFightModel!=2)
        {
           _totalScore = UserData::Instance()->totalScore;
            xData->m_iTotalScore = _totalScore;
        }
        INT32 _serGem = UserData::Instance()->pUserInfo->gemstone;
 ///       DATAPOOL->gem = _serGem;
        CCLOG("_totalScore:%d _serGem:%d",_totalScore,_serGem);
        DATAPOOL->saveArchive();
    }
    else
    {
        CCLOG("%s",UserData::Instance()->message.c_str());
    }
}

void ResultLayer::showWin(int star, int hit, int damage, int gold,int level,int fightModel,int gemstone,int gemstoneUse,int hurt)
{
    CCLOG("YOU WIN!");
    m_nFightModel = (INT16)fightModel;
    float tmp = ((float)damage/100.0 * 0.9 + (float)hit/100.0)*100;
    int score = (int)tmp;
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
    DATAPOOL->playGameEffect(GameWin);
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    CCLayerColor *layer = CCLayerColor::create(ccc4(0, 0, 0, 150));
    addChild(layer,-2,ColorLayerTag);
    
    m_bgResult = CCSprite::createWithSpriteFrameName(Game_Result_BG);
    resultNode->addChild(m_bgResult,-1);
    m_bgResult->setPosition(ccp(size.width/2, size.height/2));
    m_bgResult->setOpacity(0);
    
    m_bgResultSmall = CCSprite::createWithSpriteFrameName(Game_Result_BG_Small);
    resultNode->addChild(m_bgResultSmall);
    m_bgResultSmall->setPosition(ccp(size.width/2, size.height/2 - BG_SMALL));
    m_bgResultSmall->setOpacity(0);
    
    char buf[20] = {0};

    m_titleResult = CCSprite::createWithSpriteFrameName(Txt_YouWin);
    m_titleResult->setPosition(ccp(size.width/2, size.height/2 + TITLE_WIN_DY));
    m_titleResult->setOpacity(0);
    m_titleResult->setScale(0.1);
    resultNode->addChild(m_titleResult);
    addStar(star, size.height/2 + STAR_DY);
    
    
    memset(buf, 0, 20);
    m_txtDamage = CCSprite::createWithSpriteFrameName(Txt_Damage);
    m_txtDamage->setPosition(ccp(size.width/2 - TXT_DAMAGE_DX, size.height/2 - TXT_DAMAGE_DY));
    resultNode->addChild(m_txtDamage);
    m_txtDamage->setOpacity(0);
    
    sprintf(buf, "%d%%",damage);
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    m_labelDamage = CCLabelBMFont::create(buf, "num_green.fnt");
#else
    m_labelDamage = CCLabelBMFont::create(buf, "fonts/num_green.fnt");
#endif
    
    m_labelDamage->setPosition(ccp(size.width/2 + NUM_DAMAGE_DX, size.height/2 - NUM_DAMAGE_DY));
    resultNode->addChild(m_labelDamage);
    m_labelDamage->setOpacity(0);
    
    
    m_txtHit = CCSprite::createWithSpriteFrameName(Txt_Hit);
    m_txtHit->setPosition(ccp(size.width/2 - TXT_HIT_DX, size.height/2 - TXT_HIT_DY));
    resultNode->addChild(m_txtHit);
    m_txtHit->setOpacity(0);
    
    memset(buf, 0, 20);
    sprintf(buf, "%d%%",hit);
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    m_labelHit = CCLabelBMFont::create(buf,"num_pink.fnt");
#else
    m_labelHit = CCLabelBMFont::create(buf,"fonts/num_pink.fnt");
#endif
    
    m_labelHit->setPosition(ccp(size.width/2+NUM_HIT_DX, size.height/2 - NUM_HIT_DY));
    resultNode->addChild(m_labelHit);
    m_labelHit->setOpacity(0);
    
    m_txtJiangli = CCSprite::createWithSpriteFrameName(Txt_JiangLi);
    m_txtJiangli->setPosition(ccp(size.width/2, size.height/2 - TXT_JIANGLI));
    resultNode->addChild(m_txtJiangli);
    m_txtJiangli->setOpacity(0);
    
    
    m_iconAward = CCSprite::createWithSpriteFrameName(Icon_Gold_Small);
    m_iconAward->setPosition(ccp(size.width/2 - ICON_AWARD_DX, size.height/2 - ICON_AWARD_DY));
    resultNode->addChild(m_iconAward);
    m_iconAward->setOpacity(0);
    
    memset(buf, 0, 20);
    sprintf(buf, "%04d",gold);
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    m_labelGold = CCLabelBMFont::create(buf, "num_gold.fnt");
#else
    m_labelGold = CCLabelBMFont::create(buf, "fonts/num_gold.fnt");
#endif
    
    m_labelGold->setPosition(ccp(size.width/2 - NUM_GOLD_DX, size.height/2 - NUM_GOLD_DY));
    resultNode->addChild(m_labelGold);
    m_labelGold->setOpacity(0);
    
    CCMenuItem *itemBox = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(Icon_Box),
                                                   CCSprite::createWithSpriteFrameName(Icon_Box),
                                                   this, menu_selector(ResultLayer::clickedBoxCallBack));
    m_iconBox = CCMenu::create(itemBox,NULL);
    m_iconBox->setPosition(ccp(size.width/2 + ICON_BOX_DX, size.height/2 - ICON_BOX_DY));
    resultNode->addChild(m_iconBox);
    m_iconBox->setOpacity(0);
    itemBox->setEnabled(false);
    
    CCMenuItem *itemShare = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(Icon_Share),
                                                     CCSprite::createWithSpriteFrameName(Icon_Share),
                                                     this, menu_selector(ResultLayer::clickedShareCallBack));
    m_iconShare = CCMenu::create(itemShare,NULL);
    m_iconShare->setPosition(ccp(size.width/2 + ICON_SHARE_DX, size.height/2 - ICON_SHARE_DY));
    resultNode->addChild(m_iconShare);
    m_iconShare->setOpacity(0);
    
    CCSequence *scale = CCSequence::create(CCFadeIn::create(0.1),
                                           CCScaleTo::create(Txt_Move_Time-0.2, 1.5),
                                           CCScaleTo::create(0.2, 1),
                                           NULL);
    m_titleResult->runAction(scale);
    
    CCSequence *seq  = CCSequence::create(CCDelayTime::create(Txt_Move_Time),
                                          CCFadeIn::create(0.7),
                                          NULL);
    CCSequence *seq1 = CCSequence::create(CCDelayTime::create(Txt_Move_Time),
                                          CCFadeIn::create(0.7),
                                          CCCallFuncN::create(this, callfuncN_selector(ResultLayer::animationWinCallBack)),
                                          NULL);
    
    m_bgResult->runAction(seq1);
    m_txtHit->runAction((CCSequence*)seq->copy());
    m_txtDamage->runAction((CCSequence*)seq->copy());
    m_labelHit->runAction((CCSequence*)seq->copy());
    m_labelDamage->runAction((CCSequence*)seq->copy());
    m_iconAward->runAction((CCSequence*)seq->copy());
    m_labelGold->runAction((CCSequence*)seq->copy());
    m_iconBox->runAction((CCSequence*)seq->copy());
    m_iconShare->runAction((CCSequence*)seq->copy());
    m_bgResultSmall->runAction((CCSequence*)seq->copy());
    m_txtJiangli->runAction((CCSequence*)seq->copy());
    
    if(fightModel == 0)
    {
         CCLOG("gemstone:%d gemstoneUse:%d",gemstone,gemstoneUse);
         CCLOG("score:%d",score);
         CCLOG("totoal score:%d" ,xUser->totalScore);
         xUser->totalScore+=score;
         (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_AF_UPDATE_USERINFO(this, 1, xUser->pUserInfo->gemstone, gemstone,gemstoneUse,UserData::Instance()->enemy_userId,xUser->totalScore);
    }
    else if(fightModel == 1)
    {
        if(level==1&&!DATAPOOL->m_bRookieGift)
        {
            xUser->pUserInfo->gemstone += 10;
        }
//        (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_AF_UPDATE_USERINFO(this, 1, xUser->pUserInfo->gemstone, gemstone,gemstoneUse, 0,0);
    }
    else if(fightModel == 2)  //boss战胜利  传结果 和 状态更新
    {
        CCLOG("gemstone:%d gemstoneUse:%d hurt:%d",gemstone,gemstoneUse,hurt);
        (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_BOSS_UPDATE_FIGHT(this, 1, xUser->pUserInfo->gemstone,gemstone, gemstoneUse, hurt);
        xData->m_bGetBossPrize = true;
    }
    
    
    if(fightModel!=1) //不是关卡模式 不执行下面的解锁和礼包
        return;
    CCLOG("level:%d",level);
    int unlockHeroID = DATAPOOL->findLockHero(level);
    CCLOG("unlockHeroID:%d",unlockHeroID);
    if(unlockHeroID!=0)
    {
        CCLOG("UNLOCK HERO!");
        HeroRecord *rcd = DATAPOOL->getHeroRecord(unlockHeroID);
        if(!rcd->m_bUnlock)
        {
            rcd->m_bUnlock = true;
            rcd->m_nLevel = 0;
            if(level==1)
                rcd->m_bUnlock = true;
            DATAPOOL->setHeroRecord(unlockHeroID, *rcd);
            m_bShowLockNode = true;
            (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_COPY_RECRUIT(this, 1, unlockHeroID, 1);
            CCSprite *bg = CCSprite::createWithSpriteFrameName(Game_Result_BG);
            unlockNode->addChild(bg,-1);
            bg->setPosition(ccp(-200, size.height/2));
            
            bg->runAction(CCMoveTo::create(0.3, ccp(size.width/2, size.height/2)));
            
            CCSprite *txt = CCSprite::createWithSpriteFrameName(XinHuoBan);
            unlockNode->addChild(txt);
            txt->setPosition(ccp(size.width/2, size.height/2+270));
            txt->setScale(3);
            txt->setOpacity(0);
            CCSequence *txtSeq = CCSequence::create(CCDelayTime::create(0.3),
                                                    CCFadeIn::create(0.1),
                                                    CCEaseElasticInOut::create(CCScaleTo::create(0.2, 1),0.2f),
                                                    NULL);
            txt->runAction(txtSeq);
            
            {
//                CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(juese_plist, juese_image);
                Layout* pLayout = dynamic_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(tavern_scene_page_json));
                setCardData(pLayout,true,unlockHeroID);
                UILayer *layer = UILayer::create();
                layer->addWidget(pLayout);
                CCLOG("width:%f",pLayout->getContentSize().width);
                CCLOG("height:%f",pLayout->getContentSize().height);
                UIPanel *pPanel = dynamic_cast<UIPanel*>(pLayout->getChildByName("page_Panel"));
                
                pLayout->setPosition(ccp(-200-pPanel->getContentSize().width/2, size.height/2 - pPanel->getContentSize().height/2));
                pLayout->runAction(CCMoveTo::create(0.3, ccp(size.width/2-pPanel->getContentSize().width/2, size.height/2 - pPanel->getContentSize().height/2)));
                unlockNode->addChild(layer);
            }
            
            
            
            
            CCMenuItemSprite *btnBack = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(Btn_Right),
                                                                 CCSprite::createWithSpriteFrameName(Btn_Right),
                                                                 this,
                                                                 menu_selector(ResultLayer::unlockBackCallBack));
            CCMenu *menu = CCMenu::create(btnBack,NULL);
            menu->setPosition(ccp(size.width - btnBack->getContentSize().width/2-5, btnBack->getContentSize().height/2 + 5));
            menu->setTag(MenuTag);
            unlockNode->addChild(menu);
            resultNode->setVisible(false);
            HelpMethod::pauseNodeActions(resultNode);
        }
    }
    else if(getUnLockLansID(level)!=0&&star>=3)
    {
        CCLOG("UNLOCK LANS!");
        int nID = getUnLockLansID(level);
        CCLOG("nID:%d",nID);
        EnemyRecord *rcd =  DATAPOOL->getEnemyRecord(nID);
        if(rcd->m_bUnlock)
            return;
        rcd->m_bUnlock = true;
        rcd->m_nLevel = 0;
        m_bShowLockNode = true;
        (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_COPY_RECRUIT(this, 2, nID, 1);
        CCSprite *bg = CCSprite::createWithSpriteFrameName(Game_Result_BG);
        unlockNode->addChild(bg,-1);
        bg->setPosition(ccp(-200, size.height/2));
        
        bg->runAction(CCMoveTo::create(0.3, ccp(size.width/2, size.height/2)));
        
        CCSprite *txt = CCSprite::createWithSpriteFrameName(XinChongWu);   //替图
        unlockNode->addChild(txt);
        txt->setPosition(ccp(size.width/2, size.height/2+270));
        txt->setScale(3);
        txt->setOpacity(0);
        CCSequence *txtSeq = CCSequence::create(CCDelayTime::create(0.3),
                                                CCFadeIn::create(0.1),
                                                CCEaseElasticInOut::create(CCScaleTo::create(0.3, 1),0.2f),
                                                NULL);
        txt->runAction(txtSeq);
        
        {
//            CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(juese_plist, juese_image);
            Layout* pLayout = dynamic_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(tavern_scene_page_json));
            //                    pLayout->setSize(pHeroPageView->getSize());
            setCardData(pLayout,false,nID);
            UILayer *layer = UILayer::create();
            layer->addWidget(pLayout);
            CCLOG("width:%f",pLayout->getContentSize().width);
            CCLOG("height:%f",pLayout->getContentSize().height);
            UIPanel *pPanel = dynamic_cast<UIPanel*>(pLayout->getChildByName("page_Panel"));
            
            pLayout->setPosition(ccp(-200-pPanel->getContentSize().width/2, size.height/2 - pPanel->getContentSize().height/2));
            pLayout->runAction(CCMoveTo::create(0.3, ccp(size.width/2-pPanel->getContentSize().width/2, size.height/2 - pPanel->getContentSize().height/2)));
            unlockNode->addChild(layer);
        }
        
        
        
        
        CCMenuItemSprite *btnBack = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(Btn_Right),
                                                             CCSprite::createWithSpriteFrameName(Btn_Right),
                                                             this,
                                                             menu_selector(ResultLayer::unlockBackCallBack));
        CCMenu *menu = CCMenu::create(btnBack,NULL);
        menu->setPosition(ccp(size.width - btnBack->getContentSize().width/2-5, btnBack->getContentSize().height/2 + 5));
        menu->setTag(MenuTag);
        unlockNode->addChild(menu);
        resultNode->setVisible(false);
        HelpMethod::pauseNodeActions(resultNode);
        
    }
    
    if(level==1&&!DATAPOOL->m_bRookieGift)  //新手礼包界面
    {
        CCLOG("ROOKIE!");
        DATAPOOL->m_bRookieGift = true;
        DATAPOOL->unlockPlanHero(0);
        DATAPOOL->unlockPlanOutfit(0);
        CCSprite *txt = CCSprite::createWithSpriteFrameName(XinShouLiBao);
        txt->setPosition(ccp(-100, size.height/2+200));
        txt->runAction(CCMoveTo::create(0.2, ccp(size.width/2, size.height/2+200)));
        rookieNode->addChild(txt);
        
        CCSprite *zhadan  = CCSprite::createWithSpriteFrameName("zhadan.png");
        zhadan->setPosition(ccp(size.width/2-100, size.height/2));
        zhadan->setOpacity(0);
        zhadan->runAction(CCFadeIn::create(0.2f));
        rookieNode->addChild(zhadan);
        
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        CCLabelBMFont *zhadan_num = CCLabelBMFont::create(buf, "num_yellow.fnt");
#else
        CCLabelBMFont *zhadan_num = CCLabelBMFont::create(buf, "fonts/num_yellow.fnt");
#endif
        
        zhadan_num->setPosition(ccp(size.width/2, size.height/2));
        zhadan_num->setOpacity(0);
        zhadan_num->runAction(CCFadeIn::create(0.2f));
        zhadan_num->setString("X5");
        rookieNode->addChild(zhadan_num);
        
        CCSprite *hongzhaji = CCSprite::createWithSpriteFrameName("hongzhaji.png");
        hongzhaji->setPosition(ccp(size.width/2-100, size.height/2-70));
        hongzhaji->setOpacity(0);
        hongzhaji->runAction(CCFadeIn::create(0.2f));
        rookieNode->addChild(hongzhaji);
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        CCLabelBMFont *hongzhaji_num = CCLabelBMFont::create(buf, "num_yellow.fnt");
#else
        CCLabelBMFont *hongzhaji_num = CCLabelBMFont::create(buf, "fonts/num_yellow.fnt");
#endif
        
        hongzhaji_num->setPosition(ccp(size.width/2, size.height/2-70));
        hongzhaji_num->setOpacity(0);
        hongzhaji_num->runAction(CCFadeIn::create(0.2f));
        hongzhaji_num->setString("X1");
        rookieNode->addChild(hongzhaji_num);
        
        CCSprite *baoshi = CCSprite::createWithSpriteFrameName("baoshi.png");
        baoshi->setPosition(ccp(size.width/2-100, size.height/2-140));
        baoshi->setOpacity(0);
        baoshi->runAction(CCFadeIn::create(0.2f));
        rookieNode->addChild(baoshi);
        
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        CCLabelBMFont *baoshi_num = CCLabelBMFont::create(buf, "num_yellow.fnt");
#else
        CCLabelBMFont *baoshi_num = CCLabelBMFont::create(buf, "fonts/num_yellow.fnt");
#endif
        
        baoshi_num->setPosition(ccp(size.width/2, size.height/2-140));
        baoshi_num->setOpacity(0);
        baoshi_num->setString("X10");
        baoshi_num->runAction(CCFadeIn::create(0.2f));
        rookieNode->addChild(baoshi_num);
        
        OutfitRecord *rcd = DATAPOOL->getOutfitRecord(Prop_Type_ZhaDan);
        rcd->m_nNum += 5;
        DATAPOOL->setOutfitRecord(Prop_Type_ZhaDan, *rcd);
        
        OutfitRecord *rcd1 = DATAPOOL->getOutfitRecord(Prop_Type_HongZhaJi);
        rcd1->m_nNum += 1;
        if(rcd1->m_nNum>999)rcd1->m_nNum=999;
        DATAPOOL->setOutfitRecord(Prop_Type_HongZhaJi, *rcd1);
        
//        DATAPOOL->changeBagGemNum(10);
        
        CCMenuItemSprite *btnBack = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(Btn_Right),
                                                             CCSprite::createWithSpriteFrameName(Btn_Right),
                                                             this,
                                                             menu_selector(ResultLayer::rookieCallBack));
        CCMenu *menu = CCMenu::create(btnBack,NULL);
        menu->setPosition(ccp(size.width - btnBack->getContentSize().width/2-5, btnBack->getContentSize().height/2 + 5));
        menu->setTag(MenuTag);
        rookieNode->addChild(menu);
        unlockNode->setVisible(false);
        HelpMethod::pauseNodeActions(unlockNode);
        HelpMethod::pauseNodeActions(resultNode);
    }
}

void ResultLayer::showLose(int star, int hit, int damage,int gold,int gemstone,int gemstoneUse,int fightModel,int hurt)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
    DATAPOOL->playGameEffect(GameLose);
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    m_nFightModel = (INT16)fightModel;
    
    if(fightModel == 0)
    {
        CCLOG("gemstone:%d gemstoneUse:%d",gemstone,gemstoneUse);
        (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_AF_UPDATE_USERINFO(this, 0, xUser->pUserInfo->gemstone, gemstone,gemstoneUse,UserData::Instance()->enemy_userId,xUser->totalScore);
    }
    else if(fightModel == 1)
    {
        CCLOG("gemstone:%d gemstoneUse:%d",gemstone,gemstoneUse);
//        (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_AF_UPDATE_USERINFO(this, 0, xUser->pUserInfo->gemstone, gemstone,gemstoneUse,0,0);
    }
    else if(fightModel == 2)  //boss战失败 无需向服务器传结果 只传用户info更新
    {
        CCLOG("gemstone:%d gemstoneUse:%d",gemstone,gemstoneUse);
        if(hurt!=0)
        {
            (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_BOSS_UPDATE_FIGHT(this, 0, xUser->pUserInfo->gemstone,gemstone,gemstoneUse, hurt);
            xData->m_bGetBossPrize = true;
        }
    }
    
    CCLayerColor *layer = CCLayerColor::create(ccc4(0, 0, 0, 150));
    addChild(layer,-2,ColorLayerTag);
    m_bgResult = CCSprite::createWithSpriteFrameName(Game_Result_BG);
    resultNode->addChild(m_bgResult,-1);
    m_bgResult->setPosition(ccp(size.width/2, size.height/2));
    m_bgResult->setOpacity(0);
    
    if(fightModel != 2)
    {
        m_bgResultSmall = CCSprite::createWithSpriteFrameName(Game_Result_BG_Small);
        resultNode->addChild(m_bgResultSmall);
        m_bgResultSmall->setPosition(ccp(size.width/2, size.height/2 - BG_SMALL));
        m_bgResultSmall->setOpacity(0);
    }
    
    m_titleResult = CCSprite::createWithSpriteFrameName(Txt_YouLost);
    m_titleResult->setPosition(ccp(size.width/2, size.height));
    resultNode->addChild(m_titleResult);
    
    char buf[10];
    memset(buf, 0, 10);
    m_txtDamage = CCSprite::createWithSpriteFrameName(Txt_Damage);
    m_txtDamage->setPosition(ccp(size.width/2 - TXT_DAMAGE_DX, size.height/2 - TXT_DAMAGE_DY));
    resultNode->addChild(m_txtDamage);
    m_txtDamage->setOpacity(0);
    
    sprintf(buf,"%d%%", damage);
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    m_labelDamage = CCLabelBMFont::create(buf, "num_green.fnt");
#else
    m_labelDamage = CCLabelBMFont::create(buf, "fonts/num_green.fnt");
#endif
    
    m_labelDamage->setPosition(ccp(size.width/2 + NUM_DAMAGE_DX, size.height/2 - NUM_DAMAGE_DY));
    resultNode->addChild(m_labelDamage);
    m_labelDamage->setOpacity(0);
    
    
    m_txtHit = CCSprite::createWithSpriteFrameName(Txt_Hit);
    m_txtHit->setPosition(ccp(size.width/2 - TXT_HIT_DX, size.height/2 - TXT_HIT_DY));
    resultNode->addChild(m_txtHit);
    m_txtHit->setOpacity(0);
    
    memset(buf, 0, 10);
    sprintf(buf, "%d%%",hit);
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    m_labelHit = CCLabelBMFont::create(buf,"num_pink.fnt");
#else
    m_labelHit = CCLabelBMFont::create(buf,"fonts/num_pink.fnt");
#endif
    
    m_labelHit->setPosition(ccp(size.width/2 + NUM_HIT_DX, size.height/2 - NUM_HIT_DY));
    resultNode->addChild(m_labelHit);
    m_labelHit->setOpacity(0);
    if(fightModel != 2)
    {
        m_txtJiangli = CCSprite::createWithSpriteFrameName(Txt_JiangLi);
        m_txtJiangli->setPosition(ccp(size.width/2, size.height/2 - TXT_JIANGLI));
        resultNode->addChild(m_txtJiangli);
        m_txtJiangli->setOpacity(0);
        
        m_iconAward = CCSprite::createWithSpriteFrameName(Icon_Gold_Small);
        m_iconAward->setPosition(ccp(size.width/2 - ICON_AWARD_DX, size.height/2 - ICON_AWARD_DY));
        resultNode->addChild(m_iconAward);
        m_iconAward->setOpacity(0);
        
        memset(buf, 0, 10);
        sprintf(buf, "%04d",gold);
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        m_labelGold = CCLabelBMFont::create(buf, "num_gold.fnt");
#else
        m_labelGold = CCLabelBMFont::create(buf, "fonts/num_gold.fnt");
#endif
        
        m_labelGold->setPosition(ccp(size.width/2 - 60, size.height/2 - 220));
        resultNode->addChild(m_labelGold);
        m_labelGold->setOpacity(0);
    }
    
    CCEaseBounceIn *ebIn = CCEaseBounceIn::create(CCMoveTo::create(Txt_Move_Time, ccp(size.width/2, size.height/2 + TITLE_LOST_DY)));
    m_titleResult->runAction(ebIn);
    
    CCSequence *seq  = CCSequence::create(CCDelayTime::create(Txt_Move_Time),
                                          CCFadeIn::create(1.5),
                                          NULL);
    
    CCSequence *seq1 = CCSequence::create(CCDelayTime::create(Txt_Move_Time),
                                          CCFadeIn::create(1.5),
                                          CCCallFuncN::create(this, callfuncN_selector(ResultLayer::animationLoseCallBack)),
                                          NULL);
    m_bgResult->runAction(seq1);
    m_txtHit->runAction(seq);
    m_txtDamage->runAction((CCSequence*)seq->copy());
    m_labelHit->runAction((CCSequence*)seq->copy());
    m_labelDamage->runAction((CCSequence*)seq->copy());
    if(fightModel != 2)
    {
        m_bgResultSmall->runAction((CCSequence*)seq->copy());
        m_txtJiangli->runAction((CCSequence*)seq->copy());
        m_iconAward->runAction((CCSequence*)seq->copy());
        m_labelGold->runAction((CCSequence*)seq->copy());
    }
    resultNode->setVisible(true);
}

void ResultLayer::didAccelerate(cocos2d::CCAcceleration *pAccelerationValue)
{
    static CCAcceleration lastAcc = *pAccelerationValue;
//    const double dCurTimeStamp = pAccelerationValue->timestamp;
    double dx = pAccelerationValue->x - lastAcc.x;
    double dy = pAccelerationValue->y - lastAcc.y;
    double dz = pAccelerationValue->z - lastAcc.z;
    
    const double dLentthSq = sqrt(dx*dx+dy*dy+dz*dz);
    
    if( dLentthSq > 2 ){
        struct cocos2d::cc_timeval now;  // 秒，毫秒
        cocos2d::CCTime::gettimeofdayCocos2d(&now, NULL);
        static long lLastTime = 0;
        if( now.tv_sec - lLastTime > 1 )
        {
            shankeHand_Gift();
            lLastTime = now.tv_sec;
        }
        
    }
    
    lastAcc = *pAccelerationValue;
}

bool ResultLayer::shankeHand_Gift()
{
    if(!m_bShankeSuccess)
    {
        m_bIsShanke = false;
        m_bShankeSuccess = true;  //先等于true 防止在结果未出现前多次摇晃。
    }
    else
    {
        m_bIsShanke = true;
    }
    if(m_bIsShanke)
        return false;
    xLoading->start(false);
//    xNetEngine->setBusyEnabled(true);
    xNetEngine->setNetMessageEnabled(true);
    m_bIsShanke = true;
    DATAPOOL->playGameEffect(Shake_Sound);
    CCParticleSystem *emitter = NULL;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    emitter = CCParticleSystemQuad::create("shake.plist");
#else
    emitter = CCParticleSystemQuad::create("particle/shake.plist");
#endif
    emitter->setAutoRemoveOnFinish(true);
    emitter->setPosition(shakeNode->getChildByTag(Box_Tag)->getPosition());
    shakeNode->addChild(emitter,100);
    (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_SHAKE(this, m_nFightModel, xUser->pUserInfo->gemstone);
    return true;
}

void ResultLayer::addStar(int star ,float posY)
{
    m_nStar = star;
    int dx = 3;
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    int width = CCSprite::createWithSpriteFrameName(Icon_Star)->getContentSize().width;
    float original_x = size.width/2 - width - dx;
    float original_y = posY;
    int index = Result_Star_One;
    for (int i = 0; i<Max_Star; ++i)
    {
        CCSprite *blank = CCSprite::createWithSpriteFrameName(Icon_Star_Blank);
        blank->setPosition(ccp(original_x+(width+dx)*i, original_y));
        resultNode->addChild(blank,1,index);
        blank->setOpacity(0);
        CCSequence *seq = CCSequence::create(CCDelayTime::create(Txt_Move_Time),
                                             CCFadeIn::create(1.5f),
                                             NULL);
        blank->runAction(seq);
        index++;
    }
}

void ResultLayer::clickedBoxCallBack(cocos2d::CCObject *pSender)
{
    if(!m_bUseShake)
    {
        if(m_bStartingShakeTeach&&!m_bShakeTeach)
        {
            ((CCMenuItem*)(m_btnRight->getChildren()->objectAtIndex(0)))->setEnabled(true);
            m_bShakeTeach = true;
            DATAPOOL->m_bTeachStepOver[TEACH_STEP_YAOJIANG] = true;
        }
        
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        
        menuBox = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(Box_Close),
                                                       CCSprite::createWithSpriteFrameName(Box_Close),
                                                       this,
                                                       menu_selector(ResultLayer::boxClickedCallBack));
        CCMenu *box = CCMenu::create(menuBox,NULL);
        box->setPosition(ccp(size.width/2, size.height/2));
        shakeNode->addChild(box,1,Box_Tag);
        
        CCSprite *tips = CCSprite::createWithSpriteFrameName(Txt_Shake);
        tips->setPosition(ccp(size.width/2, box->getPositionY()-menuBox->getContentSize().height/2-tips->getContentSize().height/2-10));
        shakeNode->addChild(tips,1,123);
        
        CCMenuItem *btnBack = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(Btn_Back),
                                                       CCSprite::createWithSpriteFrameName(Btn_Back_Clicked),
                                                       this,
                                                       menu_selector(ResultLayer::backCallBack));
        m_btnBack = CCMenu::create(btnBack,NULL);
        m_btnBack->setPosition(ccp(btnBack->getContentSize().width/2+5,btnBack->getContentSize().height/2+5));
        shakeNode->addChild(m_btnBack);
        
        m_bUseShake = true;
    }
    resultNode->setVisible(false);
    setAccelerometerEnabled(true);
    shakeNode->setVisible(true);
}

void ResultLayer::boxClickedCallBack(cocos2d::CCObject *pSender)
{
    shankeHand_Gift();
}

void ResultLayer::backCallBack(cocos2d::CCObject *pSender)
{
//    resultNode->setVisible(true);
//    shakeNode->setVisible(false);
//    setAccelerometerEnabled(false);
    quitCallBack(NULL);
}

void ResultLayer::clickedShareCallBack(cocos2d::CCObject *pSender)
{
    //首先获取分享内容
    xNet->DO_CMD_REQ_APP_SENDING_PODIUM(this, 0);
}

void ResultLayer::animationWinCallBack(cocos2d::CCObject *pSender)
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCMenuItem *itemRight = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(Btn_Continue),
                                                     CCSprite::createWithSpriteFrameName(Btn_Continue_Clicked),
                                                     CCSprite::createWithSpriteFrameName(Btn_Continue_Disable),
                                                     this,menu_selector(ResultLayer::quitCallBack));
    m_btnRight = CCMenu::create(itemRight,NULL);
    m_btnRight->setPosition(ccp(size.width - itemRight->getContentSize().width/2-5,itemRight->getContentSize().height/2+5));
    resultNode->addChild(m_btnRight,100);
    m_btnRight->setTouchPriority(-129);
    
    
    if(!m_bStartingShakeTeach&&false == DATAPOOL->m_bTeachStepOver[TEACH_STEP_YAOJIANG])
    {
        m_bShakeTeach = false;
    }
    
    int index = Result_Star_One;
    for (int i=0; i<m_nStar; i++)
    {
        CCSprite *blank = (CCSprite*)resultNode->getChildByTag(index);
        if(blank)
        {
            CCSprite *star = CCSprite::createWithSpriteFrameName(Icon_Star);
            star->setPosition(blank->getPosition());
            star->setOpacity(0);
            star->setScale(4);
            resultNode->addChild(star,2);
            CCSequence *seq = CCSequence::create(CCDelayTime::create(StarFallTime*i),
                                                 CCFadeIn::create(0.1),
                                                 CCScaleTo::create(StarFallTime, 1),
                                                 CCCallFunc::create(this, callfunc_selector(ResultLayer::starFallCallBack)),
                                                 NULL);
            CCSequence *seq2 = CCSequence::create(CCDelayTime::create(StarFallTime*i),
                                                  CCFadeIn::create(0.1),
                                                  CCScaleTo::create(StarFallTime, 1),
                                                  NULL);
            if(i==m_nStar-1)
            {
                star->runAction(seq);
            }
            else
            {
                star->runAction(seq2);
            }
        }
        index++;
    }
}

void ResultLayer::animationLoseCallBack(cocos2d::CCObject *pSender)
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    CCMenuItem *itemLeft = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(Btn_Back),
                                                    CCSprite::createWithSpriteFrameName(Btn_Back_Clicked),
                                                    this,menu_selector(ResultLayer::quitCallBack));
    m_btnLeft = CCMenu::create(itemLeft,NULL);
    m_btnLeft->setPosition(ccp(itemLeft->getContentSize().width/2+5,itemLeft->getContentSize().height/2+5));
    resultNode->addChild(m_btnLeft,100);
    m_btnLeft->setTouchPriority(-129);
    
    if(m_nFightModel==1) //pve
    {
        CCMenuItem *itemRight = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(Btn_Restart),
                                                         CCSprite::createWithSpriteFrameName(Btn_Restart_Clicked),
                                                         this,menu_selector(ResultLayer::restartCallBack));
        m_btnRight = CCMenu::create(itemRight,NULL);
        m_btnRight->setPosition(ccp(size.width - itemRight->getContentSize().width/2-5,itemRight->getContentSize().height/2+5));
        resultNode->addChild(m_btnRight,100);
        m_btnRight->setTouchPriority(-129);
    }
}

void ResultLayer::restartCallBack(cocos2d::CCObject *pSender)
{
    FightScene* scene = (FightScene*)getParent();
    FightLayer *layer = (FightLayer*)(scene->getChildByTag(1));
    HelpMethod::resumeNodeActions(layer);
    this->setVisible(false);
    this->setTouchEnabled(false);
//    m_bgResult->removeFromParentAndCleanup(true);
    CCLayerColor *layercolor = (CCLayerColor*)getChildByTag(ColorLayerTag);
    if(layercolor)
        layercolor->removeFromParentAndCleanup(true);
    resultNode->removeAllChildrenWithCleanup(true);
    shakeNode->removeAllChildrenWithCleanup(true);
    unlockNode->removeAllChildrenWithCleanup(true);
    DATAPOOL->removeEffectCache(pvpLoadingRes);
    m_bIsShanke = false;
    m_bShankeSuccess = false;
    m_bUseShake = false;
    layer->gameRestart(false);
}

void ResultLayer::quitCallBack(cocos2d::CCObject *pSender)
{
    FightScene* scene = (FightScene*)getParent();
    FightLayer *layer = (FightLayer*)(scene->getChildByTag(1));
//    HelpMethod::resumeNodeActions(layer);
    this->setVisible(false);
    this->setTouchEnabled(false);
    CCLayerColor *layercolor = (CCLayerColor*)getChildByTag(ColorLayerTag);
    if(layercolor)
        layercolor->removeFromParentAndCleanup(true);
//    m_bgResult->removeFromParentAndCleanup(true);
    resultNode->removeAllChildrenWithCleanup(true);
    shakeNode->removeAllChildrenWithCleanup(true);
    unlockNode->removeAllChildrenWithCleanup(true);
    DATAPOOL->removeEffectCache(pvpLoadingRes);
    m_bIsShanke = false;
    m_bShankeSuccess = false;
    m_bUseShake = false;
    layer->gameQuit(false);
}

void ResultLayer::unlockBackCallBack(cocos2d::CCObject *pSender)
{
    if(m_bShowLockNode)
    {
        CCSequence *seq = CCSequence::create(CCMoveBy::create(0.2, ccp(400, 0)),
                                             CCCallFuncN::create(this, callfuncN_selector(ResultLayer::nodeVisibleCallBack_RemoveRes)),
                                             NULL);
        unlockNode->runAction(seq);
    }
    resultNode->setVisible(true);
    HelpMethod::resumeNodeActions(resultNode);
    if(!m_bStartingShakeTeach&&false == DATAPOOL->m_bTeachStepOver[TEACH_STEP_YAOJIANG])
    {
        CCSequence *seq1 = CCSequence::create(CCDelayTime::create(Txt_Move_Time+StarFallTime*m_nStar+1.5+0.2),
                                             CCCallFunc::create(this, callfunc_selector(ResultLayer::createTeachLayerCallBack)),
                                             NULL);
        this->runAction(seq1);
    }
}

void ResultLayer::rookieCallBack(CCObject *pSender)
{
    CCSequence *seq = CCSequence::create(CCMoveBy::create(0.2, ccp(400, 0)),
                                         CCCallFuncN::create(this, callfuncN_selector(ResultLayer::nodeVisibleCallBack)),
                                         NULL);
    rookieNode->runAction(seq);
    if(m_bShowLockNode)
    {
        unlockNode->setPosition(ccp(-400, 0));
        unlockNode->setVisible(true);
        CCMenu *menu = (CCMenu*)unlockNode->getChildByTag(MenuTag);
        if(menu)
        {
            menu->setOpacity(0);
            CCSequence *fadein = CCSequence::create(CCDelayTime::create(0.5),
                                                    CCFadeIn::create(0.1),
                                                    NULL);
            menu->runAction(fadein);
        }
        
        CCSequence *seq2 = CCSequence::create(CCDelayTime::create(0.3),
                                              CCMoveBy::create(0.2, ccp(400, 0)),
                                              CCCallFuncN::create(this, callfuncN_selector(ResultLayer::nodeResumeActCallBack)),
                                              NULL);
        unlockNode->runAction(seq2);
    }
    else
    {
        unlockBackCallBack(NULL);
    }
}

void ResultLayer::nodeResumeActCallBack(CCNode *pSender)
{
    HelpMethod::resumeNodeActions(pSender);
}

void ResultLayer::nodeVisibleCallBack(CCNode *pSender)
{
    pSender->setVisible(false);
}

void ResultLayer::nodeVisibleCallBack_RemoveRes(cocos2d::CCNode *pSender)
{
    pSender->removeAllChildrenWithCleanup(true);
    pSender->setVisible(false);
//    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(juese_plist);
}

void ResultLayer::shakeTeachCallBack()
{
    CTeachLayer *teachLayer = NULL;
    teachLayer = (CTeachLayer*)getChildByTag(ShakeTeachLayer);
    if(teachLayer)
    {
        teachLayer->removeFromParentAndCleanup(true);
    }
    m_bStartingShakeTeach = true;
    clickedBoxCallBack(NULL);
}

void ResultLayer::starFallCallBack()
{
    enableBtn();
    ((CCMenuItem*)(m_iconBox->getChildren()->objectAtIndex(0)))->runAction(CCShake::create(2, 10));
    
    if(!unlockNode->isVisible()&&!m_bStartingShakeTeach&&false == DATAPOOL->m_bTeachStepOver[TEACH_STEP_YAOJIANG])
    {
        m_bShakeTeach = false;
        
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        CTeachLayer *teachLayer = NULL;
        teachLayer = (CTeachLayer*)getChildByTag(ShakeTeachLayer);
        if(!teachLayer)
        {
            teachLayer = CTeachLayer::create();
            teachLayer->setContentSize(size);
            teachLayer->setFocusProp(m_iconBox->getPosition(), 100);
            teachLayer->setPicInfo(ccp(size.width/2, size.height/2), "teach_txt_5.png");
            teachLayer->setCallbackFun(this, callfunc_selector(ResultLayer::shakeTeachCallBack));
            addChild(teachLayer, 100,ShakeTeachLayer);
        }
    }
}

void ResultLayer::setCardData(Layout *pPageLayout, bool hero, int nID)
{
    if(hero)
    {
        int nHeroId = nID;
        Game_Hero_Data *pHeroData = DATAPOOL->getHeroData(nHeroId);
        do
        {
            CC_BREAK_IF(!pPageLayout);
            CC_BREAK_IF(!pHeroData);
            
            //名称
            UIImageView *pName = dynamic_cast<UIImageView*>(pPageLayout->getChildByName("NameImageView"));
            if (pName)
            {
                pName->loadTexture(CCString::createWithFormat(juese_format_name, nHeroId)->getCString(), UI_TEX_TYPE_PLIST);
            }
            
            //头像
            UIImageView *pHead = dynamic_cast<UIImageView*>(pPageLayout->getChildByName("HeadImageView"));
            if (pHead)
            {
                pHead->loadTexture(CCString::createWithFormat(juese_format_head, nHeroId)->getCString(), UI_TEX_TYPE_PLIST);
            }
            
            //等级星级
            UIPanel *pPanel_1 = dynamic_cast<UIPanel*>(pPageLayout->getChildByName("StarPanel_1"));
            int nLevel = DATAPOOL->findHeroLevel(nHeroId);
            for (int m = 0; m < nLevel; m++)
            {
                UICheckBox *pCheckBox = static_cast<UICheckBox *>(pPanel_1->getChildByName(CCString::createWithFormat("CheckBox_%d", (m+1))->getCString()));
                pCheckBox->setSelectedState(true);
            }
            
            //伤害星级
            UIPanel *pPanel_2 = dynamic_cast<UIPanel*>(pPageLayout->getChildByName("StarPanel_2"));
            int level_2 = pHeroData->mStar;
            for (int n = 0; n < level_2; n++)
            {
                UICheckBox *pCheckBox = dynamic_cast<UICheckBox *>(pPanel_2->getChildByName(CCString::createWithFormat("CheckBox_%d", (n+1))->getCString()));
                pCheckBox->setSelectedState(true);
            }
            
            //技能
            {
                UILabel *jinengText = dynamic_cast<UILabel *>(pPageLayout->getChildByName("jineng_Label"));
                if (jinengText)
                {
                    jinengText->setText(xStr(CCString::createWithFormat("magicId_%d", pHeroData->mMagicID)->getCString()));
                }
            }
            
            //解锁条件
            {
                UILabel *tiaojianText = dynamic_cast<UILabel *>(pPageLayout->getChildByName("tiaojian_Label"));
                if (tiaojianText)
                {
                    tiaojianText->setText(xStr(CCString::createWithFormat("tiaojian_%d", pHeroData->mBuy)->getCString()));
                }
            }
            
        } while (0);
    }
    else
    {
        int nLansID = nID;
        Enemy_Data* pEnemyData = DATAPOOL->getEnemyData(nLansID);
        do
        {
            CC_BREAK_IF(!pPageLayout);
            CC_BREAK_IF(!pEnemyData);
            
            //名称
            UIImageView *pName = dynamic_cast<UIImageView*>(pPageLayout->getChildByName("NameImageView"));
            if (pName)
            {
                pName->loadTexture(CCString::createWithFormat(juese_format_name, nLansID)->getCString(), UI_TEX_TYPE_PLIST);
            }
            
            //头像
            UIImageView *pHead = dynamic_cast<UIImageView*>(pPageLayout->getChildByName("HeadImageView"));
            if (pHead)
            {
                pHead->loadTexture(CCString::createWithFormat(juese_format_head, nLansID)->getCString(), UI_TEX_TYPE_PLIST);
            }
            
            //等级星级
            UIPanel *pPanel_1 = dynamic_cast<UIPanel*>(pPageLayout->getChildByName("StarPanel_1"));
            int nLevel = DATAPOOL->findPetLevel(nLansID);
            for (int m = 0; m < nLevel; m++)
            {
                UICheckBox *pCheckBox = static_cast<UICheckBox *>(pPanel_1->getChildByName(CCString::createWithFormat("CheckBox_%d", (m+1))->getCString()));
                pCheckBox->setSelectedState(true);
            }
            
            //伤害星级
            UIPanel *pPanel_2 = dynamic_cast<UIPanel*>(pPageLayout->getChildByName("StarPanel_2"));
            int level_2 = pEnemyData->mStar;
            for (int n = 0; n < level_2; n++)
            {
                UICheckBox *pCheckBox = dynamic_cast<UICheckBox *>(pPanel_2->getChildByName(CCString::createWithFormat("CheckBox_%d", (n+1))->getCString()));
                pCheckBox->setSelectedState(true);
            }
            
            //技能
            {
                UILabel *jinengText = dynamic_cast<UILabel*>(pPageLayout->getChildByName("jineng_Label"));
                if (jinengText)
                {
                    jinengText->setText(xStr(CCString::createWithFormat("magicId_%d", pEnemyData->mMagicID)->getCString()));
                }
            }
            
            //解锁条件
            {
                UILabel *tiaojianText = dynamic_cast<UILabel*>(pPageLayout->getChildByName("tiaojian_Label"));
                if (tiaojianText)
                {
                    tiaojianText->setText(xStr(CCString::createWithFormat("tiaojian_%d", pEnemyData->mBuy)->getCString()));
                }
            }
            
        } while (0);

    }
}

void ResultLayer::createTeachLayerCallBack()
{
    m_bShakeTeach = false;
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CTeachLayer *teachLayer = NULL;
    teachLayer = (CTeachLayer*)getChildByTag(ShakeTeachLayer);
    if(!teachLayer)
    {
        teachLayer = CTeachLayer::create();
        teachLayer->setContentSize(size);
        teachLayer->setFocusProp(m_iconBox->getPosition(), 100);
        teachLayer->setPicInfo(ccp(size.width/2, size.height/2), "teach_txt_5.png");
        teachLayer->setCallbackFun(this, callfunc_selector(ResultLayer::shakeTeachCallBack));
        addChild(teachLayer, 100,ShakeTeachLayer);
    }
}

int ResultLayer::getUnLockLansID(int nlevel)
{
    int returnID = 0;
    switch (nlevel) {
        case 5:
            returnID = 1105;
            break;
        case 10:
            returnID = 1106;
            break;
        case 15:
            returnID = 1111;
            break;
        case 20:
            returnID = 1112;
            break;
        case 25:
            returnID = 1117;
            break;
        case 30:
            returnID = 1118;
            break;
        case 35:
            returnID = 1123;
            break;
        default:
            returnID = 0;
            break;
    }
    return returnID;
}

void ResultLayer::disableBtn()
{
    ((CCMenuItem*)(m_iconBox->getChildren()->objectAtIndex(0)))->setEnabled(false);
}

void ResultLayer::enableBtn()
{
    ((CCMenuItem*)(m_iconBox->getChildren()->objectAtIndex(0)))->setEnabled(true);
}
