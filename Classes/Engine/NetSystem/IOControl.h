//
//  IOControl.h
//  coslua
//
//  Created by chency on 13-6-18.
//
//

#ifndef coslua_IOControl_h
#define coslua_IOControl_h

#include "cocos2d.h"
#include "EngineDefine.h"




USING_NS_CC;
using namespace std;
// 读写log开关
//#define show_io_log

#ifdef show_io_log
#define IOLog CCLOG
//#define IOLog printf
#else
#define IOLog 
#endif

// 以下定义引用NetDataDefine.h里的定义如有更新以NetDataDefine.h为准
#ifndef _String_def
#define _String_def
typedef struct _String_
{
	INT16 mlen;//字符串长度，字符串为空时，此处为0
	byte *pbytes;//字节流Bytes 字符串为空时，此处不存在
}String;//字符串
#endif

// 写方法
static u_int16_t Write8(byte **p,byte data)
{
	memcpy(*p,&data,1); (*p) += 1;
    IOLog("Write8: %d", data);
	return(1);
}
static u_int16_t Write16(byte **p,u_int16_t data)
{
    IOLog("Write16: %d", data);
	u_int16_t  ndata;
	ndata = HTONS(data);
	memcpy(*p,&ndata,2); (*p) += 2;
	return(2);
}
static u_int32_t Write32(byte **p,u_int32_t data)
{
    IOLog("Write32: %d", data);
	u_int32_t  ndata;
	ndata = HTONL(data);
	memcpy(*p,&ndata,4); (*p) += 4;
    
	return(4);
}
static u_int32_t Write64(byte **p,u_int64_t data)
{
	u_int64_t  ndata;
	ndata = (((int64_t)htonl((int32_t)((data << 32) >> 32))) << 32) | (u_int32_t)htonl((int32_t)(data >> 32));
	memcpy(*p,&ndata,8); (*p) += 8;
    IOLog("Write64: %d", data);
	return(8);
}
static u_int32_t WriteUTFString(byte **p, string str)
{
    IOLog("WriteUTFString: %s", str.c_str());
    int len = str.length();
    if(len == 0){
        // 避免多余打印调试，正式版可以用一条语句替换
        //Write16(p, len);
        u_int16_t  ndata;
        int len1 = len;
        ndata = HTONS(len1);
        memcpy(*p,&ndata,2); (*p) += 2;
        return(2);
    }else{
        // 避免多余打印调试，正式版可以用一条语句替换
        //Write16(p, len);
        u_int16_t  ndata;
        int len1 = len;
        ndata = HTONS(len1);
        memcpy(*p,&ndata,2); (*p) += 2;
        
        memcpy(*p,str.c_str(),len); (*p) += len;
        return (2 + len);
    }
}

/////////////////////////////////////////////////////
// 读方法
static byte Read8(byte **p)
{
	byte  data;
	memcpy(&data,*p,1);  (*p) ++;
    IOLog("Read8: %d", data);
	return(data);
}
static int16_t Read16(byte **p)
{
	int16_t  data;
	memcpy(&data,*p,2);  (*p) += 2;
    NTOHS(data);
    IOLog("Read16: %d", data);
	return data;
}
static int32_t Read32(byte **p)
{
	int32_t  data;
	memcpy(&data,*p,4);  (*p) += 4;
    NTOHL(data);
    IOLog("Read32: %d", data);
	return data;
}

static int64_t Read64(byte **p)
{
    int64_t  data;
	memcpy(&data,*p,8);  (*p) += 8;
    data = (((int64_t)htonl((int32_t)((data << 32) >> 32))) << 32) | (u_int32_t)htonl((int32_t)(data >> 32));
    IOLog("Read64: %lld", data);
    //return (((int64_t)htonl((int32_t)((data << 32) >> 32))) << 32) | (u_int32_t)htonl((int32_t)(data >> 32));
    return data;
}

static string ReadUTFString(byte **p)
{
    string str;
    // 避免重复打印调试语句
    // 正式版可以用一句代替
    //int16_t len = Read16(p);
    int16_t  data;
	memcpy(&data,*p,2);  (*p) += 2;
    NTOHS(data);
    int16_t len = data;
    byte *pBuf = new byte[len+1];
    memset(pBuf,0x00,sizeof(byte)*(len+1));
    memcpy(pBuf, *p, len); (*p) += len;
    str = (char*)pBuf;
    delete pBuf;
    IOLog("ReadUTFString: %s", (char*)str.c_str());
    return str;
}

static void ReadString(byte **p, String *str)
{
    // 避免重复打印调试语句
    // 正式版可以用一句代替
    //str->mlen = Read16(p);
    int16_t  data;
	memcpy(&data,*p,2);  (*p) += 2;
    NTOHS(data);
    str->mlen = data;
    
    if (str->mlen > 0) {
        str->pbytes = new byte[str->mlen+1];
        if(str->pbytes){
        memset(str->pbytes,0x00,sizeof(byte)*(str->mlen+1));
        memcpy(str->pbytes, *p, str->mlen); (*p) += str->mlen;
        IOLog("ReadString: %s", (char*)str->pbytes);
        }
    }
}


#endif
