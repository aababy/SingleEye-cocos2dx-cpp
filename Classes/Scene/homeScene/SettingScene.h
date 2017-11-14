//设置场景
#ifndef ___SETTING_SCENE_H__
#define ___SETTING_SCENE_H__

#include "GameInclude.h"

USING_NS_CC;
USING_NS_CC_EXT;

enum _ESettingSceneZOrder
{
    SETTINGSCENE_ZORDER_BG = 0,
    SETTINGSCENE_ZORDER_MASK,
    SETTINGSCENE_ZORDER_JSON,
    SETTINGSCENE_ZORDER_LIMIT
};

enum _ESettingSceneTag
{
    SETTINGSCENE_TAG_BG = 3000,
    SETTINGSCENE_TAG_MASK,
    SETTINGSCENE_TAG_JSON,
    SETTINGSCENE_TAG_LIMIT
};

class CSettingLayer : public CCLayer
{
public:
    CSettingLayer();
    ~CSettingLayer();
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(CSettingLayer);
    
    void CBClose(CCObject* pSender);
    void CBSwitchSound(CCObject* pSender);
    void CBSwitchMusic(CCObject* pSender);

    void toSwitchSound(bool bOpen);
    void toSwitchMusic(bool bOpen);
    
private:
    UILayer* m_pUILayer;

    bool m_bOpenSoundEffectBak;
    bool m_bOpenMusicBak;
};

class CSettingScene : public CCScene
{
public:
    CSettingScene();
    ~CSettingScene();
    
    virtual bool init();
    CREATE_FUNC(CSettingScene);
    
private:
    CSettingLayer *m_pMainLayer;
};

#endif // ___SETTING_SCENE_H__
