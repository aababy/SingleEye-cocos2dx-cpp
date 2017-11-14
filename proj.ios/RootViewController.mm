#include "MTNotificationQueue.h"
#include "Block.h"
#import "RootViewController.h"
#import "UMSocial.h"


@implementation RootViewController

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/

/*
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
}
 
*/
// Override to allow orientations other than the default portrait orientation.
// This method is deprecated on ios6
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
  //  return UIInterfaceOrientationIsLandscape( interfaceOrientation );
    return UIInterfaceOrientationIsPortrait(interfaceOrientation);
}

// For ios6, use supportedInterfaceOrientations & shouldAutorotate instead
- (NSUInteger) supportedInterfaceOrientations{
#ifdef __IPHONE_6_0
    return UIInterfaceOrientationMaskAllButUpsideDown;
#endif
}

- (BOOL) shouldAutorotate {
    return YES;
}

//fix not hide status on ios7
- (BOOL)prefersStatusBarHidden
{
    return YES;
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}


//下面得到分享完成的回调
-(void)didFinishGetUMSocialDataInViewController:(UMSocialResponseEntity *)response
{
    NSLog(@"didFinishGetUMSocialDataInViewController with response is %@",response);
    
    CCInteger *res;    
    //根据`responseCode`得到发送结果,如果分享成功
    if(response.responseCode == UMSResponseCodeSuccess)
    {
        //得到分享到的微博平台名
        NSString *sPlatform = [[response.data allKeys] objectAtIndex:0];
        const char * szPlatform =[sPlatform UTF8String];

        if (strcmp(szPlatform, "facebook") == 0 || strcmp(szPlatform, "twitter") == 0) {
            res = CCInteger::create(1);
        }
        else
        {
            res = CCInteger::create(0);
        }
    }
    else
    {
        res = CCInteger::create((int)response.responseCode);
    }
        
    //retain 后, 必须要释放
    res->retain();
    
    xMTNotify->postNotification(NC_ShareFinish, res);
}


-(void)didCloseUIViewController:(UMSViewControllerType)fromViewControllerType
{
    xBlock->unblock();
}


#pragma mark - UMUFPBannerView delegate methods

//该方法在广告获取成功时调用
- (void)UMUFPBannerView:(UMUFPBannerView *)_banner didLoadDataFinish:(NSInteger)promotersAmount {
    
    NSLog(@"%s, amount:%d", __PRETTY_FUNCTION__, promotersAmount);
}

//该方法在广告获取失败时调用
- (void)UMUFPBannerView:(UMUFPBannerView *)banner didLoadDataFailWithError:(NSError *)error {
    
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

//实现该方法将可以捕获banner即将出现这一事件，自定义banner首次展示的动画
//- (void)bannerWillAppear:(UMUFPBannerView *)_banner {
//    
//    _banner.transform = CGAffineTransformScale(CGAffineTransformIdentity, 0.01, 0.01);
//    
//    [UIView beginAnimations:nil context:nil];
//    [UIView setAnimationDuration:0.6f];
//    _banner.transform = CGAffineTransformScale(CGAffineTransformIdentity, 1.0, 1.0);
//    [UIView commitAnimations];
//    
//    NSLog(@"%s", __PRETTY_FUNCTION__);    
//}

//该方法在广告被点击后调用
- (void)UMUFPBannerView:(UMUFPBannerView *)_banner didClickedPromoterAtIndex:(NSInteger)index {
    
    NSLog(@"%s, index:%d", __PRETTY_FUNCTION__, index);    
}




@end
