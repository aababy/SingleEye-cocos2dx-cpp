//首页
#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__

#include "GameInclude.h"

USING_NS_CC;
USING_NS_CC_EXT;

#pragma mark - CTitleLayer

class CTitleLayer : public CCLayer
{
public:
    CTitleLayer();
    ~CTitleLayer();
    
	virtual bool init();
	CREATE_FUNC(CTitleLayer);
    
    void onEnter();
    void onExit();
    
    void CBMenuStart(CCObject* pSender);
    
public:
    UILayer *m_pUILayer;
};

#pragma mark - CTitleScene

class CTitleScene : public CCScene
{
public:
    CTitleScene(void);
    ~CTitleScene(void);
    
    virtual bool init();
    CREATE_FUNC(CTitleScene);
    
public:
	CTitleLayer *m_pMainLayer;
    
};

#endif // __TITLE_SCENE_H__
