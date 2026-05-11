using dataaccess.Entities;
using Microsoft.EntityFrameworkCore;

namespace DefaultNamespace;

public class AppDbContext : DbContext
{
    public AppDbContext(DbContextOptions<AppDbContext> options)
        : base(options)
    {
    }

    public DbSet<Device> Devices => Set<Device>();
    public DbSet<TelemetryReading> TelemetryReadings => Set<TelemetryReading>();

    protected override void OnModelCreating(ModelBuilder modelBuilder)
    {
        modelBuilder.Entity<Device>(entity =>
        {
            entity.HasKey(d => d.Id);

            entity.Property(d => d.DeviceId)
                .IsRequired()
                .HasMaxLength(100);

            entity.HasIndex(d => d.DeviceId)
                .IsUnique();
        });

        modelBuilder.Entity<TelemetryReading>(entity =>
        {
            entity.HasKey(t => t.Id);

            entity.HasOne(t => t.Device)
                .WithMany(d => d.TelemetryReadings)
                .HasForeignKey(t => t.DeviceIdFk)
                .OnDelete(DeleteBehavior.Cascade);

            entity.HasIndex(t => t.TimestampUtc);
            entity.HasIndex(t => new { t.DeviceIdFk, t.TimestampUtc });

            entity.Property(t => t.TemperatureC).HasPrecision(6, 2);
            entity.Property(t => t.HumidityPercent).HasPrecision(6, 2);
            entity.Property(t => t.PressureHpa).HasPrecision(7, 2);
        });
    }
}