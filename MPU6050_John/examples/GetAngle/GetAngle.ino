
#include <MPU6050_tockn.h>
#include <Wire.h>

MPU6050 mpu6050(Wire);
double sLastime = 0;
double sCurtime = 0;
double sDiffTim = 0;
double LstDifAngle = 0;
double CurDifAngle = 0;

void setup() {
  Serial.begin(57600);
  Wire.begin();
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  mpu6050.begin(MPU6050_ADDR_GND);
  mpu6050.calcGyroOffsets(true,MPU6050_ADDR_GND);  
  mpu6050.begin(MPU6050_ADDR_VCC);
  mpu6050.calcGyroOffsets(true,MPU6050_ADDR_VCC);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
}

void loop() {
	mpu6050.update(MPU6050_ADDR_GND);
	float AngleX_GND = mpu6050.getAngleX();
	mpu6050.update(MPU6050_ADDR_VCC);
	float AngleX_VCC = mpu6050.getAngleX_1();
	CurDifAngle = AngleX_GND - AngleX_VCC;
	
	sCurtime = millis();
	sDiffTim = sCurtime - sLastime;	
	if(sDiffTim >= 20)  //Data Rate is 50Hz
	{
		Serial.print("Diff time is :");
		Serial.print(sDiffTim);
		Serial.print("ms\t");
		
		//Print 2IMUs' X Angle

		Serial.print("AngleX_GND_0x68 : ");
		Serial.print(AngleX_GND);
		Serial.print("\t");
		
		Serial.print("AngleX_VCC_0x69 : ");
		Serial.print(AngleX_VCC);
		Serial.print("\t");

		Serial.print("AngleX_Diff : ");
		Serial.println(CurDifAngle);
		
		LstDifAngle = CurDifAngle;
		sLastime = sCurtime;
	}
	if(LstDifAngle != 0)
		if(CurDifAngle < LstDifAngle)
		{
			digitalWrite(4, HIGH);
			digitalWrite(5, LOW);
		}
		else if(CurDifAngle > LstDifAngle)
		{
			digitalWrite(4, LOW);
			digitalWrite(5, HIGH);
		}
}