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
#include "device_env.h"
#include "flash_if.h"
#include "bootloader.h"
#include "md5.h"
#include "utils.h"
#include "log.h"


/*
* @brief 
* @param
* @param
* @return 
* @note
*/
static int bootloader_copy_image(uint32_t from,uint32_t to,uint32_t size)
{
    int rc;
    uint32_t src_addr = from;
    uint32_t dst_addr = to;
    uint32_t write_size,write_total = 0;

    while (write_total < size) {
        write_size = (size - write_total) > BOOTLOADER_PROGRAM_SIZE ?  BOOTLOADER_PROGRAM_SIZE : (size - write_total);
        /*先擦除*/  
        rc = flash_if_erase(dst_addr,write_size);
        if (rc != 0) {
            goto err_handle;
        }
        /*然后编程写入*/
        rc = flash_if_write(dst_addr,(uint8_t*)src_addr,write_size);
        if (rc != 0) {
            goto err_handle;
        }
        log_debug("copy %d bytes from  addr:%d to addr:%d ok \r\n",write_size,src_addr,dst_addr);
        /*更新地址*/
        dst_addr += write_size;
        src_addr += write_size;
        write_total += dst_addr += write_size;;
    }
    return 0;

err_handle:

    return -1;
}

/*
* @brief 
* @param
* @param
* @return 
* @note
*/
static int bootloader_copy_update_image_to_application_region(uint32_t size)
{
    int rc;

    log_debug("start copy %d bytes update image to application region...\r\n",size);
    rc = bootloader_copy_image(APPLICATION_UPDATE_BASE_ADDR,APPLICATION_BASE_ADDR,size);
    if (rc == 0) {
        log_debug("copy update image to application region ok.\r\n");
        return 0;
    }

    log_error("copy update image to application region err.\r\n");
    return -1;


}

/*
* @brief 
* @param
* @param
* @return 
* @note
*/
static int bootloader_copy_application_image_to_backup_region(uint32_t size)
{
    int rc;

    log_debug("start copy %d bytes application image to backup region...\r\n",size);
    rc = bootloader_copy_image(APPLICATION_BASE_ADDR,APPLICATION_BACKUP_BASE_ADDR,size);
    if (rc == 0) {
        log_debug("copy application image to backup region ok.\r\n");
        return 0;
    }

    log_error("copy application image to backup region err.\r\n");
    return -1;
}

/*
* @brief 
* @param
* @param
* @return 
* @note
*/
static int bootloader_copy_backup_image_to_application_region(uint32_t size)
{
    int rc;

    log_debug("start copy %d bytes backup image to application region...\r\n",size);
    rc = bootloader_copy_image(APPLICATION_BACKUP_BASE_ADDR,APPLICATION_BASE_ADDR,size);
    if (rc == 0) {
        log_debug("copy backup image to application region ok.\r\n");
        return 0;
    }

    log_error("copy backup image to application region err.\r\n");
    return -1;
}


/*
* @brief 
* @param
* @param
* @return 
* @note
*/
int bootloader_copy_new_image_to_update_region(uint8_t *src,uint32_t size)
{
    int rc;
    static uint32_t addr = APPLICATION_UPDATE_BASE_ADDR;

    if ((size & (DEVICE_MIN_ERASE_SIZE - 1)) != 0 || size == 0) {
        log_error("err not align with erase size:%d.\r\n",DEVICE_MIN_ERASE_SIZE);
        goto err_handle;
    }
    /*先擦除*/  
    rc = flash_if_erase(addr,size);
    if (rc != 0) {
        goto err_handle;
    }
    /*然后编程写入*/
    rc = flash_if_write(addr,src,size);
    if (rc != 0) {
        goto err_handle;
    }
    /*更新地址*/
    addr += size;
    log_debug("copy %d bytes new image to update region ok \r\n",size);
    return 0;

err_handle:
    log_error("copy new image err.\r\n");
    addr = APPLICATION_UPDATE_BASE_ADDR;
    return -1;
}

