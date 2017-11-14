//
//  PvPScene.cpp
//  SingleEye
//
//  Created by gwsoft on 12/31/13.
//
//

#include "PvPScene.h"
#include "HomeScene.h"
//#include "cocos-ext.h"
//#include "HelpMethod.h"

#define CC_CAST(type)       (static_cast<type*>)
#define SPACE               (10)
#define NAME_LEN            (14)

///////////////////////////////////////////// List



typedef enum PVPTag
{
    TMailImage = 15,
    TMailNum = 18,
    TBack = 27,
    TOption = 28,
    TYes = 38,
    TLeft = 39,
    TRight = 40,
    TMail = 50,
    TShop = 51,
    TMatch = 53,
    TChallenge = 54,
    TPanelMatch = 76,
    TPanelList = 91,
    TFightTxt = 147,
    TName = 149,
    TMatchImage = 151,
    TSlider = 154,
    TPanelChallenge = 760,
    TList = 200,
    TBuyPower = 282,
    TNum = 283,
    
    TSRank = 459,
    TSName = 460,
    TSScore = 542,
    
    TBlast = 600,
    
    TTxtID = 636,
    TTxtScore = 637,
    TUserScore = 639,    
    
    TTY = 700,
    
    TLRanking = 701,
    TLRankNum = 702,
    TLName = 703,
    TLScore = 704,
    TLTxt = 705,


    TUserName = 770,
    
    
    TStar1 = 800,   //900~1000, Star 保留字段
    TStar2,
    TStar3,
    TStar4,
    TStar5,
    
    TListStart = 900,   //900~1000, List 保留字段
    TListBG,
    TListRank,
    TListName,
    TListScore,
    TListEnd = 999,
    
    PVPTag_COUNT
}PVPTag;


PvPScene::PvPScene()
{
    m_pList = NULL;
    m_aLoading = NULL;
    m_iCurIdx = 0;    
    m_aYG = NULL;

    m_nCount = 0;
    m_userId = 0;
    m_bBlock = false;
    m_iMatchCount = 0;
}


PvPScene::~PvPScene()
{
    
}


CCScene* PvPScene::scene()
{
    CCScene* scene = NULL;
    do {
        scene = CCScene::create();
        CC_BREAK_IF(!scene);
        
        PvPScene *layer = PvPScene::create();
        scene->addChild(layer);
    } while (0);
    
    return scene;
}

//init 里面除了初始化固定的控件, 不要做其它事情
bool PvPScene::init()
{
    if (CCLayer::init()) {
        m_pUILayer = UILayer::create();
        
        addChild(m_pUILayer);                        
        
        this->setTouchEnabled(true);
        //不要在init里面去调用网络命令        
                
        return true;
    }
    
    return true;
}


void PvPScene::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}


void PvPScene::onEnter()
{
    CCLayer::onEnter();
    
    xSprite->addSpriteFramesWithFile("PvPScene/Solo.plist");
    xSprite->addSpriteFramesWithFile("PvPScene/taiyang0.plist");    
    xData->addEffectCache("PvPScene/taiyang.ExportJson"); 
        
    
    m_pRootLayer = (UIPanel*)(GUIReader::shareReader()->widgetFromJsonFile("PvPScene/PvPScene_1.ExportJson"));
    HelpMethod::adapt(m_pRootLayer, "bg_2.png");
    
    m_pUILayer->addWidget(m_pRootLayer);
    
    //Touch
    m_pUILayer->setTouchEnabled(true);
    m_pUILayer->setTouchPriority(-1);
    
    //事件注册
    HelpMethod::initUIButton(TYes, toucheventselector(PvPScene::onButton), m_pRootLayer, this);
    HelpMethod::initUIButton(TRight, toucheventselector(PvPScene::onButton), m_pRootLayer, this);
    HelpMethod::initUIButton(TLeft, toucheventselector(PvPScene::onButton), m_pRootLayer, this);
    HelpMethod::initUIButton(TBuyPower, toucheventselector(PvPScene::onButton), m_pRootLayer, this);
    
    HelpMethod::initUIButton(TMail, toucheventselector(PvPScene::onButton), m_pRootLayer, this);
    HelpMethod::initUIButton(TBack, toucheventselector(PvPScene::onButton), m_pRootLayer, this);
    
    
    m_pMatch = HelpMethod::initUICheckBox(TMatch, checkboxselectedeventselector(PvPScene::onCheckBox), m_pRootLayer, this);
    m_pChallenge = HelpMethod::initUICheckBox(TChallenge, checkboxselectedeventselector(PvPScene::onCheckBox), m_pRootLayer, this);
    
    m_pMatch->setSelectedState(true);    
    
    m_pPanelMatch = static_cast<UIPanel*>((m_pRootLayer->getChildByTag(TPanelMatch)));
    m_pPanelChallenge = static_cast<UIPanel*>((m_pRootLayer->getChildByTag(TPanelChallenge)));
    
    m_pPanelMatch->setVisible(true);
    m_pPanelChallenge->setVisible(true);
    m_pPanelMatch->setEnabled(false);
    m_pPanelChallenge->setEnabled(false);
    
    m_pImage = static_cast<UIImageView*>(m_pRootLayer->getChildByTag(TTY));            
            
    checkPower();    
    checkMessage(); 
    showScore();
    MsgSystem::Instance()->RegisterObjAndHandler(this, MSGSYS_HANLE_MSG(PvPScene::handleMsg));
    
    xNet->DO_CMD_REQ_FAST_MATCH(this, xUser->pUserInfo->gemstone);
    //注册消息
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(PvPScene::newMessage), NC_NewMessage, NULL);
    //    CCScene *pRunScene = CCDirector::sharedDirector()->getRunningScene();
    //    pRunScene->addChild(dialog, 255);
    
    //    EngineSystem::SharedEngineSystem()->ShowDialog("ABC", "bot05.png", "bot05.png", DIALOG_LEVEL_SCENE, false, this);
    
    m_pRootScene = (CCScene*)getParent();            
}


