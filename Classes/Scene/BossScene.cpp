//
//  BossScene.cpp
//  SingleEye
//
//  Created by gwsoft on 1/7/14.
//
//

#include "BossScene.h"


#define CC_CAST(type)     (static_cast<type*>)
#define SPACE             (10)
#define BEAT_LEN          (60)

///////////////////////////////////////////// List



typedef enum BossTag
{
    TBack = 43,
    TChkRank = 129,    
    TBoss = 130,
    
    TInspire = 140,
    TPrize = 141,
    TStart = 142,    
    
    TBossChallenge = 144,
    TRank = 157,
            
    TBossBG = 169, 
    
    TReport1 = 401,    
    TName1  = 411,
    THP1  = 421,    
    
    TAttackAdd = 586,
    TDefenseAdd = 587,    
    
    TFightReportStart = 590,
    
    TMyRank = 622,
    TMyRankRise = 623,    
    
    TInspireCount = 689,
        
    TLRanking = 701,
    TLRankNum = 702,
    TLName = 703,
    TLScore = 704,
    
    TCurHP = 731,
    TTotalHP = 732,
    
    THPProgress = 792,
    
    TMyRankPanel = 1000,
    TMyRankRisePanel = 1001,
    
    
    BossTag_COUNT
}BossTag;


BossScene::BossScene()
{
    m_pList = NULL;
    m_aBoss = NULL;
    m_bOnTime = false;
    m_bBossInfo = false;
    m_nCount = 0;
}


CCScene* BossScene::scene()
{
    CCScene* scene = NULL;
    do {
        scene = CCScene::create();
        CC_BREAK_IF(!scene);
        
        BossScene *layer = BossScene::create();
        
        scene->addChild(layer);
    } while (0);
    
    return scene;
}


bool BossScene::init()
{
    if (CCLayer::init()) {
        m_pUILayer = UILayer::create();
        
        addChild(m_pUILayer);
        
        this->setTouchEnabled(true);
    }
    
    return true;
}


void BossScene::onEnter()
{
    CCLayer::onEnter();
        
    m_pRootLayer = (UIPanel*)(GUIReader::shareReader()->widgetFromJsonFile("BossScene/BossScene_1.ExportJson"));
    HelpMethod::adapt(m_pRootLayer, "bg_2.png");
    
    m_pUILayer->addWidget(m_pRootLayer);
    
    m_pRootLayer->setTouchEnabled(true);
    //Touch
    m_pUILayer->setTouchEnabled(true);
    //m_pUILayer->setTouchPriority(-1);        
    
    //事件注册
    HelpMethod::initUIButton(TBack, toucheventselector(BossScene::onButton), m_pRootLayer, this);
    m_uInspire = HelpMethod::initUIButton(TInspire, toucheventselector(BossScene::onButton), m_pRootLayer, this);
    HelpMethod::initUIButton(TStart, toucheventselector(BossScene::onButton), m_pRootLayer, this);    
    
    m_uPrize = HelpMethod::initUIButton(TPrize, toucheventselector(BossScene::onButton), m_pRootLayer, this);
    
    m_pChkBoss = HelpMethod::initUICheckBox(TBoss, checkboxselectedeventselector(BossScene::onCheckBox), m_pRootLayer, this);
    m_pChkBoss->setSelectedState(true);        
    m_pChkRank = HelpMethod::initUICheckBox(TChkRank, checkboxselectedeventselector(BossScene::onCheckBox), m_pRootLayer, this);
    
    m_pBossChallenge = (UIPanel*)m_pRootLayer->getChildByTag(TBossChallenge);        
    m_pRank = (UIPanel*)m_pRootLayer->getChildByTag(TRank);
    
    m_pBossChallenge->setVisible(true);
    m_pRank->setVisible(true);
        
    m_pBossChallenge->setEnabled(false);
    m_pRank->setEnabled(false);   
    
    
    m_pGold = new CMoneyItem(MONEY_ITEM_TYPE_GOLD, true);
    m_pGold->autorelease();
    addChild(m_pGold, BUYGEMSCENE_ZORDER_PROP);
    
    m_pGem = new CMoneyItem(MONEY_ITEM_TYPE_GEM, true);
    m_pGem->autorelease();
    addChild(m_pGem, BUYGEMSCENE_ZORDER_PROP); 
    
    //获取boss信息
    MsgSystem::Instance()->RegisterObjAndHandler(this, MSGSYS_HANLE_MSG(BossScene::handleMsg));
    xNet->DO_CMD_REQ_BOSS_INFO(this, xUser->pUserInfo->gemstone);
}


