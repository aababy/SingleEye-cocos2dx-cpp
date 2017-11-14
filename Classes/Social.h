//
//  Social.h
//  test04
//
//  Created by gwsoft on 12/30/13.
//
//

#ifndef __test04__Social__
#define __test04__Social__

#include <iostream>


#define UmengAppkey         @"52d4d5cf56240b429c014f75"


#define SLogin              "Login"
#define SBuyProp            "BuyProp"                   //购买道具, 每次购买道具成功
#define SRecruit            "Recruit"                   //招募, 每次招募成功
#define SUpdate             "Update"                    //升级, 每次升级成功

//单个道具的购买操作, 王文旭来补充.. (建议使用计算事件, 不需要userid) 
#define SBuyGold            "BuyGold"                   //购买金币,暂时不支持
#define SBuyGem             "BuyGem"                    //购买宝石,暂时不支持
#define SBuyBomb            "BuyBomb"                   //购买炸弹
#define SBuyIce             "BuyIce"                    //购买冰晶
#define SBuyFrost           "BuyFrost"                  //购买霜封
#define SBuyPlane           "BuyPlane"                  //购买轰炸机
#define SBuyFury            "BuyFury"                   //购买狂暴药剂
#define SBuyRecover         "BuyRecover"                //购买恢复药剂
#define SBuyStoneF          "BuyStoneF"                 //购买石栅栏
#define SBuyIronF           "BuyIronF"                  //购买铁栅栏
#define SBuyIronBow         "BuyIronBow"                //购买铁弹弓
#define SBuyMetalBow        "BuyMetalBow"               //购买合金连弩
#define SBuyRefresh         "BuyRefresh"                //刷新球


#define SPvP                "PvP"                       //PvP, 每次PvP成功.
#define SSign               "Sign"                      //签到
#define SShare              "Share"                     //分享
#define SGift               "Gift"                      //礼包
#define SEvent              "Event"                     //活动
#define SLevel              "LevelGame"                 //关卡游戏



#define xSocial             (Social::getInstance())


class Social
{
public:
    static Social* getInstance();
    ~Social();
    void share(const char *shareTxt, const char *shareImage);
    void setLandscape();
    
    void stat(const char *eventId);
    void statByUserID(const char *eventId);
    void statByKey(const char *eventId, const char *eventKey);
    void statByKeyForCalc(const char *eventId, int iNumber, char *chapter);
    void init();
    
    void showAD();
    void hideAD();
    
    void setRootView(void *vRootView);
    
private:
    Social(){};
    Social(const Social &){};
    
    //static Social* operate=(const Social&){ return getInstance(); };
};

#endif /* defined(__test04__Social__) */
