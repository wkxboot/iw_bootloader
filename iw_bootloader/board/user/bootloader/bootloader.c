/*****************************************************************************
*  通用bootloader                                                          
*  Copyright (C) 2019 wkxboot 1131204425@qq.com.                             
*                                                                            
*                                                                            
*  This program is free software; you can redistribute it and/or modify      
*  it under the terms of the GNU General Public License version 3 as         
*  published by the Free Software Foundation.                                
*                                                                            
*  @file     bootloader.c                                                   
*  @brief    通用bootloader                                                                                                                                                                                             
*  @author   wkxboot                                                      
*  @email    1131204425@qq.com                                              
*  @version  v1.0.0                                                  
*  @date     2019/5/6                                            
*  @license  GNU General Public License (GPL)                                
*                                                                            
*                                                                            
*****************************************************************************/
#include "fsl_common.h"
#include "device_env.h"
#include "flash_if.h"
#include "bootloader.h"
#include "md5.h"
#include "utils.h"
#include "log.h"

/*内部静态写缓存*/
static uint8_t write_buffer[BOOTLOADER_PROGRAM_SIZE];


/*
* @brief 从flash地址src处拷贝size个字节到flash地址dst处
* @param from flash源地址
* @param from flash目的地址
* @param size flash拷贝数量
* @return 0：成功 -1：失败
* @note 先从flash读到ram中再写入flash
*/
static int bootloader_copy_image(const uint32_t from,const uint32_t to,const uint32_t size)
{
    int rc;
    uint32_t src_addr = from;
    uint32_t dst_addr = to;
    uint32_t write_total = 0;

    /*擦除目的数据区*/
    rc = flash_if_erase(dst_addr,size);
    if (rc != 0) {
        goto err_handle;
    }

    /*多次写入*/
    while (write_total < size) {
        /*先读取*/  
        rc = flash_if_read(src_addr,write_buffer,BOOTLOADER_PROGRAM_SIZE);
        if (rc != 0) {
            goto err_handle;
        }
        /*然后编程写入*/
        rc = flash_if_write(dst_addr,write_buffer,BOOTLOADER_PROGRAM_SIZE);
        if (rc != 0) {
            goto err_handle;
        }

        log_debug("copy %d bytes from  addr:%d to addr:%d ok \r\n",BOOTLOADER_PROGRAM_SIZE,src_addr,dst_addr);
        /*更新地址*/
        dst_addr += BOOTLOADER_PROGRAM_SIZE;
        src_addr += BOOTLOADER_PROGRAM_SIZE;
        write_total += BOOTLOADER_PROGRAM_SIZE;;
    }
    return 0;

err_handle:

    return -1;
}


