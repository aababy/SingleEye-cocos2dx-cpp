//
//  HelpMethod.h
//  SingleEye
//
//  Created by qinfen on 13-11-1.
//
//

#ifndef __SingleEye__HelpMethod__
#define __SingleEye__HelpMethod__

#include "GameInclude.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define     ccitoa(iNum, buf)               HelpMethod::itoa(iNum, buf)
#define     cccycleNum(iCount, bAdd, idx)   HelpMethod::cycleNum(iCount, bAdd, idx)
#define     ccstrToIgnore(...)              HelpMethod::strToIgnore(__VA_ARGS__)


#define     xImageView(aTag)                (UIImageView*)(m_pRootLayer->getChildByTag(aTag))

struct ccTime
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    long millSecond;
};


#define PI (3.14159)
class HelpMethod
{
public:
    static float getAngle(float param1, float param2);  //param1: 现在的x点减去初始x点  param2:y
    static float getAngleDX(float angle, float length);
    static float getAngleDY(float angle, float length);
    static float getAngleAccelerateX(float angle, float accelerate);
    static float getAngleAccelerateY(float angle, float accelerate);
    static float getReboundAngle(float angle);  //获得反弹角度
    
    static void  pauseNodeActions(CCNode *node);
    static void  resumeNodeActions(CCNode *node);

    static UIButton* initUIButton(int aTag, SEL_TouchEvent onButton, UIWidget *pRoot, CCObject *target);
    static UICheckBox* initUICheckBox(int aTag, SEL_SelectedStateEvent onCheckBox, UIWidget *pRoot, CCObject *target);
    static UITextField* initUITextField(int aTag, SEL_TextFieldEvent onTextField, UIWidget *pRoot, CCObject *target);
    static char * itoa(int iNum, char *buf);
    
    static int cycleNum(int iCount, bool bAdd, int *idx);
    static const char* strToIgnore(string &str, int iLen);      //这个函数会改变原来的值
    static void adapt(UIWidget *pRoot, const char *bgFileName);
    
    static CCPoint getCenterPoint(CCNode *hero,CCNode *enemy);
    static void getWeekDay(int *weekday, int *day);
    
    static void removeUnusedResourceAtExit(CCObject *obj);

    static void addTitle(UILayer * pUILayer, const char *name);
    static void checkRetainCount(CCArray *array);
    
    static bool isTimeToPingLun();
    
    static bool isFileExist(const char* pFileName);
    static void copyData(const char* pFileName, const char *pFolder);
    static void deleteData(const char* pFileName);

};

#endif /* defined(__SingleEye__HelpMethod__) */
