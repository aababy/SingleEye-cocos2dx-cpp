//
//  MessageScene.h
//  SingleEye
//
//  Created by gwsoft on 1/7/14.
//
//

#ifndef __SingleEye__MessageScene__
#define __SingleEye__MessageScene__

#include <iostream>


#include "GameInclude.h"
#include "UIListViewExtend.h"
#include "CCTableView.h"

//#include "PopupLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;

class MessageScene : public CCLayer
{
public:
    MessageScene();
    
    CREATE_FUNC(MessageScene);
    virtual bool init();
    static CCScene* scene();
    
    UIPanel *m_pRootLayer;
    UILayer * m_pUILayer;
    
    UIListViewEx*   m_pList;
    
    char temp[10];
    
    void onEnter();
    void onExit();
    void onButton(CCObject* pSender, TouchEventType type);
    void onCheckBox(CCObject *pSender, CheckBoxEventType type);
    void createList();
    void onDialog(int id, int iTag);
    bool handleMsg(const Message &msg);
    //void listViewEvent(CCObject *pSender, ListViewEventType type);
    //void registerWithTouchDispatcher();
    
    void update(float delta);
    
    int m_nCount;
    int m_iMsgCount;
    int m_userId;

    void setCount(int iMsgCount);    
};


#endif /* defined(__SingleEye__MessageScene__) */
