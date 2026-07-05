#include <Arduino.h>
#include <Adafruit_BNO08x.h>
#include <config.h>
#define BNO08X_RESET -1

constexpr char PROGRAM_NAME[] = "Stage 1A UART Diagnostic";
constexpr char VERSION[]      = "0.1.0";
constexpr char BUILD_DATE[]   = __DATE__;
constexpr char BUILD_TIME[]   = __TIME__;

Adafruit_BNO08x  bno08x(BNO08X_RESET);
sh2_SensorValue_t sensorValue;

enum class ErrorCode
{
    None,
    UART,
    BNO_NotFound,
    EnableReport,
    ProductID,
};

void fatalError(ErrorCode code)
{
    Serial.println();
    Serial.println("========== FATAL ERROR ==========");

    switch (code)
    {
        case ErrorCode::UART:
            Serial.println("Unable to communicate with BNO085.");
            break;

        case ErrorCode::BNO_NotFound:
            Serial.println("BNO085 not detected.");
            break;

        case ErrorCode::EnableReport:
            Serial.println("Could not enable report.");
            break;

        default:
            Serial.println("Unknown error.");
            break;
    }

    while (true)
    {
        digitalWrite(PIN_STATUS_LED, HIGH);
        delay(BLINK_DELAY);
        digitalWrite(PIN_STATUS_LED, LOW);
        delay(BLINK_DELAY);
    }
}

void setReports(void);

void setup(void) {
  Serial.begin(PC_BAUD);
  Serial2.begin(BNO_BAUD, SERIAL_8N1, PIN_BNO_RX, PIN_BNO_TX);
  while (!Serial) delay(RESET_TIME_MS);     
  while (!Serial2) delay(RESET_TIME_MS); 

  Serial.println("Adafruit BNO08x Accelerometer test!");

  // Try to initialize!
  if (!bno08x.begin_UART(&Serial2)) {  
    fatalError(ErrorCode::BNO_NotFound);
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