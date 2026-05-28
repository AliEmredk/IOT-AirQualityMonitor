import type { TelemetryReading } from "../types/telemetry";

const API = import.meta.env.VITE_API_URL;

export async function getLatestTelemetry(deviceId: string): Promise<TelemetryReading> {
    const response = await fetch(`${API}/api/telemetry/${deviceId}/latest`);

    if (!response.ok) {
        throw new Error(`Latest request failed: ${response.status}`);
    }

    return response.json();
}

export async function getGraphData(
    deviceId: string,
    range: "hour" | "day"
): Promise<TelemetryReading[]> {
    const response = await fetch(`${API}/api/telemetry/${deviceId}/graph?range=${range}`);

    if (!response.ok) {
        throw new Error(`Graph request failed: ${response.status}`);
    }

    return response.json();
}

export async function subscribeToRealtimeTelemetry(
    connectionId: string,
    deviceId: string,
    minutesBack: number,
    maxPoints: number = 1000
): Promise<{ group: string; data: TelemetryReading[] }> {
    const response = await fetch(
        `${API}/api/realtime/telemetry?connectionId=${connectionId}&deviceId=${deviceId}&minutesBack=${minutesBack}&maxPoints=${maxPoints}`
    );

    if (!response.ok) {
        throw new Error(`Realtime subscribe failed: ${response.status}`);
    }

    return response.json();
}