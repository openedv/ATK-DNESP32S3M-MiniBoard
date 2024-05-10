/**
 ****************************************************************************************************
* @file        lcd.c
* @author      正点原子团队(ALIENTEK)
* @version     V1.0
* @date        2023-08-26
* @brief       SPI LCD(MCU屏) 驱动代码
*              支持驱动IC型号包括:ILI9341等
*
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

#include "lcd.h"
#include "lcdfont.h"


spi_device_handle_t MY_LCD_Handle;
uint8_t lcd_buf[LCD_TOTAL_BUF_SIZE];
lcd_obj_t lcd_self;


/* LCD需要初始化一组命令/参数值。它们存储在此结构中  */
typedef struct
{
    uint8_t cmd;
    uint8_t data[16];
    uint8_t databytes; /* 数据中没有数据；比特7＝设置后的延迟；0xFF=cmds结束 */
} lcd_init_cmd_t;

/**
 * @brief       发送命令到LCD，使用轮询方式阻塞等待传输完成(由于数据传输量很少，因此在轮询方式处理可提高速度。使用中断方式的开销要超过轮询方式)
 * @param       cmd 传输的8位命令数据
 * @retval      无
 */
void lcd_write_cmd(const uint8_t cmd)
{
    LCD_WR(0);
    spi2_write_cmd(MY_LCD_Handle, cmd);
}

/**
 * @brief       发送数据到LCD，使用轮询方式阻塞等待传输完成(由于数据传输量很少，因此在轮询方式处理可提高速度。使用中断方式的开销要超过轮询方式)
 * @param       data 传输的8位数据
 * @retval      无
 */
void lcd_write_data(const uint8_t *data, int len)
{
    LCD_WR(1);
    spi2_write_data(MY_LCD_Handle, data, len);
}

/**
 * @brief       发送数据到LCD，使用轮询方式阻塞等待传输完成(由于数据传输量很少，因此在轮询方式处理可提高速度。使用中断方式的开销要超过轮询方式)
 * @param       data 传输的16位数据
 * @retval      无
 */
void lcd_write_data16(uint16_t data)
{
    uint8_t dataBuf[2] = {0,0};
    dataBuf[0] = data >> 8;
    dataBuf[1] = data & 0xFF;
    LCD_WR(1);
    spi2_write_data(MY_LCD_Handle, dataBuf,2);
}

/**
 * @brief       设置窗口大小
 * @param       xstar：左上角x轴
 * @param       ystar：左上角y轴
 * @param       xend：右下角x轴
 * @param       yend：右下角y轴
 * @retval      无
 */
void lcd_set_window(uint16_t xstar, uint16_t ystar,uint16_t xend,uint16_t yend)
{
    uint8_t databuf[4] = {0,0,0,0};
    
    if (lcd_self.dir == 1)                  /* 横屏 */
    {
        databuf[0] = (xstar + 1) >> 8;
        databuf[1] = 0xFF & (xstar + 1);
        databuf[2] = (xend + 1) >> 8;
        databuf[3] = 0xFF & (xend + 1);
        lcd_write_cmd(lcd_self.setxcmd);
        lcd_write_data(databuf,4);

        databuf[0] = (ystar + 26) >> 8;
        databuf[1] = 0xFF & (ystar + 26);
        databuf[2] = (yend + 26) >> 8;
        databuf[3] = 0xFF & (yend + 26);
        lcd_write_cmd(lcd_self.setycmd);
        lcd_write_data(databuf,4);
    }
    else
    {
        databuf[0] = (xstar + 26) >> 8;
        databuf[1] = 0xFF & (xstar + 26);
        databuf[2] = (xend + 26) >> 8;
        databuf[3] = 0xFF & (xend + 26);
        lcd_write_cmd(lcd_self.setxcmd);
        lcd_write_data(databuf,4);

        databuf[0] = (ystar + 1) >> 8;
        databuf[1] = 0xFF & (ystar + 1);
        databuf[2] = (yend + 26) >> 8;
        databuf[3] = 0xFF & (yend + 26);
        lcd_write_cmd(lcd_self.setycmd);
        lcd_write_data(databuf,4);
    }

    lcd_write_cmd(lcd_self.wramcmd);    /* 开始写入GRAM */
}   

