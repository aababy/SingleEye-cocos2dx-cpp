//活动
#ifndef __SingleEye__HuodongScene__
#define __SingleEye__HuodongScene__

#include "GameInclude.h"

typedef enum _EHuodongSceneZOrder
{
    HUODONGSCENE_ZORDER_BACKGROUND = 0,
    HUODONGSCENE_ZORDER_MENU,
    HUODONGSCENE_ZORDER_LIMIT
}EHuodongSceneZOrder;

class CHuodongLayer : public CCLayer
{
public:
    CHuodongLayer();
    ~CHuodongLayer();
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(CHuodongLayer);
    
    void CBQianDao(CCObject *pSender);
    void CBBossZhan(CCObject *pSender);
    void CBShare(CCObject *pSender);
    void CBBack(CCObject *pSender);
};

class CHuodongScene : public CCScene
{
public:
    CHuodongScene();
    ~CHuodongScene();
    
    virtual bool init();
    CREATE_FUNC(CHuodongScene);
    
public:
    CHuodongLayer *m_pMainLayer;
};

#endif /* defined(__SingleEye__HuodongScene__) */
