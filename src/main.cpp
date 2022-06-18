#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <Wire.h>

// Wifi credentials
const char* ssid = "TINO'S_2.4GHz@unifi";
const char* password = "011114130297";

// Spotify API credentials
String client_id = "da6c8c1eee91408a8a7e575c78539536";
String redirect = "https%3A%2F%2Fhttpbin.org%2Fanything";
String cookie =
    "sp_t=170877b241c30e36cd2d367c87745201; "
    "__Host-device_id=AQAfJeTVzyH31v_"
    "slCJJPXhw2c8A4LNc1TNlsxo44qK2Y72UFThRJltP2vRVFAbjnn_XeZP_"
    "HKvYmd7sKiciWHzPfDUOiFmjN24; "
    "OptanonAlertBoxClosed=2022-04-02T10:30:28.858Z; "
    "sp_dc="
    "AQBH3IcFN1TV71NIZfaTErygtpKk61eFhWAtac9FHsZlpJRC8evbMWZHkt4goDydTD7TtocxaV"
    "asiLM3U-nW_ZI1QvwLze5BzHEYSVr_vGNYtsuIGZu6X-w7y93khQJcd-_1usacQJGb7RC-"
    "za2IZyYlMcUJPP3s; sp_key=d7e58792-892d-47dc-994e-c32deeef14c6; "
    "sp_m=my-en; inapptestgroup=; sp_landing=https://www.spotify.com/my-en/; "
    "OptanonConsent=isIABGlobal=false&datestamp=Sat+Jun+18+2022+13:38:31+GMT+"
    "0800+(Malaysia+Time)&version=6.26.0&hosts=&landingPath=NotLandingPage&"
    "groups=s00:1,f00:1,m00:1,t00:1,i00:1,f02:1,m02:1,t02:1&AwaitingReconsent="
    "false&geolocation=MY;13; "
    "__HOST-sp_fid=50486bcb-e3eb-4067-a74e-3740516be994; sp_tr=true; "
    "__Host-sp_csrf_sid="
    "b48918708927c7142c183d0af03b0409864bf3cf68495766e51da390b11abb59; "
    "csrf_token=AQB-"
    "fHcsOl26PNdSUbMpLExulOAjinPiVqQt2HWnitKZTcbv0dvsFfxvkXswjctJXWiJYBCEi0p1Si"
    "Dc2cDuNQ7CTB7eDsEucCaL6Oau1T74ZsHi";

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
Adafruit_SSD1306 display(128, 64, &Wire, -1);
bool dim = true;

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
  display.clearDisplay();

  // Song Title
  display.setCursor(0, 0);
  display.println(title);

  display.println(artists);

  display.println(album);

  display.display();
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting device...");
  wifimulti.addAP(ssid, password);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Allocation Failed"));
    for (;;)
      ;
  }

  Serial.println("Conneting to WiFi");

  display.dim(dim);
  display.display();
  delay(1000);

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Connecting...");
  display.display();
  delay(1000);
  display.clearDisplay();
  display.display();

  while ((wifimulti.run() != WL_CONNECTED)) {
    Serial.print(".");
  }

  Serial.println("Connected to WiFi");
  display.print("Connected to Wifi");
  display.display();
  delay(3000);

  getToken();
}

void loop() {
  if (isExpired) {
    getToken();
  }

  getPlayer();
  updateScreen();
}