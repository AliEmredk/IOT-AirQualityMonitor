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