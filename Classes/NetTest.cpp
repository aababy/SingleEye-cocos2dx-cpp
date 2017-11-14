//
//  NetTest.cpp
//  coslua
//
//  Created by chency on 13-9-3.
//
//

#include "NetTest.h"
#include "UserData.h"

NetTest* NetTest::Instance()
{
    static NetTest instance;
    return &instance;
}
void NetTest::init()
{
    (MsgSystem::Instance())->RegisterObjAndHandler(this, MSGSYS_HANLE_MSG(NetTest::HandleMsg));
}
// 外部调用开始测试
// 如果不测试则屏蔽掉
void NetTest::begin()
{
    DO_CMD_REQ_LOGIN();
}
void NetTest::DO_CMD_REQ_LOGIN()
{
    //2.4.6.3	校验更新用户基本信息(CMD_REQ_UPDATE_USERINFO) 20
//    (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_UPDATE_USERINFO(this);

    //2.4.6.17	发起战斗 (CMD_REQ_FIGHT) 48
//    INT32 	rankingUserId = 100011;//	战斗玩家Id
//    byte 	isRevenge = 0;//	是否复仇（0 不是  1 是）
//    (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_FIGHT(this, rankingUserId, isRevenge);

//    //2.4.6.20	世界BOSS战 (CMD_REQ_BOSS_FIGHT)  52
//    INT32 killValue = 1000;
//    (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_BOSS_FIGHT(this, killValue);
//
//    //2.4.6.21	快速匹配玩家信息 (CMD_REQ_FAST_MATCH) 54
//    (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_FAST_MATCH(this);
//    //2.4.6.8	消耗宝石 (CMD_REQ_EXPEND) 30
//    INT32 	oldGemstone = UserData::Instance()->pUserInfo->gemstone;//	客户端宝石数
//    INT32 	gemstone = 1;// 	花费宝石数
////    (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_EXPEND(this, oldGemstone, gemstone);
//    //2.4.6.21	购买宝石--首次请求(CMD_REQ_BUY_GEMSTONE) 56
//    (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_BUY_GEMSTONE(this, 1, UserData::Instance()->pUserInfo->gemstone);
//    //2.4.6.22	关卡解锁(CMD_REQ_COPY_RECRUIT) 60
//    (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_COPY_RECRUIT(this, 2, 1105, 1);


//    //2.4.6.12	签到 (CMD_REQ_SIGNIN) 38
//    (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_SIGNIN(this,UserData::Instance()->pUserInfo->gemstone);
    ////2.4.6.22	世界boss战--boss信息(CMD_REQ_BOSS_INFO) 62
//    (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_BOSS_INFO(this,UserData::Instance()->pUserInfo->gemstone);
    ////2.4.6.28	获取分享内容 (CMD_REQ_APP_SENDING_PODIUM)  72
//    (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_APP_SENDING_PODIUM(this, 0);
    //    (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_EXPEND(this, oldGemstone, gemstone);
    //2.4.6.29	世界BOSS战--发起战斗 (CMD_REQ_BOSS_BEGIN_FIGHT)  74
    (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_BOSS_BEGIN_FIGHT(this);
}

void NetTest::HandleMsg(const Message &msg)
{
    CCLOG("NetTest::HandleMsg msgType= %d", msg.m_nMsgType);
    switch (msg.m_nMsgType)
    {
        case CMD_RES_UPDATE_USERINFO://登陆验证 21
        {
            //2.4.6.4	购买体力 (CMD_REQ_BUY_STRENGTH) 22
            INT32 	oldStrength = 10;//	客户端原来体力数
            INT32 	strength = 10;//	购买体力数
            INT32 	gemstone = 1;//	花费宝石数
            (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_BUY_STRENGTH(this, oldStrength, strength, gemstone,UserData::Instance()->pUserInfo->gemstone);

//            //2.4.6.17	发起战斗 (CMD_REQ_FIGHT) 48
//            INT32 	rankingUserId = 100011;//	战斗玩家Id
//            byte 	isRevenge = 0;//	是否复仇（0 不是  1 是）
//            (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_FIGHT(this, rankingUserId, isRevenge,UserData::Instance()->pUserInfo->gemstone);
            break;
        }
        case CMD_RES_BUY_STRENGTH://2.4.6.4.1	购买体力更新成功发送返回更新后信息(CMD_RES__BUY_STRENGTH) 	23
        {
            ////2.4.6.7	酒馆解锁(CMD_REQ_RECRUIT) 28
            INT32 	oldGemstone = UserData::Instance()->pUserInfo->gemstone;//	客户端宝石数
            INT16 	upgradeType = 1;//	解锁类型（1 解锁英雄  2 解锁佣兵）
            INT32 	heroOrMercemaryId = 1001;//	解锁的英雄或佣兵ID
            INT32 	grade = 1;//	解锁的英雄或佣兵等级
            (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_RECRUIT(this, oldGemstone, upgradeType, heroOrMercemaryId, grade);
            break;
        }
        case CMD_RES_RECRUIT:    //2.4.6.7.1	酒馆解锁更新成功发送返回提示信息(CMD_RES_RECRUIT) 	29
        {
            //2.4.6.8	消耗宝石 (CMD_REQ_EXPEND) 30
            INT32 	oldGemstone = UserData::Instance()->pUserInfo->gemstone;//	客户端宝石数
            INT32 	gemstone = 1;// 	花费宝石数
            (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_EXPEND(this, oldGemstone, gemstone);
            break;
        }
        case CMD_RES_EXPEND:    //2.4.6.8.1	消耗宝石更新发送返回提示信息(CMD_RES_EXPEND) 	31
        {
            //2.4.6.9	酒馆升级(CMD_REQ_UPGRADE) 32
            INT32 	oldGemstone = UserData::Instance()->pUserInfo->gemstone;//	客户端宝石数
            INT8 	upgradeType = 1;//	升级类型（1 升级英雄  2 升级佣兵）
            INT32 	oldHeroOrMercemaryId = 1001;//	原英雄（佣兵）ID
            INT32 	oldGrade = 1;//	原英雄（佣兵）等级
            INT32 	newHeroOrMercemaryId = 1002;// 	现英雄（佣兵）ID
            INT32 	newGrade = 2;// 	现英雄（佣兵）等级
            (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_UPGRADE(this, oldGemstone, upgradeType, oldHeroOrMercemaryId, oldGrade, newHeroOrMercemaryId, newGrade);
            break;
        }
        case CMD_RES_UPGRADE:    //2.4.6.9.1	酒馆升级更新成功发送返回提示信息(CMD_RES_UPGRADE) 	33
        {
            //2.4.6.10	玩家注册战斗昵称 (CMD_REQ_REGISTER_USERNAME) 34
            string userName = "玩家昵称new";//	玩家昵称
            (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_REGISTER_USERNAME(this, userName,UserData::Instance()->pUserInfo->gemstone);
            break;
        }
        case CMD_RES_REGISTER_USERNAME:    //2.4.6.10.1	注册昵称成功发送返回提示信息(CMD_RES_REGISTER_USERNAME) 35
        {
            //2.4.6.11	战斗结束信息更新 (CMD_REQ_AF_UPDATE_USERINFO) 36
            byte 	_isSuccess = 1;// 	1 为功方胜利，0为功方失败
            INT32 	gemstone = 10;// 	宝石奖励
            INT32 	gemstoneUsed = 1;// 	宝石消耗
            INT32 	defendUserId = 100011;//	守方玩家Id(守方在胜利一定场次后可获得宝石奖励)
            INT32   score = 100;// Int32 奖励积分（功方失败积分为0）
            (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_AF_UPDATE_USERINFO(this, _isSuccess,UserData::Instance()->pUserInfo->gemstone, gemstone, gemstoneUsed, defendUserId, score);
            break;
        }
        case CMD_RES_AF_UPDATE_USERINFO:    //2.4.6.11.1	战斗结束信息更新成功后的提示信息(CMD_RES_AF_UPDATE_USERINFO) 	37
        {
            //2.4.6.12	签到 (CMD_REQ_SIGNIN) 38
            (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_SIGNIN(this,UserData::Instance()->pUserInfo->gemstone);
            break;
        }
        case CMD_RES_SIGNIN:    //2.4.6.12.1	签到后返回信息 (CMD_RES_SIGNIN) 39
        {
            //2.4.6.14	分享领奖信息更新 (CMD_REQ_APP_PODIUM)  42
            INT32 	shareTimes = 10;//任分享次数
            (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_APP_PODIUM(this, shareTimes,UserData::Instance()->pUserInfo->gemstone);
            break;
        }
        case CMD_RES_APP_PODIUM:    //2.4.6.14.1	分享领奖成功后的提示信息(CMD_RES_APP_PODIUM) 	43
        {
            //2.4.6.15	摇奖 (CMD_REQ_SHAKE) 44
            INT16 	fightModel = 0;//	战斗模式 0 PVP/ 1 PVE
            (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_SHAKE(this, fightModel,UserData::Instance()->pUserInfo->gemstone);
            break;
        }
        case CMD_RES_SHAKE:    //2.4.6.15.1	摇奖返回的信息(CMD_RES_SHAKE) 	45
        {
            //2.4.6.16	获取排行榜玩家信息 (CMD_REQ_RANKING) 46
            (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_RANKING(this,UserData::Instance()->pUserInfo->gemstone);
            break;
        }
        case CMD_RES_RANKING:    //2.4.6.16.1	获取排行榜玩家返回的信息(CMD_RES_RANKING) 	47
        {
            //2.4.6.17	发起战斗 (CMD_REQ_FIGHT) 48
            INT32 	rankingUserId = 100011;//	战斗玩家Id
            byte 	isRevenge = 0;//	是否复仇（0 不是  1 是）
            (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_FIGHT(this, rankingUserId, isRevenge,UserData::Instance()->pUserInfo->gemstone, UserData::Instance()->enemy_mercemaryNum, UserData::Instance()->enemy_listMercemary);
            break;
        }
        case CMD_RES_BOSS_INFO:    //2.4.6.22.1	世界boss战--boss信息返回提示信息(CMD_RES_BOSS_INFO) 	63
        {
            //2.4.6.26	世界Boss战--战斗结束信息更新 (CMD_REQ_BOSS_UPDATE_FIGHT) 69
            bool isSuccess = true;
            INT32 gemstoneUsed = 1;//	Int32	宝石消耗
            INT32 gemstone = 1;// Int32 宝石奖励
            INT32 hp = 100;//	Int32	伤害值

            (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_BOSS_UPDATE_FIGHT(this, isSuccess, UserData::Instance()->pUserInfo->gemstone, gemstone, gemstoneUsed, hp);
            break;

        }
        case CMD_RES_BOSS_UPDATE_FIGHT:    ////2.4.6.26.1	世界Boss战--战斗结束信息更新成功后的提示信息(CMD_RES_BOSS_UPDATE_FIGHT) 	70
        {
            //2.4.6.23	世界boss战--排行榜请求(CMD_REQ_BOSS_RANKING) 64
            (NetWorker::Instance())->getNetSender()->DO_CMD_REQ_BOSS_RANKING(this, UserData::Instance()->pUserInfo->gemstone);
            break;

        }
        case CMD_RES_BOSS_RANKING:    //2.4.6.23.1	世界boss战--排行榜返回提示信息(CMD_RES_BOSS_RANKING) 	65
        {
            //2.4.6.24	世界boss战--奖励领取(CMD_REQ_BOSS_REWARD) 66
            //(NetWorker::Instance())->getNetSender()->DO_CMD_REQ_BOSS_REWARD(this, UserData::Instance()->pUserInfo->gemstone);
            break;
        }
        case CMD_RES_BOSS_REWARD:    //2.4.6.24.1	世界boss战--奖励领取返回提示信息(CMD_RES_BOSS_REWARD) 	67
        {


        }

        default:
            break;
    }
    
}