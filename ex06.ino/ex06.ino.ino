#define LED_A 2
#define LED_B 5
const int freq = 5000;
const int res = 8;

void setup() {
  Serial.begin(115200);
  ledcAttach(LED_A, freq, res);
  ledcAttach(LED_B, freq, res);
}

void loop() {
  // A渐亮 B渐暗
  for(int i=0; i<=255; i++){
    ledcWrite(LED_A, i);
    ledcWrite(LED_B, 255 - i);
    delay(8);
  }
  // A渐暗 B渐亮
  for(int i=255; i>=0; i--){
    ledcWrite(LED_A, i);
    ledcWrite(LED_B, 255 - i);
    delay(8);
  }
}