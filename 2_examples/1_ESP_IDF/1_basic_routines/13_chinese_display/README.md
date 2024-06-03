## chinese_display example

### 1 Brief

partition table loading chinese character library.

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

This example does not require any special hardware.

**NOTE:**Please copy the complete SYSTEM folder located in the path "2_examples/4_SD_card_mootdirectory_file" to the root directory of the SD card.

### 3 Running

#### 3.1 Compilation and Download

There are two ways to download code for ESP32S3.

##### 3.1.1 USB UART

![](../../../../1_docs/3_figures/examples/led/compilation(UART).png)

**1 Compilation process**

- Connect the USB UART on the DNESP32S3M mini board to your computer using a USB data cable
- Open the '13_chinese_display' example using VS Code
- Select UART port number (Figure â‘?:ESP-IDF: Select Port to Use (COM, tty, usbserial))
- Set Engineering Target Chip (Figure â‘?:ESP-IDF: Set Espressif Device Target)
- Clearing project engineering(Figure â‘?:ESP IDF: Full Clean)
- Select Flash Method (Figure â‘?:ESP-IDF: Select Flash Method)
- Build Project (Figure â‘?:ESP-IDF: Build Project)

**2 Download process**

- Download(Figure â‘?:ESP-IDF: Flash Device)

##### 3.1.2 JTAG(USB)

![](../../../../1_docs/3_figures/examples/led/compilation(JTAG).png)

**1 Compilation process**

- Connect the USB(JTAG) on the DNESP32S3M mini board to your computer using a USB data cable
- Open the '13_chinese_display' example using VS Code
- Select JTAG port number(Figure â‘?:ESP-IDF: Select Port to Use (COM, tty, usbserial))
- Clearing project engineering(Figure â‘?:ESP IDF: Full Clean)
- Select Flash Method(Figure â‘?:ESP-IDF: Select Flash Method)
- Build Project(Figure â‘?:ESP-IDF: Build Project)

**2 Download process**

- Download(Figure â‘?:ESP-IDF: Flash Device)

#### 3.2 Phenomenon

load chinese character library in partition table and display chinese characters on LCD.

![](../../../../1_docs/3_figures/examples/chinese_display/spilcd_phenomenon_idf.png)