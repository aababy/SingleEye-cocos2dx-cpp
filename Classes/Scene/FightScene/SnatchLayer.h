//
//  SnatchLayer.h
//  SingleEye
//
//  Created by qinfen on 14-1-10.
//
//
/**
 *  夺宝界面
 */
#ifndef __SingleEye__SnatchLayer__
#define __SingleEye__SnatchLayer__

#include "GameInclude.h"
USING_NS_CC;
#define CLOUD_TAG (1001)
enum Snatch_Sprites
{
    Snatch_Sprties_Gold,
    Snatch_Sprties_Gem,
    Snatch_Sprties_Gold_Num,
    Snatch_Sprties_Gem_Num,
    Snatch_Sprites_Time,
    Snatch_Sprites_BeganTime,
    Snatch_Sprites_Time_Icon,
};

enum Snatch_Cloud_Zorder
{
    Snatch_Cloud_Zorder_1 = 10,
    Snatch_Cloud_Zorder_2,
    Snatch_Cloud_Zorder_3,
    Snatch_Cloud_Zorder_4,
    Snatch_Cloud_Zorder_5
};

class SnatchLayer : public CCLayer
{
public:
    CREATE_FUNC(SnatchLayer);
    virtual ~SnatchLayer();
    bool init();
    void onEnter();
    void onExit();
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    void removeEnemyFromArray(CCObject *enemy);
    void addAward(bool isLipin,CCPoint pos);
    void animationEvent(cocos2d::extension::CCArmature *armature, cocos2d::extension::MovementEventType movementType, const char *movementID);
    void addEscape();
private:
    void waveUpdate(float dt);
    void initUI();
    void beganTimeOut(CCNode *pSender);
//    void beganTimeUpdate(float dt);
    void timeUpdate(float dt);
    void removeAniCallBack(CCNode *pSender);
    void updateGoldGemNum();
    void cancelSnatch();
    void cancleSnatchCallBack(CCNode *pSender);
    void checkEnemyZorder();
    void update(float dt);
    void tipsCallBack();
    void callbackContinue(CCObject *pSender);
    void menuPauseCallBack(CCObject *pSender);
private:
    Fight_Level_Data *m_pLevelData;  //关卡数据
    int m_nCurrentWave;
    CCArray *m_pEnemy; //怪列表
    int m_nGold;
    int m_nGem;
    int m_nTime;
    int m_nBeganTime;
    CCMotionStreak *m_pStreak;
    int m_nCloudNum;
    int m_nEscape;
    bool m_bGameEnd;
    CCNode *pauseNode;
};

#endif /* defined(__SingleEye__SnatchLayer__) */
