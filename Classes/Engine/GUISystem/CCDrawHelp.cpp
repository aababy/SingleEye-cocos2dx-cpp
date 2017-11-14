//
//  CCDrawHelp.cpp
//  coslua
//
//  Created by yuanlu on 13-5-29.
//
//

#include "CCDrawHelp.h"
#include "../MsgSystem/Message.h"
#include "../MsgSystem/MSgSystem.h"
#include "CfgDataManage.h"

NS_ENGINE_BEGIN

#pragma mark -倒计时
/************************************************************************************************/
//倒计时
CCDrawTime* CCDrawTime::create(CCTPSprite *pTpSprt,const char *pNumName, const char *pFlagName)
{
    CCDrawTime * pRet = new CCDrawTime(pTpSprt,pNumName,pFlagName);
    if(pRet)
        pRet->autorelease();
    return pRet;
}

CCDrawTime::CCDrawTime(CCTPSprite *pTpSprt,const char *pNumName, const char *pFlagName):m_bRun(false),m_bGetSize(false),
m_iTimeMin(0),m_iTimeSec(0),m_iTimeHour(0),m_iFontSize(0),m_pListener(NULL),m_pfnSelector(NULL)
{
    m_pTpSprt = pTpSprt;
    if(pNumName)
        m_sNumName = pNumName;
    if(pFlagName)
        m_sFlagName = pFlagName;
    
    onEnter();
    onEnterTransitionDidFinish();
    schedule(schedule_selector(CCDrawTime::ChangeTime), 1.0f);
}

CCDrawTime::~CCDrawTime()
{
    unscheduleUpdate();
    onExitTransitionDidStart();
    onExit();
}

void CCDrawTime::SetStartTime(int mtime_min, int mtime_sec, int mtime_hour)
{
    m_bGetSize = false;
    m_iTimeMin = mtime_min;
    m_iTimeSec = mtime_sec;
    m_iTimeHour = mtime_hour;
    CreateTimeViewCtrl();
    UpdateTimeView();
}

void CCDrawTime::SetTime(int mtime_min, int mtime_sec, int mtime_hour)
{
    m_iTimeMin = mtime_min;
    m_iTimeSec = mtime_sec;
    m_iTimeHour = mtime_hour;
    UpdateTimeView();
}

void CCDrawTime::ChangeTime(float dt)
{
    if(!m_bRun)
        return;
    
    if(m_iTimeSec > 0)
    {
        m_iTimeSec--;
    }
    else if(m_iTimeMin > 0)
    {
        m_iTimeMin--;
        m_iTimeSec = 59;
    }
    else if(m_iTimeHour > 0)
    {
        m_iTimeHour--;
        m_iTimeMin = 59;
        m_iTimeSec = 59;
    }
    else if (m_pListener && m_pfnSelector)
    {
        ChangeState(false);
        (m_pListener->*m_pfnSelector)(this);
    }
    UpdateTimeView();
}

void CCDrawTime::CreateTimeViewCtrl()
{
    if(!m_pTpSprt || m_sNumName.length()<1)
    {
         CCLabelTTFBoard *pTimeLable = CCLabelTTFBoard::create("",m_sFontName.c_str(),m_iFontSize);
        if(pTimeLable)
        {
            pTimeLable->setPosition(ccp(0,0));
            pTimeLable->setColor(m_FontColor);
            addChild(pTimeLable,0,DRAWTIME_CTRL_LABLE);
        }
        return;
    }
        
    CCSpriteFrame *pFrameData = m_pTpSprt->GetSpriteFrameByFrameName(m_sNumName.c_str(),1);
    if(!pFrameData)
        return;

    CCSprite *pTimeSprt= NULL;
    float mDataW = pFrameData->getOriginalSizeInPixels().width;
    if(m_sFlagName.empty())//秒
    {
        m_iOnlySecCout = 1;
        int mNum = m_iTimeHour*60+m_iTimeMin*60+m_iTimeSec;
        while(mNum>9)
        {
            mNum /= 10;
            m_iOnlySecCout++;
        }
        float mPos = -(mDataW*m_iOnlySecCout)/2;
        mPos += mDataW/2;
        for(byte i=0; i<m_iOnlySecCout; i++)
        {
            pTimeSprt = CCSprite::create();
            if(pTimeSprt)
            {
                pTimeSprt->setPosition(ccp(mPos,0));
                addChild(pTimeSprt,0,DRAWTIME_CTRL_SEC+i);
            }
            mPos += mDataW;
        }
    }
    else if(!m_sNumName.empty())//小时：分：秒
    {
        CCSpriteFrame *pFrameFlag = m_pTpSprt->GetSpriteFrameByFrameName(m_sFlagName.c_str());
        if(!pFrameFlag)
            return;
        
        float mFlagW = pFrameFlag->getOriginalSizeInPixels().width;
        float mPos = -(mDataW*6+mFlagW)/2;
        mPos += mDataW/2;
        for(byte mtype=0; mtype<3; mtype++)
        {
            int mid = DRAWTIME_CTRL_HOUR;//小时
            if(mtype == 1)
                mid = DRAWTIME_CTRL_MIN;//分
            else if(mtype == 2)
                mid = DRAWTIME_CTRL_SEC;//秒
            
            for(byte i=0; i<2; i++)
            {
                pTimeSprt = CCSprite::create();
                if(pTimeSprt)
                {
                    pTimeSprt->setPosition(ccp(mPos,0));
                    addChild(pTimeSprt,0,mid+i);
                }
                mPos += mDataW;
            }
            
            if(mtype < 2)
            {
                pTimeSprt = CCSprite::createWithSpriteFrameName(m_sFlagName.c_str());
                mPos += (mFlagW/2-mDataW/2);
                if(pTimeSprt)
                {
                    pTimeSprt->setPosition(ccp(mPos,0));
                    addChild(pTimeSprt,0,mtype==0 ? DRAWTIME_CTRL_FLAG_HM:DRAWTIME_CTRL_FLAG_MS);
                }
                mPos += (mFlagW/2+mDataW/2);
            }
        }
    }
}

void CCDrawTime::UpdateTimeView()
{
    if(!m_pTpSprt || m_sNumName.empty())
    {
        CCLabelTTFBoard *pTimeLable = (CCLabelTTFBoard *)getChildByTag(DRAWTIME_CTRL_LABLE);
        if(pTimeLable)
        {
            char mtpStr[256];
            sprintf(mtpStr,"%02d:%02d:%02d",m_iTimeHour,m_iTimeMin,m_iTimeSec);
            pTimeLable->setString(mtpStr);
            if(!m_bGetSize)
            {
                this->setContentSize(pTimeLable->getContentSize());
                m_bGetSize = true;
            }
        }
        return;
    }
    
    int mdata = 0;
    CCSprite *pSptTime = NULL;
    CCSpriteFrame *pFrame = NULL;
    CCSize msize(0,0);
    if(m_sFlagName.empty())//秒
    {
        mdata = m_iTimeHour*60+m_iTimeMin*60+m_iTimeSec;
        for(byte i=0; i<m_iOnlySecCout; i++)
        {
            pSptTime = (CCSprite *)getChildByTag(DRAWTIME_CTRL_SEC+(m_iOnlySecCout-1)-i);
            if(pSptTime)
            {
                pFrame = m_pTpSprt->GetSpriteFrameByFrameName(m_sNumName.c_str(),mdata%10);
                pSptTime->initWithSpriteFrame(pFrame);
                msize.width = pFrame->getOriginalSizeInPixels().width;
                msize.height = pFrame->getOriginalSizeInPixels().height;
            }
            mdata /= 10;
        }
        msize.width*=m_iOnlySecCout;
        if(!m_bGetSize)
        {
            this->setContentSize(msize);
            m_bGetSize = true;
        }
        return;
    }
    
    msize = CCSizeMake(0, 0);
    for(byte mtype=0; mtype<3; mtype++)
    {
        int mid = DRAWTIME_CTRL_HOUR;//小时
        int mCurData = m_iTimeHour;
        if(mtype == 1)
        {
            mid = DRAWTIME_CTRL_MIN;//分
            mCurData = m_iTimeMin;
        }
        else if(mtype == 2)
        {
            mid = DRAWTIME_CTRL_SEC;//秒
            mCurData = m_iTimeSec;
        }
        
        if(mCurData > 99)//大于二位数
            mCurData = 99;
        for(byte i=0; i<2; i++)
        {
            pSptTime = (CCSprite *)getChildByTag(mid+i);
            mdata = i==0? mCurData/10:mCurData%10;
            if(pSptTime)
            {
                pFrame = m_pTpSprt->GetSpriteFrameByFrameName(m_sNumName.c_str(),mdata);
                pSptTime->initWithSpriteFrame(pFrame);
                msize.width = pFrame->getOriginalSizeInPixels().width;
                msize.height = pFrame->getOriginalSizeInPixels().height;
            }
        }
    }
    msize.width *= 8;
    if(!m_bGetSize)
    {
        this->setContentSize(msize);
        m_bGetSize = true;
    }
}
#pragma mark -数字显示
/************************************************************************************************/
//数字显示
CCDrawNum* CCDrawNum::create(CCTPSprite *pTpSprt,const char *pNumName, const char *pFlagName)
{
    CCDrawNum * pRet = new CCDrawNum(pTpSprt,pNumName,pFlagName);
    if(pRet)
        pRet->autorelease();
    return pRet;
}

