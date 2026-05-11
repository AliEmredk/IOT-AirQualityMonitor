using dataaccess.Entities;
using DefaultNamespace;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;

namespace Api.Controllers;

[ApiController]
[Route("api/telemetry")]
public class TelemetryController(AppDbContext db) : ControllerBase
{
    [HttpGet("{deviceId}/latest")]
    public async Task<ActionResult<TelemetryReading>> GetLatest(string deviceId)
    {
        var latest = await db.TelemetryReadings
            .Include(t => t.Device)
            .Where(t => t.Device.DeviceId == deviceId)
            .OrderByDescending(t => t.TimestampUtc)
            .FirstOrDefaultAsync();
        
        if(latest == null)
            return NotFound();
        
        return Ok(latest);
    }

    [HttpGet("{deviceId}/history")]
    public async Task<ActionResult<List<TelemetryReading>>> GetHistory(string deviceId, int limit = 100)
    {
        var history = await db.TelemetryReadings
            .Include(t => t.Device)
            .Where(t => t.Device.DeviceId == deviceId)
            .OrderByDescending(t => t.TimestampUtc)
            .Take(limit)
            .ToListAsync();
        
        return Ok(history);
    }
}