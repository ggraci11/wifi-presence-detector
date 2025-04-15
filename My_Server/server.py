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

@app.route('/')
def home():
    return render_template("dashboard.html", entries=get_data())

@app.route('/dashboard')
def dashboard():
    return render_template("dashboard.html", entries=get_data())

@app.route('/upload', methods=['POST'])
def upload():
    data = request.get_json()

    if not data or 'device_id' not in data:
        return jsonify({"status": "error", "message": "Invalid data"}), 400

    # Add server-side timestamp
    data['server_received'] = datetime.now(timezone.utc).strftime('%Y-%m-%d %H:%M:%S UTC')

    # Append to file
    with open(DATA_FILE, 'r+') as f:
        existing = json.load(f)
        existing.append(data)
        f.seek(0)
        json.dump(existing, f, indent=2)

    print(f"Received from {data['device_id']}: {len(data['networks'])} networks")

    return jsonify({"status": "ok"}), 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