CCDrawNum::CCDrawNum(CCTPSprite *pTpSprt,const char *pNumName, const char *pFlagName):m_uData(0)
{
    m_pTpSprt = pTpSprt;
    m_sNumName = pNumName;
    if(pFlagName)
        m_sFlagName = pFlagName;
}

CCDrawNum::~CCDrawNum()
{
    
}

void CCDrawNum::SetShowNum(UINT32 mdata, DRAWNUM_ANCHOR mAnchor)
{
    if(!m_pTpSprt || m_sNumName.empty())
        return;
    
    m_uData = mdata;
    this->removeAllChildren();
    int mcount = mdata==0 ? 1:0;
    UINT32 mtpdata = mdata;
    while(mtpdata>0)
    {
        mtpdata/=10;
        mcount++;
    }
    
    CCSprite *pFlagSprt = NULL;
    CCSprite *pNumSprt = NULL;
    int mPos,mFw,mW = 0;
    mFw = 0;
    if(!m_sFlagName.empty())
    {
        pFlagSprt  = m_pTpSprt->CreateSpriteByFrameName(m_sFlagName.c_str());
        if(pFlagSprt)
           mFw = pFlagSprt->getContentSize().width;
    }
    pNumSprt = m_pTpSprt->CreateSpriteByFrameName(m_sNumName.c_str(),1);
    if(pNumSprt)
        mW = pNumSprt->getContentSize().width;
    
    mPos = 0;
    if(mAnchor == DRAWNUM_ANCHOR_LEFT)
    {
        mPos = mW*mcount+mFw;
    }
    else if(mAnchor == DRAWNUM_ANCHOR_CENTER)
    {
        mPos = (mW*mcount+mFw)/2;
    }
    mPos -= mW/2;
    for(int i=0; i<mcount; i++)
    {
        mtpdata = mdata%10;
        pNumSprt = m_pTpSprt->CreateSpriteByFrameName(m_sNumName.c_str(),mtpdata);
        if(pNumSprt)
        {
            pNumSprt->setPosition(ccp(mPos,0));
            addChild(pNumSprt);
            mPos -= mW;
        }
        mdata /= 10;
    }
    if(pFlagSprt)
    {
        pFlagSprt->setPosition(ccp(mPos+(mW-mFw)/2,0));
        addChild(pFlagSprt);
    }
}

#pragma mark -进度条
/************************************************************************************************/
//进度条
CCDrawBar* CCDrawBar::create(const char *pImageFill, const char *pImageBg, const char *pImageFlag, bool brtol, bool bFlagFX)
{
    CCDrawBar *pRet = new CCDrawBar(pImageFill,pImageBg,pImageFlag,brtol,bFlagFX);
    if(pRet)
        pRet->autorelease();
    return pRet;
}

CCDrawBar::CCDrawBar(const char *pImageFill, const char *pImageBg, const char *pImageFlag, bool brtol, bool bFlagFX):
m_fFlagHoff(0),m_fPercent(0),m_bBarBgAnchor(false),m_bRightToLeft(brtol)
{
    CCProgressTimer *pTimer = NULL;
    if(pImageBg)
    {
        CCSprite *pBarBg = CCSprite::createWithSpriteFrameName(pImageBg);
        if(pBarBg)
        {
            pBarBg->setPosition(CCPointZero);
            setContentSize(pBarBg->getContentSize());
            addChild(pBarBg,0,DRAWBAR_CTRL_BG);
        }
    }
    
    if(pImageFill)
    {
        CCSprite *pBarFill = CCSprite::createWithSpriteFrameName(pImageFill);
        if(pBarFill)
        {
            pTimer = CCProgressTimer::create(pBarFill);
            if(pTimer)
            {
                pTimer->setType(kCCProgressTimerTypeBar);
                pTimer->setMidpoint(ccp(m_bRightToLeft?1:0,0));
                pTimer->setBarChangeRate(ccp(1,0));
                pTimer->setPercentage(0);
                pTimer->setPosition(CCPointZero);
                addChild(pTimer,1,DRAWBAR_CTRL_FILL);
            }
        }
    }
    
    if(pImageFlag)
    {
        CCSprite *pBarFlag =  CCSprite::createWithSpriteFrameName(pImageFlag);
        if(pBarFlag && pTimer)
        {
            float mfx = pTimer->getPositionX() - pTimer->getContentSize().width/2;
            pBarFlag->setAnchorPoint(ccp(0,0.5));
            if(m_bRightToLeft)
            {
                mfx = pTimer->getPositionX()+pTimer->getContentSize().width/2;
                pBarFlag->setAnchorPoint(ccp(1,0.5));
            }
            pBarFlag->setFlipX(bFlagFX);
            pBarFlag->setPosition(ccp(mfx,pTimer->getPositionY()));
            addChild(pBarFlag,1,DRAWBAR_CTRL_FLAG);
        }
    }
    scheduleUpdate();
}

CCDrawBar::~CCDrawBar()
{
    
}

void CCDrawBar::update(float delta)
{
    CCProgressTimer *pBar = (CCProgressTimer *)getChildByTag(DRAWBAR_CTRL_FILL);
    if(!pBar || m_fPercent==pBar->getPercentage())
        return;
    SetBarPercentage(pBar->getPercentage());
}

void CCDrawBar::SetBarFlagInfo(float mhoff, bool bBgAnchor)
{
    m_fFlagHoff = mhoff;
    m_bBarBgAnchor = bBgAnchor;
    CCSprite *pFlag = (CCSprite *)getChildByTag(DRAWBAR_CTRL_FLAG);
    if(pFlag)
    {
        float mfx = 0;
        CCSprite *pBarBg = (CCSprite *)getChildByTag(DRAWBAR_CTRL_BG);
        CCProgressTimer *pBar = (CCProgressTimer *)getChildByTag(DRAWBAR_CTRL_FILL);
        CCNode *pAnchorNode = m_bBarBgAnchor ? (CCNode *)pBarBg:(CCNode *)pBar;
        if(pAnchorNode)
        {
            mfx = pAnchorNode->getPositionX() - pAnchorNode->getContentSize().width/2;
            mfx += m_fFlagHoff;
            if(m_bRightToLeft)
            {
                mfx = pAnchorNode->getPositionX() + pAnchorNode->getContentSize().width/2;
                mfx -= m_fFlagHoff;
            }
        }
        pFlag->setPositionX(mfx);
    }
}

void CCDrawBar::SetBarPercentage(float fPercentage)
{
    CCProgressTimer *pTimer = (CCProgressTimer *)getChildByTag(DRAWBAR_CTRL_FILL);
    if(pTimer)
    {
        pTimer->setPercentage(fPercentage);
        m_fPercent = fPercentage;
        CCSprite *pFlag = (CCSprite *)getChildByTag(DRAWBAR_CTRL_FLAG);
        if(pFlag)
        {
            float mfx = 0;
            CCSprite *pBarBg = (CCSprite *)getChildByTag(DRAWBAR_CTRL_BG);
            CCNode *pNode = m_bBarBgAnchor ? (CCNode *)pBarBg:(CCNode *)pTimer;
            if(pNode)
            {
                if(m_bRightToLeft)
                {
                    mfx = pNode->getPositionX() + pNode->getContentSize().width/2;
                    mfx -= ((pNode->getContentSize().width*fPercentage/100)+m_fFlagHoff);
                }
                else
                {
                    mfx = pNode->getPositionX() - pNode->getContentSize().width/2;
                    mfx += ((pNode->getContentSize().width*fPercentage/100)+m_fFlagHoff);
                }
            }
            pFlag->setPositionX(mfx);
        }
    }
}

void CCDrawBar::CreateMoveFlash(float duration, float fPercentage)
{
   CCProgressTimer *pBar = (CCProgressTimer *)getChildByTag(DRAWBAR_CTRL_FILL);
   if(pBar && int(fPercentage) != int(pBar->getPercentage()))
   {
       CCProgressFromTo *pProgAct = CCProgressFromTo::create(duration, pBar->getPercentage(), fPercentage);
       pBar->runAction(pProgAct);
   }
}

#pragma mark -对话框
/************************************************************************************************/
//对话框
CCDialog* CCDialog::create(const char *pContent,const char *pLBName,const char *pRBName)
{
    CCDialog *pRet = new CCDialog(pContent,pLBName,pRBName);
    if(pRet)
        pRet->autorelease();
    return pRet;
}

