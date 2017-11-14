//剧情动画
#ifndef ___STORY_SCENE_H__
#define ___STORY_SCENE_H__

#include "GameInclude.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define STORY_SCENE_TAG_ANIMATION_BASE   (1000)

typedef enum _EStorySceneZOrder
{
    STORY_SCENE_ZORDER_BG = 0,
    STORY_SCENE_ZORDER_STORY,
    STORY_SCENE_ZORDER_MENU,
    STORY_SCENE_ZORDER_LIMIT
}EStorySceneZOrder;

class CStoryLayer : public CCLayer
{
public:
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(CStoryLayer);
    
    void CBSkip(CCObject* pSender);
    void CBStepEnd(cocos2d::extension::CCArmature *armature, cocos2d::extension::MovementEventType movementType, const char *movementID);
    void CBPlayArmature();
private:
    void setStep(int step);
    void nextStoryStep();
    void toNext();
    
private:
    byte m_uStepCount;
};

class CStoryScene : public CCScene
{
public:
    CStoryScene();
    ~CStoryScene();
    
    virtual bool init();
    CREATE_FUNC(CStoryScene);
    
private:
    CStoryLayer *m_pMainLayer;
};

#endif // ___STORY_SCENE_H__
