#define TOUCH_PIN 4
#define LED_PIN 2
#define THRESHOLD 20
unsigned long debounceTime = 100;
unsigned long lastTouchTime = 0;
bool ledState = false;
int lastTouchVal = 100;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  int touchVal = touchRead(TOUCH_PIN);
  unsigned long now = millis();

  // 边缘检测：上一次未触摸，当前触摸 + 防抖延时
  if(touchVal < THRESHOLD && lastTouchVal >= THRESHOLD && now - lastTouchTime > debounceTime){
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
    lastTouchTime = now;
  }
  lastTouchVal = touchVal;
  delay(20);
}