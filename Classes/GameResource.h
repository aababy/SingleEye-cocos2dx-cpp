//
//  GameResource.h
//  SingleEye
//
//  Created by qinfen on 13-11-28.
//
//

#ifndef SingleEye_GameResource_h
#define SingleEye_GameResource_h

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
static const char  Level_Info_Path[] = "/data/data/com.gwsoft.singleeye/files/levelinfo.lua";
#else
static const char  Level_Info_Path[] = "Script/levelinfo.lua";
#endif



static const char  Enemy_Info_Path[] = "Script/enemyinfo.json";
static const char  Hero_Info_Path[] = "Script/heroinfo.json";
static const char  Outfit_Info_Path[] = "Script/outfit.json";
static const char  Enemy_Award_Path[] = "Script/enemyaward.json";

//英雄
static const char  AppleRes[] = "Sprites/pingguo.ExportJson";
static const char  PitayaRes[] = "Sprites/huolongguo.ExportJson";
static const char  DurianRes[] = "Sprites/liulian.ExportJson";
static const char  PineappleRes[] = "Sprites/boluo.ExportJson";
static const char  PeachRes[] = "Sprites/taozi.ExportJson";
static const char  CaoMeiRes[] = "Sprites/caomei.ExportJson";
static const char  XiGuaRes[] = "Sprites/xigua.ExportJson";
static const char  XiangJiaoRes[] = "Sprites/xiangjiao.ExportJson";
static const char  LiZiRes[] = "Sprites/lizi.ExportJson";
static const char  ShiLiuRes[] = "Sprites/shiliu.ExportJson";
static const char  MangGuoRes[] = "Sprites/mangguo.ExportJson";
//怪
static const char  FengXiYiRes[] = "Sprites/fengxiyi.ExportJson";
static const char  GeBuLinRes[] = "Sprites/gebulin.ExportJson";
static const char  RuanNIGuaiRes[] = "Sprites/ruanniguai.ExportJson";
static const char  JuXingGeBuLinRes[] = "Sprites/juxinggebulin.ExportJson";
static const char  XueYuGangXieRes[] = "Sprites/xueyugangxie.ExportJson";
static const char  ChangMaoXueGuaiRes[] = "Sprites/changmaoxueguai.ExportJson";
static const char  ShuiJingZhangyuRes[] = "Sprites/shuijingzhangyu.ExportJson";
static const char  BingBaoHeTunRes[] = "Sprites/bingbaohetun.ExportJson";
static const char  ABossRes[] = "Sprites/aBoss.ExportJson";
static const char  BBossRes[] = "Sprites/bBoss.ExportJson";
static const char  CBossRes[] = "Sprites/cBoss.ExportJson";
static const char  DBossRes[] = "Sprites/dBoss.ExportJson";
static const char  HuangMoGeBuLinRes[] = "Sprites/huangmogebulin.ExportJson";
static const char  ShaMoJiaChongRes[] = "Sprites/shamojiachong.ExportJson";
static const char  ShaMoFeiLongRes[] = "Sprites/shamofeilong.ExportJson";
static const char  ShaBaoJuRenRes[] = "Sprites/shabaojuren.ExportJson";
static const char  EBossRes[] = "Sprites/eBoss.ExportJson";
static const char  FBossRes[] = "Sprites/fBoss.ExportJson";
static const char  GBossRes[] = "Sprites/gBoss.ExportJson";
static const char  DiYuFengQuanRes[] = "Sprites/diyufengquan.ExportJson";
static const char  LingHunShouGeZheRes[] = "Sprites/linghunshougezhe.ExportJson";
static const char  JiaoTangKuiLeiRes[] = "Sprites/jiaotangkuilei.ExportJson";
static const char  YouEMoRes[] = "Sprites/youemo.ExportJson";
static const char  TangXinMoRes[] = "Sprites/tangxinmo.ExportJson";
static const char  HeiTangEMoRes[] = "Sprites/heitangemo.ExportJson";
static const char  DiYuMoMengRes[] = "Sprites/diyumomeng.ExportJson";
static const char  XiaoGuiRes[] = "Sprites/xiaogui.ExportJson";
static const char  HBossRes[] = "Sprites/hBoss.ExportJson";
static const char  IBossRes[] = "Sprites/iBoss.ExportJson";


