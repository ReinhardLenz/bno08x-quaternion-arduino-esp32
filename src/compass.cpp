#include "compass.h"

namespace Compass
{
    static Adafruit_BNO08x* s_bno = nullptr;
    static sh2_SensorValue_t s_sensorValue;

    void begin(Adafruit_BNO08x& imu)
    {
        s_bno = &imu;
    }

    void setReports()
    {
        if (!s_bno) return;

        Serial.println("Setting desired reports");
        if (!s_bno->enableReport(SH2_ACCELEROMETER))
        {
            Serial.println("Could not enable accelerometer vector");
        }
    }

    void update()
    {
        if (!s_bno) return;

        delay(10);

        if (s_bno->wasReset())
        {
            Serial.print("sensor was reset ");
            setReports();
        }

        if (!s_bno->getSensorEvent(&s_sensorValue))
        {
            return;
        }

        switch (s_sensorValue.sensorId)
        {
            case SH2_ACCELEROMETER:
                // Values are intentionally NOT exposed to main.cpp.
                // Keep printing here (or remove if you want it silent).
                Serial.print("Accelerometer - x: ");
                Serial.print(s_sensorValue.un.accelerometer.x);
                Serial.print(" y: ");
                Serial.print(s_sensorValue.un.accelerometer.y);
                Serial.print(" z: ");
                Serial.println(s_sensorValue.un.accelerometer.z);
                break;

            default:
                break;
        }
    }
}