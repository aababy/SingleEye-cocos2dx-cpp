//
//  XmlData.cpp
//  FishingJoy
//
//  Created by Ringo_D on 12-9-25.
//
//
#include "XmlData.h"

USING_NS_CC;
using namespace std;

#define STATIC_DATA_PATH "Script/strings.xml"


//static XmlData* _sharedXmlData = NULL;
XmlData* XmlData::sharedXmlData()
{
    static XmlData instance;
//    if(_sharedXmlData == NULL){
//        _sharedXmlData = new XmlData();
//        _sharedXmlData->init();
//    }
    return &instance;
}


XmlData::XmlData()
{
    _staticDataPath = STATIC_DATA_PATH;
	init();
}


XmlData::~XmlData()
{
    CC_SAFE_RELEASE_NULL(_dictionary);
}


bool XmlData::init()
{
    _dictionary = CCDictionary::createWithContentsOfFile(_staticDataPath.c_str());
    CC_SAFE_RETAIN(_dictionary);
    return true;
}


const char* XmlData::stringFromKey(string key)
{
    return _dictionary->valueForKey(key)->getCString();
}


int XmlData::intFromKey(string key)
{
    return _dictionary->valueForKey(key)->intValue();
}


bool XmlData::booleanFromKey(string key)
{
    return _dictionary->valueForKey(key)->boolValue();
}


CCPoint XmlData::pointFromKey(string key)
{
    return CCPointFromString(_dictionary->valueForKey(key)->getCString());
}


CCRect XmlData::rectFromKey(string key)
{
    return CCRectFromString(_dictionary->valueForKey(key)->getCString());
}


CCSize XmlData::sizeFromKey(string key)
{
    return CCSizeFromString(_dictionary->valueForKey(key)->getCString());
}


//void XmlData::purge()
//{
//    CC_SAFE_RELEASE_NULL(_sharedXmlData);
//}