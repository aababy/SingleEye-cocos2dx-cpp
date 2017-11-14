//
//  CCDrawHelp.h
//  coslua
//
//  Created by yuanlu on 13-5-29.
//
//

#ifndef __CCDrawHelp__
#define __CCDrawHelp__

#include "CCTPSprite.h"
#include "EngineDefine.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameResouce.h"

USING_NS_CC;
USING_NS_CC_EXT;
NS_ENGINE_BEGIN

#pragma mark -倒计时
typedef enum _TouchHandlerPriority_
{
    TOUHANDPRITY_BASE = kCCMenuHandlerPriority,//系统默认
    TOUHANDPRITY_USER = kCCMenuHandlerPriority - 1,//用户使用
    TOUHANDPRITY_SYS = kCCMenuHandlerPriority - 2,//系统提示
}TouHandPrity;//触屏优先级

/************************************************************************************************/
//倒计时
enum
{
    DRAWTIME_CTRL_LABLE = 1,//文字倒计时
    DRAWTIME_CTRL_FLAG_HM = 2,//符号
    DRAWTIME_CTRL_FLAG_MS = 3,
    DRAWTIME_CTRL_HOUR = 4,//小时(两位)
    DRAWTIME_CTRL_MIN = 6,//分(两位)
    DRAWTIME_CTRL_SEC = 8,//秒(两位)
};

class CCDrawTime: public CCNode
{
public:
    //创建（Tp图片名，数字图片名，间隔图片名）
    static CCDrawTime* create(CCTPSprite *pTpSprt=NULL,const char *pNumName=NULL, const char *pFlagName=NULL);
    
    CCDrawTime(CCTPSprite *pTpSprt, const char *pNumName, const char *pFlagName);
    ~CCDrawTime();
    //设置起始时间（分,秒,小时）
    void SetStartTime(int mtime_min, int mtime_sec=0, int mtime_hour=0);
    //手动设置时间（分,秒,小时）
    void SetTime(int mtime_min, int mtime_sec, int mtime_hour);
    //注册到时回调函数
    void SetFinishCallback(CCObject* target, SEL_CallFuncN selector){m_pListener=target; m_pfnSelector=selector;};
    //设置动行状态
    void ChangeState(bool bPause=false){m_bRun=!bPause;};

    //设置时间文字信息
    void SetLableInfo(const char *pfontname, int mfontsize, ccColor3B mcolor)
    {if(pfontname)m_sFontName=pfontname; m_iFontSize=mfontsize;m_FontColor=mcolor;};
    
private:
    void ChangeTime(float dt);//定时器（1s）
    void CreateTimeViewCtrl();//创建控件
    void UpdateTimeView();//改变图片
    
    bool m_bRun;//是否运行
    bool m_bGetSize;//是否已设置容器大小
    int m_iTimeHour;//小时
    int m_iTimeMin;//分
    int m_iTimeSec;//秒
    int m_iOnlySecCout;//只显示秒位数
    int m_iFontSize;//字体大小
    ccColor3B m_FontColor;//字体颜色
    std::string m_sNumName;//数字图片名
    std::string m_sFlagName;//间隔图片名
    std::string m_sFontName;//字体名字
    
    CCTPSprite *m_pTpSprt;//Tp资源名称
    CCObject*   m_pListener;//父窗体
    SEL_CallFuncN m_pfnSelector;//回调函数
};

#pragma mark -数字显示
/************************************************************************************************/
//数字显示
typedef enum _DRAWNUM_ANCHOR_
{
    DRAWNUM_ANCHOR_LEFT = 1,//左对齐
    DRAWNUM_ANCHOR_RIGHT,//右对齐
    DRAWNUM_ANCHOR_CENTER,//居中对齐
}DRAWNUM_ANCHOR;

class CCDrawNum: public CCNode
{
public:
    //创建（Tp图片名，数字图片名，效果图片名(NULL:只显示数字)）
    static CCDrawNum* create(CCTPSprite *pTpSprt,const char *pNumName, const char *pFlagName=NULL);
    
    CCDrawNum(CCTPSprite *pTpSprt, const char *pNumName, const char *pFlagName);
    ~CCDrawNum();
    //重置图片信息
    void SetPicInfo(CCTPSprite *pTpSprt, const char *pNumName, const char *pFlagName)
    {m_pTpSprt=pTpSprt;m_sNumName = pNumName;m_sFlagName.clear();if(pFlagName)m_sFlagName = pFlagName;};
    //设置显示数字
    void SetShowNum(UINT32 mdata, DRAWNUM_ANCHOR mAnchor=DRAWNUM_ANCHOR_CENTER);
    
private:
    std::string m_sNumName;//数字图片名
    std::string m_sFlagName;//效果图片名
    CCTPSprite *m_pTpSprt;//Tp资源名称
    UINT16 m_uData;
};

