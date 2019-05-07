#ifndef  __BOOTLOADER_H__
#define  __BOOTLOADER_H__

#ifdef  __cplusplus
extern "C" {
#endif

#define  BOOTLOADER_PROGRAM_SIZE                             1024


/*
* @brief 
* @param
* @param
* @return 
* @note
*/
int bootloader_bootloader(void);

/*
* @brief 
* @param
* @param
* @return 
* @note
*/
int bootloader_copy_new_image_to_update_region(uint8_t *src,uint32_t size);



#ifdef  __cplusplus
    }
#endif

#endif  