from flask import Flask, request, jsonify, render_template
import json
from datetime import datetime, timezone
import os

app = Flask(__name__)
DATA_FILE = 'data.json'

# Initialize or create data file
if not os.path.exists(DATA_FILE):
    with open(DATA_FILE, 'w') as f:
        json.dump([], f)

def get_data():
    try:
        with open(DATA_FILE) as f:
            return json.load(f)[::-1]  # Most recent first
    except:
        return []

def get_latest_entry():
    data = get_data()
    return data[0] if data else None

@app.route('/')
def home():
    latest = get_latest_entry()
    return render_template("dashboard.html", entry=latest)

@app.route('/dashboard')
def dashboard():
    latest = get_latest_entry()
    return render_template("dashboard.html", entry=latest)

@app.route('/upload', methods=['POST'])
def upload():
    data = request.get_json()

    if not data or 'device_id' not in data:
        return jsonify({"status": "error", "message": "Invalid data"}), 400

    # Add server-side timestamp
    data['server_received'] = datetime.now(timezone.utc).strftime('%Y-%m-%d %H:%M:%S')

    # Append to file
    with open(DATA_FILE, 'r+') as f:
        try:
            existing = json.load(f)
        except json.JSONDecodeError:
            existing = []
        existing.append(data)
        f.seek(0)
        json.dump(existing, f, indent=2)

    print(f"[{data['server_received']}] Received from {data['device_id']}: {len(data['networks'])} networks")

    return jsonify({"status": "ok"}), 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
