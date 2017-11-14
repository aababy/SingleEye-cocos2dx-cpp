//部署
#ifndef ___PLAN_SCENE_H__
#define ___PLAN_SCENE_H__

#include "GameInclude.h"

#include <vector>

USING_NS_CC;
USING_NS_CC_EXT;

typedef enum _EPlanSceneState
{
    PLAN_SCENE_STATE_ROOT = 0,              //根选择
    PLAN_SCENE_STATE_HERO,            //选择英雄
    PLAN_SCENE_STATE_OUTFIT,           //选择道具
    PLAN_SCENE_STATE_PET,      //选择雇佣兵
    PLAN_SCENE_STATE_LIMIT
}EPlanSceneState;

typedef enum
{
    PLANSCENE_TYPE_NULL = 0,
    PLANSCENE_TYPE_PVE,         //普通关卡
    PLANSCENE_TYPE_PVP,         //pvp
    PLANSCENE_TYPE_BOSS,        //世界boss战
    PLANSCENE_TYPE_LIMIT
}EPlanSceneType;

#pragma mark - root

typedef enum _EPlanRootLayerZOrder
{
    PLAN_ROOT_LAYER_ZORDER_UI = 0,
    PLAN_ROOT_LAYER_ZORDER_MENU,
    PLAN_ROOT_LAYER_ZORDER_TEACH,
    PLAN_ROOT_LAYER_ZORDER_LIMIT
}EPlanRootLayerZOrder;

#define PLAN_ROOT_LAYER_TAG_HERO_BASE      (2000)
#define PLAN_ROOT_LAYER_TAG_HERO_BTN_0      (PLAN_ROOT_LAYER_TAG_HERO_BASE+0)
#define PLAN_ROOT_LAYER_TAG_HERO_BTN_1      (PLAN_ROOT_LAYER_TAG_HERO_BASE+1)
#define PLAN_ROOT_LAYER_TAG_HERO_BTN_2      (PLAN_ROOT_LAYER_TAG_HERO_BASE+2)

#define PLAN_ROOT_LAYER_TAG_OUTFIT_BASE      (3000)
#define PLAN_ROOT_LAYER_TAG_OUTFIT_BTN_0    (PLAN_ROOT_LAYER_TAG_OUTFIT_BASE+0)
#define PLAN_ROOT_LAYER_TAG_OUTFIT_BTN_1    (PLAN_ROOT_LAYER_TAG_OUTFIT_BASE+1)
#define PLAN_ROOT_LAYER_TAG_OUTFIT_BTN_2    (PLAN_ROOT_LAYER_TAG_OUTFIT_BASE+2)

#define PLAN_ROOT_LAYER_TAG_PET_BASE      (4000)
#define PLAN_ROOT_LAYER_TAG_PET_BTN_0       (PLAN_ROOT_LAYER_TAG_PET_BASE+0)
#define PLAN_ROOT_LAYER_TAG_PET_BTN_1       (PLAN_ROOT_LAYER_TAG_PET_BASE+1)
#define PLAN_ROOT_LAYER_TAG_PET_BTN_2       (PLAN_ROOT_LAYER_TAG_PET_BASE+2)
#define PLAN_ROOT_LAYER_TAG_PET_BTN_3       (PLAN_ROOT_LAYER_TAG_PET_BASE+3)

#define PLAN_ROOT_LAYER_TAG_PET_2_BASE      (5000)
#define PLAN_ROOT_LAYER_TAG_PET_BTN_4       (PLAN_ROOT_LAYER_TAG_PET_2_BASE+0)
#define PLAN_ROOT_LAYER_TAG_PET_BTN_5       (PLAN_ROOT_LAYER_TAG_PET_2_BASE+1)
#define PLAN_ROOT_LAYER_TAG_PET_BTN_6       (PLAN_ROOT_LAYER_TAG_PET_2_BASE+2)
#define PLAN_ROOT_LAYER_TAG_PET_BTN_7       (PLAN_ROOT_LAYER_TAG_PET_2_BASE+3)

enum EPlanRootLayerTag
{
    PLAN_ROOT_LAYER_TAG_GOLD = 1000,
    PLAN_ROOT_LAYER_TAG_GEM,
    PLAN_ROOT_LAYER_TAG_MENU,
    PLAN_ROOT_LAYER_TAG_MENU_ITEM_BACK,
    PLAN_ROOT_LAYER_TAG_JSON_ROOT,
    PLAN_ROOT_LAYER_TAG_SWITCH_BTN,
    PLAN_ROOT_LAYER_TAG_PET_PANEL_1,
    PLAN_ROOT_LAYER_TAG_PET_PANEL_2,
    PLAN_ROOT_LAYER_TAG_TEACH,
    PLAN_ROOT_LAYER_TAG_LIMIT
};

typedef enum
{
    PLAN_ROOT_LAYER_STATE_PET_1 = 0,
    PLAN_ROOT_LAYER_STATE_PET_2,
    PLAN_ROOT_LAYER_STATE_PET_SWITCHING,
    PLAN_ROOT_LAYER_STATE_LIMIT
}EPlanRootLayerState;

class CTeachLayer;

//根选择
class CPlanRootLayer : public CCLayer
{
public:
    CPlanRootLayer();
    ~CPlanRootLayer();
    
    virtual bool init();
    virtual void onExit();
    virtual void onEnter();
    CREATE_FUNC(CPlanRootLayer);
    
    void setType(EPlanSceneType goalType);

private:
    void MainInit();
    void MainFresh();
    void setHeadItem(int type, int n, UIImageView *pImg);
    void addPetArmature(UIWidget *pImg, int nId);
    void switchToPet1();
    void switchToPet2();
    void freshPetSwitch(int goal);
    
    void CBSwitchPet(CCObject* pSender);
    void CBSelectHero(CCObject* pSender);
    void CBPopupBack(int id, int iTag);
    void CBSelectOutfit(CCObject* pSender);
    void CBSelectPet(CCObject* pSender);
    void CBSelectPet2(CCObject* pSender);
    void CBBack(CCObject* pSender);
    void CBNext(CCObject* pSender);
    void CBToShop(CCObject *pSender);
    void CBUnlockPlane2(CCObject *pSender);
    void CBTeachEnd1();
    void CBTeachEnd2();
    void CBTeachEnd3();
    void CBTeachEnd4();
    void CBTeachEnd5();

public:
    UILayer* m_pUILayer;
    EPlanSceneType m_eSceneType;
    EPlanRootLayerState m_eState;
    CCPoint m_leftPos;
    CCPoint m_middlePos;
    CCPoint m_rightPos;
    
    CTeachLayer *m_pTeachLayer;
    float m_xOffset;
    float m_yOffset;
    
    int m_nHeroSetGoalIndex;
    int m_nOutfitSetGoalIndex;
};

#pragma mark - CPlanScene

class CPlanScene : public CCScene
{
public:
    CPlanScene();
    ~CPlanScene();
    
    virtual bool init();
    CREATE_FUNC(CPlanScene);
    
    void setType(EPlanSceneType goalType);
    
public:
    CPlanRootLayer *m_pMainLayer;
};

#endif // ___PLAN_SCENE_H__
