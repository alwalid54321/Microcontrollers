#include <ESP8266WiFi.h>
#include <DHT.h>
#include <ThingSpeak.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

// Wi-Fi settings
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// ThingSpeak settings
unsigned long myChannelNumber = YOUR_CHANNEL_NUMBER;
const char* myWriteAPIKey = "YOUR_WRITE_API_KEY";

// DHT sensor settings
#define DHTPIN D4        // Pin where the DHT sensor is connected
#define DHTTYPE DHT22    // DHT11 or DHT22
DHT dht(DHTPIN, DHTTYPE);

// Initialize Wi-Fi, ThingSpeak, and Web Server
WiFiClient client;
ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);
  
  // Initialize DHT sensor
  dht.begin();
  
  // Attempt to connect to Wi-Fi
  connectToWiFi();
  
  // Initialize ThingSpeak
  ThingSpeak.begin(client);
  
  // Setup web server routes
  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.begin();
  
  Serial.println("HTTP server started");
}

void loop() {
  // Serve any client requests
  server.handleClient();

  // Read temperature and humidity from DHT sensor
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check if any reads failed
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print the data to the Serial Monitor
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%  Temperature: ");
  Serial.print(temperature);
  Serial.println("°C");

  // Send data to ThingSpeak
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);

  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Channel update successful.");
  } else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  // Put the ESP8266 into deep sleep mode for 10 minutes (600e6 microseconds)
  Serial.println("Going to sleep...");
  ESP.deepSleep(600e6);
}

void connectToWiFi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  int retries = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    
    if (retries++ > 20) {
      Serial.println("Failed to connect to Wi-Fi. Restarting...");
      ESP.restart();
    }
  }
  
  Serial.println();
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
}

void handleRoot() {
  server.send(200, "text/plain", "Welcome to the ESP8266 Temperature and Humidity Monitor!");
}

void handleData() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check if any reads failed
  if (isnan(humidity) || isnan(temperature)) {
    server.send(500, "application/json", "{\"error\":\"Failed to read from sensor\"}");
    return;
  }

  // Create a JSON response
  StaticJsonDocument<200> jsonDoc;
  jsonDoc["temperature"] = temperature;
  jsonDoc["humidity"] = humidity;

  String response;
  serializeJson(jsonDoc, response);
  
  server.send(200, "application/json", response);
}
