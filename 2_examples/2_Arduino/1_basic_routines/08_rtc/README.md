## rtc example

Note: rtc example need to install "ESP32Time" LIBRARY.

![](../../../../1_docs/3_figures/examples/rtc/rtc_lib.png)

### 1 Brief

The main function of this code is to display time by using RTC peripherals.

### 2 Hardware Hookup

The hardware resources used in this experiment are:

- LED - IO1
- UART0
	- TXD0 - IO43
	- RXD0 - IO44
- SPI_LCD
	- CS - IO39
	- SCK - IO12
	- SDA - IO11
	- DC - IO40
	- BL - IO41
	- RST - IO38

The RTC is an internal peripheral, so there is no corresponding connection schematic diagram.

### 3 Running

#### 3.1 Compile & Download

**1 Compilation process**

- Connect the UART interface in the upper left corner of the image to the computer using a USB cable
- 08_rtc routine opened using Arduino IDE2
- Select Target Board (① Arduino IDE: Tools --> Board --> esp32 --> ESP32S3 Dev Module)
- Select UART port number (② Arduino IDE: Tools --> port --> COMx)
- Set ESP32S3 parameter (③ Arduino IDE: Tools --> esp32s3 parameter(please see led example))
- Compile arduino project (④ Arduino IDE: "Sketch --> Verify/Compile" or "Ctrl+R" or "Verify button")
- Wait for compilation success (⑤ Arduino IDE: Output Window --> show "Done compiling")

**2 Download process**

- Download (⑥ Arduino IDE: "Sketch --> Upload" or "Ctrl+U" or "Upload button")

#### 3.2 Phenomenon

After normal operation, the spilcd will display time and date information.

<img src="../../../../1_docs/3_figures/examples/rtc/rtc_phe.png" style="zoom:50%;" />