//特效文件
static const char  ChuxianRes[] = "Sprites/chuxian.ExportJson";
static const char  RanShaoRes[] = "Sprites/ranshao.ExportJson";
static const char  DuARes[] = "Sprites/du_a.ExportJson";
static const char  DuBRes[] = "Sprites/du_b.ExportJson";
static const char  BingZhaRes[] = "Sprites/bingzha.ExportJson";
static const char  XiangJiaoARes[] = "Sprites/xiangjiao_a.ExportJson";
static const char  HuoHuaRes[] = "Sprites/huohua.ExportJson";
static const char  HuDunARes[] = "Sprites/hudun_a.ExportJson";
static const char  BoLuoBaoZhaRes[] = "Sprites/boluobaozha.ExportJson";
static const char  BaoJiHuoQiuRes[] = "Sprites/baojihuoqiu.ExportJson";
static const char  XingXingRes[] = "Sprites/xingxing.ExportJson";
static const char  TengManRes[] = "Sprites/tengman.ExportJson";
static const char  ShiLiuARes[] = "Sprites/shiliu_a.ExportJson";
static const char  ShiLiuBRes[] = "Sprites/shiliu_b.ExportJson";
static const char  XuanFengRes[] = "Sprites/xuanfeng.ExportJson";
static const char  HuoQiuRes[] = "Sprites/huoqiu.ExportJson";
static const char  BaoZhaRes[] = "Sprites/baozha.ExportJson";
static const char  HeiDongRes[] = "Sprites/heidong.ExportJson";
static const char  BingBRes[] = "Sprites/bing_b.ExportJson";
static const char  BingCRes[] = "Sprites/bing_c.ExportJson";
static const char  ChuanRes[] = "Sprites/chuan.ExportJson";
static const char  ZhunXinRes[] = "Sprites/zhunxin.ExportJson";
static const char  LongJuanFengRes[] = "Sprites/longjuanfeng.ExportJson";
static const char  XiaoShiRes[] = "Sprites/xiaoshi.ExportJson";
static const char  JinBiRes[] = "Sprites/jinbi.ExportJson";
static const char  LiPinRes[] = "Sprites/lipin.ExportJson";
static const char  ZuanShiRes[] = "Sprites/zuanshi.ExportJson";
static const char  MoFaLunARes[] = "Sprites/mofalun_a.ExportJson";
static const char  MoFaLunBRes[] = "Sprites/mofalun_b.ExportJson";
static const char  ShanXianRes[] = "Sprites/shanxian.ExportJson";
static const char  HuiFuRes[] = "Sprites/huifu.ExportJson";
static const char  LuoShiRes[] = "Sprites/luoshi.ExportJson";
static const char  ShiBeiRes[] = "Sprites/shibei.ExportJson";
static const char  BingDRes[] = "Sprites/bing_d.ExportJson";
static const char  ShaBaoRes[] = "Sprites/shabao.ExportJson";
static const char  HuDunBRes[] = "Sprites/hudun_b.ExportJson";
static const char  DaoShuRes[] = "Sprites/daoshu.ExportJson";
static const char  ShiFangRes[] = "Sprites/shifang.ExportJson";
static const char  QiangBaoGuaiRes[] = "Sprites/qiangbaoguai.ExportJson";
static const char  QiangBaoTuBiaoRes[] = "Sprites/qiangbaotubiao.ExportJson";
static const char  MuBeiRes[] = "Sprites/mubei.ExportJson";

static const char  BingKuaiResA[] = "bingkuai_a.png";
static const char  BingKuaiResB[] = "bingkuai_b.png";
static const char  PeelRes[] = "peel.png";
static const char  BoneRes[] = "gutou.png";
static const char  YanShiRes[] =  "yanshi.png";
static const char  ZiDanRes[] = "zidan.png";
static const char  MoShuiRes[] = "moshui.png";
static const char  JianQiRes[] = "jianqi.png";
static const char  ShanDianRes[] = "shandian.png";
static const char  BingRes[] = "bing.png";
static const char  QiangBao_Txt[] = "qiangbao_a.png";
//static const char  QiangBao_Light[] = "qiangbao_b.png";
static const char  BingFengRes[] = "bingfeng.png";
static const char  ChongJiBoRes[] = "chongjibo.png";
static const char  SleepRes[] = "sleep.png";

