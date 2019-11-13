#ifndef MPU6050_TOCKN_H
#define MPU6050_TOCKN_H

#include "Arduino.h"
#include "Wire.h"

#define MPU6050_ADDR_GND     0x68
#define MPU6050_ADDR_VCC     0x69
#define MPU6050_SMPLRT_DIV   0x19
#define MPU6050_CONFIG       0x1a
#define MPU6050_GYRO_CONFIG  0x1b
#define MPU6050_ACCEL_CONFIG 0x1c
#define MPU6050_WHO_AM_I     0x75
#define MPU6050_PWR_MGMT_1   0x6b
#define MPU6050_TEMP_H       0x41
#define MPU6050_TEMP_L       0x42

class MPU6050{
  public:

  MPU6050(TwoWire &w);
  MPU6050(TwoWire &w, float aC, float gC);

  void begin(byte MPU6050_ADDR);

  void setGyroOffsets(byte MPU6050_ADDR, float x, float y, float z);

  void writeMPU6050(byte MPU6050_ADDR, byte reg, byte data);
  byte readMPU6050(byte MPU6050_ADDR, byte reg);

  int16_t getRawAccX(){ return rawAccX; };
  int16_t getRawAccY(){ return rawAccY; };
  int16_t getRawAccZ(){ return rawAccZ; };
  int16_t getRawTemp(){ return rawTemp; };
  int16_t getRawGyroX(){ return rawGyroX; };
  int16_t getRawGyroY(){ return rawGyroY; };
  int16_t getRawGyroZ(){ return rawGyroZ; };
  
  int16_t getRawAccX_1(){ return rawAccX_1; };
  int16_t getRawAccY_1(){ return rawAccY_1; };
  int16_t getRawAccZ_1(){ return rawAccZ_1; };
  int16_t getRawTemp_1(){ return rawTemp_1; };
  int16_t getRawGyroX_1(){ return rawGyroX_1; };
  int16_t getRawGyroY_1(){ return rawGyroY_1; };
  int16_t getRawGyroZ_1(){ return rawGyroZ_1; };


  float getTemp(){ return temp; };
  float getAccX(){ return accX; };
  float getAccY(){ return accY; };
  float getAccZ(){ return accZ; };
  float getGyroX(){ return gyroX; };
  float getGyroY(){ return gyroY; };
  float getGyroZ(){ return gyroZ; };

  float getTemp_1(){ return temp_1; };
  float getAccX_1(){ return accX_1; };
  float getAccY_1(){ return accY_1; };
  float getAccZ_1(){ return accZ_1; };
  float getGyroX_1(){ return gyroX_1; };
  float getGyroY_1(){ return gyroY_1; };
  float getGyroZ_1(){ return gyroZ_1; };


  void calcGyroOffsets(bool console = false, byte MPU6050_ADDR = 0x68, uint16_t delayBefore = 1000, uint16_t delayAfter = 3000);

  float getGyroXoffset(){ return gyroXoffset; };
  float getGyroYoffset(){ return gyroYoffset; };
  float getGyroZoffset(){ return gyroZoffset; };

  float getGyroXoffset_1(){ return gyroXoffset_1; };
  float getGyroYoffset_1(){ return gyroYoffset_1; };
  float getGyroZoffset_1(){ return gyroZoffset_1; };


  void update(byte MPU6050_ADDR);

  float getAccAngleX(){ return angleAccX; };
  float getAccAngleY(){ return angleAccY; };

  float getGyroAngleX(){ return angleGyroX; };
  float getGyroAngleY(){ return angleGyroY; };
  float getGyroAngleZ(){ return angleGyroZ; };

  float getAngleX(){ return angleX; };
  float getAngleY(){ return angleY; };
  float getAngleZ(){ return angleZ; };

  float getAccAngleX_1(){ return angleAccX_1; };
  float getAccAngleY_1(){ return angleAccY_1; };

  float getGyroAngleX_1(){ return angleGyroX_1; };
  float getGyroAngleY_1(){ return angleGyroY_1; };
  float getGyroAngleZ_1(){ return angleGyroZ_1; };

  float getAngleX_1(){ return angleX_1; };
  float getAngleY_1(){ return angleY_1; };
  float getAngleZ_1(){ return angleZ_1; };


  private:

  TwoWire *wire;

  int16_t rawAccX, rawAccY, rawAccZ, rawTemp, rawGyroX, rawGyroY, rawGyroZ;
  int16_t rawAccX_1, rawAccY_1, rawAccZ_1, rawTemp_1, rawGyroX_1, rawGyroY_1, rawGyroZ_1;

  float gyroXoffset, gyroYoffset, gyroZoffset;
  float gyroXoffset_1, gyroYoffset_1, gyroZoffset_1;

  float temp, accX, accY, accZ, gyroX, gyroY, gyroZ;
  float temp_1, accX_1, accY_1, accZ_1, gyroX_1, gyroY_1, gyroZ_1;

  float angleGyroX, angleGyroY, angleGyroZ, angleAccX, angleAccY, angleAccZ;
  float angleGyroX_1, angleGyroY_1, angleGyroZ_1, angleAccX_1, angleAccY_1, angleAccZ_1;

  float angleX, angleY, angleZ;
  float angleX_1, angleY_1, angleZ_1;

  float interval;
  long preInterval;
  
  float interval_1;
  long preInterval_1;


  float accCoef, gyroCoef;
};

#endif
