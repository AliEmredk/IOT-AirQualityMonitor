#include <Arduino.h>

#include "config.h"
#include "pins.h"

#include "AppWiFi.h"
#include "MqttService.h"
#include "Bme280Sensor.h"
#include "GasSensor.h"
#include "Buzzer.h"
#include "TimeService.h"
#include "PayloadBuilder.h"

const char* DEVICE_ID = "esp32-air-monitor-01";

const char* MQTT_SERVER = "mqtt.flespi.io";
const int MQTT_PORT = 1883;

String mqttTopic;

AppWiFi appWiFi(WIFI_SSID, WIFI_PASSWORD);
MqttService mqttService(MQTT_SERVER, MQTT_PORT, MQTT_USER);

Bme280Sensor bmeSensor(SDA_PIN, SCL_PIN);
GasSensor gasSensor(MQ2_AO_PIN, MQ2_DO_PIN);
Buzzer buzzer(BUZZER_PIN);

TimeService timeService;
PayloadBuilder payloadBuilder(DEVICE_ID);

void setup() {
    Serial.begin(115200);
    delay(1000);

    gasSensor.begin();
    buzzer.begin();

    appWiFi.begin();

    timeService.syncTime();

    mqttService.begin();

    bmeSensor.begin();

    gasSensor.calibrate();

    Serial.println("System ready.");

    mqttTopic = "air-monitor/";
    mqttTopic += DEVICE_ID;
    mqttTopic += "/telemetry";

    Serial.print("MQTT Topic: ");
    Serial.println(mqttTopic);
}

void loop() {
    appWiFi.checkReconnect();

    mqttService.loop();

    Serial.print("MQTT state: ");
    Serial.println(mqttService.state());

    int analogValue = gasSensor.readAnalog();
    int digitalValue = gasSensor.readDigital();

    float temperature = bmeSensor.readTemperature();
    float humidity = bmeSensor.readHumidity();
    float pressure = bmeSensor.readPressure();

    bool isDanger = gasSensor.isDanger(analogValue);

    Serial.println("------------------------------");
    Serial.print("Gas: ");
    Serial.println(analogValue);
    Serial.print("Temp: ");
    Serial.println(temperature);
    Serial.print("Humidity: ");
    Serial.println(humidity);
    Serial.print("Pressure: ");
    Serial.println(pressure);

    buzzer.handle(isDanger);

    String payload = payloadBuilder.buildTelemetryPayload(
        timeService.getTimestamp(),
        analogValue,
        digitalValue,
        gasSensor.getBaseline(),
        gasSensor.getDangerThreshold(),
        temperature,
        humidity,
        pressure,
        isDanger
    );

    Serial.print("Payload length: ");
    Serial.println(payload.length());

    bool success = mqttService.publish(mqttTopic, payload);

    if (success) {
        Serial.println("MQTT publish SUCCESS");
    } else {
        Serial.println("MQTT publish FAILED");
    }

    delay(1000);
}