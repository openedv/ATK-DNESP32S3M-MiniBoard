/**
 ****************************************************************************************************
 * @file        sensor.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2023-08-26
 * @brief       内部温度传感器驱动代码
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

#include "sensor.h"


esp_err_t rev_flag;
temperature_sensor_handle_t temp_handle = NULL; /* 温度传感器句柄 */

/**
 * @brief       初始化内部温度传感器
 * @param       无
 * @retval      无
 */
void temperature_sensor_init(void)
{
    temperature_sensor_config_t temp_sensor;
    
    temp_sensor.range_min = SENSOR_RANGE_MIN;   /* 要测试温度的最小值 */
    temp_sensor.range_max = SENSOR_RANGE_MAX;   /* 要测试温度的最大值 */

    rev_flag |= temperature_sensor_install(&temp_sensor, &temp_handle);
    ESP_ERROR_CHECK(rev_flag);
}

/**
 * @brief       获取内部温度传感器温度值
 * @param       无
 * @retval      返回内部温度值
 */
short sensor_get_temperature(void)
{
    float temp;

    /* 启用温度传感器 */
    rev_flag |= temperature_sensor_enable(temp_handle);

    /* 获取传输的传感器数据 */ 
    rev_flag |= temperature_sensor_get_celsius(temp_handle, &temp);

    /* 温度传感器使用完毕后，禁用温度传感器，节约功耗 */
    rev_flag |= temperature_sensor_disable(temp_handle);
    ESP_ERROR_CHECK(rev_flag);

    return temp;
}
