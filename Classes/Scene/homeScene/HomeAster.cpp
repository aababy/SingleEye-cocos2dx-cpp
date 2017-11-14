#include "HomeAster.h"

bool CHomeAster::init()
{
    bool bRst = false;
    if(!CCNode::init())
    {
        return false;
    }
    return true;
//    do
//    {
//        CC_BREAK_IF(!CCNode::init());
//        CCDirector* pDirector = CCDirector::sharedDirector();
//        CCSize visibleSize = pDirector->getVisibleSize();
//        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
//        
//        this->setContentSize(CCSizeMake(visibleSize.width, visibleSize.height));
//        
//        //main body
//        m_pMainBody = SceneReader::sharedSceneReader()->createNodeWithSceneFile(home_scene_json);
//        if (NULL != m_pMainBody)
//        {
//            m_pMainBody->ignoreAnchorPointForPosition(false);
//            m_pMainBody->setAnchorPoint(ccp(0.5, 0.5));
//            CCSize contentSize = this->getContentSize();
//            m_pMainBody->setPosition(0, contentSize.height/2);
//            addChild(m_pMainBody);
//        }
//        
//        //item
//        {
//            float fScale = 0.85;
//            //酒馆
//            CCNode *pItem_1 = m_pMainBody->getChildByTag(10201);
//            pItem_1->setVisible(false);
//            CButtonAnimation* pButton_1 = CButtonAnimation::createButtonAnimation("HomeScene/jiuguan.ExportJson", "jiuguan", false);
//            pButton_1->setRotation(pItem_1->getRotation());
//            pButton_1->setPosition(ccp(pItem_1->getPosition().x, pItem_1->getPosition().y + visibleSize.height/2));
//            pButton_1->setCallbackFun(this, callfunc_selector(CHomeAster::menuStartCallback1));
//            pButton_1->setScale(fScale);
//            addChild(pButton_1, 3);
//            CCLOG("retain count:%d",pButton_1->retainCount());
//            
//            //商店
//            CCNode *pItem_2 = m_pMainBody->getChildByTag(10202);
//            pItem_2->setVisible(false);
//            CButtonAnimation* pButton_2 = CButtonAnimation::createButtonAnimation("HomeScene/shangdian.ExportJson", "shangdian", false);
//            pButton_2->setRotation(pItem_2->getRotation());
//            pButton_2->setPosition(ccp(pItem_2->getPosition().x, pItem_2->getPosition().y + visibleSize.height/2));
//            pButton_2->setCallbackFun(this, callfunc_selector(CHomeAster::menuStartCallback2));
//            pButton_2->setScale(fScale);
//            addChild(pButton_2, 3);
//            
//            //挑战
//            CCNode *pItem_3 = m_pMainBody->getChildByTag(10203);
//            pItem_3->setVisible(false);
//            CButtonAnimation* pButton_3 = CButtonAnimation::createButtonAnimation("HomeScene/xuanze.ExportJson", "xuanze", false);
//            pButton_3->setRotation(pItem_3->getRotation());
//            pButton_3->setPosition(ccp(pItem_3->getPosition().x, pItem_3->getPosition().y + visibleSize.height/2));
//            pButton_3->setCallbackFun(this, callfunc_selector(CHomeAster::menuStartCallback3));
//            pButton_3->setScale(fScale);
//            addChild(pButton_3, 3);
//            
//            //战场
//            CCNode *pItem_4 = m_pMainBody->getChildByTag(10204);
//            pItem_4->setVisible(false);
//            CButtonAnimation* pButton_4 = CButtonAnimation::createButtonAnimation("HomeScene/hundunzhanchang.ExportJson", "hundunzhanchang", false);
//            pButton_4->setRotation(pItem_4->getRotation());
//            pButton_4->setPosition(ccp(pItem_4->getPosition().x, pItem_4->getPosition().y + visibleSize.height/2));
//            pButton_4->setCallbackFun(this, callfunc_selector(CHomeAster::menuStartCallback4));
//            pButton_4->setScale(fScale);
//            addChild(pButton_4, 3);
//            
//            //冒险
//            CCNode *pItem_5 = m_pMainBody->getChildByTag(10205);
//            pItem_5->setVisible(false);
//            CButtonAnimation* pButton_5 = CButtonAnimation::createButtonAnimation("HomeScene/fanchuan.ExportJson", "fanchuan", false);
//            pButton_5->setRotation(pItem_5->getRotation());
//            pButton_5->setPosition(ccp(pItem_5->getPosition().x, pItem_5->getPosition().y + visibleSize.height/2));
//            pButton_5->setCallbackFun(this, callfunc_selector(CHomeAster::menuStartCallback5));
//            pButton_5->setScale(fScale);
//            addChild(pButton_5, 3);
//        }
//        
//        bRst = true;
//        
//    } while (0);
    
//    return bRst;
    
}

