//
//  Social.cpp
//  test04
//
//  Created by gwsoft on 12/30/13.
//
//
//#include "SocialDelegate.h"
#include "UMSocial.h"
#include "UMSocialWechatHandler.h"
//#include "UMSocialFacebookHandler.h"
#include "MobClick.h"
#include "UserData.h"
#include "Block.h"
#include "Social.h"
#include "GameResource.h"
#include "cocos2d.h"
#include "UMUFPBannerView.h"
#include "RootViewController.h"


//SocialDelegate* m_pDelegate = nil;
UMUFPBannerView *m_pBanner = nil;
RootViewController *m_viewController = nil;


Social* Social::getInstance()
{
    static Social inst;
    
    return &inst;
}


Social::~Social()
{
    //[m_pDelegate release];
    [m_pBanner release];
}


void Social::share(const char *shareTxt, const char *shareImage)
{      
    //加入屏蔽层
    xBlock->block();
    
    NSString *sTxt;
    if (shareTxt != NULL) {
        sTxt = [[NSString alloc] initWithCString:(const char*)shareTxt encoding:NSUTF8StringEncoding];
    }
    else{
        sTxt = [[NSString alloc] initWithFormat:@"%s", ""];
    }

    NSString *sImage = nil;
    if (shareImage != NULL) {
        sImage = [[NSString alloc] initWithFormat:@"%s", shareImage];
    }
    else{
        sImage = [[NSString alloc] initWithFormat:@"%s", ""];
    }
            

    [UMSocialSnsService presentSnsIconSheetView:m_viewController
                                         appKey:nil
                                      shareText:sTxt
                                     shareImage:[UIImage imageNamed:sImage]
                                shareToSnsNames:[NSArray arrayWithObjects:UMShareToSina,UMShareToTencent,UMShareToWechatSession, UMShareToWechatTimeline, UMShareToRenren, UMShareToFacebook, UMShareToTwitter, nil]
                                       delegate:m_viewController];
    
    [sTxt release];
    [sImage release];
    
    return;
}


void Social::showAD()
{   
    if (m_pBanner == nil) {
        m_pBanner = [[UMUFPBannerView alloc] initWithFrame:CGRectMake(0, 44, 320, 50) appKey:UmengAppkey slotId:nil currentViewController:m_viewController];
    }    

    m_pBanner.delegate = (id<UMUFPBannerViewDelegate>)m_viewController;
    [m_viewController.view addSubview:m_pBanner];    
    [m_pBanner requestPromoterDataInBackground]; 
}


void Social::hideAD()
{   
    if (m_pBanner != nil) {
        [m_pBanner removeFromSuperview]; 
    }
}



void Social::setLandscape()
{
    [UMSocialConfig setSupportedInterfaceOrientations:UIInterfaceOrientationMaskLandscape];
    
    //如果你要支持不同的屏幕方向，需要这样设置，否则在iPhone只支持一个竖屏方向
    //[UMSocialConfig setSupportedInterfaceOrientations:UIInterfaceOrientationMaskAll];
}


void Social::stat(const char *eventId)
{
    NSString *sTxt = [[NSString alloc] initWithFormat:@"%s", eventId];

    [MobClick event:sTxt];
    
    [sTxt release];
}


void Social::statByUserID(const char *eventId)
{
    NSString *sTxt = [[NSString alloc] initWithFormat:@"%s", eventId];

    int userId = UserData::Instance()->pUserInfo->userId;
    NSString *sUserID = [[NSString alloc] initWithFormat:@"%d", userId];
    
    [MobClick event:sTxt label:sUserID];
    
    [sTxt release];
    [sUserID release];
}


//如果key值是有意义的数值(如购买道具数量), 建议使用 "计算事件",
void Social::statByKey(const char *eventId, const char *eventKey)
{
    NSString *sTxt = [[NSString alloc] initWithFormat:@"%s", eventId];
    NSString *sKey = [[NSString alloc] initWithFormat:@"%s", eventKey];
    
    [MobClick event:sTxt label:sKey];
    
    [sTxt release];
    [sKey release];
}


void Social::statByKeyForCalc(const char *eventId, int iNumber, char *chapter)
{
    NSString *sTxt = [[NSString alloc] initWithFormat:@"%s", eventId];
    NSString *sChapter = [[NSString alloc] initWithFormat:@"%s", chapter];    
    NSNumber *number = [NSNumber numberWithInt:iNumber];  
    
    NSString *numberKey = @"__ct__";
    NSMutableDictionary *mutableDictionary = [NSMutableDictionary dictionaryWithDictionary:@{@"LevelPlay": sChapter}];                                                                                          
    [mutableDictionary setObject:[number stringValue] forKey:numberKey];
    [MobClick event:sTxt attributes:mutableDictionary];
    
    [sTxt release];    
    [sChapter release];   
}



void Social::init()
{
    //打开调试log的开关
    [UMSocialData openLog:YES];
    
    //设置友盟社会化组件appkey
    [UMSocialData setAppKey:UmengAppkey];
    
    //打开新浪微博的SSO开关
    [UMSocialConfig setSupportSinaSSO:YES];
    
    //设置微信AppId
    [UMSocialWechatHandler setWXAppId:@"wxd9a39c7122aa6516" url:nil];
    
    //账号功能未实现
    //设置Facebook，AppID和分享url，需要#import "UMSocialFacebookHandler.h"
    //默认使用iOS自带的Facebook分享framework，在iOS 6以上有效。若要使用我们提供的facebook分享需要使用此开关：
//    [UMSocialFacebookHandler setFacebookAppID:@"723727477661340" shareFacebookWithURL:@"http://www.umeng.com/social"];    
    
    //使用友盟统计
    [MobClick startWithAppkey:UmengAppkey];    
    
    //初始化delegate
    //m_pDelegate = [[SocialDelegate alloc]init];
}
void Social::setRootView(void *vRootView)
{
    m_viewController = (RootViewController*)vRootView;    
}
