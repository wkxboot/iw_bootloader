#include "board.h"
#include "device_env.h"
#include "bootloader.h"
#include "firmware_version.h"
#include "log.h"

/*
* @brief 硬件延时 等待日志输出完毕
* @param 无
* @param
* @return  无
* @note
*/

void hal_delay(void)
{
    volatile uint32_t sleep = 40000000;
  
    while (sleep --);
}

/*
* @brief 主任务
* @param 无
* @return 无
* @note
*/
void main(void )
{
    char *value;

    bsp_board_init();
    log_init();
    log_info("bootloader version:%s\r\n",FIRMWARE_VERSION_STR);

    device_env_init();

    /*输出存在的环境变量*/
    value = device_env_get(ENV_BOOTLOADER_FLAG_NAME);
    if (value) {
        log_info("flag=%s\r\n",value);
    }
    value = device_env_get(ENV_BOOTLOADER_APPLICATION_SIZE_NAME);
    if (value) {
        log_info("app_size=%s\r\n",value);
    }
    value = device_env_get(ENV_BOOTLOADER_APPLICATION_MD5_NAME);
    if (value) {
        log_info("app_md5=%s\r\n",value);
    }
    value = device_env_get(ENV_BOOTLOADER_UPDATE_SIZE_NAME);
    if (value) {
        log_info("update_size=%s\r\n",value);
    }
    value = device_env_get(ENV_BOOTLOADER_UPDATE_MD5_NAME);
    if (value) {
        log_info("update_md5=%s\r\n",value);
    }
    value = device_env_get(ENV_BOOTLOADER_BACKUP_SIZE_NAME);
    if (value) {
        log_info("backup_size=%s\r\n",value);
    }
    value = device_env_get(ENV_BOOTLOADER_BACKUP_MD5_NAME);
    if (value) {
        log_info("backup_md5=%s\r\n",value);
    }
    

    /*进入bootloader*/
    hal_delay();

    while (1) {
        bootloader_bootloader();
    }

 
 
}  
  
 