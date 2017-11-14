#include "HomeCloud.h"

bool CHomeCloud::init()
{
    bool bRst = false;
    
    do
    {
        CC_BREAK_IF(!CCNode::init());
        
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        this->setContentSize(CCSize(200, 400));//todo 这个参数如何自动计算?
        
        int type = 1 + rand() % HOME_CLOUD_TYPE_NUM;
        switch (type)
        {
            case 1:
            {
                CCSprite* pSpriteCloud = CCSprite::createWithSpriteFrameName(home_cloud_1);
                pSpriteCloud->setPosition(ccp(getContentSize().width/2, 50));
                addChild(pSpriteCloud, 0, 1000);
                
                m_fSpeed = 0.005;
            }
                break;
                
            case 2:
            {
                CCSprite* pSpriteCloud = CCSprite::createWithSpriteFrameName(home_cloud_2);
                pSpriteCloud->setPosition(ccp(getContentSize().width/2, 150));
                addChild(pSpriteCloud, 0, 1000);
                
                m_fSpeed = 0.008;
            }
                break;
                
            case 3:
            {
                CCSprite* pSpriteCloud = CCSprite::createWithSpriteFrameName(home_cloud_3);
                pSpriteCloud->setPosition(ccp(getContentSize().width/2, 150));
                addChild(pSpriteCloud, 0, 1000);
                
                m_fSpeed = 0.009;
            }
                break;
                
            case 4:
            {
                CCSprite* pSpriteCloud = CCSprite::createWithSpriteFrameName(home_cloud_4);
                pSpriteCloud->setPosition(ccp(getContentSize().width/2, 150));
                addChild(pSpriteCloud, 0, 1000);
                
                m_fSpeed = 0.01;
            }
                break;
            
            default:
            {
                CCSprite* pSpriteCloud = CCSprite::createWithSpriteFrameName(home_cloud_5);
                pSpriteCloud->setPosition(ccp(getContentSize().width/2, 150));
                addChild(pSpriteCloud, 0, 1000);
                
                m_fSpeed = 0.03;
            }
                break;
        }

        this->setAnchorPoint(ccp(0.5, -2));
        float startRotation = (float)(rand()%90)*((rand()%2==0)?(1):(-1));
        this->setRotation(startRotation);
  ///      CCLOG("cloud rotation=%f, speed=%f",startRotation, m_fSpeed);
        
        
		this->schedule(schedule_selector(CHomeCloud::cloudMove),HOME_CLOUD_MOVE_FREQ);
		this->scheduleUpdate();
        
        bRst = true;
        
    } while (0);
    
    return bRst;
    
}

void CHomeCloud::onEnter()
{
    CCNode::onEnter();
}

void CHomeCloud::onExit()
{
    this->removeAllChildrenWithCleanup(true);
    CCNode::onExit();
}

void CHomeCloud::cloudMove(float dt)
{
    float fCurRotation = this->getRotation();
    float fGoalRotation = fCurRotation + m_fSpeed;
    if(fCurRotation > 45)
    {
        fGoalRotation = -(fCurRotation - 90);
    }
    this->setRotation(fGoalRotation);
}

bool CHomeCloudNode::init()
{
    bool bRst = false;
    do
    {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        this->setContentSize(winSize);
        
        int nCloudNum = 10;
        for (int i = 0; i < nCloudNum; i++)
        {
            CHomeCloud *pCloud = CHomeCloud::create();
            pCloud->setPosition(ccp(winSize.width/2, 0));
            addChild(pCloud);
        }
        
        this->scheduleUpdate();
        bRst = true;
    } while (0);
    
    return bRst;

}

