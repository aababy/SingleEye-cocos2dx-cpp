#include "AppDelegate.h"
#include "FightScene.h"
#include "LoadScene.h"

#include "DataPool.h"
#include "UserData.h"
#include "NetTest.h"
#include "support/zip_support/ZipUtils.h"
USING_NS_CC;
AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
    DATAPOOL->purgeDataPool();
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
    
    pDirector->setOpenGLView(pEGLView);
	
    // turn on display FPS
    pDirector->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);
    
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //固定宽度
    CCEGLView::sharedOpenGLView()->setDesignResolutionSize(640, 960, kResolutionFixedWidth);
	#endif

    //chency
    EngineSystem::SharedEngineSystem();
    //chency 让Cocos2d知道pvr图像使用了预先计算出来的alpha值
    CCTexture2D::PVRImagesHavePremultipliedAlpha(true);
    // 数据的初始化
    UserData::Instance()->init(USER_ID);

    //读取存档中的userData相关数据,必须在UserData init之后，NetWorker init之前
    DATAPOOL->loadEnemyData();
    DATAPOOL->loadHeroData();
    DATAPOOL->loadOutfitData();

    DATAPOOL->initEnemyRecord();
    DATAPOOL->initHeroRecord();
    DATAPOOL->initOutfitRecord();
    DATAPOOL->initLevelRecord();

    xData->loadArchive();
    xData->manualArchive();
    
    // 网络初始化
    NetWorker::Instance()->Init(SERVER_IP, SERVER_PORT, SERVER_DOMAIN_NAME, SERVER_DOMAIN_PORT);
    // 测试网络协议
#ifdef NET_TEST
    NetTest::Instance()->init();
    NetTest::Instance()->begin();
#endif
    
    //友盟初始化

    xSocial->init();
    xTimer->init();
    //test result page
//    CCScene *scene = CCScene::create();
//    ResultLayer *layer = ResultLayer::create();
//    scene->addChild(layer);
//    layer->showWin(3, 29, 32, 200, NULL);
////    layer->showLose(3, 3, 3, NULL);
//    pDirector->runWithScene(scene);
    ZipUtils::ccSetPvrEncryptionKeyPart(0 , 0x5822e500);
    ZipUtils::ccSetPvrEncryptionKeyPart(1 , 0xc5344f90);
    ZipUtils::ccSetPvrEncryptionKeyPart(2 , 0x26154d5e);
    ZipUtils::ccSetPvrEncryptionKeyPart(3 , 0x127e299b);
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        if(!HelpMethod::isFileExist("levelinfo.lua"))
        {
            HelpMethod::copyData("levelinfo.lua","Script");
        }
        if(!HelpMethod::isFileExist("txt.lua"))
        {
        	HelpMethod::copyData("txt.lua","Script");
        }
#endif
    xSM->forward(ST_Load);
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
     NetWorker::Instance()->Pause();
    CCDirector::sharedDirector()->stopAnimation();
//    CCDirector::sharedDirector()->pause();
    DATAPOOL->saveArchive();
    // if you use SimpleAudioEngine, it must be pause
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    
    DATAPOOL->WriteCheckData();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();
//    CCDirector::sharedDirector()->resume();
    // if you use SimpleAudioEngine, it must resume here
    if (xData->m_bOpenSoundEffect)
    {
        SimpleAudioEngine::sharedEngine()->resumeAllEffects();
    }
    
    if(xData->m_bOpenBackMusic&&!DATAPOOL->m_bGamePause)
    {
        SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    }
    
    //校验存档数据和内存数据是否一致，防修改器
    DATAPOOL->checkData();
    NetWorker::Instance()->Resume();
}





