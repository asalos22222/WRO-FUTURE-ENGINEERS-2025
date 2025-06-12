#include <Servo.h>
#include <Wire.h>
#include "HUSKYLENS.h"

Servo steeringServo;
HUSKYLENS huskyLens;

int currentSteeringAngle = 90;
bool isTurning = false;
unsigned long turnStartTime = 0;
const unsigned long minTurnDuration = 500;  // ms - ținem virajul minim

void setup() {
  Serial.begin(9600);

  steeringServo.attach(10);  // pinul unde ai servo-ul

  Wire.begin();
  if (!huskyLens.begin(Wire)) {
    Serial.println("HuskyLens initialization failed!");
    while (1);
  }

  huskyLens.writeAlgorithm(ALGORITHM_COLOR_RECOGNITION);
  Serial.println("HuskyLens ready. Starting...");
}

void loop() {
  bool obstacleDetected = false;

  if (huskyLens.request()) {
    if (huskyLens.available()) {
      HUSKYLENSResult result = huskyLens.read();
      if (result.command == COMMAND_RETURN_BLOCK) {
        obstacleDetected = true;

        if (result.ID == 1) {
          Serial.println("Red obstacle detected. Turning right.");
          currentSteeringAngle = 50;  // dreapta
          isTurning = true;
          turnStartTime = millis();
        } 
        else if (result.ID == 2) {
          Serial.println("Green obstacle detected. Turning left.");
          currentSteeringAngle = 140;  // stânga
          isTurning = true;
          turnStartTime = millis();
        }
      }
    }
  }

  // Dacă nu s-a detectat obstacol
  if (!obstacleDetected) {
    if (isTurning) {
      // Suntem în viraj → verificăm dacă a trecut timpul minim
      if (millis() - turnStartTime >= minTurnDuration) {
        isTurning = false;
        currentSteeringAngle = 90;
      }
    } else {
      // Fără obstacol și nu virăm → servo la drept
      currentSteeringAngle = 90;
    }
  }

  // Aplicăm unghiul la servo
  steeringServo.write(currentSteeringAngle);

  delay(50);  // Delay mic pt stabilitate
}
