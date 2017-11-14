//
//  GameResouce.h
//  coslua
//
//  Created by yuanlu on 13-5-7.
//
//

#ifndef coslua_GameResouce_h
#define coslua_GameResouce_h

#define GMRES_IMAGE_FILELEN   (100)

typedef struct _GameRes_SpriteFrame_
{
    char mImgeName[GMRES_IMAGE_FILELEN];//图片名称
    int mIndexmin;//最小序号
    int mIndexmax;//最大序号
    bool bDirleft;//动作方向是否向左，用于程序镜像
}GMRES_SF;//帧数据

/*-----------resouse image------------------*/
static const char g_pFlashListNShd[] = "Image/女神互动.plist";

/*-----------texturepacker------------------*/
static const char g_sPlistFIghtMap[] = "Image/Fight_Map.plist";
static const char g_sImageFightMap[] = "Image/Fight_Map.pvr.ccz";
static const char g_sPlistFIghtPic[] = "Image/Fight_Pic.plist";
static const char g_sImageFightPic[] = "Image/Fight_Pic.pvr.ccz";
static const char g_sPlistFIghtAct[] = "Image/Fight_Act.plist";
static const char g_sImageFightAct[] = "Image/Fight_Act.pvr.ccz";
static const char g_sPlistFIghtSzy1[] = "Image/Fight_Szy1.plist";
static const char g_sImageFightSzy1[] = "Image/Fight_Szy1.pvr.ccz";
static const char g_sPlistFIghtSzy2[] = "Image/Fight_Szy2.plist";
static const char g_sImageFightSzy2[] = "Image/Fight_Szy2.pvr.ccz";
static const char g_sPlistFIghtSzy3[] = "Image/Fight_Szy3.plist";
static const char g_sImageFightSzy3[] = "Image/Fight_Szy3.pvr.ccz";
static const char g_sPlistFIghtSzy21[] = "Image/Fight_Szytype21.plist";
static const char g_sImageFightSzy21[] = "Image/Fight_Szytype21.pvr.ccz";
static const char g_sPlistFIghtSzy22[] = "Image/Fight_Szytype22.plist";
static const char g_sImageFightSzy22[] = "Image/Fight_Szytype22.pvr.ccz";
static const char g_sPlistFIghtSzy31[] = "Image/Fight_Szy31.plist";
static const char g_sImageFightSzy31[] = "Image/Fight_Szy31.pvr.ccz";
static const char g_sPlistFIghtSzy32[] = "Image/Fight_Szy32.plist";
static const char g_sImageFightSzy32[] = "Image/Fight_Szy32.pvr.ccz";
static const char g_sPlistFIghtSkill[] = "Image/Fight_Skill.plist";
static const char g_sImageFightSkill[] = "Image/Fight_Skill.pvr.ccz";
static const char g_sPlistFIghtSkill2[] = "Image/Fight_Skill2.plist";
static const char g_sImageFightSkill2[] = "Image/Fight_Skill2.pvr.ccz";
static const char g_sPlistFIghtSyJh1[] = "Image/Fight_Szjh1.plist";
static const char g_sImageFightSyJh1[] = "Image/Fight_Szjh1.pvr.ccz";

static const char g_sPlistTxt[] = "Image/Txt.plist";
static const char g_sImageTxt[] = "Image/Txt.pvr.ccz";
static const char g_sPlistPublic[] = "Image/Public.plist";
static const char g_sImagePublic[] = "Image/Public.pvr.ccz";
static const char g_sPlistPublic2[] = "Image/Public2.plist";
static const char g_sImagePublic2[] = "Image/Public2.pvr.ccz";
static const char g_sPlistLogo[] = "Image/Logo.plist";
static const char g_sImageLogo[] = "Image/Logo.pvr.ccz";
static const char g_sPlistProp[] = "Image/Prop.plist";
static const char g_sImageProp[] = "Image/Prop.pvr.ccz";
static const char g_sPlistEquip[] = "Image/Equip.plist";
static const char g_sImageEquip[] = "Image/Equip.pvr.ccz";
static const char g_sPlistPerson[] = "Image/Person.plist";
static const char g_sImagePerson[] = "Image/Person.pvr.ccz";

static const char g_sPlistbutterfly[] = "Image/butterfly.plist";
static const char g_sImagebutterfly[] = "Image/butterfly.pvr.ccz";
static const char g_sPlistRole[] = "Image/role.plist";
static const char g_sImageRole[] = "Image/role.pvr.ccz";
static const char g_sPlistSkill[] = "Image/skill.plist";
static const char g_sImageSkill[] = "Image/skill.pvr.ccz";
static const char g_sPlistProperty[] = "Image/property.plist";
static const char g_sImageProperty[] = "Image/property.pvr.ccz";
static const char g_sPlistHero[] = "Image/Hero.plist";
static const char g_sImageHero[] = "Image/Hero.pvr.ccz";
static const char g_sPlistCopyscene[] = "Image/copyscene.plist";
static const char g_sImageCopysceneo[] = "Image/copyscene.pvr.ccz";