void BossScene::onExit()
{
    CCLayer::onExit();
    
    CCArmatureDataManager::purge(); 
    
    MsgSystem::Instance()->ClearObjHandler(this);
}


void BossScene::onMoneyItem(EMoneyItemType m_nType)
{
    if (xData->getAttackAdd() != 0) {
        xDialog->showYesNo(xStr("inspire_tip"), dialogeventselector(BossScene::onDialog), this);
        
        if (m_nType == MONEY_ITEM_TYPE_GOLD) {
            xDialog->setDialogID(12);
        }
        else
        {
            xDialog->setDialogID(13);
        }        
    }
    else
    {
        if (m_nType == MONEY_ITEM_TYPE_GOLD) {
            xSM->forward(ST_BuyGold);
        }
        else
        {
            xSM->forward(ST_BuyGem);
        }              
    }
}


void BossScene::setCount(int iMsgCount)
{
    m_iMsgCount = iMsgCount;
}


bool BossScene::handleMsg(const Message &msg)
{
    CCLOG("NetTest::HandleMsg msgType= %d", msg.m_nMsgType);
    switch (msg.m_nMsgType)
    {
        case CMD_RES_BOSS_INFO:
        {
            //显示信息
            m_bBossInfo = true;
            
            showInfo();            
            m_pBossChallenge->setEnabled(true);                                
        }
            break;
        case CMD_RES_BOSS_RANKING:
        {
            if (m_pList == NULL) {
                createList();
            }
            m_pRank->setEnabled(true);
        }
            break;       
        case CMD_RES_BOSS_BEGIN_FIGHT:
        {
            if (xUser->isBegin == 1) {                
                m_bOnTime = true;                
                if (m_bTestFight == false) {
                    //切换Plan界面
                    xData->setAttackAdd(0);
                    xData->setDefenseAdd(0);                     
                    xSM->forward(ST_PLan, PLANSCENE_TYPE_BOSS, NULL);
                }
                else
                {
                    onButton(m_uInspire, TOUCH_EVENT_ENDED);
                }
            }
            else
            {
                xDialog->show(xUser->message.c_str());
            }
        }
            break;
        case CMD_RES_BOSS_REWARD:
        {
            xData->m_bGetBossPrize = false;
            
            if (xUser->isReward == 1) {                
                getPrize();
            }   
            else
            {
                xDialog->show(xStr("cannot_get_prize"));
            }
        }
            break;            
            
        default:
            break;
    }
    
    return true;
}



