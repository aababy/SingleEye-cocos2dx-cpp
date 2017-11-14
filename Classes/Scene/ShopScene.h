//商店
#ifndef ___SHOP_SCENE_H__
#define ___SHOP_SCENE_H__

#include "GameInclude.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define SHOPSCENE_TAG_TOPPROP_GOLD     (1000)
#define SHOPSCENE_TAG_TOPPROP_GEM      (1000+1)
#define SHOPSCENE_TAG_TABLE            (1000+2)
#define SHOPSCENE_TAG_BUY_BUTTON       (1000+3)
#define SHOPSCENE_TAG_TEACH            (1000+4)

typedef enum _EShopSceneZOrder
{
    SHOPSCENE_ZORDER_BACKGROUND = 0,
    SHOPSCENE_ZORDER_TABLE,
    SHOPSCENE_ZORDER_PROP,
    SHOPSCENE_ZORDER_FLY_ACT,
    SHOPSCENE_ZORDER_MENU,
    SHOPSCENE_ZORDER_TEACH,
    SHOPSCENE_ZORDER_LIMIT
}EShopSceneZOrder;

typedef enum
{
    SHOPSCENE_POPUP_NULL = 0,
    SHOPSCENE_POPUP_LACK_GOLD,   //金币不足
    SHOPSCENE_POPUP_LACK_GEM,    //宝石不足
    SHOPSCENE_POPUP_LIMIT
}EShopScenePopupType;

class CTeachLayer;

class CShopLayer : public CCLayer
{
public:
    CShopLayer();
    ~CShopLayer();
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(CShopLayer);
    
    CC_PROPERTY(int, m_nCurId, CurOutfitId);
    CC_PROPERTY(int, m_nPreId, PreOutfitId);
    
private:
    void CBTeachEnd1();
    void CBTeachEnd2();
    void CBTouchCell(CCObject *pSender);
    void CBBack(CCObject* pSender);
    void CBBuyGem(CCObject *pSender);
    void CBBuyOutfit(CCObject *pSender);
    void CBBuyActionEnd(CCObject *pSender);
    
    void freshCurOutfit();
    void checkToChangeNewOutfit();
    void setNullCurOutfit();
    void freshGold();
    void freshGem();
    
    void toBuyOutfit(int nId);
    void toPopup(EShopScenePopupType type);
    void buyPropStatistics(int nId);
    
private:
    bool isReachLimit(int outfitId);
    UILayer* m_pUILayer;
    CTeachLayer *m_pTeachLayer;
    
    CCPoint m_actPos;//购买效果目标位置
    bool m_bAct;//true 购买效果action执行中，不可再次点击购买按钮
    
    float m_offsetX;
    float m_offsetY;
    CCPoint m_selectPos;
};

class CShopScene : public CCScene
{
public:
    CShopScene();
    ~CShopScene();
    
    virtual bool init();
    CREATE_FUNC(CShopScene);
    
private:
    CShopLayer *m_pMainLayer;
};

#endif // ___SHOP_SCENE_H__
