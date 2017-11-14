//酒馆-招募
#ifndef ___TAVERN_SCENE_H__
#define ___TAVERN_SCENE_H__

#include "GameInclude.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define TAVERNSCENE_TAG_JSON_ROOT           (101)
#define TAVERSCENE_TAG_TABHERO_BTN          (102)
#define TAVERSCENE_TAG_TABPET_BTN           (103)
#define TAVERSCENE_TAG_OPERATION_BTN        (104)
#define TAVERNSCENE_TAG_TABHERO_LAYOUT      (105)
#define TAVERNSCENE_TAG_TABPET_LAYOUT       (106)
#define TAVERNSCENE_TAG_TABHERO_PAGEVIEW    (107)
#define TAVERNSCENE_TAG_TABPET_PAGEVIEW     (108)
#define TAVERN_SCENE_TAG_LEFT_ARROW           (109)
#define TAVERN_SCENE_TAG_RIGHT_ARROW          (110)
#define TAVERN_SCENE_TAG_MONEYITEM_GOLD     (111)
#define TAVERN_SCENE_TAG_MONEYITEM_GEM      (112)

#define TAVERN_SCENE_TAG_LOAD_LAYER         (300)
#define TAVERN_SCENE_TAG_LOAD_WORD          (301)
#define TAVERN_SCENE_TAG_LOAD_BAR           (302)

#pragma mark - CTavernLayer

class CTavernLayer : public CCLayer
{
public:
	CTavernLayer(void);
	~CTavernLayer(void);
    
	virtual bool init();
	CREATE_FUNC(CTavernLayer);
    virtual void onEnter();
    virtual void onExit();
    virtual void update(float delta);
    
protected:
    void LoadInit();
    void UIInit(int nStep);
    UIPageView *createHeroPage();
    UIPageView *createPetPage();
    void initHeroPage(Layout* pPageLayout, int nHeroId);
    void initPetPage(Layout* pPageLayout, int nPetId);
    void SetState(int goalState);
    
    void freshTabIndex();
    void freshOperationData();
    void freshLevelStarUI();
    void freshOperationUI(int nId, int nType);
    void freshArrowUI();
    void freshMoneyProp();
    void freshHeadUI();
    
    void CBClose(CCObject* pSender);
    void CBLeft(CCObject* pSender);
    void CBRight(CCObject* pSender);
    void CBTabSwitch(CCObject* pSender);
    void CBWaitOperation(float dt);
    void CBToOperation(CCObject* pSender);
    void CBPageViewTurningEvent(CCObject *pSender);
    void CBHpButton(CCObject* pSender);
    
    void operationHero();
    void operationPet();
    void unlockHero(int nHeroId);
    void unlockPet(int nPetId);
    void recruitHero(int nHeroId);
    void recruitPet(int nPetId);
    void trainHero(int nHeroId);
    void trainPet(int nPetId);
    
    void successEffect();
    bool handleMsg(const Message &msg);
    
private:
    enum ETavernState
    {
        TAVERN_STATE_NULL = 0,
        TAVERN_STATE_LOAD,
        TAVERN_STATE_UI,
        TAVERN_STATE_DIRECT,//不经过load直接进入
        TAVERN_STATE_LIMIT
    };
    
    enum ETabType
    {
        ETAB_TYPE_NONE,
        ETAB_TYPE_HERO,
        ETAB_TYPE_PET,
    };
    
    enum ETavernSceneZOrder
    {
        TAVERN_ZORDER_BACKGROUND = 0,   //背景图
        TAVERN_ZORDER_MASK,     //遮罩
        TAVERN_ZORDER_MONEY_ITEM,
        TAVERN_ZORDER_JSON,
        TAVERN_ZORDER_MENU,
        TAVERN_ZORDER_SUCCESS_EFFECT,//解锁、升级成功粒子效果
    };//场景图层
    
private:
    UILayer* m_pUILayer;
    int m_tabLayoutZOrder_1;
    int m_tabLayoutZOrder_2;
    int m_tabButtonZOrder_1;
    int m_tabButtonZOrder_2;
    CCDictionary* m_dicHero;
    CCDictionary* m_dicPet;
    
    ETabType m_eTabType;
    int m_nCurPageIndex;
    int m_nCurPageTag;
    ETavernOperationType m_nOperationIndex;
    
    int m_eTavernState;
    bool m_bUIReady;
    float m_fLoadPercentage;
    bool m_bLockOperationBtn;//防止疯狂连续点击
};

#pragma mark - CTavernScene

class CTavernScene : public CCScene
{
public:
    virtual bool init();
    
    CREATE_FUNC(CTavernScene);
    
public:
	CTavernLayer *m_pMainLayer;
    
};


#endif // ___TAVERN_SCENE_H__
