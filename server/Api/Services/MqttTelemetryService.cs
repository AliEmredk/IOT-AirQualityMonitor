using System.Text.Json;
using Api.Models;
using dataaccess.Entities;
using DefaultNamespace;
using Microsoft.EntityFrameworkCore;

namespace Api.Services;

public class MqttTelemetryService
{
    private readonly AppDbContext _db;

    public MqttTelemetryService(AppDbContext db)
    {
        this._db = db;
    }

    public async Task SaveTelemetryAsync(TelemetryDto dto)
    {
        var device = await _db.Devices
            .FirstOrDefaultAsync(d => d.DeviceId == dto.DeviceId);

        if (device == null)
        {
            device = new Device
            {
                DeviceId = dto.DeviceId,
            };

            _db.Devices.Add(device);
            await _db.SaveChangesAsync();
        }

        var reading = new TelemetryReading
        {
            DeviceIdFk = device.Id,
            TimestampUnix = dto.Timestamp,
            TimestampUtc = DateTimeOffset.FromUnixTimeSeconds(dto.Timestamp),

            GasAnalogValue = dto.Gas.AnalogValue,
            GasDigitalValue = dto.Gas.DigitalValue,
            GasBaseline = dto.Gas.Baseline,
            GasDangerThreshold = dto.Gas.DangerThreshold,
            GasDangerDetected = dto.Gas.DangerDetected,

            TemperatureC = dto.Environment.TemperatureC,
            HumidityPercent = dto.Environment.HumidityPercent,
            PressureHpa = dto.Environment.PressureHpa,

            BuzzerActive = dto.Alarm.BuzzerActive
        };

        _db.TelemetryReadings.Add(reading);

        await _db.SaveChangesAsync();
    }

    public async Task<List<TelemetryGraphDto>> GetGraphDataAsync(string deviceId, string range)
    {
        var now = DateTimeOffset.UtcNow;

        var from = range switch
        {
            "day" => now.AddDays(-1),
            _ => now.AddHours(-1)
        };
        
        var readings = await _db.TelemetryReadings
            .Include(r => r.Device)
            .Where(r =>
                r.Device.DeviceId == deviceId &&
                r.TimestampUtc >= from)
            .OrderBy(r => r.TimestampUtc)
            .ToListAsync();

        if (range == "day")
        {
            return readings
                .GroupBy(r => new
                {
                    r.TimestampUtc.Year,
                    r.TimestampUtc.Month,
                    r.TimestampUtc.Day,
                    r.TimestampUtc.Hour,
                    MinuteGroup = r.TimestampUtc.Minute / 5
                })
                .Select(g => new TelemetryGraphDto
                {
                    Timestamp = g.First().TimestampUtc,
                    GasAnalogValue = g.Average(x => x.GasAnalogValue),
                    TemperatureC = g.Average(x => x.TemperatureC),
                    HumidityPercent = g.Average(x => x.HumidityPercent),
                    PressureHpa = g.Average(x => x.PressureHpa)
                })
                .ToList();
        }
        
        return readings.Select(r => new TelemetryGraphDto
        {
            Timestamp = r.TimestampUtc,
            GasAnalogValue = r.GasAnalogValue,
            TemperatureC = r.TemperatureC,
            HumidityPercent = r.HumidityPercent,
            PressureHpa = r.PressureHpa
        })
            .ToList();
    }
}