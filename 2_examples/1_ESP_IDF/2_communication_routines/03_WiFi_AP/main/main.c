/**
 ****************************************************************************************************
* @file        main.c
* @author      正点原子团队(ALIENTEK)
* @version     V1.0
* @date        2023-08-26
* @brief       WiFi-AP 模式创建 wifi 热点实验
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
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include <netdb.h>
#include "led.h"
#include "lcd.h"


static const char *TAG = "AP";
#define EXAMPLE_ESP_WIFI_SSID   "123"
#define EXAMPLE_ESP_WIFI_PASS   "123456789"
#define EXAMPLE_MAX_STA_CONN    5
#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"
static char lcd_buff[100] = {0};


/**
 * @brief       WIFI链接糊掉函数
 * @param       arg:传入网卡控制块
 * @param       event_base:WIFI事件
 * @param       event_id:事件ID
 * @param       event_data:事件数据
 * @retval      无
 */
static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data)
{
    /* 设备连接 */
    if (event_id == WIFI_EVENT_AP_STACONNECTED)
    {
        lcd_fill(0,40,lcd_self.width,lcd_self.height,WHITE);
        wifi_event_ap_staconnected_t *event = (wifi_event_ap_staconnected_t *)event_data;
        ESP_LOGI(TAG, "station " MACSTR " join, AID=%d",
                 MAC2STR(event->mac), event->aid);
        sprintf(lcd_buff, "MACSTR:"MACSTR,MAC2STR(event->mac));
        lcd_show_string(0, 40, lcd_self.width, 16, 16, lcd_buff, BLUE);
        lcd_show_string(0, 60, lcd_self.width, 16, 16, "With device connection", BLUE);
    } 
    /* 设备断开 */
    else if (event_id == WIFI_EVENT_AP_STADISCONNECTED)
    {
        wifi_event_ap_stadisconnected_t *event = (wifi_event_ap_stadisconnected_t *)event_data;
        ESP_LOGI(TAG, "station " MACSTR " leave, AID=%d",
                 MAC2STR(event->mac), event->aid);
        lcd_fill(0,40,lcd_self.width,lcd_self.height,WHITE);
        sprintf(lcd_buff, "Device disconnected:"MACSTR,MAC2STR(event->mac));
        lcd_show_string(0, 40, lcd_self.width, 16, 16, lcd_buff, BLUE);
    }
}

/**
 * @brief       WIFI初始化
 * @param       无
 * @retval      无
 */
static void wifi_init_softap(void)
{
    /* 初始化网卡 */
    ESP_ERROR_CHECK(esp_netif_init());

    /* 创建新的事件循环 */
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    /* 使用默认配置初始化包括netif的Wi-Fi */
    esp_netif_create_default_wifi_ap();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));
    /* 配置WIFI */
    wifi_config_t wifi_config = {
        .ap = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .ssid_len = strlen(EXAMPLE_ESP_WIFI_SSID),
            .password = EXAMPLE_ESP_WIFI_PASS,
            .max_connection = EXAMPLE_MAX_STA_CONN,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK
        },
    };

    if (strlen(EXAMPLE_ESP_WIFI_PASS) == 0)
    {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    esp_netif_ip_info_t ip_info;
    esp_netif_get_ip_info(esp_netif_get_handle_from_ifkey("WIFI_AP_DEF"), &ip_info);

    char ip_addr[16];
    inet_ntoa_r(ip_info.ip.addr, ip_addr, 16);
    ESP_LOGI(TAG, "Set up softAP with IP: %s", ip_addr);

    ESP_LOGI(TAG, "wifi_init_softap finished. SSID:'%s' password:'%s'",
             EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
    
    lcd_show_string(0, 40, lcd_self.width, 16, 16, "wifi connecting......", BLUE);
}

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
    spi2_init();                        /* 初始化SPI2 */
    lcd_init();                         /* 初始化LCD */

    lcd_show_string(0, 0, lcd_self.width, 24, 24, "WiFi AP Test", RED);
    wifi_init_softap();

    while (1)
        {
        LED_TOGGLE();
        vTaskDelay(500);
    }
}
