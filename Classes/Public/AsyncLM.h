//
//  AsyncLM.h
//  SingleEye
//
//  Created by luoxp on 1/23/14.
//
//

#ifndef __SingleEye__AsyncLM__
#define __SingleEye__AsyncLM__

#include "GameInclude.h"


USING_NS_CC;
USING_NS_CC_EXT;

#define xxAsync         (AsyncLM::getInstance())


typedef enum
{
    aTypeUnknown = 0,
    aTypePvr,
    aTypeJson,

    aTypeCount
}ALMType;


typedef enum
{
    aStateNone = 0,
    aStateFinish,
    ENUM02,
    ENUM03,
    ENUM_COUNT
}ALMState;


class AsyncLM;

struct ALMData {
    int         ID;
    string      sName;               //无后缀, plist pvr 必须使用相同名字.
    string      sImageName;
    string      sPListName;
    ALMState    eState;
    ALMType     eType;
    CCTexture2D *texture;    
    
    void *pvrClass;                  //only for ios pvr
    unsigned char *pvrData;
};


class AsyncLM : public CCObject{

public:
    static AsyncLM * getInstance();
        
    void addFile(const char * sName);    
    void start(SEL_CallFuncO onALoadFinish, CCObject *target);
    void parserFileType(ALMData *data);
    
private:
    AsyncLM();
    ~AsyncLM();
    vector<ALMData*> m_vData;
    void update(float dt);    
    void onLoad(CCObject* obj);
    void clearvData();
    int  m_ID;
    
    SEL_CallFuncO m_onALoadFinish;
    CCObject*     m_target;
    bool          m_bFinished;
    unsigned int           m_eState;
    
    void onNotifyBefore(CCObject* sender);
    void onNotifyLater(CCObject* sender);
    
    static void* onThreadBefore(void* arg);
    static void* onThreadLater(void* arg);
};



#endif /* defined(__SingleEye__AsyncLM__) */
