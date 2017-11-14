//主菜单
#ifndef ___HOME_SCENE_H__
#define ___HOME_SCENE_H__

#include "GameInclude.h"
#include "HomeCloud.h"
#include "HomeUILayer.h"
#include "HomeControlLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;

#pragma mark - CHomeLayer

class CHomeUILayer;
class CHomeControlLayer;
class CTeachLayer;

enum EHomeSceneZOrder
{
    HOMESCENE_ZORDER_BG = 0,
    HOMESCENE_ZORDER_CLOUD,
    HOMESCENE_ZORDER_ASTER,
    HOMESCENE_ZORDER_UI,
    HOMESCENE_ZORDER_TEACH,
    HOMESCENE_ZORDER_LIMIT
};

#define HOMESCENE_TAG_BG    (5000)

class CHomeLayer : public CCLayer
{
public:
    CHomeLayer();
    ~CHomeLayer();
    
    virtual bool init();
	CREATE_FUNC(CHomeLayer);
    
    virtual void onEnter();
    virtual void onExit();
    
    bool isNight();
    bool handleMsg(const Message &msg);
    
    void CBTeachEnd();
    void syncWithServer();
    void checkPingLun(float dt);
public:
    CHomeUILayer *m_pUILayer;
    CHomeControlLayer *m_pControlLayer;
    CTeachLayer *m_pTeachLayer;
    CCArray *m_pCloudArray;
};

#pragma mark - CHomeScene

class CHomeScene : public CCScene
{
public:
    CHomeScene(void);
    ~CHomeScene(void);
    
    virtual bool init();
    CREATE_FUNC(CHomeScene);
    
public:
	CHomeLayer *m_pMainLayer;
};


#endif // ___HOME_SCENE_H__
