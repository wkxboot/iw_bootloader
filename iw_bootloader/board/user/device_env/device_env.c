#include "device_env.h"
#include "flash_if.h"
#include "crc16.h"
#include "log.h"

/*****************************************************************************
*  设备环境                                                          
*  Copyright (C) 2019 wkxboot 1131204425@qq.com.                             
*                                                                            
*                                                                            
*  This program is free software; you can redistribute it and/or modify      
*  it under the terms of the GNU General Public License version 3 as         
*  published by the Free Software Foundation.                                
*                                                                            
*  @file     device_env.c                                                   
*  @brief    设备环境                                                                                                                                                                                             
*  @author   wkxboot                                                      
*  @email    1131204425@qq.com                                              
*  @version  v1.0.0                                                  
*  @date     2019/5/5                                            
*  @license  GNU General Public License (GPL)                                
*                                                                            
*                                                                            
*****************************************************************************/
#define  DEVICE_STATUS_VALID          0xA5A5
#define  DEVICE_STATUS_INVALID        0x5A5A

typedef struct
{
    uint16_t crc;
    uint16_t status;
    uint8_t  env[DEVICE_ENV_SIZE_LIMIT - 4];
}device_env_t;

static device_env_t device_env;

/*
* @brief 
* @param
* @param
* @return 
* @note
*/
static char device_env_get_char(int index) 
{
    return *((char *)(&device_env.env[0] + index));
}

/*
* @brief 
* @param
* @param
* @return 
* @note
*/
static char * device_env_get_addr(int index)
{
    return (char *)(&device_env.env[0] + index);
}

/*
* @brief 
* @param
* @param
* @return 
* @note
*/
static int device_env_match(char *name,int index2) 
{
    while (*name == device_env_get_char(index2 ++)) {
        if (*name ++ == '=') {
            return (index2);
        }
    }
    if (*name == '\0' && device_env_get_char(index2 - 1) == '=') {
        return (index2);
    }

    return (-1);
}

/*
* @brief 
* @param
* @param
* @return 
* @note
*/
static void device_env_crc_update() 
{
    device_env.crc = calculate_crc16(device_env.env,DEVICE_ENV_SIZE_LIMIT - 4);
}

/*
* @brief 
* @param
* @param
* @return 
* @note
*/
int device_env_init(void) 
{
    int rc;
    uint16_t crc;
    /*读取设备环境变量*/
    rc = flash_if_read(DEVICE_ENV_BASE_ADDR,(uint8_t*)&device_env,sizeof(device_env_t));
    if (rc != 0) {
        log_error("read env err .code:%d.\r\n",rc);
        device_env.status = DEVICE_STATUS_INVALID;
        return -1;
    }
    /*对比校验值*/
    crc = device_env_crc(&device_env.env[0],DEVICE_ENV_SIZE_LIMIT - 4);
    if (crc != device_env.crc) {
        log_warning("crc bad. ");
        memset(&device_env,0xFF,sizeof(device_env_t));
    } else {
        log_warning("crc ok. ");
    }
    device_env.status = DEVICE_STATUS_VALID;
    return 0;
}

/*
* @brief 
* @param
* @param
* @return 
* @note
*/
char *device_env_read(char *name) 
{
    int i, next;
    for (i=0; device_env_get_char(i) != '\0'; i = next + 1) {
        int val;
        for (next = i; device_env_get_char(next) != '\0'; ++ next) {
            if (next >= DEVICE_ENV_SIZE_LIMIT) {
                return (NULL);
            }
        }
        if ((val = device_env_match((char *)name, i)) < 0) {
            continue;
        }

        return ((char *)device_env_get_addr(val));
    }

    return (NULL);
}

/*
* @brief 
* @param
* @param
* @return 
* @note
*/
int device_env_save(char *name,char *value) 
{
    int   i, len, oldval;

    char *env, *next = NULL;

    char *env_data = device_env_get_addr(0);

    if (strchr(name, '=')) {
        log_error("## Error: illegal character '=' in variable name \"%s\"\n", name);
        return -1;
    }

    /*
     * search if variable with this name already exists
     */
    oldval = -1;
    for ( env = env_data; *env; env = next + 1) {
        for (next = env; *next; ++ next);
        if ((oldval = device_env_match((char *)name, env - env_data)) >= 0)
            break;
    }

    /*
     * Delete any existing definition
     */
    if (oldval >= 0) {      
        if (*++next == '\0') {
            if (env > env_data) {
                env--;
            } else {
                *env = '\0';
            }
        } else {
            while(1) {
                *env = *next++;
                if ((*env == '\0') && (*next == '\0')) {
                    break;
                }
                ++env;
            }
        }
        *++env = '\0';
    }
    /* Delete only ? */
    if (value == NULL) {
        device_env_crc_update ();
        return 0;
    }

    /*
     * Append new definition at the end
     */
    for (env = env_data; *env || *(env + 1); ++ env);

    if (env > env_data) {
        ++ env;
    }
    /*
     * Overflow when:
     * "name" + "=" + "val" +"\0\0"  > ENV_SIZE - (env-env_data)
     */
    len = strlen(name) + 2;
    /* add '=' for first arg, ' ' for all others */
    len += strlen(value) + 1;

    if (len > (&env_data[DEVICE_ENV_SIZE_LIMIT - 4] - env)) {
        log_error("## Error: environment overflow, \"%s\" deleted\n", name);
        return 1;
    }
    while ((*env = *name++) != '\0') {
        env++;
    }


    char *val = value;

    *env = '=';
    while ((*++env = *val++) != '\0');
   

    /* end is marked with double '\0' */
    *++env = '\0';

    /* Update CRC */
    device_env_crc_update();
    return 0;
}
