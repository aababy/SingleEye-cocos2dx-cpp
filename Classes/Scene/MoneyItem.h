//
//  MoneyItem.h
//  SingleEye
//
//  Created by wangwx on 13-12-19.
//
//

#ifndef __SingleEye__MoneyItem__
#define __SingleEye__MoneyItem__

#include "cocos2d.h"
#include "cocos-ext.h"


USING_NS_CC;
USING_NS_CC_EXT;

#pragma mark - CMoneyItem

typedef enum _EMoneyItemType
{
    MONEY_ITEM_TYPE_GOLD = 0,   //金币
    MONEY_ITEM_TYPE_GEM,        //宝石
    MONEY_ITEM_TYPE_POWER,      //体力
    MONEY_ITEM_TYPE_LIMIT
}EMoneyItemType;


typedef void (CCObject::*SEL_MoneyItemEvent)(EMoneyItemType m_nType);
#define moneyitemeventselector(_SELECTOR) (SEL_MoneyItemEvent)(&_SELECTOR)


class CMoneyItem : public CCNode
{
public:
    CMoneyItem(EMoneyItemType type, bool haveAddFun = true);
    ~CMoneyItem();
    
    void CBAdd(CCObject *pSender);
    void setNum(int nGoal);
    void setCallbackFun(CCObject* target, SEL_MoneyItemEvent callfun);
public:
    EMoneyItemType m_nType;
    
    UILayer *m_pUILayer;
    
    CCObject* m_callbackListener;
    SEL_MoneyItemEvent m_callback;
};

#endif /* defined(__SingleEye__MoneyItem__) */
