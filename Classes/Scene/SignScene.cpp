//
//  SignScene.cpp
//  SingleEye
//
//  Created by gwsoft on 1/7/14.
//
//


#include "Date.h"
#include "SignScene.h"
#include "PvPScene.h"



#define CC_CAST(type)     (static_cast<type*>)
#define SPACE             (10)
#define BEAT_LEN          (48)

///////////////////////////////////////////// List



typedef enum SignTag
{
    TBack = 43,
    
    TProp = 31,
    TSignMark = 32,
        
    TWeekStart = 100,
    
    TSign = 375,
    
    TPrizeCount = 422,
    
    TWeekDayName = 448,
    
    TContinueLabel = 478,
    TSignCount = 479,
    
    
    SignTag_COUNT
}SignTag;


SignScene::SignScene()
{
}


CCScene* SignScene::scene()
{
    CCScene* scene = NULL;
    do {
        scene = CCScene::create();
        CC_BREAK_IF(!scene);
        
        SignScene *layer = SignScene::create();
        
        scene->addChild(layer);
    } while (0);
    
    return scene;
}


bool SignScene::init()
{
    if (CCLayer::init()) {
        HelpMethod::getWeekDay(&m_iWeekDay, &m_iDay);
        
#ifdef SIGN_DEBUG        
        int iAdd = xUGInt("test_sign");
        
        Date date(true);       
        Date temp = date + iAdd;
        m_iDay = temp.day;
        
        for (int i = 0; i < iAdd; i++) {
            cccycleNum(7, true, &m_iWeekDay);
        }        
        
        iAdd++;
        xUSInt("test_sign", iAdd);
#endif                        
        //明天
        //m_iWeekDay = 1;
       // m_iDay = 11;
        
        m_pUILayer = UILayer::create();
        
        addChild(m_pUILayer);                
    }
    
    return true;
}


void SignScene::onEnter()
{
    CCLayer::onEnter();
    
    m_pRootLayer = (UIPanel*)(GUIReader::shareReader()->widgetFromJsonFile("SignScene/SignScene_1.ExportJson"));
    HelpMethod::adapt(m_pRootLayer, "bg_2.png");
    
    //m_pRootLayer->setTouchEnable(true);
    
    m_pUILayer->addWidget(m_pRootLayer);
    HelpMethod::addTitle(m_pUILayer, "qdao_01.png");
    
    //Touch
    m_pUILayer->setTouchEnabled(true);
    m_pUILayer->setTouchPriority(-1);
    
    
    //获得imageview    
    m_uLabel = xImageView(TContinueLabel);
    m_uSignCount = (UILabelAtlas*)(m_pRootLayer->getChildByTag(TSignCount));
    m_uPrizeCount = (UILabelAtlas*)(m_pRootLayer->getChildByTag(TPrizeCount)); 
    
    //事件注册
    HelpMethod::initUIButton(TBack, toucheventselector(SignScene::onButton), m_pRootLayer, this);
    HelpMethod::initUIButton(TSign, toucheventselector(SignScene::onButton), m_pRootLayer, this);
    
    initShow();
    
    this->setTouchEnabled(true);
    MsgSystem::Instance()->RegisterObjAndHandler(this, MSGSYS_HANLE_MSG(SignScene::handleMsg));
    
    if (m_iWeekDay == 6) {
        if (DATAPOOL->m_vSign.empty() == false) {
            for (int i = 0; i < 7; i++) {               
                DATAPOOL->m_vSign.at(i).bSign = false;
            }
        }
        
        //再保存一下
        DATAPOOL->writeFileForSign(DATAPOOL->m_vSign);
    }                
}


void SignScene::onExit()
{
    CCLayer::onExit();

    //临时fix bug, 如果是周日, 清除所有的bSign
    if (m_iWeekDay == 6) {
        if (DATAPOOL->m_vSign.empty() == false) {
            for (int i = 0; i < 7; i++) {               
                DATAPOOL->m_vSign.at(i).bSign = false;
            }
        }
    }        

    HelpMethod::removeUnusedResourceAtExit(this);
    
    //再保存一下
    DATAPOOL->writeFileForSign(DATAPOOL->m_vSign);
}