/**
 * @brief       以一种颜色清空LCD屏
 * @param       color 清屏颜色
 * @retval      无
 */
void lcd_clear(uint16_t color)
{
    uint16_t i, j;
    uint8_t data[2] = {0};

    data[0] = color >> 8;
    data[1] = color;
    
    lcd_set_window(0, 0, lcd_self.width - 1, lcd_self.height - 1);

    for(j = 0; j < LCD_BUF_SIZE / 2; j++)
    {
        lcd_buf[j * 2] =  data[0];
        lcd_buf[j * 2 + 1] =  data[1];
    }

    for(i = 0; i < (LCD_TOTAL_BUF_SIZE / LCD_BUF_SIZE); i++)
    {
        lcd_write_data(lcd_buf, LCD_BUF_SIZE);
    }
}

/**
 * @brief       在指定区域内填充单个颜色
 * @param       (sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex - sx + 1) * (ey - sy + 1)
 * @param       color:要填充的颜色(32位颜色,方便兼容LTDC)
 * @retval      无
 */
void lcd_fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color)
{
    uint16_t i;
    uint16_t j;
    uint16_t width;
    uint16_t height;

    width = ex - sx + 1;
    height = ey - sy + 1;
    lcd_set_window(sx, sy, ex, ey);

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            lcd_write_data16(color);
        }
    }
    lcd_set_window(sx, sy, ex, ey);
}


/**
 * @brief       设置光标的位置
 * @param       Xpos：左上角x轴
 * @param       Ypos：左上角y轴
 * @retval      无
 */
void lcd_set_cursor(uint16_t xpos, uint16_t ypos)
{
    lcd_set_window(xpos,ypos,xpos,ypos);
} 

/**
 * @brief       设置LCD的自动扫描方向(对RGB屏无效)
 * @param       dir:0~7,代表8个方向(具体定义见lcd.h)
 * @retval      无
 */
void lcd_scan_dir(uint8_t dir)
{
    uint8_t regval = 0;
    uint8_t dirreg = 0;
    uint16_t temp;

    /* 横屏时，对1963不改变扫描方向, 其他IC改变扫描方向！竖屏时1963改变方向, 其他IC不改变扫描方向 */
    if (lcd_self.dir == 1)
    {
        dir = 1;
    }
    else
    {
        dir = 0;
    }

    switch (dir)
    {
        case 0:                /* 竖屏右上角为0,0 */
            regval |= 0x08;
            break;

        case 1:                /* 横屏左上角为0,0 */
            regval |= 0xA8;
            break;

        case 2:                /* 竖屏左下角为0,0 */
            regval |= 0xC8;
            break;

        case 3:                /* 横屏右下角为0,0 */
            regval |= 0x78;
            break;
    }

    dirreg = 0x36;                              /* 对绝大部分驱动IC, 由0X36寄存器控制 */
    
    uint8_t date_send[1] = {regval};
    
    lcd_write_cmd(dirreg);
    lcd_write_data(date_send,1);
    
    if (regval & 0x20)
    {
        if (lcd_self.width < lcd_self.height)   /* 交换X,Y */
        {
            temp = lcd_self.width;
            lcd_self.width = lcd_self.height;
            lcd_self.height = temp;
        }
    }
    else
    {
        if (lcd_self.width > lcd_self.height)   /* 交换X,Y */
        {
            temp = lcd_self.width;
            lcd_self.width = lcd_self.height;
            lcd_self.height = temp;
        }
    }
    
    lcd_set_window(0, 0, lcd_self.width,lcd_self.height);
}

/**
 * @brief       设置LCD显示方向
 * @param       dir:0,竖屏; 1,横屏
 * @retval      无
 */
void lcd_display_dir(uint8_t dir)
{
    lcd_self.dir = dir;
    
    if (lcd_self.dir == 0)                  /* 竖屏 */
    {
        lcd_self.width      = 80;
        lcd_self.height     = 160;
        lcd_self.wramcmd    = 0X2C;
        lcd_self.setxcmd    = 0X2A;
        lcd_self.setycmd    = 0X2B;
    }
    else                                    /* 横屏 */
    {
        lcd_self.width      = 160;          /* 默认宽度 */
        lcd_self.height     = 80;           /* 默认高度 */
        lcd_self.wramcmd    = 0X2C;
        lcd_self.setxcmd    = 0X2A;
        lcd_self.setycmd    = 0X2B;
    }

    lcd_scan_dir(lcd_self.dir);             /* 默认扫描方向 */
}

