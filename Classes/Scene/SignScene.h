//
//  SignScene.h
//  SingleEye
//
//  Created by gwsoft on 1/7/14.
//
//

#ifndef __SingleEye__SignScene__
#define __SingleEye__SignScene__

#include <iostream>


#include "GameInclude.h"
#include "UIListViewExtend.h"
#include "CCTableView.h"


USING_NS_CC;
USING_NS_CC_EXT;

class SignScene : public CCLayer
{
public:
    SignScene();
    
    CREATE_FUNC(SignScene);
    virtual bool init();
    static CCScene* scene();
    
    UIPanel *m_pRootLayer;
    UILayer * m_pUILayer;
    
    UIImageView*    m_uBG[7];                   //背景
    UIImageView*    m_iProp[7];                 //道具
    UIImageView*    m_iSigned[7];               //已签到
    UIImageView*    m_uLabel;
    UILabelAtlas*   m_uSignCount;               //已签到次数
    UILabelAtlas*   m_uPrizeCount;              
    
    int  m_iDay;
    int  m_iWeekDay;
    
    char temp[10];
    
    void onEnter();
    void onExit();
    void onButton(CCObject* pSender, TouchEventType type);
    void onDialog(int id, int iTag);
    bool handleMsg(const Message &msg);
    void initShow();
    bool checkLastSign(int iTodayDay, int iTodayWeek);
    void saveLastSign();
    Prop_Type showProp(UIImageView* uImage, int iWeekDay);
    void addProp();
    void initLabel();
    void checkBigPrize();
    void showContinueSign();
    void refreshVector();
    
    int m_iContinues;
};


#endif /* defined(__SingleEye__SignScene__) */
