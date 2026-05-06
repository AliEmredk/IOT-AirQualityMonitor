# JSON Payload Format

The ESP32 air monitor publishes sensor data to the MQTT broker using JSON.

## MQTT Topic

air-monitor/esp32-air-monitor-01/telemetry

# JSON Payload Format – Air Monitor System
Used for MQTT and HTTP communication between ESP32 and backend.

{
  "deviceId": "esp32-air-monitor-01",
  "timestamp": 12345678,
  "gas": {
    "analogValue": 1450,
    "digitalValue": 0,
    "baseline": 1350,
    "dangerThreshold": 1850,
    "dangerDetected": false
  },
  "environment": {
    "temperatureC": 23.6,
    "humidityPercent": 45.2,
    "pressureHpa": 1012.8
  },
  "alarm": {
    "buzzerActive": false
  }
}





Field Explanation
deviceId: Unique name of the ESP32 device
timestamp: Time from ESP32 using millis()
gas.analogValue: MQ-2 analog sensor value
gas.digitalValue: MQ-2 digital output value
gas.baseline: Clean air baseline value
gas.dangerThreshold: Gas danger threshold
gas.dangerDetected: True when gas value is above threshold
environment.temperatureC: Temperature from BME280
environment.humidityPercent: Humidity from BME280
environment.pressureHpa: Pressure from BME280
alarm.buzzerActive: True when buzzer is active