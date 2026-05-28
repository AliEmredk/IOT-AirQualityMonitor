using Api.Services;
using DefaultNamespace;
using DotNetEnv;
using Microsoft.EntityFrameworkCore;
using Mqtt.Controllers;
using System.Text.Json.Serialization;
using StateleSSE.AspNetCore;

Env.Load();

var builder = WebApplication.CreateBuilder(args);

var connectionString =
    Environment.GetEnvironmentVariable("CONN_STR")
    ?? builder.Configuration.GetConnectionString("DefaultConnection");

builder.Services.AddInMemorySseBackplane();
builder.Services.AddEfRealtime();

builder.Services.AddDbContext<AppDbContext>((sp, options) =>
{
    options.UseNpgsql(connectionString);
    options.AddEfRealtimeInterceptor(sp);
});

builder.Services.AddCors(options =>
{
    options.AddPolicy("frontend", policy =>
    {
        policy
            .WithOrigins(
                "http://localhost:5173",
                "https://airmonitoring.fly.dev/"
            )
            .AllowAnyHeader()
            .AllowAnyMethod();
    });
});

builder.Services.AddControllers()
    .AddJsonOptions(options =>
    {
        options.JsonSerializerOptions.ReferenceHandler = ReferenceHandler.IgnoreCycles;
    });

builder.Services.AddScoped<ITelemetryRealtimeService, TelemetryRealtimeService>();

builder.Services.AddScoped<MqttTelemetryService>();

builder.Services.AddHostedService<MqttConnectHostedService>();

builder.Services.AddEndpointsApiExplorer();
builder.Services.AddSwaggerGen();
builder.Services.AddMqttControllers();

var app = builder.Build();

app.UseDeveloperExceptionPage();

app.UseCors("frontend");
app.UseSwagger();
app.UseSwaggerUI();

app.MapControllers();

app.Run();