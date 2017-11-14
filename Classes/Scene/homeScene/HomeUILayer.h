//
//  HomeUILayer.h
//  SingleEye
//
//  Created by wangwx on 13-12-17.
//
//

#ifndef __SingleEye__HomeUILayer__
#define __SingleEye__HomeUILayer__

#include "GameInclude.h"

USING_NS_CC;
USING_NS_CC_EXT;

#pragma mark - CCHomeUILayer

enum
{
    HOME_UI_LAYER_ZORDER_GOLD = 1,
    HOME_UI_LAYER_ZORDER_GEM,
    HOME_UI_LAYER_ZORDER_MASK,
    HOME_UI_LAYER_ZORDER_SLIDEMENU,
    HOME_UI_LAYER_ZORDER_LIMIT
};

enum
{
    HOMEUI_LAYER_TAG_GOLD = 1000,
    HOMEUI_LAYER_TAG_GEM,
    HOMEUI_LAYER_TAG_SLIDEMENU,
    HOMEUI_LAYER_TAG_LIMIT
};

class CHomeUILayer : public CCLayer
{
public:
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
	CREATE_FUNC(CHomeUILayer);
};

#endif /* defined(__SingleEye__HomeUILayer__) */
