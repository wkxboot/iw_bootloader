#include "board.h"
#include "cmsis_os.h"
#include "cpu_utils.h"
#include "debug_task.h"
#include "device_env.h"
#include "log.h"

osThreadId   debug_task_hdl;

/*
* @brief 调试任务
* @param argument 任务参数
* @param
* @return 无
* @note
*/
void debug_task(void const * argument)
{
    int rc;
    char cmd[20];
    uint8_t level;
    uint8_t read_cnt;
    char *read;
    device_env_init();

    while (1) {
        osDelay(DEBUG_TASK_INTERVAL); 
   
        read_cnt = log_read(cmd,19);
        cmd[read_cnt] = 0;

        /*设置日志输出等级*/
        if (strncmp(cmd,"set level ",strlen("set level ")) == 0) {
            level = atoi(cmd + strlen("set level "));
            log_set_level(level);
        }
        /*查询cpu使用率*/
        if (strncmp(cmd,"cpu",strlen("cpu")) == 0) {
           log_info("cpu:%d%%.",osGetCPUUsage());
        }
        /*读*/
        if (strncmp(cmd,"read",strlen("read")) == 0) {
           read = device_env_get("ip_addr");
           if (read) {
              log_debug("ip_addr=%s.\r\n",read);
           } else {
              log_debug("ip_addr is null.\r\n");
           }
        }

        /*写*/
        if (strncmp(cmd,"write",strlen("write")) == 0) {
           rc = device_env_set("ip_addr",&cmd[6]);
           if (rc == 0) {
              log_debug("write ok.\r\n");
           } else {
              log_error("write err.\r\n");
           }
        }
        /*读*/
        if (strncmp(cmd,"date",strlen("date")) == 0) {
           read = device_env_get("date");
           if (read) {
              log_debug("date=%s.\r\n",read);
           } else {
              log_debug("date is null.\r\n");
           }
        }

        /*写*/
        if (strncmp(cmd,"set",strlen("set")) == 0) {
           rc = device_env_set("date",&cmd[4]);
           if (rc == 0) {
              log_debug("date ok.\r\n");
           } else {
              log_error("date err.\r\n");
           }
        }

 
 
    }
}  
  
 