/*
* @brief bootloader在应用程序第一次启动后进行的初始化
* @param 无
* @param
* @return 0：成功 -1：失败
* @note
*/
static int bootloader_first_boot_init(void)
{
#define  SIZE_STR_BUFFER_SIZE       7
    int rc;

    char md5_str_buffer[33];
    char md5_value[16];
    char size_str_buffer[7];

    log_debug("start first boot init...\r\n");
    /*设置应用程序size默认为最大值*/
    snprintf(size_str_buffer,SIZE_STR_BUFFER_SIZE,"%d",APPLICATION_SIZE_LIMIT);
    rc = device_env_set(ENV_BOOTLOADER_APPLICATION_SIZE_NAME,size_str_buffer);
    if (rc != 0) {
        log_error("err in set application size.\r\n");
        goto err_handle;
    }
    /*计算MD5*/
    log_debug("calculate app md5...\r\n");
    md5((char*)APPLICATION_BASE_ADDR,APPLICATION_SIZE_LIMIT,md5_value);
    dump_hex_str(md5_value,md5_str_buffer,16);
    /*设置MD5*/    
    rc = device_env_set(ENV_BOOTLOADER_APPLICATION_MD5_NAME,md5_str_buffer);
    if (rc != 0) {
        log_error("err in set application md5.\r\n");
        goto err_handle;
    }
    
    log_debug("first boot init ok.\r\n");
    return 0;

err_handle:
    log_error("first boot init err.\r\n");
    return -1;
}
   
  
/*
* @brief 备份数据
* @param
* @param
* @return 0：成功 -1：失败
* @note
*/
static int bootloader_backup()
{
    int rc;
    int size;
    
    char *size_str;
    char *md5_str;
    char md5_str_buffer[33];
    char md5_value[16];

    log_debug("start backup...\r\n");
    /*读取当前应用的size和MD5*/
    size_str = device_env_get(ENV_BOOTLOADER_APPLICATION_SIZE_NAME);
    md5_str = device_env_get(ENV_BOOTLOADER_APPLICATION_MD5_NAME);
    if (size_str == NULL || md5_str == NULL) {
        log_error("app size or md5 is null.\r\n");
        goto err_handle;
    }
    size = atoi(size_str);        
    
    log_debug("start copy %d bytes application image to backup region...\r\n",size);
    rc = bootloader_copy_image(APPLICATION_BASE_ADDR,APPLICATION_BACKUP_BASE_ADDR,size);
    if (rc != 0) {
        log_error("copy application image to backup region err.\r\n");
        goto err_handle;
    }
    log_debug("copy application image to backup region ok.\r\n");
    /*校验拷贝后的MD5*/
    md5((char*)APPLICATION_BACKUP_BASE_ADDR,size,md5_value);
    dump_hex_str(md5_value,md5_str_buffer,16);

    if (strcmp(md5_str,md5_str_buffer) != 0) {
        log_error("backup md5 err.cal:%s config:%s.\r\n",md5_str_buffer,md5_str);
        goto err_handle;
    }
    /*设置备份区size和MD5*/
    log_debug("app backup md5 ok.\r\n");
    rc = device_env_set(ENV_BOOTLOADER_BACKUP_SIZE_NAME,size_str);
    if (rc != 0) {
        goto err_handle;
    }
    rc = device_env_set(ENV_BOOTLOADER_BACKUP_MD5_NAME,md5_str);
    if (rc != 0) {
        goto err_handle;
    }
    log_error("app backup ok.\r\n");
    return 0;

err_handle:
    log_error("app backup err.\r\n");
    return -1;
}

/*
* @brief 恢复数据
* @param
* @param
* @return 0：成功 -1：失败
* @note
*/
static int bootloader_recovery()
{
    int rc;
    int size;
    
    char *size_str;
    char *md5_str;
    char md5_str_buffer[33];
    char md5_value[16];

    log_debug("start recovery...\r\n");
    /*读取备份应用的size和MD5*/
    size_str = device_env_get(ENV_BOOTLOADER_BACKUP_SIZE_NAME);
    md5_str = device_env_get(ENV_BOOTLOADER_BACKUP_MD5_NAME);
    if (size_str == NULL || md5_str == NULL) {
        log_error("backup size or md5 is null.\r\n");
        goto err_handle;
    }
    size = atoi(size_str);        
    
    log_debug("start copy %d bytes backup image to application region...\r\n",size);
    rc = bootloader_copy_image(APPLICATION_BACKUP_BASE_ADDR,APPLICATION_BASE_ADDR,size);
    if (rc != 0) {
        log_error("copy backup image to application region err.\r\n");
        goto err_handle;
    }
    log_debug("copy backup image to application region ok.\r\n");

    /*校验拷贝后的MD5*/
    md5((char*)APPLICATION_BASE_ADDR,size,md5_value);
    dump_hex_str(md5_value,md5_str_buffer,16);

    if (strcmp(md5_str,md5_str_buffer) != 0) {
        log_error("application md5 err.cal:%s config:%s.\r\n",md5_str_buffer,md5_str);
        goto err_handle;
    }
    log_debug("app md5 ok.\r\n");
    /*设置当前应用的size和MD5*/
    rc = device_env_set(ENV_BOOTLOADER_APPLICATION_SIZE_NAME,size_str);
    if (rc != 0) {
        goto err_handle;
    }
    rc = device_env_set(ENV_BOOTLOADER_APPLICATION_MD5_NAME,md5_str);
    if (rc != 0) {
        goto err_handle;
    }
    log_error("app recovery ok.\r\n");
    return 0;

err_handle:
    log_error("app recovery err.\r\n");
    return -1;
}