//游戏结束界面
static const char Game_Result_BG[] = "dlg_mid.png";
static const char Game_Result_BG_Small[] = "dlg_small.png";
static const char Txt_Hit[] = "txt_hit.png";
static const char Txt_Damage[] = "txt_damage.png";
static const char Txt_Shake[] = "txt_shake.png";
static const char Txt_YouWin[] = "txt_youwin.png";
static const char Txt_YouLost[] = "txt_youlost.png";
static const char Txt_JiangLi[] = "txt_jiangli.png";
static const char Icon_Star[] = "icon_star.png";
static const char Icon_Star_Blank[] = "icon_star_blank.png";
static const char Icon_Box[] = "icon_box.png";
static const char Icon_Share[] = "icon_share.png";
static const char Btn_Back[] = "btn_back.png";
static const char Btn_Back_Clicked[] = "btn_back_clicked.png";
static const char Btn_Right[] = "btn_right.png";
static const char Btn_Continue[] = "btn_continue.png";
static const char Btn_Continue_Disable[] = "btn_continue_disable.png";
static const char Btn_Continue_Clicked[] = "btn_continue_clicked.png";
static const char Btn_Restart[] = "btn_restart.png";
static const char Btn_Restart_Clicked[] = "btn_restart_clicked.png";
static const char Icon_Percent_Green[] = "icon_percent_green.png";
static const char Icon_Percent_Orange[] = "icon_percent_orange.png";
static const char Box_Close[] = "baoxiang_a.png";
static const char Box_Open[] = "baoxiang_b.png";
static const char No_Reward[] = "diaobao.png";
static const char GuaiShouChuMo[] = "guaishouchumo.png";
static const char DaBoGuaiShou[] = "daboguaishoulailin.png";
static const char XinHuoBan[] = "gongxihuoban.png";
static const char XinShouLiBao[] = "gongxilibao.png";
static const char XinChongWu[] = "gongxichongwu.png";

//游戏中的ui
static const char Game_Pause_Btn_Continue[] = "game_pause_btn_continue.png";
static const char Game_Pause_Btn_restart[] = "game_pause_btn_restart.png";
static const char Game_Pause_Btn_Exit[] = "game_pause_btn_exit.png";
static const char Game_Pause_Btn_Continue_Click[] = "game_pause_btn_continue_click.png";
static const char Game_Pause_Btn_restart_Click[] = "game_pause_btn_restart_click.png";
static const char Game_Pause_Btn_Exit_Click[] = "game_pause_btn_exit_click.png";
static const char Game_Btn_Pause[] = "btn_pause.png";
static const char Game_Btn_Pause_Clicked[] = "btn_pause_clicked.png";
static const char Icon_Gold_Small[] = "icon_gold_small.png";
static const char Icon_Time[] = "game_time_icon.png";
static const char Icon_Gem_Small[] = "icon_gem_small.png";
static const char Progress_Bar[] = "progress_bar.png";
static const char Progress_Frame[] = "progress_frame.png";
static const char Progress_Icon[] = "progress_icon.png";
static const char Prop_Panzi_Random[] = "panzi_random.png";
static const char Prop_Panzi[] = "panzi.png";
static const char Icon_Prop_Random[] = "random.png";
static const char Icon_Prop_Random2[] = "random2.png";
static const char Best_Record[] = "best_record.png";
static const char Hp_Boss_Frame[] = "hp_boss_frame.png";
static const char Hp_Boss_Bar[] = "hp_boss_bar.png";
static const char Hp_Barrier_Frame[] = "hp_barrier_frame.png";
static const char Hp_Barrier_Bar[] = "hp_barrier_bar.png";
static const char Lans_Skill_Normal[] = "Lans_Skill_Normal.png";
static const char Lans_Skill_Disable[] = "Lans_Skill_Disable.png";
static const char Lans_Skill_Disable_Mark[] = "Lans_Skill_Disable_Mark.png";
static const char Loading_Progress_Frame[] = "loading_progress_frame.png";
static const char Loading_Progress_Bar[] = "loading_progress_bar.png";
static const char Loading_Text[] = "loading_text.png";
static const char Loading_BG[] = "ui/bg_2.png";
static const char Text_Miss[] = "miss.png";
static const char Text_BaoJi[] = "baoji.png";
static const char NuQi_Frame[] = "nqi_01.png";
static const char NuQi_Bar[] = "nqi_02.png";
static const char NuQi_Txt[] = "nqi_03.png";
static const char ComBo_Txt[] = "combo_txt.png";
static const char YXGL_Txt[] = "yxglai.png";

