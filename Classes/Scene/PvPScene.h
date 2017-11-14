//
//  PvPScene.h
//  SingleEye
//
//  Created by gwsoft on 12/31/13.
//
//

#ifndef __SingleEye__PvPScene__
#define __SingleEye__PvPScene__

#include <iostream>


#include "GameInclude.h"
#include "CCTableView.h"
//#include "UIListViewExtend.h"


USING_NS_CC;
USING_NS_CC_EXT;



struct MatchData {
    INT32   userId;//	Int32	玩家ID
    string  userName;//	String	玩家名称
    INT32   rankingStatus;//	Int32	是否正在战斗（1 正在战斗 0 空闲）
    INT32   score;//	Int32;//	玩家积分
    int     iHeadImage;
};


class TopLayer : public CCLayer
{
    
public:
    CREATE_FUNC(TopLayer);
    virtual bool init();
    
    // 需要重写触摸注册函数，重新给定触摸级别
    virtual void registerWithTouchDispatcher(void);
    // 重写触摸函数，永远返回 true ，屏蔽其它层，达到 “模态” 效果
    bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);    
    
    void addTopImage(CCPoint point);
    void onRiseDone();
    void sunBlast(const CCPoint &position);    
    void onBlastDone(float dt);    
    
    UILayer * m_pUILayer;
    UIPanel     *m_pRootLayer;
    UIImageView *m_pImage;
    CCParticleSystem*    m_emitter;
};



class PvPScene : public CCLayer
{
public:
    PvPScene();
    ~PvPScene();
    
    CREATE_FUNC(PvPScene);
    virtual bool init();
    static CCScene* scene();

    
    UIPanel *m_pRootLayer;
    UILayer * m_pUILayer;
    UICheckBox*     m_pMatch;
    UICheckBox*     m_pChallenge;
    CCArmature*     m_aLoading;
    
    UIListViewEx*   m_pList;
    float           m_fSlideRange;
    
    UIPanel *m_pPanelMatch;
    UIPanel *m_pPanelChallenge;
    
    UIPanel *m_pPanelInit;    
       
    char temp[10];
    int  m_iCurIdx;
    int  m_iMatchCount;
    vector<MatchData> m_vMatch;

    int  m_iChallengeCount;
    
    
    void onEnter();
    void onExit();
    void onDialog(int id, int iTag);
    void onButton(CCObject* pSender, TouchEventType type);
    void onCheckBox(CCObject *pSender, CheckBoxEventType type);
    void createList();
    //void listViewEvent(CCObject *pSender, ListViewEventType type);
    bool handleMsg(const Message &msg);
    void registerWithTouchDispatcher();
    
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void selectedItemEvent(CCObject*, ListViewEventType type);
    
    void update(float delta);
    int getRank();
    void sunRise();   

    int m_nCount;
    int m_userId;
    
    UIImageView *m_pImage;
    UIImageView *m_pTemp;
        
  
private:
    void copyMatchData();
    void showMatchData();
    void checkMessage();
    int  iNewMsgCount;
    void test();
    void newMessage(CCObject *obj);
    bool reducePower();
    void checkPower();
    void showScore();   
    int  getRandomHeadID();
    
   
    void leaveStamp(UIImageView *uOri);    
    
    CCArmature *m_aYG;
    
    TopLayer*   m_topLayer;
    CCScene*    m_pRootScene;
    
    bool        m_bBlock;
};


#endif /* defined(__SingleEye__PvPScene__) */
