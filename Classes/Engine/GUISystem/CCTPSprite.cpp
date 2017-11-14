//
//  CCTPSprite.cpp
//  coslua
//
//  Created by yuanlu on 13-6-27.
//
//

#include "CCTPSprite.h"

NS_ENGINE_BEGIN

CCTPSprite::CCTPSprite(CCNode* pNode, const char* plist, const char* pfile, bool bclearpath, const int zOrder, const int tag):
m_pSpriteNode(NULL)
{
    if(!pNode || !plist)
        return;
    
    m_sListPath = plist;
    m_bClearPath = bclearpath;
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(m_sListPath.c_str());
    if(zOrder >= 0 && pfile)
    {
        m_pSpriteNode = CCSpriteBatchNode::create(pfile);
        if(m_pSpriteNode);
        pNode->addChild(m_pSpriteNode,zOrder,tag);
    }
}

CCTPSprite::~CCTPSprite()
{
    if(m_bClearPath)
        CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(m_sListPath.c_str());
}

CCSprite *CCTPSprite::CreateSpriteByFrameName(const char *pImage, const int mid, const int zOrder, const int tag)
{
    if((zOrder>=0 && !m_pSpriteNode) || !pImage)
        return NULL;
    
    std::string mstrfull(pImage);
    if(mid >= 0)
    {
        char str[TPSPRINT_NODE_STRLEN]={0};
        sprintf(str, "%d", mid);
        int mpos = mstrfull.find_last_of('.');
        mstrfull.insert(mpos, str);
    }
    CCSprite *pSprite = CCSprite::createWithSpriteFrameName(mstrfull.c_str());
    if(zOrder>=0)
    {
        SpriteNodeAddChild(pSprite,zOrder,tag);
    }
    return pSprite;
}

CCSpriteFrame *CCTPSprite::GetSpriteFrameByFrameName(const char *pImage, const int index)
{
    if(!pImage)
        return NULL;
    
    std::string mstrfull(pImage);
    if(index >= 0)
    {
        char str[TPSPRINT_NODE_STRLEN]={0};
        sprintf(str, "%d", index);
        int mpos = mstrfull.find_last_of('.');
        mstrfull.insert(mpos, str);
    }
   return CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(mstrfull.c_str());
}

CCAnimate *CCTPSprite::CreateAnimateByFrameName(const char *pImage, const int start, const int end, const float mft, const bool reverse)
{
    int mNum = end-start+1;
    if(!pImage || mNum<1)
        return NULL;
    
    CCAnimation* pAnimation = NULL;
    CCAnimate *pAnimate = NULL;
    CCArray* pAnimFrames = CCArray::createWithCapacity(mNum);
    if(pAnimFrames)
    {
        int i,mpos=0;
        char str[TPSPRINT_NODE_STRLEN]={0};
        if (reverse == true)
        {
            for(i=end; i>=start; i--)
            {
                sprintf(str, "%d", i);
                std::string mPicStr(pImage);
                mpos = mPicStr.find('.');
                mPicStr.insert(mpos, str);
                CCSpriteFrame* pframe = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(mPicStr.c_str());
                if(pframe)
                    pAnimFrames->addObject(pframe);
            }
        }
        else
        {
            for(i=start; i<=end; i++)
            {
                sprintf(str, "%d", i);
                std::string mPicStr(pImage);
                mpos = mPicStr.find('.');
                mPicStr.insert(mpos, str);
                CCSpriteFrame* pframe = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(mPicStr.c_str());
                if(pframe)
                    pAnimFrames->addObject(pframe);
            }
        }
        
        pAnimation = CCAnimation::createWithSpriteFrames(pAnimFrames, mft);
    }
    if(pAnimation)
    {
        pAnimate = CCAnimate::create(pAnimation);
    }
    return pAnimate;
}

CCSprite *CCTPSprite::GetNodeSpriteByTag(const int tag)
{
    return m_pSpriteNode ? (CCSprite *)(m_pSpriteNode->getChildByTag(tag)):NULL;
}

bool CCTPSprite::SpriteNodeAddChild(CCNode* pNode, const int zOrder, const int tag)
{
    if(m_pSpriteNode && pNode)
    {
        m_pSpriteNode->addChild(pNode,zOrder,tag);
        return true;
    }
    return false;
}
NS_ENGINE_END