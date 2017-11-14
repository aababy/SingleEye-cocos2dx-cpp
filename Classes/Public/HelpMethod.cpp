//
//  HelpMethod.cpp
//  SingleEye
//
//  Created by qinfen on 13-11-1.
//
//

#include "HelpMethod.h"


float HelpMethod::getAngle(float param1, float param2)  
{
    float angle = 0;
    if (param1 == 0.0f&& -param2 > 0.0f) {
        angle = 0;
    }
    while (true)
    {
        if (param1>0.0f) {
            angle = (float)(atanf(-param2/param1)*(180/PI)) - 90;
        }
        else
        {
            angle = 90.0f + (float)(atanf(-param2/param1)*(180/PI));
        }
        return angle;
    }
    return angle;
}

float HelpMethod::getAngleDX(float angle, float length)
{
    return (float)(length * cosf(angle * (PI/180)));
}

float HelpMethod::getAngleDY(float angle, float length)
{
    return -(float)(length * sinf(angle * (PI/180)));
}

float HelpMethod::getAngleAccelerateX(float angle, float accelerate)
{
    return (accelerate * sinf(angle * (PI/180)));
}

float HelpMethod::getAngleAccelerateY(float angle, float accelerate)
{
    return (accelerate * cosf(angle * (PI/180)));
}

float HelpMethod::getReboundAngle(float angle)
{
    float _angle = 0.0f;
    if (angle < 0)
    {
        angle *= -1;
        _angle = angle;
    }
    else if (angle > 0)
    {
        
    }
    
    return _angle;
}


UIButton * HelpMethod::initUIButton(int aTag, SEL_TouchEvent onButton, UIWidget *pRoot, CCObject *target)
{
#if COCOS2D_DEBUG > 0
    UIButton *ui = dynamic_cast<UIButton*>(pRoot->getChildByTag(aTag));
#else
    UIButton *ui = (UIButton*)(pRoot->getChildByTag(aTag));
#endif
    CCAssert(ui != NULL, "Children is NULL, check Tag");
    ui->addTouchEventListener(target, onButton);
    ui->setTouchEnable(true);
    ui->setPressedActionEnabled(true);

    return ui;
}



UICheckBox* HelpMethod::initUICheckBox(int aTag, SEL_SelectedStateEvent onCheckBox, UIWidget *pRoot, CCObject *target)
{
    UICheckBox *ui = (UICheckBox*)pRoot->getChildByTag(aTag);
    ui->addEventListenerCheckBox(target, onCheckBox);
    ui->setTouchEnable(true);

    return ui;
}


UITextField* HelpMethod::initUITextField(int aTag, SEL_TextFieldEvent onTextField, UIWidget *pRoot, CCObject *target)
{
    UITextField *ui = (UITextField*)pRoot->getChildByTag(aTag);
    ui->addEventListenerTextField(target, onTextField);
    ui->setTouchEnable(true);
    
    return ui;
}


char * HelpMethod::itoa(int iNum, char *buf)
{
    sprintf(buf, "%d", iNum);
    return buf;
}


void HelpMethod::pauseNodeActions(cocos2d::CCNode *node)
{
    for (int i = 0; i<node->getChildrenCount(); ++i)
    {
        CCNode *child = (CCNode*)node->getChildren()->objectAtIndex(i);
        if(child)
        {
            pauseNodeActions(child);
            child->pauseSchedulerAndActions();
        }
    }
    node->pauseSchedulerAndActions();
}


void HelpMethod::resumeNodeActions(cocos2d::CCNode *node)
{
    for (int i = 0; i<node->getChildrenCount(); ++i)
    {
        CCNode *child = (CCNode*)node->getChildren()->objectAtIndex(i);
        if(child)
        {
            resumeNodeActions(child);
            child->resumeSchedulerAndActions();
        }
    }
    node->resumeSchedulerAndActions();
}


