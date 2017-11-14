
#include "LocalCondition.h"

#import <Foundation/Foundation.h>

ELocalId CLocalCondition::getLocalCondition()
{
    //获取国家和地区列表
//    NSMutableArray *countriesArray = [[NSMutableArray alloc] init];
//    NSLocale *locale = [NSLocale currentLocale];
//    NSArray *countryArray = [NSLocale ISOCountryCodes];
//
//    for (NSString *countryCode in countryArray)
//    {
//        NSString *displayNameString = [locale displayNameForKey:NSLocaleCountryCode value:countryCode];
//        [countriesArray addObject:displayNameString];
//        NSLog(@"d[w_x]b Locale: %@", displayNameString);
//    }

    // 当前所在地信息
    NSString *pIdentifier = [[NSLocale currentLocale] localeIdentifier];
    NSString *displayName = [[NSLocale currentLocale] displayNameForKey:NSLocaleIdentifier value:pIdentifier];
    NSLog(@"d[w_x]b Current Locale is %@ of %@", pIdentifier, displayName);
    
    if(NSOrderedSame == [pIdentifier compare:@"zh_TW"])//台湾
    {
        return LOCAL_ID_TAIWAN;
    }
    else if(NSOrderedSame == [pIdentifier compare:@"zh_HK"]
            || NSOrderedSame == [pIdentifier compare:@"zh-Hans_HK"])//香港
    {
        return LOCAL_ID_XIANGGANG;
    }
    else if(NSOrderedSame == [pIdentifier compare:@"zh_MO"]
            || NSOrderedSame == [pIdentifier compare:@"zh-Hans_MO"])//澳门
    {
        return LOCAL_ID_AOMEN;
    }
    else if(NSOrderedSame == [pIdentifier compare:@"zh_SG"])//新加坡
    {
        return LOCAL_ID_XINJIAPO;
    }
    else if(NSOrderedSame == [pIdentifier compare:@"zh_CN"])//大陆
    {
        return LOCAL_ID_ZHONGGUO;
    }
    else//英文 美元
    {
        return LOCAL_ID_EN;
    }
    
//    // 当前所在地的使用语言
//    NSLocale *currentLocale = [NSLocale currentLocale];
//    NSLog(@"Language Code is %@", [currentLocale objectForKey:NSLocaleLanguageCode]);
//    
//    // 系统语言
//    NSArray *arLanguages = [[NSUserDefaults standardUserDefaults] objectForKey:@"AppleLanguages"];
//    NSString *strLang = [arLanguages objectAtIndex:0];
//    NSLog(@"LANG:%@",strLang);
}
//前往评论页面
void CLocalCondition::toPingLun()
{
    /*
    mt 代表 meta-type，有效值如下：
    1   Music
    2   Podcasts
    3   Audiobooks
    4   TV Shows
    5   Music Videos
    6   Movies
    7   iPod Games
    8   Mobile Software Applications
    9   Ringtones
    10  iTunes U
    11  E-Books
    12  Desktop Apps
    当链接进行查询时，如果没有定义id，就有可能出现不同类别的内容，但是名字相同，例如某专辑的名字和某个app的名字重合。这时mt就起作用了。
    ls代表link special，当查询的类型为某首歌曲时，不定义ls，默认指向歌曲的专辑，定义后直接进入该歌曲并试播。（貌似是这样，apple的论坛里看到的，跟app的关系不大）
    */
    //打开AppStore
    NSString *pStringOfAppStore = [NSString stringWithFormat:@"itms-apps://itunes.apple.com/%@/app/shui-guo-ying-xiong-chuan-shuo/id%@?ls=1&mt=8", @"us", @"804430674"];
//test    NSString *pStringOfAppStore = [NSString stringWithFormat:@"itms-apps://itunes.apple.com/cn/app/yi-xin-mian-fei-liao-tian/id683688634?mt=8"];
    
//    //打开iTunes
//    NSString *pStringOfITunes = [NSString stringWithFormat:@"itms://itunes.apple.com/%@/app/shui-guo-ying-xiong-chuan-shuo/id%@?ls=1&mt=8", @"us", @"804430674"];
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:pStringOfAppStore]];
}
