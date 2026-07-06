#include <Arduino.h>
#include <Adafruit_BNO08x.h>
#include <config.h>
#include "compass.h"

#define BNO08X_RESET -1
Adafruit_BNO08x bno08x(BNO08X_RESET);
Compass compass(bno08x);

constexpr char PROGRAM_NAME[] = "Stage 1A UART Diagnostic";
constexpr char VERSION[]      = "0.1.0";
constexpr char BUILD_DATE[]   = __DATE__;
constexpr char BUILD_TIME[]   = __TIME__;



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

void setup(void)
{
    Serial.begin(PC_BAUD);
    Serial2.begin(BNO_BAUD, SERIAL_8N1, PIN_BNO_RX, PIN_BNO_TX);

    while (!Serial)  delay(RESET_TIME_MS);
    while (!Serial2) delay(RESET_TIME_MS);

    Serial.println("Adafruit BNO08x Accelerometer test!");

    if (!bno08x.begin_UART(&Serial2))
    {
        fatalError(ErrorCode::BNO_NotFound);
    }

    Serial.println("BNO08x Found!");

    Compass::setReports(&bno08x, SH2_ROTATION_VECTOR, 100000);

    Serial.println("Reading events");
    delay(100);
}

void loop()
{
    compass.update();
}