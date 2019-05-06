#include "cmsis_os.h"
#include "debug_task.h"
#include "tasks_init.h"
#include "firmware_version.h"
#include "log.h"

/*
* @brief 任务初始化
* @param 无
* @return 无
* @note
*/

void tasks_init(void)
{
    /**************************************************************************/  
    /* 任务消息队列                                                           */
    /**************************************************************************/  

  
    /**************************************************************************/  
    /* 任务创建                                                               */
    /**************************************************************************/  
    /*调试任务*/
    osThreadDef(debug_task, debug_task, osPriorityNormal, 0, 348);
    debug_task_hdl = osThreadCreate(osThread(debug_task), NULL);
    log_assert(debug_task_hdl);

    


    log_warning("firmware version: %s.\r\n",FIRMWARE_VERSION_STR);

}

