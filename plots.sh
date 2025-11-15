#!/bin/bash

CSV_PATH="build/filtered.csv"
PY_SCRIPT="scripts/plots.py"

if [ ! -f "$CSV_PATH" ]; then
    echo "No $CSV_PATH found. Run the demo to generate results before plotting."
    exit 0
fi

if [ ! -d ".venv" ]; then
    echo "Python venv not found, creating it."
    python3 -m venv .venv
fi

source .venv/bin/activate

pip install --quiet pandas matplotlib

python "$PY_SCRIPT"

deactivate