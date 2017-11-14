//部署
#ifndef ___DEFENSE_SCENE_H__
#define ___DEFENSE_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

#pragma mark - root

#define DEFENSE_UI_TAG_GOLD   (1000)
#define DEFENSE_UI_TAG_GEM   (1000+1)

typedef enum
{
    DEFENSE_ROOT_LAYER_ZORDER_UI = 0,
    DEFENSE_ROOT_LAYER_ZORDER_MENU,
    DEFENSE_ROOT_LAYER_ZORDER_POPUP,
    DEFENSE_ROOT_LAYER_ZORDER_LIMIT
}EDefenseRootLayerZOrder;

//根选择
class CDefenseRootLayer : public CCLayer
{
public:
    CDefenseRootLayer();
    ~CDefenseRootLayer();
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(CDefenseRootLayer);

private:
    void MainInit();
    void MainFresh();
    void setHeadItem(int type, int n, UIImageView *pImg);
    
    void CBSelectLine1(CCObject* pSender, TouchEventType type);
    void CBSelectLine2(CCObject* pSender, TouchEventType type);
    void CBBack(CCObject* pSender);
    void CBNext(CCObject* pSender);
    void CBPopupBack(int id, int iTag);
    void CBToShop(CCObject *pSender);
    
public:
    UILayer* m_pUILayer;
    Layout* m_pLayout;
};

#pragma mark - CDefenseScene

class CDefenseScene : public CCScene
{
public:
    CDefenseScene();
    ~CDefenseScene();
    
    virtual bool init();
    CREATE_FUNC(CDefenseScene);
    
public:
    CDefenseRootLayer *m_pMainLayer;
};

#endif // ___DEFENSE_SCENE_H__
