//关卡选择
#ifndef ___LEVEL_SCENE_H__
#define ___LEVEL_SCENE_H__

#include "GameInclude.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define LEVEL_BUTTON_TAG_BASE         (1001)
#define LEVEL_JSON_ROOT_TAG           (2000)
#define LEVEL_LAYER_TAG_TEACH             (2001)

typedef enum _ELevelSceneZOrder
{
    LEVELSCENE_ZORDER_MAP = 0,
    LEVELSCENE_ZORDER_FOCUS,
    LEVELSCENE_ZORDER_MENU,
    LEVELSCENE_ZORDER_TEACH,
    LEVELSCENE_ZORDER_LIMIT
}ELevelSceneZOrder;

class CTeachLayer;

class CLevelLayer : public CCLayer
{
public:
    CLevelLayer();
    ~CLevelLayer();
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    CREATE_FUNC(CLevelLayer);
    
protected:
    void CBBack(CCObject* pSender);
    void ToNext(int nLevelIndex);
    void CBTeachEnd1();
    void CBTeachEnd2();
    
    // map
    void MapInit();
    void buildTouchEvent(CCObject* pSender);
    bool isBossLevel(int chapter, int level);
    
protected:
    UILayer* m_pUILayer;
    CTeachLayer *m_pTeachLayer;
    CCArmature* m_pArmature;
    
    float m_offsetX;
    float m_offsetY;
};

class CLevelScene : public CCScene
{
public:
    CLevelScene();
    ~CLevelScene();
    
    virtual bool init();
    CREATE_FUNC(CLevelScene);
    
private:
    CLevelLayer *m_pMainLayer;
};

#endif // ___LEVEL_SCENE_H__
