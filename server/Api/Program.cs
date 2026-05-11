using api.Services;
using Mqtt.Controllers;

var builder = WebApplication.CreateBuilder(args);

builder.Services.AddOpenApi();

builder.Services.AddControllers();
builder.Services.AddMqttControllers();

builder.Services.AddHostedService<MqttConnectHostedService>();

// Remember to register your own services too
//builder.Services.AddScoped<IWindmillTelemetryService, WindmillTelemetryService>();

var app = builder.Build();

if (app.Environment.IsDevelopment())
{
    app.MapOpenApi();
}

app.UseHttpsRedirection();

app.MapControllers();

app.Run();