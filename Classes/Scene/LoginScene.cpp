//
//  LoginScene.cpp
//  SingleEye
//
//  Created by gwsoft on 1/7/14.
//
//

#include "LoginScene.h"
#include "PvPScene.h"



#define CC_CAST(type)       (static_cast<type*>)
#define SPACE               (10)
#define TOUCH_DELAY         (0.5f)


///////////////////////////////////////////// List



typedef enum LoginTag
{
    TBack = 43,
    TLogin = 49,
    TInput = 53,
    TInputTouch = 199,
    
    
    LoginTag_COUNT
}LoginTag;


LoginScene::LoginScene()
{   
    m_bIMEOn = false;
    m_bBlock = false;
}


CCScene* LoginScene::scene()
{
    CCScene* scene = NULL;
    do {
        scene = CCScene::create();
        CC_BREAK_IF(!scene);
        
        LoginScene *layer = LoginScene::create();
        scene->addChild(layer);
    } while (0);
    
    return scene;
}


bool LoginScene::init()
{
    if (CCLayer::init()) {
        m_pUILayer = UILayer::create();
        
        addChild(m_pUILayer);

        //不要在init里面去调用网络命令
        //(NetWorker::Instance())->getNetSender()->DO_CMD_REQ_RANKING(this);
    }
    
    return true;
}


void LoginScene::onEnter()
{
    CCLayer::onEnter();    
    
    m_pRootLayer = (UIPanel*)(GUIReader::shareReader()->widgetFromJsonFile("PvPScene/PvPScene_Login.ExportJson"));
    HelpMethod::adapt(m_pRootLayer, "bg_2.png");
    
    //m_pRootLayer->setTouchEnable(true);
    
    m_pUILayer->addWidget(m_pRootLayer);
    
    //Touch
    m_pUILayer->setTouchEnabled(true);
    m_pUILayer->setTouchPriority(-1);
    
    
    //事件注册
    HelpMethod::initUIButton(TBack, toucheventselector(LoginScene::onButton), m_pRootLayer, this);
    HelpMethod::initUIButton(TLogin, toucheventselector(LoginScene::onButton), m_pRootLayer, this);
    m_pInput = HelpMethod::initUITextField(TInput, textfieldeventselector(LoginScene::textFieldEvent), m_pRootLayer, this);     
    
    m_UserName = CCUserDefault::sharedUserDefault()->getStringForKey(UUserName);
    m_sInput= m_UserName;
    
    m_pInput->setText(m_UserName.c_str());
    m_pInput->setPlaceHolder(xStr("place_holder"));
    
    m_uImageForInput = (UIPanel*)m_pRootLayer->getChildByTag(TInputTouch);
    m_uImageForInput->addTouchEventListener(this, toucheventselector(LoginScene::onButton));
    m_uImageForInput->setTouchEnabled(true);    
    MsgSystem::Instance()->RegisterObjAndHandler(this, MSGSYS_HANLE_MSG(LoginScene::handleMsg));
}


void LoginScene::onExit()
{
    removeAllChildrenWithCleanup(true);
    CCLayer::onExit();
    MsgSystem::Instance()->ClearObjHandler(this);
}


bool LoginScene::handleMsg(const Message &msg)
{
    CCLOG("NetTest::HandleMsg msgType= %d", msg.m_nMsgType);
    switch (msg.m_nMsgType)
    {
        case CMD_RES_REGISTER_USERNAME:
        {
            if(UserData::Instance()->isSuccess)
            {
                xUser->pUserInfo->userName = m_pInput->getStringValue();
                
                CCUserDefault::sharedUserDefault()->setStringForKey(UUserName, m_pInput->getStringValue());
                
                xSM->forward(ST_PVP);
            }
            else
            {
                xDialog->show(xUser->message.c_str());
            }
            
            break;
        }
        default:
            break;
    }
    return true;
}


void LoginScene::onButton(CCObject* pSender, TouchEventType type)
{
    if(type != TOUCH_EVENT_ENDED)
    {
        return;
    }
    
    
    UIWidget *widget = (UIWidget*)pSender;
    
    int iTag = widget->getTag();
    
    CCLOG("********** %d", iTag);
    
    switch (iTag) {
        case TInputTouch:
        {
            //只负责[打开]输入键盘                                                
            m_pInput->setAttachWithIME(true);
            m_pInput->attachWithIME();
        }
            break;
        case TLogin:
        {
            const char* input = m_pInput->getStringValue();
            if(m_UserName.empty() || strcmp(m_UserName.c_str(), input) != 0)
            {
                if (strcmp(input, "") == 0) {
                    xDialog->show(xStr("input_tip"));
                }
                else
                {
                    //注册战斗昵称
                    CCLOG("********** %s", m_pInput->getStringValue());
                    xNet->DO_CMD_REQ_REGISTER_USERNAME(this, m_pInput->getStringValue(), xUser->pUserInfo->gemstone);
                }
            }
            else
            {
                xSM->forward(ST_PVP);
            }
            
            break;

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


void LoginScene::textFieldEvent(CCObject *pSender, TextFiledEventType type)
{
    UITextField* textField = dynamic_cast<UITextField*>(pSender);
    switch (type)
    {
        case TEXTFIELD_EVENT_ATTACH_WITH_IME:
        {
            xBlock->block(blockeventselector(LoginScene::onBlock), this);                 
            scheduleOnce(schedule_selector(LoginScene::imeOn), TOUCH_DELAY);            
        }
            break;            
        case TEXTFIELD_EVENT_DETACH_WITH_IME:
        {
            scheduleOnce(schedule_selector(LoginScene::imeOff), TOUCH_DELAY);          
            
        }
            break;            
        case TEXTFIELD_EVENT_INSERT_TEXT:
        {
            const char * txt = textField->getStringValue();
            
            if (strlen(txt) > 3*7) {
                textField->setText(m_sInput.c_str());
            }
            else
            {
                m_sInput = string(txt);
            }
        
        }
            break;
        case TEXTFIELD_EVENT_DELETE_BACKWARD:
        {
            const char * txt = textField->getStringValue();
            m_sInput = string(txt);
        }
            break;
        default:
            break;
    }
}


bool LoginScene::onBlock(CCTouch *pTouch, CCEvent *pEvent, TouchType type)
{   
    if (type == TOUCH_ENDED) {
        if (m_bIMEOn == true)
        {
            m_bIMEOn = false;
            m_pInput->setDetachWithIME(true);                
            m_pInput->attachWithIME();
        }
    }
    
    return true;
}


void LoginScene::imeOn(float dt)
{
    m_bIMEOn = true;
}


void LoginScene::imeOff(float dt)
{
    m_bIMEOn = false;
    xBlock->unblock();
}

