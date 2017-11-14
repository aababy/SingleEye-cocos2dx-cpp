//
//  ResultLayer.h
//  SingleEye
//
//  Created by qinfen on 13-11-27.
//
//

#ifndef __SingleEye__ResultLayer__
#define __SingleEye__ResultLayer__

#include "GameInclude.h"
USING_NS_CC;

#define Max_Star (3)
#define Loading_Tag (100)
#define Box_Tag (101)
#define Txt_Move_Time (0.3)
#define ShakeTeachLayer (102)
#define StarFallTime (0.2)
#define HeroLayoutTag (1000)
#define ColorLayerTag (1001)
#define MenuTag (1002)
#define TITLE_WIN_DY (250)
#define STAR_DY (120)
#define TXT_DAMAGE_DX (140)
#define TXT_DAMAGE_DY (-40)
#define NUM_DAMAGE_DX (15)
#define NUM_DAMAGE_DY (-60)
#define TXT_HIT_DX (140)
#define TXT_HIT_DY (30)
#define NUM_HIT_DX (15)
#define NUM_HIT_DY (10)
#define ICON_AWARD_DX (160)
#define ICON_AWARD_DY (220)
#define NUM_GOLD_DX (60)
#define NUM_GOLD_DY (220)
#define ICON_BOX_DX (100)
#define ICON_BOX_DY (170)
#define ICON_SHARE_DX (180)
#define ICON_SHARE_DY (-20)
#define TITLE_LOST_DY (250)
#define TXT_JIANGLI (120)
#define BG_SMALL (180)
enum Result_Star
{
    Result_Star_One = 103,
    Result_Star_Two,
    Result_Star_Three,
};

#pragma mark - ResultLayer
class ResultLayer : public CCLayer
{
public:
    CREATE_FUNC(ResultLayer);
    virtual ~ResultLayer();
    bool init();
    void onEnter();
    void onExit();
    void showWin(int star ,int hit,int damage,int gold,int level,int fightModel,int gemstone,int gemstoneUse,int hurt); //gemstone:宝石奖励 gemstoneUse:宝石消耗
    void showLose(int star,int hit,int damage,int gold,int gemstone,int gemstoneUse,int fightModel,int hurt);
    virtual void didAccelerate(CCAcceleration* pAccelerationValue);
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    bool handleMsg(const Message &msg);
private:
    bool m_bIsShanke;
    bool m_canTouch;
    CCSprite *m_titleResult;  //win /  lose
    int m_nStar;
    
    CCSprite *m_txtHit;
    CCSprite *m_txtDamage;
    CCLabelBMFont *m_labelHit;   //命中率
    CCLabelBMFont *m_labelDamage;  //损坏率
    CCSprite *m_txtJiangli;
//    CCSprite *m_percentGreen;  //绿色百分号
//    CCSprite *m_percentOrange;  //橘色百分号
    
    
    CCSprite *m_iconAward;
    CCLabelBMFont *m_labelGold;
    
    CCMenu *m_iconBox;
    CCMenu *m_iconShare;
    
    CCSprite *m_bgResult;
    CCSprite *m_bgResultSmall;
    
    CCMenu *m_btnLeft;
    CCMenu *m_btnRight;
    
    CCMenu *m_btnBack;
    
    INT16 m_nFightModel;
    bool m_bShankeSuccess;  //摇奖是否成功
    
    CCNode *resultNode;
    CCNode *shakeNode;
    
    CCNode *unlockNode;
    CCNode *rookieNode;
    bool m_bUseShake;
    
    CCMenuItemSprite  *menuBox;
    
    bool m_bShakeTeach;
    bool m_bStartingShakeTeach;
    bool m_bShowLockNode;
    
    
    bool shankeHand_Gift();
    void addStar(int star, float posY);
    void clickedBoxCallBack(CCObject *pSender);
    void clickedShareCallBack(CCObject *pSender);
    void animationWinCallBack(CCObject *pSender);
    void animationLoseCallBack(CCObject *pSender);
    void restartCallBack(CCObject *pSender);
    void quitCallBack(CCObject *pSender);
    void getResData(); //获取摇奖结果
    void getResDataUpdate();
    void backCallBack(CCObject *pSender);
    void boxClickedCallBack(CCObject *pSender);
    void unlockBackCallBack(CCObject *pSender);
    void rookieCallBack(CCObject *pSender);
    void shakeTeachCallBack();
    void starFallCallBack();
    void setCardData(Layout *pPageLayout,bool hero,int nID);
    void createTeachLayerCallBack();
    int  getUnLockLansID(int nlevel); //获取雇佣兵解锁信息 返回0为没有解锁雇佣兵 其余返回解锁雇佣兵id
    void nodeVisibleCallBack(CCNode *pSender);
    void nodeVisibleCallBack_RemoveRes(CCNode *pSender);
    void nodeResumeActCallBack(CCNode *pSender);
    void disableBtn();
    void enableBtn();
};
#endif /* defined(__SingleEye__ResultLayer__) */
