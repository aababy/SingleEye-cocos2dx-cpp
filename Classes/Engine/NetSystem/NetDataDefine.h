#ifndef _FightDataDefined_H_
#define _FightDataDefined_H_

#include "UserData.h"
typedef enum
{
    ROUND_ACT_NOR = 1,//普攻
    ROUND_ACT_SKILL,//技攻
    ROUND_ACT_HJ,//合击
    ROUND_ACT_SZY,//神之语
    ROUND_ACT_MFJ,//闪避反击
    ROUND_ACT_DFJ,//格档反击
    ROUND_ACT_BUFFOVER = 100,//战斗结束(buffer效果播放)
}ROUNDTYPE;//回合类型

typedef enum
{
    CHANGE_MISS = 0, //闪避
    CHANGE_GOAL,    //命中
    CHANGE_BAOJI,   //爆击
    CHANGE_GEDANG,  //格档
    CHANGE_FHURT,   //反伤
    CHANGE_FRLIFE,  //被动技复活
    CHANGE_ADSKHP,  //技能吸血
    CHANGE_ADSKMP,  //技能回蓝
    CHANGE_ROUNDMP, //每轮发起进攻者的加蓝或减蓝
    CHANGE_BUFF,    //仅buffer改变
}CHANGE_TYPE;//受击类型

typedef enum
{
    CHANGE_NONE = 0,//无改变
    CHANGE_HP,      //生命值
    CHANGE_MP,      //魔法值
    CHANGE_SPEED,   //速度值
}CHANGE_PROP_TYPE;//卡牌属性改变

typedef enum _FIGHT_LINE_LEVL_
{
    LINE_LEV1 = 1,//普通状态
    LINE_LEV2,//合击读数状态
    LINE_LEV3,//合击激活状态
    LINE_LEV4,//神之语读数状态
    LINE_LEV5,//神之语激活状态
}LINE_LEVEL;//六芒星阵状态

typedef enum _DROP_TYPE_
{
    DROP_NONE = 0,//空
    DROP_GOLDEN = 1,//1.金币(暂未使用)
    DROP_STRENGTH,//2.体力(暂未使用)
    DROP_STONE,//3.魔晶石(暂未使用)
    DROP_TOOL,//4.道具
    DROP_CARD,//5.卡牌
    DROP_EQUP,//6.装备
    DROP_NS,//7.解锁女神卡
}DROP_TYPE;//物品掉落种类
#ifndef _String_def
#define _String_def
typedef struct _String_
{
	INT16 mlen;//字符串长度，字符串为空时，此处为0
	byte *pbytes;//字节流Bytes 字符串为空时，此处不存在
}String;//字符串
#endif
typedef struct _KeyValue_
{
    String key;
    String value;
}KeyValue; //2.3.2	字符串(KeyValue) 一对键值对应字符串



typedef struct _AddBufferInfo_
{
	INT16	BufferId;//BufferId
	INT32	effectTime;//生效时间单位毫秒（值为0时表示去掉这个Buffer）
}AddBufferInfo;//战斗时卡牌上附加的buffer信息