static const char LianNuFrame[] = "liannu1.png";
static const char LianNuTray[] = "liannu2.png";
static const char CatapultFrame[] = "catapult1.png";
static const char CatapultTray[] = "catapult2.png";
static const char CatapultFrameTie[] = "catapult3.png";
static const char ZhaLanRes[] = "zhalan.png";
static const char ShiZhaLanRes[] = "shilanshan_1.png";
static const char TieZhaLanRes[] = "tieshanlan_1.png";
static const char LockRes[] = "suo.png";
static const char HongZhaJiRes[] = "feiting.png";

//音效
static const char Jizhong[] = "Sound/jizhong.ogg";
static const char Xuli[] = "Sound/xuli.ogg";
static const char Change[] = "Sound/menusel_0.ogg";
static const char Moveup[] = "Sound/moveup.ogg";
static const char Time[] = "Sound/times.ogg";
static const char Sound_Btn_Clicked[] = "Sound/menusel_1.ogg";
static const char GetGift[] = "Sound/bonus.ogg";
//static const char NoGift[] = "Sound/jus_0.wav";
static const char GameWin[] = "Sound/Victory.ogg";
static const char GameLose[] = "Sound/Failure.ogg";
static const char Die1[] = "Sound/die1.ogg";
static const char Die2[] = "Sound/die2.ogg";
static const char UseProp[] = "Sound/useprop.ogg";
static const char Item_Explode[] = "Sound/explode.ogg";
static const char Sound_Atk[] = "Sound/sound_atk.ogg";
static const char Item_Frozen[] = "Sound/frozen.ogg";
static const char Award_Gold[] = "Sound/sound_award_gold.ogg";
static const char Award_Gem[] = "Sound/sound_award_gem.ogg";
static const char Shake_Sound[]= "Sound/shake.ogg";
static const char Shake_Award[] = "Sound/shake_award.ogg"; //摇奖中奖
static const char Shake_Not_Award[] = "Sound/shake_not_award.ogg"; //摇奖未中奖
static const char Add_HP[]= "Sound/add_hp.ogg";  //加满血
static const char Sound_ShanXian[] = "Sound/Invisibility.ogg"; //闪现
static const char Sound_ChongCi[] = "Sound/sprint.ogg";
static const char Sound_Click_Error[] = "Sound/click_error.ogg";
static const char Sound_Energy[] = "Sound/energy.ogg";
static const char Sound_Angry[] = "Sound/angry.ogg";

static const char load_scene_music[] = "Sound/logo.ogg";
static const char title_scene_back_music[] = "Sound/menu.ogg";
static const char title_scene_menu_start[] = "Sound/result_star.ogg";
static const char home_scene_back_music[] = "Sound/game_0.ogg";

//Tips
static const char tips_TxtPlist[] = "Tips/Tips.plist";
static const char tips_TxtPic[] = "Tips/Tips.pvr.ccz";
static const char tips_format_str[] = "tips_txt%d.png";

