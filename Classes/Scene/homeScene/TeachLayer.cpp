//
//  TeachLayer.cpp
//  SingleEye
//
//  Created by wangwx on 14-1-19.
//
//

#include "TeachLayer.h"

CTeachLayer::CTeachLayer()
{
    
}
CTeachLayer::~CTeachLayer()
{
//    CC_SAFE_RELEASE(m_infoStr);
}
bool CTeachLayer::init()
{
    bool bRst = false;
    
    do
    {
        CC_BREAK_IF(!CCLayer::init());
        
        CCSize winSize =CCDirector::sharedDirector()->getWinSize();
        setTouchEnabled(true);
        //data
        m_focusPos = CCPoint(winSize.width/2, winSize.height/2);
        m_focusRadius = winSize.width/4;
        m_bAnimationOver = false;
        m_bOpenActionFun = true;
        
        m_callbackListener = NULL;
        m_callback = NULL;
        
        bRst = true;
        
    } while (0);
    
    return bRst;
}

void CTeachLayer::registerWithTouchDispatcher()
{
    //-128为了屏蔽下方的触摸
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
}

void CTeachLayer::onEnter()
{
    CCLayer::onEnter();

    if (m_bOpenActionFun)
    {
        if(false == m_bAnimationOver)
        {
            initClipMask();
        }
        else
        {
            initAnimation();
        }
    }
    else
    {
        initNoAnimation();
    }
}

void CTeachLayer::onExit()
{
    removeAllChildrenWithCleanup(true);
    
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    
    CCLayer::onExit();
}

void CTeachLayer::touchDelegateRetain()
{
    this->retain();
}

void CTeachLayer::touchDelegateRelease()
{
    this->release();
}

bool CTeachLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    if (m_bOpenActionFun
        && false == m_bAnimationOver)
    {
        return true;
    }
    
    if(containsTouchLocation(touch))
    {
        CBTouchGoalArea();
        return true;
    }
    
    
    return true;
}

void CTeachLayer::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    
}
void CTeachLayer::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    
}
void CTeachLayer::CBTouchGoalArea()
{
    xData->playKeytone();
    if (m_callback && m_callbackListener)
    {
        (m_callbackListener->*m_callback)();
    }
    this->removeFromParent();
}
bool CTeachLayer::containsTouchLocation(CCTouch* touch)
{
    CCRect contentRect = CCRectMake(m_focusPos.x - m_focusRadius, m_focusPos.y - m_focusRadius, m_focusRadius*2, m_focusRadius*2);
    //todo 为了精准，判断是否在圆内
//    return contentRect.containsPoint(convertTouchToNodeSpaceAR(touch));
     return contentRect.containsPoint(convertTouchToNodeSpace(touch));
}

void CTeachLayer::scaleActionEnd()
{
    CCLOG("scale action end");
 
    initAnimation();
}

void CTeachLayer::moveActionEnd()
{
//    CCLOG("move action end");
    
    m_bAnimationOver = true;
    
    //点击动画
    do{
        //点击动画
        CCArray *animFrames = CCArray::createWithCapacity(2);
        CC_BREAK_IF(!animFrames);
        CCSpriteFrame* frame1 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName( teach_shouzhi1 );
        animFrames->addObject(frame1);
        CCSpriteFrame* frame2 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName( teach_shouzhi2 );
        animFrames->addObject(frame2);
        CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames, 0.3f);
        CC_BREAK_IF(!animation);
        CCSprite *pSprite = dynamic_cast<CCSprite*>(this->getChildByTag(TEACH_LAYER_TAG_ANIMATION));
        CC_BREAK_IF(!pSprite);
        pSprite->runAction(CCRepeatForever::create(CCAnimate::create(animation)));
    } while (0);
    
}

