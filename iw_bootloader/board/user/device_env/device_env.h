#ifndef  __DEVICE_ENV_H__
#define  __DEVICE_ENV_H__

#ifdef  __cplusplus
extern "C" {
#endif

/********************    配置设备环境开始    **************************************/

#define  BOOTLOADER_BASE_ADDR                  0x00000000UL /*bootloader基地址*/
#define  BOOTLOADER_SIZE_LIMIT                 0xA000       /*bootloader最大容量*/

#define  APPLICATION_SIZE_LIMIT                0x19000      /*application最大容量*/

#define  APPLICATION_BASE_ADDR                 0x0000A000UL /*application基地址*/

#define  APPLICATION_BACKUP_BASE_ADDR          0x00023000UL /*application备份基地址*/

#define  APPLICATION_UPDATE_BASE_ADDR          0x0003C000UL /*application更新基地址*/

#define  DEVICE_MIN_ERASE_SIZE                 128          /*最小擦除单元大小 bytes*/
#define  DEVICE_ADDR_MAP_LIMIT                 0x80000      /*设备最大地址映射*/        
#define  DEVICE_ENV_BASE_ADDR                  0x00055000   /*环境变量基地址*/       
#define  DEVICE_ENV_BACKUP_BASE_ADDR           0x00055080   /*环境变量备份基地址*/           
#define  DEVICE_ENV_SIZE_LIMIT                 (DEVICE_MIN_ERASE_SIZE * 1)   /*环境变量大小*/  


/********************    配置设备环境结束    **************************************/
/*配置校验
*/
#if  (BOOTLOADER_BASE_ADDR + BOOTLOADER_SIZE_LIMIT) > APPLICATION_BASE_ADDR
#error "application base addr too small."
#endif

#if  (APPLICATION_BASE_ADDR + APPLICATION_SIZE_LIMIT) > APPLICATION_BACKUP_BASE_ADDR
#error "application backup base addr too small."
#endif

#if  (APPLICATION_BACKUP_BASE_ADDR + APPLICATION_SIZE_LIMIT) > APPLICATION_UPDATE_BASE_ADDR
#error "application update base addr too small."
#endif

#if  (APPLICATION_UPDATE_BASE_ADDR + APPLICATION_SIZE_LIMIT) > DEVICE_ENV_BASE_ADDR
#error "device env base addr too small."
#endif

#if  (DEVICE_ENV_BASE_ADDR + DEVICE_ENV_SIZE_LIMIT) > DEVICE_ENV_BACKUP_BASE_ADDR
#error "device env backup base addr too small."
#endif

#if  (DEVICE_ENV_BACKUP_BASE_ADDR + DEVICE_ENV_SIZE_LIMIT) > DEVICE_ADDR_MAP_LIMIT
#error "device env backup base addr large than device addr map."
#endif


#ifdef  __cplusplus
    }
#endif

#endif  