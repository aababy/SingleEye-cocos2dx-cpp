//
//  ShareScene.h
//  SingleEye
//
//  Created by gwsoft on 1/7/14.
//
//

#ifndef __SingleEye__ShareScene__
#define __SingleEye__ShareScene__

#include <iostream>


#include "GameInclude.h"
#include "UIListViewExtend.h"
#include "CCTableView.h"

USING_NS_CC;
USING_NS_CC_EXT;

class ShareScene : public CCLayer
{
public:
    ShareScene();
    
    CREATE_FUNC(ShareScene);
    virtual bool init();
    static CCScene* scene();
    
    UIPanel *m_pRootLayer;
    UILayer * m_pUILayer;
    
    UIImageView*    m_uImage[5];
    UIButton*       m_uButton[5];
    UILabel*        m_uLabel;
    
    CCTableView*    m_pList;
    
    char temp[10];
    
    void onEnter();
    void onExit();
    void onButton(CCObject* pSender, TouchEventType type);
    void onCheckBox(CCObject *pSender, CheckBoxEventType type);
    void onDialog(int id, int iTag);
    bool handleMsg(const Message &msg);
    void initLabel();
    int getShareCountByLevel(int iLevel);
    void shareSuccess();
    void accpetPrize();
    void onShare(int iResponse);
    //void listViewEvent(CCObject *pSender, ListViewEventType type);
    //void registerWithTouchDispatcher();
    
    bool m_bAccpeted[5];        
    
    int m_iShareLevel;
};


#endif /* defined(__SingleEye__ShareScene__) */
