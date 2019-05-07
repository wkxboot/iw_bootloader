#ifndef  __UTILS_H__
#define  __UTILS_H__
#include "stdbool.h"
#include "stdint.h"
#include "stdlib.h"

typedef struct
{
    bool up;
    uint32_t start;
    uint32_t value;
}utils_timer_t;


#ifndef   IS_POWER_OF_TWO
#define   IS_POWER_OF_TWO(A)   (((A) != 0) && ((((A) - 1) & (A)) == 0))
#endif

#ifndef   MIN
#define   MIN(A,B)             ((A) > (B) ? (B) :(A))
#endif

#ifndef   MAX
#define   MAX(A,B)             ((A) > (B) ? (A) :(B))
#endif

#ifndef  UTILS_ASSERT
#define  UTILS_ASSERT(x)                            \
do {                                                \
    if ((void *)(x) == (void *)0) {                 \
        while(1);                                   \
    }                                               \
}while (0)                                         
#endif



/*
* @brief 数组转成HEX字符串
* @param
* @param
* @return 
* @note
*/
int dump_hex_str(const char *src,char *dst,uint16_t src_len);
   
   

   
#endif