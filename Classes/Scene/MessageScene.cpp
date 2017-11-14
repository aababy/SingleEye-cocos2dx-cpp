//
//  MessageScene.cpp
//  SingleEye
//
//  Created by gwsoft on 1/7/14.
//
//

#include "MessageScene.h"
#include "PvPScene.h"



#define CC_CAST(type)     (static_cast<type*>)
#define SPACE             (10)
#define BEAT_LEN          (90)

///////////////////////////////////////////// List



typedef enum MessageTag
{
    TBack = 43,
    TText = 82,
    TRevenge = 91,
    
    TPanelList = 125,
    
    MessageTag_COUNT
}MessageTag;


MessageScene::MessageScene()
{
    m_pList = NULL;
    m_iMsgCount = CDataPool::sharedDataPool()->m_vSystem.size();
    m_nCount = 0;
}


CCScene* MessageScene::scene()
{
    CCScene* scene = NULL;
    do {
        scene = CCScene::create();
        CC_BREAK_IF(!scene);
        
        MessageScene *layer = MessageScene::create();
        
        scene->addChild(layer);
    } while (0);
    
    return scene;
}


bool MessageScene::init()
{
    if (CCLayer::init()) {
        m_pUILayer = UILayer::create();
        
        addChild(m_pUILayer);
                
        this->setTouchEnabled(true);
    }
    
    return true;
}


void MessageScene::onEnter()
{
    CCLayer::onEnter();
    
    m_pRootLayer = (UIPanel*)(GUIReader::shareReader()->widgetFromJsonFile("PvPScene/PvPScene_3.ExportJson"));
    HelpMethod::adapt(m_pRootLayer, "bg_2.png");
    
    //m_pRootLayer->setTouchEnable(true);
    
    m_pUILayer->addWidget(m_pRootLayer);
    HelpMethod::addTitle(m_pUILayer, "xxi_01.png");
    
    //Touch
    m_pUILayer->setTouchEnabled(true);
    m_pUILayer->setTouchPriority(-1);
    
    
    //事件注册
    HelpMethod::initUIButton(TBack, toucheventselector(MessageScene::onButton), m_pRootLayer, this);    
    
    createList();    
}


void MessageScene::onExit()
{
    CCLayer::onExit();
    
    CDataPool::sharedDataPool()->makeAllMessageToOld();    
    HelpMethod::removeUnusedResourceAtExit(this);
}


void MessageScene::setCount(int iMsgCount)
{
    m_iMsgCount = iMsgCount;
}


bool MessageScene::handleMsg(const Message &msg)
{
    CCLOG("NetTest::HandleMsg msgType= %d", msg.m_nMsgType);
    switch (msg.m_nMsgType)
    {
        case CMD_RES_FIGHT:
        {
            xSM->forward(ST_PLan, PLANSCENE_TYPE_PVP, NULL);
        }
            break;
            
        default:
            break;
    }
    
    return true;
}



void MessageScene::onButton(CCObject* pSender, TouchEventType type)
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
        case TRevenge:
        {
            xDialog->show(xStr("fight_tip"), "ksu_08.png", dialogeventselector(MessageScene::onDialog), this);
            
            CCInteger* i = (CCInteger*)widget->getUserObject();            
            m_userId = xData->m_vSystem.at(i->getValue()).iData;
        }
            break;
        case TBack:
        {
            xSM->back();
        }
            break;
        default:
        {
        }
            break;
    }
}




void MessageScene::createList()
{        
    UIPanel *panel = (UIPanel*)m_pRootLayer->getChildByTag(125);
    
    // Create the list view
    m_pList = UIListViewEx::create();
    
    m_pList->setDirection(SCROLLVIEW_DIR_VERTICAL);
    m_pList->setTouchEnabled(true);
    m_pList->setBounceEnabled(true);
    m_pList->setSize(panel->getSize());
    m_pList->setItemsMargin(26.f);
    
    //m_pList->addEventListenerListViewEx(this, listviewexeventselector(BossScene::selectedItemEvent));
    panel->addChild(m_pList);
    
    
    Layout* default_item = (UIPanel*)(GUIReader::shareReader()->widgetFromJsonFile("PvPScene/MessageScene_List.ExportJson"));        
    //default_item->setTouchEnabled(true);  
    
    // set model
    m_pList->setItemModel(default_item);
    
    for (int i = 0; i < m_iMsgCount; ++i)
    {
        m_pList->pushBackDefaultItem();
    }
    
    m_pList->setGravity(LISTVIEW_GRAVITY_CENTER_VERTICAL);
    
    CCPoint point;
    
    //填充内容
    for (int i = 0; i < m_iMsgCount; i++) {
        UIPanel *rootPanel = (UIPanel*) m_pList->getItem(i);
        UIPanel *panel = (UIPanel*)rootPanel->getChildByTag(TText); 

        UIButton *uRevenge = (UIButton *)rootPanel->getChildByTag(TRevenge);
        
        UITextArea *uText = UITextArea::create();   
        uText->setFontSize(28);
        
        
        if (xData->m_vSystem.at((m_iMsgCount - 1) - i).bNew == true) {
            UIImageView *uMail = (UIImageView*)rootPanel->getChildByTag(88);
            
            uMail->setTexture("xxi_04.png", tPlist);
        }
        
        
        if (xData->m_vSystem.at((m_iMsgCount - 1) - i).iType == 0) {
            uRevenge->setVisible(false);
            uText->setTextAreaSize(panel->getSize());
            uText->setPosition(ccp(panel->getSize().width/2, panel->getSize().height/2));   
        }
        else
        {
            uText->setTextAreaSize(CCSizeMake(panel->getSize().width - uRevenge->getSize().width *1.3f, panel->getSize().height));            
            uText->setPosition(ccp((panel->getSize().width - uRevenge->getSize().width *1.3f)/2, panel->getSize().height/2)); 
            
            uRevenge->setTouchEnable(true);
            uRevenge->addTouchEventListener(this, toucheventselector(MessageScene::onButton));
            
            CCInteger *integer = CCInteger::create(i);            
            uRevenge->setUserObject(integer);
        }                
        

        uText->setTextVerticalAlignment(kCCVerticalTextAlignmentCenter);
        panel->addChild(uText);
        
        
        string str = string(xData->m_vSystem.at((m_iMsgCount - 1) - i).msg);                 
        uText->setText(str.c_str());        
    }                    
}


void MessageScene::update(float delta)
{
    //m_pListEx->getChildrenPosition();
	//CCLOG("********** %f", m_pListEx->getChildrenPosition());
}


/////////////////////////////////////////////   ListStart

	
void MessageScene::onDialog(int id, int iTag)
{
    switch (iTag) {
        case 0:
            (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_FIGHT(this, m_userId, 0, xUser->pUserInfo->gemstone, xUserInfo->mercemaryNum, xUserInfo->listMercemary);
            break;
            
        default:
            break;
    }
}
