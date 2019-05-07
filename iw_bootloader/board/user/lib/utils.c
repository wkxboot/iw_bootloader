#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include "stdbool.h"
#include "utils.h"

/*
* @brief 数组转成HEX字符串
* @param
* @param
* @return 
* @note
*/
int dump_hex_str(const char *src,char *dst,uint16_t src_len)
 {
    char temp;

    if (src == NULL || dst == NULL) {
        return -1;
    }

    char hex_digital[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    for (uint16_t i = 0; i < src_len; i++){  
        temp = src[i];  
        dst[2 * i] = hex_digital[(temp >> 4) & 0x0f ];  
        dst[2 * i + 1] = hex_digital[temp & 0x0f];  
    }
    dst[src_len * 2] = '\0';

    return 0;
}