//
//  CfgDataManage.h
//  SingleEye
//
//  Created by qinfen on 13-11-6.
//
//

#ifndef __SingleEye__CfgDataManage__
#define __SingleEye__CfgDataManage__

//#include "GameInclude.h"
//#include "LogicDefine.h"
#include <iostream>
#include <math.h>

#include "cocos2d.h"

#include "cocos-ext.h"
#include "LuaManager.h"
#include "GameDefine.h"

#define MAX_NAME_STRLEN (60)//名字长度
USING_NS_CC;
using namespace cocos2d::extension;

typedef enum _LOAD_DATA_TYPE_
{
    LOAD_ROLEPARA = 1<<0,//角色数据
    LOAD_SKILLPARA = 1<<1,//技能数据
    LOAD_BUFFPARA = 1<<2,//Buff数据
    LOAD_ANIINFO = 1<<3,//动画数据
    LOAD_NSINFO = 1<<4,//女神数据
    LOAD_NAMEINFO = 1<<5,//名字字库
    LOAD_PROPINFO = 1<<6,//道具信息
    LOAD_EQUTINFO = 1<<7,//装备信息
    LOAD_PROPERTYINFO = 1<<8,//角色属性信息
    LOAD_COPYINFO = 1<<9,//副本信息
}LOAD_DATA_TYPE;

typedef enum _FIGHT_SKILLVIEW_TYPE_
{
    FIGHT_SKILLVIEW_NONE = -1,//无
    //卡牌普通攻击
    FIGHT_SKILLVIEW_SRPHYACT = 0,//近程物理劈砍攻击
    FIGHT_SKILLVIEW_SRPHYACT1,//近程物理刺击攻击
    FIGHT_SKILLVIEW_REPHYACT,//远程物理攻击
    FIGHT_SKILLVIEW_REMAGACT1,//远程魔法攻击（地）
    FIGHT_SKILLVIEW_REMAGACT2,//水
    FIGHT_SKILLVIEW_REMAGACT3,//风
    FIGHT_SKILLVIEW_REMAGACT4,//火
    FIGHT_SKILLVIEW_REMAGACT5,//光
    FIGHT_SKILLVIEW_REMAGACT6,//暗
    FIGHT_SKILLVIEW_GD,//格挡特效
    //神之语技能
    FIGHT_SKILLVIEW_SMNS_RNS=10,//生命女神－诺恩斯
    FIGHT_SKILLVIEW_LYZTS_MJL,//六翼战天使－米迦勒
    FIGHT_SKILLVIEW_MFZZ_HDS,//冥府主宰－哈迪斯
    //卡牌技能特效
    FIGHT_SKILLVIEW_YCGJGJ1=13,//远程弓箭类攻击1
    FIGHT_SKILLVIEW_YCGJGJ2,//远程弓箭类攻击2
    FIGHT_SKILLVIEW_HPHFJL1,//HP恢复技能1
    FIGHT_SKILLVIEW_MPHFJL1,//MP恢复技能1
    FIGHT_SKILLVIEW_AYJCMF1,//暗影系近程魔法攻击技能1
    FIGHT_SKILLVIEW_AYYCMF1,//暗影系远程魔法攻击技能1
    FIGHT_SKILLVIEW_JCWLJL1,//近程物理攻击技能1
    FIGHT_SKILLVIEW_JCWLJL2,//近程物理攻击技能2
    FIGHT_SKILLVIEW_GMJCMF1,//光明系近程魔法攻击技能1
    FIGHT_SKILLVIEW_TFMFYC1,//天赋魔法远程攻击技能1
    FIGHT_SKILLVIEW_JCCQJL1,//近程长枪类攻击技能1
    FIGHT_SKILLVIEW_JCDJL1,//近程刀类攻击技能1
    FIGHT_SKILLVIEW_YCBJL1,//远程鞭类攻击技能1
    FIGHT_SKILLVIEW_AYJCMF2,//暗影系近程魔法攻击技能2
    FIGHT_SKILLVIEW_BXYCMF1,//冰系远程魔法攻击技能1
    FIGHT_SKILLVIEW_BXYCMF2,//冰系远程魔法攻击技能2
    FIGHT_SKILLVIEW_AYJCMF3,//暗影系近程魔法攻击技能3
    FIGHT_SKILLVIEW_ZRYCMF1,//自然系远程魔法技能1
    FIGHT_SKILLVIEW_MAQUANT,//BOSS技能1（猛玛中心点全体攻击）
    FIGHT_SKILLVIEW_BINQUANT,//冰系中心全体攻击
    FIGHT_SKILLVIEW_LGLS,//莱格拉斯中心全体攻击
    FIGHT_SKILLVIEW_NUM,
}FIGHT_SKILLVIEW_TYPE;//技能表现

