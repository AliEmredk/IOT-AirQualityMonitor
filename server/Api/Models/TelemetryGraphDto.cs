namespace Api.Models;

public class TelemetryGraphDto
{
    public DateTimeOffset Timestamp { get; set; }
    public double? GasAnalogValue { get; set; }
    public double? TemperatureC { get; set; }
    public double? HumidityPercent { get; set; }
    public double? PressureHpa { get; set; }
}