static const char g_sPlistChouquEffect[] = "Image/ChouquEffect.plist";
static const char g_sImageChouquEffect[] = "Image/ChouquEffect.png";
static const char g_sPlistMain[] = "Image/Main.plist";
static const char g_sImageMain[] = "Image/Main.pvr.ccz";
static const char g_sPlistEquipment[] = "Image/Equipment.plist";
static const char g_sImageEquipment[] = "Image/Equipment.pvr.ccz";
/*-----------Public.plist------------------*/
static const GMRES_SF g_tTpNumY = {"Public_.png",0,9,false};
static const GMRES_SF g_tTpNumYS = {"Public_s-.png",0,9,false};
static const GMRES_SF g_tTpNumYB = {"Public_b_.png",0,9,false};
static const GMRES_SF g_tTpBox = {"Public_选择卡牌框.png",1,2,false};
static const GMRES_SF g_tTpEqupUp = {"Public_equp_.png",0,8,false};
static const GMRES_SF g_tTpNumFlash = {"Public_numflash_.png",1,11,false};
static const GMRES_SF g_tTpSelBox = {"Public_selbox_.png",0,4,false};
static const GMRES_SF g_tTpSelRound = {"Public_selround_.png",0,5,false};
static const GMRES_SF g_tTpExpFire = {"Public_expfire_.png",0,5,false};
static const GMRES_SF g_tTpJianFlash = {"Public_jianflash_.png",0,7,false};
static const char g_tTpButClose1[] = "Public_关闭-1.png";
static const char g_tTpButClose2[] = "Public_关闭-2.png";
static const char g_sImageTPDzW[] = "Public_白bg.png";
static const char g_sImageTPDzG[] = "Public_绿bg.png";
static const char g_sImageTPDzB[] = "Public_蓝bg.png";
static const char g_sImageTPDzZ[] = "Public_紫bg.png";
static const char g_sImageTPDzGd[] = "Public_金bg.png";
static const char g_sImageTPDzGD[] = "Public_橙bg.png";
static const char g_sImageTPDzR[] = "Public_红bg.png";
static const char g_sImageTPRutStarRay[] = "Public_灰色星星.png";
static const char g_sImageTPRutGloden[] = "Public_钱.png";
static const char g_sImageTPUI[] = "Public_框.png";
static const char g_sImageTPRutStarBg[] = "Public_星星底图.png";
static const char g_sImageTPRutStarLight[] = "Public_星星上层.png";
static const char g_sImageTPZY[] = "Public_职业标示.png";
static const char g_sImageTPReultBox[] = "Public_物品栏.png";
static const char g_sImageTPBZBg[] = "Public_界面背景.png";
static const char g_sImageTPBZStdFlash[] = "Public_站位开启特效.png";
static const char g_sImageTpLodBarBg[] = "Public_loing-槽.png";
static const char g_sImageTpLodFill[] = "Public_loing-条.png";
static const char g_sImageTpLodArrow[] = "Public_loing-头.png";
static const char g_sImageTpBTRedBig[] = "Public_button31.png";
static const char g_sImageTpHW[] = "Public_花纹.png";
static const char g_sImageTpHW21[] = "Public_花纹21.png";
static const char g_sImageTpHW201[] = "Public_花纹201.png";
static const char g_sImageTpZoneOut[] = "Public_六芒星.png";
static const char g_sImageTpZoneIn[] = "Public_天芒星2.png";
static const char g_sImageTpInBOx[] = "Public_内框.png";
static const char g_sImageTpLableBox[] = "Public_图标框.png";
static const char g_sImageTpCrlSeled[] = "Public_选中旋转.png";
static const char g_sImageTpMapLock[] = "Public_锁.png";
static const char g_sImageTpButonAn[] = "Public_an.png";
static const char g_sImageTpButonWeiAn[] = "Public_weian.png";
static const char g_sImageTpBarVBg[] = "Public_滚动条-1.png";
static const char g_sImageTpBarVFillU[] = "Public_滚动条-2-1.png";
static const char g_sImageTpBarVFillD[] = "Public_滚动条-2-2.png";
static const char g_sImageTpBarVFillM[] = "Public_滚动条-2-3.png";
static const char g_sImageTpSeltedFlag[] = "Public_选中显示3.png";
static const char g_sImageTpButleve3[] = "Public_三级按钮-1.png";
static const char g_sImageTpButleve3F[] = "Public_三级按钮-2.png";
static const char g_sImageTpButleveS3[] = "Public_特殊三级按钮-1.png";
static const char g_sImageTpButleveS3F[] = "Public_特殊三级按钮-2.png";
static const char g_sImageTpInfoBox1[] = "Public_信息框-1.png";
static const char g_sImageTpInfoBox2[] = "Public_信息框2.png";
static const char g_sImageTpInfoBox3[] = "Public_信息框3.png";
static const char g_sImageTpInfoBox4[] = "Public_信息框4.png";
static const char g_sImageTpInfoBox5[] = "Public_信息框5.png";
static const char g_sImageTpInfoBox6[] = "Public_信息框6.png";
static const char g_sImageTpInfoBox7[] = "Public_信息框7.png";
static const char g_sImageTpInfoBox8[] = "Public_信息框8.png";
static const char g_sImageTpInfoBox9[] = "Public_信息框-9.png";
static const char g_sImageTpInfoBox10[] = "Public_信息框10.png";
static const char g_sImageTpInfoBox13[] = "Public_信息框13.png";
static const char g_sImageTpButLeve4[] = "Public_4级按钮-1.png";
static const char g_sImageTpButLeve4F[] = "Public_4级按钮-2.png";
static const char g_sImageTpButCloseL2[] = "Public_二级关闭-1.png";
static const char g_sImageTpButCloseL2F[] = "Public_二级关闭-2.png";
static const char g_sImageTpAward[] = "Public_奖品.png";
static const char g_sImageTpJCDS[] = "Public_竞技对手.png";
static const char g_sImageTpJCDS2[] = "Public_竞技对手-2.png";
static const char g_sImageTpNextPage[] = "Public_下一页-2.png";
static const char g_sImageTpNextPageF[] = "Public_下一页.png";
static const char g_sImageTpDSSearch[] = "Public_对手搜索.png";
static const char g_sImageTpDSSearch2[] = "Public_对手搜索-2.png";
static const char g_sImageTpNSHeadBox[] = "Public_女神头像边框.png";
static const char g_sImageTpProgBarBg[] = "Public_培养槽.png";
static const char g_sImageTpProgBarFill[] = "Public_培养值.png";
static const char g_sImageTPExpBar1[] = "Public_经验槽增长特效_1.png";
static const char g_sImageTPExpBar2[] = "Public_经验槽增长特效_2.png";
static const char g_sImageTPFtPowerBg[] = "Public_战斗值显示.png";
static const char g_sImageTPFtBloodBg[] = "Public_ui3.png";
static const char g_sImageTPFtChargeDB[] = "Public_充值型活动-单笔累积充值.png";
static const char g_sImageTPFtChargeLH[] = "Public_充值型活动-连环充值.png";
static const char g_sImageTPFtChargeJC[] = "Public_达标型活动-精彩活动.png";
static const char g_sImageTPFtHDDL[] = "Public_登录型活动.png";
static const char g_sImageTPFtHDDH[] = "Public_兑换型活动.png";
static const char g_sImageTPFtHDPM[] = "Public_排名型活动.png";
static const char g_sImageTPFtHDTG[] = "Public_团购活动.png";
static const char g_sImageTPFtXFHDZK[] = "Public_消费型活动-折扣消费.png";
static const char g_sImageTPFtXFHDDB[] = "Public_消费性活动-单笔累积消费.png";
static const char g_sImageTPFtXFHDHT[] = "Public_消费性活动-环套消费.png";
static const char g_sImageTPBarBg22[] = "Public_进度槽22.png";
static const char g_sImageTPBarFill22[] = "Public_进度值22.png";
static const char g_sImageTPBox9[] = "Public_这个框请你拉吧拉吧.png";
static const char g_sImageTPBox9Tran[] = "Public_这个框请你拉吧拉吧1.png";
static const char g_sImageTPBox9Bian[] = "Public_这个框请你拉吧拉吧花边.png";
static const char g_sImageTPRoleUI3[] = "Public_人物ui3.png";
static const char g_sImageTPRSceenL[] = "Public_石门左.png";
static const char g_sImageTPRSceenR[] = "Public_石门右.png";

