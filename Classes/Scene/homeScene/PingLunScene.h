//评论有礼
#ifndef ___PINGLUN_SCENE_H__
#define ___PINGLUN_SCENE_H__

#include "GameInclude.h"

USING_NS_CC;
USING_NS_CC_EXT;

enum _EPingLunSceneZOrder
{
    PINGLUNSCENE_ZORDER_BG = 0,
    PINGLUNSCENE_ZORDER_MASK,
    PINGLUNSCENE_ZORDER_JSON,
    PINGLUNSCENE_ZORDER_MENU,
    PINGLUNSCENE_ZORDER_LIMIT
};

enum _EPingLunSceneTag
{
    PINGLUNSCENE_TAG_BG = 3000,
    PINGLUNSCENE_TAG_MASK,
    PINGLUNSCENE_TAG_JSON,
    PINGLUNSCENE_TAG_LIMIT
};

class CPingLunLayer : public CCLayer
{
public:
    CPingLunLayer();
    ~CPingLunLayer();
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(CPingLunLayer);
    
    void CBClose(CCObject* pSender);
    void CBYes(CCObject* pSender);
    void CBNo(CCObject* pSender);

private:
    UILayer* m_pUILayer;
};

class CPingLunScene : public CCScene
{
public:
    CPingLunScene();
    ~CPingLunScene();
    
    virtual bool init();
    CREATE_FUNC(CPingLunScene);
    
private:
    CPingLunLayer *m_pMainLayer;
};

#endif // ___SETTING_SCENE_H__
