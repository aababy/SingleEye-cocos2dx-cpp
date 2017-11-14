//
//  Share.h
//  SingleEye
//
//  Created by luoxp on 3/20/14.
//
//

#ifndef __SingleEye__Share__
#define __SingleEye__Share__

#include "GameInclude.h"


USING_NS_CC;
USING_NS_CC_EXT;


#define xShare          (Share::getInstance())


typedef bool (CCObject::*SEL_ShareEvent)(int iResponse);
#define shareeventselector(_SELECTOR) (SEL_ShareEvent)(&_SELECTOR)


class Share : public CCNode {
        
public:
    static Share * getInstance();
    void snsShare(const char *shareTxt, const char *shareImage);
    void snsShare(const char *shareTxt, const char *shareImage, SEL_ShareEvent onShare, CCObject* target);
    
private:
    Share();
    ~Share();
    void onNotification(CCObject *obj);
    
    CCObject*           m_target;
    SEL_ShareEvent      m_onShare;
};


#endif /* defined(__SingleEye__Share__) */