void CHomeAster::onEnter()
{
    do
    {
        CCNode::onEnter();
        CCDirector* pDirector = CCDirector::sharedDirector();
        CCSize visibleSize = pDirector->getVisibleSize();
        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
        
        this->setContentSize(CCSizeMake(visibleSize.width, visibleSize.height));
        
        //main body
        m_pMainBody = SceneReader::sharedSceneReader()->createNodeWithSceneFile(home_scene_json);
        if (NULL != m_pMainBody)
        {
            m_pMainBody->ignoreAnchorPointForPosition(false);
            m_pMainBody->setAnchorPoint(ccp(0.5, 0.5));
            CCSize contentSize = this->getContentSize();
            m_pMainBody->setPosition(0, contentSize.height/2);
            addChild(m_pMainBody);
        }
        
        //item
        {
            float fScale = 0.85;
            //酒馆
            CCNode *pItem_1 = m_pMainBody->getChildByTag(10201);
            pItem_1->setVisible(false);
            CButtonAnimation* pButton_1 = CButtonAnimation::createButtonAnimation("HomeScene/jiuguan.ExportJson", "jiuguan", false);
            pButton_1->setRotation(pItem_1->getRotation());
            pButton_1->setPosition(ccp(pItem_1->getPosition().x, pItem_1->getPosition().y + visibleSize.height/2));
            pButton_1->setCallbackFun(this, callfunc_selector(CHomeAster::menuStartCallback1));
            pButton_1->setScale(fScale);
            addChild(pButton_1, 3, HOME_ASTER_TAG_ITEM_1);
            CCLOG("retain count:%d",pButton_1->retainCount());
            
            //商店
            CCNode *pItem_2 = m_pMainBody->getChildByTag(10202);
            pItem_2->setVisible(false);
            CButtonAnimation* pButton_2 = CButtonAnimation::createButtonAnimation("HomeScene/shangdian.ExportJson", "shangdian", false);
            pButton_2->setRotation(pItem_2->getRotation());
            pButton_2->setPosition(ccp(pItem_2->getPosition().x, pItem_2->getPosition().y + visibleSize.height/2));
            pButton_2->setCallbackFun(this, callfunc_selector(CHomeAster::menuStartCallback2));
            pButton_2->setScale(fScale);
            addChild(pButton_2, 3, HOME_ASTER_TAG_ITEM_2);
            
            //挑战
            CCNode *pItem_3 = m_pMainBody->getChildByTag(10203);
            pItem_3->setVisible(false);
            CButtonAnimation* pButton_3 = CButtonAnimation::createButtonAnimation("HomeScene/xuanze.ExportJson", "xuanze", false);
            pButton_3->setRotation(pItem_3->getRotation());
            pButton_3->setPosition(ccp(pItem_3->getPosition().x, pItem_3->getPosition().y + visibleSize.height/2));
            pButton_3->setCallbackFun(this, callfunc_selector(CHomeAster::menuStartCallback3));
            pButton_3->setScale(fScale);
            addChild(pButton_3, 3, HOME_ASTER_TAG_ITEM_3);
            
            //战场
            CCNode *pItem_4 = m_pMainBody->getChildByTag(10204);
            pItem_4->setVisible(false);
            CButtonAnimation* pButton_4 = CButtonAnimation::createButtonAnimation("HomeScene/hundunzhanchang.ExportJson", "hundunzhanchang", false);
            pButton_4->setRotation(pItem_4->getRotation());
            pButton_4->setPosition(ccp(pItem_4->getPosition().x, pItem_4->getPosition().y + visibleSize.height/2));
            pButton_4->setCallbackFun(this, callfunc_selector(CHomeAster::menuStartCallback4));
            pButton_4->setScale(fScale);
            addChild(pButton_4, 3, HOME_ASTER_TAG_ITEM_4);
            
            //冒险
            CCNode *pItem_5 = m_pMainBody->getChildByTag(10205);
            pItem_5->setVisible(false);
            CButtonAnimation* pButton_5 = CButtonAnimation::createButtonAnimation("HomeScene/fanchuan.ExportJson", "fanchuan", false);
            pButton_5->setRotation(pItem_5->getRotation());
            pButton_5->setPosition(ccp(pItem_5->getPosition().x, pItem_5->getPosition().y + visibleSize.height/2));
            pButton_5->setCallbackFun(this, callfunc_selector(CHomeAster::menuStartCallback5));
            pButton_5->setScale(fScale);
            addChild(pButton_5, 3, HOME_ASTER_TAG_ITEM_5);
            
        }
    } while (0);
    
}

