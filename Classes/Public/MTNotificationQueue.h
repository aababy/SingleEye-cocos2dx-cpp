//
//  MTNotificationQueue.h
//  Demo
//
//  Created by KodeLover on 8/25/13.
//
//

#ifndef __Demo__MTNotificationQueue__
#define __Demo__MTNotificationQueue__

#include "cocos2d.h"
#include <pthread.h>
#include <vector>

using namespace cocos2d;
using namespace std;

#define NC_Dialog       "ShowDialog"
#define NC_SystemBusy   "SystemBusy"
#define NC_ShareSuccess "ShareSuccess"
#define NC_ShareFinish  "ShareFinish"


#define xMTNotify      (MTNotificationQueue::getInstance())


class MTNotificationQueue : public CCObject {
    
public:
    
    static MTNotificationQueue* getInstance();
    
    void postNotifications(float in_fDeltaTime);
    
    void postNotification(const char* in_pArrCharName, CCObject* in_pCcObj);
    
private:
    
    MTNotificationQueue();
    
    ~MTNotificationQueue();
    
private:
    
	static MTNotificationQueue* sm_pInstance;
    
 	class CGarbo {
    public:
        ~CGarbo() {
            if (MTNotificationQueue::sm_pInstance) {
                delete MTNotificationQueue::sm_pInstance;
            }
            sm_pInstance = NULL;
        }
 	};
    
 	static CGarbo sm_oCGarbo;
    
    typedef struct {
        string m_oStrName;
        CCObject* m_pCcObj;
    } NotificationArgs;
    
    vector<NotificationArgs> m_oVecNotifications;
};

#endif /* defined(__Demo__MTNotificationQueue__) */