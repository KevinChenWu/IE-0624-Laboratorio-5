#include <Arduino_LSM9DS1.h>

const float noise_threshold = 2.2;
const int max_samples = 32;
volatile int num_sample = max_samples;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  IMU.begin();
  while(!Serial);
  Serial.println("gX,gY,gZ");
}

void loop() {
  // put your main code here, to run repeatedly:
  float aX, aY, aZ, gX, gY, gZ;
  while (num_sample == max_samples) {
    if (IMU.accelerationAvailable()) {
      IMU.readAcceleration(aX, aY, aZ);
      float aceleration = fabs(aX) + fabs(aY) + fabs(aZ);
      if (aceleration >= noise_threshold) {
        num_sample = 0;
      }
    }
  }

  while (num_sample < max_samples) {
    if (IMU.gyroscopeAvailable()) {
      IMU.readGyroscope(gX, gY, gZ);
      Serial.print(gX, 4);
      Serial.print(",");
      Serial.print(gY, 4);
      Serial.print(",");
      Serial.print(gZ, 4);
      Serial.println();
      num_sample++;
    }
  }
}