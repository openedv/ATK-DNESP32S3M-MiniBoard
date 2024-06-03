## wifi sta example

### 1 Brief

To connect to a WiFi hotspot.

### 2 Hardware Hookup

The hardware resources used in this experiment are:

- LED - IO1

- LCD - 0.96 inch screen
  - CS - IO21
  - SCK - IO12
  - SDA - IO11
  - DC - IO40
  - PWR - IO41
  - RST - IO38


The WiFi is an internal peripheral, so there is no corresponding connection schematic diagram.

### 3 Running

#### 3.1 Compilation and Download

There are two ways to download code for ESP32S3.

##### 3.1.1 USB UART

![](../../../../1_docs/3_figures/examples/led/compilation(UART).png)

**1 Compilation process**

- Connect the USB UART on the DNESP32S3 development board to your computer using a USB data cable
- Open the '02_WiFi_STA' example using VS Code
- Select UART port number (Figure â‘?:ESP-IDF: Select Port to Use (COM, tty, usbserial))
- Set Engineering Target Chip (Figure â‘?:ESP-IDF: Set Espressif Device Target)
- Clearing project engineeringï¼ˆFigure â‘?:ESP IDF: Full Cleanï¼?
- Select Flash Method (Figure â‘?:ESP-IDF: Select Flash Method)
- Build Project (Figure â‘?:ESP-IDF: Build Project)

**2 Download process**

- Downloadï¼ˆFigure â‘?:ESP-IDF: Flash Deviceï¼?

##### 3.1.2 JTAG(USB)

![](../../../../1_docs/3_figures/examples/led/compilation(JTAG).png)

**1 Compilation process**

- Connect the USB(JTAG) on the DNESP32S3 development board to your computer using a USB data cable
- Open the '02_WiFi_STA' example using VS Code
- Select JTAG port number (Figure â‘?:ESP-IDF: Select Port to Use (COM, tty, usbserial))
- Clearing project engineeringï¼ˆFigure â‘?:ESP IDF: Full Cleanï¼?
- Select Flash Method (Figure â‘?:ESP-IDF: Select Flash Method)
- Build Project (Figure â‘?:ESP-IDF: Build Project)

**2 Download process**

- Downloadï¼ˆFigure â‘?:ESP-IDF: Flash Deviceï¼?

#### 3.2 Phenomenon

After normal operation,To display the account and password for connecting to the WiFi hotspot on the LCD.