//存档 关键字
static const char _isHaveArchiveFile[] = "isHaveArchiveFile";
static const char _firsttime[] = "firsttime";  //第一次启动
static const char _unlockDefense2[] = "unlockDefense2";
static const char _gold[] = "gold";      //金币
static const char _energy[] = "energy";  //能量
static const char _gem[] = "gem";        //钻石
static const char _power[] = "power";    //体力
static const char _rookie[] = "rookie";  //新手礼包
static const char _openBackMusic[] = "openBackMusic";
static const char _openSoundEffect[] = "openSoundEffect";
static const char _chapter[] = "chapter";   //章节数
static const char _level[] = "level";  //关卡数
static const char _unlockChapter[] = "unlockChapter";
static const char _bestTime[] = "bestTime";
static const char _teachLevelOne[] = "levelOne";
static const char _teachLevelTwo[] = "levelTwo";

//ui
static const char ui_plist[] = "ui/ui.plist";
static const char ui_image[] = "ui/ui.pvr.ccz";
static const char bg0_plist[] = "ui/bg0.plist";
static const char bg0_image[] = "ui/bg0.pvr.ccz";
static const char bg1_plist[] = "ui/bg1.plist";
static const char bg1_image[] = "ui/bg1.pvr.ccz";
static const char juese_plist[] = "ui/juese.plist";
static const char juese_image[] = "ui/juese.pvr.ccz";

static const char background_0[] = "bg_0.png";
static const char background_1[] = "bg_1.png";
static const char background_2[] = "ui/bg_2.png";
static const char about_content[] = "ui/gyu01.png";
static const char topbar_1[] = "topbar_1.png";
static const char topbar_2[] = "topbar_2.png";

static const char juese_format_head[] = "head_%d.png";
static const char juese_format_head_gray[] = "head_%d_a.png";
static const char juese_format_name[] = "name_%d.png";

static const char btn_back[] = "btn_prev.png";
static const char btn_back2[] = "btn_prev2.png";
static const char btn_chuzhan[] = "btn_chuzhan.png";
static const char btn_chuzhan2[] = "btn_chuzhan2.png";
static const char btn_next[] = "btn_next.png";
static const char btn_next2[] = "btn_next2.png";
static const char arrow_left_1[] = "ico03.png";
static const char arrow_right_1[] = "ico04.png";

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
static const char success_particle[] = "fireworks.plist";
#else
static const char success_particle[] = "particle/fireworks.plist";
#endif

//teach
static const char teach_text_plist[] = "TeachText/TeachText.plist";
static const char teach_text_ccz[] = "TeachText/TeachText.pvr.ccz";
static const char teach_enter_maoxian[] = "teach_txt_1.png";
static const char teach_enter_fuben1[] = "teach_txt_12.png";
static const char teach_enter_fuben2[] = "teach_txt_25.png";
static const char teach_enter_fuben3[] = "teach_txt_26.png";
static const char teach_enter_fuben4[] = "teach_txt_27.png";
static const char teach_enter_fuben5[] = "teach_txt_28.png";
static const char teach_enter_level[] = "teach_txt_2.png";
static const char teach_shop_select_outfit[] = "teach_txt_23.png";
static const char teach_shop_buy_outfit[] = "teach_txt_24.png";
static const char teach_enter_selectHero[] = "teach_txt_7.png";
static const char teach_enter_selectOutfit[] = "teach_txt_8.png";
static const char teach_enter_selectPet[] = "teach_txt_19.png";
static const char teach_to_start_game[] = "teach_txt_22.png";//点击开始游戏
static const char teach_to_selectHero[] = "teach_txt_17.png";
static const char teach_to_selectOutfit[] = "teach_txt_23.png";
static const char teach_to_upgrade_pet[] = "teach_txt_18.png";
static const char teach_shouzhi_plist[] = "scene/shouzhi.plist";
static const char teach_shouzhi_ccz[] = "scene/shouzhi.pvr.ccz";
static const char teach_shouzhi1[] = "shouzhi1.png";
static const char teach_shouzhi2[] = "shouzhi2.png";

//load scene
static const char loading_1a[] = "loading_1a.png";
static const char loading_1b[] = "loading_1b.png";
static const char loading_wd[] = "loading_wd.png";

