//
//  BossScene.h
//  SingleEye
//
//  Created by gwsoft on 1/7/14.
//
//

#ifndef __SingleEye__BossScene__
#define __SingleEye__BossScene__

#include <iostream>


#include "GameInclude.h"
#include "CCTableView.h"



USING_NS_CC;
USING_NS_CC_EXT;

class BossScene : public CCLayer
{
public:
    BossScene();
    
    CREATE_FUNC(BossScene);
    virtual bool init();
    static CCScene* scene();
    
    UIPanel *m_pRootLayer;
    
    UIPanel *m_pBossChallenge;
    UIPanel *m_pRank;
    
    UILayer * m_pUILayer;
    
    UICheckBox *m_pChkBoss;
    UICheckBox *m_pChkRank;
    
    UIListViewEx* m_pList;
    CCArmature* m_aBoss;
    
    UIButton *m_uInspire;    
    UIButton *m_uPrize;
    CMoneyItem *m_pGold;
    CMoneyItem *m_pGem;
    
    char temp[30];
    
    void onEnter();
    void onExit();
    void onMoneyItem(EMoneyItemType m_nType);
    void onButton(CCObject* pSender, TouchEventType type);
    void onCheckBox(CCObject *pSender, CheckBoxEventType type);
    void createList();
    void onDialog(int id, int iTag);
    bool handleMsg(const Message &msg);
    void selectedItemEvent(CCObject*, ListViewEventType type);
    void addBoss();
    void showInspire();
    void getPrize();
    void showMyRank();
    void checkPrize();
    //void listViewEvent(CCObject *pSender, ListViewEventType type);
    //void registerWithTouchDispatcher();
    
    void update(float delta);
    
    int m_nCount;
    int m_iMsgCount;
    int m_userId;    
    

    void setCount(int iMsgCount);
    void showInfo();
    
    bool m_bOnTime;
    bool m_bTestFight;
    bool m_bBossInfo;
};


#endif /* defined(__SingleEye__BossScene__) */