void BossScene::onButton(CCObject* pSender, TouchEventType type)
{
    if(type != TOUCH_EVENT_ENDED)
    {
        return;
    }
    
    xData->playKeytone();    
    
    UIWidget *widget = (UIWidget*)pSender;
    
    int iTag = widget->getTag();    
    
    switch (iTag) {
        case TBack:
        {
            if (xData->getAttackAdd() != 0) {
                xDialog->showYesNo(xStr("inspire_tip"), dialogeventselector(BossScene::onDialog), this);
                xDialog->setDialogID(11);
            }
            else
            {
                xSM->back();
            }                        
        }
            break;
        case TStart:
        {
            if (m_pBossChallenge->isEnabled()) {           
                m_bTestFight = false;
                xNet->DO_CMD_REQ_BOSS_BEGIN_FIGHT(this);
            }
            else
            {
                xDialog->show(xStr("no_net_connect"));
            }
        }
            break;            
        case TPrize:
        {
            if (!xData->m_bGetBossPrize) {
                xDialog->show(xStr("cannot_get_prize"));
            }
            else
            {
                xNet->DO_CMD_REQ_BOSS_REWARD(this, xUserInfo->gemstone);
            }                        
        }
            break;            
        case TInspire:
        {            
            if (m_bOnTime == false) {
                if (m_pBossChallenge->isEnabled()) {   
                    m_bTestFight = true;
                    xNet->DO_CMD_REQ_BOSS_BEGIN_FIGHT(this);
                }
                else
                {
                    xDialog->show(xStr("no_net_connect"));
                }
            }
            else
            {
                if (xData->changeBagGoldNum(-500) == true) {                
                    m_pGold->setNum(xData->gold);                
                    xData->setAttackAdd(xData->getAttackAdd() + 3);
                    xData->setDefenseAdd(xData->getDefenseAdd() + 3); 
                    
                    if (xData->getAttackAdd() >= 30) {
                        m_uInspire->disable();
                    }
                    
                    showInspire();
                }              
                else
                {
                    xDialog->showYesNo(xStr("lack_of_gold"), dialogeventselector(BossScene::onDialog), this);
                    xDialog->setDialogID(10);
                }
            }                        
        }
            break;               
        default:
        {
        }
            break;
    }
}


void BossScene::onCheckBox(CCObject *pSender, CheckBoxEventType type)
{   
    xData->playKeytone();    
    
    UICheckBox *check = (UICheckBox*)pSender;
    
    switch (type)
    {
            //暂时不处理 UNSELECTED
        case CHECKBOX_STATE_EVENT_UNSELECTED:
            if (check->getTag() == TBoss) {
                m_pChkBoss->setSelectedState(true);
            }
            else
            {
                m_pChkRank->setSelectedState(true);
            }
            
            break;
            
        case CHECKBOX_STATE_EVENT_SELECTED:
            if (check->getTag() == TBoss) {
                m_pChkRank->setSelectedState(false);
                
                if (m_bBossInfo) {
                    m_pBossChallenge->setEnabled(true);
                }                
                m_pRank->setEnabled(false);                
            }
            else
            {
                m_pChkBoss->setSelectedState(false);                	
                m_pBossChallenge->setEnabled(false);
                                
                if (m_pList == NULL) {
                    xNet->DO_CMD_REQ_BOSS_RANKING(this, xUserInfo->gemstone);
                }                
                else
                {
                    m_pRank->setEnabled(true);    
                }
            }
            
            break;
            
        default:
            break;
    }
}


void BossScene::update(float delta)
{
    //m_pListEx->getChildrenPosition();
	//CCLOG("********** %f", m_pListEx->getChildrenPosition());
}


