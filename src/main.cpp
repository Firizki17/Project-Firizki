#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// WiFi credentials
const char* ssid = "fu.kyy";
const char* password = "aaaaaaa1";

// Pins
const int trigPin = 27;
const int echoPin = 26;
const int buzzerPin = 25;

// OLED settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Web server on port 80
WebServer server(80);

// Buzzer timing
unsigned long lastBeepTime = 0;
const unsigned long beepInterval = 1500;

// Function to read distance from ultrasonic sensor
long readDistanceCM() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;
}

void setup() {
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  noTone(buzzerPin);

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED gagal");
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println("Tandon Air");
  display.setTextSize(1);
  display.setCursor(0, 35);
  display.println("Monitoring...");
  display.display();
  delay(2000);
  display.clearDisplay();

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan ke WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nTerhubung!");
  Serial.println(WiFi.localIP());

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("WiFi Terhubung!");
  display.setTextSize(1);
  display.setCursor(0, 15);
  display.println(WiFi.localIP());
  display.display();
  delay(2000);
  display.clearDisplay();

  // Mount SPIFFS filesystem
  if (!SPIFFS.begin(true)) {
    Serial.println("Gagal mount SPIFFS");
    return;
  }

  // Handle root path - serve index.html from SPIFFS
  server.on("/", HTTP_GET, []() {
    File file = SPIFFS.open("/dashboard.html", "r");
    if (!file) {
      server.send(500, "text/plain", "index.html tidak ditemukan");
      return;
    }
    server.streamFile(file, "text/html");
    file.close();
  });

  // Endpoint to return distance as JSON
  server.on("/jarak", HTTP_GET, []() {
    long jarak = readDistanceCM();
    String json = "{\"jarak\":" + String(jarak) + "}";
    server.send(200, "application/json", json);
  });

  // Start server
  server.begin();
  Serial.println("Web server aktif");
}

void loop() {
  server.handleClient();

  long jarak = readDistanceCM();
  Serial.println("Jarak: " + String(jarak) + " cm");

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);

  if (jarak < 10) {
    display.print("Air Penuh:");
  } else if (jarak > 60) {
    display.print("Air Kosong:");
  } else {
    display.print("Air Terisi:");
  }

  display.setTextSize(3);
  display.setCursor(0, 25);
  display.print(jarak);
  display.println(" cm");
  display.display();

// Logika buzzer
if (jarak < 10 || jarak > 60) {
  unsigned long now = millis();
  if (now - lastBeepTime >= beepInterval) {
    tone(buzzerPin, 1500, 300);
    lastBeepTime = now;
  }
} else {
  noTone(buzzerPin);
}

delay(300);
}