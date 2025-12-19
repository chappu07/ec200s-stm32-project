#include <DHT.h>

// ---------------- PIN CONFIG ----------------
#define DHTPIN PA5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ---------------- UART CONFIG ----------------
HardwareSerial Serial3(PB11, PB10);   // EC200S (RX, TX)
HardwareSerial Serial2(PA3, PA2);     // FTDI debug

// ---------------- MQTT CONFIG ----------------
#define MQTT_BROKER  "broker.hivemq.com"
#define MQTT_PORT    1883
#define MQTT_TOPIC   "test/safwan"
#define MQTT_CLIENT  "stm32client"

// ---------------- HELPERS ----------------
bool waitFor(const String &match, uint32_t timeout) {
  unsigned long start = millis();
  String buf = "";
  while (millis() - start < timeout) {
    while (Serial3.available()) {
      char c = Serial3.read();
      buf += c;
      Serial2.write(c);
      if (buf.indexOf(match) != -1) return true;
    }
  }
  return false;
}

bool sendAT(String cmd, String match, uint32_t timeout) {
  Serial2.println(">> " + cmd);
  Serial3.println(cmd);
  return waitFor(match, timeout);
}

// ---------------- SETUP ----------------
void setup() {
  Serial2.begin(115200);
  Serial3.begin(115200);
  dht.begin();
  delay(1500);

  Serial2.println("\n=== EC200S MQTT (PUBLIC BROKER) BEGIN ===");

  sendAT("AT", "OK", 1000);
  sendAT("AT+CPIN?", "READY", 2000);
  sendAT("AT+CREG?", "1", 3000);
  sendAT("AT+CGATT?", "1", 3000);
  sendAT("AT+CSQ", "OK", 2000);

  sendAT("AT+QICSGP=1,1,\"airtelgprs.com\",\"\",\"\",1", "OK", 4000);
  sendAT("AT+QIDEACT=1", "OK", 3000);
  sendAT("AT+QIACT=1", "OK", 8000);
  sendAT("AT+QIACT?", "+QIACT:", 5000);

  // MQTT settings
  sendAT("AT+QMTCFG=\"recv-mode\",0,0,1", "OK", 2000);

  // Open MQTT connection
  String openCmd = "AT+QMTOPEN=0,\"" MQTT_BROKER "\"," + String(MQTT_PORT);
  sendAT(openCmd, "+QMTOPEN: 0,0", 15000);

  // Connect to broker
  String connCmd = "AT+QMTCONN=0,\"" MQTT_CLIENT "\"";
  sendAT(connCmd, "+QMTCONN: 0,0", 8000);

  Serial2.println("MQTT CONNECTED!");
}

// ---------------- MQTT PUBLISH FUNCTION ----------------
void mqttPublish(String payload) {
  String pubCmd = "AT+QMTPUB=0,0,0,0,\"" MQTT_TOPIC "\"";
  Serial3.println(pubCmd);

  if (!waitFor(">", 3000)) {
    Serial2.println("!! MQTT PUBLISH ERROR");
    return;
  }

  Serial3.print(payload);
  waitFor("+QMTPUB", 5000);

  Serial2.println("Published: " + payload);
}

// ---------------- LOOP ----------------
void loop() {

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial2.println("DHT FAILED");
    delay(3000);
    return;
  }

  String payload = "{\"temp\":" + String(t,1) +
                   ",\"hum\":" + String(h,1) + "}";

  mqttPublish(payload);

  delay(5000);
}
