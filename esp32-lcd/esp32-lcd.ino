#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const char* ssid = "Phong 501";
const char* password = "0984378397";
String serverIP = "192.168.51.68"; 

#define LCD_COLS 20
#define LCD_ROWS 4
#define I2C_ADDR 0x27
LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLS, LCD_ROWS);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  lcd.begin(20,4);
  lcd.init();
  lcd.backlight();                 
  lcd.clear();                     
  lcd.setCursor(0, 0);            
  lcd.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  
}

void loop() {
  HTTPClient http;
  http.begin("http://" + serverIP + "/name");
  int httpCode = http.GET();

  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
      lcd.clear();                          
      lcd.setCursor(0, 0);
      if (payload!=""){
        lcd.print("Someone appeared:"); 
      }
      else lcd.print("No one appeared:");        
      lcd.setCursor(0, 1);        
      lcd.print(payload);  
    }
  } else {
    Serial.println("Error on HTTP request");
  }
  http.end();
  delay(1000);
}
