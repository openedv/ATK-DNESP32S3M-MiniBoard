/**
 ****************************************************************************************************
* @file        jpeg.c
* @author      正点原子团队(ALIENTEK)
* @version     V1.0
* @date        2023-12-01
* @brief       图片解码-jpeg/jpg解码 代码
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

#include "jpeg.h"


/* ROM代码外的TJPGD较新，在解码回调中具有不同的返回类型 */
typedef int jpeg_decode_out_t;
JDEC jpeg_dev;                  /* 待解码对象结构体指针 */

/**
 * @brief       jpeg数据输入回调函数
 * @param       jd       : 储存待解码的对象信息的结构体
 * @param       buf      : 输入数据缓冲区 (NULL:执行地址偏移)
 * @param       num      : 需要从输入数据流读出的数据量/地址偏移量
 * @retval      读取到的字节数/地址偏移量
 */
static unsigned int infunc(JDEC *decoder, uint8_t *buf, unsigned int len)
{
    uint16_t  rb;                           /* 读取到的字节数 */
    FIL *dev = (FIL *)decoder->device;      /* 待解码的文件的信息，使用FATFS中的FIL结构类型进行定义 */

    if (buf)                                /* 读取数据有效，开始读取数据 */
    {
        f_read(dev, buf, len, (UINT *)&rb); /* 调用FATFS的f_read函数，用于把jpeg文件的数据读取出来 */
        return rb;                          /* 返回读取到的字节数目 */
    }
    else
    {
        return (f_lseek(dev, f_tell(dev) + len) == FR_OK) ? len : 0;    /* 重新定位数据点，相当于删除之前的n字节数据 */
    }
}

/**
 * @brief       输出功能。将来自解码器的RGB888数据重新编码为big-endian RGB565
 * @param       decoder : JPEG解码结构体
 * @param       bitmap  : 位图数据
 * @param       rect    : 图像坐标信息
 * @retval      返回1输出成功
 */
static jpeg_decode_out_t outfunc(JDEC *decoder, void *bitmap, JRECT *rect)
{
    JDEC *jd = (JDEC *) decoder;
    uint8_t *in = (uint8_t *) bitmap;

    for (int y = rect->top; y <= rect->bottom; y++)
    {
        for (int x = rect->left; x <= rect->right; x++)
        {
            if (y < jd->screenHeight && x < jd->screenWidth)
            {
                jd->outData[y][x] = rgb565(in[0], in[1], in[2]);
            }

            in += 3;
        }
    }

    return 1;
}

/**
 * @brief       指定输出的比例因子N。输出图像被缩放为1/2^N（N=0到3）
 * @param       screenWidth     : 屏幕宽度
 * @param       screenHeight    : 屏幕高度
 * @param       decodeWidth     : 解码宽度
 * @param       decodeHeight    : 解码高度
 * @retval      0:失败;1;2倍比例;2:4倍比例;3:原本大小
 */
uint8_t getScale(int screenWidth, int screenHeight, uint16_t decodeWidth, uint16_t decodeHeight)
{
    if (screenWidth >= decodeWidth && screenHeight >= decodeHeight)
    {
        return 0;
    }

    double scaleWidth = (double)decodeWidth / (double)screenWidth;
    double scaleHeight = (double)decodeHeight / (double)screenHeight;
    double scale = scaleWidth;

    if (scaleWidth < scaleHeight)
    {
        scale = scaleHeight;
    }

    if (scale <= 2.0)
    {
        return 1;
    }

    if (scale <= 4.0)
    {
        return 2;
    }

    return 3;
}

/**
 * @brief       JPEG/JPG解码函数
 * @param       pixels          : 像素点
 * @param       file            : 读取文件名称
 * @param       screenWidth     : 屏幕宽度
 * @param       screenHeight    : 屏幕高度
 * @param       imageWidth      : 解码宽度
 * @param       imageHeight     : 解码高度
 * @retval      0:失败;1;2倍比例;2:4倍比例;3:原本大小
 */
