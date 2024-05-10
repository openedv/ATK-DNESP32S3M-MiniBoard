/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2023-12-01
 * @brief       汉字显示实验
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
#include "led.h"
#include "spi.h"
#include "lcd.h"
#include "nvs_flash.h"
#include "fonts.h"
#include "spi_sdcard.h"
#include "text.h"
#include "key.h"


/**
 * @brief       程序入口
 * @param       无
 * @retval      无
 */
void app_main(void)
{
    esp_err_t ret;
    uint8_t t;
    uint8_t key;
    uint32_t fontcnt;
    uint8_t i;
    uint8_t j;
    uint8_t fontx[2];

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

    while (sd_spi_init())                               /* 检测不到SD卡 */
    {
        lcd_show_string(0, 0, 200, 16, 16, "SD Card Failed!", RED);
        vTaskDelay(200);
        lcd_fill(0, 0, 200 + 30, 50 + 16, WHITE);
        vTaskDelay(200);
    }

    while (fonts_init())                                /* 检查字库 */
    {
UPD:
        lcd_clear(WHITE);                               /* 清屏 */
        lcd_show_string(0, 0, 200, 16, 16, "ESP32-S3", RED);
        
        key = fonts_update_font(0, 20, 16, (uint8_t *)"0:", RED);  /* 更新字库 */

        while (key)                                     /* 更新失败 */
        {
            lcd_show_string(0, 50, 200, 16, 16, "Font Update Failed!", RED);
            vTaskDelay(200);
            lcd_fill(0, 50, 200 + 20, 90 + 16, WHITE);
            vTaskDelay(200);
        }

        lcd_show_string(0, 20, 200, 16, 16, "Font Update Success!   ", RED);
        vTaskDelay(1500);
        lcd_clear(WHITE);                               /* 清屏 */
    }
   
    text_show_string(0, 0, 200, 16, "GBK字库测试程序", 16, 0, RED);
    text_show_string(0, 20, 200, 16, "KEY0: 更新字库", 16, 0, RED);
    text_show_string(0, 40, 200, 16, "汉字计数器:", 16, 0, BLUE);
    text_show_string(0, 60, 200, 12, "对应汉字为:", 12, 0, BLUE);

    while (1)
    {
        fontcnt = 0;
        
        for (i = 0x81; i < 0xFF; i++)                                           /* GBK内码高字节范围为0x81~0xFE */
        {
            fontx[0] = i;
            
            for (j = 0x40; j < 0xFE; j ++)                                      /* GBK内码低字节范围为0x40~0x7E、0x80~0xFE) */
            {
                if (j == 0x7F)
                {
                    continue;
                }
                
                fontcnt++;
                lcd_show_num(108, 40, fontcnt, 5, 16, BLUE);                   /* 汉字计数显示 */
                fontx[1] = j;
                text_show_font(108, 60, fontx, 12, 0, BLUE);
                
                t = 200;
                
                while ((t --) != 0)                                              /* 延时，同时扫描按键 */
                {
                    vTaskDelay(1);
                    
                    key = key_scan(0);
                    
                    if (key == BOOT)
                    {
                        goto UPD;                                               /* 跳转到UPD位置（强制更新字库） */
                    }
                }
                
                LED_TOGGLE();
            }
        }
    }
}
