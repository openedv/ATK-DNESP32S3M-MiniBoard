## spiffs example

### 1 Brief

The main function of this code is learn partition table operations .

### 2 Hardware Hookup

The hardware resources used in this experiment are:

- None

This example does not require any special hardware.

### 3 Running

#### 3.1 Compilation and Download

There are two ways to download code for ESP32S3.

##### 3.1.1 USB UART

![](../../../../1_docs/3_figures/examples/led/compilation(UART).png)

**1 Compilation process**

- Connect the USB UART on the DNESP32S3M mini board to your computer using a USB data cable
- Open the '12_spiffs' example using VS Code
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
- Open the '12_spiffs' example using VS Code
- Select JTAG port number(Figure �?:ESP-IDF: Select Port to Use (COM, tty, usbserial))
- Clearing project engineering(Figure �?:ESP IDF: Full Clean)
- Select Flash Method(Figure �?:ESP-IDF: Select Flash Method)
- Build Project(Figure �?:ESP-IDF: Build Project)

**2 Download process**

- Download(Figure �?:ESP-IDF: Flash Device)

#### 3.2 Phenomenon

Performing read and write operations on partition tables.
