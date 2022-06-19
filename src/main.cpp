#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <SSD1306Wire.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <Wire.h>

// Wifi credentials
const char* ssid = "__SSID__";
const char* password = "__PASSWORD__";

// Spotify API credentials
String client_id = "__CLIENT-ID__";
String redirect = "https%3A%2F%2Fhttpbin.org%2Fanything";
String cookie = "__COOKIE__";

// Song Info
long current = 0, duration = 0;
int volume = 0;
String artists = "", title, album, device, type;
bool isActive, playing;

// Spotify Auth token Info
bool isExpired = false;
String token = "";

// Wifi
WiFiMulti wifimulti;

// Display
// SDA 21
// SCL 22
SSD1306Wire display(0x3c, 21, 22);

void getToken() {
  HTTPClient http;
  String url =
      "https://accounts.spotify.com/"
      "authorize?response_type=token&redirect_uri=" +
      redirect + "&client_id=" + client_id +
      "&scope=user-read-playback-state+user-read-playback-position+user-modify-"
      "playback-state&state=cryq3";
  Serial.println(url);
  http.begin(url);
  http.addHeader("Cookie", cookie);
  int httpCode = http.GET();

  String payload = http.getLocation();
  token = payload.substring(payload.indexOf("access_token=") + 13,
                            payload.indexOf("&token_type"));
  Serial.print("Token: ");
  Serial.println(token);

  http.end();
}

void getPlayer() {
  HTTPClient http;
  http.begin("https://api.spotify.com/v1/me/player");
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Accept", "application/json");
  http.addHeader("Authorization", "Bearer " + token);
  int httpCode = http.GET();
  String payload = http.getString();
  // Serial.println(payload);
  if (httpCode == HTTP_CODE_OK) {
    DynamicJsonDocument json(20000);
    deserializeJson(json, payload);

    title = json["item"]["name"].as<String>();
    album = json["item"]["album"]["name"].as<String>();
    artists = "";
    JsonArray arr = json["item"]["artists"].as<JsonArray>();
    for (JsonVariant value : arr) {
      artists += value["name"].as<String>() + " ";
    }

    current = json["progress_ms"].as<long>();
    duration = json["item"]["duration_ms"].as<long>();
    playing = json["is_playing"].as<bool>();

    device = json["device"]["name"].as<String>();
    isActive = json["device"]["is_active"].as<bool>();
    type = json["device"]["type"].as<String>();
    volume = json["device"]["volume_percent"].as<int>();
    isExpired = false;

  } else {
    current = 0, duration = 10;
    volume = 0;
    artists = "", title = "ERROR", album = "Token expired", device = "",
    type = "";
    isActive = false, playing = false;
    isExpired = true;
  }

  http.end();
}

void updateScreen() {
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);

  display.drawString(64, 0, title);
  display.drawString(64, 10, artists);
  display.drawString(64, 20, album);
  display.drawString(64, 30, playing ? ">" : "ll");
  display.drawString(64, 40, "Vol. " + String(volume) + "%");

  // Duration calculation
  int minute = duration / 60000;
  int second = (duration % 60000) / 1000;

  // Current position calculation
  int minuteCurrent = current / 60000;
  int secondCurrent = (current % 60000) / 1000;

  // Progress Calculation
  int progress = map(current, 0, duration, 0, 100);

  // Progress Bar
  display.drawProgressBar(0, 55, 127, 8, progress);
  // Serial.println(progress);

  // Current song position
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 40,
                     String(minuteCurrent) + ":" +
                         (secondCurrent < 10 ? "0" : "") +
                         String(secondCurrent));

  // Duration of song
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(
      128, 40,
      String(minute) + ":" + (second < 10 ? "0" : "") + String(second));

  display.display();
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting device...");
  wifimulti.addAP(ssid, password);

  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

  display.drawString(0, 0, "Connecting to Wifi...");
  display.display();
  Serial.println("Connecting to WiFi");
  while ((wifimulti.run() != WL_CONNECTED)) {
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
  display.clear();
  delay(1000);

  getToken();
}

void loop() {
  if (isExpired) {
    getToken();
  }

  getPlayer();
  updateScreen();
}
