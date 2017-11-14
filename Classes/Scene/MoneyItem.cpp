//
//  MoneyItem.cpp
//  SingleEye
//
//  Created by wangwx on 13-12-19.
//
//
#include "GameInclude.h"
#include "MoneyItem.h"


#pragma mark - CMoneyItem

#define offset              (25)

CMoneyItem::CMoneyItem(EMoneyItemType type, bool haveAddFun):CCNode()
{
    m_nType = type;
    
    m_callbackListener = NULL;
    m_callback = NULL;
    
    do {
        m_pUILayer = UILayer::create();
        CC_BREAK_IF(!m_pUILayer);
        addChild(m_pUILayer);
        
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(money_item_plist, money_item_image);
        
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        
        Layout *pJsonRoot = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(top_money_item_json));
        CC_BREAK_IF(!pJsonRoot);
        m_pUILayer->addWidget(pJsonRoot);
        this->setContentSize(pJsonRoot->getContentSize());
        
        UIImageView *pIco = dynamic_cast<UIImageView*>(pJsonRoot->getChildByName("ImageView_Ico"));
        CC_BREAK_IF(!pIco);
        
        UILabelAtlas *pNum = dynamic_cast<UILabelAtlas*>(pJsonRoot->getChildByName("LabelAtlas_Num"));
        CC_BREAK_IF(!pNum);
        
        UIButton *pBtn = dynamic_cast<UIButton*>(pJsonRoot->getChildByName("Button_Buy"));
        CC_BREAK_IF(!pBtn);
        pBtn->addReleaseEvent(this, coco_releaseselector(CMoneyItem::CBAdd));
        if(haveAddFun)
        {
            pBtn->active();
            pBtn->setVisible(true);

        }
        else
        {
            pBtn->disable();
            pBtn->setVisible(false);
        }
        
        switch (type)
        {
            case MONEY_ITEM_TYPE_GOLD:
            {
                //pos
                this->setPosition(0, winSize.height - this->getContentSize().height);
                
                //ico
                pIco->loadTexture("money_ico04.png", UI_TEX_TYPE_PLIST);
                
                //num
                pNum->setStringValue(CCString::createWithFormat("%d", DATAPOOL->getGoldCoin())->getCString());
                
            }
                break;
            case MONEY_ITEM_TYPE_GEM:
            {
                //pos
                this->setPosition(this->getContentSize().width, winSize.height - this->getContentSize().height);
                
                //ico
                pIco->loadTexture("money_ico03.png", UI_TEX_TYPE_PLIST);
                
                //num
                pNum->setStringValue(CCString::createWithFormat("%d", xUser->pUserInfo->gemstone)->getCString());
            }
                break;
            case MONEY_ITEM_TYPE_POWER:
            {
                //pos
                this->setPosition(0, winSize.height - this->getContentSize().height);
                
                //ico
                pIco->loadTexture("money_ico05.png", UI_TEX_TYPE_PLIST);
                
                //num
                pNum->setStringValue(CCString::createWithFormat("%d", xUser->pUserInfo->strength)->getCString());
            }
                break;
        }
    } while (0);
    

}
CMoneyItem::~CMoneyItem()
{
//    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(ui_plist);
}
void CMoneyItem::CBAdd(CCObject *pSender)
{
    if (m_callbackListener && m_callback)
    {
        xData->playKeytone();
        (m_callbackListener->*m_callback)(m_nType);
        return;
    }
    
    switch (m_nType)
    {
        case MONEY_ITEM_TYPE_GOLD:
        {
            xData->playKeytone();
            xSM->forward(ST_BuyGold);
        }
            break;
        case MONEY_ITEM_TYPE_GEM:
        {
            xData->playKeytone();
            xSM->forward(ST_BuyGem);
        }
            break;
        case MONEY_ITEM_TYPE_POWER:
        {
            
        }
            break;
            
        default:
            break;
    }
}
void CMoneyItem::setNum(int nGoal)
{
    UILabelAtlas *pNum = dynamic_cast<UILabelAtlas*>(m_pUILayer->getWidgetByName("LabelAtlas_Num"));
    if (pNum)
    {
        pNum->setStringValue(CCString::createWithFormat("%d", nGoal)->getCString());
    }
}
void CMoneyItem::setCallbackFun(CCObject* target, SEL_MoneyItemEvent callfun)
{
    m_callbackListener = target;
    m_callback = callfun;
}