CCDialog::CCDialog(const char *pContent,const char *pLBName,const char *pRBName):m_pTarget(NULL),m_pEndHandle(NULL)
{
    CCLayerColor::init();
    if(pContent)
        m_sTxtCont = pContent;
    if(pLBName)
        m_sLButName = pLBName;
    if(pRBName)
        m_sRButName = pRBName;
}

CCDialog::~CCDialog()
{
    
}

void CCDialog::onEnter()
{
    CCLayerColor::onEnter();
    OnInitDialog();
    //屏蔽所有priority比自己大的消息
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, TOUHANDPRITY_SYS,true);
}

void CCDialog::onExit()
{
    CCLayerColor::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool CCDialog::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    if(m_pMenu)
        m_bTouchedMenu = m_pMenu->ccTouchBegan(touch, event);
    if( m_pScrollView)
        m_bTouchedView = m_pScrollView->ccTouchBegan(touch, event);
    return true;
}

void CCDialog::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    if(m_bTouchedMenu)
    {
        m_pMenu->ccTouchEnded(touch, event);
        m_bTouchedMenu = false;
    }
    if(m_bTouchedView)
    {
        m_pScrollView->ccTouchEnded(touch, event);
        m_bTouchedView = false;
    }
}

void CCDialog::ccTouchCancelled(CCTouch *touch, CCEvent* event)
{
    if(m_bTouchedMenu)
    {
        m_pMenu->ccTouchCancelled(touch, event);
        m_bTouchedMenu = false;
    }
    if(m_bTouchedView)
    {
        m_pScrollView->ccTouchCancelled(touch, event);
        m_bTouchedView = false;
    }
}

void CCDialog::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    if(m_bTouchedMenu)
        m_pMenu->ccTouchMoved(touch, event);
    if(m_bTouchedView)
        m_pScrollView->ccTouchMoved(touch, event);
}

void CCDialog::OKButCallBack(CCObject *obj)
{
    EndDialog(true);
}

void CCDialog::CancelButCallBack(CCObject *obj)
{
    EndDialog(false);
}

bool CCDialog::OnInitDialog()
{
    m_pMenu = NULL;
    m_pScrollView = NULL;
    m_bTouchedMenu = false;
    m_bTouchedView = false;

    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(DIALOG_DEFAULT_TPNAME1);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(DIALOG_DEFAULT_TPNAME2);
    CCScale9Sprite *pBoxFrame = CCScale9Sprite::createWithSpriteFrameName(DIALOG_DEFAULT_FRAMNAME);
    if(!pBoxFrame)
        return false;
    
    CCSize tSize = CCDirector::sharedDirector()->getWinSize();
    //pBoxFrame->setContentSize(CCSizeMake(tSize.width*2/5, tSize.height*2/5));
    pBoxFrame->setContentSize(CCSizeMake(tSize.width*4/5, tSize.height*3/5));
    pBoxFrame->setPosition(ccp(tSize.width/2, tSize.height/2));
    addChild(pBoxFrame);
    
    bool bOnlyOne = m_sRButName.length()>0 ? false:true;
    float mfButW,mfButH = 0;
    if(m_sLButName.length() < 1)
        m_sLButName = DIALOG_DEFAULT_LBUT;
    CCMenuItemImage *pOkBut = CCMenuItemImage::CCMenuItemImage::create(NULL,NULL,this,
                                                                       menu_selector(CCDialog::OKButCallBack));
    if(pOkBut)
    {
        pOkBut->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(DIALOG_DEFAULT_BUTBG));
        pOkBut->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(DIALOG_DEFAULT_BUTBGF));
        mfButW = pOkBut->getContentSize().width;
        mfButH = pOkBut->getContentSize().height;
        if(bOnlyOne)
            pOkBut->setPosition(ccp(pBoxFrame->getContentSize().width/2, mfButH/2+DIALOG_DEFAULT_OFF));
        else
            pOkBut->setPosition(ccp(DIALOG_DEFAULT_OFF+mfButW/2,DIALOG_DEFAULT_OFF+mfButH/2));
        CCSprite *ptxt = CCSprite::createWithSpriteFrameName(m_sLButName.c_str());
        if(ptxt)
        {
            ptxt->setPosition(ccp(mfButW/2,mfButH/2));
            pOkBut->addChild(ptxt);
        }
    }
    CCMenuItemImage *pCanelBut = bOnlyOne ? NULL:CCMenuItemImage::CCMenuItemImage::create(NULL,NULL,this,
                                                                                          menu_selector(CCDialog::CancelButCallBack));
    if(pCanelBut)
    {
        pCanelBut->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(DIALOG_DEFAULT_BUTBG));
        pCanelBut->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(DIALOG_DEFAULT_BUTBGF));
        mfButW = pCanelBut->getContentSize().width;
        mfButH = pCanelBut->getContentSize().height;
        pCanelBut->setPosition(ccp(pBoxFrame->getContentSize().width-DIALOG_DEFAULT_OFF-mfButW/2,DIALOG_DEFAULT_OFF+mfButH/2));
        CCSprite *ptxt = CCSprite::createWithSpriteFrameName(m_sRButName.c_str());
        if(ptxt)
        {
            ptxt->setPosition(ccp(mfButW/2,mfButH/2));
            pCanelBut->addChild(ptxt);
        }
    }
    
    CCMenuItemImage *pExitBut = CCMenuItemImage::CCMenuItemImage::create(NULL,NULL,this,
                                                                         menu_selector(CCDialog::CancelButCallBack));
    if(pExitBut)
    {
        pExitBut->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(DIALOG_DEFAULT_EXIT));
        pExitBut->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(DIALOG_DEFAULT_EXITF));
        mfButW = pExitBut->getContentSize().width;
        mfButH = pExitBut->getContentSize().height;
        pExitBut->setPosition(ccp(pBoxFrame->getContentSize().width-DIALOG_DEFAULT_EXITOFF-mfButW/2,
                                  pBoxFrame->getContentSize().height-DIALOG_DEFAULT_EXITOFF-mfButH/2));
    }
    
    m_pMenu = CCMenu::create(pExitBut,pOkBut,pCanelBut,NULL);
    if(m_pMenu)
    {
        m_pMenu->setContentSize(pBoxFrame->getContentSize());
        m_pMenu->setPosition(CCPointZero);
        pBoxFrame->addChild(m_pMenu);
    }
    
    CCLayer *pScollLayer = CCLayer::create();
    if(pScollLayer)
    {
        CCSize msize = CCSizeMake(pBoxFrame->getContentSize().width-DIALOG_DEFAULT_TXTHOFF*2,
                                  pBoxFrame->getContentSize().height-mfButH-DIALOG_DEFAULT_TXTVOFF*2-DIALOG_DEFAULT_OFF);

        pScollLayer->setContentSize(msize);
        pScollLayer->setPosition(ccp(0,msize.height));
        pScollLayer->setAnchorPoint(ccp(0,1));
        
        m_pScrollView = CCScrollView::create(msize,pScollLayer);
        if(m_pScrollView)
        {
            m_pScrollView->setPosition(ccp(DIALOG_DEFAULT_TXTHOFF,mfButH+DIALOG_DEFAULT_TXTVOFF));
            m_pScrollView->setDirection(kCCScrollViewDirectionBoth);
            pBoxFrame->addChild(m_pScrollView);
        }
        
        CCLabelTTFBoard *pTxt = CCLabelTTFBoard::create(m_sTxtCont.c_str(),
                                              CfgDataManage::getInstance()->GetTxtNameByKey("scene_font").c_str(),
                                              CfgDataManage::getInstance()->GetTxtSizeByKey("scene_size"),
                                                        //"LiSu", 30,
                                              CCSizeMake(msize.width, 0),kCCTextAlignmentCenter);
        if(pTxt)
        {
            if(pTxt->getContentSize().height > msize.height)
                pScollLayer->setContentSize(CCSizeMake(msize.width,pTxt->getContentSize().height));
            pTxt->setPosition(CCPointMake(pScollLayer->getContentSize().width/2, pScollLayer->getContentSize().height/2));
            pTxt->setAnchorPoint(ccp(0.5,0.5));
            pScollLayer->addChild(pTxt);
        }
    }
    
    CCScaleTo *pScale = CCScaleTo::create(DIALOG_DEFAULT_TIME, 1.0);
    CCCallFuncND *pfnEndCallBack = CCCallFuncND::create(this,callfuncND_selector(CCDialog::FlashEndCallback),this);
    CCSequence *pSeq = CCSequence::create(pScale,pfnEndCallBack,NULL);
    if(pScale && pfnEndCallBack && pSeq)
    {
        pBoxFrame->setScale(DIALOG_DEFAULT_INIT);
        pBoxFrame->runAction(pSeq);
    }
    return true;
}

void CCDialog::EndDialog(bool bok)
{
    removeFromParentAndCleanup(true);
    if(m_pTarget && m_pEndHandle)
    {
        (m_pTarget->*m_pEndHandle)(bok);
    }
}

