#include "SelectOutfitScene.H"

USING_NS_CC;
USING_NS_CC_EXT;

#pragma mark - outfit

CSelectOutfitLayer::CSelectOutfitLayer()
{
    
}
CSelectOutfitLayer::~CSelectOutfitLayer()
{
    
}
bool CSelectOutfitLayer::init()
{
    return(CCLayer::init());
}
void CSelectOutfitLayer::onEnter()
{
    do
    {
        CCLayer::onEnter();
        
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(plan_scene_body_plist, plan_scene_body_image);
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(tavern_scene_body_plist, tavern_scene_body_image);

        
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
        
        //bg
        CCSprite *pBg = CCSprite::create(background_2);
		pBg->setPosition(ccp(winSize.width/2, winSize.height/2));
		addChild(pBg, PLAN_OUTFIT_LAYER_ZORDER_BG);
        
        //mask
        CCLayerColor *pMask = CCLayerColor::create(ccc4(0,0,0,110));
        addChild(pMask, PLAN_OUTFIT_LAYER_ZORDER_MASK);
        
        //top bg
        CCSprite *pTopBg = CCSprite::createWithSpriteFrameName(topbar_1);
        pTopBg->setPosition(ccp(winSize.width/2, winSize.height - pTopBg->getContentSize().height/2));
        addChild(pTopBg, PLAN_OUTFIT_LAYER_ZORDER_TOPBG);
        
        //top name
        CCSprite *pTopName = CCSprite::createWithSpriteFrameName(plan_topbar_seloutfit);
        pTopName->setPosition(ccp(winSize.width/2 + 10, winSize.height  - pTopBg->getContentSize().height/2 - 12));
        addChild(pTopName, PLAN_OUTFIT_LAYER_ZORDER_TOPNAME);
        
        //menu
        CCMenuItemImage *pBackItem = CCMenuItemImage::create(NULL, \
                                                             NULL, \
                                                             this, \
                                                             menu_selector(CSelectOutfitLayer::CBClose));
        pBackItem->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(btn_back));
        pBackItem->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(btn_back2));
        pBackItem->setPosition(ccp(MIN_SPACE_TO_SCREEN_EDGE + pBackItem->getContentSize().width/2, MIN_SPACE_TO_SCREEN_EDGE + pBackItem->getContentSize().height/2));
        CCMenu* pMenu = CCMenu::create(pBackItem, NULL);
        pMenu->setPosition(CCPointZero);
        addChild(pMenu, PLAN_OUTFIT_LAYER_ZORDER_MENU);
        
        //table
        m_pUILayer = UILayer::create();
        CC_BREAK_IF(!m_pUILayer);
        addChild(m_pUILayer, PLAN_OUTFIT_LAYER_ZORDER_TABLE);
        CCPoint teachGoalCellPos = CCPoint(0, 0);
        float teachGoalRadius = 80;
        
        Layout* pJsonRootLayout = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(plan_scene_outfit_json));
        CC_BREAK_IF(!pJsonRootLayout);
        pJsonRootLayout->setPosition(ccp(winSize.width/2 - pJsonRootLayout->getContentSize().width/2, winSize.height/2 - pJsonRootLayout->getContentSize().height/2));
        m_pUILayer->addWidget(pJsonRootLayout);
        
        CCArray *pCellArray = pJsonRootLayout->getChildren();
        CC_BREAK_IF(!pCellArray);
        //    CCLOG("cell count=%d",pCellArray->count());
        //这里依赖编辑器中设定的Button_celltag依次为各个道具的id
        for (int i = 0; i < pCellArray->count(); i++)
        {
            UIPanel *pCellLayout = dynamic_cast<UIPanel*>(pCellArray->objectAtIndex(i));
            //    CCLOG("cell index=%d", i);
            if (pCellLayout)
            {
                //       CCLOG("cell content");
                UIButton *pBtn = dynamic_cast<UIButton *>(pCellLayout->getChildByName("Button_cell"));
                if(pBtn)
                {
                    int nId = pBtn->getTag();
                    bool bBeenPlan = DATAPOOL->isOutfitBeenPlan(nId);
                    
                    //光晕闪动效果
                    CCActionInterval *actionBy = CCScaleBy::create(0.8f, 0.95f);
                    CCActionInterval *actionBack = actionBy->reverse();
                    CCAction *scaleAction = CCRepeatForever::create(CCSequence::create(actionBy, actionBack, NULL));
                    
                    //         CCLOG("cell id=%d", nId);
                    UIImageView *pSel = dynamic_cast<UIImageView*>(pCellLayout->getChildByName("ImageView_select"));
                    if(pSel)
                    {
                        pSel->setVisible(bBeenPlan);
                    }
                    if (bBeenPlan)
                    {
                        pCellLayout->setBackGroundImage("plan_board5.png", UI_TEX_TYPE_PLIST);
                        pCellLayout->runAction(scaleAction);
                    }
                    else
                    {
                        pCellLayout->setBackGroundImage("plan_board4.png", UI_TEX_TYPE_PLIST);
                        pCellLayout->stopAllActions();
                    }
                    
                    
                    if (Prop_Type_HongZhaJi == nId)
                    {
                        float xOffset = winSize.width/2 - pJsonRootLayout->getContentSize().width/2;
                        float yOffset = winSize.height/2 - pJsonRootLayout->getContentSize().height/2;
                        
                        teachGoalCellPos.x = pCellLayout->getPosition().x + xOffset + pCellLayout->getContentSize().width/2;
                        teachGoalCellPos.y = pCellLayout->getPosition().y + yOffset + pCellLayout->getContentSize().height/2;
                        teachGoalRadius = pBtn->getContentSize().width/2 + 10;
                    }
                    COutfitData *pOutfitData = DATAPOOL->getOutfitData(nId);
                    if(pOutfitData)
                    {
                        pBtn->setOpacity(255);
                        pBtn->setTextures(pOutfitData->m_pSourceName->getCString(), pOutfitData->m_pSourceName->getCString(), pOutfitData->m_pSourceName->getCString(), UI_TEX_TYPE_PLIST);
                        pBtn->setTouchEnable(true);
                        pBtn->addReleaseEvent(this, coco_releaseselector(CSelectOutfitLayer::CBTouchCell));
                    }
                    
                    OutfitRecord *pOutfitRcd = DATAPOOL->getOutfitRecord(nId);
                    if (pOutfitRcd)
                    {
                        UILabelBMFont *pNumLabel = UILabelBMFont::create();
                        pNumLabel->setText(CCString::createWithFormat("X%d", pOutfitRcd->m_nNum)->getCString());
                        pNumLabel->setFntFile("fonts/num_yellow.fnt");
                        pNumLabel->setPosition(ccp(pCellLayout->getContentSize().width/2, 0));
                        pNumLabel->setZOrder(pBtn->getZOrder()+2);
                        pCellLayout->addChild(pNumLabel);
                    }
                    
                    // draw bounding box
                    //    {
                    //        CCPoint pos = getPosition();
                    //        CCSize size = CCSizeMake(100, 100);
                    //        CCPoint vertices[4]={
                    //            ccp(pos.x+1, pos.y+1),
                    //            ccp(pos.x+size.width-1, pos.y+1),
                    //            ccp(pos.x+size.width-1, pos.y+size.height-1),
                    //            ccp(pos.x+1, pos.y+size.height-1),
                    //        };
                    //        ccDrawColor4B(0, 0, 255, 255);
                    //        ccDrawPoly(vertices, 4, true);
                    //    }
                }
            }
        }
        
        //teach layer
        OutfitRecord *pRcd = xData->getOutfitRecord(Prop_Type_HongZhaJi);
        if (false == DATAPOOL->m_bTeachStepOver[TEACH_STEP_SELECT_OUTFIT]
            && pRcd
            && pRcd->m_nNum > 0)
        {
            CCSize winSize = CCDirector::sharedDirector()->getWinSize();
            m_pTeachLayer = CTeachLayer::create();
            m_pTeachLayer->setContentSize(winSize);
            m_pTeachLayer->setFocusProp(teachGoalCellPos, teachGoalRadius);
            m_pTeachLayer->setPicInfo(CCPoint(winSize.width/2, winSize.height/4), teach_to_selectOutfit);
            m_pTeachLayer->setCallbackFun(this, callfunc_selector(CSelectOutfitLayer::CBTeachEnd));
            addChild(m_pTeachLayer, PLAN_OUTFIT_LAYER_ZORDER_TEACH, PLAN_OUTFIT_LAYER_TAG_TEACH);
        }
        
    } while (0);
    
}

