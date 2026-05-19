import { useEffect, useState } from "react";
import "./App.css";
import Card from "./components/Card";
import StatusPanel from "./components/StatusPanel";

import {
  LineChart,
  Line,
  XAxis,
  YAxis,
  Tooltip,
  ResponsiveContainer,
  CartesianGrid, ReferenceLine
} from "recharts";

type TelemetryReading = {
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

export default function App() {
  const API = import.meta.env.VITE_API_URL;
  const [data, setData] = useState<TelemetryReading | null>(null);
  const [history, setHistory] = useState<TelemetryReading[]>([]);
  const [range, setRange] = useState<"hour" | "day">("hour");
  const DEVICE_ID = "esp32-air-monitor-01";
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    async function fetchTelemetry() {
      try {
        const response = await fetch(
            `${API}/api/telemetry/${DEVICE_ID}/latest`
        );

        if (!response.ok) {
          throw new Error(`Latest request failed: ${response.status}`);
        }

        const json = await response.json();
        console.log("Latest:", json);
        setData(json);

      } catch (error) {
        console.error("Failed to fetch telemetry: ", error);
        setError("Could not connect to backend");
      }
    }

    fetchTelemetry();

    const interval = setInterval(fetchTelemetry, 2000);

    return () => clearInterval(interval);
  }, [API]);

  useEffect(() => {
    async function fetchHistory() {
      try {
        const response = await fetch(
            `${API}/api/telemetry/${DEVICE_ID}/graph?range=${range}`
        );

        const json = await response.json();
        console.log("History:", json);
        setHistory(json);

      } catch (error) {
        console.error("Failed to fetch history: ", error);
      }
    }

    fetchHistory();

    const interval = setInterval(fetchHistory, 10000);

    return () => clearInterval(interval);
  }, [API, range]);

  if (error) {
    return (
        <div className="loading-container">
          <h1>{error}</h1>
          <p>Check that backend is running and VITE_API_URL is correct.</p>
        </div>
    );
  }
  
  if(!data) {
    return (
        <div className="loading-container">
          <h1>Loading telemetry...</h1>
        </div>
    );
  }
  
  return (
    <div className="page">
      <h1 className="title">Gas Detection Dashboard</h1>
      
      <StatusPanel danger={data.gasDangerDetected} />
      
      <div className="stats-grid">
        <Card
          title="Temperature"
          value={`${data.temperatureC.toFixed(1)} °C`}
        />
        
        <Card
        title="Humidity"
        value={`${data.humidityPercent.toFixed(1)} %`}
        />

        <Card
            title="Pressure"
            value={`${data.pressureHpa.toFixed(1)} hPa`}
        />
        
        <Card
          title="Gas Analog"
          value={data.gasAnalogValue}
        />
        
        <Card
          title="Danger Detected"
          value={data.gasDangerDetected ? "YES" : "NO"}
          danger={data.gasDangerDetected}
        />
        
        <Card
          title="Last Updated"
          value={new Date(data.timestampUtc).toLocaleString("en-GB", {
              hour: "2-digit",
              minute: "2-digit",
              second: "2-digit",
          })}
        />
      </div>

      <div className="chart-section">
        <div className="chart-header">
          <h2>Gas Analog History</h2>

          <select
              value={range}
              onChange={(e) => setRange(e.target.value as "hour" | "day")}
          >
            <option value="hour">Last hour</option>
            <option value="day">Last 24 hours</option>
          </select>
        </div>

        <ResponsiveContainer width="100%" height={300}>
          <LineChart data={history}>
            <CartesianGrid strokeDasharray="3 3" />
            <XAxis
                dataKey="timestampUtc"
                tickFormatter={(value) =>
                    new Date(value).toLocaleTimeString("en-GB", {
                      hour: "2-digit",
                      minute: "2-digit",
                    })
                }
                minTickGap={80}
                interval="preserveStartEnd"
            />
            <YAxis />
            <Tooltip
                labelFormatter={(value) => new Date(value).toLocaleString()}
                contentStyle={{
                  backgroundColor: "#1e293b",
                  border: "1px solid #334155",
                  borderRadius: "12px",
                  color: "white"
                }}
            />
            <Line
                type="monotone"
                dataKey="gasAnalogValue"
                stroke="#22c55e"
                strokeWidth={3}
                dot={false}
            />
            <ReferenceLine 
              y={data.gasDangerThreshold}
              stroke="#ef4444"
              strokeDasharray="5 5"
            />
          </LineChart>
        </ResponsiveContainer>
      </div>
    </div>
  );
}