/**
 ****************************************************************************************************
 * @file        bmp.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2023-12-01
 * @brief       图片解码-bmp解码 代码
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

#include "bmp.h"


/**
 * @brief       BMP图片解码
 * @param       filename      : 包含路径的文件名(.bmp/.jpg/.jpeg/.gif/.png等)
 * @param       width, height : 显示区域
 * @retval      返回BMP解码速度
 */
TickType_t bmp_decode(const char *filename, int width, int height)
{
    TickType_t startTick, endTick, diffTick;
    startTick = xTaskGetTickCount();

    /* 打开文件 */
    esp_err_t ret;
    FIL* fp;
    uint16_t br = 0;
    fp = (FIL *)malloc(sizeof(FIL));    /* 申请内存 */

    ret = f_open(fp, (const TCHAR *)filename, FA_READ); /* 打开文件 */

    if (fp == NULL)
    {
        ESP_LOGW(__FUNCTION__, "File not found [%s]", filename);
        return 0;
    }

    /* 读取BMP首部 */
    bmpfile_t *result = (bmpfile_t*)malloc(sizeof(bmpfile_t));
    ret |= f_read(fp,result->header.magic, 2, (UINT *)&br);

    /* 判断图像是否是BMP文件 */
    if (result->header.magic[0]!='B' || result->header.magic[1] != 'M')
    {
        ESP_LOGW(__FUNCTION__, "File is not BMP");
        free(result);
        f_close(fp);
        return 0;
    }

    /* 读取BMP首部相关信息，如图片大小，偏移及深度等 */
    ret |= f_read(fp,&result->header.filesz, 4, (UINT *)&br);
    ret |= f_read(fp,&result->header.creator1, 2, (UINT *)&br);
    ret |= f_read(fp,&result->header.creator2, 2, (UINT *)&br);
    ret |= f_read(fp,&result->header.offset, 4, (UINT *)&br);
    ret |= f_read(fp,&result->dib.header_sz, 4, (UINT *)&br);
    ret |= f_read(fp,&result->dib.width, 4, (UINT *)&br);
    ret |= f_read(fp,&result->dib.height, 4, (UINT *)&br);
    ret |= f_read(fp,&result->dib.nplanes, 2, (UINT *)&br);
    ret |= f_read(fp,&result->dib.depth, 2, (UINT *)&br);
    ret |= f_read(fp,&result->dib.compress_type, 4, (UINT *)&br);
    ret |= f_read(fp,&result->dib.bmp_bytesz, 4, (UINT *)&br);
    ret |= f_read(fp,&result->dib.hres, 4, (UINT *)&br);
    ret |= f_read(fp,&result->dib.vres, 4, (UINT *)&br);
    ret |= f_read(fp,&result->dib.ncolors, 4, (UINT *)&br);
    ret |= f_read(fp,&result->dib.nimpcolors, 4, (UINT *)&br);

    /* 判断BMP图像深度 */
    if ((result->dib.depth == 1) && (result->dib.compress_type == 0))
    {
        /* 还未实现 */
    }
    else if((result->dib.depth == 24) && (result->dib.compress_type == 0))
    {
        /* BMP行填充（如果需要）到4字节边界 */
        uint32_t rowSize = (result->dib.width * 3 + 3) & ~3;
        int w = result->dib.width;
        int h = result->dib.height;
        int _x;
        int _w;
        int _cols;
        int _cole;
        int _y;
        int _rows;
        int _rowe;

        if (width >= w)
        {
            _x = (width - w) / 2;
            _w = w;
            _cols = 0;
            _cole = w - 1;
        }
        else
        {
            _x = 0;
            _w = width;
            _cols = (w - width) / 2;
            _cole = _cols + width - 1;
        }

        if (height >= h)
        {
            _y = (height - h) / 2;
            _rows = 0;
            _rowe = h -1;
        }
        else
        {
            _y = 0;
            _rows = (h - height) / 2;
            _rowe = _rows + height - 1;
        }

        uint8_t sdbuffer[3 * 20]; /* 像素缓冲区（每个像素R+G+B）*/
        uint16_t *colors = (uint16_t*)malloc(sizeof(uint16_t) * w);

        for (int row = 0; row < h; row++)
        {
            
            if (row < _rows || row > _rowe)
            {
                continue;
            }

            /* 寻找扫描线的起点 */
            int pos = result->header.offset + (h - 1 - row) * rowSize;
            f_lseek(fp, pos);
            int buffidx = sizeof(sdbuffer); /* 强制重新加载缓冲区 */
            int index = 0;

            for (int col = 0; col < w; col++)
            {
                if (buffidx >= sizeof(sdbuffer))
                {
                    f_read(fp,sdbuffer, sizeof(sdbuffer), (UINT *)&br);
                    buffidx = 0;
                }

                if (col < _cols || col > _cole)
                {
                    continue;
                }

                /* 将像素从BMP转换为TFT格式 */
                uint8_t b = sdbuffer[buffidx++];
                uint8_t g = sdbuffer[buffidx++];
                uint8_t r = sdbuffer[buffidx++];
                colors[index++] = rgb565(r, g, b);
            }

            pic_phy.multicolor(_x, _y, _w, colors);
            _y++;
        }

        free(colors);
    }

    free(result);
    f_close(fp);

    endTick = xTaskGetTickCount();
    diffTick = endTick - startTick;
    ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
    return diffTick;
}
