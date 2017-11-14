//
//  Loading.h
//  SingleEye
//
//  Created by gwsoft on 1/16/14.
//
//

#ifndef __SingleEye__Loading__
#define __SingleEye__Loading__

#include "GameInclude.h"


USING_NS_CC;
USING_NS_CC_EXT;


#define xLoading        Loading::getInstance()


class Loading : public CCLayer
{
public:
    static Loading * getInstance();

    void start();
    void start(bool bBlocked);
    void stop();
    void setEnabled(bool bEnabled);
    
private:
    bool init();
    Loading();
    void registerWithTouchDispatcher();
    void onExit();
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void scheduleSetBlockedState(float dt);
    void setBlockedState(bool bBlocked);    
    
    CCArmature* m_aLoading;
    bool bEnabled;
    bool m_bBlocked;
};


#endif /* defined(__SingleEye__Loading__) */
