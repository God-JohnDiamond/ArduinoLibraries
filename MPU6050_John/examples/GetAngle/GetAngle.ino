
#include <MPU6050_tockn.h>
#include <Wire.h>

MPU6050 mpu6050(Wire);
double sLastime = 0;
double sCurtime = 0;
double sDiffTim = 0;

void setup() {
  Serial.begin(57600);
  Wire.begin();
  mpu6050.begin(MPU6050_ADDR_GND);
  mpu6050.calcGyroOffsets(true,MPU6050_ADDR_GND);  
  mpu6050.begin(MPU6050_ADDR_VCC);
  mpu6050.calcGyroOffsets(true,MPU6050_ADDR_VCC);
}

void loop() {


	sCurtime = millis();
	sDiffTim = sCurtime - sLastime;	
	if(sDiffTim >= 20)  //Data Rate is 50Hz
	{
		Serial.print("Now time is :");
		Serial.print(sCurtime);
		Serial.print("ms\t");
		Serial.print("Diff time is :");
		Serial.print(sDiffTim);
		Serial.print("ms\t");
		
		//Print 2IMUs' X Angle
		mpu6050.update(MPU6050_ADDR_GND);
		float AngleX_GND = mpu6050.getAngleX();
		Serial.print("AngleX_GND_0x68 : ");
		Serial.print(AngleX_GND);
		Serial.print("\t");
		
		mpu6050.update(MPU6050_ADDR_VCC);
		float AngleX_VCC = mpu6050.getAngleX_1();
		Serial.print("AngleX_VCC_0x69 : ");
		Serial.print(AngleX_VCC);
		Serial.print("\t");
		Serial.print("DiffAngel : ");
		Serial.println(AngleX_VCC - AngleX_GND);

		sLastime = sCurtime;
	}
}
