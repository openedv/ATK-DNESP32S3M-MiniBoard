/**
 ****************************************************************************************************
 * @file        rng.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2023-08-26
 * @brief       RNG驱动代码
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

#include "rng.h"


/**
 * @brief       得到随机数
 * @param       无
 * @retval      获取到的随机数(32bit)
 */
uint32_t rng_get_random_num(void)
{
    uint32_t randomnum;
    
    randomnum = esp_random();
    
    return randomnum;
}

/**
 * @brief       得到某个范围内的随机数
 * @param       min,max: 最小,最大值.
 * @retval      得到的随机数(rval),满足:min<=rval<=max
 */
int rng_get_random_range(int min, int max)
{ 
    uint32_t randomnum;
    
    randomnum = esp_random();
    
    return randomnum % (max - min + 1) + min;
}
