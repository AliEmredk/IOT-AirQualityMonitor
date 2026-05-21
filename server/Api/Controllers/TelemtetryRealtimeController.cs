using Api.Models;
using Api.Services;
using Microsoft.AspNetCore.Mvc;
using StateleSSE.AspNetCore;
using StateleSSE.AspNetCore.EfRealtime;

namespace Api.Controllers;

[ApiController]
[Route("api/realtime")]
public class TelemetryRealtimeController : RealtimeControllerBase
{
    private readonly ITelemetryRealtimeService _service;

    public TelemetryRealtimeController(
        ITelemetryRealtimeService service,
        ISseBackplane backplane) : base(backplane)
    {
        _service = service;
    }

    [HttpGet("telemetry")]
    public async Task<RealtimeListenResponse<List<TelemetryRealtimeDto>>> GetTelemetry(
        [FromQuery] string connectionId,
        [FromQuery] string deviceId,
        [FromQuery] int minutesBack = 60,
        [FromQuery] int maxPoints = 1000)
    {
        return await _service.GetTelemetryAsync(
            connectionId,
            deviceId,
            minutesBack,
            maxPoints
        );
    }
}