esp_err_t decode_jpeg(pixel_jpeg ***pixels, char * file, int screenWidth, int screenHeight, int * imageWidth, int * imageHeight)
{
    char *work = NULL;
    FIL *f_jpeg = NULL;     /* JPEG文件指针 */
    *pixels = NULL;
    JRESULT res = JDR_OK;
    esp_err_t ret = ESP_OK;
    uint32_t jd_work_size = 6144 + 4096;

    /* Alocate像素内存。每一行是IMAGE_W 16位像素的阵列；“*像素”数组本身包含指向这些行的指针 */
    *pixels = malloc( sizeof(pixel_jpeg *) * screenHeight);
    /* 申请失败 */
    if (*pixels == NULL)
    {
        ESP_LOGE(__FUNCTION__, "Error allocating memory for lines");
        ret = ESP_ERR_NO_MEM;
        goto err;
    }

    for (int i = 0; i < screenHeight; i++)
    {
        (*pixels)[i] = malloc(screenWidth * sizeof(pixel_jpeg));

        if ((*pixels)[i] == NULL)
        {
            ESP_LOGE(__FUNCTION__, "Error allocating memory for line %d", i);
            ret = ESP_ERR_NO_MEM;
            goto err;
        }
    }

    /* 为jpeg解码器分配工作空间 */
    work = malloc(jd_work_size);

    if (work == NULL)
    {
        ESP_LOGE(__FUNCTION__, "Cannot allocate workspace");
        ret = ESP_ERR_NO_MEM;
        goto err;
    }
    
    /* 填充JpegDev结构的字段 */
    jpeg_dev.outData = *pixels;
    jpeg_dev.screenWidth = screenWidth;
    jpeg_dev.screenHeight = screenHeight;

    f_jpeg = (FIL *)malloc(sizeof(FIL));    /* 申请内存 */

    f_open(f_jpeg, (const TCHAR *)file, FA_READ); /* 打开文件 */

    if (f_jpeg == NULL)
    {
        ESP_LOGW(__FUNCTION__, "Image file not found [%s]", file);
        ret = ESP_ERR_NOT_FOUND;
        goto err;
    }

    /* 准备并解码jpeg */
    res = jd_prepare(&jpeg_dev, infunc, work, jd_work_size, f_jpeg);

    if (res != JDR_OK)
    {
        ESP_LOGE(__FUNCTION__, "Image decoder: jd_prepare failed (%d)", res);
        ret = ESP_ERR_NOT_SUPPORTED;
        goto err;
    }

    /* 计算比例因子 */
    uint8_t scale = getScale(screenWidth, screenHeight, jpeg_dev.width, jpeg_dev.height);

    /* 计算图像大小 */
    double factor = 1.0;
    if (scale == 1) factor = 0.5;
    if (scale == 2) factor = 0.25;
    if (scale == 3) factor = 0.125;
    *imageWidth = (double)jpeg_dev.width * factor;
    *imageHeight = (double)jpeg_dev.height * factor;
    /* 反编译 */
    res = jd_decomp(&jpeg_dev, outfunc, scale);

    if (res != JDR_OK)
    {
        ESP_LOGE(__FUNCTION__, "Image decoder: jd_decode failed (%d)", res);
        ret = ESP_ERR_NOT_SUPPORTED;
        goto err;
    }

    /* 全部完成！释放内存 */
    free(work);
    f_close(f_jpeg);
    return ret;

    /* 出现解码错误的话，执行以下代码 */
    err:
    f_close(f_jpeg);

    if (*pixels != NULL)
    {
        for (int i = 0; i < screenHeight; i++)
        {
            if ((*pixels)[i]) free((*pixels)[i]);
        }

        free(*pixels);
    }

    free(work);
    return ret;
}

/**
 * @brief       显示完成后释放内存
 * @param       pixels          : 像素点地址
 * @param       screenWidth     : 屏幕宽度
 * @param       screenHeight    : 屏幕高度
 * @retval      ESP_OK:成功;其他:失败
 */
esp_err_t release_image(pixel_jpeg ***pixels, int screenWidth, int screenHeight)
{
    if (*pixels != NULL)
    {
        for (int i = 0; i < screenHeight; i++)
        {
            free((*pixels)[i]);
        }
        free(*pixels);
    }

    return ESP_OK;
}

/**
 * @brief       JPEG图片解码
 * @param       filename      : 包含路径的文件名(.bmp/.jpg/.jpeg/.gif/.png等)
 * @param       width, height : 显示区域
 * @retval      返回BMP解码速度
 */
TickType_t jpeg_decode(const char *filename, int width, int height)
{
    TickType_t startTick, endTick, diffTick;
    startTick = xTaskGetTickCount();

    pixel_jpeg **pixels;
    int imageWidth;
    int imageHeight;
    esp_err_t err = decode_jpeg(&pixels, (char *)filename, width, height, &imageWidth, &imageHeight);
    ESP_LOGI(__FUNCTION__, "decode_image err=%d imageWidth=%d imageHeight=%d", err, imageWidth, imageHeight);
    if (err == ESP_OK)
    {

        uint16_t _width = width;
        uint16_t _cols = 0;

        if (width > imageWidth)
        {
            _width = imageWidth;
            _cols = (width - imageWidth) / 2;
        }

        ESP_LOGI(__FUNCTION__, "_width=%d _cols=%d", _width, _cols);

        uint16_t _height = height;
        uint16_t _rows = 0;

        if (height > imageHeight)
        {
            _height = imageHeight;
            _rows = (height - imageHeight) / 2;
        }

        ESP_LOGI(__FUNCTION__, "_height=%d _rows=%d", _height, _rows);
        uint16_t *colors = (uint16_t*)malloc(sizeof(uint16_t) * _width);

        for(int y = 0; y < _height; y++)
        {
            for(int x = 0;x < _width; x++)
            {
                colors[x] = pixels[y][x];
            }

            pic_phy.multicolor(_cols, y+_rows, _width, colors);
            vTaskDelay(1);
        }

        free(colors);
        release_image(&pixels, width, height);
        ESP_LOGI(__FUNCTION__, "Finish");
    }
    else
    {
        ESP_LOGE(__FUNCTION__, "decode_jpeg fail=%d", err);
    }

    endTick = xTaskGetTickCount();
    diffTick = endTick - startTick;
    ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
    return diffTick;
}
