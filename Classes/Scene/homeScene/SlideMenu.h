//滑出菜单
#ifndef __SingleEye__SlideMenu__
#define __SingleEye__SlideMenu__

#include "GameInclude.h"

USING_NS_CC;
USING_NS_CC_EXT;

typedef enum
{
    SLIDEMENU_STATE_CLOSE = 0,  //关闭
    SLIDEMENU_STATE_OPEN,       //开启
    SLIDEMENU_STATE_SLIDE,      //滑动中
    SLIDEMENU_STATE_LIMIT
}ESlideMenuState;

enum ESlideMenuTag
{
    SLIDEMENU_TAG_MENU = 1000,
    SLIDEMENU_TAG_CALL_ITEM,
    SLIDEMENU_TAG_ITEM_MUSIC,
    SLIDEMENU_TAG_ITEM_SOUND,
    SLIDEMENU_TAG_LIMIT
};

class CSlideMenu : public CCNode
{
public:
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(CSlideMenu);
    
    const CCSize& getContentSizeWithCloseState();
    void toShow();
    void toHide();
    
private:
    void CBMenuCall(CCObject *pSender);
    void CBSheZhi(CCObject *pSender);
    void CBTongZhi(CCObject *pSender);
    void CBHuoDong(CCObject *pSender);
    void CBAbout(CCObject *pSender);
    
    void openSlideOver();
    void closeSlideOver();
    void newMessage(CCObject *obj);
    
private:
    CCPoint m_pBirthPosition;
    CCSize m_contentSizeWithCloseState;
    float m_fSlideArea;
    ESlideMenuState m_eSlideState;
    CCMenuItemImage *pTongZhi;
    
    bool m_bIsDown;
};

#endif /* defined(__SingleEye__SlideMenu__) */