/*
* @brief 更新数据
* @param
* @param
* @return 0：成功 -1：失败
* @note
*/
static int bootloader_update()
{
    int rc;
    int size;
    
    char *size_str;
    char *md5_str;
    char md5_str_buffer[33];
    char md5_value[16];

    log_debug("start update...\r\n");
    /*读取更新区的size和MD5*/
    size_str = device_env_get(ENV_BOOTLOADER_UPDATE_SIZE_NAME);
    md5_str = device_env_get(ENV_BOOTLOADER_UPDATE_MD5_NAME);
    if (size_str == NULL || md5_str == NULL) {
        log_error("update size or md5 is null.\r\n");
        goto err_handle;
    }
    size = atoi(size_str);        

    /*校验更新区的MD5*/
    md5((char*)APPLICATION_UPDATE_BASE_ADDR,size,md5_value);
    dump_hex_str(md5_value,md5_str_buffer,16);

    if (strcmp(md5_str,md5_str_buffer) != 0) {
        log_error("update md5 err.cal:%s config:%s.\r\n",md5_str_buffer,md5_str);
        goto err_handle;
    }
    log_debug("update md5 ok.\r\n");

    /*从更新区拷贝到应用区*/
    log_debug("start copy %d bytes update image to application region...\r\n",size);
    rc = bootloader_copy_image(APPLICATION_UPDATE_BASE_ADDR,APPLICATION_BASE_ADDR,size);

    if (rc != 0) {
        log_error("copy update image to application region err.\r\n");
        goto err_handle;
    }
    log_debug("copy update image to application region ok.\r\n");

    /*校验应用区*/   
    /*MD5*/
    md5((char*)APPLICATION_BASE_ADDR,size,md5_value);
    dump_hex_str(md5_value,md5_str_buffer,16);

    if (strcmp(md5_str,md5_str_buffer) != 0) {
        log_error("apfter. app md5 err.cal:%s config:%s.\r\n",md5_str_buffer,md5_str);
        goto err_handle;
    }
    log_debug("copy app md5 ok.\r\n");
    /*设置应用的size和MD5*/
    rc = device_env_set(ENV_BOOTLOADER_APPLICATION_SIZE_NAME,size_str);
    if (rc != 0) {
        goto err_handle;
    }
    rc = device_env_set(ENV_BOOTLOADER_APPLICATION_MD5_NAME,md5_str);
    if (rc != 0) {
        goto err_handle;
    }
    log_error("app update ok.\r\n");
    return 0;

err_handle:
    log_error("app update err.\r\n");
    return -1;
}
  
/*
* @brief 重新启动
* @param
* @param
* @return 无
* @note
*/
static void bootloader_reboot(void)
{
    volatile uint32_t delay = 50000000;

    log_warning("reboot...\r\n");
    /*等待日志输出完毕*/
    while(delay-- );

    __NVIC_SystemReset();
}

typedef void (*application_t)(void);   
/*
* @brief 跳转到应用程序
* @param
* @param 无
* @return 无
* @note
*/
static void bootloader_jump_to_application(void)
{
    volatile uint32_t delay = 50000000;

    application_t application;
    uint32_t user_application_msp;
    uint32_t user_app_addr;

    /*初始化栈指针*/
    user_application_msp = *(uint32_t*)(APPLICATION_BASE_ADDR);
    /*获取用户APP地址*/
    user_app_addr = *(uint32_t*)(APPLICATION_BASE_ADDR + 4);

    log_warning("boot user app --> addr:0x%X stack:0x%X....\r\n",user_app_addr,user_application_msp);

    /*等待日志输出完毕*/
    while(delay -- );
    application = (application_t)user_app_addr;
   /*跳转*/
    __disable_irq();
    __set_MSP(user_application_msp);
    application(); 
    while(1);

}  

