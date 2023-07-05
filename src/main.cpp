#include <Arduino.h>
#include <WiFi.h>
#include <EEPROM.h>
#include <HTTPClient.h>


// Constantes pour l'index de stockage en EEPROM
#define ADDR_TEMP_FREQ 0
#define ADDR_CONNECTION_CONFIG 1
#define ADDR_CONNECTION_FREQ 2

#define M1_CALPOINT1_CELSIUS 23.0f
#define M1_CALPOINT1_RAW 128253742.0f
#define M1_CALPOINT2_CELSIUS -20.0f
#define M1_CALPOINT2_RAW 114261758.0f


// Constantes pour les valeurs de configuration
#define TEMP_FREQ_5_SECONDS 5
#define TEMP_FREQ_10_SECONDS 10
#define CONNECTION_FREQ_30_SECONDS 30
#define CONNECTION_FREQ_1_MINUTE 60
#define CONNECTION_FREQ_5_MINUTES 300
#define CONNECTION_CONFIG_HTTP 2
#define CONNECTION_CONFIG_MQTT 3

const char* ssid = "test";
const char* password = "Azerty121";
const char* serverHost = "192.168.87.113:3000";
const char* esp32Name = "esp32";

int tempFreq = 5;
int connectionConfig = 2;
int connectionFreq = 30;

// void readConfigFromEEPROM() {
//   tempFreq = EEPROM.read(ADDR_TEMP_FREQ);
//   connectionConfig = EEPROM.read(ADDR_CONNECTION_CONFIG);
//   connectionFreq = EEPROM.read(ADDR_CONNECTION_FREQ);
// }

void setup() {
  Serial.begin(9600);

  // Initialisation de la mémoire EEPROM
  // EEPROM.begin(512);

  // Lecture des données de configuration sauvegardées en EEPROM
  // readConfigFromEEPROM();

  // Connexion WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  
  // float celsius = temperatureRead();

  // Serial.print(F("Temperature: "));
  // Serial.print(celsius);
  // Serial.println(F("°C"));

  // Mise en veille de l'ESP32 pour économiser les ressources
  // esp_sleep_enable_timer_wakeup(connectionFreq * 1000000); // Conversion en microsecondes
  // Serial.println("Going to sleep...");
  // esp_deep_sleep_start();
}

// void saveConfigToEEPROM() {
//   EEPROM.write(ADDR_TEMP_FREQ, tempFreq);
//   EEPROM.write(ADDR_CONNECTION_CONFIG, connectionConfig);
//   EEPROM.write(ADDR_CONNECTION_FREQ, connectionFreq);
//   EEPROM.commit();
// }

void sendDataToServer() {
  // Créer une instance de client HTTP
  WiFiClient client;
  HTTPClient http;
  
  float celsius = temperatureRead();

  Serial.print(F("Temperature: "));
  Serial.print(celsius);
  Serial.println(F("°C"));

  // Construire l'URL
  String url = "http://" + String(serverHost) + "/api/Esp32/" + String(esp32Name);

  // Construire le JSON à envoyer
  String jsonString = "{\"config\":{\"tempFreq\":" + String(tempFreq) + ",\"connectionConfig\":" + String(connectionConfig) + ",\"connectionFreq\":" + String(connectionFreq) + "},\"temperatures\":" + String(celsius) +"}";
  // Envoyer la requête PUT
  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.PUT(jsonString);

  // Vérifier la réponse
  if (httpResponseCode > 0) {
    Serial.printf("HTTP PUT request successful, response code: %d\n", httpResponseCode);
  } else {
    Serial.printf("HTTP PUT request failed, error: %s\n", http.errorToString(httpResponseCode).c_str());
  }

  // Libérer les ressources
  http.end();
}

void loop() {
  sendDataToServer();
  delay(5000);
}