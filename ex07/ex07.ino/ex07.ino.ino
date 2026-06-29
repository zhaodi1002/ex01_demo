#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "美女的热点你别碰";
const char* pwd = "zd20061002";
const int LED_PIN = 2;
const int freq = 5000;
const int res = 8;
WebServer server(80);
int brightness = 0;

void handleRoot() {
  String html = R"raw(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width">
<title>ESP32无极调光</title>
<style>body{text-align:center;margin-top:50px;font-size:22px}</style>
</head>
<body>
<h2>LED亮度滑动调节</h2>
<input type="range" min="0" max="255" value=")" + String(brightness) + R"raw(" id="slider" style="width:80%;height:30px;">
<p>当前亮度值：<span id="val">)" + String(brightness) + R"raw(</span></p>
<script>
const slider = document.getElementById("slider");
const val = document.getElementById("val");
slider.oninput = function(){
  let num = this.value;
  val.innerText = num;
  fetch("/set?b="+num);
}
</script>
</body></html>
)raw";
  server.send(200, "text/html;charset=utf-8", html);
}

void handleSetBright(){
  if(server.hasArg("b")){
    brightness = server.arg("b").toInt();
    ledcWrite(LED_PIN, brightness);
  }
  server.sendHeader("Location","/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  ledcAttach(LED_PIN, freq, res);
  WiFi.begin(ssid, pwd);
  while(WiFi.status() != WL_CONNECTED){delay(500);Serial.print(".");}
  Serial.println("\nWiFi连接成功，IP："+WiFi.localIP().toString());
  server.on("/", handleRoot);
  server.on("/set", handleSetBright);
  server.begin();
}

void loop() {
  server.handleClient();
}