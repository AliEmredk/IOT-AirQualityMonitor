namespace Api.Models;

public class TelemetryRealtimeDto
{
    public DateTimeOffset TimestampUtc { get; set; }

    public int GasAnalogValue { get; set; }
    public int GasDigitalValue { get; set; }
    public int GasBaseline { get; set; }
    public int GasDangerThreshold { get; set; }
    public bool GasDangerDetected { get; set; }

    public double TemperatureC { get; set; }
    public double HumidityPercent { get; set; }
    public double PressureHpa { get; set; }

    public bool BuzzerActive { get; set; }
}