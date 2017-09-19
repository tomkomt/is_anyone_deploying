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

char wifiSsid[] = "12te";
char wifiPswd[] = "tusomdoma";
char url[] = "https://9bf47b18-d4a7-437d-9a02-0c8456cdbcdb-bluemix:f3f4c7d8dd4e4237242c0987302d6b3a4a7370480ca465e86fdff721f57e52d9@9bf47b18-d4a7-437d-9a02-0c8456cdbcdb-bluemix.cloudant.com/tonis_deployments/_all_docs";
char* certThumbprint = "9F:53:E5:11:69:A4:85:22:00:19:AB:1B:A4:19:18:D5:F1:AD:21:20";

void shutAll() {
  digitalWrite(redLedPin, LOW);
  digitalWrite(yelLedPin, LOW);
  digitalWrite(greLedPin, LOW);
  delay(100);
}

void notConnectedState() {
  for (int i = 0; i <= 4; i++) {
    shutAll();
    digitalWrite(yelLedPin, HIGH);
    delay(250);
    digitalWrite(yelLedPin, LOW);
    delay(100);
    digitalWrite(redLedPin, HIGH);
    delay(250);
    digitalWrite(redLedPin, LOW);
    delay(750);
  }
}

void getFailureState() {
  shutAll();
  digitalWrite(redLedPin, HIGH);
  delay(10000);
}

void getSuccessState() {
  shutAll();
  digitalWrite(greLedPin, HIGH);
  delay(10000);
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

void setup() {
  pinMode(redLedPin, OUTPUT);
  pinMode(yelLedPin, OUTPUT);
  pinMode(greLedPin, OUTPUT);
  Serial.begin(115200);

  WiFiMulti.addAP(wifiSsid, wifiPswd);
  
  diodesTest();
}

void loop() {
  if((WiFiMulti.run() == WL_CONNECTED)) {
    Serial.println("[WIFI] Connected");
    
    HTTPClient http;
    Serial.println("[HTTP] begin...");
    http.begin(url, certThumbprint);
    getPendingState();
    Serial.println("[HTTP] request payload...");
    int httpCode = http.GET();
    if(httpCode > 0) {
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      String payload = http.getString();
      Serial.println(payload);
      getSuccessState();
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      getFailureState();
    }
    http.end();
  } else {
    Serial.println("[WIFI] Not connected");
    notConnectedState();
  }
  Serial.println("Reconnecting...");  
}
