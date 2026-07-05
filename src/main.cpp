#include <Arduino.h>
#include <Adafruit_BNO08x.h>

#define BNO08X_RESET -1
#define RX2 15
#define TX2 14

Adafruit_BNO08x  bno08x(BNO08X_RESET);
sh2_SensorValue_t sensorValue;

void setReports(void);

void setup(void) {
  Serial.begin(115200);
  Serial2.begin(3000000, SERIAL_8N1, RX2, TX2);
  while (!Serial) delay(10);     // will pause Zero, Leonardo, etc until serial console opens
  while (!Serial2) delay(10); 

  Serial.println("Adafruit BNO08x Accelerometer test!");

  // Try to initialize!
  if (!bno08x.begin_UART(&Serial2)) {  // Requires a device with > 300 byte UART buffer!
    Serial.println("Failed to find BNO08x chip");
    while (1) { delay(10); }
  }
  Serial.println("BNO08x Found!");

  setReports();

  Serial.println("Reading events");
  delay(100);
}

// Here is where you define the sensor outputs you want to receive
void setReports(void) {
  Serial.println("Setting desired reports");
  if (! bno08x.enableReport(SH2_ACCELEROMETER)) {
    Serial.println("Could not enable accelerometerame vector");
  }
}


void loop() {
  delay(10);

  if (bno08x.wasReset()) {
    Serial.print("sensor was reset ");
    setReports();
  }
  
  if (! bno08x.getSensorEvent(&sensorValue)) {
    return;
  }
  
  switch (sensorValue.sensorId) {
    
    case SH2_ACCELEROMETER:
      Serial.print("Accelerometer - x: ");
      Serial.print(sensorValue.un.accelerometer.x);
      Serial.print(" y: ");
      Serial.print(sensorValue.un.accelerometer.y);
      Serial.print(" z: ");
      Serial.println(sensorValue.un.accelerometer.z);
      break;
  }
}