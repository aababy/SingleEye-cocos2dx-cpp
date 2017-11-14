//
//  AsyncLM.cpp
//  SingleEye
//
//  Created by luoxp on 1/23/14.
//
//

#include "AsyncLM.h"


#define suf_pvr                 ".pvr"
#define suf_json                ".exportjson"
#define suf_ccz                 ".ccz"
#define suf_plist               ".plist"


typedef enum
{
    allStateNone = 0,
    allStateRunning,
    allStateFinished,
    allStateCount,
}ALLState;


AsyncLM * AsyncLM::getInstance()
{
    static AsyncLM instance;
    return &instance;
}


AsyncLM::AsyncLM()
{
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AsyncLM::onNotifyBefore), NC_AsyncBeforeFinish, NULL);
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AsyncLM::onNotifyLater), NC_AsyncLaterFinish, NULL);
    
    m_eState = allStateNone;
}


AsyncLM::~AsyncLM()
{
    clearvData();
    CCDirector::sharedDirector()->getScheduler()->unscheduleUpdateForTarget(this);
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}


void AsyncLM::update(float dt)
{
    if (m_eState != allStateNone) {
        
        //检测是否所有的都被加载
        m_eState = allStateFinished;
        
        for (int i = 0; i < m_vData.size(); i++) {
            
            if (m_vData.at(i)->eState != aStateFinish) {
                m_bFinished = allStateRunning;
                break;
            }
        }
        
        //完成后通知
        if (m_eState == allStateFinished) {
            m_eState = allStateNone;
            clearvData();
            if (m_onALoadFinish) {
                (m_target->*m_onALoadFinish)(NULL);
            }
        }
    }
    

    
}


void AsyncLM::addFile(const char * sName)
{
    ALMData* data = new ALMData;
    
    data->ID = m_ID++;
    data->eState = aStateNone;
    
    data->sName = sName;
    parserFileType(data);
    
    m_vData.push_back(data);
}


void AsyncLM::start(SEL_CallFuncO onALoadFinish, CCObject *target)
{
    m_onALoadFinish = onALoadFinish;
    m_target        = target;
    
    vector<ALMData*>::iterator iter = m_vData.begin();
    
    while(iter != m_vData.end())
    {
        ALMData* data = *iter;
        
        switch (data->eType) {
            case aTypePvr:
            {
                pthread_t tid;
                pthread_create(&tid, NULL, &onThreadBefore, data);
            }
                break;
            default:
                break;
        }
        
        iter++;
    }
    
    
    m_eState = allStateRunning;
}


void AsyncLM::parserFileType(ALMData *data)
{
    data->eType = aTypeUnknown;
    
    std::string lowerCase(data->sName);
    
    for (unsigned int i = 0; i < lowerCase.length(); ++i)
    {
        lowerCase[i] = tolower(lowerCase[i]);
    }
    
    do
    {
        int index = std::string::npos;
        
        index = lowerCase.find(suf_pvr);
        if (index == std::string::npos) {
            index = lowerCase.find(suf_plist);
        }
        
        //pvr
        if (std::string::npos != index) {
            
            data->eType = aTypePvr;
            data->sImageName = data->sName.substr(0, index) + suf_pvr + suf_ccz;
            data->sPListName = data->sName.substr(0, index) + suf_plist;
            
            CCLOG("********** %s", data->sImageName.c_str());
    
            break;
        }
        
        //json
        index = lowerCase.find(suf_json);
        if (std::string::npos != index) {
            data->eType = aTypeJson;
            data->sImageName = data->sName.substr(0, index) + suf_json;
            
            break;
        }
    }while(0);
}




void* AsyncLM::onThreadBefore(void* arg){
    
    ALMData *data = (ALMData*)arg;
    
    //执行Async操作 TBD
    //data->texture = CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFileAsync(data->sImageName.c_str(), &(data->pvrClass), &(data->pvrData));
    
    xMTNotify->postNotification(NC_AsyncBeforeFinish, (CCObject*)arg);
    
    return NULL;
}


void AsyncLM::onNotifyBefore(CCObject* sender)
{
    ALMData *data = (ALMData*)sender;
    
    //gl 操作 TBD
    //CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFileGL(data->texture, data->pvrClass);
    
    pthread_t tid;
    pthread_create(&tid, NULL, &onThreadLater, data);
}


void* AsyncLM::onThreadLater(void* arg){
    
    ALMData *data = (ALMData*)arg;
    
    //TBD
    //xSprite->addSpriteFramesWithFileAsyncLater(data->sPListName.c_str(), data->sImageName.c_str(), data->texture, data->pvrClass, data->pvrData);
    
    xMTNotify->postNotification(NC_AsyncLaterFinish, (CCObject*)arg);
    
    return NULL;
}


void AsyncLM::onNotifyLater(CCObject* sender)
{
    ALMData *data = (ALMData*)sender;
    
    data->eState = aStateFinish;
}


void AsyncLM::clearvData()
{
    vector<ALMData*>::iterator iter = m_vData.begin();
    
    while(iter != m_vData.end())
    {
        ALMData *data = *iter;
        CC_SAFE_DELETE(data);
        iter++;
    }
    
    m_vData.clear();
}