/*-----------Public2.plist------------------*/
static const char g_sImageTPBZSixZoneBg[] = "Public2_布阵底图.png";
static const char g_sImageTpInfoBox11[] = "Public2_信息框11.png";
static const char g_sImageTpInfoBox12[] = "Public2_信息框12.png";
static const char g_sImageTpInfoBox14[] = "Public2_信息框14.png";
static const char g_sImageTpInfoBox15[] = "Public2_信息框15.png";
static const char g_sImageTpHLButF[] = "Public2_按钮1-1.png";
static const char g_sImageTpHLBut[] = "Public2_按钮1-2.png";
static const char g_sImageTpHLBg[] = "Public2_按钮2-2.png";
static const char g_sImageTpHLBgF[] = "Public2_按钮2-1.png";
static const char g_sImageTpHLBackGd[] = "Public2_大底板.png";
static const char g_sImageTpHLJZ[] = "Public2_卷轴.png";
static const char g_sImageTpHLBox3[] = "Public2_框3.png";
static const char g_sImageTpHLMH[] = "Public2_冒号.png";
static const char g_sImageTpHLWPBox[] = "Public2_物品框1.png";
static const char g_sImageTpHLWPBoxF[] = "Public2_物品框2.png";
static const char g_sImageTpHLInfoBg[] = "Public2_信息底板.png";
static const char g_sImageTpHLInfoBg1[] = "Public2_信息底板1.png";
static const char g_sImageButton[] = "Public2_按钮3-2.png";
static const char g_sImageButtonF[] = "Public2_按钮3-1.png";
static const char g_sImageClose[] = "Public2_关闭-1.png";
static const char g_sImageCloseF[] = "Public2_关闭-2.png";
static const char g_sImageJindutiao[] = "Public2_进度条.png";
static const char g_sImageJindutiaoF[] = "Public2_进度条框.png";
static const char g_sImageJindutiaoP[] = "Public2_进度条指标.png";
static const char g_sImageTpHLBox5[] = "Public2_框5.png";
static const char g_sImageTpHLInput[] = "Public2_输入框.png";
static const char g_sImageTpVipKey[] = "Public2_vip按键.png";
static const char g_sImageTpVipBg[] = "Public2_底.png";
static const char g_sImageTpSister[] = "Public2_妹子.png";
static const char g_sImageTpBJFlag[] = "Public2_白金.png";
static const char g_sImageTpBJHY2[] = "Public2_白金会员2.png";
static const char g_sImageTpBJHY3[] = "Public2_白金会员3.png";
static const char g_sImageTpBYFlag[] = "Public2_白银符号.png";
static const char g_sImageTpBYHY2[] = "Public2_白银会员2.png";
static const char g_sImageTpBYHY3[] = "Public2_白银会员3.png";
static const char g_sImageTpHJFlag[] = "Public2_黄金.png";
static const char g_sImageTpHJHY2[] = "Public2_黄金会员2.png";
static const char g_sImageTpHJHY3[] = "Public2_黄金会员3.png";
static const char g_sImageTpQTFlag[] = "Public2_青铜符号.png";
static const char g_sImageTpQTHY2[] = "Public2_青铜会员2.png";
static const char g_sImageTpQTHY3[] = "Public2_青铜会员3.png";
static const char g_sImageTpZSFlag[] = "Public2_钻石.png";
static const char g_sImageTpZSHY2[] = "Public2_钻石会员1.png";
static const char g_sImageTpZSHY3[] = "Public2_钻石会员3.png";
static const char g_sImageTpNoHy[] = "Public2_非VIP.png";
static const char g_sImageTpBox12[] = "Public2_框12.png";
static const char g_sImageTpIns1[] = "Public2_说明1.png";
static const char g_sImageTpIns2[] = "Public2_说明2.png";
static const char g_sImageTpNPage3[] = "Public2_下一页3.png";
static const char g_sImageTpBg001[] = "Public2_001.png";
static const char g_sImageTpBut41[] = "Public2_按钮4-1.png";
static const char g_sImageTpBut42[] = "Public2_按钮4-2.png";
static const char g_sImageTpBut51[] = "Public2_按钮5-1.png";
static const char g_sImageTpBut52[] = "Public2_按钮5-2.png";
static const char g_sImageTpHDBoard[] = "Public2_活动进入地板.png";
static const char g_sImageTpHDBox1[] = "Public2_活动框1.png";
static const char g_sImageTpHDBox2[] = "Public2_活动框2.png";
static const char g_sImageTpHDBox3[] = "Public2_活动框3.png";
static const char g_sImageTpTaskTip[] = "Public2_任务提示旗帜.png";
static const char g_sImageTpMailUnRd[] = "Public2_未读邮件.png";
static const char g_sImageTpMailRd[] = "Public2_已读邮件.png";
static const char g_sImageTpMailFH[] = "Public2_邮件分域横.png";
static const char g_sImageTpMailFV[] = "Public2_邮件分域竖.png";
static const char g_sImageTpMailBox[] = "Public2_邮件框.png";
static const char g_sImageTpMailSetd[] = "Public2_邮件选中.png";
static const char g_sImageTpMailSetBd[] = "Public2_邮件选中底板.png";
static const char g_sImageTpMailFj[] = "Public2_有附件.png";
static const char g_sImageTpWear[] = "Public2_穿戴座中.png";
static const char g_sImageTpFG1[] = "Public2_分割线1.png";
static const char g_sImageTpFG2[] = "Public2_分割线2.png";
static const char g_sImageTpRoleSX[] = "Public2_人物属性.png";
static const char g_sImageEqLevel1[] = "Public2_一阶白星.png";
static const char g_sImageEqLevel2[] = "Public2_二阶铜星.png";
static const char g_sImageEqLevel3[] = "Public2_三阶银星.png";
static const char g_sImageEqLevel4[] = "Public2_四阶金星.png";
static const char g_sImageEqLevel5[] = "Public2_五阶绿宝星.png";
static const char g_sImageEqLevel6[] = "Public2_六阶蓝宝星.png";
static const char g_sImageEqLevel7[] = "Public2_七阶紫宝星.png";
static const char g_sImageEqLevel8[] = "Public2_八阶黄宝星.png";
static const char g_sImageEqLevel9[] = "Public2_九阶红宝星.png";
static const char g_sImageEqLevel10[] = "Public2_十阶黑龙星.png";
static const char g_sImageSelectlist[] = "Public2_选择列表.png";

