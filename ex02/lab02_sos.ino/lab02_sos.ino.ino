const int ledPin = 2;
unsigned long lastTick = 0;
const unsigned long interval = 1000;
bool ledState = false;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  unsigned long current = millis();
  if(current - lastTick >= interval){
    lastTick = current;
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
  }
}