typedef enum _SPECIAL_PROPID_
{
    SPECIAL_PROPID_GLODEN = 1,//金币
    SPECIAL_PROPID_STREN,//体力
    SPECIAL_PROPID_STONE,//魔晶币
}SPECIAL_PROPID;

typedef enum _PROPERTY_TYPE_
{
    PROPERTY_TYPE_LIFE = 6,//生命上限
    PROPERTY_TYPE_PHYACT,//物理攻击
    PROPERTY_TYPE_MGCACT,//魔法攻击
    PROPERTY_TYPE_PHYDEF,//物理防卸
    PROPERTY_TYPE_MGCDEF,//魔法防卸
}PROPERTY_TYPE;

//typedef struct _FIGHT_ROLEPARA_INFO_
//{
//    int mRoleId;//ID 剧情
//    int mRoleId1;//ID 常规
//    int mShortId;//近程攻击ID
//    int mRemoteId;//远程攻击ID
//    GMRES_SF mResSF;//图片信息
//    GMRES_SF mResHead;//图片信息
//    char mNameStr[MAX_NAME_STRLEN];//文字
//}FIGHT_ROLEPARA_INFO;//打斗执行参数

//typedef struct _FIGHT_ROLEDATA_LIST_
//{
//    int mTotalNum;//总数
//    FIGHT_ROLEPARA_INFO *pParmInfo;//基础数据
//}FIGHT_ROLEDATA_LIST;//角色参数数据

typedef struct _FIGHT_SKILLPARA_INFO_
{
    bool bszycenter;//神之语技能是否中间播放
    int mSkillId;//ID
    int mViewId;//动画ID
    int mHurtTime;//伤害显示次数
    string mTxtName;//技能文字
    string mStrName;//技能名称
    string mImage;
}FIGHT_SKILLPARA_INFO;//技能参数

typedef struct _FIGHT_SKILLDATA_LIST_
{
    int mTotalNum;//总数
    int mBaseNum;//基础值
    FIGHT_SKILLPARA_INFO *pParmInfo;//基础数据
}FIGHT_SKILLDATA_LIST;//技能参数数据

typedef struct _FIGHT_BUFFPARA_INFO_
{
    int mBuffId;//ID
    int mTipId;//提示ID
    int mViewId;//动画ID
}FIGHT_BUFFPARA_INFO;//Buff参数

typedef struct _FIGHT_BUFFDATA_LIST_
{
    int mTotalNum;//总数
    FIGHT_BUFFPARA_INFO *pParmInfo;//基础数据
}FIGHT_BUFFDATA_LIST;//Buff参数数据

//typedef struct _FIGHT_ANI_INFO_
//{
//    GMRES_SF mSFList[FIGHT_ANITYPE_NUM];//攻击特效
//    float mfTime[FIGHT_ANITYPE_NUM];//攻击时长
//    CCPoint mPointOFF[FIGHT_ANITYPE_NUM];//偏移
//}FIGHT_ANI_INFO;//动画特效

//typedef struct _FIGHT_ANI_LIST_
//{
//    int mNum;//总数
//    FIGHT_ANI_INFO *pAniInfo;//基础数据
//}FIGHT_ANI_LIST;//动画特效数组
//
//typedef struct _NS_PIC_INFO_
//{
//    int mNsID;//ID
//    string mTxtCont;//说明文字
//    char mNameStr[MAX_NAME_STRLEN];
//    CCPoint mPointOFF;//Login中心偏移
//    GMRES_SF mBackSF;//女神背景图
//    GMRES_SF mResSF;//图片信息
//    GMRES_SF mNameSF;//名称信息
//    GMRES_SF mHeadSF;//头像信息
//    GMRES_SF mBodySF;//半身信息
//    GMRES_SF mBodyFullSF;//全身信息
//}NS_PIC_INFO;//女神图片信息

