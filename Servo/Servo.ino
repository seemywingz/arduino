#include <Button1.h>
#include <IOPin.h>
#include <Servo.h>

IOPin btn1(3, INPUT_PULLUP);
IOPin btn2(9, INPUT_PULLUP);
IOPin servoPin(6);

Button1 b1(btn1.pin());

Servo servo;
const float servoSpeed = 0.30;
const int servoMax = 70;
const int servoMin = 0;
int servoPos = servoMin;

void setup() {
  Serial.begin(115200);
  initButtonHandlers();
  servo.attach(servoPin.pin());
  setServoAngle(servoPos);
};

void loop() {
  // Serial.print("Servo Position: ");
  // Serial.println(servo.read());
  delay(1000);
}

void setServoAngle(int targetAngle) {
  int currentPosition = servo.read();
  Serial.println("\nSetting Servo Angle");
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

void toggleOpenClose() {
  servoPos = (servoPos == servoMin) ? servoMax : servoMin;
  setServoAngle(servoPos);
}

void initButtonHandlers() {
  b1.setVerbose(true);
  attachInterrupt(
      digitalPinToInterrupt(b1.pin()), []() { b1.isr(); }, CHANGE);
  b1.setSinglePressCallback([]() {
    // toggleOpenClose();
  });
  b1.setDoublePressCallback([]() {});
  b1.setLongPressCallback([]() {

  });
}