/**
 * @brief       硬件复位
 * @param       self_in：LCD结构体
 * @retval      无
 */
void lcd_hard_reset(void)
{
    /* 复位显示屏 */
    LCD_RST(0);
    vTaskDelay(200);
    LCD_RST(1);
    vTaskDelay(200);
}

/**
 * @brief       绘画一个像素点
 * @param       self_in：LCD结构体
 * @param       x：x轴坐标
 * @param       y：y轴坐标
 * @param       color：颜色值
 * @retval      无
 */
void lcd_draw_pixel(uint16_t x, uint16_t y, uint16_t color)
{
    lcd_set_cursor(x, y);
    lcd_write_data16(color);
}

/**
 * @brief       画线函数(直线、斜线)
 * @param       x1,y1   起点坐标
 * @param       x2,y2   终点坐标
 * @param       color 填充颜色
 * @retval      无
 */
void lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    uint16_t t; 
    int xerr = 0, yerr = 0, delta_x, delta_y, distance; 
    
    int incx, incy, urow, ucol; 

    delta_x = x2 - x1;                      /* 计算坐标增量 */
    delta_y = y2 - y1; 
    urow = x1; 
    ucol = y1; 
    
    if (delta_x > 0)
    {
        incx = 1;                           /* 设置单步方向 */
    }
    else if (delta_x == 0)
    {
        incx = 0;                           /* 垂直线 */
    }
    else
    {
        incx =-1;
        delta_x =-delta_x;
    } 
    if(delta_y > 0)
    {
        incy = 1; 
    }
    else if(delta_y == 0)
    {
        incy = 0;                           /* 水平线 */
    }
    else
    {
        incy =-1;
        delta_y=-delta_y;
    } 
    
    if( delta_x>delta_y)
    {
        distance = delta_x;                 /* 选取基本增量坐标轴 */
    }
    else
    {
        distance = delta_y; 
    }
    
    for (t = 0;t <= distance + 1;t++ )      /* 画线输出 */
    {
        lcd_draw_pixel(urow,ucol,color);    /* 画点 */ 
        xerr += delta_x ; 
        yerr += delta_y ; 
        
        if(xerr>distance)
        { 
            xerr -= distance; 
            urow += incx; 
        } 
        
        if (yerr > distance)
        { 
            yerr -= distance; 
            ucol += incy; 
        } 
    } 
}

/**
 * @brief       画水平线
 * @param       x0,y0: 起点坐标
 * @param       len  : 线长度
 * @param       color: 矩形的颜色
 * @retval      无
 */
void lcd_draw_hline(uint16_t x, uint16_t y, uint16_t len, uint16_t color)
{
    if ((len == 0) || (x > lcd_self.width) || (y > lcd_self.height))return;

    lcd_fill(x, y, x + len - 1, y, color);
}

/**
 * @brief       画一个矩形
 * @param       x1,y1   起点坐标
 * @param       x2,y2   终点坐标
 * @param       color 填充颜色
 * @retval      无
 */
void lcd_draw_rectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,uint16_t color)
{
    lcd_draw_line(x0, y0, x1, y0,color);
    lcd_draw_line(x0, y0, x0, y1,color);
    lcd_draw_line(x0, y1, x1, y1,color);
    lcd_draw_line(x1, y0, x1, y1,color);
}

/**
 * @brief       画一个圆
 * @param       x0,y0   圆心坐标
 * @param       r   圆半径
 * @param       color 填充颜色
 * @retval      无
 */
void lcd_draw_circle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
{
    int a, b;
    int di;
    a = 0;
    b = r;
    di = 3 - (r << 1);

    while (a <= b)
    {
        lcd_draw_pixel(x0 - b, y0 - a, color);
        lcd_draw_pixel(x0 + b, y0 - a, color);
        lcd_draw_pixel(x0 - a, y0 + b, color);
        lcd_draw_pixel(x0 - b, y0 - a, color);
        lcd_draw_pixel(x0 - a, y0 - b, color);
        lcd_draw_pixel(x0 + b, y0 + a, color);
        lcd_draw_pixel(x0 + a, y0 - b, color);
        lcd_draw_pixel(x0 + a, y0 + b, color);
        lcd_draw_pixel(x0 - b, y0 + a, color);
        a++;

        if (di < 0)
        {
            di += 4 * a + 6;
        }
        else
        {
            di += 10 + 4 * (a - b);
            b--;
        }

        lcd_draw_pixel(x0 + a, y0 + b, color);
    }
}

