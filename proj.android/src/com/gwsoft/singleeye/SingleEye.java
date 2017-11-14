/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package com.gwsoft.singleeye;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;



import com.tencent.connect.common.Constants;
import com.tencent.mm.sdk.openapi.BaseReq;
import com.tencent.mm.sdk.openapi.BaseResp;
import com.tencent.mm.sdk.openapi.IWXAPI;
import com.tencent.mm.sdk.openapi.IWXAPIEventHandler;
import com.tencent.mm.sdk.openapi.SendAuth;
import com.umeng.analytics.MobclickAgent;
import com.umeng.socialize.bean.SHARE_MEDIA;
import com.umeng.socialize.bean.SocializeEntity;
import com.umeng.socialize.controller.RequestType;
import com.umeng.socialize.controller.UMServiceFactory;
import com.umeng.socialize.controller.UMSocialService;
import com.umeng.socialize.controller.listener.SocializeListeners.SnsPostListener;
import com.umeng.socialize.media.CircleShareContent;
import com.umeng.socialize.media.MailShareContent;
import com.umeng.socialize.media.QQShareContent;
import com.umeng.socialize.media.QZoneShareContent;
import com.umeng.socialize.media.UMImage;
import com.umeng.socialize.media.WeiXinShareContent;
import com.umeng.socialize.sso.SinaSsoHandler;
import com.umeng.socialize.sso.TencentWBSsoHandler;
import com.umeng.socialize.sso.UMSsoHandler;
import com.umeng.socialize.sso.UMWXHandler;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;

public class SingleEye extends Cocos2dxActivity{
		
	/**
     * Handler, 用于包装友盟的openShare方法，保证openShare方法在UI线程执行
     */
    private static Handler mHandler = null;
    /**
     * 保存当前Activity实例， 静态变量
     */
    private static Activity mActivity = null;
    /**
     * 友盟Social SDK实例，整个SDK的Controller
     */
    private static UMSocialService mController = UMServiceFactory.getUMSocialService(
                    "com.gwsoft", RequestType.SOCIAL);
    
    private static WeiXinShareContent weixinContent = null; 
    private static CircleShareContent circleMedia = null;
		
    protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);	
		
		mWXApi = null;
		
		mActivity = this;
				
        mController.getConfig().registerListener(mShareListener);
        mController.getConfig().setSsoHandler(new SinaSsoHandler());
        mController.getConfig().removePlatform(
                SHARE_MEDIA.DOUBAN, SHARE_MEDIA.QQ, SHARE_MEDIA.SMS, SHARE_MEDIA.QZONE, SHARE_MEDIA.EMAIL);
        
        //为了保证人人分享成功且能够在PC上正常显示，请设置website 
        mController.setAppWebSite(SHARE_MEDIA.RENREN, mTargetUrl);
        
        addWXPlatform(mActivity, "abc");  
        
        //Twitter
        mController.getConfig().supportAppPlatform(this, SHARE_MEDIA.TWITTER, "com.gwsoft.singleeye", true);
                                            
        // 分享⾯板中的平台将按照如下顺序进⾏排序, 微信朋友圈、⼈⼈⽹、新浪、QQ...... 
        mController.getConfig().setPlatformOrder(SHARE_MEDIA.SINA, 
        		SHARE_MEDIA.TENCENT, SHARE_MEDIA.WEIXIN, SHARE_MEDIA.WEIXIN_CIRCLE, SHARE_MEDIA.RENREN);
	}

    public Cocos2dxGLSurfaceView onCreateView() {
    	Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
    	// SingleEye should create stencil buffer
    	glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
    	
    	return glSurfaceView;
    }

    static {
        System.loadLibrary("cocos2dcpp");
    }     
    
    /**
     * @Title:  openShareBoard
     * @Description:
     *       调用友盟的openShare方法， 打开分享平台选择面板
     * @throws
     */
    public static void openShareBoard(final String shareTxt) {
    	
    	System.out.println("share");
    	
        mHandler = new Handler(Looper.getMainLooper());
        mHandler.postDelayed(new Runnable() {

            @Override
            public void run() {
                if (mActivity != null) {
                    /**
                     *   设置新浪微博、QQ空间、腾讯微博的Handler,进行SSO授权
                     *
                     *   注意 ： 新浪平台支持SSO则需要把友盟提供的com.sina.sso拷进src里面，
                     *      需要将友盟提供的libs、res文件拷贝到您的工程对应的目录里面.
                     */
                    //mController.getConfig().setSsoHandler(new SinaSsoHandler());
                    mController.getConfig().setSsoHandler(new TencentWBSsoHandler());
                    
                    
                    //设置分享内容
                    mController.setShareContent(shareTxt);
                    weixinContent.setShareContent(shareTxt);
                    circleMedia.setShareContent(shareTxt);
                    

                    // 打开友盟的分享平台选择面板
                    mController.openShare(mActivity, false);
                }
            }
        }, 100);
    }
    
    
    /**
     * @功能描述 : 添加微信平台分享
     * @return
     */
    public void addWXPlatform(Activity activty, String strShare) {

    	String appID = "wx61f93ce949686969";
    	
    	UMWXHandler wxHandler = mController.getConfig().supportWXPlatform(activty,
    			appID, mTargetUrl);
        wxHandler.setWXTitle(mstrShareTitle);

        UMWXHandler cycHandler = mController.getConfig().supportWXCirclePlatform(activty,
        		appID, mTargetUrl);
        cycHandler.setCircleTitle(mstrShareTitle);

        UMImage localImage = new UMImage(activty, R.drawable.icon);
        weixinContent = new WeiXinShareContent(localImage);
        //weixinContent.setShareContent(strShare);
        weixinContent.setTitle("水果英雄传说");
        mController.setShareMedia(weixinContent);

        // 设置朋友圈分享的内容
        circleMedia =
                new CircleShareContent(new UMImage(activty, R.drawable.icon));
        //circleMedia.setShareContent(strShare);
        mController.setShareMedia(circleMedia);
    }       

    public void onNewIntent(Activity activity, Intent intent) {
        if (mWXApi != null) {// for weixin call back
            activity.setIntent(intent);
            mWXApi.handleIntent(intent, new IWXAPIEventHandler() {
                @Override
                public void onResp(BaseResp arg0) {
                    SendAuth.Resp resp = (SendAuth.Resp) arg0;
                    System.out.println(resp.userName);
                }

                @Override
                public void onReq(BaseReq arg0) {}
            });
        }
    }
    
    @Override 
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
    	super.onActivityResult(requestCode, resultCode, data);
    	/**使用SSO授权必须添加如下代码 */
        UMSsoHandler ssoHandler = mController.getConfig().getSsoHandler(requestCode) ;
        if(ssoHandler != null){
            ssoHandler.authorizeCallBack(requestCode, resultCode, data);
        }
    }
    
    //主要用于微信
    private SnsPostListener mShareListener = new SnsPostListener() {

        @Override
        public void onStart() {
            //Toast.makeText(mContext, "share begin", Toast.LENGTH_SHORT).show();
        }

        @Override
        public void onComplete(SHARE_MEDIA platform, int eCode,
                SocializeEntity entity) {
            
            if (eCode == 200) {
            	onResponse(0);
            } 
            else 
            {
            	onResponse(eCode);
            }
        }
    };
    
    private String mTargetUrl = "http://www.umeng.com/social";
    private String mstrShareTitle = "Title";
    private IWXAPI mWXApi;
    
    
    //统计分析
    public void onResume() {
    	super.onResume();
    	MobclickAgent.onResume(this);
    }
    
    
    public void onPause() {
    	super.onPause();
    	MobclickAgent.onPause(this);
    }
     
    public static Object getActivity(){
    	return mActivity;
    }    
        
    public void mobclickAgentnEvent(String eventId, String label)
    {    	
    	if(label.length() == 0)
    	{
    		MobclickAgent.onEvent(this, eventId);
    	}
    	else
    	{
    		MobclickAgent.onEvent(this, eventId, label);    		
    	}
    }

    //native function
    native void onResponse(int eCode);
}