#pragma mark -进度条
/************************************************************************************************/
//进度条
#define CCDRAWBAR_DEF_FLAGHOFF  (-85)//默认Bar Flag 水平偏移(从左向右)

enum
{
    DRAWBAR_CTRL_BG = 1,//背景
    DRAWBAR_CTRL_FILL,//Bar
    DRAWBAR_CTRL_FLAG,//Bar Flag
};

class CCDrawBar: public CCNode
{
public:
    //创建（填充图，背景图，Bar图标）
    static CCDrawBar* create(const char *pImageFill, const char *pImageBg=NULL, const char *pImageFlag=NULL, bool brtol=false, bool bFlagFX=false);
    
    CCDrawBar(const char *pImageFill, const char *pImageBg, const char *pImageFlag, bool brtol, bool bFlagFX);
    ~CCDrawBar();
    
    void update(float delta);
    
    void SetBarFlagInfo(float mhoff=CCDRAWBAR_DEF_FLAGHOFF, bool bBgAnchor=true);//设置Bar图标水平偏移
    void SetBarPercentage(float fPercentage);//设置当前显示进度（0－100）
    void CreateMoveFlash(float duration, float fPercentage);//设置移动动画
    
private:
    bool m_bRightToLeft;//是否从右到左
    bool m_bBarBgAnchor;//Bar图标是否为背景对齐
    float m_fFlagHoff;//Bar图标水平偏移
    float m_fPercent;//当前进度
};

#pragma mark -对话框
/************************************************************************************************/
//对话框
#define DIALOG_DEFAULT_COLOR    (ccc4(0,0,0,125))//默认背景颜色
#define DIALOG_DEFAULT_OFF      (10)//默认按扭偏移
#define DIALOG_DEFAULT_EXITOFF  (5)//默认退出按扭偏移
#define DIALOG_DEFAULT_INIT     (0.5)//默认框体初始大小
#define DIALOG_DEFAULT_TIME     (0.2)//默认弹出动画时长
#define DIALOG_DEFAULT_TXTHOFF   (30)//默认内容水平偏移
#define DIALOG_DEFAULT_TXTVOFF   (10)//默认内容垂直偏移
#define DIALOG_DEFAULT_TPNAME1  (g_sPlistPublic)//默认Tp资源
#define DIALOG_DEFAULT_TPNAME2  (g_sPlistTxt)//默认Tp资源
#define DIALOG_DEFAULT_FRAMNAME (g_sImageTpInfoBox6)//默认框体资源
#define DIALOG_DEFAULT_BUTBG    (g_sImageTpButLeve4F)//默认按扭背景
#define DIALOG_DEFAULT_BUTBGF   (g_sImageTpButLeve4)//默认按扭背景选中
#define DIALOG_DEFAULT_EXIT     (g_sImageTpButCloseL2)//默认退出按扭
#define DIALOG_DEFAULT_EXITF   (g_sImageTpButCloseL2F)//默认退出按扭选中
#define DIALOG_DEFAULT_LBUT     (g_sImageTPOK)//默认左按扭

typedef void (CCObject::*DLG_CB_HANDLE)(bool bok);
#define DLG_HANDLE_SEL(_CALLBACK_) (DLG_CB_HANDLE)(&_CALLBACK_)

class CCDialog: public CCLayerColor
{
public:
    static CCDialog* create(const char *pContent, const char *pLBName=NULL, const char *pRBName=NULL);
    
    CCDialog(const char *pContent,const char *pLBName,const char *pRBName);
    ~CCDialog();
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    virtual void ccTouchCancelled(CCTouch *touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    
    virtual void OKButCallBack(CCObject *obj);//确定回调
    virtual void CancelButCallBack(CCObject *obj);//取消回调
    virtual bool OnInitDialog();//初始化
    
    void EndDialog(bool bok=true);//自动清除
    void RegisterEndFun(CCObject *ptarget, DLG_CB_HANDLE pCbHandle){m_pTarget=ptarget;m_pEndHandle=pCbHandle;};//注册消失回调
    
private:
    void FlashEndCallback(CCNode* pSender, void* pData);//显示动画回调
    CCObject *m_pTarget;//回调窗口
    DLG_CB_HANDLE m_pEndHandle;//消失回调
    bool m_bTouchedMenu;//是否按下菜单
    bool m_bTouchedView;//是否内容翻页
    CCMenu *m_pMenu;//菜单
    CCScrollView *m_pScrollView;//内容
    
    std::string m_sTxtCont;//文本内容
    std::string m_sLButName;//右软键名称
    std::string m_sRButName;//右软键名称
};

#pragma mark -移动按扭
/************************************************************************************************/
//移动按扭
#define MOVEBUTTON_DEFAULT_TIME  (0.2f)//默认按扭时长
#define MOVEBUTTON_PRESS_TAGID   (0xCCCB0002)//长按回调ID
#define MOVEBUTTON_ZOOM_TAGID    (0xCCCB0001)//按扭缩放ID

class CCMoveButton: public CCControlButton
{
public:
    CCMoveButton();
    ~CCMoveButton(){};
    
