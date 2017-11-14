//
//  CFIO.h
//  coslua
//
//  Created by chency on 13-7-1.
//
//

#ifndef coslua_CFIO_h
#define coslua_CFIO_h
#include "platform/CCPlatformMacros.h"
#include <string>
#include "EngineDefine.h"

using namespace std;
class CFIO {
    

public:
    static void Test();
    static const char* GetDocumentDir();
    // 获得写目录的绝对路径
    static string FullPathForFilename(char *pName);
    // 写文件
    static bool FileWrite(const char *pFile, byte *buf, int len);
    // 读文件
    static void FileRead(const char *pFile, byte **buf, int &len);
    static void FileRead(const char *pFile, byte *buf, int len);
    static string MacAddress();
    static string GetBase64Encoding(const char *pchar);

};


#endif
