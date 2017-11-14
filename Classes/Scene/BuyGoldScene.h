//
//  BuyGoldScene.h
//  SingleEye
//
//  Created by wangwx on 13-12-16.
//
//
//购买金币
#ifndef __SingleEye__BuyGoldScene__
#define __SingleEye__BuyGoldScene__

#include "GameInclude.h"

#pragma mark - CBuyGoldLayer

#define BUYGOLD_TAG_TABLE    (1000)
#define BUYGOLD_TAG_TOPPROP_GOLD     (1000 + 1)
#define BUYGOLD_TAG_TOPPROP_GEM      (1000 + 2)

#define BUYGOLD_CELL_NUM     (5)

typedef enum _EBuyGoldSceneZOrder
{
    BUYGOLDSCENE_ZORDER_BACKGROUND = 0,
    BUYGOLDSCENE_ZORDER_PROP,
    BUYGOLDSCENE_ZORDER_MENU,
    BUYGOLDSCENE_ZORDER_TABLE,
    BUYGOLDSCENE_ZORDER_LIMIT
}EBuyGoldSceneZOrder;

class CBuyGoldTableViewCell : public CCTableViewCell
{
public:
    CBuyGoldTableViewCell(int num, int price);
//	virtual void draw();
public:
    int m_nNum;
    int m_nPrice;
    
    UILayer *m_pUILayer;
};

class CBuyGoldLayer : public CCLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
    CBuyGoldLayer(void);
    ~CBuyGoldLayer(void);
    
    virtual bool init();
    CREATE_FUNC(CBuyGoldLayer);
    virtual void onEnter();
    virtual void onExit();
    
    void CBBack(CCObject *pSender);
    
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
    virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
    virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    
private:
    void freshPropUI();
    
private:
    float m_nTableWidth;
    float m_nTableHeight;
};

#pragma mark - CBuyGoldScene

class CBuyGoldScene : public CCScene
{
public:
    CBuyGoldScene();
    ~CBuyGoldScene();
    
    virtual bool init();
    CREATE_FUNC(CBuyGoldScene);
    
public:
    CBuyGoldLayer *m_pMainLayer;
};

#endif /* defined(__SingleEye__BuyGoldScene__) */
