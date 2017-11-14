//
//  CCDialog.h
//  SingleEye
//
//  Created by gwsoft on 1/13/14.
//
//

#ifndef __SingleEye__CCDialog__
#define __SingleEye__CCDialog__

#include "GameInclude.h"


#define xDialog                 Dialog::getInstance()

#define Dialog_Yes              "tckang_02.png"


#define kDialogHandlerPriority  (kCCMenuHandlerPriority - 1)
#define kDialogZOrder           (128)


USING_NS_CC;
USING_NS_CC_EXT;

typedef void (CCObject::*SEL_DialogEvent)(int, int);
#define dialogeventselector(_SELECTOR) (SEL_DialogEvent)(&_SELECTOR)

//类型分为 Yes, YesNo, MidNo, LeftRightNo

class Dialog : public CCLayer
{
    
public:
    static Dialog * getInstance();
    
    //不要在create, onEnter, init 内调用这个函数.
    void show(const char* txt);
    void showYes(const char* txt, SEL_DialogEvent onButton, CCObject *target);
    void showYesNo(const char* txt, SEL_DialogEvent onButton, CCObject *target);
        
    void show(const char* txt, const char* leftImageName, const char *rightImageName, SEL_DialogEvent onButton, CCObject *target);
    void show(const char* txt, const char* midImageName, SEL_DialogEvent onButton, CCObject *target);
    void onButton(CCObject* pSender, TouchEventType type);
    
    void setDialogID(int ID);
    
    void setCancelEnabled(bool bEnabled);
    
private:
    Dialog();
    virtual bool init();
    
    // 需要重写触摸注册函数，重新给定触摸级别
    virtual void registerWithTouchDispatcher(void);
    // 重写触摸函数，永远返回 true ，屏蔽其它层，达到 “模态” 效果
    bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
   
    CCObject*           m_target;
    SEL_DialogEvent     m_onDialog;
    int                 m_id;
    
    
    static void destroy();
    static void setEnabled(bool value);
    int getCallbackTag(int iTag);
    
    //void onEnter();
    void onExit();
    
    UILayer * m_pUILayer;
    UIButton * m_pLeft;
    UIButton * m_pRight;
    UIButton * m_pCancel;
    UITextArea * m_pTxt;
    
    CCPoint    m_pLeftBtnOldPoint;
    UIPanel     *m_pRootLayer;
};




#endif /* defined(__SingleEye__CCDialog__) */
