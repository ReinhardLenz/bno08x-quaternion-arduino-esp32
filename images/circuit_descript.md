# **Circuit Documentation**

## **Summary**

This circuit integrates an ESP32-CAM module, a CP2102 USB-to-UART bridge, and a BNO085 IMU sensor. The ESP32-CAM serves as the main microcontroller, interfacing with the BNO085 sensor to read orientation data. The CP2102 module is used for serial communication with a host computer, facilitating programming and debugging of the ESP32-CAM.

## **Component List**

1. **CP2102 USB-to-UART Bridge**  
   * **Description**: A USB-to-UART bridge that allows for serial communication between a USB host and a UART device.  
   * **Pins**: VCC IO, GND, TXD, RXD, RTS, CTS  
2. **ESP32-CAM**  
   * **Description**: A microcontroller module with integrated Wi-Fi and Bluetooth, equipped with a camera interface.  
   * **Pins**: 5V, GND, OI12, OI13, IO15, IO14, IO2, IO1, 3V3, IO16, IO0, VCC, UOR, UOT, GND/R  
3. **BNO085 IMU Sensor**  
   * **Description**: An inertial measurement unit (IMU) sensor capable of providing orientation data.  
   * **Pins**: VCC, GND, SCL/SCK/RX, SDA/MISO/TX, ADR/MOSI, CS, INT, RST, PS1, PS0  
4. **Comment V2**  
   * **Description**: A placeholder component for comments or notes in the circuit design.  
   * **Pins**: None

## **Wiring Details**

### **CP2102 USB-to-UART Bridge**

* **VCC IO** is connected to **ESP32-CAM 5V**.  
* **GND** is connected to **ESP32-CAM GND/R** and **BNO085 GND**.  
* **TXD** is connected to **ESP32-CAM UOR**.  
* **RXD** is connected to **ESP32-CAM UOT**.

### **ESP32-CAM**

* **5V** is connected to **CP2102 VCC IO**.  
* **GND/R** is connected to **CP2102 GND** and **BNO085 GND**.  
* **IO15** is connected to **BNO085 SDA/MISO/TX**.  
* **IO14** is connected to **BNO085 SCL/SCK/RX**.  
* **3V3** is connected to **BNO085 VCC**.  
* **UOR** is connected to **CP2102 TXD**.  
* **UOT** is connected to **CP2102 RXD**.  
* **IO0** is connected to **ESP32-CAM GND**.

### **BNO085 IMU Sensor**

* **VCC** is connected to **ESP32-CAM 3V3**.  
* **GND** is connected to **ESP32-CAM GND/R** and **CP2102 GND**.  
* **SDA/MISO/TX** is connected to **ESP32-CAM IO15**.  
* **SCL/SCK/RX** is connected to **ESP32-CAM IO14**.  
* **PS0** is connected to **ESP32-CAM GND/R**.  
* **PS1** is connected to **ESP32-CAM 3V3**.

## **Documented Code**

### **ESP32-CAM Code**

`#include "Adafruit_BNO08x_RVC.h"`

`HardwareSerial IMU(1);  // UART1`  
`Adafruit_BNO08x_RVC rvc;`

`void setup() {`  
  `Serial.begin(115200);`

  `// ESP32 UART: RX=16, TX=17 (you can change)`  
  `IMU.begin(115200, SERIAL_8N1, 16, 17);`

  `Serial.println("Starting BNO085 RVC...");`

  `if (!rvc.begin(&IMU)) {`  
    `Serial.println("Failed to find BNO085");`  
    `while (1);`  
  `}`

  `Serial.println("BNO085 ready!");`  
`}`

`void loop() {`  
  `BNO08x_RVC_Data data;`

  `if (!rvc.read(&data)) {`  
    `return;`  
  `}`

  `Serial.print("Yaw: ");`  
  `Serial.print(data.yaw);`  
  `Serial.print("  Pitch: ");`  
  `Serial.print(data.pitch);`  
  `Serial.print("  Roll: ");`  
  `Serial.println(data.roll);`  
`}`

This code initializes the ESP32-CAM to communicate with the BNO085 IMU sensor over UART. It reads orientation data (yaw, pitch, roll) from the sensor and outputs it to the serial monitor.