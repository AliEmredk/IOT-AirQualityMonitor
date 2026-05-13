using Mqtt.Controllers;

namespace api.Services;

public class MqttConnectHostedService(
    IMqttClientService mqtt,
    ILogger<MqttConnectHostedService> logger
) : IHostedService
{
    public async Task StartAsync(CancellationToken cancellationToken)
    {
        var host = Environment.GetEnvironmentVariable("MQTT_HOST");
        var port = int.Parse(Environment.GetEnvironmentVariable("MQTT_PORT")!);
        var user = Environment.GetEnvironmentVariable("MQTT_USER");
        var password = Environment.GetEnvironmentVariable("MQTT_PASSWORD");

        await mqtt.ConnectAsync(
            host!,
            port,
            user!,
            password
        );

        logger.LogInformation("✅ MQTT connected to {Host}", host);
    }

    public Task StopAsync(CancellationToken cancellationToken) => Task.CompletedTask;
}