/**
 * @brief       在指定位置显示一个字符
 * @param       x,y  : 坐标
 * @param       chr  : 要显示的字符:" "--->"~"
 * @param       size : 字体大小 12/16/24/32
 * @param       mode : 叠加方式(1); 非叠加方式(0);
 * @param       color : 字符的颜色;
 * @retval      无
 */
void lcd_show_char(uint16_t x, uint16_t y, uint8_t chr, uint8_t size, uint8_t mode, uint16_t color)
{
    uint8_t temp = 0,t1 = 0, t = 0;
    uint8_t *pfont = 0;
    uint8_t csize = 0;                                      /* 得到字体一个字符对应点阵集所占的字节数 */
    uint16_t colortemp = 0;
    uint8_t sta = 0;

    csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2); /* 得到字体一个字符对应点阵集所占的字节数 */
    chr = chr - ' ';                                        /* 得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库） */

    if ((x > (lcd_self.width - size / 2)) || (y > (lcd_self.height - size)))
    {
        return;
    }

    lcd_set_window(x, y, x + size / 2 - 1, y + size - 1);   /* (x,y,x+8-1,y+16-1) */

    switch (size)
    {
        case 12:
            pfont = (uint8_t *)asc2_1206[chr];              /* 调用1206字体 */
            break;

        case 16:
            pfont = (uint8_t *)asc2_1608[chr];              /* 调用1608字体 */
            break;

        case 24:
            pfont = (uint8_t *)asc2_2412[chr];              /* 调用2412字体 */
            break;

        case 32:
            pfont = (uint8_t *)asc2_3216[chr];              /* 调用3216字体 */
            break;

        default:
            return ;
    }

    if (size != 24)
    {
        csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);
        
        for (t = 0; t < csize; t++)
        {
            temp = pfont[t];                                /* 获取字符的点阵数据 */

            for (t1 = 0; t1 < 8; t1++)
            {
                    if (temp & 0x80)
                    {
                        colortemp = color;
                    }
                    else if (mode == 0)                     /* 无效点,不显示 */
                    {
                        colortemp = 0xFFFF;
                    }

                    lcd_write_data16(colortemp);
                    temp <<= 1;
            }
        }
    }
    else
    {
        csize = (size * 16) / 8;
        
        for (t = 0; t < csize; t++)
        {
            temp = asc2_2412[chr][t];

            if (t % 2 == 0)
            {
                sta = 8;
            }
            else
            {
                sta = 4;
            }

            for (t1 = 0; t1 < sta; t1++)
            {
                if(temp & 0x80)
                {
                    colortemp = color;
                }
                else if (mode == 0)                         /* 无效点,不显示 */
                {
                    colortemp = 0xFFFF;
                }

                lcd_write_data16(colortemp);
                temp <<= 1;
            }
        }
    }
}

/**
 * @brief       m^n函数
 * @param       m,n     输入参数
 * @retval      m^n次方
 */
uint32_t lcd_pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;

    while(n--)result *= m;

    return result;
}

/**
 * @brief       显示len个数字
 * @param       x,y : 起始坐标
 * @param       num : 数值(0 ~ 2^32)
 * @param       len : 显示数字的位数
 * @param       size: 选择字体 12/16/24/32
 * @retval      无
 */
void lcd_show_num(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint16_t color)
{
    uint8_t t, temp;
    uint8_t enshow = 0;

    for (t = 0; t < len; t++)                                               /* 按总显示位数循环 */
    {
        temp = (num / lcd_pow(10, len - t - 1)) % 10;                       /* 获取对应位的数字 */

        if (enshow == 0 && t < (len - 1))                                   /* 没有使能显示,且还有位要显示 */
        {
            if (temp == 0)
            {
                lcd_show_char(x + (size / 2)*t, y, ' ', size, 0, color);    /* 显示空格,占位 */
                continue;                                                   /* 继续下个一位 */
            }
            else
            {
                enshow = 1;                                                 /* 使能显示 */
            }

        }

        lcd_show_char(x + (size / 2)*t, y, temp + '0', size, 0, color);     /* 显示字符 */
    }
}

