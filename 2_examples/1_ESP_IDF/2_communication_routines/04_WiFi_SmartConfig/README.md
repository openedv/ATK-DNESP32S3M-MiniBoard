## wifi smartconfig example

### 1 Brief

Learn about one-click network configuration.

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

- Connect the USB UART on the DNESP32S3 mini board to your computer using a USB data cable
- Open the '04_WiFi_SmartConfig' example using VS Code
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

- Connect the USB(JTAG) on the DNESP32S3 mini board to your computer using a USB data cable
- Open the '04_WiFi_SmartConfig' example using VS Code
- Select JTAG port number (Figure â‘?:ESP-IDF: Select Port to Use (COM, tty, usbserial))
- Clearing project engineeringï¼ˆFigure â‘?:ESP IDF: Full Cleanï¼?
- Select Flash Method (Figure â‘?:ESP-IDF: Select Flash Method)
- Build Project (Figure â‘?:ESP-IDF: Build Project)

**2 Download process**

- Downloadï¼ˆFigure â‘?:ESP-IDF: Flash Deviceï¼?

#### 3.2 Phenomenon

After normal operation,Configuring network using [EspTouch](https://www.espressif.com.cn/en/support/download/apps) mobile app.

1.Download the EspTouch app software.

![](../../../../1_docs/3_figures/examples/wifi_smartconfig/01_download_esp-touch.png)

2.After successfully downloading, you need to transfer the installation package to your Android or iOS device for installation.

3.After successfully downloading the program, open the 'EspTouch' app. In the app, click on the 'EspTouch' option. Note: Your phone must be connected to WiFi for one-click network configuration to work.

![](../../../../1_docs/3_figures/examples/wifi_smartconfig/02_configuration_to_work.png)

4.When the DNESP32S3 Mini board receives this message, the system will extract the SSID and password from it to connect to that network.