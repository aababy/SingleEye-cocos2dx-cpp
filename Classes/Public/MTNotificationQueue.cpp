//
//  MTNotificationQueue.cpp
//  Demo
//
//  Created by KodeLover on 8/25/13.
//
//

#include "MTNotificationQueue.h"

pthread_mutex_t sharedNotificationQueueLock;

class LifeManager_PThreadMutex {
    
private:
    
    pthread_mutex_t* m_pMutex;
    
public:
    
    LifeManager_PThreadMutex(pthread_mutex_t* in_pMutex) : m_pMutex(in_pMutex) {
        pthread_mutex_init(m_pMutex, NULL);
    }
    
    ~LifeManager_PThreadMutex() {
        pthread_mutex_destroy(m_pMutex);
    }
    
}__LifeManager_sharedNotificationQueueLock(&sharedNotificationQueueLock);

class LifeCircleMutexLocker {
    
private:
    
    pthread_mutex_t* m_pMutex;
    
public:
    
    LifeCircleMutexLocker(pthread_mutex_t* in_pMutex) : m_pMutex(in_pMutex) {
        pthread_mutex_lock(m_pMutex);
    }
    
    ~LifeCircleMutexLocker() {
        pthread_mutex_unlock(m_pMutex);
    }
};

#define LifeCircleMutexLock(mutex) LifeCircleMutexLocker __locker__(mutex)

MTNotificationQueue* MTNotificationQueue::sm_pInstance = NULL;

MTNotificationQueue* MTNotificationQueue::getInstance() {
	if (!sm_pInstance) {
		sm_pInstance = new MTNotificationQueue();
	}
	return sm_pInstance;
}

MTNotificationQueue::MTNotificationQueue() {}

MTNotificationQueue::~MTNotificationQueue() {}

void MTNotificationQueue::postNotifications(float in_fDeltaTime) {
    LifeCircleMutexLock(&sharedNotificationQueueLock);
    
    CCNotificationCenter* t_pNotiCenter = CCNotificationCenter::sharedNotificationCenter();
	for (uint16_t i = 0; i < m_oVecNotifications.size(); i ++) {
        NotificationArgs& tmp_oNotiArgs = m_oVecNotifications[i];
        t_pNotiCenter->postNotification(tmp_oNotiArgs.m_oStrName.c_str(), tmp_oNotiArgs.m_pCcObj);
    }
    m_oVecNotifications.clear();
}


void MTNotificationQueue::postNotification(const char* in_pArrCharName, CCObject* in_pCcObj) {
    LifeCircleMutexLock(&sharedNotificationQueueLock);
    
    NotificationArgs t_oNotiArgs;
    t_oNotiArgs.m_oStrName = in_pArrCharName;
    
    if (in_pCcObj) {
    	t_oNotiArgs.m_pCcObj = in_pCcObj;   // object->copy();
    } else {
        t_oNotiArgs.m_pCcObj = NULL;
    }
    
    m_oVecNotifications.push_back(t_oNotiArgs);
}