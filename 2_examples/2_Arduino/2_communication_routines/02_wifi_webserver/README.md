## wifi_webserver example

### 1 Brief

The main function of this code is to learn how to control led by using web.

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

The WiFi is an internal peripheral, so there is no corresponding connection schematic diagram.

### 3 Running

#### 3.1 Compile & Download

**1 Compilation process**

- Connect the UART interface in the upper left corner of the image to the computer using a USB cable
- 02_wifi_webserver routine opened using Arduino IDE2
- Select Target Board (① Arduino IDE: Tools --> Board --> esp32 --> ESP32S3 Dev Module)
- Select UART port number (② Arduino IDE: Tools --> port --> COMx)
- Set ESP32S3 parameter (③ Arduino IDE: Tools --> esp32s3 parameter(please see led example))
- Compile arduino project (④ Arduino IDE: "Sketch --> Verify/Compile" or "Ctrl+R" or "Verify button")
- Wait for compilation success (⑤ Arduino IDE: Output Window --> show "Done compiling")

**2 Download process**

- Download (⑥ Arduino IDE: "Sketch --> Upload" or "Ctrl+U" or "Upload button")

#### 3.2 Phenomenon

After normal operation, the wifi connected to the mobile phone must be **in the same frequency band** as the wifi connected to the ESP32S3 (such as 192.168.1.xx).Then enter the local IP address output by the serial console in the mobile web page to enter the LED of the mobile web page control board.

connect to wifi and wait for web access

<img src="../../../../1_docs/3_figures/examples/wifi_ble/wifi_webserver.png" style="zoom:50%;" />

access 192.168.1.247 on the web and press ON button

![](../../../../1_docs/3_figures/examples/wifi_ble/wifi_webserver.jpg)

the client is connected and has received a light on request
