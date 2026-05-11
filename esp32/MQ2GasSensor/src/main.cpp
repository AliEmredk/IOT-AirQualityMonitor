#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_BME280.h>
#include <config.h>
#include <PubSubClient.h>
#include <time.h>

const int MQ2_AO_PIN = 34;
const int MQ2_DO_PIN = 25;
const int BUZZER_PIN = 26;

// BME280 I2C pins
const int SDA_PIN = 21;
const int SCL_PIN = 22;

Adafruit_BME280 bme;

int baseline = 0;
int dangerThreshold = 0;

const int WARMUP_SECONDS = 120;
const int BASELINE_SAMPLES = 50;
const int DANGER_OFFSET = 500;

unsigned long lastWiFiCheck = 0;
const unsigned long WIFI_CHECK_INTERVAL = 5000;

const char* mqtt_server = "mqtt.flespi.io";
const int mqtt_port = 1883;
const char* mqtt_user = MQTT_USER;

const char* DEVICE_ID = "esp32-air-monitor-01";

String mqttTopic;

WiFiClient espClient;
PubSubClient client(espClient);

void beepBuzzer() {
  for (int i = 0; i < 100; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(500);
    digitalWrite(BUZZER_PIN, LOW);
    delayMicroseconds(500);
  }
}

void onWiFiEvent(WiFiEvent_t event) {
  switch (event) {
    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
      Serial.println("WiFi connected to router.");
      break;

    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      Serial.print("WiFi IP address: ");
      Serial.println(WiFi.localIP());
      break;

    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      Serial.println("WiFi disconnected.");
      break;

    default:
      break;
  }
}

bool connectToWiFi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long startAttemptTime = millis();
  const unsigned long WIFI_TIMEOUT = 10000;

  while (WiFi.status() != WL_CONNECTED &&
         millis() - startAttemptTime < WIFI_TIMEOUT) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi connection successful.");
    return true;
  } else {
    Serial.println("WiFi connection failed.");
    return false;
  }
}

void connectToMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to Flespi MQTT...");

    String clientId = "esp32-air-monitor-01-" + String(random(0xffff), HEX);

if (client.connect(clientId.c_str(), mqtt_user, NULL)) {
      Serial.println("Connected!");
    } else {
      Serial.print("Connection failed! rc=");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(MQ2_DO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  WiFi.onEvent(onWiFiEvent);
  connectToWiFi();

  configTime(0, 0, "pool.ntp.org", "time.nist.gov");

Serial.println("Syncing time with NTP...");

struct tm timeinfo;
while (!getLocalTime(&timeinfo)) {
  Serial.println("Waiting for NTP time...");
  delay(1000);
}

Serial.println("Time synchronized!");

  client.setServer(mqtt_server, mqtt_port);
  client.setBufferSize(1024);

  // Start I2C for BME280
  Wire.begin(SDA_PIN, SCL_PIN);

  Serial.println("Starting BME280...");

  if (!bme.begin(0x76)) {
    Serial.println("Could not find BME280 at 0x76, trying 0x77...");

    if (!bme.begin(0x77)) {
      Serial.println("Could not find BME280 sensor!");
      Serial.println("Check wiring: VCC, GND, SDA=21, SCL=22");
    } else {
      Serial.println("BME280 found at 0x77");
    }
  } else {
    Serial.println("BME280 found at 0x76");
  }

  Serial.println("MQ-2 Gas Sensor Starting...");
  Serial.println("Warming up sensor...");

  for (int i = WARMUP_SECONDS; i > 0; i--) {
    Serial.print("Warm-up remaining: ");
    Serial.print(i);
    Serial.println(" seconds");
    delay(1000);
  }

  Serial.println("Measuring clean air baseline...");
  long total = 0;

  for (int i = 0; i < BASELINE_SAMPLES; i++) {
    int value = analogRead(MQ2_AO_PIN);
    total += value;

    Serial.print("Baseline sample ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(value);

    delay(200);
  }

  baseline = total / BASELINE_SAMPLES;
  dangerThreshold = baseline + DANGER_OFFSET;

  Serial.println("Calibration finished.");
  Serial.print("Clean air baseline: ");
  Serial.println(baseline);
  Serial.print("Danger threshold: ");
  Serial.println(dangerThreshold);
  Serial.println("System ready.");

  mqttTopic = "air-monitor/";
mqttTopic += DEVICE_ID;
mqttTopic += "/telemetry";

Serial.print("MQTT Topic: ");
Serial.println(mqttTopic);
}

void handleBuzzer(bool isDanger) {
  if (isDanger) {
    Serial.println("DANGEROUS GAS DETECTED!");
    beepBuzzer();
  } else {
    Serial.println("Air looks normal");
    digitalWrite(BUZZER_PIN, LOW);
  }
}

long getTimestamp() {
  time_t now;
  time(&now);
  return now; // seconds since 1970 (UTC)
}

String buildPayload(int analogValue, int digitalValue, float temperature, float humidity, float pressure, bool isDanger) {
  String payload = "{";
  payload += "\"deviceId\":\"" + String(DEVICE_ID) + "\",";
  payload += "\"timestamp\":" + String(getTimestamp()) + ",";

  payload += "\"gas\":{";
  payload += "\"analogValue\":" + String(analogValue) + ",";
  payload += "\"digitalValue\":" + String(digitalValue) + ",";
  payload += "\"baseline\":" + String(baseline) + ",";
  payload += "\"dangerThreshold\":" + String(dangerThreshold) + ",";
  payload += "\"dangerDetected\":" + String(isDanger ? "true" : "false");
  payload += "},";

  payload += "\"environment\":{";
  payload += "\"temperatureC\":" + String(temperature) + ",";
  payload += "\"humidityPercent\":" + String(humidity) + ",";
  payload += "\"pressureHpa\":" + String(pressure);
  payload += "},";

  payload += "\"alarm\":{";
  payload += "\"buzzerActive\":" + String(isDanger ? "true" : "false");
  payload += "}";

  payload += "}";
  return payload;
}



void loop() {
  // WiFi reconnect
  if (millis() - lastWiFiCheck >= WIFI_CHECK_INTERVAL) {
    lastWiFiCheck = millis();

    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi not connected. Trying to reconnect...");
      connectToWiFi();
    }
  }

  // MQTT reconnect
  if (!client.connected()) {
    connectToMQTT();
  }
  client.loop();

  Serial.print("MQTT state: ");
Serial.println(client.state());

  // Read sensors
  int analogValue = analogRead(MQ2_AO_PIN);
  int digitalValue = digitalRead(MQ2_DO_PIN);

  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0F;

  bool isDanger = analogValue > dangerThreshold;

  // Debug logs
  Serial.println("------------------------------");
  Serial.print("Gas: "); Serial.println(analogValue);
  Serial.print("Temp: "); Serial.println(temperature);
  Serial.print("Humidity: "); Serial.println(humidity);
  Serial.print("Pressure: "); Serial.println(pressure);

  // Handle alarm
  handleBuzzer(isDanger);

  // Build & send payload
  String payload = buildPayload(
    analogValue,
    digitalValue,
    temperature,
    humidity,
    pressure,
    isDanger
  );

  Serial.print("Payload length: ");
  Serial.println(payload.length());

  bool success = client.publish(mqttTopic.c_str(), payload.c_str());

if (success) {
  Serial.println("MQTT publish SUCCESS");
} else {
  Serial.println("MQTT publish FAILED");
}

  delay(1000);
}

