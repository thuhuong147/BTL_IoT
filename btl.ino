#include <WebServer.h>
#include <WiFi.h>
#include <esp32cam.h>
 
const char* WIFI_SSID = "Phong 501";
const char* WIFI_PASS = "0984378397";
WebServer server(80);
String name1=""; 
static auto hiRes = esp32cam::Resolution::find(800, 600);

void serveJpg(){
  auto frame = esp32cam::capture();
  if (frame == nullptr) {
    // Serial.println("CAPTURE FAIL");
    server.send(503, "", "");
    return;
  }
  // Serial.printf("CAPTURE OK %dx%d %db\n", frame->getWidth(), frame->getHeight(),
  //               static_cast<int>(frame->size()));
 
  server.setContentLength(frame->size());
  server.send(200, "image/jpeg");
  WiFiClient client = server.client();
  frame->writeTo(client);
}
 
void handleJpgHi(){
  if (!esp32cam::Camera.changeResolution(hiRes)) {
    Serial.println("SET-HI-RES FAIL");
  }
  serveJpg();
  String name = server.arg("name");
  name1=name;
  // Serial.println("Detected Name: " + name);
  server.send(200, "text/plain", "Received name: " + name);
}
void handleName(){
  server.send(200, "text/plain",name1);
}
void  setup(){
  Serial.begin(115200);
  // Serial.println();
  {
    using namespace esp32cam;
    Config cfg;
    cfg.setPins(pins::AiThinker);
    cfg.setResolution(hiRes);
    cfg.setBufferCount(2);
    cfg.setJpeg(80);
 
    bool ok = Camera.begin(cfg);
    // Serial.println(ok ? "CAMERA OK" : "CAMERA FAIL");
  }
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  // Serial.print("http://");
  // Serial.println(WiFi.localIP());
  // Serial.println("  /cam-hi.jpg");
  server.on("/cam-hi.jpg", handleJpgHi);
  server.on("/name", HTTP_GET,handleName);
  server.begin();
}
 
void loop(){
  server.handleClient();
}