#include <Arduino.h>
#include <Adafruit_BNO08x.h>
#include <config.h>


constexpr char PROGRAM_NAME[] = "Stage 1A UART Diagnostic";
constexpr char VERSION[]      = "0.1.0";
constexpr char BUILD_DATE[]   = __DATE__;
constexpr char BUILD_TIME[]   = __TIME__;

Adafruit_BNO08x  bno08x(PIN_BNO_RESET);
sh2_SensorValue_t sensorValue;

void setup(void) {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, PIN_BNO_RX, PIN_BNO_TX);
  while (!Serial) delay(10);     
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