/**
 * @brief       扩展显示len个数字(高位是0也显示)
 * @param       x,y : 起始坐标
 * @param       num : 数值(0 ~ 2^32)
 * @param       len : 显示数字的位数
 * @param       size: 选择字体 12/16/24/32
 * @param       mode: 显示模式
 *              [7]:0,不填充;1,填充0.
 *              [6:1]:保留
 *              [0]:0,非叠加显示;1,叠加显示.
 * @param       color : 数字的颜色;
 * @retval      无
 */
void lcd_show_xnum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint8_t mode, uint16_t color)
{
    uint8_t t, temp;
    uint8_t enshow = 0;

    for (t = 0; t < len; t++)                                                           /* 按总显示位数循环 */
    {
        temp = (num / lcd_pow(10, len - t - 1)) % 10;                                   /* 获取对应位的数字 */

        if (enshow == 0 && t < (len - 1))                                               /* 没有使能显示,且还有位要显示 */
        {
            if (temp == 0)
            {
                if (mode & 0X80)                                                        /* 高位需要填充0 */
                {
                    lcd_show_char(x + (size / 2)*t, y, '0', size, mode & 0X01, color);  /* 用0占位 */
                }
                else
                {
                    lcd_show_char(x + (size / 2)*t, y, ' ', size, mode & 0X01, color);  /* 用空格占位 */
                }
                continue;
            }
            else
            {
                enshow = 1;                                                             /* 使能显示 */
            }
        }
        lcd_show_char(x + (size / 2)*t, y, temp + '0', size, mode & 0X01, color);
    }
}


/**
 * @brief       显示字符串
 * @param       x,y         : 起始坐标
 * @param       width,height: 区域大小
 * @param       size        : 选择字体 12/16/24/32
 * @param       p           : 字符串首地址
 * @retval      无
 */
void lcd_show_string(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, char *p, uint16_t color)
{
    uint8_t x0 = x;
    width += x;
    height += y;

    while ((*p <= '~') && (*p >= ' '))   /* 判断是不是非法字符! */
    {
        if (x >= width)
        {
            x = x0;
            y += size;
        }

        if (y >= height)break;  /* 退出 */

        lcd_show_char(x, y, *p, size, 0, color);
        x += size / 2;
        p++;
    }
}

/**
 * @brief       打开LCD
 * @param       self_in：SPI控制块
 * @retval      mp_const_none：初始化成功
 */
void lcd_on(void)
{
    LCD_PWR(1);
    vTaskDelay(10);
}

/**
 * @brief       关闭LCD
 * @param       self_in：SPI控制块
 * @retval      mp_const_none：初始化成功
 */
void lcd_off(void)
{
    LCD_PWR(0);
    vTaskDelay(10);
}

/**
 * @brief       LCD初始化
 * @param       无
 * @retval      无
 */
