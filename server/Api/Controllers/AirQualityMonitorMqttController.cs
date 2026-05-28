using Api.Models;
using Api.Services;
using Mqtt.Controllers;

namespace Api.Controllers;

public class AirQualityMonitorMqttController(
    ILogger<AirQualityMonitorMqttController> logger,
    MqttTelemetryService telemetryService
) : MqttController
{
    [MqttRoute("air-monitor/{deviceId}/telemetry")]
    public async Task HandleTelemetry(string deviceId, TelemetryDto data)
    {
        logger.LogInformation(
            "Received MQTT telemetry from topic device={TopicDeviceId}, payload device={PayloadDeviceId}",
            deviceId,
            data.DeviceId
        );

        await telemetryService.SaveTelemetryAsync(deviceId, data);
    }
}