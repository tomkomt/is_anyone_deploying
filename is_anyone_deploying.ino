#include <Arduino.h>
#include <math.h>
#include <stdio.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

ESP8266WiFiMulti WiFiMulti;

int redLedPin = 5;
int yelLedPin = 0;
int greLedPin = 2;                                                                                                                                                                                                      ;

char* wifiSsid = "12te";
char* wifiPswd = "tusomdoma";
char* host = "is-toni-deploying.eu-gb.mybluemix.net";
char* url = "/esp/opm";
char* certThumbprint = "4B E9 5D 98 11 8E E1 65 B6 53 7E 21 FC 2B F7 BE 73 36 85 0F";

void setup() {
  pinMode(redLedPin, OUTPUT);
  pinMode(yelLedPin, OUTPUT);
  pinMode(greLedPin, OUTPUT);
  Serial.begin(115200);

  WiFiMulti.addAP(wifiSsid, wifiPswd);

  diodesTest();
}

void loop() {
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    Serial.println("[WIFI] Connected");

    HTTPClient http;
    Serial.println("[HTTPS] begin...");
    http.begin(host, 443, url, true, certThumbprint);
    Serial.println("[HTTPS] connect...");
    int httpCode = http.GET();
    getPendingState();
    if (httpCode > 0) {
      Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
      String payload = http.getString();
      switch (payload.toInt()) {
        case 0:
          Serial.println("[Response] Server is DOWN...");
          getDownState();
          break;

        case 1:
          Serial.println("[Response] Server is UP...");
          getUpState();
          break;

        case 2:
          Serial.println("[Response] Last status is too old...");
          getTooOldState();
          break;

        default:
          Serial.println("[Response] Cannot understand server's status...");
          Serial.println(payload);
          getMismatchState();
          break;
      }
    } else {
      Serial.printf("[HTTPS] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      notConnectedState();
    }
    http.end();
  } else {
    Serial.println("[WIFI] Not connected");
    notConnectedState();
  }
  Serial.println("Reconnecting...");
}

void shutAll() {
  digitalWrite(redLedPin, LOW);
  digitalWrite(yelLedPin, LOW);
  digitalWrite(greLedPin, LOW);
  delay(100);
}

void notConnectedState() {
  shutAll();
  for(int i=0;i<25;i++) {
    digitalWrite(redLedPin, HIGH);
    digitalWrite(yelLedPin, HIGH);
    delay(600);
    digitalWrite(redLedPin, LOW);
    digitalWrite(yelLedPin, LOW);
    delay(600);
  }
}

void getMismatchState() {
  shutAll();
  for(int i=0;i<50;i++) {
    digitalWrite(redLedPin, HIGH);
    digitalWrite(yelLedPin, LOW);
    digitalWrite(greLedPin, HIGH);
    delay(600);
    digitalWrite(redLedPin, LOW);
    digitalWrite(yelLedPin, HIGH);
    digitalWrite(greLedPin, LOW);
    delay(600);
  }
}

void getDownState() {
  shutAll();
  digitalWrite(redLedPin, HIGH);
  delay(60000);
}

void getTooOldState() {
  shutAll();
  digitalWrite(redLedPin, HIGH);
  digitalWrite(yelLedPin, HIGH);
  digitalWrite(greLedPin, HIGH);
  delay(60000);
}

void getUpState() {
  shutAll();
  digitalWrite(greLedPin, HIGH);
  delay(60000);
}

void getPendingState() {
  shutAll();
  digitalWrite(yelLedPin, HIGH);
}

void diodesTest() {
  digitalWrite(redLedPin, HIGH);
  delay(200);
  digitalWrite(yelLedPin, HIGH);
  delay(200);
  digitalWrite(greLedPin, HIGH);
  delay(200);
  digitalWrite(greLedPin, LOW);
  delay(200);
  digitalWrite(yelLedPin, LOW);
  delay(200);
  digitalWrite(redLedPin, LOW);
  delay(500);
}
