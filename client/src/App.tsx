import { useEffect, useState } from "react";
import "./App.css";
import Card from "./components/Card";

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
  console.log("API:", API);
  console.log("ENV:", import.meta.env);
  const [data, setData] = useState<TelemetryReading | null>(null);
  
  useEffect(() => {
    async function fetchTelemetry() {
      try {
        const response = await fetch(`${API}/api/telemetry/esp32-air-monitor-01/latest`);

        const json = await response.json();
        console.log(json);
        setData(json);
        
      } catch (error) {
        console.error("Failed to fetch telemetry: ", error);
      }
    }
    
    fetchTelemetry();
    
    const interval = setInterval(fetchTelemetry, 2000);
    
    return () => clearInterval(interval);
  }, []);
  
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
      
      <div className="grid">
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
          title="Gas Digital"
          value={data.gasDigitalValue}
        />
        
        <Card
          title="Gas Baseline"
          value={data.gasBaseline}
        />
        
        <Card
          title="Danger Threshold"
          value={data.gasDangerThreshold}
        />
        
        <Card
          title="Danger Detected"
          value={data.gasDangerDetected ? "YES" : "NO"}
          danger={data.gasDangerDetected}
        />
        
        <Card
          title="Buzzer"
          value={data.buzzerActive ? "YES" : "NO"}
          danger={data.buzzerActive}
        />
        
        <Card
          title="Last Updated"
          value={new Date(data.timestampUtc).toLocaleString()}
        />
      </div>
    </div>
  );
}