    static CCMoveButton* create();

    virtual void setSelected(bool enabled);
    virtual void setHighlighted(bool enabled);
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    virtual void ccTouchCancelled(CCTouch *touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    inline bool isPressed(){return m_bPress;};//是否长按
    void SetMoveState(bool bstop){m_bStop=bstop;};//是否去掉移动功能
    
private:
    void ButtonPressCallback(CCNode* pSender, void* pData);//按扭长按后回调
    void ButtonClearPressAction();//清除长按事件
    
    bool m_bPress;//是否长按
    bool m_bStop;//是否去掉移动
};

#pragma mark -光标输入框
/************************************************************************************************/
//光标输入框
#define CURSORTEXT_DETLIMT  (10)//默认最大位数
#define CURSORTEXT_FLAGTIME (0.25f)//光标动画时间
#define CURSORTEXT_DEFTIME (0.5f)//字符添加动画时间
#define CURSORTEXT_DISTIME (1.0f)//字符删除动画时间
#define CURSORTEXT_DISREPT (5)//字符删除动画重复次数
#define CURSORTEXT_DEFSIZE (8)//字符动画缩放倍数
#define CURSORTEXT_DEFCOLOR (ccc3(90,60,244))//字符动画颜色


class CursorTextField: public CCTextFieldTTF, public CCTextFieldDelegate, public CCTouchDelegate
{
public:
    CursorTextField();
    virtual ~CursorTextField();
    
    static CursorTextField *TextFieldWithPlaceHolder(const char *placeholder, const char *fontName, float fontSize, CCObject *pObj);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual void setString(const char *text);
    
    virtual bool onTextFieldAttachWithIME(CCTextFieldTTF * sender);
    virtual bool onTextFieldDetachWithIME(CCTextFieldTTF * sender);
    virtual bool onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen);
    virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen);
    
    virtual void keyboardWillShow(CCIMEKeyboardNotificationInfo& info);
    virtual void keyboardWillHide(CCIMEKeyboardNotificationInfo& info);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void SetCharFlash(bool bflash){m_bCharFlash=bflash;};//设置是否字符动画
    void SetLimitNum(byte num){m_u8LimitNum=num;};//设置显示位数
    void SetMsgHander(CCObject *pObj){m_pMsgHander=pObj;};//设置消息接收对象
    void SetVaildRect(CCRect mrect){m_RectVaild=mrect;};//设置点击区域
    void SetText(const char *text);//设置显示文字
    
protected:
     virtual void insertText(const char * text, int len);
     virtual void deleteBackward();
    
private:
    void ClearFlahsEndCallback(CCNode* pSender, void* pData);//字符动画回调
    void StartAddCharFlash(CCTextFieldTTF *psender, const char *ptext);//设置字符添加动画
    void StartDelCharFlash(CCTextFieldTTF *psender, const char *ptext);//设置字符删除动画
    
    void InitCursorSprite(int mHeight);//初始化光标
    bool isInTextField(CCTouch *pTouch);//判断是否点击在TextField处
    void SetIMEState(bool bopen);//设置输入法开关
    int CalcCharCount(const char * pszText);//计算字符长度
    
    bool m_bCharFlash;//字符动画开关
    byte m_u8LimitNum;//显示位数
    CCRect m_RectVaild;//点击有效区
    CCSprite *m_pCursorSprite;//光标精灵
    CCAction *m_pCursorAction;//光标动画
    CCObject *m_pMsgHander;//消息接收对象
};

#pragma mark -系统繁忙提示
/************************************************************************************************/
//系统繁忙提示
#define SYSBUSYTIP_TPNAME   (g_sPlistPublic)//默认Tp资源
#define SYSBUSYTIP_FRAMNAME (g_sImageTpInfoBox6)//默认框体资源

class SysTip: public CCLayer
{
public:
    static SysTip* create();
    