int HelpMethod::cycleNum(int iCount, bool bAdd, int *idx)
{
    if (bAdd == true)
    {
        if (*idx >= iCount - 1)
        {
            *idx = 0;
        }
        else
        {
            (*idx)++;
        }
    }
    else if (bAdd == false)
    {
        if ((*idx) <= 0)
        {
            (*idx) = iCount - 1;
        }
        else
        {
            (*idx)--;
        }
    }
    
    return (*idx);
}


const char* HelpMethod::strToIgnore(string &str, int iLen)
{
    if (str.length() > iLen) {
        char buf[iLen + 1];
        sprintf(buf, "%s..", str.substr(0, iLen - 2).c_str());
        str = string(buf);
    }
    
    return str.c_str();
}


void HelpMethod::adapt(UIWidget *pRoot, const char *bgFileName)
{
    CCSize designSize = CCSizeMake(640, 960);   //设计尺寸
    CCSize screenSize = CCEGLView::sharedOpenGLView()->getFrameSize();              //实际尺寸
    
    //设置背景图片
    UIPanel *panel = dynamic_cast<UIPanel*>(pRoot);
    panel->setBackGroundImage(bgFileName, UI_TEX_TYPE_PLIST);
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();    
    pRoot->setPosition(ccp((size.width - pRoot->getContentSize().width)/2, (size.height - pRoot->getContentSize().height)/2));
        
    //其它微调控件,扩展半透明Panel
    float fHeight = designSize.width * screenSize.height / screenSize.width;
    
    UIPanel *uOpacity = dynamic_cast<UIPanel*>(pRoot->getChildByTag(41));
    if (uOpacity) {
        uOpacity->setSize(CCSizeMake(uOpacity->getContentSize().width, fHeight));        
        uOpacity->setPosition(ccp(uOpacity->getPosition().x, uOpacity->getPosition().y - (fHeight - designSize.height)/2));        
    }    
    
    
    UIButton *uBack = dynamic_cast<UIButton*>(pRoot->getChildByTag(27));
    if (uBack) {                
        uBack->setPosition(ccp(uBack->getPosition().x, uBack->getPosition().y - (fHeight - designSize.height)/2));
    }
    
#if 0
    //调整每一个二级节点的坐标(不包括三级节点, 四级节点..),暂时不用这种方案
    CCArray *array = pRoot->getChildren();
    CCObject *object = NULL;
    CCARRAY_FOREACH(array, object)
    {
        UIWidget* ui = (UIWidget*) object;
        
        //判断是否是全屏的控件
        if (ui->getSize().equals(designSize)) {
            ui->setSize(screenSize);
            continue;
        }

        
        //获取中心点
        CCPoint uiPoint = ui->getPosition();
        CCPoint anchor = ui->getAnchorPoint();
        CCSize uiSize = ui->getSize();
        CCPoint offsetPoint(uiSize.width * (0.5f - anchor.x), uiSize.height * (0.5f - anchor.y));
        uiPoint = ccpAdd(uiPoint, offsetPoint);
        
        //变换
        uiPoint.x *= screenSize.width / designSize.width;
        uiPoint.y *= screenSize.height / designSize.height;
        
        uiPoint = ccpSub(uiPoint, offsetPoint);
        ui->setPosition(uiPoint);
    }
#endif    
    
    return;
}

CCPoint HelpMethod::getCenterPoint(CCNode *hero,CCNode *enemy)
{
    CCPoint pos1 = hero->getPosition();
    CCPoint pos2 = enemy->getPosition();
    float _x = 0;
    float _y = 0;
    
    float dx = pos1.x - pos2.x;
    if(dx<0)
    {
        dx *= -1;
    }
    
    float dy = pos1.y - pos2.y;
    if(dy<0)
    {
        dy *= -1;
    }
    
    if(pos1.x<pos2.x)
    {
        _x = pos1.x + dx /2;
    }
    else
    {
        _x = pos2.x + dx /2;
    }
    
    if(pos1.y<pos2.y)
    {
        _y = pos1.y + dy /2;
    }
    else
    {
        _y = pos2.y + dy /2;
    }

    return ccp(_x,_y);
}


