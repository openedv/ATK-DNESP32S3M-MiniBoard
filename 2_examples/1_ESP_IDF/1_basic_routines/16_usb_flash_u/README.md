## usb_flash example

### 1 Brief

Virtualize the internal flash memory as a USB drive.

### 2 Hardware Hookup

The hardware resources used in this experiment are:

- UART0

  - TXD0 - IO43
  - RXD0 - IO44
- SPI_LCD
  - CS - IO21
  - SCK - IO12
  - SDA - IO11
  - DC - IO40
  - PWR - IO41
  - RST - IO38
- USB

  - D- - IO19
  - D+ - IO20

The hardware is consistent with the **usb_uart example**, and will not be described in detail here.

### 3 Running

#### 3.1 Compilation and Download

There are two ways to download code for ESP32S3.

##### 3.1.1 USB UART

![](../../../../1_docs/3_figures/examples/led/compilation(UART).png)

**1 Compilation process**

- Connect the USB UART on the DNESP32S3M mini board to your computer using a USB data cable
- Open the '16_usb_flash_u' example using VS Code
- Select UART port number (Figure â‘?:ESP-IDF: Select Port to Use (COM, tty, usbserial))
- Set Engineering Target Chip (Figure â‘?:ESP-IDF: Set Espressif Device Target)
- Clearing project engineeringï¼ˆFigure â‘?:ESP IDF: Full Cleanï¼?
- Select Flash Method (Figure â‘?:ESP-IDF: Select Flash Method)
- Build Project (Figure â‘?:ESP-IDF: Build Project)

**2 Download process**

- Download(Figure â‘?:ESP-IDF: Flash Device)

##### 3.1.2 JTAG(USB)

![](../../../../1_docs/3_figures/examples/led/compilation(JTAG).png)

**1 Compilation process**

- Connect the USB(JTAG) on the DNESP32S3M mini board to your computer using a USB data cable
- Open the '16_usb_flash_u' example using VS Code
- Select JTAG port number(Figure â‘?:ESP-IDF: Select Port to Use (COM, tty, usbserial))
- Clearing project engineering(Figure â‘?:ESP IDF: Full Clean)
- Select Flash Method(Figure â‘?:ESP-IDF: Select Flash Method)
- Build Project(Figure â‘?:ESP-IDF: Build Project)

**2 Download process**

- Download(Figure â‘?:ESP-IDF: Flash Device)

#### 3.2 Phenomenon

The system virtualizes the storage partition as a USB drive, which can be seen as a disk on the computer.

![](../../../../1_docs/3_figures/examples/usb_sd/spilcd_phenomenon_idf.png)