    SysTip(){};
    ~SysTip(){};
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event){return true;};
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event){};
    virtual void ccTouchCancelled(CCTouch *touch, CCEvent* event){};
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event){};
};

#pragma mark -滚动条列表
/************************************************************************************************/
//滚动条列表
#define TABLEBAR_VIEW_TPNAME   (g_sPlistPublic)//默认Tp资源
#define TABLEBAR_VIEW_BARBG    (g_sImageTpBarVBg)//默认进度条背景
#define TABLEBAR_VIEW_FILLM    (g_sImageTpBarVFillM)//默认进度条中间
#define TABLEBAR_VIEW_FILLU    (g_sImageTpBarVFillU)//默认进度条上部
#define TABLEBAR_VIEW_FILLD    (g_sImageTpBarVFillD)//默认进度条下部
#define TABLEBAR_HOFF       (5)//列表背景水平偏移
#define TABLEBAR_FILL_VOFF  (30)//列表BAR垂直偏移

typedef enum _TABLEBAR_CTRLID_
{
    TABLEBAR_CTRLID_FILLM = 1,//中间
    TABLEBAR_CTRLID_FILLU,//顶
    TABLEBAR_CTRLID_FILLD//底
}TABLEBAR_CTRLID;//滚动条填充

class CCTableBarView: public CCTableView
{
public:
    static CCTableBarView* create(CCTableViewDataSource* dataSource, CCSize size);
    static CCTableBarView* create(CCTableViewDataSource* dataSource, CCSize size, CCNode *container);
    
    CCTableBarView();
    ~CCTableBarView(){};
    
    virtual void onEnter();
    virtual void onExit();
    virtual void update(float delta);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    void SetBarInfo(CCNode *pListBg, int mbarid, int mpagenum, int mtotal, float mHoff=TABLEBAR_HOFF,
                    float mVoff=0, float mFVoff=TABLEBAR_FILL_VOFF);//设置进度条信息
    void SetBarTotalNum(int mtotal);//设置总行数
    void ResetContOff();//重置偏移
    void SetPageTurn(bool bpre=false);//翻页
    inline int GetShowNum(){return m_iShowNum;};//获取当前显示行数
    inline int GetCellId(){return m_iSedId;};//获取当前按下Cellid
    inline CCPoint GetCellPoint(){return m_Point;};//获取当前按下Cellid

private:
    void CreateTableBar();//创建进度条
    void ResetBarPos();//重置列表BarPos
    void UpdateBarPos();//更新列表Bar
    bool IsBarEnabled();//判断当前是否满足Bar使用条件
    
    CCNode *m_pListBg;//进度条父窗体
    CCPoint m_Point;//当前按下Point;
    int m_iBarBgID;//进度条背景ID
    int m_iPageNum;//列表单页显示数量
    int m_iTotolNum;//列表显示总数
    int m_iShowNum;//当前显示数
    int m_iSedId;//当前按下CellID
    float m_fBarLHOff;//进度条背景左偏移
    float m_fBarVOff;//进度条背景中心偏移
    float m_fBarFillTVOff;//进度条顶偏移
};

#pragma mark -文字内容提示
/************************************************************************************************/
//文字内容提示
#define TIPMSG_DEFAULT_COLOR    (ccc3(0,0,255))//默认背景颜色

typedef enum _TIPMSG_ID_
{
    TIPMSG_ID_TXT = 1,//文字
}TIPMSG_ID;

class TipMsg: public CCLayer
{
public:
    //建创提示（内容\时长＼垂直偏移）
    static TipMsg* create(const char *ptxt, float mTime, CCPoint mpot, float voff, ccColor3B color,
                          ccColor3B linecolor, float size, float linesize);
    
    TipMsg(){};
    ~TipMsg(){};
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event){return true;};
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event){};
    virtual void ccTouchCancelled(CCTouch *touch, CCEvent* event){};
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event){};
    
    void SetTipMsg(const char *ptxt, float mTime, CCPoint pot, float mVdis, ccColor3B color, ccColor3B linecolor, float size, float linesize);//设置提示内容
    
private:
    void MsgTipCallback(CCNode* pSender, void* pData);//提示结束回调
    
    float m_fShowTime;//显示时长
    float m_fVoffDis;//垂直偏移
};

#pragma mark -画线
/************************************************************************************************/
//画线
#define DRAWLINE_DEFAULT_COLOR  (ccc4(255,255,255,255))//默认画线颜色
#define DRAWLINE_DEFAULT_SIZE   (2.0f)//默认画线宽度

class CCDrawLineHelp: public CCLayer
{
public:
    
