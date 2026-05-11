using System;
using Microsoft.EntityFrameworkCore.Migrations;
using Npgsql.EntityFrameworkCore.PostgreSQL.Metadata;

#nullable disable

namespace dataaccess.Migrations
{
    /// <inheritdoc />
    public partial class InitialCreate : Migration
    {
        /// <inheritdoc />
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.CreateTable(
                name: "Devices",
                columns: table => new
                {
                    Id = table.Column<int>(type: "integer", nullable: false)
                        .Annotation("Npgsql:ValueGenerationStrategy", NpgsqlValueGenerationStrategy.IdentityByDefaultColumn),
                    DeviceId = table.Column<string>(type: "character varying(100)", maxLength: 100, nullable: false),
                    Name = table.Column<string>(type: "text", nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Devices", x => x.Id);
                });

            migrationBuilder.CreateTable(
                name: "TelemetryReadings",
                columns: table => new
                {
                    Id = table.Column<int>(type: "integer", nullable: false)
                        .Annotation("Npgsql:ValueGenerationStrategy", NpgsqlValueGenerationStrategy.IdentityByDefaultColumn),
                    DeviceIdFk = table.Column<int>(type: "integer", nullable: false),
                    TimestampUnix = table.Column<long>(type: "bigint", nullable: false),
                    TimestampUtc = table.Column<DateTimeOffset>(type: "timestamp with time zone", nullable: false),
                    GasAnalogValue = table.Column<int>(type: "integer", nullable: false),
                    GasDigitalValue = table.Column<int>(type: "integer", nullable: false),
                    GasBaseline = table.Column<int>(type: "integer", nullable: false),
                    GasDangerThreshold = table.Column<int>(type: "integer", nullable: false),
                    GasDangerDetected = table.Column<bool>(type: "boolean", nullable: false),
                    TemperatureC = table.Column<double>(type: "double precision", precision: 6, scale: 2, nullable: false),
                    HumidityPercent = table.Column<double>(type: "double precision", precision: 6, scale: 2, nullable: false),
                    PressureHpa = table.Column<double>(type: "double precision", precision: 7, scale: 2, nullable: false),
                    BuzzerActive = table.Column<bool>(type: "boolean", nullable: false),
                    CreatedAtUtc = table.Column<DateTimeOffset>(type: "timestamp with time zone", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_TelemetryReadings", x => x.Id);
                    table.ForeignKey(
                        name: "FK_TelemetryReadings_Devices_DeviceIdFk",
                        column: x => x.DeviceIdFk,
                        principalTable: "Devices",
                        principalColumn: "Id",
                        onDelete: ReferentialAction.Cascade);
                });

            migrationBuilder.CreateIndex(
                name: "IX_Devices_DeviceId",
                table: "Devices",
                column: "DeviceId",
                unique: true);

            migrationBuilder.CreateIndex(
                name: "IX_TelemetryReadings_DeviceIdFk_TimestampUtc",
                table: "TelemetryReadings",
                columns: new[] { "DeviceIdFk", "TimestampUtc" });

            migrationBuilder.CreateIndex(
                name: "IX_TelemetryReadings_TimestampUtc",
                table: "TelemetryReadings",
                column: "TimestampUtc");
        }

        /// <inheritdoc />
        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropTable(
                name: "TelemetryReadings");

            migrationBuilder.DropTable(
                name: "Devices");
        }
    }
}
