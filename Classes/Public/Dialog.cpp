//
//  Dialog.cpp
//  SingleEye
//
//  Created by gwsoft on 1/13/14.
//
//

#include "Dialog.h"


typedef enum DialogTag
{
    TLeft = 2,
    TRight = 3,    
    TCancel = 4,
    
    DialogTag_COUNT
}DialogTag;


//static Dialog* instance = NULL;

Dialog * Dialog::getInstance()
{
    static Dialog instance;
    //Dialog 自己不是autorelease的, 不用retain
    
    return &instance;
}


Dialog::Dialog()
{
    m_id = 0;
    init();
}


void Dialog::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kDialogHandlerPriority, true);
}


bool Dialog::init()
{
    CCLayer::init();
    
    m_pRootLayer = (UIPanel*)(GUIReader::shareReader()->widgetFromJsonFile("PvPScene/Dialog.ExportJson"));
    m_pUILayer = UILayer::create();
    m_pUILayer->addWidget(m_pRootLayer);
    m_pUILayer->setTouchEnabled(true);
    m_pUILayer->setTouchPriority(kDialogHandlerPriority);
    
    
    CCLOG("********** %d", TLeft);
    m_pLeft = (UIButton*)HelpMethod::initUIButton(TLeft, toucheventselector(Dialog::onButton), m_pRootLayer, this);
    
    m_pRight = (UIButton*)HelpMethod::initUIButton(TRight, toucheventselector(Dialog::onButton), m_pRootLayer, this);
    
    m_pCancel = (UIButton*)HelpMethod::initUIButton(TCancel, toucheventselector(Dialog::onButton), m_pRootLayer, this);
    
    m_pTxt = (UITextArea*)m_pRootLayer->getChildByTag(10);
    
    m_pLeftBtnOldPoint = m_pLeft->getPosition();
    
    addChild(m_pUILayer);
    
    setTouchEnabled(true);
    
    HelpMethod::adapt(m_pRootLayer, NULL);
    
    //特殊自适应
    CCSize designSize = CCSizeMake(640, 960);   //设计尺寸
    CCSize screenSize = CCEGLView::sharedOpenGLView()->getFrameSize();              //实际尺寸
    float fHeight = designSize.width * screenSize.height / screenSize.width;
    
    UIPanel *uOpacity = dynamic_cast<UIPanel*>(m_pRootLayer->getChildByTag(1));
    if (uOpacity) {                     
        uOpacity->setSize(CCSizeMake(uOpacity->getContentSize().width, fHeight));        
        uOpacity->setPosition(ccp(uOpacity->getPosition().x, uOpacity->getPosition().y - (fHeight - designSize.height)/2));        
    }   
    
    return true;
    
}


void Dialog::onExit()
{
    CCLayer::onExit();
}


void Dialog::show(const char* txt)
{
    show(txt, Dialog_Yes, NULL, NULL);
    setCancelEnabled(false);
}


void Dialog::showYes(const char* txt, SEL_DialogEvent onButton, CCObject *target)
{
    show(txt, Dialog_Yes, onButton, target);
    setCancelEnabled(false);    
}


void Dialog::showYesNo(const char* txt, SEL_DialogEvent onButton, CCObject *target)
{
    show(txt, Dialog_Yes, onButton, target);
}


void Dialog::show(const char* txt, const char* leftImageName, const char *rightImageName, SEL_DialogEvent onButton, CCObject *target)
{
    if (getParent() != NULL) {
        return;
    }
    setCancelEnabled(true);    
        
    m_pLeft->setPosition(m_pLeftBtnOldPoint);
    m_pRight->setEnabled(true);
    
    m_pLeft->loadTextures(leftImageName, leftImageName, NULL, UI_TEX_TYPE_PLIST);
    m_pRight->loadTextures(rightImageName, rightImageName, NULL, UI_TEX_TYPE_PLIST);

    m_pTxt->setText(txt);
    
    m_onDialog = onButton;
    m_target = target;
    
    if (getParent() == NULL) {
        CCScene *pRunScene = CCDirector::sharedDirector()->getRunningScene();
        
        if (pRunScene) {
            pRunScene->addChild(this, kDialogZOrder);
        }
    }
    
    
    setDialogID(0);
}


void Dialog::show(const char* txt, const char* midImageName, SEL_DialogEvent onButton, CCObject *target)
{
    if (getParent() != NULL) {
        return;
    }
    
    show(txt, midImageName, NULL, onButton, target);
    
    m_pLeft->setPosition(ccp(0, m_pLeftBtnOldPoint.y));
    m_pRight->setEnabled(false);
}


void Dialog::onButton(CCObject* pSender, TouchEventType type)
{
    if(type != TOUCH_EVENT_ENDED)
    {
        return;
    }
    
    xData->playKeytone();
    
    UIWidget *widget = (UIWidget*)pSender;
    
    int iTag = widget->getTag();
    
    if (iTag != TCancel) {
        if (m_onDialog) {
            
            CCScene *pRunScene = CCDirector::sharedDirector()->getRunningScene();
            pRunScene->removeChild(this);
            (m_target->*m_onDialog)(m_id, getCallbackTag(iTag));
        }
        else
        {
            CCScene *pRunScene = CCDirector::sharedDirector()->getRunningScene();
            pRunScene->removeChild(this);
        }
    }
    else
    {
        CCScene *pRunScene = CCDirector::sharedDirector()->getRunningScene();
        pRunScene->removeChild(this);
    }
}


void Dialog::setCancelEnabled(bool bEnabled)
{
    m_pCancel->setEnabled(bEnabled);
}


int Dialog::getCallbackTag(int iTag)
{
    switch (iTag) {
        case TLeft:
            return 0;
        case TRight:
            return 1;
        case TCancel:
            return 2;
        default:
            return 0;
    }
}


bool Dialog::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}


void Dialog::setEnabled(bool value)
{
    Dialog *dialog = Dialog::getInstance();
    CCScene *pRunScene = CCDirector::sharedDirector()->getRunningScene();
    
    if (value) {
        pRunScene->addChild(dialog, kDialogZOrder);
    }
    else
    {
        pRunScene->removeChild(dialog);
    }
}


void Dialog::setDialogID(int id)
{
    Dialog *dialog = Dialog::getInstance();
    
    dialog->m_id = id;
}
