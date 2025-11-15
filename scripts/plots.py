import os
import pandas as pd
import matplotlib.pyplot as plt

repo_root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
csv_path = os.path.join(repo_root, "build", "filtered.csv")
plot_path = os.path.join(repo_root, "build", "kalman_results.png")

if not os.path.exists(csv_path):
    print(f"ERROR: {csv_path} not found, skipping plot.")
    exit(0)

df = pd.read_csv(csv_path)

plt.figure(figsize=(8, 5))
plt.plot(df['time'], df['filtered'], label='Kalman Filtered', color='blue')
plt.plot(df['time'], df['measured'], label='Measured (Noisy)', color='orange', alpha=0.5)
if 'velocity' in df.columns:
    plt.plot(df['time'], df['velocity'], label='Velocity', color='green', linestyle='--')
plt.xlabel("Time")
plt.ylabel("Value")
plt.title("Kalman Filter: Estimation vs. Measured Data")
plt.legend()
plt.tight_layout()
plt.savefig(plot_path, dpi=150)
print(f"Plot saved as {plot_path}.")
