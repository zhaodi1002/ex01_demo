#define TOUCH_PIN 4
void setup() {
  Serial.begin(115200);
}
void loop() {
  Serial.print("Touch数值：");
  Serial.println(touchRead(TOUCH_PIN));
  delay(150);
}