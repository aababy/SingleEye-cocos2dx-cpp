//云朵
#ifndef __SingleEye__HomeCloud__
#define __SingleEye__HomeCloud__

#include "GameInclude.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define HOME_CLOUD_TYPE_NUM     (5)         //云朵种类
#define HOME_CLOUD_MOVE_FREQ    (0.01)     //移动间隔

class CHomeCloud : public cocos2d::CCNode
{
public:
    virtual bool init();
    
    CREATE_FUNC(CHomeCloud);
    
    void cloudMove(float dt);
    virtual void onEnter();
    virtual void onExit();
    
public:
    float m_fSpeed;
};

class CHomeCloudNode : public CCNode
{
public:
    virtual bool init();
    CREATE_FUNC(CHomeCloudNode);
};

#endif /* defined(__SingleEye__HomeCloud__) */