//星期数从 0（星期一）到 6（星期天）
void HelpMethod::getWeekDay(int *weekday, int *day)
{
    //获取当前时间
    unsigned long long timestamp = time(NULL);
    
    struct tm *tm;
    tm = localtime((time_t*)&timestamp);
    
    *day = tm->tm_mday;
    *weekday = tm->tm_wday;
    
    cccycleNum(7, false, weekday);              //星期一为0
}


void HelpMethod::removeUnusedResourceAtExit(CCObject *obj)
{
    SceneReader::sharedSceneReader()->purgeSceneReader();
    GUIReader::shareReader()->purgeGUIReader();
	ActionManager::purgeActionManager();
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    MsgSystem::Instance()->ClearObjHandler(obj);
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(obj);   
}


void HelpMethod::addTitle(UILayer * pUILayer, const char *name)
{
    UIImageView *uTitle = UIImageView::create();
    uTitle->setTexture("gyong_10.png", tPlist);
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();    
    uTitle->setPosition(ccp(size.width/2, size.height - uTitle->getSize().height/2));
    
    UIImageView *uName = UIImageView::create();
    uName->setTexture(name, tPlist);
    uName->setPosition(ccp(7, -20));
    
    uTitle->addChild(uName);
    pUILayer->addWidget(uTitle);
}


void HelpMethod::checkRetainCount(cocos2d::CCArray *array)
{
    return;
    for(int i = 0;i<array->count();i++)
    {
        CCNode *child = (CCNode*)array->objectAtIndex(i);
        if(child)
        {
            CCLOG("child retain count:%d  index:%d",child->retainCount(),i);
            CCAssert(child->retainCount() < 2, "child wouldn't release!");
        }
    }
}

bool HelpMethod::isTimeToPingLun()
{
    struct tm *tm;
    time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    time(&timep);
#else
    struct cc_timeval now;
    CCTime::gettimeofdayCocos2d(&now, NULL);
    timep = now.tv_sec;
#endif
     return ((timep - xData->m_runTime.tv_sec) > TO_PINGLUN_TIME_INTERVAL);
//    return ((timep - xData->m_runTime.tv_sec) > 10*60);//10分钟
}

bool HelpMethod::isFileExist(const char *pFileName)
{
    if( !pFileName ) return false;
    
    //strFilePathName is :/data/data/ + package name
    std::string filePath =  CCFileUtils::sharedFileUtils()->getWritablePath();
    filePath += pFileName;
    CCLog("filePath:%s",filePath.c_str());
    FILE *fp = fopen(filePath.c_str(),"r");
    if(fp)
    {
        fclose(fp);
        CCLog("file exist!");
        return true;
    }
    return false;
}

void HelpMethod::copyData(const char *pFileName, const char *pFolder)
{
    std::string path;
    path.append(pFolder);
    path.append("/");
    path.append(pFileName);
    CCLog("path:%s",path.c_str());
    std::string strPath =  CCFileUtils::sharedFileUtils()->fullPathForFilename(path.c_str());
    unsigned long len = 0;
    unsigned char *data = NULL;
    CCLog("strPath:%s",strPath.c_str());
    data = CCFileUtils::sharedFileUtils()->getFileData(strPath.c_str(),"r",&len);
    CCLog("Length:%lu",len);
    std::string destPath = CCFileUtils::sharedFileUtils()->getWritablePath();
    CCLog("destPath:%s",destPath.c_str());
    destPath += pFileName;
    FILE *fp = fopen(destPath.c_str(),"w+");
    fwrite(data,sizeof(char),len,fp);
    fclose(fp);
    delete []data;
    data = NULL;
}

void HelpMethod::deleteData(const char *pFileName)
{
	std::string destPath = CCFileUtils::sharedFileUtils()->getWritablePath();
	destPath += pFileName;
	remove(destPath.c_str());
}
