export type TelemetryReading = {
    id: number;
    timestampUtc: string;
    gasAnalogValue: number;
    gasDigitalValue: number;
    gasBaseline: number;
    gasDangerThreshold: number;
    gasDangerDetected: boolean;
    temperatureC: number;
    humidityPercent: number;
    pressureHpa: number;
    buzzerActive: boolean;
};