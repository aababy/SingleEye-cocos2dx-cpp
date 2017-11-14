#include "SettingScene.H"

USING_NS_CC;
USING_NS_CC_EXT;

#pragma mark - CSettingLayer

CSettingLayer::CSettingLayer()
{
    
}
CSettingLayer::~CSettingLayer()
{

}

bool CSettingLayer::init()
{
    return (CCLayer::init());
}
void CSettingLayer::onEnter()
{
    do {
        CCLayer::onEnter();
        
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("soundSetting.plist", "soundSetting.pvr.ccz");
        
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        
        //bg
        CCSprite *pBg = CCSprite::create(background_2);
        pBg->setPosition(ccp(winSize.width/2, winSize.height/2));
        addChild(pBg, SETTINGSCENE_ZORDER_BG);
        
        //mask
        CCLayerColor *pMask = CCLayerColor::create(ccc4(0,0,0,110));
        addChild(pMask, SETTINGSCENE_ZORDER_MASK);
        
        //content
        m_pUILayer = UILayer::create();
        addChild(m_pUILayer, SETTINGSCENE_ZORDER_JSON);
        
        //Layout from json
        Layout *pJsonRoot = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile("HomeScene/SoundSetting.json"));
        CC_BREAK_IF(!pJsonRoot);
        pJsonRoot->setPosition(ccp(winSize.width/2 - pJsonRoot->getContentSize().width/2, winSize.height/2 - pJsonRoot->getContentSize().height/2));
        pJsonRoot->setTag(SETTINGSCENE_TAG_JSON);
        m_pUILayer->addWidget(pJsonRoot);
        
        //close btn
        UIButton *pCloseBtn = dynamic_cast<UIButton*>(pJsonRoot->getChildByName("Button_Close"));
        CC_BREAK_IF(!pCloseBtn);
        pCloseBtn->addReleaseEvent(this, coco_releaseselector(CSettingLayer::CBClose));
        
        //sound btn
        UIButton *pSoundBtn = dynamic_cast<UIButton*>(pJsonRoot->getChildByName("Button_Sound"));
        CC_BREAK_IF(!pSoundBtn);
        pSoundBtn->addReleaseEvent(this, coco_releaseselector(CSettingLayer::CBSwitchSound));
    
        //music btn
        UIButton *pMusicBtn = dynamic_cast<UIButton*>(pJsonRoot->getChildByName("Button_Music"));
        CC_BREAK_IF(!pMusicBtn);
        pMusicBtn->addReleaseEvent(this, coco_releaseselector(CSettingLayer::CBSwitchMusic));
        
        //sound on & off
        toSwitchSound(xData->m_bOpenSoundEffect);
        toSwitchMusic(xData->m_bOpenBackMusic);
        
        //bak
        m_bOpenSoundEffectBak = xData->m_bOpenSoundEffect;
        m_bOpenMusicBak = xData->m_bOpenBackMusic;
        
    } while (0);
    
}

void CSettingLayer::onExit()
{
    if (m_bOpenSoundEffectBak != xData->m_bOpenSoundEffect
        || m_bOpenMusicBak != xData->m_bOpenBackMusic)
    {
        DATAPOOL->saveArchive();
    }
    
    CCLayer::onExit();
    HelpMethod::removeUnusedResourceAtExit(this);
    
}

void CSettingLayer::CBClose(CCObject* pSender)
{
    xData->playKeytone();
    xSM->back();
}
void CSettingLayer::CBSwitchSound(CCObject* pSender)
{
    xData->playKeytone();
    DATAPOOL->m_bOpenSoundEffect = !DATAPOOL->m_bOpenSoundEffect;
    toSwitchSound(DATAPOOL->m_bOpenSoundEffect);
}
void CSettingLayer::CBSwitchMusic(CCObject* pSender)
{
    xData->playKeytone();

    DATAPOOL->m_bOpenBackMusic = !DATAPOOL->m_bOpenBackMusic;
    toSwitchMusic(DATAPOOL->m_bOpenBackMusic);

    if (DATAPOOL->m_bOpenBackMusic)
    {
    //    if (!CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
        {
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(home_scene_back_music,true);
        }
    }
    else
    {
    //    if (CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
        {
            CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();// stopBackgroundMusic(false);
        }
    }
}

void CSettingLayer::toSwitchSound(bool bOpen)
{
    UIImageView *pSoundIco = dynamic_cast<UIImageView*>(m_pUILayer->getWidgetByName("ImageView_SoundIco"));
    if(pSoundIco)
    {
        if(bOpen)
        {
            pSoundIco->loadTexture("setting_sound_open.png", UI_TEX_TYPE_PLIST);
        }
        else
        {
            pSoundIco->loadTexture("setting_sound_close.png", UI_TEX_TYPE_PLIST);
        }
    }
    
    UIImageView *pSoundOn = dynamic_cast<UIImageView*>(m_pUILayer->getWidgetByName("ImageView_SoundOn"));
    if(pSoundOn)
    {
        pSoundOn->setVisible(bOpen);
    }

    UIImageView *pSoundOff = dynamic_cast<UIImageView*>(m_pUILayer->getWidgetByName("ImageView_SoundOff"));
    if (pSoundOff)
    {
        pSoundOff->setVisible(!bOpen);
    }
}
void CSettingLayer::toSwitchMusic(bool bOpen)
{
    UIImageView *pMusicIco = dynamic_cast<UIImageView*>(m_pUILayer->getWidgetByName("ImageView_MusicIco"));
    if(pMusicIco)
    {
        if(bOpen)
        {
            pMusicIco->loadTexture("setting_music_open.png", UI_TEX_TYPE_PLIST);
        }
        else
        {
            pMusicIco->loadTexture("setting_music_close.png", UI_TEX_TYPE_PLIST);
        }
    }
    
    UIImageView *pMusicOn = dynamic_cast<UIImageView*>(m_pUILayer->getWidgetByName("ImageView_MusicOn"));
    if(pMusicOn)
    {
        pMusicOn->setVisible(bOpen);
    }
    
    UIImageView *pMusicOff = dynamic_cast<UIImageView*>(m_pUILayer->getWidgetByName("ImageView_MusicOff"));
    if (pMusicOff)
    {
        pMusicOff->setVisible(!bOpen);
    }
}

#pragma mark - CSettingScene

CSettingScene::CSettingScene()
{
    m_pMainLayer = NULL;
}
CSettingScene::~CSettingScene()
{
    
}
bool CSettingScene::init()
{
    bool bRst = false;
    
    do
    {
        CC_BREAK_IF(!CCScene::init());
        
        m_pMainLayer = CSettingLayer::create();
		CC_BREAK_IF(!m_pMainLayer);
		addChild(m_pMainLayer);
        
        bRst = true;
        
    } while (0);

    return bRst;
}