/*
* @brief 
* @param
* @param
* @return 
* @note
*/
static int bootloader_first_boot_init()
{
#define  SIZE_STR_BUFFER_SIZE       7
    int rc;

    char md5_str_buffer[33];
    char md5_value[16];
    char size_str_buffer[7];

    /*设置应用程序size默认为最大值*/
    snprintf(size_str_buffer,SIZE_STR_BUFFER_SIZE,"%d",APPLICATION_SIZE_LIMIT);
    rc = device_env_set(ENV_BOOTLOADER_APPLICATION_SIZE_NAME,size_str_buffer);
    if (rc != 0) {
        log_error("err in set application size.\r\n");
        goto err_handle;
    }
    /*计算并设置MD5*/
    md5((char*)APPLICATION_BASE_ADDR,APPLICATION_SIZE_LIMIT,md5_value);
    bytes_to_hex_str(md5_value,md5_str_buffer,16);
        
    rc = device_env_set(ENV_BOOTLOADER_APPLICATION_MD5_NAME,md5_str_buffer);
    if (rc != 0) {
        log_error("err in set application md5.\r\n");
        goto err_handle;
    }
    
    /*标记状态为正常模式*/
    rc = device_env_set(ENV_BOOTLOADER_FLAG_NAME,ENV_BOOTLOADER_NORMAL);
    if (rc != 0) {
        log_error("err in set bootloader flag.\r\n");
        goto err_handle;
    }
    log_debug("first boot init ok.\r\n");
    return 0;

err_handle:
    log_error("first boot init err.\r\n");
    return -1;
}
   
  
/*
* @brief 
* @param
* @param
* @return 
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

    size_str = device_env_get(ENV_BOOTLOADER_APPLICATION_SIZE_NAME);
    md5_str = device_env_get(ENV_BOOTLOADER_APPLICATION_MD5_NAME);
    if (size_str == NULL || md5_str == NULL) {
        log_error("app size or md5 is null.\r\n");
        goto err_handle;
    }
    size = atoi(size_str);        
    
    rc = bootloader_copy_application_image_to_backup_region(size);
    if (rc != 0) {
        goto err_handle;
    }
    /*校验拷贝后的MD5*/
    md5((char*)APPLICATION_BACKUP_BASE_ADDR,size,md5_value);
    bytes_to_hex_str(md5_value,md5_str_buffer,16);

    if (strcmp(md5_str,md5_str_buffer) != 0) {
        log_error("backup md5 err.cal:%s config:%s.\r\n",md5_str_buffer,md5_str);
        goto err_handle;
    }
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
* @brief 
* @param
* @param
* @return 
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

    size_str = device_env_get(ENV_BOOTLOADER_BACKUP_SIZE_NAME);
    md5_str = device_env_get(ENV_BOOTLOADER_BACKUP_MD5_NAME);
    if (size_str == NULL || md5_str == NULL) {
        log_error("backup size or md5 is null.\r\n");
        goto err_handle;
    }
    size = atoi(size_str);        
    
    rc = bootloader_copy_backup_image_to_application_region(size);
    if (rc != 0) {
        goto err_handle;
    }
    /*校验拷贝后的MD5*/
    md5((char*)APPLICATION_BASE_ADDR,size,md5_value);
    bytes_to_hex_str(md5_value,md5_str_buffer,16);

    if (strcmp(md5_str,md5_str_buffer) != 0) {
        log_error("application md5 err.cal:%s config:%s.\r\n",md5_str_buffer,md5_str);
        goto err_handle;
    }
    log_debug("app md5 ok.\r\n");
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
* @brief 
* @param
* @param
* @return 
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

    size_str = device_env_get(ENV_BOOTLOADER_UPDATE_SIZE_NAME);
    md5_str = device_env_get(ENV_BOOTLOADER_UPDATE_MD5_NAME);
    if (size_str == NULL || md5_str == NULL) {
        log_error("update size or md5 is null.\r\n");
        goto err_handle;
    }
    size = atoi(size_str);        

    /*校验更新区的MD5*/
    md5((char*)APPLICATION_UPDATE_BASE_ADDR,size,md5_value);
    bytes_to_hex_str(md5_value,md5_str_buffer,16);

    if (strcmp(md5_str,md5_str_buffer) != 0) {
        log_error("update md5 err.cal:%s config:%s.\r\n",md5_str_buffer,md5_str);
        goto err_handle;
    }
    log_debug("update md5 ok.\r\n");

    /*从更新区拷贝到应用区*/
    rc = bootloader_copy_backup_image_to_application_region(size);
    if (rc != 0) {
        goto err_handle;
    }

    /*再次校验应用区数据*/
    size_str = device_env_get(ENV_BOOTLOADER_APPLICATION_SIZE_NAME);
    md5_str = device_env_get(ENV_BOOTLOADER_APPLICATION_MD5_NAME);
    if (size_str == NULL || md5_str == NULL) {
        log_error("app size or md5 is null.\r\n");
        goto err_handle;
    }
    size = atoi(size_str);        
    /*MD5*/
    md5((char*)APPLICATION_BASE_ADDR,size,md5_value);
    bytes_to_hex_str(md5_value,md5_str_buffer,16);

    if (strcmp(md5_str,md5_str_buffer) != 0) {
        log_error("app md5 err.cal:%s config:%s.\r\n",md5_str_buffer,md5_str);
        goto err_handle;
    }
    log_debug("update md5 ok.\r\n");

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
* @brief 
* @param
* @param
* @return 
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
        log_debug("first boot.\r\n");
        rc = bootloader_first_boot_init();

    /*正常模式 跳转到应用程序*/
    } else if (strcmp(flag,ENV_BOOTLOADER_NORMAL) == 0) {  
        bootloader_jump_to_application();
        
    /*有更新标志，启动更新*/
    } else if (strcmp(flag,ENV_BOOTLOADER_UPDATE) == 0) { 
        rc = bootloader_backup();
        
        rc = bootloader_update();
        
    /*有更新完成标志 证明应用程序没有升级成功*/
    } else if (strcmp(flag,ENV_BOOTLOADER_COMPLETE) == 0) {  
        rc = bootloader_recovery();

    /*有OK标志 证明应用程序升级成功 置为正常模式*/
    } else if (strcmp(flag,ENV_BOOTLOADER_OK) == 0) {  
        rc = bootloader_recovery();
    }


}