void BossScene::onDialog(int id, int iTag)
{
    switch (iTag) {
        case 0:
            if (id == 10) {
                xSM->forward(ST_BuyGold);
            }
            else if (id == 11 || id == 12 || id == 13)
            {
                //退回金币
                if (xData->getAttackAdd() != 0)
                {
                    xData->changeBagGoldNum(xData->getAttackAdd()/3 * 500);
                    xData->setAttackAdd(0);
                    xData->setDefenseAdd(0);                    
                }
                
                switch (id) {
                    case 11:
                        xSM->back();
                        break;
                    case 12:
                        xSM->forward(ST_BuyGold);
                        break;           
                    case 13:
                        xSM->forward(ST_BuyGem);
                        break;                          
                    default:
                        break;
                }                                
            }
            else
            {
                (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_FIGHT(this, m_userId, 0, xUser->pUserInfo->gemstone, xUserInfo->mercemaryNum, xUserInfo->listMercemary);
            }

            break;
            
        default:
            break;
    }
}


void BossScene::createList()
{        
    UIPanel *panel = (UIPanel*)m_pRootLayer->getChildByTag(653);
    
    // Create the list view
    m_pList = UIListViewEx::create();
        
    m_pList->setDirection(SCROLLVIEW_DIR_VERTICAL);
    m_pList->setTouchEnabled(true);
    m_pList->setBounceEnabled(true);
    m_pList->setSize(panel->getSize());
    m_pList->setItemsMargin(24.f);
    
    m_pList->addEventListenerListViewEx(this, listviewexeventselector(BossScene::selectedItemEvent));
    panel->addChild(m_pList);
    
        
    Layout* default_item = (UIPanel*)(GUIReader::shareReader()->widgetFromJsonFile("BossScene/List.ExportJson"));        
    default_item->setTouchEnabled(true);          
    
    // set model
    m_pList->setItemModel(default_item);
                
    int count = xUser->rankingCount;     //数量
    for (int i = 0; i < count; ++i)
    {
        m_pList->pushBackDefaultItem();
    }
        
    m_pList->setGravity(LISTVIEW_GRAVITY_CENTER_VERTICAL);
        
    //填充内容
    for (int i = 0; i < count; i++) {
        UIPanel *rootPanel = (UIPanel*) m_pList->getItem(i);                        

        UIImageView *uRanking = (UIImageView*)rootPanel->getChildByTag(TLRanking);
        UILabelAtlas *uRankNum = (UILabelAtlas*)rootPanel->getChildByTag(TLRankNum);               
        UILabel *uName = (UILabel*)rootPanel->getChildByTag(TLName);
        UILabelAtlas *uScore = (UILabelAtlas*)rootPanel->getChildByTag(TLScore); 
        
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
        
        BossFightRanking **rankData = UserData::Instance()->rankingInfoList;
        uRankNum->setStringValue(ccitoa(i + 1, temp));
        
        if (i > 2) {            
            CCPoint point = uRankNum->getPosition();            
            uRankNum->setPosition(ccp(point.x, point.y - 14));
        }
        
        uName->setText(rankData[i]->userName.c_str());
        uScore->setStringValue(ccitoa(rankData[i]->hp, temp));   
    }                
    
    showMyRank();
}


void BossScene::selectedItemEvent(CCObject*, ListViewEventType type)
{
    switch (type)
    {
        case LISTVIEWEX_ONSELECTEDITEM:
        {
            //UIListView* listViewEx = static_cast<UIListView*>(pSender);
//                        CCLOG("select child index = %d", listViewEx->getCurSelectedIndex());
            CCLOG("select child selectedItemEvent");
        }
            break;
            
        default:
            break;
    }

}


void BossScene::addBoss()
{
    if (m_aBoss == NULL) {
        
        Enemy_Data * boss = xData->getEnemyData(xUser->bossId);
        
        sprintf(temp, "Sprites/%s.ExportJson", boss->mSourceName);
                                        
        xData->addEffectCache(temp);
        
        UIImageView *uBG = xImageView(TBossBG);                        
        m_aBoss = CCArmature::create(boss->mSourceName);            
        
        uBG->addRenderer(m_aBoss, 50);
        m_aBoss->setVisible(true);
        m_aBoss->setPosition(ccp(0, -65));
        
        if (strcmp(boss->mSourceName, "iBoss") != 0) {
            m_aBoss->getAnimation()->play("yidong");
        }
        else if (xUser->bossId == 1129)
        {
            m_aBoss->getAnimation()->play("yidong_1");
        }
        else if (xUser->bossId == 1130)
        {
            m_aBoss->setScale(0.8f);
            m_aBoss->getAnimation()->play("yidong_2");
        }                
        
        boss = NULL;
    }             
}


void BossScene::showInfo()
{
    addBoss();
    showInspire();
    
    UILabelAtlas *uCurHP = (UILabelAtlas*)m_pRootLayer->getChildByTag(TCurHP);    
    UILabelAtlas *uTotalHP = (UILabelAtlas*)m_pRootLayer->getChildByTag(TTotalHP);
        
    uCurHP->setStringValue(ccitoa(xUser->leftHp, temp));
    uTotalHP->setStringValue(ccitoa(xUser->allHp, temp));    
    
    UILoadingBar *uLoad = (UILoadingBar*)m_pRootLayer->getChildByTag(THPProgress);
    int fPercent = xUser->leftHp/xUser->allHp * 100;
    uLoad->setPercent(fPercent);
    
    //限制战报为4条
    if (xUser->fightCount > 4) {
        xUser->fightCount = 4;
    }
        
    for (int i = 0; i < 4; i++) {
        UIPanel *uPanel = (UIPanel*)m_pRootLayer->getChildByTag(TFightReportStart + i);
        UILabel *uName = (UILabel*)uPanel->getChildByTag(412);
                
        if (i < xUser->fightCount) {
            uName->setText(xUser->fightInfoList[i].c_str());
        }
        else
        {
            uPanel->setVisible(false);
        }
    }
    
}


void BossScene::showInspire()
{
    UILabelAtlas *uAttackAdd = (UILabelAtlas*)m_pRootLayer->getChildByTag(TAttackAdd);
    UILabelAtlas *uDefenseAdd = (UILabelAtlas*)m_pRootLayer->getChildByTag(TDefenseAdd);    
    UILabelAtlas *uInspireCount = (UILabelAtlas*)m_pRootLayer->getChildByTag(TInspireCount);            
    
    uAttackAdd->setStringValue(ccitoa(xData->getAttackAdd(), temp));
    uDefenseAdd->setStringValue(ccitoa(xData->getDefenseAdd(), temp));
    
    uInspireCount->setStringValue(ccitoa(xData->getAttackAdd()/3, temp));
}


void BossScene::getPrize()
{
    xData->changeBagGoldNum(xUser->gold);
    m_pGold->setNum(xData->gold);    
    
    xData->changeBagGemNum(xUser->addGemstone);
    m_pGem->setNum(xUserInfo->gemstone);
    
    OutfitRecord *rcd = DATAPOOL->getOutfitRecord(Prop_Type_ZhaDan);
    rcd->m_nNum += xUser->bombNum;
    DATAPOOL->setOutfitRecord(Prop_Type_ZhaDan, *rcd);
    
    rcd = DATAPOOL->getOutfitRecord(Prop_Type_HongZhaJi);
    rcd->m_nNum += xUser->bomberNum;
    DATAPOOL->setOutfitRecord(Prop_Type_HongZhaJi, *rcd);        
    
    string str = xStr("bprize1");
    
    if (xUser->addGemstone != 0) {                
        str.append(xStr("bprize2"));
        str.append(ccitoa(xUser->addGemstone, temp));
    }
    if (xUser->gold != 0) {
        str.append(xStr("bprize3"));
        str.append(ccitoa(xUser->gold, temp)); 
    }
    if (xUser->bombNum != 0) {
        str.append(xStr("bprize4"));
        str.append(ccitoa(xUser->bombNum, temp));         
    }
    if (xUser->bomberNum != 0) {
        str.append(xStr("bprize5"));
        str.append(ccitoa(xUser->bomberNum, temp));         
    }
        
    xDialog->show(str.c_str());            
}


void BossScene::showMyRank()
{
    UILabelAtlas *uRank = (UILabelAtlas*)m_pRootLayer->getChildByTag(TMyRank); 
    UILabelAtlas *uMyRankRise = (UILabelAtlas*)m_pRootLayer->getChildByTag(TMyRankRise); 
    UIPanel *uRankP = (UIPanel*)m_pRootLayer->getChildByTag(TMyRankPanel); 
    UIPanel *uMyRankRiseP = (UIPanel*)m_pRootLayer->getChildByTag(TMyRankRisePanel); 

    if (xUser->todayMyRanking != 0) {
        uRank->setStringValue(ccitoa(xUser->todayMyRanking, temp));        
        
        if (xUser->yestodayMyRanking != 0) {
            int iAdd = xUser->todayMyRanking - xUser->yestodayMyRanking;
            
            if (iAdd > 0) {
                uMyRankRise->setStringValue(ccitoa(iAdd, temp));
            }            
        }
        else
        {
            uMyRankRiseP->setVisible(false);
        }
    }
    else
    {
        uRankP->setVisible(false);
        uMyRankRiseP->setVisible(false);
    }
}


void BossScene::checkPrize()
{    
    if (!xData->m_bGetBossPrize) {
        m_uPrize->disable(); 
    }
}
