/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2023-08-26
 * @brief       内部温度传感器实验
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

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "led.h"
#include "lcd.h"
#include "sensor.h"


/**
 * @brief       程序入口
 * @param       无
 * @retval      无
 */
void app_main(void)
{
    int16_t temp;
    esp_err_t ret;

    ret = nvs_flash_init();                                                     /* 初始化NVS */

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    led_init();                                                                 /* 初始化LED */
    spi2_init();                                                                /* 初始化SPI2 */
    lcd_init();                                                                 /* 初始化LCD */
    temperature_sensor_init();                                                  /* 初始化内部温度传感器 */
    
    lcd_show_string(0, 0, 200, 16, 16, "TEMPERATE: 00.00C", BLUE);

    while(1)
    {
        temp = sensor_get_temperature();                                        /* 得到温度值 */

        if (temp < 0)
        {
            temp = -temp;
            lcd_show_string(0 + 10 * 8, 0, 16, 16, 16, "-", BLUE);           /* 显示符号 */
        }
        else
        {
            lcd_show_string(0 + 10 * 8, 0, 16, 16, 16, " ", BLUE);           /* 无符号 */
        }

        lcd_show_xnum(0 + 11 * 8, 0, temp, 2, 16, 0, BLUE);                  /* 显示整数部分 */
        lcd_show_xnum(0 + 14 * 8, 0, temp * 100 % 100, 2, 16, 0x80, BLUE);   /* 显示小数部分 */
        
        LED_TOGGLE();                                                           /* LED闪烁,提示程序运行 */
        vTaskDelay(250);
    }
}
