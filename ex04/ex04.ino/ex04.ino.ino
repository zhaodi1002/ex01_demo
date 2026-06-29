#define TOUCH_PIN 4
#define LED_PIN 2
#define THRESHOLD 20
unsigned long debounceTime = 120;
unsigned long lastTouch = 0;
int speedLevel = 1; // 1慢 2中 3快
int freq = 5000;
int res = 8;

void setup() {
  Serial.begin(115200);
  ledcAttach(LED_PIN, freq, res);
}

void loop() {
  // 触摸档位切换逻辑
  int touchVal = touchRead(TOUCH_PIN);
  unsigned long now = millis();
  static int lastTouchSt = 100;
  if(touchVal < THRESHOLD && lastTouchSt >= THRESHOLD && now - lastTouch > debounceTime){
    speedLevel++;
    if(speedLevel > 3) speedLevel = 1;
    lastTouch = now;
    Serial.print("当前档位：");
    Serial.println(speedLevel);
  }
  lastTouchSt = touchVal;

  // 根据档位设置呼吸步长延时
  int stepDelay;
  switch(speedLevel){
    case 1: stepDelay = 30; break;
    case 2: stepDelay = 12; break;
    case 3: stepDelay = 4; break;
  }

  // 渐亮
  for(int i=0; i<=255; i++){
    ledcWrite(LED_PIN, i);
    delay(stepDelay);
  }
  // 渐暗
  for(int i=255; i>=0; i--){
    ledcWrite(LED_PIN, i);
    delay(stepDelay);
  }
}