/*-----------Fight_Map.plist------------------*/
static const GMRES_SF g_tTpPhySRCCAtk = {"Fight_Map_通用刺击近程物理攻击-.png",1,6,false};
static const char g_sImageTPMap[] = "Fight_Map_战斗背景.png";
static const char g_sImageDlgBg[] = "Fight_Map_dlgbg.png";
static const char g_sImageMap[] = "Fight_Map_战场站位.png";
static const char g_sImageFuWen[] = "Fight_Map_符文.png";

/*-----------Fight_Pic.plist------------------*/
static const GMRES_SF g_tTpAprD = {"Fight_Pic_cd.png",1,1,false};
static const GMRES_SF g_tTpAprE = {"Fight_Pic_ce.png",1,12,false};
static const GMRES_SF g_tTpLGLSSF = {"Fight_Pic_LGLS_SF_.png",1,12,true};
static const char g_sImageTPReultBg[] = "Fight_Pic_win-bg.png";
static const char g_sImageTPDlgBg[] = "Fight_Pic_对话背景.png";
static const char g_sImageTPDlgBgName[] = "Fight_Pic_对话姓名ui.png";
static const char g_sImageTPSNBg[] = "Fight_Pic_技能背景.png";
static const char g_sImageTPDead[] = "Fight_Pic_墓碑副本.png";
static const char g_sImageTPRutMap[] = "Fight_Pic_内圈.png";
static const char g_sImageTPRutMapBg[] = "Fight_Pic_通关外.png";
static const char g_sImageTPLight[] = "Fight_Pic_光照.png";
static const char g_sImageTPJLBg[] = "Fight_Pic_技能地板.png";
static const char g_sImageTPTitleBg[] = "Fight_Pic_ui1.png";
static const char g_sImageTPTitleNq[] = "Fight_Pic_ui3.png";
static const char g_sImageTPTitleTBg[] = "Fight_Pic_计时器.png";
static const char g_sImageTPBotPowBg[] = "Fight_Pic_卡牌槽-1.png";
static const char g_sImageTPBotPowFill[] = "Fight_Pic_卡牌槽-2.png";
static const char g_sImageTPBotNL[] = "Fight_Pic_卡牌能量值-1.png";
static const char g_sImageTPBotNLBar[] = "Fight_Pic_卡牌能量值.png";
static const char g_sImageTPBotBdBar[] = "Fight_Pic_卡牌血值.png";

/*-----------Fight_Act.plist------------------*/
static const GMRES_SF g_tTpPhySRPKAtk = {"Fight_Act_通用劈砍近程物理攻击-.png",1,5,true};
static const GMRES_SF g_tTpPhyRENorAtk = {"Fight_Act_SFX_PhyRemoteAtk_Cast_R_.png",1,5,false};
static const GMRES_SF g_tTpPhyRENorEft = {"Fight_Act_SFX_PhyRemoteAtk_Effect_.png",1,6,false};
static const GMRES_SF g_tTpSklDaoAtk = {"Fight_Act_SFX_BladeAtk_L_.png",1,5,true};
static const GMRES_SF g_tTpSklMpAtk = {"Fight_Act_SFX_MPSupply_Cast_.png",1,9,false};
static const GMRES_SF g_tTpSklMpEft = {"Fight_Act_SFX_MPSupply_Effect_.png",1,12,false};
static const GMRES_SF g_tTpBossAtk = {"Fight_Act_SFX_BossSkill_L_.png",1,7,true};
static const GMRES_SF g_tTpStunState = {"Fight_Act_SFX_Stun_.png",1,8,false};
static const GMRES_SF g_tTpSklIce1Efe = {"Fight_Act_by.png",1,7,false};
static const GMRES_SF g_tTpSklIce2Efe = {"Fight_Act_bb.png",1,7,false};
static const GMRES_SF g_tTpSkillHL = {"Fight_Act_hl.png",1,8,false};
static const GMRES_SF g_tTpYGF = {"Fight_Act_ygf.png",1,4,true};
static const GMRES_SF g_tTpYGE = {"Fight_Act_yge.png",1,7,false};
static const GMRES_SF g_tTpJCW2 = {"Fight_Act_sz.png",1,6,true};
static const GMRES_SF g_tTpHpSF = {"Fight_Act_HpSF_.png",1,7,false};
static const GMRES_SF g_tTpHpSJ = {"Fight_Act_HpSJ_.png",1,9,false};
static const GMRES_SF g_tTpGMJCMagSF = {"Fight_Act_GMJC_MAGIC_SF_.png",1,7,false};
static const GMRES_SF g_tTpGMJCMagSJ = {"Fight_Act_GMJC_MAGIC_SJ_.png",1,6,false};
static const GMRES_SF g_tTpZRMagSJ = {"Fight_Act_ZR_MAGIC_SF_.png",1,9,false};

