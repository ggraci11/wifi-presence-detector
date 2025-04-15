from flask import Flask, request, jsonify, render_template
import json
from datetime import datetime
import os

app = Flask(__name__)
DATA_FILE = 'data.json'

# Initialize or create data file
if not os.path.exists(DATA_FILE):
    with open(DATA_FILE, 'w') as f:
        json.dump([], f)

@app.route('/upload', methods=['POST'])
def upload():
    data = request.get_json()

    if not data or 'device_id' not in data:
        return jsonify({"status": "error", "message": "Invalid data"}), 400

    # Add server-side timestamp
    data['server_received'] = datetime.utcnow().isoformat()

    # Append to file
    with open(DATA_FILE, 'r+') as f:
        existing = json.load(f)
        existing.append(data)
        f.seek(0)
        json.dump(existing, f, indent=2)

    print(f"Received from {data['device_id']}: {len(data['networks'])} networks")

    return jsonify({"status": "ok"}), 200

@app.route('/dashboard')
def dashboard():
    try:
        with open(DATA_FILE) as f:
            data = json.load(f)
    except:
        data = []

    return render_template("dashboard.html", entries=data[::-1])  # most recent first

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
