//
//  Share.cpp
//  SingleEye
//
//  Created by luoxp on 3/20/14.
//
//

#include "Share.h"


Share * Share::getInstance()
{
    static Share instance;
    
    return &instance;
}


Share::Share()
{
    CCNode::init();
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(Share::onNotification), NC_ShareFinish, NULL);
}


Share::~Share()
{
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}


void Share::onNotification(CCObject *obj)
{
    xBlock->unblock();
    
    CCInteger *response = (CCInteger*)obj;
    
    //10以内是保留字
    if (response->getValue() < 10) {
        int iCount = xData->getShareCounts();
        xData->setShareCounts(iCount + 1);
        
        iCount = xUGInt("today_share_count");
        xUSInt("today_share_count", iCount + 1);
        xSocial->statByUserID(SShare);
        //1 表示facebook 或者 twitter
        if (response->getValue() == 1) {
            xDialog->show(xStr("share_success"));
        }
    }
    
    if (m_onShare) {
        (m_target->*m_onShare)(response->getValue());
    }
    
    //必须释放
    obj->release();
}


void Share::snsShare(const char *shareTxt, const char *shareImage)
{
    m_onShare = NULL;  
    
    int iCount = xUGInt("today_share_count");
    
#ifdef SIGN_DEBUG
    if (true) {
#else    
    if (iCount <= 2) {
#endif
        xSocial->share(shareTxt, shareImage);
    }
    else
    {
        xDialog->show(xStr("share_limit"));
    }     
}


void Share::snsShare(const char *shareTxt, const char *shareImage, SEL_ShareEvent onShare, CCObject* target)
{
    m_target = target;
    m_onShare = onShare;
    
    int iCount = xUGInt("today_share_count");
    
#ifdef SIGN_DEBUG
    if (true) {
#else    
    if (iCount <= 2) {
#endif
        xSocial->share(shareTxt, shareImage);
    }
    else
    {
        xDialog->show(xStr("share_limit"));
    }     
}





