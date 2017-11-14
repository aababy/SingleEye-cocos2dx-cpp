//启动加载
#ifndef __LOAD_SCENE_H__
#define __LOAD_SCENE_H__

#include "GameInclude.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define LOAD_SCENE_TAG_PROCESSBAR   (1000)
#define LOAD_SCENE_TAG_WORD         (1000 + 1)

typedef enum _ELoadSceneZOrder
{
    LOAD_SCENE_ZORDER_BG = 0,
    LOAD_SCENE_ZORDER_PROCESS_FRAME,
    LOAD_SCENE_ZORDER_PROCESS_BAR,
    LOAD_SCENE_ZORDER_WORD,
    LOAD_SCENE_ZORDER_LIMIT
}ELoadSceneZOrder;

class CLoadLayer : public CCLayer
{
public:
    CLoadLayer();
    ~CLoadLayer();
    
    virtual bool init();
    CREATE_FUNC(CLoadLayer);
    
    virtual void onEnter();
    virtual void onExit();
    virtual void update(float delta);
    
public:
    void toNext();
    
protected:
    float m_fPercentage;
};

class CLoadScene : public CCScene
{
public:
    CLoadScene();
    ~CLoadScene();
    
    virtual bool init();
    CREATE_FUNC(CLoadScene);
    
public:
	CLoadLayer *m_pMainLayer;
};

#endif // __LOAD_SCENE_H__
