#ifndef COMPASS_H
#define COMPASS_H

#include <Arduino.h>
#include <Adafruit_BNO08x.h>

namespace Compass
{
    // Initialize the compass module with an already-created BNO08x instance
    void begin(Adafruit_BNO08x& imu);

    // Enable desired sensor reports (e.g., accelerometer)
    void setReports();

    // Call repeatedly from loop(); handles reset + reading events
    void update();
}

#endif // COMPASS_H