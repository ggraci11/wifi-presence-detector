from flask import Flask, request, jsonify, render_template
import json
from datetime import datetime, timezone
import os

app = Flask(__name__)
DATA_FILE = 'data.json'

def initialize_data_file():
    """Ensure the data file exists."""
    if not os.path.exists(DATA_FILE):
        with open(DATA_FILE, 'w') as f:
            json.dump([], f)

def load_data():
    """Load entries from the data file."""
    try:
        with open(DATA_FILE, 'r') as f:
            return json.load(f)[::-1]  # Show newest first
    except (json.JSONDecodeError, FileNotFoundError):
        return []

def save_data(new_entry):
    """Save a new entry to the data file."""
    try:
        with open(DATA_FILE, 'r+') as f:
            try:
                data = json.load(f)
            except json.JSONDecodeError:
                data = []
            data.append(new_entry)
            f.seek(0)
            f.truncate()
            json.dump(data, f, indent=2)
    except Exception as e:
        print(f"Error saving data: {e}")

@app.route('/')
@app.route('/dashboard')
def dashboard():
    entries = load_data()
    return render_template('dashboard.html', entries=entries)

@app.route('/upload', methods=['POST'])
def upload():
    data = request.get_json()

    if not data or 'device_id' not in data:
        return jsonify({"status": "error", "message": "Invalid or missing device_id"}), 400

    # Add server-side timestamp
    data['server_received'] = datetime.now(timezone.utc).strftime('%Y-%m-%d %H:%M:%S')

    # Save to file
    save_data(data)

    print(f"[{data['server_received']}] Received from {data['device_id']}: {len(data.get('networks', []))} networks")

    return jsonify({"status": "ok"}), 200

if __name__ == '__main__':
    initialize_data_file()
    port = int(os.environ.get("PORT", 5000))
    app.run(host='0.0.0.0', port=port)