/*-----------Fight_Szy1.plist------------------*/
static const GMRES_SF g_tTpSzyLight = {"Fight_Szy1_g.png",1,8,false};
static const GMRES_SF g_tTpSzyTile = {"Fight_Szy1_z.png",1,8,false};
static const GMRES_SF g_tTpSzyFire1 = {"Fight_Szy1_f.png",1,5,false};

/*-----------Fight_Szy2.plist------------------*/
static const GMRES_SF g_tTpSzySj = {"Fight_Szy2_sj.png",1,8,false};
static const GMRES_SF g_tTpSzyT1 = {"Fight_Szy2_t.png",1,7,false};

/*-----------Fight_Szytype21.plist------------------*/
static const GMRES_SF g_tTpSzyType2NS1 = {"Fight_Szytype21_anlei.png",1,9,true};

/*-----------Fight_Szytype22.plist------------------*/
static const GMRES_SF g_tTpSzyType2NS2 = {"Fight_Szytype21_anlei.png",10,12,true};
static const GMRES_SF g_tTpSzyType2Efe = {"Fight_Szytype22_anleiefe.png",1,8,false};
static const GMRES_SF g_tTpSzyType2Txt = {"Fight_Szytype22_anleitxt.png",1,8,false};

/*-----------Fight_Szy31.plist------------------*/
static const GMRES_SF g_tTpSzy31NS1 = {"Fight_Szy31_r.png",1,7,false};
static const GMRES_SF g_tTpSzy31NS2 = {"Fight_Szy31_ra.png",1,9,false};

/*-----------Fight_Szy32.plist------------------*/
static const GMRES_SF g_tTpSzy32Map = {"Fight_Szy32_mp.png",1,8,false};
static const GMRES_SF g_tTpSzy32Efe = {"Fight_Szy32_efe.png",1,8,false};
static const GMRES_SF g_tTpSzy32Txt = {"Fight_Szytype22_anleitxt.png",1,8,false};

/*-----------Fight_Skill.plist------------------*/
static const GMRES_SF g_tTpSkillRun = {"Fight_Skill_run.png",1,7,true};
static const GMRES_SF g_tTpSkillRunF = {"Fight_Skill_runf.png",1,12,true};

/*-----------Fight_Skill2.plist------------------*/
static const GMRES_SF g_tTpSkillBQT = {"Fight_Skill2_bqt.png",1,10,false};
static const GMRES_SF g_tTpSkillBQTF = {"Fight_Skill2_bqtf.png",1,4,false};

/*-----------Fight_Szjh1.plist------------------*/
static const GMRES_SF g_tTpSzyJH1 = {"Fight_Szjh1_.png",1,10,false};

/*-----------Prop.plist------------------*/
static const char g_sImageTPShenGeSP[] = "Prop_神格碎片.png";
static const char g_sImageTPLuckSJ[] = "Prop_灵魂水晶.png";

