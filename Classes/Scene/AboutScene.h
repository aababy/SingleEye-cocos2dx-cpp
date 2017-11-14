//about信息
#ifndef __ABOUT_SCENE_H__
#define __ABOUT_SCENE_H__

#include "GameInclude.h"

USING_NS_CC;
USING_NS_CC_EXT;

#pragma mark - CAboutLayer

class CAboutLayer : public CCLayer
{
public:
	virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(CAboutLayer);
    
    void CBBack(CCObject* pSender);
    
};

#pragma mark - CAboutScene

class CAboutScene : public CCScene
{
public:
    CAboutScene(void);
    ~CAboutScene(void);
    
    virtual bool init();
    CREATE_FUNC(CAboutScene);
    
public:
	CAboutLayer *m_pMainLayer;
    
};

#endif // __ABOUT_SCENE_H__
