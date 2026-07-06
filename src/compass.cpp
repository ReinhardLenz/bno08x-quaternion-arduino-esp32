#include "compass.h"

Compass::Compass(Adafruit_BNO08x& imu)
: m_imu(imu)
{
}
/*
void Compass::setReports()
{
    Serial.println("Setting desired reports");
    if (!m_imu.enableReport(SH2_ACCELEROMETER))
    {
        Serial.println("Could not enable accelerometer vector");
    }
}


void Compass::setReports()
{
  Serial.println("Setting desired reports");

  // choose what you want enabled:
  // Example: game rotation vector at 100ms
  Compass::setReports(&m_imu, SH2_GAME_ROTATION_VECTOR, 100000); // microseconds

  // or rotation vector:
  // Compass::setReports(&m_imu, SH2_ROTATION_VECTOR, 100000);
}
*/

void Compass::setReports(Adafruit_BNO08x* imu, sh2_SensorId_t reportType, long report_interval)
{
  
  if (!imu->enableReport(reportType, report_interval)) {
    Serial.println("Could not enable report");
  }
}

void Compass::quaternionToEuler(float qr, float qi, float qj, float qk, euler_t* ypr, bool degrees) {
float sqr = qr * qr;
float sqi = qi * qi;
float sqj = qj * qj;
float sqk = qk * qk;

  ypr->yaw   = atan2f(2.0f * (qi * qj + qk * qr), (sqi - sqj - sqk + sqr));
  ypr->pitch = asinf(-2.0f * (qi * qk - qj * qr) / (sqi + sqj + sqk + sqr));
  ypr->roll  = atan2f(2.0f * (qj * qk + qi * qr), (-sqi - sqj + sqk + sqr));

  if (degrees) {
    ypr->yaw   *= RAD_TO_DEG;
    ypr->pitch *= RAD_TO_DEG;
    ypr->roll  *= RAD_TO_DEG;
    Serial.print("Yaw: ");
    Serial.print(ypr->yaw);
  }
}

void Compass::quaternionToEulerRV(sh2_RotationVectorWAcc_t* rv, euler_t* ypr, bool degrees) {
  quaternionToEuler(rv->real, rv->i, rv->j, rv->k, ypr, degrees);
}
void Compass::quaternionToEulerGI(sh2_GyroIntegratedRV_t* gi, euler_t* ypr, bool degrees) {
  quaternionToEuler(gi->real, gi->i, gi->j, gi->k, ypr, degrees);
}

float Compass::getNorthDirection(float yaw) {
  return (yaw < 0) ? (360.0f + yaw) : yaw;
}


void Compass::update()
{
    delay(10);

    if (m_imu.wasReset())
    {
        Serial.print("sensor was reset ");
        setReports(&m_imu, SH2_ROTATION_VECTOR, 100000);
    }

    if (!m_imu.getSensorEvent(&sensorValue_1))
    {
        return;
    }


  switch (sensorValue_1.sensorId) {
    
    case SH2_ROTATION_VECTOR:
    quaternionToEulerRV(&sensorValue_1.un.rotationVector, &ypr_1, true);
    break;

    case SH2_GAME_ROTATION_VECTOR: {
        // gameRotationVector has no accuracy field type; use raw components
        const auto& grv = sensorValue_1.un.gameRotationVector;
        quaternionToEuler(grv.real, grv.i, grv.j, grv.k, &ypr_1, true);
        break;
      }

    case SH2_GYRO_INTEGRATED_RV:
        quaternionToEulerGI(&sensorValue_1.un.gyroIntegratedRV, &ypr_1, true);
        break;

    default:
      // leave ypr_1 unchanged
    break;

  }
    float absDir   = getNorthDirection(ypr_1.yaw);
//    Serial.println(absDir);
}

