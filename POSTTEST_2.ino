#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define LED1 D4  
#define BUZZER D8
#define BUTTON_PIN1 D2 // buzzer volume rendah
#define BUTTON_PIN2 D3 // Buzzer volume tinggi
#define BUTTON_PIN3 D1 // on off komponen button

const char* ssid = "Lyubimaya";       // Nama SSID AP/Hotspot
const char* password = "terserahmu";    // Password Wifi 

ESP8266WebServer server(80);      //Menyatakan Webserver pada port 80
String webpage;

int brightness = 0; // Variabel untuk menyimpan kecerahan lampu
int toneValue = 0; // Variabel untuk menyimpan nilai tone/nada buzzer
bool isEnabled = false; // Variabel untuk menyimpan status enable/disable komponen

unsigned long lastDebounceTime1 = 0; // Waktu terakhir untuk debounce button 1
unsigned long debounceDelay = 50;    // Delay debounce

void setup() {
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON_PIN1, INPUT_PULLUP);
  pinMode(BUTTON_PIN2, INPUT_PULLUP);
  pinMode(BUTTON_PIN3, INPUT_PULLUP);

  // Connect ke WiFi
  Serial.println();
  Serial.print("Configuring access point...");
  WiFi.mode(WIFI_STA); // Mode Station
  WiFi.begin(ssid, password); // Mencocokan SSID dan Password
   
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  digitalWrite(LED1, LOW);
  analogWrite(LED1, brightness); // Mengatur intensitas cahaya awal

  digitalWrite(BUZZER, LOW);

  // Membuat file webpage 
  server.on("/", []() {
    String content = "<html><head><title>ESP8266 Control</title></head><body>";
    content += "<h1>ESP8266 Control</h1>";
    content += "<p>Tone Value: " + String(toneValue) + "</p>";
    content += "<p>Brightness: " + String(brightness) + "</p>";
    content += "</body></html>";
    server.send(200, "text/html", content);
  });

  server.begin(); 
}

void loop() {
  server.handleClient();

  // Membaca tombol dan memprosesnya
  int buttonState1 = digitalRead(BUTTON_PIN1);
  int buttonState2 = digitalRead(BUTTON_PIN2);
  int buttonState3 = digitalRead(BUTTON_PIN3);

  // Button 1 (naikkan nada buzzer dan intensitas cahaya LED)
  if (buttonState1 == LOW && millis() - lastDebounceTime1 > debounceDelay) {
    lastDebounceTime1 = millis();
    toneValue += 10;
    if (toneValue > 2500) toneValue = 2500;
    tone(BUZZER, toneValue);

    brightness += 3; // Menambah intensitas cahaya LED
    if (brightness > 255) brightness = 255;
    analogWrite(LED1, brightness); // Mengatur intensitas cahaya LED
  }

  // Button 2 (turunkan nada buzzer dan intensitas cahaya LED)
  if (buttonState2 == LOW) {
    toneValue -= 10;
    if (toneValue < 0) toneValue = 0  ;
    tone(BUZZER, toneValue);

    brightness -= 3; // Mengurangi intensitas cahaya LED
    if (brightness < 0) brightness = 0;
    analogWrite(LED1, brightness); // Mengatur intensitas cahaya LED
  }

  // Button 3 (mematikan semua komponen)
  if (buttonState3 == LOW) {
    digitalWrite(LED1, LOW);
    noTone(BUZZER);}
    // else {
    //   digitalWrite(LED1, HIGH);
    //   toneValue = 2500 ;
    //   brightness = 255;
    // };
}