void CSelectOutfitLayer::onExit()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(plan_scene_body_plist);
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(tavern_scene_body_plist);
    
    CCLayer::onExit();
    HelpMethod::removeUnusedResourceAtExit(this);
}

void CSelectOutfitLayer::CBClose(CCObject *pSender)
{
    xData->playKeytone();
    xSM->back();}

void CSelectOutfitLayer::CBTouchCell(CCObject *pSender)
{
    CCLOG("CBTouchCell()");
    
    xData->playKeytone();
    
    UIButton *pBtn = dynamic_cast<UIButton*>(pSender);
    if (!pBtn)
    {
        return;
    }
    int nId = pBtn->getTag();
    if (DATAPOOL->isOutfitBeenPlan(nId))
    {
        return;
    }
    
    OutfitRecord *pOutfitRcd = DATAPOOL->getOutfitRecord(nId);
    if (NULL == pOutfitRcd)
    {
        return;
    }
    
    if (pOutfitRcd->m_nNum > 0)
    {
        DATAPOOL->setPlanOutfit(m_nGoalIndex, nId);
        DATAPOOL->saveArchive();
        
        xSM->back();
    }
}
void CSelectOutfitLayer::CBTeachEnd()
{
    DATAPOOL->m_bTeachStepOver[TEACH_STEP_SELECT_OUTFIT] = true;
    DATAPOOL->setPlanOutfit(m_nGoalIndex, Prop_Type_HongZhaJi);
    DATAPOOL->saveArchive();
    
    this->removeChildByTag(PLAN_OUTFIT_LAYER_TAG_TEACH, true);
    
    xSM->back();
}

void CSelectOutfitLayer::setGoalIndex(int index)
{
    m_nGoalIndex = index;
}

#pragma mark - CSelectOutfitScene

CSelectOutfitScene::CSelectOutfitScene()
{
    m_pMainLayer = NULL;
}
CSelectOutfitScene::~CSelectOutfitScene()
{
    
}
bool CSelectOutfitScene::init()
{
    bool bRst = false;
    
    do
    {
        CC_BREAK_IF(!CCScene::init());
        
        m_pMainLayer = CSelectOutfitLayer::create();
		CC_BREAK_IF(!m_pMainLayer);
		addChild(m_pMainLayer);
        
        bRst = true;
        
    } while (0);

    return bRst;
}

void CSelectOutfitScene::setGoalIndex(int index)
{
    m_pMainLayer->setGoalIndex(index);
}
