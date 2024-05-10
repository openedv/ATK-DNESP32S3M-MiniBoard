/**
 ****************************************************************************************************
 * @file        png.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2023-12-01
 * @brief       图片解码-png解码 代码
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

#include "png.h"


/**
 * @brief       PNG解码数据存储到指定存储区
 * @param       pngle   : PNG句柄
 * @param       w       : 宽度
 * @param       h       : 高度
 * @retval      无
 */
void png_init(pngle_t *pngle, uint32_t w, uint32_t h)
{
    pngle->imageWidth = w;
    pngle->imageHeight = h;
    pngle->reduction = false;
    pngle->scale_factor = 1.0;

    /* 计算折减 */
    if (pngle->screenWidth < pngle->imageWidth || pngle->screenHeight < pngle->imageHeight)
    {
        pngle->reduction = true;
        double factorWidth = (double)pngle->screenWidth / (double)pngle->imageWidth;
        double factorHeight = (double)pngle->screenHeight / (double)pngle->imageHeight;
        pngle->scale_factor = factorWidth;
        if (factorHeight < factorWidth) pngle->scale_factor = factorHeight;
        pngle->imageWidth = pngle->imageWidth * pngle->scale_factor;
        pngle->imageHeight = pngle->imageHeight * pngle->scale_factor;
    }
}

/**
 * @brief       PNG解码数据存储到指定存储区
 * @param       pngle   : PNG句柄
 * @param       x       : x坐标
 * @param       y       : y坐标
 * @param       w       : 宽度
 * @param       h       : 高度
 * @param       rgb     : RGB颜色值
 * @retval      无
 */
void png_draw(pngle_t *pngle, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t rgba[4])
{
    uint32_t _x = x;
    uint32_t _y = y;

    if (pngle->reduction)
    {
        _x = x * pngle->scale_factor;
        _y = y * pngle->scale_factor;
    }

    if (_y < pngle->screenHeight && _x < pngle->screenWidth)
    {
        pngle->pixels[_y][_x] = rgb565(rgba[0], rgba[1], rgba[2]);
    }
}

/**
 * @brief       PNG解码完成回调函数
 * @param       pngle   : PNG句柄
 * @retval      无
 */
void png_finish(pngle_t *pngle)
{
    ESP_LOGI(__FUNCTION__, "png_finish");
}

/**
 * @brief       PNG图片解码
 * @param       filename      : 包含路径的文件名(.bmp/.jpg/.jpeg/.gif/.png等)
 * @param       width, height : 显示区域
 * @retval      返回BMP解码速度
 */
TickType_t png_decode(const char *filename, int width, int height)
{
    TickType_t startTick, endTick, diffTick;
    startTick = xTaskGetTickCount();
    uint16_t _width = width;
    uint16_t _cols = 0;
    char buf[1024];
    size_t remain = 0;
    uint16_t _height = height;
    uint16_t _rows = 0;
    double display_gamma = 2.2;
    
    /* 打开PNG文件 */
    FIL* fp;
    uint16_t len;
    fp = (FIL *)malloc(sizeof(FIL));                /* 申请内存 */
    f_open(fp, (const TCHAR *)filename, FA_READ);   /* 打开文件 */

    if (fp == NULL)
    {
        ESP_LOGW(__FUNCTION__, "File not found [%s]", filename);
        return 0;
    }

    /* 初始化解码库并设置回调函数 */
    pngle_t *pngle = pngle_new(width, height);
    pngle_set_init_callback(pngle, png_init);
    pngle_set_draw_callback(pngle, png_draw);
    pngle_set_done_callback(pngle, png_finish);
    pngle_set_display_gamma(pngle, display_gamma);

    while (!f_eof(fp))
    {
        if (remain >= sizeof(buf))
        {
            ESP_LOGE(__FUNCTION__, "Buffer exceeded");
            while(1) vTaskDelay(1);
        }

        f_read(fp,buf + remain,sizeof(buf) - remain, (UINT *)&len);


        int fed = pngle_feed(pngle, buf, remain + len);

        if (fed < 0)
        {
            ESP_LOGE(__FUNCTION__, "ERROR; %s", pngle_error(pngle));
            while(1) vTaskDelay(1);
        }

        remain = remain + len - fed;

        if (remain > 0)
        {
            memmove(buf, buf + fed, remain);
        }
    }

    f_close(fp);

    if (width > pngle->imageWidth)
    {
        _width = pngle->imageWidth;
        _cols = (width - pngle->imageWidth) / 2;
    }

    if (height > pngle->imageHeight)
    {
        _height = pngle->imageHeight;
        _rows = (height - pngle->imageHeight) / 2;
    }

    uint16_t *colors = (uint16_t*)malloc(sizeof(uint16_t) * _width);

    for(int y = 0; y < _height; y++)
    {
        for(int x = 0;x < _width; x++)
        {
            colors[x] = pngle->pixels[y][x];
        }

        pic_phy.multicolor(_cols, y+_rows, _width, colors);
        vTaskDelay(1);
    }

    free(fp);
    free(colors);
    pngle_destroy(pngle, width, height);

    endTick = xTaskGetTickCount();
    diffTick = endTick - startTick;
    ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
    return diffTick;
}
