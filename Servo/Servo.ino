#include <IOPin.h>
#include <OmniButton.h>
#include <Servo.h>

IOPin btn1(3, INPUT_PULLUP);
IOPin btn2(9, INPUT_PULLUP);
IOPin servoPin(6);

OmniButton b1(btn1.pin());

Servo servo;
const float servoSpeed = 0.30;
const int servoMax = 45;
const int servoMin = 0;
int servoPos = servoMin;

void setup() {
  Serial.begin(115200);
  buttonConfig();
  servo.attach(servoPin.pin());
  setServoAngle(servoPos);
};

void loop() {
  // Serial.print("Servo Position: ");
  // Serial.println(servo.read());
  delay(100);
}

void setServoAngle(int targetAngle) {
  int currentPosition = servo.read();
  Serial.println("\nSetting Servo Angle");
  Serial.print("Current Position: ");
  Serial.println(currentPosition);
  Serial.print("Target Position: ");
  Serial.println(targetAngle);
  targetAngle = constrain(targetAngle, servoMin, servoMax);
  float millisToWait =
      (abs(targetAngle - currentPosition) / 60.0) * servoSpeed * 1000;
  servo.write(targetAngle);
  delay(millisToWait);
}

void toggleOpenClose() {
  servoPos = (servoPos == servoMin) ? servoMax : servoMin;
  setServoAngle(servoPos);
}

void buttonConfig() {
  attachInterrupt(
      digitalPinToInterrupt(b1.pin()), []() { b1.listen(); }, CHANGE);
  // b1.setVerbose(true);
  b1.setSinglePressCallback([]() { toggleOpenClose(); });
}