//typedef struct _NS_PIC_LIST_
//{
//    int mNum;//总数
//    NS_PIC_INFO *pPicInfo;//基础数据
//}NS_PIC_LIST;//女神列表信息

typedef struct _LOGIN_NAME_INFO_
{
    char mNameStr[MAX_NAME_STRLEN];//文字
}LOGIN_NAME_INFO;//名字信息

typedef struct _LOGIN_NMAE_LIST_
{
    int mNum;//总数
    LOGIN_NAME_INFO *pNameInfo;//名字信息
}LOGIN_NMAE_LIST;//登陆名字列表信息


//typedef struct _PROP_INFO_
//{
//    int mPropID;//ID
//    GMRES_SF mSFInfo;//图片信息
//    char mNameStr[MAX_NAME_STRLEN];//文字
//}PROP_INFO;//道具信息

//typedef struct _PROP_LIST_
//{
//    int mNum;//总数
//    PROP_INFO *pPropInfo;//列表信息
//}PROP_LIST;//道具列表信息
//
//typedef struct _EQUIP_INFO_
//{
//    int mEquipID;//ID
//    GMRES_SF mSFInfo;//图片信息
//    char mNameStr[MAX_NAME_STRLEN];//文字
//}EQUIP_INFO;//装备信息

//typedef struct _EQUIP_LIST_
//{
//    int mNum;//总数
//    EQUIP_INFO *pEquipInfo;//列表信息
//}EQUIP_LIST;//装备列表信息


//typedef struct _PROPERTY_INFO_
//{
//    int mPropertyID;//ID
//    GMRES_SF mSFInfo;//图片信息
//    char mNameStr[MAX_NAME_STRLEN];//文字
//    bool btype;
//}PROPERTY_INFO;

//typedef struct _PROPERTY_LIST_
//{
//    int mNum;//总数
//    PROPERTY_INFO *pPropertyInfo;//列表信息
//}PROPERTY_LIST;//装备列表信息

//typedef struct _COPY_INFO_
//{
//    int mCopyID;//ID
//    CCPoint mPoint;
//    char mNameStr[MAX_NAME_STRLEN];
//    char mNameTxt1[GMRES_IMAGE_FILELEN];
//    char mNameTxt2[GMRES_IMAGE_FILELEN];
//    char mNameImage1[GMRES_IMAGE_FILELEN];
//    char mNameImage2[GMRES_IMAGE_FILELEN];
//
//}COPY_INFO;

//typedef struct _COPY_LIST_
//{
//    int mNum;//总数
//    COPY_INFO *pCopyInfo;//列表信息
//}COPY_LIST;//副本列表信息


#define CFG_DATA_MGR    (CfgDataManage::getInstance())

//数据管理
class CfgDataManage
{
public:
    static CfgDataManage* getInstance();
    CfgDataManage();
    ~CfgDataManage();

    std::string getTxtByKey(const char *pKey,const char *pPath); //根据key获得相对应的值
    int getNumByKey(const char *pKey,const char *pPath); //根据key获得相对应的数值
    Fight_Level_Info* getLevelInfo(int nChapter ,int nLevel);   //获取关卡资源
    Fight_Level_Data* getLevelData(int nLevel);   //获取关卡数据

    Enemy_Data** getTotalEnemyData();      //获取怪数据
    Game_Hero_Data** getTotalHeroData();
    COutfitData** getTotalOutfitData();

    int getEnemyNum();
    int getHeroNum();
    int getOutfitNum();

    int getEnemyAwardID();  //掉落表返回id

    void SetLoadData(int mtype);
    void ClearCfgData();