void CCDialog::FlashEndCallback(CCNode* pSender, void* pData)
{
    initWithColor(DIALOG_DEFAULT_COLOR);
}

#pragma mark -移动按扭
/************************************************************************************************/
//移动按扭
CCMoveButton* CCMoveButton::create()
{
    CCMoveButton *pMoveButton = new CCMoveButton();
    if (pMoveButton && pMoveButton->init())
    {
        pMoveButton->autorelease();
        return pMoveButton;
    }
    CC_SAFE_DELETE(pMoveButton);
    return NULL;
}

CCMoveButton::CCMoveButton():m_bStop(false),m_bPress(false)
{
    
}

void CCMoveButton::setSelected(bool enabled)
{
    if (enabled)
    {
        m_eState = CCControlStateSelected;
    }
    else
    {
        m_eState = CCControlStateNormal;
    }
    CCControl::setSelected(enabled);
    needsLayout();
}

void CCMoveButton::setHighlighted(bool enabled)
{
    if(isSelected())
    {
        m_eState = CCControlStateSelected;
    }
    else if (enabled)
    {
        m_eState = CCControlStateHighlighted;
    }
    else
    {
        m_eState = CCControlStateNormal;
    }
    CCControl::setHighlighted(enabled);
    stopActionByTag(MOVEBUTTON_ZOOM_TAGID);
    needsLayout();
    if( m_zoomOnTouchDown )
    {
        float scaleValue = (isHighlighted() && isEnabled()) ? 1.1f : 1.0f;
        CCAction *zoomAction = CCScaleTo::create(0.05f, scaleValue);
        zoomAction->setTag(MOVEBUTTON_ZOOM_TAGID);
        this->runAction(zoomAction);
    }
}

bool CCMoveButton::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    if (!isTouchInside(touch) || !isEnabled() || !isVisible())
    {
        return false;
    }
    
    m_isPushed = true;
    this->setHighlighted(true);
    sendActionsForControlEvents(CCControlEventTouchDown);
    
    CCDelayTime *pDt = CCDelayTime::create(MOVEBUTTON_DEFAULT_TIME);
    CCCallFuncND *pfnEndCallBack = CCCallFuncND::create(this,callfuncND_selector(CCMoveButton::ButtonPressCallback),this);
    CCSequence *pSeq = CCSequence::create(pDt,pfnEndCallBack,NULL);
    if(pSeq)
    {
        pSeq->setTag(MOVEBUTTON_PRESS_TAGID);
        this->runAction(pSeq);
    }
    return true;
}

void CCMoveButton::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    m_isPushed = false;
    setHighlighted(false);
    
    if (isTouchInside(touch))
    {
        sendActionsForControlEvents(CCControlEventTouchUpInside);
    }
    else
    {
        sendActionsForControlEvents(CCControlEventTouchUpOutside);
    }
    ButtonClearPressAction();
}

void CCMoveButton::ccTouchCancelled(CCTouch* touch, CCEvent* event)
{
    CCControlButton::ccTouchCancelled(touch, event);
    ButtonClearPressAction();
}

void CCMoveButton::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    if(!isEnabled() || !isPushed() || !isSelected() || !isHighlighted() || m_bStop)
    {
        return;
    }
   
    if(isPressed())
    {
        CCPoint touchLocation = touch->getLocation();
        touchLocation = this->getParent()->convertToNodeSpace(touchLocation);
        this->setPosition(touchLocation);
    }
    else if(!isTouchInside(touch))
    {
        ButtonClearPressAction();
    }
}

void CCMoveButton::ButtonPressCallback(CCNode* pSender, void* pData)
{
    CCMoveButton *pMoveBut = (CCMoveButton *)pData;
    if(pMoveBut)
        sendActionsForControlEvents(CCControlEventTouchDragEnter);
    m_bPress = true;
}

void CCMoveButton::ButtonClearPressAction()
{
    stopActionByTag(MOVEBUTTON_PRESS_TAGID);
    m_bPress = false;
}

#pragma mark -光标输入框
/************************************************************************************************/
//光标输入框
CursorTextField::CursorTextField():m_pCursorSprite(NULL),m_pCursorAction(NULL),
m_u8LimitNum(CURSORTEXT_DETLIMT),m_RectVaild(CCRectZero),m_bCharFlash(false)
{

}

CursorTextField::~CursorTextField()
{
    
}

CursorTextField *CursorTextField::TextFieldWithPlaceHolder(const char *placeholder, const char *fontName, float fontSize, CCObject *pObj)
{
    CursorTextField *pRet = new CursorTextField();
    if(pRet && pRet->initWithString("", fontName, fontSize))
    {
        pRet->autorelease();
        if(placeholder)
            pRet->setPlaceHolder(placeholder);
        pRet->InitCursorSprite(fontSize);
        pRet->SetMsgHander(pObj);
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

void CursorTextField::onEnter()
{
    CCTextFieldTTF::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
    this->setDelegate(this);
}

void CursorTextField::onExit()
{
    SetIMEState(false);
    CCTextFieldTTF::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

// input text property
void CursorTextField::setString(const char *text)
{
    CC_SAFE_DELETE(m_pInputText);
    
    if (text)
    {
        m_pInputText = new std::string(text);
    }
    else
    {
        m_pInputText = new std::string;
    }
    
    // if there is no input text, display placeholder instead
    if (! m_pInputText->length())
    {
        CCLabelTTF::setString(m_pPlaceHolder->c_str());
    }
    else
    {
        CCLabelTTF::setString(m_pInputText->c_str());
    }
    m_nCharCount = CalcCharCount(m_pInputText->c_str());
}

bool CursorTextField::onTextFieldAttachWithIME(CCTextFieldTTF * sender)
{
    if(m_pCursorSprite)
    {
        m_pCursorSprite->setPositionX(sender->getCharCount()>0 ? getContentSize().width:0);
        m_pCursorSprite->setVisible(true);
    }
    return false;
}

bool CursorTextField::onTextFieldDetachWithIME(CCTextFieldTTF * sender)
{
    if(m_pCursorSprite)
        m_pCursorSprite->setVisible(false);
    return false;
}

bool CursorTextField::onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen)
{
    if('\n'==*text)
        return false;
    
    if(m_nCharCount >= m_u8LimitNum)
        return true;
    
    m_nCharCount += CalcCharCount(text);
    std::string sOldText(*m_pInputText);
    std::string sText(*m_pInputText);
    sText.append(text);
    setString(sText.c_str());
    while(m_nCharCount > m_u8LimitNum)
        deleteBackward();
    
    std::string sAddText(text,m_pInputText->length()-sOldText.length());
    StartAddCharFlash(sender,sAddText.c_str());
    if(m_pCursorSprite)
        m_pCursorSprite->setPositionX(getContentSize().width);
    return false;
}

bool CursorTextField::onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen)
{
    if(m_pCursorSprite)
    {
        m_pCursorSprite->setPositionX(getContentSize().width);
       if(sender->getCharCount() < 1)
           m_pCursorSprite->setPositionX(0);
    }
    return false;
}

void CursorTextField::keyboardWillShow(CCIMEKeyboardNotificationInfo& info)
{
    if(m_pMsgHander && isVisible())
        (MsgSystem::Instance())->SendMsg(this, m_pMsgHander, SYS_KEYBOARD_SHOW, 0, &info);
}

void CursorTextField::keyboardWillHide(CCIMEKeyboardNotificationInfo& info)
{
    if(m_pMsgHander && isVisible())
        (MsgSystem::Instance())->SendMsg(this, m_pMsgHander, SYS_KEYBOARD_HIDE, 0, &info);
}

bool CursorTextField::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return isVisible();
}

void CursorTextField::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if(isVisible())
        SetIMEState(isInTextField(pTouch));
}

void CursorTextField::SetText(const char *text)
{
    setString(text);
    StartAddCharFlash(this, text);
}

void CursorTextField::insertText(const char * text, int len)
{
    std::string sInsert(text, len);
    
    // insert \n means input end
    int nPos = sInsert.find('\n');
    if ((int)sInsert.npos != nPos)
    {
        len = nPos;
        sInsert.erase(nPos);
    }
    
    if (len > 0)
    {
        if (m_pDelegate && m_pDelegate->onTextFieldInsertText(this, sInsert.c_str(), len))
        {
            // delegate doesn't want to insert text
            return;
        }
        
    }
    
    if ((int)sInsert.npos == nPos) {
        return;
    }
    
    // '\n' inserted, let delegate process first
    if (m_pDelegate && m_pDelegate->onTextFieldInsertText(this, "\n", 1))
    {
        return;
    }
    
    // if delegate hasn't processed, detach from IME by default
    detachWithIME();
}

