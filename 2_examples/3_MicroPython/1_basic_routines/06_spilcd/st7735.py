"""
 ****************************************************************************************************
 * @file        main.py
 * @author      ALIENTEK
 * @brief       LCD Core
 * @license     Copyright (C) 2020-2030, ALIENTEK
 ****************************************************************************************************
 * @attention
 *
 * platform     : ALIENTEK DNESP32S3 board
 * website      : www.alientek.com
 * forum        : www.openedv.com/forum.php
 *
 * change logs  :
 * version      data         notes
 * V1.0         20240430     the first version
 *
 ****************************************************************************************************
"""

import machine
import time
from math import sqrt
import ustruct as struct

"""
 * @brief       RGB888 to RGB565
 * @param       R:red value
 * @param       G:green value
 * @param       B:blue value
 * @retval      RGB565
"""
def TFTColor( R, G, B ) :
    
    return ((R & 0xF8) << 8) | ((G & 0xFC) << 3) | (B >> 3)

# color
BLACK = TFTColor(0x00, 0x00, 0x00)
RED = TFTColor(0xFF, 0x00, 0x00)
MAROON = TFTColor(0x80, 0x00, 0x00)
GREEN = TFTColor(0x00, 0xFF, 0x00)
FOREST = TFTColor(0x00, 0x80, 0x80)
BLUE = TFTColor(0x00, 0x00, 0xFF)
NAVY = TFTColor(0x00, 0x00, 0x80)
CYAN = TFTColor(0x00, 0xFF, 0xFF)
YELLOW = TFTColor(0xFF, 0xFF, 0x00)
PURPLE = TFTColor(0xFF, 0x00, 0xFF)
WHITE = TFTColor(0xFF, 0xFF, 0xFF)
GRAY = TFTColor(0x80, 0x80, 0x80)

_ENCODE_PIXEL = ">H"
_BUFFER_SIZE = const(256)


