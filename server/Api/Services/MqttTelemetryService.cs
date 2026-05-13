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
}