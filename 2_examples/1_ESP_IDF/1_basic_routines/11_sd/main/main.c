/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2023-08-26
 * @brief       SD卡实验
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
#include "nvs_flash.h"
#include "led.h"
#include "spi.h"
#include "lcd.h"
#include "spi_sdcard.h"


/**
 * @brief       程序入口
 * @param       无
 * @retval      无
 */
void app_main(void)
{
    esp_err_t ret;
    size_t bytes_total, bytes_free;                     /* SD卡的总空间与剩余空间 */

    ret = nvs_flash_init();                             /* 初始化NVS */

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    led_init();                                         /* 初始化LED */
    spi2_init();                                        /* 初始化SPI */
    lcd_init();                                         /* 初始化LCD */

    while (sd_spi_init())                               /* 检测不到SD卡 */
    {
        lcd_show_string(0, 0, 200, 16, 16, "SD Card Error!", RED);
        vTaskDelay(500);
        lcd_show_string(0, 20, 200, 16, 16, "Please Check! ", RED);
        vTaskDelay(500);
    }

    lcd_show_string(0, 0, 200, 16, 16, "SD Card OK!", RED);
    lcd_show_string(0, 20, 200, 16, 16, "Total:       MB", RED);
    lcd_show_string(0, 40, 200, 16, 16, "Free :       MB", RED);
    sd_get_fatfs_usage(&bytes_total, &bytes_free);

    lcd_show_num(60, 20,(int)bytes_total / 1024,5,16,BLUE);
    lcd_show_num(60, 40,(int)bytes_free / 1024,5,16,BLUE);

    while (1)
    {
        LED_TOGGLE();
        vTaskDelay(500);
    }
}