//typedef struct _PlayerInfo_
//{
//	INT32	userId; //用户ID
//	INT16	serverId;//所属服务器ID
//	String	UserName;//玩家昵称
//	byte	userSex;//”用户性别:0-女 1-男
//	INT16	gameLevelSeq;//玩家游戏级别序号
//	String	gameLevel;//玩家游戏级别文字说明
//	INT32   gamePoint;//玩家总的经验值
//	INT32	NextLevelPoint;//下一级所需经验值
//	INT32	currentLevelPoint;//升到下一级已获得的经验（玩家总经验-当前级所需总经验）
//	INT32	teamPoint;//玩家队伍实力
//	INT32	alexa;//玩家在游戏中的排名
//	INT16	ActiveValue;//体力值（行动值）
//    INT16   ActiveUpTime;//下次体力增长倒记时(秒)
//    INT16	vipLevel;//VIP等级
//	INT64	glodCoin;//游戏金币数量
//	INT32	Money;//货币数量
//	byte	ISREG;//是否绑定发用户名、密码 0：未绑定 1：绑定
//    INT32   GOD_POINT;//神格碎片（女神升级）
//    INT32   SOUL_CRYSTAL;//灵魂水晶（女神技）
//    INT32   LUCK_CRYSTAL;//幸运水晶
//    INT32   MOON_STONE;//月石（装备获取）
//    INT32   STAR_STONE;//星石（装备升级）
//    INT32   STAR_ELITE;//星之精华（抽卡）
//    INT32   MOON_ELITE;//月之精华（抽卡）
//    INT32   SUN_ELITE;//日之精华（抽卡）
//    INT32   SOUL_POINT_WHILE;//白色灵魂碎片
//    INT32   SOUL_POINT_GREEN;//绿色灵魂碎片
//    INT32   SOUL_POINT_BLUE;//蓝色灵魂碎片
//    INT32   SOUL_POINT_PURPLE;//紫色灵魂碎片
//    INT32   SOUL_POINT_GLODEN;//金色灵魂碎片
//    INT32   SOUL_POINT_DEEP_GOLDEN;//暗金色灵魂碎片
//    INT32   SOUL_POINT_RED;//红色灵魂碎片
//    INT16   SOUL_BAG_WHITE;//白色品质角色卡包
//    INT16   SOUL_BAG_GREEN;//绿色品质角色卡包
//    INT16   SOUL_BAG_BLUE;//蓝色品质角色卡包
//    INT16   SOUL_BAG_PURPLE;//紫色品质角色卡包
//    INT16   SOUL_BAG_GOLDEN;//金色品质角色卡包
//    INT16   SOUL_BAG_DEEP_GOLDEN;//暗金色品质角色卡包
//    INT16   SOUL_BAG_RED;//红色品质角色卡包
//    INT16   SOUL_BOX_NORMAL;//排名宝箱
//    INT16   SOUL_BOX_LOWER;//精美幸运宝箱
//    INT16   SOUL_BOX_MIDDLE;//高贵幸运宝箱
//    INT16   SOUL_BOX_HIGHER;//至尊幸运宝箱
//}PlayerInfo;//玩家基本信息

//typedef struct _PlayerInfoDetail_
//{
//    PlayerInfo	PlayerInfo;//玩家基本信息
//    byte        CardsNum;//	Byte	玩家当前携带的上阵卡牌数
//    Card[CardsNum]	Card [CardsNum]	Card [CardsNum]
//}PlayerInfoDetail;//2.3.12	玩家信息(PlayerInfoDetail)(附带了出战卡信息)

typedef struct _OutBattleCardProp_
{
	INT32	HP;//生命值
	INT16	MP;//魔法值
	INT16	SPEED;//速度值
}OutBattleCardProp;

typedef struct _OutBattleCard_
{
	INT16	cardId;//卡牌ID（ID为0时表示此位置无卡，后续数据都不生成）
	String	cardName;//卡牌名称
	INT16	cardLevel;//卡牌等级
	byte	isAlive;//0：阵亡 1：存活
	OutBattleCardProp	CardProp;//卡牌各项数值
}OutBattleCard;//战场出战卡牌信息

typedef struct _BattleCard_
{
	INT16	cardId;//卡牌ID（ID为0时表示此位置无卡，后续数据都不生成）
	byte	isAlive;//0:阵亡 1:存活
	INT16	currentSpeed;//当前卡牌的速度值
	byte	SkillActive;//0.没满足技能施放条件 1.已满足下次可以施放
	byte	BufferCount;//战斗中附加的Buffer数
    AddBufferInfo *pAddBufferInfo;//战斗中附加的Buffer信息
}BattleCard;//战斗中卡牌信息

typedef struct _BufferChange_
{
	byte	chairId;//变化的卡牌方
	byte	changeCardSeq;//变化的卡牌序号
	INT16	bufferId;//bufferId
	byte	changePropType;//属性改变类型0.无改变 1.生命 2.魔法 3.速度
	INT32	changeVal;//改变值
	byte    dropBuffer;//1 去掉buffer  0 保留buffer
}BufferChange;//卡牌buffer状态改变

typedef struct _CardChange_
{
	byte	changeCardSeq;//变化的卡牌序号
	byte	changeType;//0:MISS ; 1命中; 2爆击; 3格档; 4反伤; 5被动技复活; 6技能吸血; 7技能回蓝; 8每轮发起进攻者的加蓝或减蓝; 10仅buffer改变
	byte	changePropType;//属性改变类型：0:未改变 1.生命值 2.魔法值 3.速度值
	INT32	changeVal;//改变值（正数表增加，负数表减小）
	byte	addBufferCount;//增加了的Buffer数量
	AddBufferInfo *paddBuffer;//增加的Buffer信息
}CardChange;//卡牌状态改变

