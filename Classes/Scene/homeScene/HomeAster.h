//旋转星球
#ifndef __SingleEye__HomeAster__
#define __SingleEye__HomeAster__

#include "GameInclude.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define HOME_ASTER_TAG_ITEM_BASE   (3000)
#define HOME_ASTER_TAG_ITEM_1   (HOME_ASTER_TAG_ITEM_BASE + 1)
#define HOME_ASTER_TAG_ITEM_2   (HOME_ASTER_TAG_ITEM_BASE + 2)
#define HOME_ASTER_TAG_ITEM_3   (HOME_ASTER_TAG_ITEM_BASE + 3)
#define HOME_ASTER_TAG_ITEM_4   (HOME_ASTER_TAG_ITEM_BASE + 4)
#define HOME_ASTER_TAG_ITEM_5   (HOME_ASTER_TAG_ITEM_BASE + 5)

class CHomeAster : public cocos2d::CCNode
{
public:
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    CREATE_FUNC(CHomeAster);
    
    void menuStartCallback1(CCObject* pSender);
    void menuStartCallback2(CCObject* pSender);
    void menuStartCallback3(CCObject* pSender);
    void menuStartCallback4(CCObject* pSender);
    void menuStartCallback5(CCObject* pSender);
    
    void clickHotItem(int flag);
    CCPoint getItemPosition(int index);
    
private:
    cocos2d::CCNode *m_pMainBody;
};

#endif /* defined(__SingleEye__HomeAster__) */
