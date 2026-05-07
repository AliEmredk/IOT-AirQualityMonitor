namespace dataaccess.Entities;

public class Device
{
    public int Id { get; set; }

    public string DeviceId { get; set; } = null!;

    public string? Name { get; set; }

    public List<TelemetryReading> TelemetryReadings { get; set; } = new();
}