bool SignScene::handleMsg(const Message &msg)
{
    CCLOG("NetTest::HandleMsg msgType= %d", msg.m_nMsgType);
    switch (msg.m_nMsgType)
    {
        case CMD_RES_SIGNIN:
        {
            //数据存档
            DATAPOOL->m_vSign.at(m_iWeekDay).bSign = true;
            DATAPOOL->m_vSign.at(m_iWeekDay).iDay = m_iDay;
            DATAPOOL->writeFileForSign(DATAPOOL->m_vSign);
            
            //界面
            m_iSigned[m_iWeekDay]->setVisible(true);

#ifdef SIGN_DEBUG
            if (true)
#else            
            if (xUser->isSuccess)
#endif                
            {
                m_iContinues++;
                DATAPOOL->setContinueSign(m_iContinues);
                addProp();
                checkBigPrize();
                xSocial->statByUserID(SSign);

                //保存最后一次签到
                saveLastSign();         
                
 
                xDialog->showYes(xStr("sign_success"), dialogeventselector(SignScene::onDialog), this);
            }
            else
            {
                xDialog->show(xStr("repeat_sign"), Dialog_Yes, NULL, NULL);
            }
            
            showContinueSign();
        }
            break;
            
        default:
            break;
    }
    
    return true;
}



void SignScene::onButton(CCObject* pSender, TouchEventType type)
{
    if(type != TOUCH_EVENT_ENDED)
    {
        return;
    }
    
    xData->playKeytone();    
    
    UIWidget *widget = (UIWidget*)pSender;
    
    int iTag = widget->getTag();
    
    CCLOG("********** %d", iTag);
    
    switch (iTag) {
        case TSign:
        {
            //签到
            if (!m_iSigned[m_iWeekDay]->isVisible()) {
                xNet->DO_CMD_REQ_SIGNIN(this, xUser->pUserInfo->gemstone);
            }
            else
            {
                xDialog->show(xStr("repeat_sign"));
            }
        }
            break;
        case TBack:
        {            
            if (xSM->getLastSceneTag() != ST_Huodong) {
                xSM->enableTransit();
                xSM->forward(ST_Home);
            }
            else
            {
                xSM->back();
            }            
        }
            break;
        default:
        {
        }
            break;
    }
}


/////////////////////////////////////////////   ListStart
void SignScene::onDialog(int id, int iTag)
{
    switch (iTag) {
        case 0:
            xSM->enableTransit();
            xSM->forward(ST_Home);            
            break;
            
        default:
            break;
    }
}


void SignScene::initShow()
{
    char temp[10];
    
    int iYesterday = m_iWeekDay;
    cccycleNum(7, false, &iYesterday);
   
    int iTomorrow = m_iWeekDay;
    cccycleNum(7, true, &iTomorrow);
        
    sprintf(temp, "qdao_0%d.png", m_iWeekDay + 2);    
    UIImageView *uWeekName = xImageView(TWeekDayName);
    uWeekName->setTexture(temp, tPlist);
    
    m_iContinues = DATAPOOL->getContinueSign();    
        
    DATAPOOL->readFileForSign(DATAPOOL->m_vSign);    
        
    //默认值
    refreshVector();
    
    if (checkLastSign(m_iDay, m_iWeekDay) == false) {    
        m_iContinues = 0;
    }   
    
    for (int i = 0; i < 7; i++) {
        m_uBG[i] = dynamic_cast<UIImageView*>(m_pRootLayer->getChildByTag(TWeekStart + i));
        m_iProp[i] = (UIImageView*)m_uBG[i]->getChildByTag(TProp);
        m_iSigned[i] = (UIImageView*)m_uBG[i]->getChildByTag(TSignMark);                
        
                
        showProp(m_iProp[i], i);
        
        if (i < m_iWeekDay) {
            if (xData->m_vSign.at(i).bSign == false) {
                m_uBG[i]->setTexture("qdao_12.png", tPlist);
                m_iSigned[i]->setTexture("qdao_21.png", tPlist);
            }
        }
        else if(i == m_iWeekDay)
        {
            if (xData->m_vSign.at(i).bSign == true) {
                m_iSigned[i]->setVisible(true);
            }        
            else
            {
                m_iSigned[i]->setVisible(false);                
            }
        }
        else
        {
            //fix bug
            xData->m_vSign.at(i).bSign = false;
            m_iSigned[i]->setVisible(false);          
        }
    }                
    
    DATAPOOL->setContinueSign(m_iContinues);
    
    showContinueSign();    
}


