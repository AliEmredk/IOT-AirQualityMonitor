namespace dataaccess.Entities;

public class TelemetryReading
{
    public int Id { get; set; }

    public int DeviceIdFk { get; set; }
    public Device Device { get; set; } = null!;

    // From ESP32 payload timestamp check what special with unix
    public long TimestampUnix { get; set; }

    public DateTimeOffset TimestampUtc { get; set; }

    // Gas
    public int GasAnalogValue { get; set; }
    public int GasDigitalValue { get; set; }
    public int GasBaseline { get; set; }
    public int GasDangerThreshold { get; set; }
    public bool GasDangerDetected { get; set; }

    // Environment
    public double TemperatureC { get; set; }
    public double HumidityPercent { get; set; }
    public double PressureHpa { get; set; } 

    // Alarm
    public bool BuzzerActive { get; set; }

    // Backend receive time
    public DateTimeOffset CreatedAtUtc { get; set; } = DateTimeOffset.UtcNow;
}