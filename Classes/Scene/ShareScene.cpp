//
//  ShareScene.cpp
//  SingleEye
//
//  Created by gwsoft on 1/7/14.
//
//

#include "ShareScene.h"


#define CC_CAST(type)     (static_cast<type*>)
#define SPACE             (10)
#define BEAT_LEN          (48)

///////////////////////////////////////////// List



typedef enum ShareTag
{
    TBack = 43,
    T10 = 150,
    T20,
    T30,
    T50,
    T1000,
    TShare = 176,
    TShareTxt = 178,
    
    ShareTag_COUNT
}ShareTag;


ShareScene::ShareScene()
{
    m_pList = NULL;
    
    for (int i = 0; i < 5; i++) {
        m_uImage[i] = NULL;
    }

    m_iShareLevel = 0;
}


CCScene* ShareScene::scene()
{
    CCScene* scene = NULL;
    do {
        scene = CCScene::create();
        CC_BREAK_IF(!scene);
        
        ShareScene *layer = ShareScene::create();
        
        scene->addChild(layer);
    } while (0);
    
    return scene;
}


bool ShareScene::init()
{
    if (CCLayer::init()) {
        m_pUILayer = UILayer::create();
        
        addChild(m_pUILayer);
                        
        this->setTouchEnabled(true);        
    }
    
    return true;
}


void ShareScene::onEnter()
{
    CCLayer::onEnter();    
    
    m_pRootLayer = (UIPanel*)(GUIReader::shareReader()->widgetFromJsonFile("PvPScene/ShareScene.ExportJson"));
    HelpMethod::adapt(m_pRootLayer, "bg_2.png");
    
    m_pUILayer->addWidget(m_pRootLayer);
    HelpMethod::addTitle(m_pUILayer, "fxiang_01.png");
    
    //Touch
    m_pUILayer->setTouchEnabled(true);
    m_pUILayer->setTouchPriority(-1);
    
    
    //事件注册
    for (int i = 0; i < 4; i++) {
        m_uButton[i] = HelpMethod::initUIButton(T10 + i, toucheventselector(ShareScene::onButton), m_pRootLayer, this);        
    }
    
    HelpMethod::initUIButton(TBack, toucheventselector(ShareScene::onButton), m_pRootLayer, this);
    HelpMethod::initUIButton(TShare, toucheventselector(ShareScene::onButton), m_pRootLayer, this);
    
    m_uLabel = (UILabel*)(m_pRootLayer->getChildByTag(TShareTxt));    
    initLabel();
    MsgSystem::Instance()->RegisterObjAndHandler(this, MSGSYS_HANLE_MSG(ShareScene::handleMsg));
    
    xNet->DO_CMD_REQ_APP_SENDING_PODIUM(this, 0);
}


void ShareScene::onExit()
{
    CCLayer::onExit();

    HelpMethod::removeUnusedResourceAtExit(this);
}


void ShareScene::onShare(int iResponse)
{    
    if (iResponse < 10) {
        shareSuccess();
    }        
}


bool ShareScene::handleMsg(const Message &msg)
{
    switch (msg.m_nMsgType)
    {
        case CMD_RES_APP_PODIUM:
        {
            if (xUser->isSuccess) {
                //分享成功, 领奖
                
                accpetPrize();
                
                //设置数据
                xData->setAccpetSharePrize(m_iShareLevel);
                
                //重新显示数据
                initLabel();
            }
            else
            {
                //设置数据
                xData->setAccpetSharePrize(m_iShareLevel);
                
                //重新显示数据
                initLabel();
                
                
                xDialog->show(xUser->message.c_str());
            }
        }
            break;
        case CMD_RES_APP_SENDING_PODIUM:
        {            
            xUSString("share_text", xUser->message.c_str());
        }
            break;            
        default:
            break;
    }
    
    return true;
}



void ShareScene::onButton(CCObject* pSender, TouchEventType type)
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
        case TShare:
        {
            xShare->snsShare(xData->getDefaultShareTxt().c_str(), NULL, shareeventselector(ShareScene::onShare), this);         
        }
            break;
        case TBack:
        {
            xSM->back();
        }
            break;
        default:
        {
            if (iTag >= T10 && iTag <= T1000) {
                m_iShareLevel = iTag - T10;
                xNet->DO_CMD_REQ_APP_PODIUM(this, getShareCountByLevel(m_iShareLevel), xUser->pUserInfo->gemstone);
            }
        }
            break;
    }
}


/////////////////////////////////////////////   ListStart
void ShareScene::onDialog(int id, int iTag)
{
    switch (iTag) {
        case 0:
            break;
            
        default:
            break;
    }
}


void ShareScene::initLabel()
{
    int iCount = xData->getShareCounts();
        
    xData->getAccpetSharePrize(m_bAccpeted);

    //button
    for (int i = 0; i < 4; i++) {
        if (iCount >= getShareCountByLevel(i)) {
           
            if (m_bAccpeted[i] == true) {
                
                if (m_uImage[i] == NULL) {
                    m_uImage[i] = UIImageView::create();
                    m_uImage[i]->loadTexture("fxiang_03.png", tPlist);
                    m_uImage[i]->setPosition(m_uButton[i]->getPosition());
                    
                    UIWidget *uParent = m_uButton[i]->getParent();
                    uParent->addChild(m_uImage[i]);
                    m_uButton[i]->setEnabled(false);
                }
            }
            else
            {
                m_uButton[i]->setEnabled(true);
                m_uButton[i]->active();
            }
        }
        else
        {
            m_uButton[i]->disable();        
        }
    }
    
    
    char temp[100];
    sprintf(temp, "%s%d%s", xStr("share_count0"), iCount, xStr("share_count1"));
    m_uLabel->setText(temp);
}


int ShareScene::getShareCountByLevel(int iLevel)
{
    int iCount = 0;
    
    switch (iLevel) {
        case 0:
            iCount = 5;
            break;
        case 1:
            iCount = 10;
            break;
        case 2:
            iCount = 20;
            break;
        case 3:
            iCount = 50;
            break;
        default:
            break;
    }

    return iCount;
}


void ShareScene::accpetPrize()
{
    int iGold = 0;

    //获取奖励
    switch (m_iShareLevel) {
        case 0:
            iGold = 5;
            break;
        case 1:
            iGold = 10;
            break;
        case 2:
            iGold = 30;
            break;
        case 3:
            iGold = 50;
            break;
        default:
            break;
    }
    
    //服务器已增加
    xData->changeBagGemNum(iGold);
}


void ShareScene::shareSuccess()
{
    initLabel();    
}


