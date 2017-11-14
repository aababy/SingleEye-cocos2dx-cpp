//
//  Timer.cpp
//  SingleEye
//
//  Created by luoxp on 4/3/14.
//
//

#include "Timer.h"


Timer * Timer::getInstance()
{
    static Timer instance;
    
    return &instance;
}


Timer::Timer()
{

}


Timer::~Timer()
{
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(Timer::checkIsTomorrow), this);
}


bool Timer::init()
{        
    CCNode::init();
    
    checkIsTomorrow(0.f);
    
    //10分钟检查一次
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(Timer::checkIsTomorrow), this, 600.f, false);    
    
    return true;
}


void Timer::checkIsTomorrow(float dt)
{
    //获取当前时间
    unsigned long long timestamp = time(NULL);
    
    struct tm *tm;
    tm = localtime((time_t*)&timestamp);
    int year = tm->tm_year + 1900;
    int month = tm->tm_mon + 1;
    int day = tm->tm_mday;
    
    char sTime[15];
    sprintf(sTime, "%04d-%02d-%02d", year, month, day);
    
    string sOldTime = cocos2d::CCUserDefault::sharedUserDefault()->getStringForKey(ULoginTime);
    
    cocos2d::CCUserDefault::sharedUserDefault()->setStringForKey(ULoginTime, sTime);
    
    if (sOldTime.compare(sTime) != 0) {
        //不匹配, 表示需要发送事件
        xSocial->statByUserID(SLogin);
        CCUserDefault::sharedUserDefault()->setIntegerForKey("today_share_count", 0);
    }        
}


