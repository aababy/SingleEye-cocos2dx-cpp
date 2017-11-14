//
//  EngineDefine.h
//  coslua
//
//  Created by yuanlu on 13-5-16.
//
//

#ifndef EngineDefine_h
#define EngineDefine_h

#ifdef __cplusplus
#define NS_ENGINE_BEGIN     namespace engine {
#define NS_ENGINE_END       }
#define USING_NS_ENGINE     using namespace engine
#else
#define NS_ENGINE_BEGIN
#define NS_ENGINE_END
#define USING_NS_ENGINE
#endif

#define ENGINE_SAFE_DEL(obj) if(obj){delete obj; obj=NULL;}
#define ENGINE_SAFE_DELPOOL(obj) if(obj){delete []obj; obj=NULL;}

#ifndef _BYTE_
#define _BYTE_
typedef signed char   byte;
#endif /*_BYTE_ */

#ifndef _INT8_
#define _INT8_
typedef signed char INT8;
#endif /*_INT8_ */

#ifndef _INT16_
#define _INT16_
typedef short       INT16;
#endif /* _INT16_ */

#ifndef _INT32_
#define _INT32_
typedef int         INT32;
#endif /* _INT32_ */

#ifndef _INT64_
#define _INT64_
typedef long long   INT64;
#endif /* _INT64_ */

#ifndef _UINT8_
#define _UINT8_
typedef unsigned char   UINT8;
#endif /*_UINT8_ */

#ifndef _UINT16_
#define _UINT16_
typedef unsigned short  UINT16;
#endif /* _UINT16_ */

#ifndef _UINT32_
#define _UINT32_
typedef unsigned int    UINT32;
#endif /* _UINT32_ */

#ifndef _UINT64_
#define _UINT64_
typedef unsigned long long   UINT64;
#endif/* _UINT64_ */

#endif