//title scene
static const char title_scene_plist[] = "TitleScene/TitleScene.plist";
static const char title_scene_image[] = "TitleScene/TitleScene.pvr.ccz";
static const char title_scene_startBtn[] = "TitleScene/anniu.ExportJson";
static const char title_1[] = "title_1.png";
static const char title_2[] = "title_2.png";

//home scene
static const char home_scene_json[] = "HomeScene/HomeScene.json";
static const char home_scene_plist[] = "HomeScene/homeScene.plist";
static const char home_scene_image[] = "HomeScene/homeScene.pvr.ccz";
static const char home_scene_bg_plist[] = "HomeScene/homeSceneBg.plist";
static const char home_scene_bg_image[] = "HomeScene/homeSceneBg.pvr.ccz";
static const char home_scene_bg_day[] = "bg_sky_01_a.png";
static const char home_scene_bg_night[] = "bg_sky_02_a.png";
static const char home_cloud_1[] = "bg_yun_01.png";
static const char home_cloud_2[] = "bg_yun_02.png";
static const char home_cloud_3[] = "bg_yun_03.png";
static const char home_cloud_4[] = "bg_yun_04.png";
static const char home_cloud_5[] = "bg_yun_05.png";

static const char set_ico_board[] = "ico93.png";
static const char set_ico_call_out[] = "ico90.png";
static const char set_ico_call_in[] = "ico91.png";


//buy gem scene
//buy gold scene
static const char top_money_item_json[] = "MoneyItem/TopMoneyItem.json";
static const char money_item_json[] = "MoneyItem/MoneyItem.json";
static const char money_item_plist[] = "MoneyItem/moneyItem.plist";
static const char money_item_image[] = "MoneyItem/moneyItem.pvr.ccz";
static const char money_item_board[] = "money_board.png";

//shop scene
static const char shop_scene_json[] = "ShopScene/ShopScene_Table.json";
static const char shop_scene_body_plist[] = "ShopScene/ShopSceneBody.plist";
static const char shop_scene_body_image[] = "ShopScene/ShopSceneBody.pvr.ccz";
static const char shop_buy_gem[] = "shop_btn1.png";
static const char shop_item_null_board[] = "shop_outfit.png";
static const char shop_price_board_gold[] = "shop_price1.png";
static const char shop_price_board_gem[] = "shop_price2.png";

//tavern scene
static const char tavern_scene_body_json[] = "scene/TavernScene_Body.json";
static const char tavern_scene_body_plist[] = "scene/tavernScene_body.plist";
static const char tavern_scene_body_image[] = "scene/tavernScene_body.pvr.ccz";
static const char tavern_scene_page_json[] = "scene/TavernScene_Page.json";
static const char tavern_button_unlock[] = "jguan_btn_jiesuo.png";
static const char tavern_button_train[] = "jguan_btn_xunlian.png";
static const char tavern_tab_hero[] = "ico75.png";
static const char tavern_tab_hero_disable[] = "ico75a.png";
static const char tavern_tab_pet[] = "ico76.png";
static const char tavern_tab_pet_disable[] = "ico76a.png";

//dungeon scene
static const char dungeon_island_json[] = "scene/DungeonScene_Island.json";
static const char dungeon_island_plist[] = "scene/dungeonScene.plist";
static const char dungeon_island_image[] = "scene/dungeonScene.pvr.ccz";
static const char fuben_info_plist[] = "TeachText/fubenInfo.plist";
static const char fuben_info_image[] = "TeachText/fubenInfo.pvr.ccz";

