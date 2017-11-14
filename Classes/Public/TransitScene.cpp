//
//  TransitScene.cpp
//  SingleEye
//
//  Created by luoxp on 2/25/14.
//
//

#include "TransitScene.h"



TransitScene::TransitScene()
{   
    
}


CCScene* TransitScene::scene(SceneData &data)
{
    CCScene* scene = NULL;
    do {
        scene = CCScene::create();
        CC_BREAK_IF(!scene);
        
        TransitScene *layer = TransitScene::create(data);
        scene->addChild(layer);
    } while (0);
    
    return scene;
}


TransitScene* TransitScene::create(SceneData &data)
{ 
    TransitScene *pRet = new TransitScene(); 
    if (pRet && pRet->init(data)) 
    { 
        pRet->autorelease();
        return pRet; 
    } 
    else 
    { 
        delete pRet; 
        pRet = NULL; 
        return NULL; 
    } 
}


bool TransitScene::init(SceneData &data)
{
    if (CCLayer::init()) {
        m_tData = data;
        
        //截图
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        CCRenderTexture* screen = CCRenderTexture::create(size.width, size.height);
        CCScene* pRunScene = CCDirector::sharedDirector()->getRunningScene();
        screen->begin();
        pRunScene->visit();
        screen->end();
        screen->setPosition(ccp(size.width/2, size.height/2));                
        addChild(screen);
        
        CCLayerColor *color = CCLayerColor::create(ccc4(0, 0, 0, 128));                
        addChild(color);
                
        CCSprite *loading = CCSprite::createWithSpriteFrameName("zrz.png");
        loading->setPosition(ccp(size.width/2, size.height/2));        
        addChild(loading);
    }
    
    return true;
}


void TransitScene::onEnter()
{
    CCLayer::onEnter();
    schedule(schedule_selector(TransitScene::onTest), 0.f);
}


//void TransitScene::onEnterTransitionDidFinish()
//{
//    CCLayer::onEnterTransitionDidFinish();
//    schedule(schedule_selector(TransitScene::onTest), 0.f);
//}


void TransitScene::onTest(float dt)
{ 
    unschedule(schedule_selector(TransitScene::onTest)); 
    
    xSM->backFromTrans(m_tData);
}


void TransitScene::onExit()
{
    CCLayer::onExit();    
    unscheduleAllSelectors();
}