void CursorTextField::deleteBackward()
{
    int nStrLen = m_pInputText->length();
    if (! nStrLen)
    {
        // there is no string
        return;
    }
    
    // get the delete byte number
    int nDeleteLen = 1;    // default, erase 1 byte
    
    while(0x80 == (0xC0 & m_pInputText->at(nStrLen - nDeleteLen)))
    {
        ++nDeleteLen;
    }
    
    std::string sDelText(m_pInputText->c_str() + nStrLen - nDeleteLen);
    StartDelCharFlash(this,sDelText.c_str());
    // if all text deleted, show placeholder string
    if (nStrLen <= nDeleteLen)
    {
        CC_SAFE_DELETE(m_pInputText);
        m_pInputText = new std::string;
        m_nCharCount = 0;
        CCLabelTTF::setString(m_pPlaceHolder->c_str());
    }
    else
    {
        // set new input text
        std::string sText(m_pInputText->c_str(), nStrLen - nDeleteLen);
        setString(sText.c_str());
    }

    if (m_pDelegate && m_pDelegate->onTextFieldDeleteBackward(this, sDelText.c_str(), nDeleteLen))
    {
        return;
    }
}

void CursorTextField::ClearFlahsEndCallback(CCNode* pSender, void* pData)
{
    CCLabelTTFBoard *pLabel = (CCLabelTTFBoard*)pData;
    if(pLabel)
        this->removeChild(pLabel, true);
}

void CursorTextField::StartAddCharFlash(CCTextFieldTTF *psender, const char *ptext)
{
    CCLabelTTFBoard *pLabel = CCLabelTTFBoard::create(ptext, getFontName(), getFontSize());
    if(!m_bCharFlash || !psender || !pLabel || !ptext)
        return;
    
    pLabel->setColor(CURSORTEXT_DEFCOLOR);
    addChild(pLabel);
    
    CCPoint endPos = CCPointMake(psender->getContentSize().width, psender->getContentSize().height/2);;
    CCPoint beginPos = CCPointMake(endPos.x, endPos.y+pLabel->getContentSize().height*2);
    
    pLabel->setPosition(beginPos);
    pLabel->setScale(CURSORTEXT_DEFSIZE);
    
    CCMoveTo *pMove = CCMoveTo::create(CURSORTEXT_DEFTIME, endPos);
    CCScaleTo *pScal = CCScaleTo::create(CURSORTEXT_DEFTIME, 1.0f);
    CCFadeOut *pFadeOut = CCFadeOut::create(CURSORTEXT_DEFTIME);
    CCSpawn *pSpawn = CCSpawn::create(pMove,pScal,pFadeOut,NULL);
    CCCallFuncND *pfnEndCallBack = CCCallFuncND::create(this,callfuncND_selector(CursorTextField::ClearFlahsEndCallback),pLabel);
    CCSequence *pSeq = CCSequence::create(pSpawn,pfnEndCallBack,NULL);
    pLabel->runAction(pSeq);
}

void CursorTextField::StartDelCharFlash(CCTextFieldTTF *psender, const char *ptext)
{
    CCLabelTTFBoard *pDelLabel = CCLabelTTFBoard::create(ptext, getFontName(), getFontSize());
    if(!m_bCharFlash || !psender || !pDelLabel || !ptext)
        return;
    
    pDelLabel->setColor(psender->getColor());
    addChild(pDelLabel);
    
    CCPoint beginPos = CCPointMake(psender->getContentSize().width, psender->getContentSize().height/2);
    CCPoint endPos = CCPointMake(-psender->getContentSize().width*2, beginPos.y);;

    pDelLabel->setPosition(beginPos);
    
    CCMoveTo *pMove = CCMoveTo::create(CURSORTEXT_DISTIME, endPos);
    CCRotateBy *pRot = CCRotateBy::create(CURSORTEXT_DISTIME/CURSORTEXT_DISREPT, (rand()%2) ? 360:-360);
    CCRepeat *pRet = CCRepeat::create(pRot, CURSORTEXT_DISREPT);
    CCFadeOut *pFadeOut = CCFadeOut::create(CURSORTEXT_DISTIME);
    CCSpawn *pSpawn = CCSpawn::create(pMove,pRet,pFadeOut,NULL);
    CCCallFuncND *pfnEndCallBack = CCCallFuncND::create(this,callfuncND_selector(CursorTextField::ClearFlahsEndCallback),pDelLabel);
    CCSequence *pSeq = CCSequence::create(pSpawn,pfnEndCallBack,NULL);
    pDelLabel->runAction(pSeq);
}

void CursorTextField::InitCursorSprite(int mHeight)
{
    int clonum = 4;
    int pixels[mHeight][clonum];
    for(int i=0; i<mHeight; i++)
    {
        for(int j=0; j<clonum; j++)
        {
            pixels[i][j] = 0xffffffff;
        }
    }
    
    CCTexture2D *pTexture = new CCTexture2D();
    if(!pTexture)
        return;
    pTexture->initWithData(pixels, kCCTexture2DPixelFormat_RGB888,1,1,CCSizeMake(clonum,mHeight));
    m_pCursorSprite = CCSprite::createWithTexture(pTexture);
    if(!m_pCursorSprite)
        return;
    CCSize winSize = getContentSize();
    m_pCursorSprite->setPosition(ccp(0,winSize.height/2));
    m_pCursorSprite->setVisible(false);
    addChild(m_pCursorSprite);
    
    CCFadeIn *pFadeIn = CCFadeIn::create(CURSORTEXT_FLAGTIME);
    CCFadeOut *pFadeOut = CCFadeOut::create(CURSORTEXT_FLAGTIME);
    CCSequence *pSeq = CCSequence::create(pFadeOut,pFadeIn,NULL);
    m_pCursorAction = CCRepeatForever::create(pSeq);
    if(m_pCursorAction)
        m_pCursorSprite->runAction(m_pCursorAction);
    
}

bool CursorTextField::isInTextField(CCTouch *pTouch)
{
    CCPoint touchLocation = pTouch->getLocation();
    touchLocation = this->getParent()->convertToNodeSpace(touchLocation);
    return m_RectVaild.containsPoint(touchLocation);
}

void CursorTextField::SetIMEState(bool bopen)
{
    if(bopen)
        attachWithIME();
    else
        detachWithIME();
}

int CursorTextField::CalcCharCount(const char * pszText)
{
    int n = 0;
    char ch = 0;
    while ((ch = *pszText))
    {
        CC_BREAK_IF(! ch);
        
        if (0x80 != (0xC0 & ch))
        {
            ++n;
        }
        ++pszText;
    }
    return n;
}

#pragma mark -系统繁忙提示
/************************************************************************************************/
//系统繁忙提示
SysTip* SysTip::create()
{
    SysTip *pRet = new SysTip();
    if(pRet && pRet->init())
        pRet->autorelease();
    return pRet;
}

void SysTip::onEnter()
{
    CCLayer::onEnter();
    
//    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(SYSBUSYTIP_TPNAME);
//    CCScale9Sprite *pBoxFrame = CCScale9Sprite::createWithSpriteFrameName(SYSBUSYTIP_FRAMNAME);
//    if(!pBoxFrame)
//        return;
//    
//    CCSize tSize = CCDirector::sharedDirector()->getWinSize();
//    pBoxFrame->setContentSize(CCSizeMake(tSize.width*2/5, tSize.height/5));
//    pBoxFrame->setPosition(ccp(tSize.width/2, tSize.height/2));
//    addChild(pBoxFrame);
//    
//    CCLabelTTFBoard *pTxt = CCLabelTTFBoard::create(CfgDataManage::Instance()->GetTxtNameByKey("drawhelp_busy_tip").c_str(),
//                                          CfgDataManage::Instance()->GetTxtNameByKey("scene_font").c_str(),
//                                          CfgDataManage::Instance()->GetTxtSizeByKey("scene_size"),
//                                          CCSizeMake(pBoxFrame->getContentSize().width, 0),kCCTextAlignmentCenter);
//    if(pTxt)
//    {
//        pTxt->setPosition(CCPointMake(pBoxFrame->getContentSize().width/2, pBoxFrame->getContentSize().height/2));
//        pTxt->setAnchorPoint(ccp(0.5,0.5));
//        pBoxFrame->addChild(pTxt);
//    }
//    //屏蔽所有priority比自己大的消息
//    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, TOUHANDPRITY_SYS,true);
}

