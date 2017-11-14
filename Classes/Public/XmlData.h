//
//  XmlData.h
//  FishingJoy
//
//  Created by Ringo_D on 12-9-25.
//
//

#ifndef __FishingJoy__XmlData__
#define __FishingJoy__XmlData__

#include "cocos2d.h"


#define xStr(key)       XmlData::sharedXmlData()->stringFromKey(key)
#define ccint(key)      XmlData::sharedXmlData()->intFromKey(key)
#define ccboolean(key)  XmlData::sharedXmlData()->booleanFromKey(key)


USING_NS_CC;
using namespace std;

class XmlData
{
public:    
    static XmlData* sharedXmlData();

    const char* stringFromKey(string key);
    int intFromKey(string key);
    bool booleanFromKey(string key);
    CCPoint pointFromKey(string key);
    CCRect rectFromKey(string key);
    CCSize sizeFromKey(string key);
    
    /**
     *	@brief	 内存不足时调用
     */
    //void purge();
    
    CC_SYNTHESIZE_READONLY(string, _staticDataPath, XmlDataPath);

protected:
    CCDictionary* _dictionary;
private:
    XmlData();
    ~XmlData();
    bool init();
};
#endif /* defined(__FishingJoy__XmlData__) */