"""
 * @brief       tft class
 * @param       None
 * @retval      obj
"""
class atk_tft(object):
    
    """
     * @brief       reset TFT
     * @param       None
     * @retval      None
    """
    def resrt(self):
        
        self.rst(1)
        time.sleep_ms(200)
        self.rst(0)
        time.sleep_ms(200)
        self.rst(1)
        time.sleep_ms(200)
    """
     * @brief       write cmd
     * @param       cmd_data:cmd
     * @retval      None
    """
    def write_cmd(self,cmd):
        
        self.dc(0)
        self.cs(0)
        self.spi.write(bytes([cmd]))
        self.cs(1)
    """
     * @brief       write data
     * @param       data:data
     * @retval      None
    """
    def write_data(self,data):
    
        self.dc(1)
        self.cs(0)
        self.spi.write(data)
        self.cs(1)
    """
     * @brief       set window
     * @param       xstar : The x-coordinate of end point 1 of the line to be drawn
     * @param       ystar : The y-coordinate of end point 1 of the line to be drawn
     * @param       xend  : The x-coordinate of end point 2 of the line to be drawn
     * @param       yend  : The y-coordinate of end point 2 of the line to be drawn
     * @retval      None
    """
    def set_window(self,xstar,ystar,xend,yend):
    
        if self.rotate == 1:
            self.window_data[0] = ((xstar + 1) >> 8)
            self.window_data[1] = (0xFF & (xstar + 1))
            self.window_data[2] = ((xend + 1) >> 8)
            self.window_data[3] = (0xFF & (xend + 1))
            self.write_cmd(self.setxcmd)
            self.write_data(self.window_data)
            
            self.window_data[0] = ((ystar + 26) >> 8)
            self.window_data[1] = (0xFF & (ystar + 26))
            self.window_data[2] = ((yend + 26) >> 8)
            self.window_data[3] = (0xFF & (yend + 26))
            self.write_cmd(self.setycmd)
            self.write_data(self.window_data)
        else:
            self.window_data[0] = (xstar + 26) >> 8
            self.window_data[1] = 0xFF & (xstar + 26)
            self.window_data[2] = (xend + 26) >> 8
            self.window_data[3] = 0xFF & (xend + 26)
            self.write_cmd(self.setxcmd)
            self.write_data(self.window_data)
            
            self.window_data[0] = (ystar + 1) >> 8
            self.window_data[1] = 0xFF & (ystar + 1)
            self.window_data[2] = (yend + 26) >> 8
            self.window_data[3] = 0xFF & (yend + 26)
            self.write_cmd(self.setycmd)
            self.write_data(self.window_data)
        self.write_cmd(self.wramcmd)
    """
     * @brief       set dir
     * @param       tf_dir:0,portrait; 1,landscape
     * @retval      None
    """
    def set_scan_dir(self,tf_dir):
    
        regval = 0
        dirreg = 0
        temp = 0
        
        if tf_dir == 1:
        
            regval |= 0xA8
            
        else:
            regval |= 0xC8
            
        dirreg = 0x36
        data_reg = bytearray([regval])
        
        self.write_cmd(dirreg)
        self.write_data(data_reg)
        
        if (regval & 0x20) == 1:
        
            if self.width < self.height :
                
                temp = self.width
                self.width = self.height
                self.height = temp
            else:
                if self.width > self.height:
                
                    temp = self.width
                    self.width = self.height
                    self.height = temp    
        self.set_window(0,0,self.width,self.height)
    """
     * @brief       vline
     * @param       x,y    : The x/y-coordinate of end point 1 of the line to be drawn
     * @param       length : length
     * @param       color  : color
     * @retval      None
    """
    def vline(self, x, y, length, color):
        self.fill_rect(x, y, 1, length, color)
    """
     * @brief       hline
     * @param       x,y    : The x/y-coordinate of end point 1 of the line to be drawn
     * @param       length : length
     * @param       color  : color
     * @retval      None
    """
    def hline(self, x, y, length, color):
        self.fill_rect(x, y, length, 1, color)
    """
     * @brief       pixel
     * @param       x,y    : The x/y-coordinate of end point 1 of the line to be drawn
     * @param       color  : color
     * @retval      None
    """
    def pixel(self, x, y, color):
        self.set_window(x, y, x, y)
        self.write_data(self._encode_pixel(color))
    """
     * @brief       rect
     * @param       x,y    : The x/y-coordinate of end point 1 of the line to be drawn
     * @param       w,h    : length
     * @param       color  : color
     * @retval      None
    """
    def rect(self, x, y, w, h, color):
        self.hline(x, y, w, color)
        self.vline(x, y, h, color)
        self.vline(x + w - 1, y, h, color)
        self.hline(x, y + h - 1, w, color)
    """
     * @brief       Encode pixel colors into bytes
     * @param       color  : color
     * @retval      None
    """
    def _encode_pixel(self, color):
        """Encode a pixel color into bytes."""
        return struct.pack(_ENCODE_PIXEL, color)
    """
     * @brief       fill rect
     * @param       x,y    : The x/y-coordinate of end point 1 of the line to be drawn
     * @param       w,h    : length
     * @param       color  : color
     * @retval      None
    """
    def fill_rect(self, x, y, width, height, color):
        self.set_window(x, y, x + width - 1, y + height - 1)
        chunks, rest = divmod(width * height, _BUFFER_SIZE)
        pixel = self._encode_pixel(color)
        self.dc(1)
        if chunks:
            data = pixel * _BUFFER_SIZE
            for _ in range(chunks):
                self.write_data(data)
        if rest:
            self.write_data(pixel * rest)
    """
     * @brief       clear
     * @param       color  : color
     * @retval      None
    """
    def clear(self, color):
        self.fill_rect(0, 0, self.width, self.height, color)
    """
     * @brief       bmp
     * @param       x,y    : The x/y-coordinate of end point 1 of the line to be drawn
     * @param       w,h    : length
     * @param       buffer : bmp data
     * @retval      None
    """
    def draw_bmp(self,x,y,w,h,buffer):
        if((x >= self.width) or (y >= self.height)):
            return
        if (x + w - 1) >= self.width:
            w = self.width  - x
        if (y + h - 1) >= self.height:
            h = self.height - y
        self.set_window(x,y,x+w-1,y+h-1)
        self.dc(1)
        self.cs(0)
        self.write_data(buffer)     # write bytes on MOSI
        self.cs(1)
    """
     * @brief       char
     * @param       x,y    : The x/y-coordinate of end point 1 of the line to be drawn
     * @param       ch     : char
     * @retval      None
    """
    def p_char(self, x, y, ch):
        fp = (ord(ch)-0x20) * 5
        f = open('font5x7.fnt','rb')
        f.seek(fp)
        b = f.read(5)
        char_buf = bytearray(b)
        char_buf.append(0)

        # make 8x6 image
        char_image = bytearray()
        for bit in range(8):
            for c in range (6):
                if ((char_buf[c]>>bit) & 1)>0:
                    char_image.append(self._color >> 8)
                    char_image.append(self._color & 0xff)
                else:
                    char_image.append(self._bground >> 8)
                    char_image.append(self._bground & 0xff)
        self.draw_bmp(x,y,6,8,char_image)
    """
     * @brief       string
     * @param       x,y    : The x/y-coordinate of end point 1 of the line to be drawn
     * @param       str    : string
     * @retval      None
    """
    def p_string(self, x, y, str):

        for ch in (str):
            self.p_char(x, y, ch)
            x += 6
    """
     * @brief       bmp
     * @param       fname  : .bmp file
     * @param       x,y    : The x/y-coordinate of end point 1 of the line to be drawn
     * @param       color  : color
     * @retval      None
    """    
    def bmp(self,fname, x, y, color = 0):
        
        f = open(fname,'rb')
        b = bytearray(54)
        b = f.read(54)
        # header check
        if b[0] == 0x42 and b[1] == 0x4D:
            # is bitmap
            size = b[2] + (b[3]<<8) + (b[4]<<16) +(b[5]<<24)
            offset = b[10] + (b[11]<<8) + (b[12]<<16) +(b[13]<<24)
            width = b[18] + (b[19]<<8) + (b[20]<<16) +(b[21]<<24)
            height = b[22] + (b[23]<<8) + (b[24]<<16) +(b[25]<<24)
            color_planes = b[26] + (b[27]<<8)
            bits_per_pixel = b[28] + (b[29]<<8)
            compression = b[30] + (b[31]<<8) + (b[32]<<16) +(b[33]<<24)
            image_size = b[34] + (b[35]<<8) + (b[36]<<16) +(b[37]<<24)
            
            f.seek(offset)
            
            row_bytes = int(bits_per_pixel/8) * width
            # Add up to multiple of 4
            if row_bytes % 4 > 0:
                row_bytes += 4 - row_bytes % 4
            
            buffer = bytearray(row_bytes)
            for row in range(height):
                # print(row)
                # read in a whole row
                buffer=f.read(row_bytes)
                d_buffer = bytearray(width*2)
                index = 0
                for index in range(width):
                    y1 = (height-1) - row + y
                    if color:
                        b = buffer[index*3]
                        g = buffer[index*3+1]
                        r = buffer[index*3+2]
                        c = TFTColor(r,g,b)
                        d_buffer[index*2] = c >> 8
                        d_buffer[index*2+1] = c & 0xff
                    else:
                        if buffer[index*3]!=0xff:
                            self.pixel(x,y,1)
                if color:
                    self.draw_bmp(x,y1,width,1,d_buffer)
        f.close()
    """
     * @brief       tft init
     * @param       None
     * @retval      None
    """  
    def tft_init(self):
        
        self.write_cmd(0x11)
        time.sleep_ms(120)
        self.write_cmd(0x21)
        time.sleep_ms(120)
        data1 = bytearray([0x05,0x3A,0x3A])
        self.write_cmd(0xB1)
        self.write_data(data1)
        self.write_cmd(0xB2)
        self.write_data(data1)
        data2 = bytearray([0x05,0x3A,0x3A,0x05,0x3A,0x3A])
        self.write_cmd(0xB3)
        self.write_data(data2)
        self.write_cmd(0xB4)
        data_reg = bytearray([0x03])
        self.write_data(data_reg)
        data3 = bytearray([0x62,0x02,0x04])
        self.write_cmd(0xC0)
        self.write_data(data3)
        self.write_cmd(0xC1)
        data_reg[0] = 0xC0
        self.write_data(data_reg)
        data4 = bytearray([0x0D,0x00])
        self.write_cmd(0xC2)
        self.write_data(data4)
        data5 = bytearray([0x8D,0x6A])
        self.write_cmd(0xC3)
        self.write_data(data5)
        data6 = bytearray([0x8D,0xEE])
        self.write_cmd(0xC4)
        self.write_data(data6)
        self.write_cmd(0xC5)
        data_reg[0] = 0x0E
        self.write_data(data_reg)
        data7 = bytearray([0x10, 0x0E, 0x02, 0x03, 0x0E, 0x07, 0x02, 0x07, 0x0A, 0x12, 0x27, 0x37, 0x00, 0x0D , 0x0E , 0x10])
        self.write_cmd(0xE0)
        self.write_data(data7)
        data8 = bytearray([0x10, 0x0E, 0x03, 0x03, 0x0F, 0x06, 0x02, 0x08, 0x0A, 0x13, 0x26, 0x36, 0x00, 0x0D , 0x0E , 0x10])
        self.write_cmd(0xE1)
        self.write_data(data8)
        self.write_cmd(0x3A)
        data_reg[0] = 0x05
        self.write_data(data_reg)
        self.write_cmd(0x36)
        data_reg[0] = 0xA8
        self.write_data(data_reg)
        self.write_cmd(0x29)
        time.sleep_ms(120)

    def __init__(self,spi,dc,rst,cs,bl,rotate = 0):
    
        self.rotate = rotate
        self.window_data = bytearray(4)
        self.spi = spi
        self._offset = bytearray([0,0])
        self._color = RED
        self._bground = WHITE

        self.dc = machine.Pin(dc,machine.Pin.OUT)
        self.rst = machine.Pin(rst,machine.Pin.OUT)
        self.cs = machine.Pin(cs,machine.Pin.OUT)
        self.bl = machine.Pin(bl,machine.Pin.OUT)
        self.resrt()
        
        if self.rotate == 0:
           
           self.height = 160
           self.width = 80
           self.wramcmd = 0x2C
           self.setxcmd = 0x2A
           self.setycmd = 0x2B
        else:
            
           self.height = 80
           self.width = 160
           self.wramcmd = 0x2C
           self.setxcmd = 0x2A
           self.setycmd = 0x2B
            
        self.tft_init()
        self.set_scan_dir(self.rotate)
        self.bl(1)
        self.clear(WHITE)