void SysTip::onExit()
{
    CCLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

#pragma mark -滚动条列表
/************************************************************************************************/
//滚动条列表
CCTableBarView *CCTableBarView::create(CCTableViewDataSource* dataSource, CCSize size)
{
    return CCTableBarView::create(dataSource, size, NULL);
};


CCTableBarView* CCTableBarView::create(CCTableViewDataSource* dataSource, CCSize size, CCNode *container)
{
    CCTableBarView *table = new CCTableBarView();
    table->initWithViewSize(size, container);
    table->autorelease();
    table->setDataSource(dataSource);
    table->_updateContentSize();
    return table;
}

CCTableBarView::CCTableBarView():m_pListBg(NULL),m_fBarLHOff(0),m_fBarVOff(0),m_fBarFillTVOff(0),
m_iBarBgID(0),m_iPageNum(0),m_iTotolNum(0),m_iShowNum(0),m_iSedId(0),m_Point(0,0)
{
    
}

void CCTableBarView::onEnter()
{
    CCTableView::onEnter();
    CreateTableBar();
    if(m_iPageNum>0 && m_iTotolNum>0)
        scheduleUpdate();
}

void CCTableBarView::onExit()
{
    CCTableView::onExit();
    if(m_pListBg)
        m_pListBg->release();
}

void CCTableBarView::update(float delta)
{
    UpdateBarPos();
}

bool CCTableBarView::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_iSedId = -1;
    m_Point = ccp(-1,-1);
    bool bbegin = CCTableView::ccTouchBegan(pTouch, pEvent);
//    if(bbegin )
//    {
//        if (m_pTouches->count() == 1 && !this->isTouchMoved()) {
//            unsigned int        index;
//            CCTableViewCell   *cell;
//            CCPoint           point;
//            
//            point = this->getContainer()->convertTouchToNodeSpace(pTouch);
//            if (m_eVordering == kCCTableViewFillTopDown) {
//                float extra = m_pDataSource->cellSizeForTable(this).height;
//                unsigned cell_ct = m_pDataSource->numberOfCellsInTableView(this);
//                if (! m_pDataSource->hasFixedCellSize()) {
//                    if (cell_ct < 1) {
//                        extra = 0;
//                    } else {
//                        extra = m_pDataSource->cellSizeForIndex(this, cell_ct-1).height;
//                    }
//                }
//                point.y -= extra;
//            }
//            index = this->_indexFromOffset(point);
//            cell  = this->_cellWithIndex(index);
//            if (cell) {
//                m_Point = point;
//                m_iSedId = index;
//            }
//        }
//    }
    return bbegin;
}

void CCTableBarView::SetBarInfo(CCNode *pListBg, int mbarid, int mpagenum, int mtotal, float mHoff, float mVoff, float mFVoff)
{
    if(m_pListBg)
        m_pListBg->release();
    m_pListBg = pListBg;
    if(m_pListBg)
        m_pListBg->retain();
    m_iBarBgID = mbarid;
    m_iPageNum = mpagenum;
    m_fBarLHOff = mHoff;
    m_fBarVOff = mVoff;
    m_fBarFillTVOff = mFVoff;
    SetBarTotalNum(mtotal);
}

void CCTableBarView::SetBarTotalNum(int mtotal)
{
    m_iTotolNum=mtotal;
    m_iShowNum = min(m_iPageNum,m_iTotolNum);
    ResetBarPos();
}

void CCTableBarView::ResetContOff()
{
    if (m_eDirection == kCCScrollViewDirectionHorizontal)
    {
        this->setContentOffset(ccp(0,0));
    }
    else
    {
        this->setContentOffset(ccp(0,this->minContainerOffset().y));
    }
}

void CCTableBarView::SetPageTurn(bool bpre)
{
    CCPoint mpoint = ccp(0,0);
    CCPoint mOffpot = getContentOffset();
    CCPoint mmaxpot = maxContainerOffset();
    CCPoint mminpot = minContainerOffset();
    if(m_eDirection==kCCScrollViewDirectionHorizontal)
    {
        mpoint = ccp(getViewSize().width,0);
    }
    else
    {
        mpoint = ccp(0,getViewSize().height);
    }
    if(!bpre)
    {
        mpoint.x = -mpoint.x;
        mpoint.y = -mpoint.y;
    }
    mOffpot.x += mpoint.x;
    mOffpot.y += mpoint.y;
 
    if(mOffpot.x < mminpot.x)
        mOffpot.x = mminpot.x;
    if(mOffpot.x > mmaxpot.x)
        mOffpot.x = mmaxpot.x;

    if(mOffpot.y < mminpot.y)
        mOffpot.y = mminpot.y;
    if(mOffpot.y > mmaxpot.y)
        mOffpot.y = mmaxpot.y;

    setContentOffset(mOffpot,true);
}

void CCTableBarView::CreateTableBar()
{
//    CC_RETURN_IF(!IsBarEnabled());
//    
//    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(TABLEBAR_VIEW_TPNAME);
//    CCSprite *pBarBg =  CCSprite::createWithSpriteFrameName(TABLEBAR_VIEW_BARBG);
//    CC_RETURN_IF(!pBarBg);
//    pBarBg->setPosition(ccp(m_fBarLHOff+pBarBg->getContentSize().width/2,m_pListBg->getContentSize().height/2+m_fBarVOff));
//    m_pListBg->addChild(pBarBg,0,m_iBarBgID);
//    
//    CCSprite *pBarM =  CCSprite::createWithSpriteFrameName(TABLEBAR_VIEW_FILLM);
//    CC_RETURN_IF(!pBarM);
//    pBarM->setPosition(ccp(pBarBg->getContentSize().width/2,pBarBg->getContentSize().height - m_fBarFillTVOff));
//    pBarM->setAnchorPoint(ccp(0.5,1));
//    if(m_iTotolNum>m_iPageNum)
//        pBarM->setScaleY((float)m_iPageNum/(float)m_iTotolNum);
//    pBarBg->addChild(pBarM,0,TABLEBAR_CTRLID_FILLM);
//    
//    CCSprite *pBarU =  CCSprite::createWithSpriteFrameName(TABLEBAR_VIEW_FILLU);
//    CC_RETURN_IF(!pBarU);
//    pBarU->setPosition(pBarM->getPosition());
//    pBarBg->addChild(pBarU,0,TABLEBAR_CTRLID_FILLU);
//    
//    CCSprite *pBarD =  CCSprite::createWithSpriteFrameName(TABLEBAR_VIEW_FILLD);
//    CC_RETURN_IF(!pBarD);
//    pBarD->setPosition(ccp(pBarM->getPositionX(),
//                           pBarM->getPositionY()-pBarM->getContentSize().height*pBarM->getScaleY()));
//    pBarBg->addChild(pBarD,0,TABLEBAR_CTRLID_FILLD);
}

void CCTableBarView::ResetBarPos()
{
//    CCNode *pBarBg =  m_pListBg ? (CCNode *)(m_pListBg->getChildByTag(m_iBarBgID)):NULL;
//    CC_RETURN_IF(!pBarBg);
//    
//    CCSprite *pBarM = (CCSprite *)(pBarBg->getChildByTag(TABLEBAR_CTRLID_FILLM));
//    pBarM->setScaleY(1.0f);
//    pBarM->setPosition(ccp(pBarBg->getContentSize().width/2,pBarBg->getContentSize().height - m_fBarFillTVOff));
//    if(m_iTotolNum>m_iPageNum)
//        pBarM->setScaleY((float)m_iPageNum/(float)m_iTotolNum);
//    
//    CCSprite *pBarU =  (CCSprite *)(pBarBg->getChildByTag(TABLEBAR_CTRLID_FILLU));
//    CC_RETURN_IF(!pBarU);
//    pBarU->setPosition(pBarM->getPosition());
//    
//    CCSprite *pBarD =  (CCSprite *)(pBarBg->getChildByTag(TABLEBAR_CTRLID_FILLD));
//    CC_RETURN_IF(!pBarD);
//    pBarD->setPosition(ccp(pBarM->getPositionX(),
//                           pBarM->getPositionY()-pBarM->getContentSize().height*pBarM->getScaleY()));
}

void CCTableBarView::UpdateBarPos()
{
//    CCSize mtableSize = this->getContentSize();
//    CCSize mviewSize = this->getViewSize();
//    if(this->getDirection() == kCCScrollViewDirectionVertical)
//    {
//        CC_RETURN_IF(mtableSize.height<=mviewSize.height);
//        CCPoint curoffPoint = this->getContentOffset();
//        float fmaxoff = mviewSize.height - mtableSize.height;
//        float fcuroff = curoffPoint.y>fmaxoff ? curoffPoint.y-fmaxoff : 0;
//        float mCelH = getViewSize().height/m_iPageNum;
//        m_iShowNum = fabs(fcuroff)==0 ? 0:(fabs(fcuroff)/mCelH)+1;
//        m_iShowNum += m_iPageNum;
//        if(m_iShowNum > m_iTotolNum)
//            m_iShowNum = m_iTotolNum;
//        
//        CCNode *pBarBg =  m_pListBg ? (CCNode *)(m_pListBg->getChildByTag(m_iBarBgID)):NULL;
//        CC_RETURN_IF(!pBarBg);
//        CCSprite *pBarM = (CCSprite *)(pBarBg->getChildByTag(TABLEBAR_CTRLID_FILLM));
//        CCSprite *pBarU = (CCSprite *)(pBarBg->getChildByTag(TABLEBAR_CTRLID_FILLU));
//        CCSprite *pBarD = (CCSprite *)(pBarBg->getChildByTag(TABLEBAR_CTRLID_FILLD));
//        CC_RETURN_IF(!pBarM || !pBarU || !pBarD);
//        
//        CCPoint moffsetPot = this->getContentOffset();
//        fmaxoff = mviewSize.height - mtableSize.height;
//        fcuroff = moffsetPot.y - fmaxoff;
//        float fpercent = fabs(fcuroff)/fabs(fmaxoff);
//        
//        if(fpercent < 0.1f)
//            fpercent = 0;
//        if(fpercent > 1.0f)
//            fpercent = 1;
//        
//        float mbarMH = pBarM->getContentSize().height*pBarM->getScaleY();
//        float fbarTopPosY = pBarBg->getContentSize().height - m_fBarFillTVOff;
//        float fbarLowPosY = m_fBarFillTVOff + mbarMH;
//        float fPosY = fbarTopPosY - fpercent*(fbarTopPosY-fbarLowPosY);
//        pBarM->setPositionY(fPosY);
//        pBarU->setPositionY(fPosY);
//        pBarD->setPositionY(fPosY-mbarMH);
//    }
//    else
//    {
//        CC_RETURN_IF(mtableSize.width<=mviewSize.width);
//        
//        CCPoint curoffPoint = this->getContentOffset();
//        float fmaxoff = mviewSize.width - mtableSize.width;
//        float fcuroff = curoffPoint.x>fmaxoff ? curoffPoint.x-fmaxoff : 0;
//        float mCelW = getViewSize().width/m_iPageNum;
//        m_iShowNum = fabs(fcuroff)==0 ? 0:(fabs(fcuroff)/mCelW)+1;
//        m_iShowNum += m_iPageNum;
//        if(m_iShowNum > m_iTotolNum)
//            m_iShowNum = m_iTotolNum;
//    }
}