    static CCDrawLineHelp* create(CCPoint mStart, CCPoint mEnd, ccColor4B mcolor=DRAWLINE_DEFAULT_COLOR, float mW=DRAWLINE_DEFAULT_SIZE);
    
    CCDrawLineHelp(CCPoint mStart, CCPoint mEnd, float mW, ccColor4B mcolor);
    ~CCDrawLineHelp(){};
    
    virtual void draw();
    
    void SetLineInfo(CCPoint mStart, CCPoint mEnd, float mW, ccColor4B mcolor){m_Start=mStart;m_End=mEnd;m_fW=mW;m_Color=mcolor;};
    
private:
    CCPoint m_Start;//开始坐标
    CCPoint m_End;//结束坐标
    ccColor4B m_Color;//颜色
    float m_fW;//线宽
};

#pragma mark -文字描边
/************************************************************************************************/
//文字描边
#define TTFBOARD_DEFAULT_SIZE    (2.0f)//默认描边宽度
#define TTFBOARD_DEFAULT_COLOR   (ccc3(0,0,0))//默认描边宽度

class CCLabelTTFBoard: public CCNode 
{
public:
    
    static CCLabelTTFBoard* create(const char *string, const char *fontName, float fontSize, ccColor3B mcolor=TTFBOARD_DEFAULT_COLOR,
                                   float mboardsize=TTFBOARD_DEFAULT_SIZE, const CCSize dimensions=CCSizeZero,
                                   CCTextAlignment hAlignment=kCCTextAlignmentCenter,
                                   CCVerticalTextAlignment vAlignment=kCCVerticalTextAlignmentTop);
    static CCLabelTTFBoard* create(const char *string, const char *fontName, float fontSize, const CCSize dimensions,
                                   CCTextAlignment hAlignment=kCCTextAlignmentCenter,
                                   CCVerticalTextAlignment vAlignment=kCCVerticalTextAlignmentTop);
    
    bool InitWithString(const char *string, const char *fontName, float fontSize, float mboardsize,
                        ccColor3B mcolor, const CCSize dimensions,CCTextAlignment hAlignment,
                        CCVerticalTextAlignment vAlignment);
    
    CCLabelTTFBoard();
    ~CCLabelTTFBoard(){};
    
    void setSize(float msize);//设置文字大小
    void setString(const char *label);//设置显示文字
    void setColor(ccColor3B color);//设置文字颜色
    void SetBoardSize(float msize);//设置描边宽度
    void SetBoardColor(ccColor3B mcolor);//设置描边颜色
    void setHorizontalAlignment(CCTextAlignment alignment);//设置水平对齐
    void setVerticalAlignment(CCVerticalTextAlignment verticalAlignment);//设置垂直对齐
    void RunAction(CCAction* action);//设置动作
    
    const char *getFontName();//获取字体名字
    float getFontSize();//获取字体大小
    const char *getString();//获取文字内容
    const ccColor3B& getColor();//获取字体颜色
    
private:
    void UpdateBoard();//更新描边背景
    
    CCLabelTTF *m_pLable;//显示内容
    CCSprite *m_pBoardSprt;//描边背景
    ccColor3B m_BoardColor;//描边颜色
    float m_fBoardSize;//描边宽度
};

#pragma mark -粉碎动画
/************************************************************************************************/
//粉碎动画
#define BROKEN_DEF_TIME     (0.01f)//默认变化率
#define BROKEN_DEF_APARAM   (1.3f)//默认变化率
#define BROKEN_DEF_SEED     (1000)//默认随机种子

typedef struct _BROKEN_TILE_
{
    CCPoint    position;
    CCSize    delta;
}BROKEN_TILE;

class CCBrokenPiece : public CCShuffleTiles
{
public:
    ~CCBrokenPiece(void);
    static CCBrokenPiece* create(float duration, const CCSize& gridSize, float range=0,
                                 float aparam=BROKEN_DEF_APARAM, unsigned int seed=BROKEN_DEF_SEED);
    
    virtual bool initWithDuration(float duration, const CCSize& gridSize, float range, unsigned int seed, float aparam);
    virtual void startWithTarget(CCNode *pTarget);
    virtual CCObject* copyWithZone(CCZone* pZone);
    virtual void update(float time);
    
    void ShowTile(const CCPoint& pos, BROKEN_TILE *pt);
    CCSize getDelta(const CCSize& pos);
protected:
    float m_fRange;
    float m_fTime;
    float m_fAparam;
    BROKEN_TILE* m_pBPTiles;
};

NS_ENGINE_END

#endif
