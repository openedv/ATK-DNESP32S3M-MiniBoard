/**
 ****************************************************************************************************
* @file        main.c
* @author      正点原子团队(ALIENTEK)
* @version     V1.0
* @date        2023-08-26
* @brief       Smartconfig一键配网实验
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
#include "esp_wpa2.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_smartconfig.h"
#include "led.h"
#include "lcd.h"


/* 定义事件 */
static EventGroupHandle_t s_wifi_event_group;
static const int CONNECTED_BIT = BIT0;
static const int ESPTOUCH_DONE_BIT = BIT1;
static const char *TAG = "smartconfig_example";
static void smartconfig_task(void * parm);
static char lcd_buff[100] = {0};


/**
 * @brief       WIFI链接糊掉函数
 * @param       arg:传入网卡控制块
 * @param       event_base:WIFI事件
 * @param       event_id:事件ID
 * @param       event_data:事件数据
 * @retval      无
 */
static void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        xTaskCreate(smartconfig_task, "smartconfig_task", 4096, NULL, 3, NULL);
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        esp_wifi_connect();
        xEventGroupClearBits(s_wifi_event_group, CONNECTED_BIT);
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        xEventGroupSetBits(s_wifi_event_group, CONNECTED_BIT);
    }
    else if (event_base == SC_EVENT && event_id == SC_EVENT_SCAN_DONE)
    {
        ESP_LOGI(TAG, "Scan done");
        lcd_show_string(0, 20, lcd_self.width, 16, 16, "In the distribution network......", BLUE);
    }
    else if (event_base == SC_EVENT && event_id == SC_EVENT_FOUND_CHANNEL)
    {
        ESP_LOGI(TAG, "Found channel");
    }
    /* 已获取SSID和密码,表示配网成功 */
    else if (event_base == SC_EVENT && event_id == SC_EVENT_GOT_SSID_PSWD)
    {
        ESP_LOGI(TAG, "Got SSID and password");

        smartconfig_event_got_ssid_pswd_t *evt = (smartconfig_event_got_ssid_pswd_t *)event_data;
        wifi_config_t wifi_config;
        uint8_t ssid[33] = { 0 };
        uint8_t password[65] = { 0 };
        uint8_t rvd_data[33] = { 0 };

        bzero(&wifi_config, sizeof(wifi_config_t));
        memcpy(wifi_config.sta.ssid, evt->ssid, sizeof(wifi_config.sta.ssid));
        memcpy(wifi_config.sta.password, evt->password, sizeof(wifi_config.sta.password));
        wifi_config.sta.bssid_set = evt->bssid_set;

        if (wifi_config.sta.bssid_set == true)
        {
            memcpy(wifi_config.sta.bssid, evt->bssid, sizeof(wifi_config.sta.bssid));
        }

        memcpy(ssid, evt->ssid, sizeof(evt->ssid));
        memcpy(password, evt->password, sizeof(evt->password));
        ESP_LOGI(TAG, "SSID:%s", ssid);
        ESP_LOGI(TAG, "PASSWORD:%s", password);

        lcd_fill(0,20,lcd_self.width,lcd_self.height,WHITE);
        sprintf(lcd_buff, "%s",ssid);
        lcd_show_string(0, 20, lcd_self.width, 16, 16, lcd_buff, BLUE);
        sprintf(lcd_buff, "%s",password);
        lcd_show_string(0, 40, lcd_self.width, 16, 16, lcd_buff, BLUE);
        lcd_show_string(0, 60, lcd_self.width, 16, 16, "Successful distribution network", BLUE);

        /* 手机APPEspTouch软件使用ESPTOUCH V2模式，会执行以下代码 */
        if (evt->type == SC_TYPE_ESPTOUCH_V2)
        {
            ESP_ERROR_CHECK( esp_smartconfig_get_rvd_data(rvd_data, sizeof(rvd_data)) );
            ESP_LOGI(TAG, "RVD_DATA:");

            for (int i = 0; i < 33; i++)
            {
                printf("%02x ", rvd_data[i]);
            }

            printf("\n");
        }

        ESP_ERROR_CHECK( esp_wifi_disconnect() );
        ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
        esp_wifi_connect();
    }
    else if (event_base == SC_EVENT && event_id == SC_EVENT_SEND_ACK_DONE)
    {
        xEventGroupSetBits(s_wifi_event_group, ESPTOUCH_DONE_BIT);
    }
}

/**
 * @brief       WiFi一键配网
 * @param       无
 * @retval      无
 */
static void wifi_smartconfig_sta(void)
{
    /* 初始化网卡 */
    ESP_ERROR_CHECK(esp_netif_init());
    /* 创建事件 */
    s_wifi_event_group = xEventGroupCreate();
    /* 使用默认配置初始化包括netif的Wi-Fi */
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    /* 把WIFI网卡设置为STA模式 */
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    /* WIFI初始化 */
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );

    /* 注册WIFI事件 */
    ESP_ERROR_CHECK( esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL) );
    ESP_ERROR_CHECK( esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL) );
    ESP_ERROR_CHECK( esp_event_handler_register(SC_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL) );

    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK( esp_wifi_start() );
}

/**
 * @brief       一键配网回调函数
 * @param       parm:传入的形参(未使用)
 * @retval      无
 */
static void smartconfig_task(void * parm)
{
    parm = parm;
    EventBits_t uxBits;
    /* 设置配网协议 */
    ESP_ERROR_CHECK( esp_smartconfig_set_type(SC_TYPE_ESPTOUCH) );
    /* 设置配网参数 */
    smartconfig_start_config_t cfg = SMARTCONFIG_START_CONFIG_DEFAULT();
    /* 开始配网 */
    ESP_ERROR_CHECK( esp_smartconfig_start(&cfg) );

    while (1)
    {
        /* 获取事件 */
        uxBits = xEventGroupWaitBits(s_wifi_event_group, CONNECTED_BIT | ESPTOUCH_DONE_BIT, true, false, portMAX_DELAY);
        
        /* 配网成功 */
        if(uxBits & CONNECTED_BIT)
        {
            ESP_LOGI(TAG, "WiFi Connected to ap");
        }

        /* 智能配置结束 */
        if(uxBits & ESPTOUCH_DONE_BIT)
        {
            ESP_LOGI(TAG, "smartconfig over");
            esp_smartconfig_stop();
            vTaskDelete(NULL);
        }
    }
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

    lcd_show_string(0, 0, lcd_self.width, 16, 16, "WiFi SmartConfig Test", RED);
    wifi_smartconfig_sta();

    while (1)
    {
        LED_TOGGLE();
        vTaskDelay(500);
    }
}