void PvPScene::newMessage(CCObject *obj)
{
    checkMessage();
}

void PvPScene::onExit()
{
    CCLayer::onExit();     
    
    HelpMethod::removeUnusedResourceAtExit(this);
    CCArmatureDataManager::purge();
}


//////////////////////////////////////////
bool PvPScene::handleMsg(const Message &msg)
{
    CCLOG("NetTest::HandleMsg msgType= %d", msg.m_nMsgType);
    switch (msg.m_nMsgType)
    {
        case CMD_RES_FAST_MATCH:
        {
            if (UserData::Instance()->rankingListCount > 0 && m_pMatch->getSelectedState() == true) {
                copyMatchData();
                showMatchData();
                
                m_pPanelMatch->setEnabled(true);
            }

            break;
        }
        case CMD_RES_RANKING:    //2.4.6.16.1	获取排行榜玩家返回的信息(CMD_RES_RANKING) 	47
        {
            if (m_pChallenge->getSelectedState() == true) {
                createList();
            }            
            break;
        }
        case CMD_RES_FIGHT:
        {
            xSocial->statByUserID(SPvP);
            sunRise();            
        }
            break;                      
        default:
            break;
    }
    
    return true;
}


void PvPScene::onDialog(int id, int iTag)
{
    if (id == 1) {
        switch (iTag) {
            case 0:
                //跳转商店界面
                xSM->forward(ST_BuyPower);
                break;
            default:
                break;
        }
    }
    else
    {
        switch (iTag) {
            case 0:
            {
                if (reducePower() == false) {
                    xDialog->showYesNo(xStr("power_tip"), dialogeventselector(PvPScene::onDialog), this);
                    xDialog->setDialogID(1);
                }
                else
                {
                    xData->setPvPId(getRandomHeadID());                    
                    xData->waitSyncPetData();
                    (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_FIGHT(this, m_userId, 0, xUser->pUserInfo->gemstone, xUserInfo->mercemaryNum, xUserInfo->listMercemary);
                }
            }
                break;
            case 1:
            {
                CCLOG("********** No");
            }
                break;
            default:
            {
            }
                break;
        }

    }
    
    
}



void PvPScene::onButton(CCObject* pSender, TouchEventType type)
{
    if(type != TOUCH_EVENT_ENDED || m_bBlock == true)
    {
        return;
    }
    
    xData->playKeytone();
    
    UIWidget *widget = (UIWidget*)pSender;
    
    int iTag = widget->getTag();
    
    switch (iTag) {
        case TMail:
        {
            xSM->forward(ST_Message);
        }
            break;
        case TMatch:
        {
        }
            break;
        case TLeft:
        {
            cccycleNum(m_iMatchCount, false, &m_iCurIdx);
            showMatchData();
        }
            break;
        case TRight:
        {
            cccycleNum(m_iMatchCount, true, &m_iCurIdx);
            showMatchData();
        }
            break;
        case TBuyPower:
        {
            xSM->forward(ST_BuyPower);
        }
            break;
        case TYes:
        {
            if (reducePower() == false) {
                xDialog->showYesNo(xStr("power_tip"), dialogeventselector(PvPScene::onDialog), this);
                xDialog->setDialogID(1);
            }
            else
            {
                //临时保存id
                xData->setPvPId(m_vMatch.at(m_iCurIdx).iHeadImage);
                xData->waitSyncPetData();
                (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_FIGHT(this, m_vMatch.at(m_iCurIdx).userId, 0, xUser->pUserInfo->gemstone, xUserInfo->mercemaryNum, xUserInfo->listMercemary);
            }
        }
            break;
        case TBack:
        {
            xSM->enableTransit();
            xSM->back(ST_Home);
        }
            break;
        default:
        {
            if (iTag >= TListStart && iTag < TListEnd) {
                
                RankingUser **rankData = UserData::Instance()->rankingList;
                m_userId = rankData[iTag - TListStart]->userId;
                
                if (m_userId == xUser->pUserInfo->userId) {
                    xDialog->show(xStr("cannot_pk"));
                }
                else
                {
                    xDialog->show(xStr("fight_tip"), "ksu_08.png", dialogeventselector(PvPScene::onDialog), this);
                }                
            } 
        }
            break;
    }
}



void PvPScene::onCheckBox(CCObject *pSender, CheckBoxEventType type)
{
    if (m_bBlock == true) {
        return;
    }
    
    xData->playKeytone();    
    
    UICheckBox *check = (UICheckBox*)pSender;
    
    switch (type)
    {
            //暂时不处理 UNSELECTED
        case CHECKBOX_STATE_EVENT_UNSELECTED:
            if (check->getTag() == TMatch) {
                m_pMatch->setSelectedState(true);
            }
            else
            {
                m_pChallenge->setSelectedState(true);
            }
            
            break;
            
        case CHECKBOX_STATE_EVENT_SELECTED:
            if (check->getTag() == TMatch) {
                m_pChallenge->setSelectedState(false);
                
                if (m_iMatchCount != 0) {                
                m_pPanelMatch->setEnabled(true);
                }  
                else
                {
                    xNet->DO_CMD_REQ_FAST_MATCH(this, xUser->pUserInfo->gemstone);
                }

                m_pPanelChallenge->setEnabled(false);
                if(m_pList != NULL)
                {
                    m_pList->setTouchEnabled(false);
                    m_pList->setVisible(false);
                }
                
            }
            else
            {
                m_pMatch->setSelectedState(false);
                m_pPanelMatch->setEnabled(false);
                m_pPanelChallenge->setEnabled(true);
                
                if(m_pList == NULL)
                {
                    (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_RANKING(this, xUser->pUserInfo->gemstone);
                }
                else
                {
                    m_pList->setTouchEnabled(true);
                    m_pList->setVisible(true);
                }
            }
            
            break;
            
        default:
            break;
    }
}


bool PvPScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}


void PvPScene::createList()
{        
    UIPanel *panel = (UIPanel*)m_pRootLayer->getChildByTag(760);
    
    // Create the list view
    m_pList = UIListViewEx::create();
    
    m_pList->setDirection(SCROLLVIEW_DIR_VERTICAL);
    m_pList->setTouchEnabled(true);
    m_pList->setBounceEnabled(true);
    m_pList->setSize(panel->getSize());
    m_pList->setItemsMargin(12.f);
    
    //m_pList->addEventListenerListViewEx(this, listviewexeventselector(BossScene::selectedItemEvent));
    panel->addChild(m_pList);
    
    
    Layout* default_item = (UIPanel*)(GUIReader::shareReader()->widgetFromJsonFile("BossScene/List.ExportJson"));        
    //default_item->setTouchEnabled(true);  
    
    // set model
    m_pList->setItemModel(default_item);
    
    
    m_iChallengeCount = UserData::Instance()->rankingListCount;     //数量
    for (int i = 0; i < m_iChallengeCount; ++i)
    {
        m_pList->pushBackDefaultItem();
    }
    
    m_pList->setGravity(LISTVIEW_GRAVITY_CENTER_VERTICAL);
    
    CCPoint point;
    
    //填充内容
    for (int i = 0; i < m_iChallengeCount; i++) {
        UIPanel *rootPanel = (UIPanel*) m_pList->getItem(i);
        rootPanel->setTag(TListStart + i);
        rootPanel->setTouchEnabled(true);
        rootPanel->addTouchEventListener(this, toucheventselector(PvPScene::onButton));
                        
        UIImageView *uRanking = (UIImageView*)rootPanel->getChildByTag(TLRanking);        
        UILabelAtlas *uRankNum = (UILabelAtlas*)rootPanel->getChildByTag(TLRankNum); 
        UILabel *uName = (UILabel*)rootPanel->getChildByTag(TLName);
        UILabelAtlas *uScore = (UILabelAtlas*)rootPanel->getChildByTag(TLScore); 
        UIImageView *uTxt = (UIImageView*)rootPanel->getChildByTag(TLTxt);
        
        if (i == 0) {
            //默认就是
        }
        else if (i == 1) {
            uRanking->loadTexture("pming_07.png", tPlist);
        }
        else if (i == 2)
        {
            uRanking->loadTexture("pming_08.png", tPlist);            
        }
        else
        {
            uRanking->loadTexture("pming_09.png", tPlist); 
        }
        
        RankingUser **rankData = UserData::Instance()->rankingList;
        uRankNum->setStringValue(ccitoa(i + 1, temp));
        
        if (i > 2) {            
            point = uRankNum->getPosition();            
            uRankNum->setPosition(ccp(point.x, point.y - 14));
        }
        
        uName->setText(rankData[i]->userName.c_str());
        uScore->setStringValue(ccitoa(rankData[i]->score, temp));   
        uTxt->setTexture("pming_13.png", tPlist);
    }                    
}


void PvPScene::update(float delta)
{
    //CCLOG("********** %f", m_pListEx->getChildrenPosition());
    //m_pListEx->getChildrenPosition();
	//CCLOG("********** %f", m_pListEx->getChildrenPosition());
}


void PvPScene::copyMatchData()
{
    MatchData match;
    RankingUser **rankData = UserData::Instance()->rankingList;
    m_iMatchCount = UserData::Instance()->rankingListCount;
    for (int i = 0; i < m_iMatchCount; i++) {
        match.userId = rankData[i]->userId;
        match.userName = rankData[i]->userName;
        match.score = rankData[i]->score;        
        match.iHeadImage = getRandomHeadID();                  //临时征用 作为图像id
        
        m_vMatch.push_back(match);
    }
    
}


void PvPScene::showMatchData()
{
    UILabel *name = dynamic_cast<UILabel*>(m_pRootLayer->getChildByTag(TName));
    name->setText(m_vMatch.at(m_iCurIdx).userName.c_str());
    
    UIImageView *view = dynamic_cast<UIImageView*>(m_pRootLayer->getChildByTag(TMatchImage));
    
    char str[30];
    sprintf(str, "pvp_%d.png", m_vMatch.at(m_iCurIdx).iHeadImage);
    view->loadTexture(str, UI_TEX_TYPE_PLIST);
    
    UILabelAtlas *rank = (UILabelAtlas*)m_pRootLayer->getChildByTag(TSScore);
    rank->setStringValue(ccitoa(m_vMatch.at(m_iCurIdx).score, temp));
}


void PvPScene::checkMessage()
{
    int iNewMsgCount = CDataPool::sharedDataPool()->getNewMessageCount();
    
    if (iNewMsgCount != 0) {
        UILabelAtlas* num = static_cast<UILabelAtlas*>((m_pRootLayer->getChildByTag(TMailNum)));
        num->setStringValue(ccitoa(iNewMsgCount, temp));
        UIImageView* message = static_cast<UIImageView*>((m_pRootLayer->getChildByTag(TMailImage)));
        message->setVisible(true);
    }
    else
    {
        UIImageView* message = static_cast<UIImageView*>((m_pRootLayer->getChildByTag(TMailImage)));
        message->setVisible(false);
    }
}


void PvPScene::checkPower()
{   
    int i =  xUser->pUserInfo->strength;    

    if (m_aYG == NULL) {
        m_aYG = CCArmature::create("taiyang");            
        m_aYG->setVisible(true);
        m_aYG->getAnimation()->play("a");
        m_pImage->addRenderer(m_aYG, 50);          
    }      
        
    UILabelAtlas* num = static_cast<UILabelAtlas*>((m_pRootLayer->getChildByTag(TNum)));
    
    num->setStringValue(ccitoa(i, temp));
}



bool PvPScene::reducePower()
{
    int iPower = xUser->pUserInfo->strength;
    
    if (iPower >= 1) {           
        return true;
    }
    else
    {
        return false;
    }
}


void PvPScene::sunRise()
{
    m_bBlock = true;
        
    //创建加入 TopLayer    
    m_topLayer = TopLayer::create();
    m_pRootScene->addChild(m_topLayer, 1);       
    m_topLayer->addTopImage(m_pImage->convertToWorldSpace(CCPointZero));    
    
    CDataPool::sharedDataPool()->changePower(-1);    
    checkPower();
}


void PvPScene::test()
{
    CDataPool *pool = xData;
    
    const char *s = "afasdfefelfelfleflellslsldflelfleflelflelfelfleflelfel";
    
    GMessage tGMsg;
    tGMsg.iType = 1;
    tGMsg.iData = 1;
    tGMsg.iLen = strlen(s);
    tGMsg.msg = new char[tGMsg.iLen + 1];
    strcpy(tGMsg.msg, s);
    
    pool->m_vSystem.push_back(tGMsg);
    
    pool->m_iNewMsgCount++;
    
    iNewMsgCount++;
}


int PvPScene::getRank()
{
    int iRank = 0;
    RankingUser **rankData = UserData::Instance()->rankingList;
    string sUserName = CCUserDefault::sharedUserDefault()->getStringForKey(UUserName);
    
    for (int i = 0; i < m_iChallengeCount; i++) {
        if (rankData[i]->userName.compare(sUserName) == 0
            && rankData[i]->score == xUser->totalScore) {
            iRank = i + 1;
            break;
        }
    }
    
    return iRank;
}


void PvPScene::leaveStamp(UIImageView *uOri)
{
    m_pTemp = UIImageView::create();
    m_pTemp->loadTexture("yangguang2.png", UI_TEX_TYPE_PLIST);
    m_pTemp->setZOrder(0);
    m_pTemp->setPosition(uOri->getPosition());
    
    
    UIWidget *widget = uOri->getParent();    
    widget->addChild(m_pTemp);    
}


/////////////////////////////////////////////
bool TopLayer::init()
{    
    m_emitter = NULL;
    
    m_pUILayer = UILayer::create();    
    addChild(m_pUILayer);
    
    setTouchEnabled(true);
    
    return true;
}


void TopLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kDialogHandlerPriority + 1, true);
}