/*-----------Txt.plist------------------*/
static const GMRES_SF g_sTPArwUp = {"Txt_augreen.png",1,5,false};
static const GMRES_SF g_sTPArwDown = {"Txt_adred.png",1,5,false};
static const GMRES_SF g_sTPBjRed = {"Txt_baored.png",1,6,false};
static const GMRES_SF g_sTPBjGreen = {"Txt_baogreen.png",1,6,false};
static const GMRES_SF g_sTPDangRed = {"Txt_dangred.png",1,6,false};
static const GMRES_SF g_sTPDangGreen = {"Txt_danggreen.png",1,6,false};
static const GMRES_SF g_sTPDefRed = {"Txt_fangred.png",1,6,false};
static const GMRES_SF g_sTPDefGreen = {"Txt_fanggreen.png",1,6,false};
static const GMRES_SF g_sTPActRed = {"Txt_gongred.png",1,6,false};
static const GMRES_SF g_sTPActGreen = {"Txt_gonggreen.png",1,6,false};
static const GMRES_SF g_sTPKangRed = {"Txt_kangred.png",1,6,false};
static const GMRES_SF g_sTPKangGreen = {"Txt_konggreen.png",1,6,false};
static const GMRES_SF g_sTPMingRed = {"Txt_mingred.png",1,6,false};
static const GMRES_SF g_sTPMingGreen = {"Txt_minggreen.png",1,6,false};
static const GMRES_SF g_sTPPoRed = {"Txt_pored.png",1,6,false};
static const GMRES_SF g_sTPPoGreen = {"Txt_pogreen.png",1,6,false};
static const GMRES_SF g_sTPMissRed = {"Txt_sanred.png",1,6,false};
static const GMRES_SF g_sTPMissGreen = {"Txt_sangreen.png",1,6,false};
static const GMRES_SF g_sTPGdFlash = {"Txt_gd.png",1,10,false};
static const GMRES_SF g_sTPGdTip = {"Txt_gdtxt.png",1,10,false};
static const GMRES_SF g_tTpCHNNum = {"Txt_chn.png",1,7,false};
static const GMRES_SF g_tTpJump = {"Txt_跳过-.png",1,2,false};
static const char g_sImageTPFJ[] = "Txt_反击.png";
static const char g_sImageTPSB[] = "Txt_闪避.png";
static const char g_sImageTPFAIL[] = "Txt_失败.png";
static const char g_sImageTPStart[] = "Txt_开始.png";
static const char g_sImageTPRutTxtPj[] = "Txt_通关评价.png";
static const char g_sImageTPRutTxtTile5[] = "Txt_完美通关.png";
static const char g_sImageTPRutTxtTile4[] = "Txt_华丽通关.png";
static const char g_sImageTPRutTxtTile3[] = "Txt_优秀通关.png";
static const char g_sImageTPRutTxtTile2[] = "Txt_熟练通关.png";
static const char g_sImageTPRutTxtTile1[] = "Txt_普通通关.png";
static const char g_sImageTPBuZhen[] = "Txt_布阵.png";
static const char g_sImageTPOPen4[] = "Txt_四级开启.png";
static const char g_sImageTPOPen12[] = "Txt_12级开启.png";
static const char g_sImageTPOPen17[] = "Txt_17级开启.png";
static const char g_sImageTPOPen22[] = "Txt_22级开启.png";
static const char g_sImageTPOPen27[] = "Txt_27级开启.png";
static const char g_sImageTPZKXQ[] = "Txt_查看详情.png";
static const char g_sImageTPCK[] = "Txt_查看.png";
static const char g_sImageTPNo[] = "Txt_否.png";
static const char g_sImageTPYes[] = "Txt_是.png";
static const char g_sImageTPJJC[] = "Txt_竞技场.png";
static const char g_sImageTPLQJL[] = "Txt_领取奖励.png";
static const char g_sImageTPDayTast[] = "Txt_每日任务.png";
static const char g_sImageTPDayConCust[] = "Txt_内容说明.png";
static const char g_sImageTPOK[] = "Txt_确定.png";
static const char g_sImageTPCancel[] = "Txt_取消.png";
static const char g_sImageTPTimerClr[] = "Txt_计时清零.png";
static const char g_sImageTPBZ[] = "Txt_霸占-1.png";
static const char g_sImageTPProtect[] = "Txt_保护.png";
static const char g_sImageTPCR[] = "Txt_仇人.png";
static const char g_sImageTPBuy2[] = "Txt_购买2.png";
static const char g_sImageTPBuyTime[] = "Txt_购买次数.png";
static const char g_sImageTPChangePi[] = "Txt_换一批.png";
static const char g_sImageTPJieJiu[] = "Txt_解救.png";
static const char g_sImageTPNYNS[] = "Txt_奴役女神.png";
static const char g_sImageTPNS[] = "Txt_女神.png";
static const char g_sImageTPQD[] = "Txt_抢夺.png";
static const char g_sImageTPQDNS[] = "Txt_抢夺女神.png";
static const char g_sImageTPQDLOST[] = "Txt_剩余抢夺次数.png";
static const char g_sImageTPSearch[] = "Txt_搜索.png";
static const char g_sImageTPProduce[] = "Txt_推荐.png";
static const char g_sImageTPMyGod[] = "Txt_我的女神.png";
static const char g_sImageTPGod2[] = "Txt_女神-2.png";
static const char g_sImageTPCZM[] = "Txt_充值满.png";
static const char g_sImageTPHDOT[] = "Txt_活动结束时间.png";
static const char g_sImageTPJSKZ[] = "Txt_即刻馈赠.png";
static const char g_sImageTPLJLQ[] = "Txt_累计领取.png";
static const char g_sImageTPLQ[] = "Txt_领取.png";
static const char g_sImageTPLQH[] = "Txt_ylq.png";
static const char g_sImageTPFINISH[] = "Txt_未完成.png";
static const char g_sImageTPDay[] = "Txt_天.png";
static const char g_sImageTPLJCZ[] = "Txt_已累计充值.png";
static const char g_sImageChongzhi[] = "Txt_充值.png";
static const char g_sImageHuiyuan[] = "Txt_会员.png";
static const char g_sImageHuiyuanLibao[] = "Txt_会员礼包.png";
static const char g_sImageRank[] = "Txt_排行榜.png";
static const char g_sImageGetProp[] = "Txt_奖励领取.png";
static const char g_sImageRankNumber[] = "Txt_排名.png";
static const char g_sImageRankName[] = "Txt_姓名.png";
static const char g_sImageRankValue[] = "Txt_消耗量.png";
static const char g_sImageGroup[] = "Txt_团购.png";
static const char g_sImageGrouplist[] = "Txt_奖励查看.png";
static const char g_sImageTPBuy[] = "Txt_购买.png";
static const char g_sImageBjHy[] = "Txt_白金会员.png";
static const char g_sImageByHy[] = "Txt_白银会员.png";
static const char g_sImageChengWei[] = "Txt_成为.png";
static const char g_sImageCharg1[] = "Txt_充值1.png";
static const char g_sImageHjHy[] = "Txt_黄金会员.png";
static const char g_sImageHy2[] = "Txt_会员2.png";
static const char g_sImageNXZS[] = "Txt_您现在是.png";
static const char g_sImageQtHy[] = "Txt_青铜会员.png";
static const char g_sImageZsHy[] = "Txt_钻石会员.png";
static const char g_sImagePacket[] = "Txt_背包.png";
static const char g_sImageUse[] = "Txt_使用.png";
static const char g_sImageBuy22[] = "Txt_购买22.png";
static const char g_sImageRX2[] = "Txt_热销2.png";
static const char g_sImageNS2[] = "Txt_女神2.png";
static const char g_sImageTool2[] = "Txt_道具2.png";
static const char g_sImageBuyLimt2[] = "Txt_限购2.png";
static const char g_sImageMJQD[] = "Txt_魔晶签到.png";
static const char g_sImageReturn[] = "Txt_返回.png";
static const char g_sImageGDQD[] = "Txt_金币签到.png";
static const char g_sImageQDJL[] = "Txt_签到奖励.png";
static const char g_sImageSysMail[] = "Txt_系统邮件.png";
static const char g_sImageMail[] = "Txt_邮件.png";
static const char g_sImageBQJJ[] = "Txt_本期奖券.png";
static const char g_sImageBQGDJJ[] = "Txt_本期已有金币奖券.png";
static const char g_sImageBQMJJJ[] = "Txt_本期已有魔晶奖券.png";
static const char g_sImageSend[] = "Txt_发送.png";
static const char g_sImageSMail[] = "Txt_发邮件.png";
static const char g_sImageKJNum[] = "Txt_开奖号码.png";
static const char g_sImageDel[] = "Txt_删除.png";
static const char g_sImagePMail[] = "Txt_玩家邮件.png";
static const char g_sImageMyJJ[] = "Txt_我的奖券.png";
static const char g_sImageLuckJL[] = "Txt_幸运大抽奖奖励.png";
static const char g_sImageLastJJ[] = "Txt_上期奖券.png";
static const char g_sImageHD[] = "Txt_活动.png";
static const char g_sImageTH[] = "Txt_特惠.png";
static const char g_sImageNewPrdct[] = "Txt_新品.png";
static const char g_sImageShare[] = "Txt_分享.png";
static const char g_sImageShouji[] = "Txt_收集.png";
static const char g_sImageShuju[] = "Txt_数据.png";
static const char g_sImageTongji[] = "Txt_统计.png";
static const char g_sImageXiaofei[] = "Txt_消费.png";
static const char g_sImageXingwei[] = "Txt_行为.png";
static const char g_sImageFightValue[] = "Txt_战斗力.png";
static const char g_sImageTPEquip1[] = "Txt_装备1.png";
static const char g_sImageTPZhen1[] = "Txt_星阵1.png";
static const char g_sImageTPShop1[] = "Txt_商城1.png";
static const char g_sImageTPRole1[] = "Txt_人物1.png";
static const char g_sImageTPSet1[] = "Txt_设置1.png";
static const char g_sImageTPFight1[] = "Txt_竞技1.png";
static const char g_sImageTPCopy1[] = "Txt_副本1.png";
static const char g_sImageTPBag1[] = "Txt_背包1.png";
static const char g_sImageTPHeroPub[] = "Txt_英雄之门.png";
static const char g_sImageTPNvshenPub[] = "Txt_女神祭坛.png";
static const char g_sImageTPFuwenPub[] = "Txt_符文之语.png";
static const char g_sImageTPZhan[] = "Txt_战.png";
static const char g_sImageTPGongFen[] = "Txt_供奉.png";
static const char g_sImageTPQiTao2[] = "Txt_祈祷2.png";
static const char g_sImageHelp[] = "Txt_求救.png";
static const char g_sImageMail1[] = "Txt_邮箱1.png";
static const char g_sImageHdFb[] = "Txt_活动副本.png";
static const char g_sImageJfPh[] = "Txt_积分排行.png";
static const char g_sImageJsCc[] = "Txt_角色抽取.png";
static const char g_sImageJyFb[] = "Txt_精英副本.png";
static const char g_sImageKbBuy[] = "Txt_卡包购买.png";
static const char g_sImagePySj[] = "Txt_培养升阶.png";
static const char g_sImagePtFb[] = "Txt_普通副本.png";
static const char g_sImageSpFh[] = "Txt_碎片互换.png";
static const char g_sImageBaoWu[] = "Txt_宝物.png";
static const char g_sImageCuanDaiUp[] = "Txt_穿戴升级.png";
static const char g_sImageDjLq1[] = "Txt_点击领取-1.png";
static const char g_sImageDjLq2[] = "Txt_点击领取-2.png";
static const char g_sImageFangJiu[] = "Txt_防具.png";
static const char g_sImageFejie[] = "Txt_分解.png";
static const char g_sImageShenJie[] = "Txt_升阶.png";
static const char g_sImageUpScUp[] = "Txt_十次升级.png";
static const char g_sImageShouShi[] = "Txt_首饰.png";
static const char g_sImageWeapon[] = "Txt_武器.png";
static const char g_sImageXieXia[] = "Txt_卸下.png";
static const char g_sImageYcUp[] = "Txt_一次升级.png";
static const char g_sImageYjFJ[] = "Txt_一键分解-.png";
static const char g_sImageZhuangBei[] = "Txt_装备.png";
static const char g_sImageZhuangBeiFj[] = "Txt_装备分解.png";
static const char g_sImageZhuangBeiGet[] = "Txt_装备获取.png";
static const char g_sImageTPRutExp[] = "Txt_exp.png";
static const char g_sImageTpCanZhanBg[] = "Txt_参战2.png";
static const char g_sImageTpJNYCBig[] = "Txt_技能养成.png";
static const char g_sImageTPReultExit[] = "Txt_离开.png";
static const char g_sImageTPExit[] = "Txt_离开点击.png";
static const char g_sImageTPExitF[] = "Txt_离开击中.png";
static const char g_sImageTpNSJLBig[] = "Txt_女神降临.png";
static const char g_sImageTpNSYCBig[] = "Txt_女神养成.png";
static const char g_sImageTpTuPo[] = "Txt_突破.png";
static const char g_sImageTpOnZoneFlag[] = "Txt_已上阵.png";
static const char g_sImageDoor1[] = "Txt_远古.png";
static const char g_sImageDoor2[] = "Txt_混沌.png";
static const char g_sImageDoor3[] = "Txt_遗迹.png";
static const char g_sImageTi[] = "Txt_体.png";
static const char g_sImageNu[] = "Txt_怒.png";
static const char g_sImageNsCZ[] = "Txt_出战.png";
static const char g_sImageNsPY[] = "Txt_培养.png";

