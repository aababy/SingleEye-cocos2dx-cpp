//
//  BuyGemScene.h
//  SingleEye
//
//  Created by wangwx on 13-12-16.
//
//
//购买宝石
#ifndef __SingleEye__BuyGemScene__
#define __SingleEye__BuyGemScene__

#include "GameInclude.h"

#pragma mark - CBuyGemLayer

#define BUYGEM_TAG_TABLE    (1000)
#define BUYGEM_TAG_TOPPROP_GOLD     (1000 + 1)
#define BUYGEM_TAG_TOPPROP_GEM      (1000 + 2)

#define BUYGEM_CELL_NUM     (3)

typedef enum _EBuyGemSceneZOrder
{
    BUYGEMSCENE_ZORDER_BACKGROUND = 0,
    BUYGEMSCENE_ZORDER_PROP,
    BUYGEMSCENE_ZORDER_MENU,
    BUYGEMSCENE_ZORDER_TABLE,
    BUYGEMSCENE_ZORDER_LIMIT
}EBuyGemSceneZOrder;

class CBuyGemTableViewCell : public CCTableViewCell
{
public:
    CBuyGemTableViewCell(int gemNum, int rmbPrice, ELocalId localId);
//	virtual void draw();
public:
    int m_nGemNum;
    int m_nRmbPrice;
    ELocalId m_eLocal;
    
    UILayer *m_pUILayer;
};

class CBuyGemLayer : public CCLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
    CBuyGemLayer(void);
    ~CBuyGemLayer(void);
    
    virtual bool init();
    CREATE_FUNC(CBuyGemLayer);
    virtual void onEnter();
    virtual void onExit();
    
    void CBBack(CCObject *pSender);
    
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
    virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
    virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    
    bool handleMsg(const Message &msg);
    
private:
    void freshPropUI();
    
private:
    float m_nTableWidth;
    float m_nTableHeight;
    ELocalId m_eLocal;
};

#pragma mark - CBuyGemScene

class CBuyGemScene : public CCScene
{
public:
    CBuyGemScene();
    ~CBuyGemScene();
    
    virtual bool init();
    CREATE_FUNC(CBuyGemScene);
    
public:
    CBuyGemLayer *m_pMainLayer;
};

#endif /* defined(__SingleEye__BuyGemScene__) */
