//選擇英雄
#ifndef ___SELECT_HERO_SCENE_H__
#define ___SELECT_HERO_SCENE_H__

#include "GameInclude.h"

USING_NS_CC;
USING_NS_CC_EXT;

enum EPlanHeroLayerTag
{
    PLAN_HERO_LAYER_TAG_CHUZHAN = 3000,
    PLAN_HERO_LAYER_TAG_LEFTARROW,
    PLAN_HERO_LAYER_TAG_RIGHTARROW,
    PLAN_HERO_LAYER_TAG_BOARD_JSON,
    PLAN_HERO_LAYER_TAG_TEACH,
    PLAN_HERO_LAYER_TAG_LIMIT
};

typedef enum _EPlanHeroLayerZOrder
{
    PLAN_HERO_LAYER_ZORDER_BG = 0,
    PLAN_HERO_LAYER_ZORDER_MASK,
    PLAN_HERO_LAYER_ZORDER_MENU,
    PLAN_HERO_LAYER_ZORDER_BOARD,
    PLAN_HERO_LAYER_ZORDER_TABLE,
    PLAN_HERO_LAYER_ZORDER_TOPBG,
    PLAN_HERO_LAYER_ZORDER_TOPNAME,
    PLAN_HERO_LAYER_ZORDER_SUCCESS_EFFECT,
    PLAN_HERO_LAYER_ZORDER_TEACH,
    PLAN_HERO_LAYER_ZORDER_LIMIT
}EPlanHeroLayerZOrder;

class CTeachLayer;

class CSelectHeroLayer : public CCLayer
{
public:
    CSelectHeroLayer();
    ~CSelectHeroLayer();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(CSelectHeroLayer);
    
    void setGoalIndex(int index);
    
private:
    void HeroInit();
    void InitHeroPage(Layout* pLayout, int nId);
    void HeroFresh();
    
    void CBPageViewTurningEvent(CCObject *pSender);
    void CBLeftArrow(CCObject* pSender);
    void CBRightArrow(CCObject* pSender);
    void CBWaitOperation(float dt);
    void CBToOperation(CCObject* pSender);
    void CBPopupHeroChuZhan(CCObject* pSender);
    void CBPopupHeroClose(CCObject* pSender);
    void CBTeachEnd();
    
    void freshChuzhanBtn();
    void freshArrowUI();
    void freshOperationData();
    void freshOperationUI(int nId);
    void freshLevelStarUI();
    void freshPageProp();
    void freshHeadUI();
    
    void unlockHero(int nHeroId);
    void trainHero(int nHeroId);
    void successEffect();
    
public:
    UILayer* m_pUILayer;
    Layout* m_pLayout;
    UIPageView* m_pPageView;
    
    CTeachLayer *m_pTeachLayer;
    
    int m_nCurPageIndex;
    int m_nCurPageTag;
    ETavernOperationType m_nOperationIndex;
    
    int m_nGoalIndex;
    bool m_bLockOperationBtn;//防止疯狂连续点击
};

class CSelectHeroScene : public CCScene
{
public:
    CSelectHeroScene();
    ~CSelectHeroScene();
    
    virtual bool init();
    CREATE_FUNC(CSelectHeroScene);
    
    void setGoalIndex(int index);
    
private:
    CSelectHeroLayer *m_pMainLayer;
};

#endif // ___SELECT_HERO_SCENE_H__
