const int ledPin = 2;
const unsigned long shortOn = 200;
const unsigned long shortGap = 200;
const unsigned long longOn = 600;
const unsigned long charGap = 400;
const unsigned long sosEndPause = 2000;

enum State{IDLE,S1,S2,S3,O1,O2,O3,SS1,SS2,SS3,PAUSE} state=IDLE;
unsigned long lastTime = 0;
bool ledOn = false;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  unsigned long now = millis();
  unsigned long delta = now - lastTime;
  switch(state){
    case IDLE: state=S1; lastTime=now; break;
    case S1: ledOn=1;digitalWrite(ledPin,1);if(delta>=shortOn){ledOn=0;digitalWrite(ledPin,0);lastTime=now;state=S2;}break;
    case S2: if(delta>=shortGap){lastTime=now;state=S3;}break;
    case S3: ledOn=1;digitalWrite(ledPin,1);if(delta>=shortOn){ledOn=0;digitalWrite(ledPin,0);lastTime=now;state=O1;}break;
    case O1: if(delta>=charGap){lastTime=now;state=O2;}break;
    case O2: ledOn=1;digitalWrite(ledPin,1);if(delta>=longOn){ledOn=0;digitalWrite(ledPin,0);lastTime=now;state=O3;}break;
    case O3: if(delta>=charGap){lastTime=now;state=SS1;}break;
    case SS1: ledOn=1;digitalWrite(ledPin,1);if(delta>=shortOn){ledOn=0;digitalWrite(ledPin,0);lastTime=now;state=SS2;}break;
    case SS2: if(delta>=shortGap){lastTime=now;state=SS3;}break;
    case SS3: ledOn=1;digitalWrite(ledPin,1);if(delta>=shortOn){ledOn=0;digitalWrite(ledPin,0);lastTime=now;state=PAUSE;}break;
    case PAUSE: if(delta>=sosEndPause){lastTime=now;state=IDLE;}break;
  }
}