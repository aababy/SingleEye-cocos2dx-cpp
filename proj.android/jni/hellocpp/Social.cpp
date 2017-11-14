//
//  YourJni.cpp
//  test04
//
//  Created by luoxp on 4/14/14.
//
//

#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include "UserData.h"
#include "Social.h"

USING_NS_CC;

Social* Social::getInstance()
{
    static Social inst;

    return &inst;
}


Social::~Social()
{

}


void Social::init()
{
	//Android 暂时不需要处理
}


void Social::share(const char *shareTxt, const char *shareImage)
{
    JniMethodInfo methodInfo;

    bool isHave = JniHelper::getStaticMethodInfo(methodInfo,
    		"com/gwsoft/singleeye/SingleEye", "openShareBoard", "(Ljava/lang/String;)V");

    if (isHave)
    {         
        jstring jshareTxt = methodInfo.env->NewStringUTF(shareTxt);
                        
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jshareTxt);
    }
}


void Social::setLandscape()
{

}


void Social::stat(const char *eventId)
{
	statByKey(eventId, NULL);
}


void Social::statByUserID(const char *eventId)
{
	int userId = UserData::Instance()->pUserInfo->userId;

	char buf[30];
	sprintf(buf, "%d", userId);

	statByKey(eventId, buf);
}


void Social::statByKey(const char *eventId, const char *eventKey)
{
	JniMethodInfo methodInfo;
	jobject jobj;

	bool isHave = JniHelper::getStaticMethodInfo(methodInfo,
			"com/gwsoft/singleeye/SingleEye", "getActivity", "()Ljava/lang/Object;");

    if (isHave)
    {
    	jobj = methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);

    	isHave = JniHelper::getMethodInfo(methodInfo,
        		"com/gwsoft/singleeye/SingleEye", "mobclickAgentnEvent", "(Ljava/lang/String;Ljava/lang/String;)V");

    	if(isHave)
    	{
    		jstring jeventId = methodInfo.env->NewStringUTF(eventId);
    		jstring jeventKey;

    		if(eventKey != NULL)
    		{
    			jeventKey = methodInfo.env->NewStringUTF(eventKey);
    		}
    		else
    		{
    			jeventKey = methodInfo.env->NewStringUTF("");
    		}

    		methodInfo.env->CallVoidMethod(jobj, methodInfo.methodID, jeventId, jeventKey);
    	}
    }
}


void Social::statByKeyForCalc(const char *eventId, int iNumber, char *chapter)
{

}


void Social::showAD()
{

}


void Social::hideAD()
{

}


void Social::setRootView(void *vRootView)
{

}