typedef struct _SixZonePoint_
{
    byte	Point1;//0:锁定 1:解锁
	byte	Point2;//0:锁定 1:解锁
	byte	Point3;//0:锁定 1:解锁
	byte	Point4;//0:锁定 1:解锁
	byte	Point5;//0:锁定 1:解锁
	byte	Point6;//0:锁定 1:解锁
    byte    center;//神之语开启 0:锁定 1:解锁
    INT16   godSkill;//神之语技能 0:无
}SixZonePoint;//六芒星阵-点

typedef struct _SixZoneLine_
{
    byte	Line1;//1:普通状态;2:合击读数状态;3:合击激活状态;4:神之语读数状态;5:神之语激活状态（point1到poin2）
	byte	Line2;//状态同Line1   (point2到point3)
	byte	Line3;//状态同Line1   (point3到point1)
	byte	Line4;//状态同Line1   (point4到point5)
	byte	Line5;//状态同Line1   (point5到point6)
	byte	Line6;//状态同Line1   (point6到point4)
}SixZoneLine;//六芒星阵-线

typedef struct _PlayBack_
{
	byte roundType;//1:普通 2:技能 3:合击 4:神之语 5:闪避后反击 6:格档后反击
                   //100:战斗结束(Buffer效果回放后结束)
	BattleCard AttackCard[6];//攻方卡牌信息（数组的顺序对应站位）
	BattleCard GuardCard[6];//守方卡牌信息（数组的顺序对应站位)
	INT16	bufferChangeCount;//所有卡牌的Buffer变动数
	BufferChange *pallBufferChange;//所有卡牌的Buffer变动
	INT32	costTime;//当前轮次所耗时间（毫秒）目前roundType 1,2 才增加轮次数，就是不增加轮次的这里值是0
	byte	activeChair;//当前行动方（0，1）
	byte	activeCardSeq;//当前行动卡牌序号
	INT16	skillId;//所发技能ID,无技能时值为0
	byte	targetChair;//受击方（0，1）
	byte	changeCount;//受击方卡牌改变数
	CardChange *ptargetCardChange;//受击方卡牌改变
	byte    otherChangeCount;//非受击一方的卡牌改变数
	CardChange *potherCardChange;//非受击一方卡牌改变
	SixZoneLine	AttackSixZone;//攻方六芒星阵
	SixZoneLine	GuardSixZone;//守方六芒星阵
    // add 2013.11.5 
    INT16 AttackAP;//	Int16	攻方怒气值
    INT16 GuardAP;//	Int16	守方怒气值

}PlayBack;//战斗回放

typedef struct _BattleSeq_
{
	byte	activeChairId;//行动方(0,1)
	byte	activeCardSeq;//当前行动卡牌序号
}BattleSeq;//
#ifndef _RES_STAGE_SWEEP_def
#define _RES_STAGE_SWEEP_def
typedef struct _DropItemInfo_
{
	byte    dropType;//掉落类型（1.金币 2.体力 3.魔晶石 ｜ 4.道具 5.卡牌 6.装备 7.解锁女神卡）
	INT32	dropItemId;//掉落道具ID
    String  dropItemName;//掉落道具名称
    INT32   dropItemIdNums;//掉落数量
    String  dropItemNote;//掉落的道具说明
}DropItemInfo;//关卡掉落物品信息

typedef struct _RES_STAGE_SWEEP_
{
    INT16 gamePoint;//	Int32	获得经验
    INT16 userGameLevel;//	//Int16	玩家当前等级
    INT32 dropGoldCoin;//	//Int32	金币掉落
    INT16 dropItemCount;//	//Int16	道具掉落数
    DropItemInfo *pDropItemInfo;//    道具掉落信息列表
}RES_STAGE_SWEEP;
#endif


//typedef struct _RES_LOGIN_
//{
//    INT32       sessionID;//	用法参考MessageHeader
//    byte        nextStep;//     客户端下一步操作
//    byte        forceFlag;//	强制标志位
//    String      clientVer;// 	String	客户端程序版本号
//    CPlayerInfo  userInfo;//     PlayerInfo	用户信息
//    INT16       configNum;//	Int16	配置项数
//    KeyValue*    configList;//	KeyValue[configNum]	配置项清单
//}DATA_RES_LOGIN;

typedef struct _RES_MESSAGE_
{
    INT16   resCode;    //	Int16	响应码
    String  resMessage;//	String	响应信息
}DATA_RES_MESSAGE;

#endif 
