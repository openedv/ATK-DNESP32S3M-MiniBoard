## key example

### 1 Brief

The main function of this code is to control the LED through the onboard KEY BOOT key.

### 2 Hardware Hookup

The hardware resources utilized in this experiment include:

- LED - IO1
- KEY - IO0

![](../../../../1_docs/3_figures/examples/key/key_sch.png)

The position of the KEY in the development board is shown as follows:

![](../../../../1_docs/3_figures/examples/key/key_position.png)

### 3 Running

#### 3.1 Compilation and Download

There are two ways to download code for ESP32S3.

##### 3.1.1 USB UART

![](../../../../1_docs/3_figures/examples/led/compilation(UART).png)

**1 Compilation process**

- Connect the USB UART on the DNESP32S3 mini board to your computer using a USB data cable
- Open the '02_key' example using VS Code
- Select UART port number(Figure �?:ESP-IDF: Select Port to Use (COM, tty, usbserial))
- Set Engineering Target Chip(Figure �?:ESP-IDF: Set Espressif Device Target)
- Clearing project engineering(Figure �?:ESP IDF: Full Clean)
- Select Flash Method(Figure �?:ESP-IDF: Select Flash Method)
- Build Project(Figure �?:ESP-IDF: Build Project)

**2 Download process**

- Download(Figure �?:ESP-IDF: Flash Device)

##### 3.1.2 JTAG(USB)

![](../../../../1_docs/3_figures/examples/led/compilation(JTAG).png)

**1 Compilation process**

- Connect the USB(JTAG) on the DNESP32S3 mini board to your computer using a USB data cable
- Open the '02_key' example using VS Code
- Select JTAG port number(Figure �?:ESP-IDF: Select Port to Use (COM, tty, usbserial))
- Clearing project engineering(Figure �?:ESP IDF: Full Clean)
- Select Flash Method(Figure �?:ESP-IDF: Select Flash Method)
- Build Project(Figure �?:ESP-IDF: Build Project)

**2 Download process**

- Download(Figure �?:ESP-IDF: Flash Device)

#### 3.2 Phenomenon

After normal operation, press the BOOT key to turn on the red LED.Otherwise, the red LED is off.

![]()