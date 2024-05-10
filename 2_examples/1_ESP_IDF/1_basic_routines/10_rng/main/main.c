/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2023-08-26
 * @brief       RNG 实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 ESP32-S3 开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "led.h"
#include "spi.h"
#include "lcd.h"
#include "rng.h"
#include "key.h"


/**
 * @brief       程序入口
 * @param       无
 * @retval      无
 */
void app_main(void)
{
    uint8_t key;
    uint32_t random;
    uint8_t t = 0;
    esp_err_t ret;
    
    ret = nvs_flash_init();                                         /* 初始化NVS */

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    led_init();                                                     /* 初始化LED */
    spi2_init();                                                    /* 初始化SPI2 */
    lcd_init();                                                     /* 初始化LCD */
    key_init();                                                     /* 初始化按键 */
    
    lcd_show_string(0, 0, 200, 16, 16, "RNG Test", RED);
    lcd_show_string(0, 20, 200, 16, 16, "BOOT:Get Random Num", RED);
    lcd_show_string(0, 40, 200, 16, 16, "Num:", RED);
    lcd_show_string(0, 60, 200, 16, 16, "Num[0-9]:", RED);

    while(1)
    {
        key = key_scan(0);
        
        if (key == BOOT)                                            /* 获取随机数并显示至LCD */
        {
            random = rng_get_random_num();
            lcd_show_num(30 + 8 * 5, 40, random, 10, 16, BLUE);
        }
        
        if ((t % 20) == 0)                                          /* 获取0~9间的随机数并显示至LCD */
        {
            LED_TOGGLE();                                           /* 每200ms,翻转一次LED */
            random = rng_get_random_range(0, 9);                    /* 取[0,9]区间的随机数 */
            lcd_show_num(32 + 8 * 11, 60, random, 1, 16, BLUE);    /* 显示随机数 */
        }

        vTaskDelay(10);
        t++;
    }
}