    //FIGHT_ROLEPARA_INFO *GetRoleParmInfoById(const int mroleid);//通过角色ID,获得相应的基础数据
    FIGHT_SKILLPARA_INFO *GetSkillParmInfoById(const int mskillid);//通过技能ID,获得相应的基础数据
    FIGHT_BUFFPARA_INFO *GetBuffParmInfoById(const int mbuffid);//通过BuffID,获得相应的基础数据
    //    FIGHT_ANI_INFO *GetAniInfoById(const int mid);//通过特效ID,获得相应的基础数据
    //    NS_PIC_INFO *GetNSInfoById(const int mnsid);//通过女神ID,获得相应的基础数据
    //    NS_PIC_LIST *GetNsListInfo(){return m_pNSList;};//获取女神ID列表
    //    PROP_INFO *GetPropInfoById(const int propid);//通过道具ID,获得相应数据
    //    EQUIP_INFO *GetEquipInfoById(const int equipid);//通过装备ID,获得相应数据
    //    PROPERTY_INFO *GetPropertyInfoById(const int propertyid);//通过ID获得相应属性数据
    //    COPY_INFO *GetCopyInfoById(const int copyid);//通过ID获得相应副本数据
    std::string GetTxtNameByKey(const char *pkey);//通过Key,获得相应文字内容
    int GetTxtSizeByKey(const char *pkey);//通过Key,获得相应文体大小
    LOGIN_NMAE_LIST *GetLoginNameListInfo(bool bName1){return bName1? m_pName1List:m_pName2List;};//获取登陆名字字库信息
    inline int GetSkillBaseId(){return m_pSkillDataList ? m_pSkillDataList->mBaseNum:0;};//获得技能最小ID
    //    const char* GetImageNameByID(int id, int mtype=DROP_NONE, bool bhead=true, bool bfull=false);//通过ID(道具/装备/角色)获取图片
    //    const char* GetNameByID(int id, int mtype=DROP_NONE, bool bhead=true, bool bfull=false);//通过ID(道具/装备/角色)获取名称
private:
    void SetRoleDataList(LuaManager *plua);//设置角色基础数据
    void SetSkillDataList(LuaManager *plua);//设置技能基础数据
    void SetBuffDataList(LuaManager *plua);//设置Buff基础数据
    void SetAniList(LuaManager *plua);//特效基础数据
    void SetNSList(LuaManager *plua);//女神基础数据
    void GetLoginNameListInfo(LuaManager *plua, LOGIN_NMAE_LIST *pList, int mindex);//解析名字
    void SetLoginNameList(LuaManager *plua);//名字字库
    void SetPropDataList(LuaManager *plua);//道具基础数据
    void SetEquipDataList(LuaManager *plua);//装备基础数据
    void SetPropertyList(LuaManager *plua);//人物属性基础数据
    void SetCopyList(LuaManager *plua);//副本数据

    //    void ParseJSON(const JSONNode &n,Enemy_Data *data);
    //    void ParseJSON_Hero(const JSONNode &n,Game_Hero_Data *data);
    void ParseJSON(cs::CSJsonDictionary *dict,Enemy_Data *data);
    void ParseJSON_Hero(cs::CSJsonDictionary *dict,Game_Hero_Data *data);
    void ParseJSON_Outfit(cs::CSJsonDictionary *dict, COutfitData *data);

    //FIGHT_ROLEDATA_LIST *m_pRoleDataList;//角色基础数据
    FIGHT_SKILLDATA_LIST *m_pSkillDataList;//技能基础数据
    FIGHT_BUFFDATA_LIST *m_pBuffDataList;//Buff基础数据
    //    FIGHT_ANI_LIST *m_pAniList;//动画特效数据
    //    NS_PIC_LIST *m_pNSList;//女神数据
    LOGIN_NMAE_LIST *m_pName1List;//姓字库
    LOGIN_NMAE_LIST *m_pName2List;//名字库
    //    PROP_LIST *m_pPropList;//道具列表
    //    EQUIP_LIST *m_pEquipList;//装备列表
    //    PROPERTY_LIST *m_pPropertyList;//装备列表
    //    COPY_LIST *m_pCopyList;//副本列表

    LuaManager *m_pTxtlua;//界面文字
    LuaScript *m_pScript;
};

#endif /* defined(__SingleEye__CfgDataManage__) */