/*-----------main.plist------------------*/
static const char g_sImageMainBackGround[] = "Main_background.png";
static const char g_sImageMainUi1[] = "Main_ui1.png";
static const char g_sImageMainUi2[] = "Main_ui2.png";
static const char g_sImageMainUi3[] = "Main_ui3.png";
static const char g_sImageMainUi4[] = "Main_ui4.png";
static const char g_sImageMainUi5[] = "Main_ui5.png";
static const char g_sImageMainBag2[] = "Main_背包2.png";
static const char g_sImageMainBag3[] = "Main_背包3.png";
static const char g_sImageMainCopy2[] = "Main_副本2.png";
static const char g_sImageMainCopy3[] = "Main_副本3.png";
static const char g_sImageMainCoin[] = "Main_金币图标.png";
static const char g_sImageMainCoinA[] = "Main_金币增加.png";
static const char g_sImageMainExp1[] = "Main_经验1.png";
static const char g_sImageMainExp2[] = "Main_经验2.png";
static const char g_sImageMainFight2[] = "Main_竞技2.png";
static const char g_sImageMainFight3[] = "Main_竞技3.png";
static const char g_sImageMainMojing[] = "Main_魔晶图标.png";
static const char g_sImageMainMojingA[] = "Main_魔晶增加.png";
static const char g_sImageMainRole2[] = "Main_人物2.png";
static const char g_sImageMainRole3[] = "Main_人物3.png";
static const char g_sImageMainShop2[] = "Main_商城2.png";
static const char g_sImageMainShop3[] = "Main_商城3.png";
static const char g_sImageMainSet2[] = "Main_设置2.png";
static const char g_sImageMainSet3[] = "Main_设置3.png";
static const char g_sImageMainZhen2[] = "Main_星阵2.png";
static const char g_sImageMainZhen3[] = "Main_星阵3.png";
static const char g_sImageMainEquip2[] = "Main_装备2.png";
static const char g_sImageMainEquip3[] = "Main_装备3.png";
static const char g_sImageMainMail2[] = "Main_邮箱2.png";
static const char g_sImageMainMail3[] = "Main_邮箱3.png";
static const char g_sImageMainFrame[] = "Main_战斗值显示.png";
static const char g_sImageMainPub1[] = "Main_pub1.png";
static const char g_sImageMainPub2[] = "Main_pub2.png";
static const char g_sImageMainPub3[] = "Main_pub3.png";
static const char g_sImageMainCloud[] = "Main_cloud.png";

