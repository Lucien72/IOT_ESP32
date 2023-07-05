from flask import Flask, request, jsonify

app = Flask(__name__)

# Route pour recevoir les données de l'ESP32 en utilisant une requête PUT
@app.route('/api/Esp32/<string:esp32Name>', methods=['PUT'])
def receive_data(esp32Name):
    data = request.get_json()

    config = data.get('config', {})
    tempFreq = config.get('tempFreq', 5)
    connectionConfig = config.get('connectionConfig', 2)
    connectionFreq = config.get('connectionFreq', 30)

    temperatures = data.get('temperatures', [])

    print(f"Nom de l'ESP32: {esp32Name}")
    print(f"TempFreq: {tempFreq}")
    print(f"ConnectionConfig: {connectionConfig}")
    print(f"ConnectionFreq: {connectionFreq}")
    print("Températures:", temperatures)

    # Traitez les données comme vous le souhaitez ici, par exemple, enregistrez-les dans une base de données

    return jsonify({"message": "Données reçues avec succès"})

if __name__ == '__main__':
    app.run(host='172.20.10.9', port=8080)
