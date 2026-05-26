using Api.Models;
using dataaccess.Entities;
using DefaultNamespace;
using Microsoft.EntityFrameworkCore;
using StateleSSE.AspNetCore;
using StateleSSE.AspNetCore.EfRealtime;

namespace Api.Services;

public class TelemetryRealtimeService : ITelemetryRealtimeService
{
    private readonly ISseBackplane _backplane;
    private readonly IRealtimeManager _realtimeManager;
    private readonly AppDbContext _db;

    public TelemetryRealtimeService(
        ISseBackplane backplane,
        IRealtimeManager realtimeManager,
        AppDbContext db)
    {
        _backplane = backplane;
        _realtimeManager = realtimeManager;
        _db = db;
    }

    public async Task<RealtimeListenResponse<List<TelemetryRealtimeDto>>> GetTelemetryAsync(
        string connectionId,
        string deviceId,
        int minutesBack = 60,
        int maxPoints = 1000)
    {
        deviceId = (deviceId ?? "").Trim();

        if (string.IsNullOrWhiteSpace(deviceId))
            throw new ArgumentException("Missing deviceId");

        var group = $"telemetry:{deviceId}";

        await _backplane.Groups.AddToGroupAsync(connectionId, group);
        
        Console.WriteLine($"Subscribing connection {connectionId} to device {deviceId}");

        _realtimeManager.Subscribe<AppDbContext>(
            connectionId,
            group,
            criteria: changes =>
            {
                Console.WriteLine("EFRealtime detected SaveChanges");
                var hasTelemetryChanges = changes.HasChanges<TelemetryReading>();
                Console.WriteLine($"TelemetryReading changed: {hasTelemetryChanges}");
                return hasTelemetryChanges;
            },
            query: async ctx => await LoadTelemetryAsync(ctx, deviceId, minutesBack, maxPoints));

        var initialData = await LoadTelemetryAsync(_db, deviceId, minutesBack, maxPoints);

        return new RealtimeListenResponse<List<TelemetryRealtimeDto>>(group, initialData);
    }

    private async Task<List<TelemetryRealtimeDto>> LoadTelemetryAsync(
        AppDbContext ctx,
        string deviceId,
        int minutesBack,
        int maxPoints)
    {
        var device = await ctx.Devices
            .AsNoTracking()
            .FirstOrDefaultAsync(d => d.DeviceId == deviceId);

        if (device == null)
            return new List<TelemetryRealtimeDto>();

        var from = DateTimeOffset.UtcNow.AddMinutes(-minutesBack);

        return await ctx.TelemetryReadings
            .AsNoTracking()
            .Where(r => r.DeviceIdFk == device.Id && r.TimestampUtc >= from)
            .OrderByDescending(r => r.TimestampUtc)
            .Take(maxPoints)
            .OrderBy(r => r.TimestampUtc)
            .Select(r => new TelemetryRealtimeDto
            {
                TimestampUtc = r.TimestampUtc,

                GasAnalogValue = r.GasAnalogValue,
                GasDigitalValue = r.GasDigitalValue,
                GasBaseline = r.GasBaseline,
                GasDangerThreshold = r.GasDangerThreshold,
                GasDangerDetected = r.GasDangerDetected,

                TemperatureC = r.TemperatureC,
                HumidityPercent = r.HumidityPercent,
                PressureHpa = r.PressureHpa,

                BuzzerActive = r.BuzzerActive
            })
            .ToListAsync();
    }
}