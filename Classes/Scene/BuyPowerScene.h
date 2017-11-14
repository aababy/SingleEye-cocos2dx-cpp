//
//  BuyGemScene.h
//  SingleEye
//
//  Created by wangwx on 13-12-16.
//
//
//购买宝石
#ifndef __SingleEye__BuyPowerScene__
#define __SingleEye__BuyPowerScene__

#include "GameInclude.h"

#pragma mark - CBuyPowerLayer

#define BUYPOWER_TAG_TABLE    (1000)
#define BUYPOWER_TAG_TOPPROP_GOLD     (1000 + 1)
#define BUYPOWER_TAG_TOPPROP_POWER      (1000 + 2)

#define BUYPOWER_CELL_NUM     (4)

typedef enum _EBuyPowerSceneZOrder
{
    BUYPOWERSCENE_ZORDER_BACKGROUND = 0,
    BUYPOWERSCENE_ZORDER_PROP,
    BUYPOWERSCENE_ZORDER_MENU,
    BUYPOWERSCENE_ZORDER_TABLE,
    BUYPOWERSCENE_ZORDER_LIMIT
}EBuyPowerSceneZOrder;

class CBuyPowerTableViewCell : public CCTableViewCell
{
public:
    CBuyPowerTableViewCell(int gemNum, int rmbPrice);
//	virtual void draw();
public:
    int m_nPowerNum;
    int m_nRmbPrice;
    
    UILayer *m_pUILayer;
};

class CBuyPowerLayer : public CCLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
    CBuyPowerLayer(void);
    ~CBuyPowerLayer(void);
    
    virtual bool init();
    CREATE_FUNC(CBuyPowerLayer);
    virtual void onEnter();
    virtual void onExit();
    
    void CBBack(CCObject *pSender);
    
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
    virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
    virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    void getPowerNumber(int idx, int *nPower, int *nRmb);
    void onDialog(int id, int iTag);
    
    void freshPower();
    bool handleMsg(const Message &msg);
    
    float m_nTableWidth;
    float m_nTableHeight;
    
    int  m_iCost;
    int  m_iPower;
private:

    
private:

};

#pragma mark - CBuyPowerScene

class CBuyPowerScene : public CCScene
{
public:
    CBuyPowerScene();
    ~CBuyPowerScene();
    
    virtual bool init();
    CREATE_FUNC(CBuyPowerScene);

    
public:
    CBuyPowerLayer *m_pMainLayer;
};

#endif /* defined(__SingleEye__BuyPowerScene__) */
