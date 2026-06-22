const int ledPin = 2;
const int freq = 5000;
const int resolution = 8;

void setup() {
  Serial.begin(115200);
  ledcAttach(ledPin, freq, resolution);
}

void loop() {
  for(int duty=0; duty<=255; duty++){
    ledcWrite(ledPin, duty);
    delay(10);
  }
  for(int duty=255; duty>=0; duty--){
    ledcWrite(ledPin, duty);
    delay(10);
  }
  Serial.println("单次呼吸周期完成");
}