bool TopLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}


void TopLayer::addTopImage(CCPoint point)
{
    m_pImage = UIImageView::create();  
    m_pImage->setPosition(point);
    
    CCArmature * armature = CCArmature::create("taiyang");            
    m_pImage->addRenderer(armature, 50);
    armature->getAnimation()->play("a");
        
    m_pUILayer->addWidget(m_pImage);
    
    
    CCFiniteTimeAction*  sequence = CCSequence::create(CCScaleTo::create(0.5f, 1.5f), 
                                                       CCMoveBy::create(1.f, ccp(0, 400)), 
                                                       CCCallFunc::create(this,callfunc_selector(TopLayer::onRiseDone)),
                                                       CCFadeOut::create(0.1f),
                                                       NULL);
    
    
    m_pImage->runAction(sequence);
}


void TopLayer::onRiseDone()
{
    sunBlast(m_pImage->getPosition());
}



void TopLayer::sunBlast(const CCPoint &position)
{
    //删除之前的粒子效果
    if (m_emitter != NULL) {
        removeChild(m_emitter);
        CCLOG("######### %d", m_emitter->retainCount());
        m_emitter->release();
    }
    
    m_emitter = new CCParticleSystemQuad();
    m_emitter->initWithFile("PvPScene/blast.plist");
    
    m_emitter->setPosition(CCPointZero);        
    m_emitter->setPosition(m_pImage->convertToWorldSpace(CCPointZero));
    
    addChild(m_emitter, 100);    
    
    schedule(schedule_selector(TopLayer::onBlastDone), .1f);    
}


