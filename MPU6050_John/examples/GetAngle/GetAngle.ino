
#include <MPU6050_tockn.h>
#include <Wire.h>

MPU6050 mpu6050(Wire);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu6050.begin(MPU6050_ADDR_GND);
  mpu6050.calcGyroOffsets(true,MPU6050_ADDR_GND);  
  mpu6050.begin(MPU6050_ADDR_VCC);
  mpu6050.calcGyroOffsets(true,MPU6050_ADDR_VCC);
}

void loop() {
  //Print 2IMUs' X Angle
  mpu6050.update(MPU6050_ADDR_GND);
  float AngleX_GND = mpu6050.getAngleX();
  Serial.print("AngleX_GND_0x68 : ");
  Serial.print(AngleX_GND);
  Serial.print("\t");
  
  mpu6050.update(MPU6050_ADDR_VCC);
  float AngleX_VCC = mpu6050.getAngleX_1();
  Serial.print("AngleX_VCC_0x69 : ");
  Serial.println(AngleX_VCC);
}
