//
//  NetWorker.h
//  coslua
//
//  Created by chency on 13-6-9.
//
//

#ifndef coslua_NetWorker_h
#define coslua_NetWorker_h

#include "socketcc.h"
#include "cocos2d.h"
#include "MsgSystem.h"
#include "NetRecvThread.h"
#include "NetSendThread.h"


USING_NS_CC;

//域名及端口
//首次连接失败会尝试用域名连接
//#define SERVER_DOMAIN_NAME "ytring.51wog.com"
//#define SERVER_DOMAIN_PORT 9010
#define SERVER_DOMAIN_NAME "ring.51wog.com"
#define SERVER_DOMAIN_PORT 9010

#define VersionMode 1 //0-商用版本 1-测试-外网稳定版本 2-测试-外网最新版本 3-测试——内网稳定版本 4测试——内网最新版本
#if VersionMode == 0
//商用版本
#define SERVER_IP "203.90.239.142"
#define SERVER_PORT 9010
#endif
#if VersionMode == 1
//测试-外网稳定版本
#define SERVER_IP "118.192.93.73"
#define SERVER_PORT 9010
#endif
#if VersionMode == 2
//测试-外网最新版本
#define SERVER_IP "118.192.93.73"
#define SERVER_PORT 9010
#endif
#if VersionMode == 3
//测试——内网稳定版本
#define SERVER_IP "192.168.8.148"
#define SERVER_PORT 9020
#endif
#if VersionMode == 4
//测试——内网最新版本
#define SERVER_IP "192.168.8.148"
#define SERVER_PORT 9010
#endif
//固定用户ID 测试使用
#define UserID_Mode 0 // 0-正常 1-测试
#if UserID_Mode == 1
#define USER_ID  1240 // 当值>0 表示使用测试ID
#endif
#if UserID_Mode == 0
#define USER_ID  0 // 当值为0 表示是正常模式
#endif

#define MAX_SEND_DATA 5120 // 预估的发送字节长度

class NetSendThread;
class NetRecvThread;
class NetWorker {


public:
    // 单例设计
    static NetWorker* Instance();
    
    NetSendThread* getNetSender();
    NetRecvThread* getNetRecver();
    void Init(string ip, int port, string _domain_name, int _domain_port);
    void SetAddress(string _ip, int _port);//设置服务器地址
    void GetAddress(string &_ip, int &_port);//得到服务器地址
    void CreateNewRecver();
    void DealException(const SocketException &se, bool showErrorMsg=true);
    void ReleaseHandle();
    void Close();
    void Pause();//暂停
    void Resume();//继续
    
    // 统计流量
    void AddFlowData(int64_t t);
    // 打印流量和游戏时间
    void PrintFlowData();
    string GetCMDDescription(const int cmd);//获得网络命令字描述
    // 设置默认的消息处理方法
    // 以下两条用默认的消息处理方法处理：
    // 1、服务器主动推送的数据处理。
    // 2、客户端发送消息时不指明消息回调处理。
    void SetDefaultMsgHandle(HANDLE_MSG handleMsg);

    // 获得当前时间
    //        // 得到年，月，日的方法
    //        {
    //        struct tm *tm;
    //        tm = localtime(&now.tv_sec);
    //        int year = tm->tm_year + 1900;
    //        int month = tm->tm_mon + 1;
    //        int day = tm->tm_mday;
    //        int hour=tm->tm_hour;
    //        int minute=tm->tm_min;
    //        int second=tm->tm_sec;
    //        std::stringstream currentTime;
    //        currentTime<<year<<"-"<<month<<"-"<<day<<"  "<<hour<<":"<<minute<<":"<<second<<"   "<<millSecond;
    //        CCLOG("currentTime is %s ",currentTime.str().c_str());
    //        }
    //打印时间 time 是服务器传过来的毫秒时间
    static void PrintTime(const INT64 *time);
    // 当前系统时间 注意：需要使用long long 类型表示64位毫秒数据
    static INT64 GetTime();
    TCPClientSocket* GetSocket();
    // 得到接收数据的进度
    float GetRecvPercent();
    // test
    int GetRecvPackageNum() const;
    int64_t GetLastRecvTime() const;
    void LogoutMsgSender(int cmd_index); // 注销命令
    void SendMsgToRegister(int cmd_index, int cmd, void *pdata);

    bool lock;

private:
    void Start();
    
private:
    NetSendThread *m_pNetSender;
    NetRecvThread *m_pNetRecver;
    // 默认的消息处理
    HANDLE_MSG defaultMsgHandle;

    // 网络总流量
    int64_t flowData;
    // 开始游戏的时间
    int64_t netBeginTime;

    
    
};

#endif
