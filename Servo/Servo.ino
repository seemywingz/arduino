#include <Button1.h>
#include <IOPin.h>
#include <Servo.h>

IOPin btn1(3, INPUT_PULLUP);
IOPin btn2(9, INPUT_PULLUP);
IOPin servoPin(6);

Button1 b1(btn1.pin());

Servo servo;
const float servoSpeed = 0.30;
const int servoMax = 180;
const int servoMin = 0;
int startPos = 0;
int servoPos = startPos;

void setup() {
  Serial.begin(115200);
  initButtonHandlers();
  servo.attach(servoPin.pin());
  servo.write(startPos);
  setServoAngle(startPos);
};

void loop() { delay(1000); }

void setServoAngle(int targetAngle) {
  int currentPosition = servo.read();
  Serial.println("Setting Servo Angle");
  Serial.print("Current Position: ");
  Serial.println(currentPosition);
  Serial.print("Target Position: ");
  Serial.println(targetAngle);
  targetAngle = constrain(targetAngle, servoMin, servoMax);
  servo.write(targetAngle);
  float millisToWait =
      (abs(targetAngle - currentPosition) / 60.0) * servoSpeed * 1000;
  delay(millisToWait);
}

void initButtonHandlers() {
  attachInterrupt(
      digitalPinToInterrupt(b1.pin()), []() { b1.isr(); }, CHANGE);
  b1.setVerbose(true);
  b1.setSingleClickCallback([]() {
    servoPos = (servoPos >= servoMax) ? servoMin : servoPos + servoMax / 5;
    setServoAngle(servoPos);
    Serial.print("Servo Position: ");
    Serial.println(servoPos);
  });
  b1.setDoubleClickCallback([]() { Serial.println("Double Click"); });
  b1.setLongClickCallback([]() {

  });
}