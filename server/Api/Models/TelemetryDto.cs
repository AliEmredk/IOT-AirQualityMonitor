namespace Api.Models;

public class TelemetryDto
{
    public string DeviceId { get; set; } = "";
    public long Timestamp { get; set; }

    public GasDto Gas { get; set; } = new();

    public EnvironmentDto Environment { get; set; } = new();

    public AlarmDto Alarm { get; set; } = new();
}

public class GasDto
{
    public int AnalogValue { get; set; }
    
    public int DigitalValue { get; set; }
    
    public int Baseline { get; set; }
    
    public int DangerThreshold { get; set; }
    
    public bool DangerDetected { get; set; }
}

public class EnvironmentDto
{
    public double TemperatureC { get; set; }
    
    public double HumidityPercent { get; set; }
    
    public double PressureHpa { get; set; }
}

public class AlarmDto
{
    public bool BuzzerActive { get; set; }
}