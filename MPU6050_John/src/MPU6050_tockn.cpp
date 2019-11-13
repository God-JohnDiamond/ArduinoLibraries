#include "MPU6050_tockn.h"
#include "Arduino.h"

MPU6050::MPU6050(TwoWire &w){
  wire = &w;
  accCoef = 0.02f;
  gyroCoef = 0.98f;
}

MPU6050::MPU6050(TwoWire &w, float aC, float gC){
  wire = &w;
  accCoef = aC;
  gyroCoef = gC;
}

void MPU6050::begin(byte MPU6050_ADDR){
  writeMPU6050(MPU6050_ADDR, MPU6050_SMPLRT_DIV, 0x00);
  writeMPU6050(MPU6050_ADDR, MPU6050_CONFIG, 0x00);
  writeMPU6050(MPU6050_ADDR, MPU6050_GYRO_CONFIG, 0x08);
  writeMPU6050(MPU6050_ADDR, MPU6050_ACCEL_CONFIG, 0x00);
  writeMPU6050(MPU6050_ADDR, MPU6050_PWR_MGMT_1, 0x01);
  this->update(MPU6050_ADDR);
  angleGyroX = 0;
  angleGyroX_1 = 0;
  angleGyroY = 0;
  angleGyroY_1 = 0;
  angleX = this->getAccAngleX();
  angleX_1 = this->getAccAngleX_1();
  angleY = this->getAccAngleY();
  angleY_1 = this->getAccAngleY_1();
  preInterval = millis();
  preInterval_1 = millis();
}

void MPU6050::writeMPU6050(byte MPU6050_ADDR, byte reg, byte data){
  wire->beginTransmission(MPU6050_ADDR);
  wire->write(reg);
  wire->write(data);
  wire->endTransmission();
}

byte MPU6050::readMPU6050(byte MPU6050_ADDR, byte reg) {
  wire->beginTransmission(MPU6050_ADDR);
  wire->write(reg);
  wire->endTransmission(true);
  wire->requestFrom((int)MPU6050_ADDR, 1);
  byte data =  wire->read();
  return data;
}

void MPU6050::setGyroOffsets(byte MPU6050_ADDR, float x, float y, float z){
  if(MPU6050_ADDR == MPU6050_ADDR_GND){
	  gyroXoffset = x;
	  gyroYoffset = y;
	  gyroZoffset = z;
  }
  else if(MPU6050_ADDR == MPU6050_ADDR_VCC){
		  gyroXoffset_1 = x;
		  gyroYoffset_1 = y;
		  gyroZoffset_1 = z;
		}
}

void MPU6050::calcGyroOffsets(bool console, byte MPU6050_ADDR, uint16_t delayBefore, uint16_t delayAfter){
	float x = 0, y = 0, z = 0;
	int16_t rx, ry, rz;

  delay(delayBefore);
	if(console){
    Serial.println();
    Serial.println("========================================");
    Serial.println("Calculating gyro offsets");
    Serial.println("DO NOT MOVE MPU6050");
    Serial.print("NOW IS ");
	Serial.println(MPU6050_ADDR);
  }
  for(int i = 0; i < 3000; i++){
    if(console && i % 1000 == 0){
      Serial.print(".");
    }
    wire->beginTransmission(MPU6050_ADDR);
    wire->write(0x43);
    wire->endTransmission(false);
    wire->requestFrom((int)MPU6050_ADDR, 6);

    rx = wire->read() << 8 | wire->read();
    ry = wire->read() << 8 | wire->read();
    rz = wire->read() << 8 | wire->read();

    x += ((float)rx) / 65.5;
    y += ((float)ry) / 65.5;
    z += ((float)rz) / 65.5;
  }
  if(MPU6050_ADDR == MPU6050_ADDR_GND)
  {
	  gyroXoffset = x / 3000;
	  gyroYoffset = y / 3000;
	  gyroZoffset = z / 3000;
  }
  else if(MPU6050_ADDR == MPU6050_ADDR_VCC)
  {
	  gyroXoffset_1 = x / 3000;
	  gyroYoffset_1 = y / 3000;
	  gyroZoffset_1 = z / 3000;
  }

  if(console){
		if(MPU6050_ADDR == MPU6050_ADDR_GND){
			Serial.println();
			Serial.println("Done!");
			Serial.print("X : ");Serial.println(gyroXoffset);
			Serial.print("Y : ");Serial.println(gyroYoffset);
			Serial.print("Z : ");Serial.println(gyroZoffset);
			Serial.println("Just wait");
			Serial.print("========================================");
			Serial.println();
			delay(delayAfter);
		}
		else if(MPU6050_ADDR == MPU6050_ADDR_VCC){
			Serial.println();
			Serial.println("Done!");
			Serial.print("X_1 : ");Serial.println(gyroXoffset_1);
			Serial.print("Y_1 : ");Serial.println(gyroYoffset_1);
			Serial.print("Z_1 : ");Serial.println(gyroZoffset_1);
			Serial.println("Just wait");
			Serial.print("========================================");
			Serial.println();
			delay(delayAfter);
		}		
	}
}

