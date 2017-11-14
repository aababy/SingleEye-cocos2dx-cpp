//副本选择
#ifndef ___DUNGEON_SCENE_H__
#define ___DUNGEON_SCENE_H__

#include "GameInclude.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define ISLAND_ROOT_TAG                (2000)
#define ISLAND_TAG_TEACH               (2001)
#define ISLAND_TAG_MASK                (2002)
#define ISLAND_TAG_SINGLE_ISLAND       (2003)
#define ISLAND_CHECKBOX_TAG_BASE       (1000)

enum
{
    DUNGEON_ZORDER_BG = 0,
    DUNGEON_ZORDER_MASK,
    DUNGEON_ZORDER_SINGLE_ISLAND,
    DUNGEON_ZORDER_MENU,
///    DUNGEON_ZORDER_INFO,    //副本介绍
    DUNGEON_ZORDER_TEACH,   //教程
};

class CDungeonLayer : public CCLayer
{
public:
    CDungeonLayer();
    ~CDungeonLayer();
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(CDungeonLayer);
    
protected:
    void IslandInit();
    void AddIntroduceMaskBg(int goalIndex);
    void IslandTouchEvent(CCObject* pSender);
    
    void CBBack(CCObject* pSender);
    void CBTeachEnd1();
    void CBTeachEnd2();
    void CBTeachEnd3();
    void CBTeachEnd4();
    void CBTeachEnd5();
    void ToNext(int nChapterIndex);
    
    void clickedIsland();
protected:
    UILayer* m_pUILayer;    
    CTeachLayer *m_pTeachLayer;
    float m_offsetX;
    float m_offsetY;
    
    int m_nCurIndex;
    bool m_bSwitching;
};

class CDungeonScene : public CCScene
{
public:
    CDungeonScene();
    ~CDungeonScene();
    
    virtual bool init();
    CREATE_FUNC(CDungeonScene);
    
protected:
    CDungeonLayer *m_pMainLayer;

};

#endif // ___DUNGEON_SCENE_H__