void CHomeAster::onExit()
{
    SceneReader::sharedSceneReader()->purgeSceneReader();
    this->removeAllChildrenWithCleanup(true);
    CCNode::onExit();
}

void CHomeAster::menuStartCallback1(CCObject* pSender)
{
    clickHotItem(1);
}
void CHomeAster::menuStartCallback2(CCObject* pSender)
{
    clickHotItem(2);
}
void CHomeAster::menuStartCallback3(CCObject* pSender)
{
    clickHotItem(3);
}
void CHomeAster::menuStartCallback4(CCObject* pSender)
{
    clickHotItem(4);
}
void CHomeAster::menuStartCallback5(CCObject* pSender)
{
    clickHotItem(5);
}

void CHomeAster::clickHotItem(int flag)
{
    xData->playKeytone();
    
    switch (flag)
    {
        case 1://酒馆
        {
            xSM->forward(ST_Tavern);
        }
            break;
        case 2://商店
        {
            xSM->forward(ST_Shop);
        }
            break;
        case 3://挑战
        {
            xSM->forward(ST_Defense);
        }
            break;
        case 4://战场
        {
            string sUserName = CCUserDefault::sharedUserDefault()->getStringForKey(UUserName);
            if (sUserName.empty()) {
                xSM->forward(ST_Login);
            }
            else
            {
                xSM->forward(ST_PVP);
            }            
        }
            break;
        case 5://冒险
        {
            xSM->forward(ST_Dungeon);
        }
            break;
        default:
            break;
    }
}

CCPoint CHomeAster::getItemPosition(int index)
{
    int tag = 0;
    CCPoint point = CCPoint(0, 0);
    switch (index)
    {
        case 1:
            tag = HOME_ASTER_TAG_ITEM_1;
            break;
        case 2:
            tag = HOME_ASTER_TAG_ITEM_2;
            break;
        case 3:
            tag = HOME_ASTER_TAG_ITEM_3;
            break;
        case 4:
            tag = HOME_ASTER_TAG_ITEM_4;
            break;
        case 5:
            tag = HOME_ASTER_TAG_ITEM_5;
            break;
        default:
            return point;
    }
    CButtonAnimation* pButton = dynamic_cast<CButtonAnimation*>(this->getChildByTag(tag));
    if (pButton)
    {
        point = pButton->getPosition();
    }
    
    return point;
}
