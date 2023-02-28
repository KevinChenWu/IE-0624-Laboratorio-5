#include <Arduino_LSM9DS1.h>
#include <TensorFlowLite.h>
#include <tensorflow/lite/micro/all_ops_resolver.h>
#include <tensorflow/lite/micro/micro_interpreter.h>
#include <tensorflow/lite/schema/schema_generated.h>

#include "model.h"

const float noise_threshold = 2.2;
const int max_samples = 32;
volatile int num_sample = max_samples;

tflite::AllOpsResolver tflOpsResolver;
const tflite::Model* tflModel = nullptr;
tflite::MicroInterpreter* tflInterpreter = nullptr;
TfLiteTensor* tflInputTensor = nullptr;
TfLiteTensor* tflOutputTensor = nullptr;
constexpr int tensorArenaSize = 8 * 1024;
byte tensorArena[tensorArenaSize] __attribute__((aligned(16)));

const char* GESTURES[] = {
  "punch",
  "flex",
  "circle"
};

#define NUM_GESTURES (sizeof(GESTURES) / sizeof(GESTURES[0]))

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  IMU.begin();
  while(!Serial);
  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");
  Serial.println();

  tflModel = tflite::GetModel(model);
  if (tflModel->version() != TFLITE_SCHEMA_VERSION) {
    Serial.println("Model schema mismatch!");
    while (1);
  }
  tflInterpreter = new tflite::MicroInterpreter(tflModel, tflOpsResolver, tensorArena, tensorArenaSize);
  tflInterpreter->AllocateTensors();
  tflInputTensor = tflInterpreter->input(0);
  tflOutputTensor = tflInterpreter->output(0);  
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
      tflInputTensor->data.f[num_sample * 3 + 0] = (gX + 2000.0) / 4000.0;
      tflInputTensor->data.f[num_sample * 3 + 1] = (gY + 2000.0) / 4000.0;
      tflInputTensor->data.f[num_sample * 3 + 2] = (gZ + 2000.0) / 4000.0;
      num_sample++;

      if (num_sample == max_samples) {
        TfLiteStatus invokeStatus = tflInterpreter->Invoke();
        if (invokeStatus != kTfLiteOk) {
          Serial.println("Invoke failed!");
          while (1);
          return;
        }

        for (int i = 0; i < NUM_GESTURES; i++) {
          Serial.print(GESTURES[i]);
          Serial.print(": ");
          Serial.println(tflOutputTensor->data.f[i], 4);
        }
        Serial.println();
      }
    }
  }
}
