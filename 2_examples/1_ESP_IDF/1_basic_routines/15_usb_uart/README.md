## usb_uart example

### 1 Brief

Learning USB simulated virtual serial port.

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

![](../../../../1_docs/3_figures/examples/usb_uart/usb_sch.png)

The position of the USB in the development board is shown as follows:

![](../../../../1_docs/3_figures/examples/usb_uart/usb_position.png)

### 3 Running

#### 3.1 Compilation and Download

There are two ways to download code for ESP32S3.

##### 3.1.1 USB UART

![](../../../../1_docs/3_figures/examples/led/compilation(UART).png)

**1 Compilation process**

- Connect the USB UART on the DNESP32S3M mini board to your computer using a USB data cable
- Open the '15_usb_uart' example using VS Code
- Select UART port number (Figure �?:ESP-IDF: Select Port to Use (COM, tty, usbserial))
- Set Engineering Target Chip (Figure �?:ESP-IDF: Set Espressif Device Target)
- Clearing project engineering（Figure �?:ESP IDF: Full Clean�?
- Select Flash Method (Figure �?:ESP-IDF: Select Flash Method)
- Build Project (Figure �?:ESP-IDF: Build Project)

**2 Download process**

- Download(Figure �?:ESP-IDF: Flash Device)

##### 3.1.2 JTAG(USB)

![](../../../../1_docs/3_figures/examples/led/compilation(JTAG).png)

**1 Compilation process**

- Connect the USB(JTAG) on the DNESP32S3M mini board to your computer using a USB data cable
- Open the '15_usb_uart' example using VS Code
- Select JTAG port number(Figure �?:ESP-IDF: Select Port to Use (COM, tty, usbserial))
- Clearing project engineering(Figure �?:ESP IDF: Full Clean)
- Select Flash Method(Figure �?:ESP-IDF: Select Flash Method)
- Build Project(Figure �?:ESP-IDF: Build Project)

**2 Download process**

- Download(Figure �?:ESP-IDF: Flash Device)

#### 3.2 Phenomenon

After the program runs, it will simulate a virtual serial port. After connecting to the PC via a data cable, data communication can be performed using a serial debugging assistant on the PC side. The development board side will utilize the USB virtual serial port to send and receive data in real-time, achieving efficient data interaction with the PC side.
