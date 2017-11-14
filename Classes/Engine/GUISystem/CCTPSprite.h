//
//  CCTPSprite.h
//  coslua
//
//  Created by yuanlu on 13-6-27.
//
//

#ifndef __coslua__CCTPSprite__
#define __coslua__CCTPSprite__

#include "EngineDefine.h"
#include "cocos2d.h"

USING_NS_CC;
NS_ENGINE_BEGIN

#define TPSPRINT_NODE_STRLEN    (100)

class CCTPSprite
{
public:
    //zOrder>=0时，创建SpriteNode;
    CCTPSprite(CCNode* pNode, const char* plist, const char* pfile=NULL, bool bclearpath=true, const int zOrder=-1, const int tag=kCCNodeTagInvalid);
    ~CCTPSprite();
    
    //创建Sprite,zOrder>=0时，添加到SpriteNode;
    CCSprite *CreateSpriteByFrameName(const char *pImage, const int mid=-1, const int zOrder=-1, const int tag=kCCNodeTagInvalid);
    //得到SpriteNode,child;
    CCSprite *GetNodeSpriteByTag(const int tag);
    //得到单个Frame
    CCSpriteFrame *GetSpriteFrameByFrameName(const char *pImage, const int index=-1);
    //创建精灵动作
    CCAnimate *CreateAnimateByFrameName(const char *pImage, const int start, const int end, const float mft, const bool reverse = false);
    //向SpriteNode添加child;
    bool SpriteNodeAddChild(CCNode* pNode, const int zOrder=-1, const int tag=kCCNodeTagInvalid);
    
private:
    CCSpriteBatchNode *m_pSpriteNode;
    std::string m_sListPath;
    bool m_bClearPath;
};

NS_ENGINE_END

#endif /* defined(__coslua__CCTPSprite__) */