//level scene
static const char level_cup_plist[] = "LevelScene/levelCup.plist";
static const char level_cup_image[] = "LevelScene/levelCup.pvr.ccz";
static const char level_map_json_format[] = "LevelScene/LevelMap_%d.json";
static const char level_cup_1[] = "level_cup_1.png";
static const char level_cup_2[] = "level_cup_2.png";
static const char level_cup_3[] = "level_cup_3.png";
static const char level_cup_1a[] = "level_cup_1a.png";
static const char level_cup_2a[] = "level_cup_2a.png";
static const char level_cup_3a[] = "level_cup_3a.png";
static const char level_cup_lock[] = "level_cup_null.png";
static const char level_cup_lock2[] = "level_cup_null2.png";//boss
static const char level_cup_normal[] = "level_cup_normal.png";
static const char level_cup_boss[] = "level_cup_boss.png";
static const char level_cup_focus_animation[] = "LevelScene/dianji.ExportJson";
static const char level_bg1_plist[] = "LevelScene/level_bg_1.plist";
static const char level_bg1_image[] = "LevelScene/level_bg_1.pvr.ccz";
static const char level_bg2_plist[] = "LevelScene/level_bg_2.plist";
static const char level_bg2_image[] = "LevelScene/level_bg_2.pvr.ccz";
static const char level_bg3_plist[] = "LevelScene/level_bg_3.plist";
static const char level_bg3_image[] = "LevelScene/level_bg_3.pvr.ccz";
static const char level_bg4_plist[] = "LevelScene/level_bg_4.plist";
static const char level_bg4_image[] = "LevelScene/level_bg_4.pvr.ccz";
static const char level_bg5_plist[] = "LevelScene/level_bg_5.plist";
static const char level_bg5_image[] = "LevelScene/level_bg_5.pvr.ccz";

//plan scene
static const char plan_scene_main_pvp_json[] = "scene/PlanScene_Main_PVP.json";
static const char plan_scene_main_pve_json[] = "scene/PlanScene_Main_PVE.json";
static const char plan_scene_outfit_json[] = "scene/PlanScene_Outfit.json";
static const char plan_scene_petpage_json[] = "scene/PlanScene_PetPage.json";
static const char plan_scene_body_plist[] = "scene/PlanSceneBody.plist";
static const char plan_scene_body_image[] = "scene/PlanSceneBody.pvr.ccz";
static const char plan_item_lock[] = "plan_lock.png";
static const char plan_scene_headboard_gray[] = "box10_a.png";

static const char top_ico_shop[] = "ico_shop.png";

static const char plan_topbar_selhero[] = "plan_top1.png";
static const char plan_topbar_seloutfit[] = "plan_top2.png";        //“道具选择”
static const char plan_topbar_selpet[] = "plan_top3.png";        //"宠物选择"
static const char plan_seloutfit_board[] = "ico09.png";

//defense scene
static const char defense_scene_json[] = "scene/DefenseScene.json";

//story scene
#define STORY_SCENE_STEP_NUM    (10)
static const char* storyJsonArray[STORY_SCENE_STEP_NUM] =
{
    "StoryScene/a.ExportJson",
    "StoryScene/b.ExportJson",
    "StoryScene/c.ExportJson",
    "StoryScene/d.ExportJson",
    "StoryScene/e.ExportJson",
    "StoryScene/f.ExportJson",
    "StoryScene/g.ExportJson",
    "StoryScene/h.ExportJson",
    "StoryScene/i.ExportJson",
    "StoryScene/j.ExportJson"
};

static const char* storyAniArray[STORY_SCENE_STEP_NUM] =
{
    "a",
    "b",
    "c",
    "d",
    "e",
    "f",
    "g",
    "h",
    "i",
    "j"
};

//share scene
static const char share_scene_plist[] = "scene/shareScene.plist";
static const char share_scene_image[] = "scene/shareScene.pvr.ccz";

//huodong scene
static const char huodong_scene_body_plist[] = "scene/HuodongSceneBody.plist";
static const char huodong_scene_body_image[] = "scene/HuodongSceneBody.pvr.ccz";
static const char huodong_scene_topbar_name[] = "huodong_top1.png";
static const char huodong_qiandao[] = "huodong_item3.png";
static const char huodong_boss[] = "huodong_item2.png";
static const char huodong_share[] = "huodong_item1.png";

//pvp scene                         
static const char pvpLoadingRes[] = "Sprites_UI/loading.ExportJson";

#define UUserName    "UserName"
#define ContinueSign "ContinueSign"
#define ShareCount   "ShareCount"
#define BigPrize     "BigPrize"
#define TotalScore   "TotalScore"
#define LastSignW    "LastSignW"
#define LastSignD    "LastSignD"


//social
#define ULoginTime   "LoginTime"


#endif
