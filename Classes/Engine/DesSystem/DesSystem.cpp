//
//  DesSystem.cpp
//  cardgame
//
//  Created by yuanlu on 14-2-25.
//
//

#include "DesSystem.h"

NS_ENGINE_BEGIN

DesSystem* DesSystem::SharedSceneSystem()
{
    static DesSystem instance;
    return &instance;
}

DesSystem::DesSystem()
{
    
}

DesSystem::~DesSystem()
{
    
}

void DesSystem::SetKey(const char key[8])
{
    bool k[64], *pkl=&k[0], *pkr=&k[28];
    Bytetobit(k,key,64);
    Transform(k,k,des_pc1_table,56);
    for(int i=0; i<16; i++)
    {
        Rotatel(pkl,28,des_loop_table[i]);
        Rotatel(pkr,28,des_loop_table[i]);
        Transform(m_des_subkey[i],k,des_pc2_table,48);
    }
}

void DesSystem::Run(char *pout, char *pin, int len, DES_TYPE mtype)
{
#ifdef DES_RUNTIME_SPRINTF
    struct cc_timeval time_start;
    CCTime::gettimeofdayCocos2d(&time_start, NULL);
#endif
    
    if(pout && pin && len>0)
    {
        int mcout = (len/8)+1;
        int mtplen = mcout*8;
        char *ptpOut = new char[mtplen];
        char *ptpIn = new char[mtplen];
        if(ptpOut && ptpIn)
        {
            memset(ptpOut,0x00,sizeof(char)*mtplen);
            memset(ptpIn,0x00,sizeof(char)*mtplen);
            memcpy(ptpIn, pin, len);
            int mpos = 0;
            for(int i=0; i<mcout; i++)
            {
                DesRun(&ptpOut[mpos],&ptpIn[mpos],mtype);
                mpos += 8;
            }
            memcpy(pout, ptpOut, len);
        }
        if(ptpOut)
        {
            delete [] ptpOut;
            ptpOut = NULL;
        }
        if(ptpIn)
        {
            delete [] ptpIn;
            ptpIn = NULL;
        }
    }
#ifdef DES_RUNTIME_SPRINTF
    struct cc_timeval time_end;
    CCTime::gettimeofdayCocos2d(&time_end, NULL);
    CCLOG("DesRun_%scry time:%fs\n", mtype==DES_ENCRYPT ? "En":"De",
          time_end.tv_sec-time_start.tv_sec+(time_end.tv_usec-time_start.tv_usec)/1000000.0000f);
#endif
}

void DesSystem::Ffunc(bool in[32],const bool ki[48])
{
    bool mr[48];
    Transform(mr,in,des_e_table,48);
    Xor(mr,ki,48);
    Sfunc(in,mr);
    Transform(in,in,des_p_table,32);
}

void DesSystem::Sfunc(bool out[32],const bool in[48])
{
    for(char i=0,j,k;i<8;i++,in+=6,out+=4)
    {
        j=(in[0]<<1)+in[5];
        k=(in[1]<<3)+(in[2]<<2)+(in[3]<<1)+in[4];
        Bytetobit(out,&des_s_box[i][j][k],4);
    }
}

void DesSystem::Transform(bool *out, bool *in, const char *table, int len)
{
    if(out && in && table)
    {
        bool tmp[256];
        for(int i=0;i<len;i++)
            tmp[i]=in[table[i]-1];
        memcpy(out,tmp,len);
    }
}

void DesSystem::Xor(bool *ina, const bool *inb, int len)
{
    if(ina && inb)
    {
        for(int i=0;i<len;i++)
            ina[i]^=inb[i];
    }
}

void DesSystem::Rotatel(bool *in, int len, int loop)
{
    if(in)
    {
        bool tmp[256];
        memcpy(tmp,in,loop);
        memcpy(in,in+loop,len-loop);
        memcpy(in+len-loop,tmp,loop);
    }
}

void DesSystem::Bytetobit(bool *out,const char *in, int bits)
{
    if(out && in)
    {
        for(int i=0;i<bits;i++)
            out[i]=(in[i/8]>>(i%8)) & 1;
    }
}

void DesSystem::Bittobyte(char *out, const bool *in, int bits)
{
    if(out && in)
    {
        memset(out,0,(bits+7)/8);
        for(int i=0;i<bits;i++)
            out[i/8]|=in[i]<<(i%8);
    }
}

void DesSystem::DesRun(char out[8],char in[8],DES_TYPE mtype)
{
    bool m[64],tmp[32],*pli=&m[0], *pri=&m[32];
    Bytetobit(m,in,64);
    Transform(m,m,des_ip_table,64);
    if(mtype==DES_ENCRYPT)
    {
        for(int i=0; i<16; i++)
        {
            memcpy(tmp,pri,32);
            Ffunc(pri,m_des_subkey[i]);
            Xor(pri,pli,32);
            memcpy(pli,tmp,32);
        }
    }
    else
    {
        for(int i=15; i>=0; i--)
        {
            memcpy(tmp,pli,32);
            Ffunc(pli,m_des_subkey[i]);
            Xor(pli,pri,32);
            memcpy(pri,tmp,32);
        }
    }
    Transform(m,m,des_ipr_table,64);
    Bittobyte(out,m,64);
}

const char* DesSystem::GetDesPwdByVersion(int ver, bool bkey2)
{
    int mcount  = sizeof(mDBDesList)/sizeof(DB_DES_INFO);
    for(int i=0; i<mcount; i++)
    {
        if(mDBDesList[i].mver == ver)
            return bkey2 ? &mDBDesList[i].mkey[8]:&mDBDesList[i].mkey[0];
    }
    return "";
}

NS_ENGINE_END