void CTeachLayer::setFocusProp(CCPoint pos, float r)
{
    m_focusPos = ccp(pos.x, pos.y);
    m_focusRadius = r;
}
//void CTeachLayer::setTextInfo(CCRect rc, const char* pFileName)
//{
//    m_infoRect = CCRect(rc);
//    
//    CC_SAFE_RELEASE(m_infoStr);
//    if(pFileName)
//    {
//        m_infoStr = CCString::createWithFormat("%s", pFileName);
//        m_infoStr->retain();
//    }
//}
void CTeachLayer::setPicInfo(CCPoint centerPos, const char* pPicPath, TextureResType texType)
{
    CCSprite *pInfo = NULL;
    if(UI_TEX_TYPE_LOCAL == texType)
    {
        pInfo = CCSprite::create(pPicPath);
    }
    else if(UI_TEX_TYPE_PLIST == texType)
    {
        pInfo = CCSprite::createWithSpriteFrameName(pPicPath);
    }
    if (pInfo)
    {
        addChild(pInfo, TEACH_LAYER_ZORDER_INFO_TEXT, TEACH_LAYER_TAG_INFO_PIC);
        pInfo->setPosition(centerPos);
        pInfo->setOpacity(0);
        
        CCSize picSize = pInfo->getContentSize();
        float fIndent = 10;
        m_infoRect = CCRect(centerPos.x - picSize.width/2 - fIndent, centerPos.y - picSize.height/2 - fIndent, picSize.width + fIndent*2, picSize.height + fIndent*2);
    }
}
void CTeachLayer::setCallbackFun(CCObject* target, SEL_CallFunc callfun)
{
    m_callbackListener = target;
    m_callback = callfun;
}
void CTeachLayer::setAnimation(bool bOpen)
{
    m_bOpenActionFun = bOpen;
}
//遮罩动画
void CTeachLayer::initClipMask()
{
    CCSize winSize =CCDirector::sharedDirector()->getWinSize();
    do
    {
        //剪裁节点
        CCClippingNode* pClip=CCClippingNode::create();
        CC_BREAK_IF(!pClip);
        pClip->setInverted(true);//设置是否反向，将决定画出来的圆是透明的还是黑色的
        //将遮罩层添加到剪裁节点上
        CCLayerColor *pBackMask = CCLayerColor::create(ccc4(100, 100, 150, 220));
        CC_BREAK_IF(!pBackMask);
        pClip->addChild(pBackMask);
        /*绘制圆形区域*/
        //设置参数
        static ccColor4F red={1,0,0,1};//顶点颜色设置为红色，参数是R,G,B,透明度
        float startRadius = winSize.height/2;//设置圆的半径
        const int nCount = 200;//设置顶点数，此处我们将圆看成200边型
        const float angel = 2.0f * (float)M_PI/nCount;//两个顶点与中心的夹角（弧度）
        static CCPoint circle[nCount];  //保存顶点的数组
        for(int i = 0; i < nCount; i++)
        {
            float radian = i*angel; //弧度
            circle[i].x = startRadius * cosf(radian);//顶点x坐标
            circle[i].y = startRadius * sinf(radian);//顶点y坐标
        }
        //注意不要将pStencil addChild
        CCDrawNode *pStencil = CCDrawNode::create();
        pStencil->drawPolygon(circle, nCount, red, 0, red);
        //将这个圆形从剪裁节点上面抠出来，Stencil是模板的意思
        pClip->setStencil(pStencil);
        addChild(pClip, TEACH_LAYER_ZORDER_MASK);
        //聚焦动画
        float fScale = m_focusRadius/startRadius;
        CCActionInterval *act = CCScaleTo::create(0.9f, fScale);
        CCCallFunc*  callFun = CCCallFunc::create(this,callfunc_selector(CTeachLayer::scaleActionEnd));
        pStencil->runAction(CCSequence::create(act, callFun, NULL));
        pStencil->setPosition(m_focusPos);
        
    } while (0);
}
//动画
void CTeachLayer::initAnimation()
{
    do
    {
        CCSize winSize =CCDirector::sharedDirector()->getWinSize();
        this->setContentSize(winSize);
        
        //手指
        CCSprite *pSprite = CCSprite::createWithSpriteFrameName(teach_shouzhi1);
        CC_BREAK_IF(!pSprite);
        addChild(pSprite, TEACH_LAYER_ZORDER_HAND, TEACH_LAYER_TAG_ANIMATION);
        pSprite->setPosition(CCPoint(0, 0));
        
        //移入动画
        CCPoint startPos = CCPoint(0, 0);
        CCPoint endPos = CCPoint(m_focusPos.x, m_focusPos.y - m_focusRadius);
        CCActionInterval *moveIn1 = CCMoveTo::create(0.1f, startPos);
        CCActionInterval *moveIn2 = CCMoveTo::create(0.8f, endPos);
        CCCallFunc*  callFun = CCCallFunc::create(this,callfunc_selector(CTeachLayer::moveActionEnd));
        pSprite->runAction(CCSequence::create(moveIn1, moveIn2, callFun, NULL));

//不要底板了
//        //board
//        CCSprite* tmp = CCSprite::create(board9_teach);
//        CC_BREAK_IF(!tmp);
//        CCSize tmpSize = tmp->getContentSize();
//        CCRect picRc = CCRectMake(0,0, tmpSize.width, tmpSize.height);
//        float indent = 5;
//        CCRect insetRc = CCRectMake(tmpSize.width/3, tmpSize.height/2, tmpSize.width/3, tmpSize.height/3);
//        CCScale9Sprite *pBoard = CCScale9Sprite::create(board9_teach, picRc, insetRc);
//        CC_BREAK_IF(!pBoard);
//        CCSize boardSize = CCSize(m_infoRect.getMaxX() - m_infoRect.getMinX(), m_infoRect.getMaxY() - m_infoRect.getMinY());
//        pBoard->setContentSize(boardSize);
//        pBoard->setPosition(CCPoint(m_infoRect.getMidX(), m_infoRect.getMidY()));
//        addChild(pBoard, TEACH_LAYER_ZORDER_INFO_BOARD);
        
        //文字
        CCSprite *pInfo = dynamic_cast<CCSprite*>(this->getChildByTag(TEACH_LAYER_TAG_INFO_PIC));
        CC_BREAK_IF(!pInfo);
        pInfo->setPosition(ccp(m_infoRect.getMidX(), m_infoRect.getMidY()));
        pInfo->setOpacity(255);
        
    
    } while (0);

}
void CTeachLayer::initNoAnimation()
{
    do
    {
        CCSize winSize =CCDirector::sharedDirector()->getWinSize();
        this->setContentSize(winSize);
        
        //文字
        CCSprite *pInfo = dynamic_cast<CCSprite*>(this->getChildByTag(TEACH_LAYER_TAG_INFO_PIC));
        CC_BREAK_IF(!pInfo);
        pInfo->setPosition(ccp(m_infoRect.getMidX(), m_infoRect.getMidY()));
        pInfo->setOpacity(255);
        
    } while (0);
    
}
