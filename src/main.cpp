#include <Arduino.h>
#include <Adafruit_BNO08x.h>
#include <config.h>
#include "compass.h"
#include <FastLED.h>

#define LED_PIN   13
#define LED_COUNT 45
CRGB leds[LED_COUNT];
int led=0;
float angle=0.0;

constexpr gpio_num_t PIN_BNO_RESET = GPIO_NUM_13;

Adafruit_BNO08x bno08x(PIN_BNO_RESET);

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
    
    Serial.println("Adafruit BNO08x Accelerometer test!");

    if (!bno08x.begin_UART(&Serial2))
    {
        fatalError(ErrorCode::BNO_NotFound);
    }

    Serial.println("BNO08x Found!");

    Compass::setReports(&bno08x, SH2_ROTATION_VECTOR, 100000);

    Serial.println("Reading events");
    delay(100);

    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_COUNT);
    FastLED.setBrightness(50);

}

void loop()
{
    compass.processSensor();
    Serial.print("Yaw  ");
    angle=compass.getYawNorthDeg();
    int led = (int)(angle * LED_COUNT / 360.0) % LED_COUNT;
    fill_solid(leds, LED_COUNT, CRGB::Black);
    leds[led] = CRGB::White;
    FastLED.show();
    Serial.println(angle);


}