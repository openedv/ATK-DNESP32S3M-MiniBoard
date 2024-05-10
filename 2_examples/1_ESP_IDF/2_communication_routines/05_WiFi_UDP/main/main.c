/**
 ****************************************************************************************************
* @file        main.c
* @author      正点原子团队(ALIENTEK)
* @version     V1.0
* @date        2023-08-26
* @brief       WIFI UDP实验
* @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
****************************************************************************************************
* @attention
*
* 实验平台:正点原子 ESP32-S3 最小系统板
* 在线视频:www.yuanzige.com
* 技术论坛:www.openedv.com
* 公司网址:www.alientek.com
* 购买地址:openedv.taobao.com
*
****************************************************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "esp_system.h"
#include "nvs_flash.h"
#include "led.h"
#include "lcd.h"
#include "wifi_config.h"
#include "lwip_demo.h"
#include "key.h"

/*FreeRTOS*********************************************************************************************/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

/******************************************************************************************************/
/*FreeRTOS配置*/
/* LED_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define LED_TASK_PRIO           10          /* 任务优先级 */
#define LED_STK_SIZE            2048        /* 任务堆栈大小 */
TaskHandle_t LEDTask_Handler;               /* 任务句柄 */
void led_task(void *pvParameters);          /* 任务函数 */

/* KEY_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define KEY_TASK_PRIO           11          /* 任务优先级 */
#define KEY_STK_SIZE            2048        /* 任务堆栈大小 */
TaskHandle_t KEYTask_Handler;               /* 任务句柄 */
void key_task(void *pvParameters);          /* 任务函数 */

static portMUX_TYPE my_spinlock = portMUX_INITIALIZER_UNLOCKED;

/**
 * @brief       程序入口
 * @param       无
 * @retval      无
 */
void app_main(void)
{
    esp_err_t ret;

    ret = nvs_flash_init();             /* 初始化NVS */

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    led_init();                         /* 初始化LED */
    key_init();                         /* 初始化KEY */
    spi2_init();                        /* 初始化SPI2 */
    lcd_init();                         /* 初始化LCD */

    lcd_show_string(0, 0, lcd_self.width, 16, 16, "WiFi UDP Test", RED);
    wifi_sta_init();
    taskENTER_CRITICAL(&my_spinlock);
    /* key任务 */
    xTaskCreate((TaskFunction_t )key_task,
                (const char *   )"key_task",
                (uint16_t       )KEY_STK_SIZE,
                (void *         )NULL,
                (UBaseType_t    )KEY_TASK_PRIO,
                (TaskHandle_t * )&KEYTask_Handler);

    /* LED测试任务 */
    xTaskCreate((TaskFunction_t )led_task,
                (const char*    )"led_task",
                (uint16_t       )LED_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED_TASK_PRIO,
                (TaskHandle_t*  )&LEDTask_Handler);
    taskEXIT_CRITICAL(&my_spinlock);
    lwip_demo();            /* lwip测试代码 */
}

/**
 * @brief       key_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void key_task(void *pvParameters)
{
    pvParameters = pvParameters;
    
    uint8_t key;
  
    while (1)
    {
        key = key_scan(0);

        if (BOOT == key)
        {
            g_lwip_send_flag |= LWIP_SEND_DATA; /* 标记LWIP有数据要发送 */
        }
        
        vTaskDelay(10);
    }
}

/**
 * @brief       系统再运行
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void led_task(void *pvParameters)
{
    pvParameters = pvParameters;

    while (1)
    {
        LED_TOGGLE();
        vTaskDelay(500);
    }
}
