import "./Card.css";

type CardProps = {
    title: string;
    value: string | number;
    danger?: boolean;
};

export default function Card({
    title,
    value,
    danger = false,
    }: CardProps) {
    return (
        <div className={`card ${danger ? "danger" : ""}`}>
            <h3 className="card-title">{title}</h3>

            <p className={`card-value ${danger ? "danger-text" : ""}`}>
                {value}
            </p>
        </div>
    );
}