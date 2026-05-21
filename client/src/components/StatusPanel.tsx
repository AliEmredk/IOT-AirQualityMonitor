import "./StatusPanel.css"

type Props = {
    danger: boolean;
};

export default function StatusPanel({danger }: Props) {
    return (
        <div className={`status-panel ${danger ? "danger" : "safe"}`}>
            <div className="status-light" />
            
            <div>
                <h2>
                    {danger ? "DANGER DETECTED" : "AIR QUALITY SAFE"}
                </h2>
                
                <p>
                    {danger
                    ? "Gas levels exceeded safe threshold"
                    : "Environment operating normally"}
                </p>
            </div>
        </div>
    );
}