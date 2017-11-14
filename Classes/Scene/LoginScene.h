//注册登录
#ifndef __LOGIN_SCENE__
#define __LOGIN_SCENE__

#include "GameInclude.h"


USING_NS_CC;
USING_NS_CC_EXT;

class LoginScene : public CCLayer
{
public:
    LoginScene();
    
    CREATE_FUNC(LoginScene);
    virtual bool init();
    static CCScene* scene();
    
    UIPanel *m_pRootLayer;
    UILayer * m_pUILayer;
    UITextField *m_pInput;
    UIPanel *m_uImageForInput;
    
    char temp[10];
    string m_sInput;    
    string m_UserName;
    
    void onButton(CCObject* pSender, TouchEventType type);
    void textFieldEvent(CCObject *pSender, TextFiledEventType type);
    void imeOn(float dt);    
    void imeOff(float dt);  
    bool onBlock(CCTouch *pTouch, CCEvent *pEvent, TouchType type);
    
    void onEnter();
    void onExit();
    bool handleMsg(const Message &msg);
    
    bool m_bIMEOn;
    bool m_bBlock;
};

#endif /* defined(__LOGIN_SCENE__) */