void lcd_init(void)
{
    int cmd = 0;
    esp_err_t ret = 0;
    
    lcd_self.dir = 0;
    lcd_self.wr = LCD_NUM_WR;                                       /* 配置WR引脚 */
    lcd_self.cs = LCD_NUM_CS;                                       /* 配置CS引脚 */
    lcd_self.bl = LCD_NUM_BL;                                       /* 配置BL引脚 */
    lcd_self.rst = LCD_NUM_RST;                                     /* 配置RST引脚 */

    gpio_config_t gpio_init_struct;

    /* SPI驱动接口配置 */
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 60 * 1000 * 1000,                         /* SPI时钟 */
        .mode = 0,                                                  /* SPI模式0 */
        .spics_io_num = lcd_self.cs,                                /* SPI设备引脚 */
        .queue_size = 7,                                            /* 事务队列尺寸 7个 */
    };
    
    /* 添加SPI总线设备 */
    ret = spi_bus_add_device(SPI2_HOST, &devcfg, &MY_LCD_Handle);   /* 配置SPI总线设备 */
    ESP_ERROR_CHECK(ret);

    /* WR管脚 */
    gpio_init_struct.intr_type = GPIO_INTR_DISABLE;                 /* 失能引脚中断 */
    gpio_init_struct.mode = GPIO_MODE_OUTPUT;                       /* 配置输出模式 */
    gpio_init_struct.pin_bit_mask = 1ull << lcd_self.wr ;           /* 配置引脚位掩码 */
    gpio_init_struct.pull_down_en = GPIO_PULLDOWN_DISABLE;          /* 失能下拉 */
    gpio_init_struct.pull_up_en = GPIO_PULLUP_ENABLE;               /* 使能上拉 */
    gpio_config(&gpio_init_struct);                                 /* 引脚配置 */
    /* BL管脚 */
    gpio_init_struct.intr_type = GPIO_INTR_DISABLE;                 /* 失能引脚中断 */
    gpio_init_struct.mode = GPIO_MODE_OUTPUT;                       /* 配置输出模式 */
    gpio_init_struct.pin_bit_mask = 1ull << lcd_self.bl;            /* 配置引脚位掩码 */
    gpio_init_struct.pull_down_en = GPIO_PULLDOWN_ENABLE;           /* 使能下拉 */
    gpio_init_struct.pull_up_en = GPIO_PULLUP_DISABLE;              /* 失能上拉 */
    gpio_config(&gpio_init_struct);                                 /* 引脚配置 */
    /* RST管脚 */
    gpio_init_struct.intr_type = GPIO_INTR_DISABLE;                 /* 失能引脚中断 */
    gpio_init_struct.mode = GPIO_MODE_OUTPUT;                       /* 配置输出模式 */
    gpio_init_struct.pin_bit_mask = 1ull << lcd_self.rst;           /* 配置引脚位掩码 */
    gpio_init_struct.pull_down_en = GPIO_PULLDOWN_DISABLE;          /* 失能下拉 */
    gpio_init_struct.pull_up_en = GPIO_PULLUP_ENABLE;               /* 使能上拉 */
    gpio_config(&gpio_init_struct);                                 /* 引脚配置 */
    lcd_off();
    lcd_hard_reset();                                               /* LCD硬件复位 */
    /* 0.96寸lcd屏幕初始化序列 */
    lcd_init_cmd_t ili_init_cmds[] =
    {
        {0x11, {0}, 0x80},
        {0x21, {0}, 0x80},
        {0xB1, {0x05,0x3A,0x3A}, 3},
        {0xB2, {0x05,0x3A,0x3A}, 3},
        {0xB3, {0x05,0x3A,0x3A,0x05,0x3A,0x3A}, 6},
        {0xB4, {0x03}, 1},
        {0xC0, {0x62,0x02,0x04}, 3},
        {0xC1, {0xC0}, 1},
        {0xC2, {0x0D,0x00}, 2},
        {0xC3, {0x8D,0x6A}, 2},
        {0xC4, {0x8D,0xEE}, 2},
        {0xC5, {0x0E}, 1},
        {0xE0, {0x10, 0x0E, 0x02, 0x03, 0x0E, 0x07, 0x02, 0x07, 0x0A, 0x12, 0x27, 0x37, 0x00, 0x0D , 0x0E , 0x10}, 16},
        {0xE1, {0x10, 0x0E, 0x03, 0x03, 0x0F, 0x06, 0x02, 0x08, 0x0A, 0x13, 0x26, 0x36, 0x00, 0x0D , 0x0E , 0x10}, 16},
        {0x3A, {0x05}, 1},
        {0x36, {0xA8}, 1},
        {0x29, {0}, 0x80},
        {0, {0}, 0xff},
    };

    /* 发送初始化序列 */
    while (ili_init_cmds[cmd].databytes != 0xff)
    {
        lcd_write_cmd(ili_init_cmds[cmd].cmd);
        lcd_write_data(ili_init_cmds[cmd].data, ili_init_cmds[cmd].databytes & 0x1F);
        
        if (ili_init_cmds[cmd].databytes & 0x80)
        {
            vTaskDelay(120);
        }
        
        cmd++;
    }

    lcd_display_dir(1);                                             /* 设置屏幕方向 */
    lcd_clear(WHITE);                                               /* 清屏 */
    lcd_on();
}
