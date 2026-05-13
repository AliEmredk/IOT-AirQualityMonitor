using api.Services;
using Api.Services;
using DefaultNamespace;
using DotNetEnv;
using Microsoft.EntityFrameworkCore;
using Mqtt.Controllers;
using System.Text.Json.Serialization;

Env.Load();

var builder = WebApplication.CreateBuilder(args);

var connectionString =
    Environment.GetEnvironmentVariable("CONN_STR")
    ?? builder.Configuration.GetConnectionString("DefaultConnection");

builder.Services.AddDbContext<AppDbContext>(options =>
    options.UseNpgsql(connectionString));

builder.Services.AddCors(options =>
{
    options.AddPolicy("frontend", policy =>
    {
        policy
            .AllowAnyOrigin()
            .AllowAnyHeader()
            .AllowAnyMethod();
    });
});

builder.Services.AddControllers()
    .AddJsonOptions(options =>
    {
        options.JsonSerializerOptions.ReferenceHandler = ReferenceHandler.IgnoreCycles;
    });

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