/*-----------role.plist------------------*/
static const char g_sImageRole1[] = "Role_taiyang.png";
static const char g_sImageRole2[] = "Role_xing.png";
static const char g_sImageRole3[] = "Role_yueliang.png";
static const char g_sImageRole4[] = "Role_进化.png";
static const char g_sImageRole5[] = "Role_培养背景.png";
static const char g_sImageRole6[] = "Role_培养槽.png";
static const char g_sImageRole7[] = "Role_人物ui3.png";
static const char g_sImageRole8[] = "Role_人物1.png";
static const char g_sImageRole9[] = "Role_培养值.png";

/*-----------Logo.plist------------------*/
static const char g_sImageTpLoginBg[] = "Logo_bg.png";
static const char g_sImageTpEnterBut[] = "Logo_进入游戏.png";
static const char g_sImageTpRandBut[] = "Logo_随机.png";
static const char g_sImageTpNameEdt[] = "Logo_玩家姓名.png";
static const char g_sImageTpInfoBox[] = "Logo_信息框.png";
static const char g_sImageTpSelted[] = "Logo_选中特效.png";
static const char g_sImageTpNSName[] = "Logo_地神.png";
static const char g_sImageTpNSTxtBg[] = "Logo_描述底板.png";
static const char g_sImageTpNSSelect[] = "Logo_选择信仰.png";
static const char g_sImageTpLogo[] = "Logo_logo.png";
static const char g_sImageTpLogoBg[] = "Logo_loing-bg.png";

/*-----------butterfly.plist------------------*/
static const char g_sImagepbutterfly[] = "Butterfly_00000.png";
static const GMRES_SF g_tTpbutterfly1 = {"Butterfly_0000.png",0,21,true};
static const GMRES_SF g_tTpbutterfly2 = {"Butterfly_0000.png",22,36,true};

/*-----------choujiang.plist------------------*/
static const GMRES_SF g_tTpChouquEffect = {"Effect_0000.png",1,26,true};

/*-----------sounde---------------------*/
static const char g_sSoundLoad[] = "Sound/登陆界面.mp3";
static const char g_sSoundMain[] = "Sound/主界面.mp3";
static const char g_sSoundScene[] = "Sound/系统界面.mp3";
static const char g_sSoundFtNor[] = "Sound/普通and精英副本.mp3";
static const char g_sSoundFtAo[] = "Sound/活动副本—奥林匹斯山.mp3";
static const char g_sSoundFtEye[] = "Sound/活动副本—索伦之眼.mp3";
/*-----------font---------------------*/
static const char g_sFontRed[] = "Fonts/FontRed.fnt";
static const char g_sFontRedBig[] = "Fonts/FontRedBig.fnt";
static const char g_sFontGreen[] = "Fonts/FontGreen.fnt";

/*-----------script---------------------*/
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
static const char g_sScriptCfg[] = "Script/config.lua";
static const char g_sScriptTxtCfg[] = "Script/txt.lua";
#else
static const char g_sScriptTxtCfg[] = "/data/data/com.gwsoft.singleeye/files/txt.lua";
static const char g_sScriptCfg[] = "/data/data/com.gwsoft.singleeye/files/config.lua";
#endif


#endif