bool CCTableBarView::IsBarEnabled()
{
    return (m_pListBg && m_iPageNum>0 && m_iTotolNum>0 && m_iBarBgID>0);
}

#pragma mark -文字内容提示
TipMsg* TipMsg::create(const char *ptxt, float mTime, CCPoint mpot, float voff, ccColor3B color, ccColor3B linecolor, float size, float linesize)
{
    TipMsg *pRet = new TipMsg();
    if(pRet && pRet->init())
    {
        pRet->autorelease();
        pRet->SetTipMsg(ptxt, mTime, mpot, voff, color, linecolor, size, linesize);
    }
    return pRet;
}

bool TipMsg::init()
{
    if(!CCLayer::init())
        return false;
    
//    CCSize tSize = CCDirector::sharedDirector()->getWinSize();
//    
//    CCLabelTTFBoard *pTxt = CCLabelTTFBoard::create("",
//                                          CfgDataManage::Instance()->GetTxtNameByKey("scene_font").c_str(),
//                                          CfgDataManage::Instance()->GetTxtSizeByKey("scene_size_tipmsg"),
//                                          CCSizeMake(tSize.width/2, 0),kCCTextAlignmentCenter);
//    if(pTxt)
//    {
//        pTxt->setPosition(CCPointMake(tSize.width/2, tSize.height/2));
//        pTxt->setAnchorPoint(ccp(0.5,0.5));
//        pTxt->setColor(TIPMSG_DEFAULT_COLOR);
//        addChild(pTxt,0,TIPMSG_ID_TXT);
//    }
    return true;
}

void TipMsg::onEnter()
{
    CCLayer::onEnter();
    
    CCLabelTTFBoard *pLabelCtrl = (CCLabelTTFBoard *)getChildByTag(TIPMSG_ID_TXT);
    CCMoveBy *pMoveBg = CCMoveBy::create(m_fShowTime, ccp(0,m_fVoffDis));
    CCFadeOut *pFadeOut = CCFadeOut::create(m_fShowTime);
    CCCallFuncND *pfnEndCallBack = CCCallFuncND::create(this,callfuncND_selector(TipMsg::MsgTipCallback),this);
    if(pLabelCtrl && pLabelCtrl->getString() && pMoveBg && pFadeOut && pfnEndCallBack)
    {
        pLabelCtrl->RunAction(pMoveBg);
        pLabelCtrl->RunAction(CCSequence::create(pFadeOut,pfnEndCallBack,NULL));
        //屏蔽所有priority比自己大的消息
        //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, TOUHANDPRITY_SYS,true);
    }
    else
    {
        MsgTipCallback(this,this);
    }
}

