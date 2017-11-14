#include "MTNotificationQueue.h"
#include "Social.h"
#include "com_gwsoft_singleeye_SingleEye.h"


JNIEXPORT void JNICALL Java_com_gwsoft_singleeye_SingleEye_onResponse
  (JNIEnv *, jobject, jint eCode)
{
	CCInteger *res = CCInteger::create((int)eCode);

    //retain 后, 必须要释放
    res->retain();

    xMTNotify->postNotification(NC_ShareFinish, res);
}
