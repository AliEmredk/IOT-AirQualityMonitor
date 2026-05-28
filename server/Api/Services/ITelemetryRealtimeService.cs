using Api.Models;
using StateleSSE.AspNetCore.EfRealtime;

namespace Api.Services;

public interface ITelemetryRealtimeService
{
    Task<RealtimeListenResponse<List<TelemetryRealtimeDto>>> GetTelemetryAsync(
        string connectionId,
        string deviceId,
        int minutesBack = 60,
        int maxPoints = 1000);
}