void MPU6050::update(byte MPU6050_ADDR){
	wire->beginTransmission(MPU6050_ADDR);
	wire->write(0x3B);
	wire->endTransmission(false);
	wire->requestFrom((int)MPU6050_ADDR, 14);
	if(MPU6050_ADDR == MPU6050_ADDR_GND)
	{
	  rawAccX = wire->read() << 8 | wire->read();
	  rawAccY = wire->read() << 8 | wire->read();
	  rawAccZ = wire->read() << 8 | wire->read();
	  rawTemp = wire->read() << 8 | wire->read();
	  rawGyroX = wire->read() << 8 | wire->read();
	  rawGyroY = wire->read() << 8 | wire->read();
	  rawGyroZ = wire->read() << 8 | wire->read();

	  temp = (rawTemp + 12412.0) / 340.0;

	  accX = ((float)rawAccX) / 16384.0;
	  accY = ((float)rawAccY) / 16384.0;
	  accZ = ((float)rawAccZ) / 16384.0;

	  angleAccX = atan2(accY, accZ + abs(accX)) * 360 / 2.0 / PI;
	  angleAccY = atan2(accX, accZ + abs(accY)) * 360 / -2.0 / PI;

	  gyroX = ((float)rawGyroX) / 65.5;
	  gyroY = ((float)rawGyroY) / 65.5;
	  gyroZ = ((float)rawGyroZ) / 65.5;

	  gyroX -= gyroXoffset;
	  gyroY -= gyroYoffset;
	  gyroZ -= gyroZoffset;

	  interval = (millis() - preInterval) * 0.001;

	  angleGyroX += gyroX * interval;
	  angleGyroY += gyroY * interval;
	  angleGyroZ += gyroZ * interval;

	  angleX = (gyroCoef * (angleX + gyroX * interval)) + (accCoef * angleAccX);
	  angleY = (gyroCoef * (angleY + gyroY * interval)) + (accCoef * angleAccY);
	  angleZ = angleGyroZ;

	  preInterval = millis();
	}
	else if(MPU6050_ADDR == MPU6050_ADDR_VCC)
	{
	  rawAccX_1 = wire->read() << 8 | wire->read();
	  rawAccY_1 = wire->read() << 8 | wire->read();
	  rawAccZ_1 = wire->read() << 8 | wire->read();
	  rawTemp_1 = wire->read() << 8 | wire->read();
	  rawGyroX_1 = wire->read() << 8 | wire->read();
	  rawGyroY_1 = wire->read() << 8 | wire->read();
	  rawGyroZ_1 = wire->read() << 8 | wire->read();

	  temp_1 = (rawTemp_1 + 12412.0) / 340.0;

	  accX_1 = ((float)rawAccX_1) / 16384.0;
	  accY_1 = ((float)rawAccY_1) / 16384.0;
	  accZ_1 = ((float)rawAccZ_1) / 16384.0;

	  angleAccX_1 = atan2(accY_1, accZ_1 + abs(accX_1)) * 360 / 2.0 / PI;
	  angleAccY_1 = atan2(accX_1, accZ_1 + abs(accY_1)) * 360 / -2.0 / PI;

	  gyroX_1 = ((float)rawGyroX_1) / 65.5;
	  gyroY_1 = ((float)rawGyroY_1) / 65.5;
	  gyroZ_1 = ((float)rawGyroZ_1) / 65.5;

	  gyroX_1 -= gyroXoffset_1;
	  gyroY_1 -= gyroYoffset_1;
	  gyroZ_1 -= gyroZoffset_1;

	  interval_1 = (millis() - preInterval_1) * 0.001;

	  angleGyroX_1 += gyroX_1 * interval_1;
	  angleGyroY_1 += gyroY_1 * interval_1;
	  angleGyroZ_1 += gyroZ_1 * interval_1;

	  angleX_1 = (gyroCoef * (angleX_1 + gyroX_1 * interval_1)) + (accCoef * angleAccX_1);
	  angleY_1 = (gyroCoef * (angleY_1 + gyroY_1 * interval_1)) + (accCoef * angleAccY_1);
	  angleZ_1 = angleGyroZ_1;

	  preInterval_1 = millis();
	}
}
