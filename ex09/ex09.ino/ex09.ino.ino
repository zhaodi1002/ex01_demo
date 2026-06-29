#include <WiFi.h>
#include <WebServer.h>

// 填写你的2.4G WiFi信息，区分大小写，不要5G
const char* ssid     = "美女的热点你别碰";
const char* password = "zd20061002";

#define TOUCH_PIN 4
#define LED_PIN   2
#define THRESHOLD 35

WebServer server(80);
bool isArm = false;
bool alarmFlag = false;
unsigned long lastBlink = 0;
const unsigned long blinkInterval = 80;
unsigned long wifiStart = 0;
const unsigned long wifiTimeout = 15000;

// 网页拼接，兼容所有IDE，不会空白
String getHtml() {
  String state = isArm ? "已布防" : "未布防";
  String html = "";
  html += "<!DOCTYPE html>";
  html += "<html style='text-align:center;margin-top:60px;font-size:24px'>";
  html += "<body>";
  html += "<h2>ESP32安防报警系统</h2>";
  html += "<p>当前状态：" + state + "</p>";
  html += "<a href='/arm'><button style='font-size:24px;padding:10px 30px;margin:10px'>布防</button></a>";
  html += "<a href='/disarm'><button style='font-size:24px;padding:10px 30px;margin:10px'>撤防</button></a>";
  html += "</body></html>";
  return html;
}

void handleRoot() {
  server.send(200, "text/html;charset=utf-8", getHtml());
}

void armPage() {
  isArm = true;
  server.sendHeader("Location", "/");
  server.send(303);
}

void disarmPage() {
  isArm = false;
  alarmFlag = false;
  digitalWrite(LED_PIN, HIGH);
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  // 【关键修复1：上电第一行初始化串口，立刻有打印，不会空白】
  Serial.begin(115200);
  delay(200);
  Serial.println("=====================");
  Serial.println("ex09 安防系统启动");
  Serial.println("=====================");

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  // WiFi连接
  Serial.print("开始连接WiFi：");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  wifiStart = millis();

  // 带超时，不会无限卡死程序导致串口无输出
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (millis() - wifiStart > wifiTimeout) {
      Serial.println("\nWiFi连接超时！检查WiFi名称/密码/2.4G网络");
      break;
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi连接成功！");
    Serial.print("设备IP地址：");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("WiFi连接失败，网页功能不可用");
  }

  // 绑定网页接口
  server.on("/", handleRoot);
  server.on("/arm", armPage);
  server.on("/disarm", disarmPage);
  server.begin();
  Serial.println("网页服务启动完成");
}

void loop() {
  server.handleClient();
  int touchVal = touchRead(TOUCH_PIN);
  unsigned long now = millis();
  Serial.print("触摸检测数值：");
  Serial.println(touchVal);

  // 布防后触摸触发报警
  if (isArm && touchVal < THRESHOLD && !alarmFlag) {
    alarmFlag = true;
    Serial.println("=====触发报警=====");
  }

  // 非阻塞LED闪烁，不卡网页、不卡串口打印
  if (alarmFlag) {
    if (now - lastBlink >= blinkInterval) {
      static bool ledState = false;
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState ? LOW : HIGH);
      lastBlink = now;
    }
  } else {
    digitalWrite(LED_PIN, HIGH);
  }
}