bool SignScene::checkLastSign(int iTodayDay, int iTodayWeek)
{    
    bool bRet = false;
    int iLastSignDay = xUGInt(LastSignD);
    int iLastSignWeek = xUGInt(LastSignW);    
    
    //如果今天已签到, 直接返回true;
    if (iLastSignDay == iTodayDay && iLastSignWeek == iTodayWeek) {
        bRet = true;
    }
    //特殊情况
    else if (iTodayDay == 1) {
        if (iLastSignDay >= 28 && iLastSignWeek == cccycleNum(7, false, &iTodayWeek)) {
            bRet = true;
        }
    }
    else
    {
        if (iLastSignDay == iTodayDay - 1 && iLastSignWeek == cccycleNum(7, false, &iTodayWeek)) {
            bRet = true;
        }
    }
    
    return bRet;
}


void SignScene::showContinueSign()
{            
    if (m_iContinues > 4)
    {
        m_uLabel->setTexture("qdao_25.png", tPlist);
        m_uPrizeCount->setStringValue(ccitoa(20, temp));        
    }
    else if (m_iContinues > 2) {
        m_uLabel->setTexture("qdao_24.png", tPlist);
        m_uPrizeCount->setStringValue(ccitoa(10, temp));
    }
    else
    {       
        m_uLabel->setTexture("qdao_23.png", tPlist);
        m_uPrizeCount->setStringValue(ccitoa(5, temp));                
    }        
        
    m_uSignCount->setStringValue(ccitoa(m_iContinues, temp));
}



/*
获取类型
星期一 炸弹
星期二 狂暴药剂
星期三 冰晶
星期四 恢复药剂
星期五 炸弹
星期六 狂暴药剂
星期日 冰晶
*/

Prop_Type SignScene::showProp(UIImageView* uImage, int iWeekDay)
{
    Prop_Type iTag = Prop_Type_None;
    const char *name;
    
    switch (iWeekDay) {
        case 0:
        case 4:
            iTag = Prop_Type_ZhaDan;
            name = "zhadan.png";
            break;
        case 1:
        case 5:
            iTag = Prop_Type_KuangBaoYao;
            name = "kuangbaoyao.png";
            break;
            break;
        case 2:
        case 6:
            iTag = Prop_Type_BingJing;
            name = "bingjing.png";
            break;
        case 3:
            iTag = Prop_Type_HuiFuYao;
            name = "huifuyao.png";
            break;
        default:
            break;
    }
    
    
    if (uImage != NULL) {
        uImage->setTexture(name, UI_TEX_TYPE_PLIST);
    }

    return iTag;
}


void SignScene::addProp()
{
    //金币
    DATAPOOL->changeBagGoldNum(1000);
    
    Prop_Type eType = showProp(NULL, m_iWeekDay);
    OutfitRecord *rcd = DATAPOOL->getOutfitRecord(eType);
    rcd->m_nNum++;
    DATAPOOL->setOutfitRecord(eType, *rcd);
}


void SignScene::checkBigPrize()
{       
    if (m_iContinues == 3) {
        DATAPOOL->changeBagGemNum(5);
    }
    else if (m_iContinues == 5)
    {
        DATAPOOL->changeBagGemNum(10);        
    }
    else if(m_iContinues == 7)
    {
        m_iContinues = 0;
        xData->setContinueSign(m_iContinues);        
        DATAPOOL->changeBagGemNum(20);        
    }                
}


void SignScene::saveLastSign()
{
    xUSInt(LastSignW, m_iWeekDay);
    xUSInt(LastSignD, m_iDay);
}


void SignScene::refreshVector()
{
    if (DATAPOOL->m_vSign.empty()) {
        //初始化数组元素
        for (int i = 0; i < 7; i++) {
            GSign sign;
            
            sign.bSign = false;
            sign.iDay = 0;
            
            DATAPOOL->m_vSign.push_back(sign);
        }
        
        m_iContinues = 0;
    }
    
          
    //检查今天是否处于Vector中, 如果iDay不等于Vector(0).iDay, 就刷新(如何刷新)
    if (DATAPOOL->m_vSign.at(m_iWeekDay).iDay != m_iDay) {
        
        Date date(true);  
        for (int i = 0; i < 7; i++) {
            if (i >= m_iWeekDay) {
                Date temp = date + (i - m_iWeekDay);
                DATAPOOL->m_vSign.at(i).iDay = temp.day;
            }        
        }                                
    }    
}
