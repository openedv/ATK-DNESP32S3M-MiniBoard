/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2023-12-01
 * @brief       SD卡模拟U盘 实验
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
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "led.h"
#include "spi.h"
#include "lcd.h"
#include "tud_sd.h"
#include "spi_sdcard.h"


/**
 * @brief       程序入口
 * @param       无
 * @retval      无
 */
void app_main(void)
{
    esp_err_t ret;

    ret = nvs_flash_init();                             /* 初始化NVS */
    
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    ESP_ERROR_CHECK(ret);

    led_init();                                         /* 初始化LED */
    spi2_init();                                        /* 初始化SPI */
    lcd_init();                                         /* 初始化LCD */
    /* 显示实验信息 */
    lcd_show_string(30, 0, 200, 16, 16, "ESP32-S3", RED);
    lcd_show_string(30, 20, 200, 16, 16, "USB SD TEST", RED);
    lcd_show_string(30, 40, 200, 16, 16, "ATOM@ALIENTEK", RED);
    
    while (sd_spi_init())                               /* 检测不到SD卡 */
    {
        lcd_show_string(0, 60, 200, 16, 16, "SD Card Error!", RED);
        vTaskDelay(500);
        lcd_show_string(0, 60, 200, 16, 16, "Please Check! ", RED);
        vTaskDelay(500);
    }

    tud_usb_sd();                                       /* USB初始化 */

    while(1)
    {
        if ((g_usbdev.status & 0x0f) == 0x01)
        {
            lcd_show_string(0, 60, lcd_self.width, 16, 16, "connect success.....", BLUE);
        }
        else if ((g_usbdev.status & 0x0f) == 0x00)
        {
            lcd_show_string(0, 60, lcd_self.width, 16, 16, "connect fail........", BLUE);
        }

        LED_TOGGLE();
        vTaskDelay(500);
    }
}
