//
//  TransitScene.h
//  SingleEye
//
//  Created by luoxp on 2/25/14.
//
//

#ifndef __SingleEye__TransitScene__
#define __SingleEye__TransitScene__


#include "GameInclude.h"


USING_NS_CC;
USING_NS_CC_EXT;


class TransitScene : public CCLayer
{
public:
    TransitScene();

    virtual bool init(SceneData &data);
    static TransitScene* create(SceneData &data);
    static CCScene* scene(SceneData &data);
    
    void onEnter();
    //void onEnterTransitionDidFinish();
    
    void onExit();       
    
    void onTest(float dt);
    
    SceneData m_tData;
};

#endif /* defined(__SingleEye__TransitScene__) */