void TipMsg::onExit()
{
    CCLayer::onExit();
    removeAllChildrenWithCleanup(true);
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

void TipMsg::SetTipMsg(const char *ptxt, float mTime, CCPoint pot, float mVdis, ccColor3B color, ccColor3B linecolor, float size, float linesize)
{
    m_fShowTime = mTime;
    m_fVoffDis = mVdis;
    CCLabelTTFBoard *pLabelCtrl = (CCLabelTTFBoard *)getChildByTag(TIPMSG_ID_TXT);
    if(!ptxt || !pLabelCtrl)
        return;
    
    pLabelCtrl->setString(ptxt);
    pLabelCtrl->setPosition(pot);
    ccColor3B oldColor = pLabelCtrl->getColor();
    if(color.r != oldColor.r || color.g != oldColor.g || color.b != oldColor.b)
        pLabelCtrl->setColor(color);
    if(size>0 && size != pLabelCtrl->getFontSize())
        pLabelCtrl->setSize(size);
    pLabelCtrl->SetBoardColor(linecolor);
    pLabelCtrl->SetBoardSize(linesize);
}

void TipMsg::MsgTipCallback(CCNode* pSender, void* pData)
{
    removeFromParentAndCleanup(true);
}

#pragma mark -画线
CCDrawLineHelp* CCDrawLineHelp::create(CCPoint mStart, CCPoint mEnd, ccColor4B mcolor, float mW)
{
    CCDrawLineHelp *pRet = new CCDrawLineHelp(mStart,mEnd,mW,mcolor);
    if(pRet && pRet->init())
    {
        pRet->autorelease();
    }
    return pRet;
}

CCDrawLineHelp::CCDrawLineHelp(CCPoint mStart, CCPoint mEnd, float mW,ccColor4B mcolor)
{
    SetLineInfo(mStart,mEnd,mW,mcolor);
}

void CCDrawLineHelp::draw()
{
    CHECK_GL_ERROR_DEBUG();
    glLineWidth(m_fW);
    ccDrawColor4B(m_Color.r, m_Color.g, m_Color.b, m_Color.a);
    ccDrawLine(m_Start,m_End);
    CHECK_GL_ERROR_DEBUG();
}

#pragma mark -文字描边
CCLabelTTFBoard *CCLabelTTFBoard::create(const char *string, const char *fontName, float fontSize,
                                         ccColor3B mcolor,float mboardsize,
                                         const CCSize dimensions,
                                         CCTextAlignment hAlignment,
                                         CCVerticalTextAlignment vAlignment)
{
    CCLabelTTFBoard *pRet = new CCLabelTTFBoard();
    if(pRet && pRet->InitWithString(string,fontName,fontSize,mboardsize,mcolor,dimensions,hAlignment,vAlignment))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

CCLabelTTFBoard* CCLabelTTFBoard::create(const char *string, const char *fontName, float fontSize,
                                         const CCSize dimensions, CCTextAlignment hAlignment,
                                         CCVerticalTextAlignment vAlignment)
{
    CCLabelTTFBoard *pRet = new CCLabelTTFBoard();
    if(pRet && pRet->InitWithString(string,fontName,fontSize,TTFBOARD_DEFAULT_SIZE,TTFBOARD_DEFAULT_COLOR,dimensions,hAlignment,vAlignment))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;

}

CCLabelTTFBoard::CCLabelTTFBoard():m_pBoardSprt(NULL),m_pLable(NULL)
{
    ignoreAnchorPointForPosition(false);
    setAnchorPoint(ccp(0.5,0.5));
}

bool CCLabelTTFBoard::InitWithString(const char *string, const char *fontName, float fontSize,float mboardsize,
                    ccColor3B mcolor, const CCSize dimensions,CCTextAlignment hAlignment,
                    CCVerticalTextAlignment vAlignment)
{
    m_fBoardSize = mboardsize;
    m_BoardColor = mcolor;
    m_pLable = CCLabelTTF::create(string,fontName,fontSize,dimensions,hAlignment,vAlignment);
    if(m_pLable)
        m_pLable->retain();
    UpdateBoard();
    return true;
}

void CCLabelTTFBoard::setColor(ccColor3B color)
{
    if(m_pLable)
    {
        ccColor3B mcor = m_pLable->getColor();
        if(mcor.r!=color.r || mcor.g!=color.g || mcor.b!=color.b)
        {
            m_pLable->setColor(color);
            UpdateBoard();
        }
    }
}

void CCLabelTTFBoard::setSize(float msize)
{
    if(m_pLable && m_pLable->getFontSize()!=msize)
    {
        m_pLable->setFontSize(msize);
        UpdateBoard();
    }
}

void CCLabelTTFBoard::setString(const char *label)
{
   if(m_pLable && 0!=strcmp(label,m_pLable->getString()))
   {
       m_pLable->setString(label);
       UpdateBoard();
   }
}

void CCLabelTTFBoard::SetBoardSize(float msize)
{
    if(m_fBoardSize != msize)
    {
        m_fBoardSize = msize;
        UpdateBoard();
    }
}

void CCLabelTTFBoard::SetBoardColor(ccColor3B mcolor)
{
    if(m_BoardColor.r != mcolor.r || m_BoardColor.g != mcolor.g || m_BoardColor.b != mcolor.b)
    {
        m_BoardColor = mcolor;
        UpdateBoard();
    }
}

void CCLabelTTFBoard::setHorizontalAlignment(CCTextAlignment alignment)
{
    if(m_pLable)
        m_pLable->setHorizontalAlignment(alignment);
}

void CCLabelTTFBoard::setVerticalAlignment(CCVerticalTextAlignment verticalAlignment)
{
    if(m_pLable)
        m_pLable->setVerticalAlignment(verticalAlignment);
}

void CCLabelTTFBoard::RunAction(CCAction* action)
{
    if(!action)
        return;
    
    if(m_pLable)
        m_pLable->runAction(action);
    if(m_pBoardSprt)
        m_pBoardSprt->runAction(action);
}

const char *CCLabelTTFBoard::getFontName()
{
    return m_pLable ? m_pLable->getFontName():NULL;
}

float CCLabelTTFBoard::getFontSize()
{
    return m_pLable ? m_pLable->getFontSize():0;
}

const char *CCLabelTTFBoard::getString()
{
    return m_pLable ? m_pLable->getString():NULL;
}

const ccColor3B &CCLabelTTFBoard::getColor()
{
    return m_pLable ? m_pLable->getColor():ccWHITE;
}

void CCLabelTTFBoard::UpdateBoard()
{
    if(!m_pLable)
        return;
    
    if(m_pBoardSprt)
        removeChild(m_pBoardSprt, true);
    
    CCSize mtextSize = m_pLable->getContentSize();
    mtextSize.width += 2*m_fBoardSize;
    mtextSize.height += 2*m_fBoardSize;
    if(mtextSize.height==0 || mtextSize.width==0)
        return;
    
    glGetError();
    CCRenderTexture *pRendTexture = CCRenderTexture::create(mtextSize.width, mtextSize.height);
    if(!pRendTexture)
        return;
    
    ccColor3B moldcolor = m_pLable->getColor();
    m_pLable->setColor(m_BoardColor);
    ccBlendFunc originalBlend = m_pLable->getBlendFunc();
    ccBlendFunc func = {GL_SRC_ALPHA,GL_ONE};
    m_pLable->setBlendFunc(func);
    m_pLable->setAnchorPoint(ccp(0.5,0.5));
    
    pRendTexture->begin();
    for(int i=0; i<360; i+=15)
    {
        float r = CC_DEGREES_TO_RADIANS(i);
        m_pLable->setPosition(ccp(mtextSize.width*0.5f + sin(r)*m_fBoardSize,mtextSize.height*0.5f + cos(r)*m_fBoardSize));
        m_pLable->visit();
    }
    
    m_pLable->setColor(moldcolor);
    m_pLable->setBlendFunc(originalBlend);
    m_pLable->setPosition(ccp(mtextSize.width*0.5f,mtextSize.height*0.5f));
    m_pLable->visit();
    pRendTexture->end();
    
    CCTexture2D *pTexture = pRendTexture->getSprite()->getTexture();
    pTexture->setAliasTexParameters();
    m_pBoardSprt = CCSprite::createWithTexture(pTexture);
    setContentSize(pTexture->getContentSize());
    m_pBoardSprt->setAnchorPoint(ccp(0,0));
    m_pBoardSprt->setPosition(ccp(0,0));
    m_pBoardSprt->setFlipY(true);
    addChild(m_pBoardSprt);
}

#pragma mark -粉碎动画
CCBrokenPiece* CCBrokenPiece::create(float duration, const CCSize& gridSize, float range, float aparam, unsigned int seed)
{
    CCBrokenPiece *pAction = new CCBrokenPiece();
    if(pAction)
    {
        if(pAction->initWithDuration(duration, gridSize, range, seed, aparam))
            pAction->autorelease();
        else
            CC_SAFE_RELEASE_NULL(pAction);
    }
    return pAction;
}

CCBrokenPiece::~CCBrokenPiece(void)
{
    CC_SAFE_DELETE_ARRAY(m_pTilesOrder);
    CC_SAFE_DELETE_ARRAY(m_pTiles);
}

bool CCBrokenPiece::initWithDuration(float duration, const CCSize& gridSize, float range, unsigned int seed, float aparam)
{
    if(CCShuffleTiles::initWithDuration(duration, gridSize,seed))
    {
        m_fRange = range;
        m_fAparam = aparam;
        return true;
    }
    return false;
}

void CCBrokenPiece::startWithTarget(CCNode *pTarget)
{
    CCTiledGrid3DAction::startWithTarget(pTarget);
    
    if (m_nSeed != (unsigned int)-1)
    {
        srand(m_nSeed);
    }
    
    m_fTime = m_fRange>0 ? m_fDuration/m_fRange:BROKEN_DEF_TIME;
    m_nTilesCount = m_sGridSize.width * m_sGridSize.height;
    m_pTilesOrder = new unsigned int[m_nTilesCount];
    int i, j;
    unsigned int k;
    
    /**
     * Use k to loop. Because m_nTilesCount is unsigned int,
     * and i is used later for int.
     */
    if(m_pTilesOrder)
    {
        for (k = 0; k < m_nTilesCount; ++k)
        {
            m_pTilesOrder[k] = k;
        }
    }
     
    shuffle(m_pTilesOrder, m_nTilesCount);
    
    m_pBPTiles = new BROKEN_TILE[m_nTilesCount];
    BROKEN_TILE *ptileArray = (BROKEN_TILE*)m_pBPTiles;
    if(ptileArray)
    {
        for(i=0; i<m_sGridSize.width; ++i)
        {
            for(j=0; j<m_sGridSize.height; ++j)
            {
                ptileArray->position = ccp((float)i, (float)j);
                ptileArray->delta = getDelta(CCSizeMake(i, j)); //CCSizeMake(i%2==0 ? 10:-10, j%2==0? 10:-10);//
                ++ptileArray;
            }
        }
    }
}

CCObject* CCBrokenPiece::copyWithZone(CCZone *pZone)
{
    CCZone* pNewZone = NULL;
    CCBrokenPiece* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject)
    {
        pCopy = (CCBrokenPiece*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCBrokenPiece();
        pZone = pNewZone = new CCZone(pCopy);
    }
    CCTiledGrid3DAction::copyWithZone(pZone);
    pCopy->initWithDuration(m_fDuration, m_sGridSize, m_fRange, m_nSeed, m_fAparam);
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCBrokenPiece::update(float time)
{
    BROKEN_TILE *ptileArray = (BROKEN_TILE *)m_pBPTiles;
    if(ptileArray)
    {
        for(int i=0; i<m_sGridSize.width; i++)
        {
            for(int j=0; j<m_sGridSize.height; j++)
            {
                ptileArray->position = ccpMult(ccp((float)(ptileArray->delta.width), (float)(ptileArray->delta.height)),m_fTime);
                ShowTile(ccp(i, j), ptileArray);
                ptileArray++;
            }
        }
        m_fTime *= m_fAparam;
    }
}

void CCBrokenPiece::ShowTile(const CCPoint& pos, BROKEN_TILE *pt)
{
    if(!pt)
        return;
    
    ccQuad3 coords = originalTile(pos);
    
    CCPoint step = m_pTarget->getGrid()->getStep();

    coords.bl.x += (int)(pt->position.x * step.x);
    coords.bl.y += (int)(pt->position.y * step.y);
    coords.br.x += (int)(pt->position.x * step.x);
    coords.br.y += (int)(pt->position.y * step.y);
    coords.tl.x += (int)(pt->position.x * step.x);
    coords.tl.y += (int)(pt->position.y * step.y);
    coords.tr.x += (int)(pt->position.x * step.x);
    coords.tr.y += (int)(pt->position.y * step.y);
    
    if(m_fRange>0 && (fabs(pt->position.x * step.x)>m_fRange || fabs(pt->position.y * step.y)>m_fRange))//消失
        memset(&coords, 0, sizeof(ccQuad3));
    setTile(pos, coords);
}

CCSize CCBrokenPiece::getDelta(const CCSize& pos)
{
    CCPoint    pos2;
    unsigned int idx = pos.width * m_sGridSize.height + pos.height;
    pos2.x = (float)(m_pTilesOrder[idx] / (int)m_sGridSize.height);
    pos2.y = (float)(m_pTilesOrder[idx] % (int)m_sGridSize.height);
    return CCSizeMake((int)(pos2.x - pos.width), (int)(pos2.y - pos.height));
}

NS_ENGINE_END