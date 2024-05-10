/**
 ****************************************************************************************************
 * @file        tud_usart.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2023-12-01
 * @brief       USB模拟串口 代码
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

#include "tud_usart.h"


static const char *TAG = "usb_msc";
static uint8_t buf[CONFIG_TINYUSB_CDC_RX_BUFSIZE + 1];

/**
 * @brief       SD卡模拟U盘函数初始化
 * @param       itf     :设置的简要CDC端口
 * @param       event   :CDC事件
 * @retval      无
 */
void tinyusb_cdc_rx_callback(int itf, cdcacm_event_t *event)
{
    size_t rx_size = 0;

    /* 读取串口的数据 */
    esp_err_t ret = tinyusb_cdcacm_read(itf, buf, CONFIG_TINYUSB_CDC_RX_BUFSIZE, &rx_size);

    if (ret == ESP_OK)
    {
        ESP_LOGI(TAG, "Data from channel %d:", itf);
        ESP_LOG_BUFFER_HEXDUMP(TAG, buf, rx_size, ESP_LOG_INFO);
    }
    else
    {
        ESP_LOGE(TAG, "Read error");
    }

    /* 发送数据 */
    tinyusb_cdcacm_write_queue(itf, buf, rx_size);
    tinyusb_cdcacm_write_flush(itf, 0);
}

/**
 * @brief       SD卡模拟U盘函数初始化
 * @param       itf     :设置的简要CDC端口
 * @param       event   :CDC事件
 * @retval      无
 */
void tinyusb_cdc_line_state_changed_callback(int itf, cdcacm_event_t *event)
{
    int dtr = event->line_state_changed_data.dtr;
    int rts = event->line_state_changed_data.rts;
    ESP_LOGI(TAG, "Line state changed on channel %d: DTR:%d, RTS:%d", itf, dtr, rts);
}

/**
 * @brief       SD卡模拟U盘函数初始化
 * @param       无
 * @retval      无
 */
void tud_usb_usart(void)
{
    ESP_LOGI(TAG, "USB initialization");
    const tinyusb_config_t tusb_cfg = {
        .device_descriptor = NULL,
        .string_descriptor = NULL,
        .external_phy = false,
        .configuration_descriptor = NULL,
    };
    /* USB设备登记 */
    ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));

    tinyusb_config_cdcacm_t acm_cfg = {
        .usb_dev = TINYUSB_USBDEV_0,                /* USB设备 */
        .cdc_port = TINYUSB_CDC_ACM_0,              /* CDC端口 */
        .rx_unread_buf_sz = 64,                     /* 配置RX缓冲区大小 */
        .callback_rx = &tinyusb_cdc_rx_callback,    /* 接收回调函数 */
        .callback_rx_wanted_char = NULL,
        .callback_line_state_changed = NULL,
        .callback_line_coding_changed = NULL
    };
    /* USB CDC初始化 */
    ESP_ERROR_CHECK(tusb_cdc_acm_init(&acm_cfg));
    /* 注册回调函数 */
    ESP_ERROR_CHECK(tinyusb_cdcacm_register_callback(
                        TINYUSB_CDC_ACM_0,
                        CDC_EVENT_LINE_STATE_CHANGED,
                        &tinyusb_cdc_line_state_changed_callback));

    ESP_LOGI(TAG, "USB initialization DONE");
}
