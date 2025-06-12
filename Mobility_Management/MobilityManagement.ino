#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <Servo.h>

// Motor
const int motorPWM = 8;
const int motorDir1 = 26;
const int motorDir2 = 28;

// Servo
Servo steeringServo;

// Senzor de culoare (I2C pe pin 20 și 21)
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

// Variabile de stare
int laps = 0;
bool turning = false;
unsigned long lastTurnTime = 0;
const unsigned long turnCooldown = 2000; // 2 secunde ca să nu detecteze de 2 ori aceași culoare

void setup() {
  Serial.begin(9600);

  // Inițializare senzori
  if (tcs.begin()) {
    Serial.println("Color sensor ready");
  } else {
    Serial.println("Color sensor not detected");
    while (1);
  }

  // Motoare
  pinMode(motorPWM, OUTPUT);
  pinMode(motorDir1, OUTPUT);
  pinMode(motorDir2, OUTPUT);

  // Servo
  steeringServo.attach(9);
  steeringServo.write(90);

  // Pornește înainte
  moveForward(200);
}

void loop() {
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);

  Serial.print("R: "); Serial.print(r);
  Serial.print(" G: "); Serial.print(g);
  Serial.print(" B: "); Serial.print(b);
  Serial.print(" C: "); Serial.println(c);

  unsigned long currentTime = millis();

  // Cooldown pentru a preveni detecții duble
  if (currentTime - lastTurnTime > turnCooldown) {
    if (isBlue(r, g, b)) {
      Serial.println("Blue detected → turning");
      makeLeftTurn();
      lastTurnTime = currentTime;
    } else if (isYellow(r, g, b)) {
      laps++;
      Serial.print("Yellow detected → Lap: ");
      Serial.println(laps);
      lastTurnTime = currentTime;

      if (laps >= 4) {
        Serial.println("4 laps completed.");
        stopMotor();
        while (1);
      }
    }
  }
}

// Funcții de culoare
bool isBlue(uint16_t r, uint16_t g, uint16_t b) {
  return (b > r + 60 && b > g + 60 && b > 150);
}

bool isYellow(uint16_t r, uint16_t g, uint16_t b) {
  return (r > 150 && g > 150 && b < 100);
}

// Funcții motor
void moveForward(int speed) {
  analogWrite(motorPWM, speed);
  digitalWrite(motorDir1, HIGH);
  digitalWrite(motorDir2, LOW);
}

void stopMotor() {
  analogWrite(motorPWM, 0);
  digitalWrite(motorDir1, LOW);
  digitalWrite(motorDir2, LOW);
}

// Funcție turn stânga
void makeLeftTurn() {
  steeringServo.write(140);
  delay(1300);  // ajustează în funcție de robot
  steeringServo.write(90);
}
