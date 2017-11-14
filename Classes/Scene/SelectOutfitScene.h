//選擇道具
#ifndef ___SELECT_OUTFIT_SCENE_H__
#define ___SELECT_OUTFIT_SCENE_H__

#include "GameInclude.h"

USING_NS_CC;
USING_NS_CC_EXT;

class CTeachLayer;

//道具选择列表
#define PLAN_OUTFIT_LAYER_TAG_TABLE                (1000)
#define PLAN_OUTFIT_LAYER_TAG_CELL_SELECTICO      (1001)
#define PLAN_OUTFIT_LAYER_TAG_CELL_NUMLABEL       (1002)
#define PLAN_OUTFIT_LAYER_TAG_TEACH            (2000)

typedef enum _EPlanOutfitLayerZOrder
{
    PLAN_OUTFIT_LAYER_ZORDER_BG = 0,
    PLAN_OUTFIT_LAYER_ZORDER_MASK,
    PLAN_OUTFIT_LAYER_ZORDER_TOPBG,
    PLAN_OUTFIT_LAYER_ZORDER_TOPNAME,
    PLAN_OUTFIT_LAYER_ZORDER_MENU,
    PLAN_OUTFIT_LAYER_ZORDER_TABLE,
    PLAN_OUTFIT_LAYER_ZORDER_TEACH,
    PLAN_OUTFIT_LAYER_ZORDER_LIMIT
}EPlanOutfitLayerZOrder;

class CSelectOutfitLayer : public cocos2d::CCLayer
{
public:
    CSelectOutfitLayer();
    ~CSelectOutfitLayer();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    void CBClose(CCObject *pSender);
    
    CREATE_FUNC(CSelectOutfitLayer);
    
    void CBTouchCell(CCObject *pSender);
    void CBTeachEnd();
    
    void setGoalIndex(int index);
private:
    UILayer* m_pUILayer;
    
    CTeachLayer *m_pTeachLayer;
    
    int m_nGoalIndex;
};

class CSelectOutfitScene : public CCScene
{
public:
    CSelectOutfitScene();
    ~CSelectOutfitScene();
    
    virtual bool init();
    CREATE_FUNC(CSelectOutfitScene);
    void setGoalIndex(int index);
    
private:
    CSelectOutfitLayer *m_pMainLayer;
};

#endif // ___SELECT_OUTFIT_SCENE_H__