void TopLayer::onBlastDone(float dt)
{    
    if (m_emitter->getParticleCount() == 0) {
        unscheduleAllSelectors();
        xSM->forward(ST_PLan, PLANSCENE_TYPE_PVP, NULL);
    }    
}


void PvPScene::showScore()
{
    UILabel *label = (UILabel*)(m_pRootLayer->getChildByTag(TUserName));

    label->setText(xUser->pUserInfo->userName.c_str());
    
    UILabelAtlas *uScore = (UILabelAtlas*)(m_pRootLayer->getChildByTag(TUserScore));

    uScore->setStringValue(ccitoa(xUser->totalScore, temp));
}


int PvPScene::getRandomHeadID()
{
    return 1001 + rand()%11;
}


//
//int i = getRank();
//if (i != 0) {
//    m_uRank->setStringValue(ccitoa(i, temp));
//    m_uRank->setVisible(true);
//}

//
//RankingUser **rankData = UserData::Instance()->rankingList;
//
//CCLabelTTF *labelTTF = (CCLabelTTF*)cell->getChildByTag(TListName);
////labelTTF->setString(rankData[index]->userName.c_str());
//labelTTF->setString(ccstrToIgnore(rankData[index]->userName, NAME_LEN));
//
//CCLabelAtlas *labelScore = (CCLabelAtlas*)cell->getChildByTag(TListScore);
//labelScore->setString(ccitoa(rankData[index]->score, temp));

