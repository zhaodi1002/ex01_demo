#include <WiFi.h>
#include <WebServer.h>

// ========== 修改这里的WiFi信息 ==========
const char* ssid     = "美女的热点你别碰";
const char* password = "zd20061002";
// ========================================

#define TOUCH_PIN 4
#define LED_PIN   2
#define THRESHOLD 35

WebServer server(80);
bool isArm = false;
bool alarmFlag = false;
unsigned long lastBlink = 0;
const unsigned long blinkInterval = 80;
// WiFi连接超时计时
unsigned long wifiStart = 0;
const unsigned long wifiTimeout = 15000; // 15秒连不上WiFi自动放弃

// 网页页面（替换旧raw语法，全Arduino版本兼容，不会空白）
String getHtml() {
  String stateText = isArm ? "已布防" : "未布防";
  String html = "<!DOCTYPE html>";
  html += "<html style='text-align:center;margin-top:60px;font-size:24px'>";
  html += "<body>";
  html += "<h2>ESP32安防报警主机</h2>";
  html += "<p>当前状态：" + stateText + "</p>";
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
  server.send(303, "text/plain", "");
}

void disarmPage() {
  isArm = false;
  alarmFlag = false;
  digitalWrite(LED_PIN, HIGH); // D2反向：高电平熄灭LED
  server.sendHeader("Location", "/");
  server.send(303, "text/plain", "");
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // 上电默认熄灭

  // WiFi连接初始化
  Serial.print("正在连接WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  wifiStart = millis();

  // 带超时的WiFi等待，不会永久卡死
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    // 15秒超时跳出循环
    if (millis() - wifiStart > wifiTimeout) {
      Serial.println("\nWiFi连接超时，请检查WiFi名称/密码！");
      break;
    }
  }

  // 连接成功打印IP
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi连接成功！");
    Serial.print("设备访问IP：");
    Serial.println(WiFi.localIP().toString());
  } else {
    Serial.println("WiFi连接失败，无网络功能");
  }

  // 绑定网页路由
  server.on("/", handleRoot);
  server.on("/arm", armPage);
  server.on("/disarm", disarmPage);
  server.begin();
  Serial.println("Web服务已启动");
}

void loop() {
  server.handleClient(); // 持续处理网页，不阻塞

  int touchVal = touchRead(TOUCH_PIN);
  unsigned long now = millis();
  Serial.print("触摸数值：");
  Serial.println(touchVal); // 调试：看触摸引脚是否正常

  // 仅布防状态下触摸触发报警
  if (isArm && touchVal < THRESHOLD && !alarmFlag) {
    alarmFlag = true;
    Serial.println("触发报警！");
  }

  // 非阻塞LED闪烁，不会卡住网页
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