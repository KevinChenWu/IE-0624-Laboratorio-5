#include <Arduino_LSM9DS1.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial);
  IMU.begin();
  Serial.println("aX,aY,aZ,gX,gY,gZ");
}

void loop() {
  // put your main code here, to run repeatedly:
  float aX, aY, aZ, gX, gY, gZ;
  if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
    IMU.readAcceleration(aX, aY, aZ);
    IMU.readGyroscope(gX, gY, gZ);
    Serial.print(aX, 4);
    Serial.print(",");
    Serial.print(aY, 4);
    Serial.print(",");
    Serial.print(aZ, 4);
    Serial.print(",");
    Serial.print(gX, 4);
    Serial.print(",");
    Serial.print(gY, 4);
    Serial.print(",");
    Serial.print(gZ, 4);
    Serial.println();
  }
}
