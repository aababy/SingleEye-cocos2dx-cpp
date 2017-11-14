//選擇寵物
#ifndef ___SELECT_PET_SCENE_H__
#define ___SELECT_PET_SCENE_H__

#include "GameInclude.h"

USING_NS_CC;
USING_NS_CC_EXT;

class CTeachLayer;

enum EPlanPetLayerTag
{
    PLAN_PET_LAYER_TAG_CHUZHAN = 3000,
    PLAN_PET_LAYER_TAG_LEFTARROW,
    PLAN_PET_LAYER_TAG_RIGHTARROW,
    PLAN_PET_LAYER_TAG_BOARD_JSON,
    PLAN_PET_LAYER_TAG_TEACH,
    PLAN_PET_LAYER_TAG_LIMIT
};

enum EPlanPetLayerZOrder
{
    PLAN_PET_LAYER_ZORDER_BG = 0,
    PLAN_PET_LAYER_ZORDER_MASK,
    PLAN_PET_LAYER_ZORDER_TOPBG,
    PLAN_PET_LAYER_ZORDER_TOPNAME,
    PLAN_PET_LAYER_ZORDER_MENU,
    PLAN_PET_LAYER_ZORDER_JSON,
    PLAN_PET_LAYER_ZORDER_SUCCESS_EFFECT,
    PLAN_PET_LAYER_ZORDER_TEACH,
    PLAN_PET_LAYER_ZORDER_LIMIT
};

//雇佣兵选择
class CSelectPetLayer : public CCLayer
{
public:
    CSelectPetLayer(void);
    ~CSelectPetLayer(void);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(CSelectPetLayer);
    
    void setGoalIndex(int index);
    
private:
    void PetInit();
    void InitPetPage(Layout* pLayout, int nId);
    void PetFresh();
    
    void CBPageViewTurningEvent(CCObject *pSender);
    void CBLeftArrow(CCObject* pSender);
    void CBRightArrow(CCObject* pSender);
    void CBWaitOperation(float dt);
    void CBToOperation(CCObject* pSender);
    void CBPetChuZhan(CCObject* pSender);
    void CBPetClose(CCObject* pSender);
    void CBTeachEnd();
    
    void freshChuzhanBtn();
    void freshArrowUI();
    void freshOperationData();
    void freshOperationUI(int nId);
    void freshLevelStarUI();
    void freshPageProp();
    void freshHeadUI();
    
    void unlockPet(int nHeroId);
    void trainPet(int nHeroId);
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


class CSelectPetScene : public CCScene
{
public:
    CSelectPetScene();
    ~CSelectPetScene();
    
    virtual bool init();
    CREATE_FUNC(CSelectPetScene);
    
    void setGoalIndex(int index);
    
private:
    CSelectPetLayer *m_pMainLayer;
};

#endif // ___SELECT_PET_SCENE_H__
