using Mqtt.Controllers;

namespace api.Services;

public class MqttConnectHostedService(
    IMqttClientService mqtt,
    ILogger<MqttConnectHostedService> logger
) : IHostedService
{
    public async Task StartAsync(CancellationToken cancellationToken)
    {
        await mqtt.ConnectAsync(
            "mqtt.flespi.io",
            1883,
            "YOUR_FLESPI_TOKEN",
            ""
        );

        logger.LogInformation("✅ MQTT connected to mqtt.flespi.io");
    }

    public Task StopAsync(CancellationToken cancellationToken) => Task.CompletedTask;
}