/*
* @brief bootloader主程序
* @param
* @param 无
* @return 0：成功 -1：失败
* @note
*/
int bootloader_bootloader(void)
{
    int rc;
    char *flag;

    /*读取bootloader标志*/
    flag = device_env_get(ENV_BOOTLOADER_FLAG_NAME);
    /*无参数 第一次启动*/
    if (flag == NULL) {
        log_debug("application not boot ever.\r\n");
        /*跳转到应用开始执行*/
        bootloader_jump_to_application();
    } else if (strcmp(flag,ENV_BOOTLOADER_INIT) == 0) {  
        /*初始模式*/
        rc = bootloader_first_boot_init();
        if (rc != 0) {
            /*重启重新初始化*/
            bootloader_reboot(); 
        } else {
            /*标记状态为正常模式*/
            log_debug("set flag normal...\r\n");
            device_env_set(ENV_BOOTLOADER_FLAG_NAME,ENV_BOOTLOADER_NORMAL);
            /*跳转到应用开始执行*/
            bootloader_jump_to_application();
        }
    /*NORMAL正常模式 跳转到应用程序*/
    } else if (strcmp(flag,ENV_BOOTLOADER_NORMAL) == 0) {  
        /*跳转到应用开始执行*/
        bootloader_jump_to_application();
        
    /*NEW标志，启动更新*/
    } else if (strcmp(flag,ENV_BOOTLOADER_NEW) == 0) { 
        rc = bootloader_backup();
        if (rc != 0) {
            /*备份失败 强制设置为正常状态*/
            log_debug("set flag normal...\r\n");
            device_env_set(ENV_BOOTLOADER_FLAG_NAME,ENV_BOOTLOADER_NORMAL);
        } else {
            /*备份成功 设置为UPDATE模式*/
            log_debug("set flag update...\r\n");
            device_env_set(ENV_BOOTLOADER_FLAG_NAME,ENV_BOOTLOADER_UPDATE);
            /*重启进入UPDATE模式*/
            bootloader_reboot();
        }

    /*UPDATE标志,拷贝更新数据到应用区*/
    } else if (strcmp(flag,ENV_BOOTLOADER_UPDATE) == 0) { 
        /*覆盖新数据*/
        rc = bootloader_update();
        /*不论是否升级成功 设置为COMPLETE模式,可以随时恢复*/
        log_debug("set flag complete...\r\n");
        device_env_set(ENV_BOOTLOADER_FLAG_NAME,ENV_BOOTLOADER_COMPLETE);
        if (rc != 0 ) {
            /*升级失败 重启进入恢复模式*/
            bootloader_reboot();    
        } else {
            /*跳转到应用开始执行*/
            bootloader_jump_to_application();
        }
        
    /*CPOMPLETE标志 证明应用程序不是我们想要的或者意外断电，进入恢复模式*/
    } else if (strcmp(flag,ENV_BOOTLOADER_COMPLETE) == 0) {  
        rc = bootloader_recovery();
        if (rc != 0) {
            /*重启再次尝试*/
            bootloader_reboot();   
        } else {
            /*恢复成功 设置为NORMAL模式*/
            log_debug("set flag normal...\r\n");
            device_env_set(ENV_BOOTLOADER_FLAG_NAME,ENV_BOOTLOADER_NORMAL);
            /*跳转到应用开始执行*/
            bootloader_jump_to_application();
        }

    /*OK标志 证明应用程序升级成功 置为正常模式*/
    } else if (strcmp(flag,ENV_BOOTLOADER_OK) == 0) {  
            /*被升级后的应用置为OK，升级真正成功。 设置为NORMAL模式*/
            log_debug("set flag normal...\r\n");
            device_env_set(ENV_BOOTLOADER_FLAG_NAME,ENV_BOOTLOADER_NORMAL);
            /*跳转到应用开始执行*/
            bootloader_jump_to_application();
    }

    return -1;
}


