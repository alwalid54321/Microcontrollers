#include <ESP8266WiFi.h>

// Replace with your network credentials
const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

// Create a WiFiServer object
WiFiServer server(80);

void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);

  // Initialize the LED pin
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);  // Turn off LED (ESP-12E/ESP-07 built-in LED is active LOW)

  // Connect to Wi-Fi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Print the IP address
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();
  Serial.println("Server started");
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("New Client.");
  String currentLine = "";
  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      Serial.write(c);
      if (c == '\n') {
        // If the current line is blank, you got two newline characters in a row.
        // That's the end of the client HTTP request, so send a response:
        if (currentLine.length() == 0) {
          // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();

          // The actual HTML content
          client.println("<html><body><h1>ESP8266 LED Control</h1>");
          client.println("<p><a href=\"/LED=ON\"><button>Turn ON</button></a></p>");
          client.println("<p><a href=\"/LED=OFF\"><button>Turn OFF</button></a></p>");
          client.println("</body></html>");

          // The HTTP response ends with another blank line
          client.println();
          break;
        } else {  // If you got a newline, then clear currentLine
          currentLine = "";
        }
      } else if (c != '\r') {  // If you got anything else but a carriage return character,
        currentLine += c;      // add it to the end of the currentLine
      }

      // Check if the client request was to turn the LED on or off
      if (currentLine.endsWith("GET /LED=ON")) {
        digitalWrite(LED_BUILTIN, LOW);  // Turn the LED on (LOW level is the voltage level for the active LOW LED)
      }
      if (currentLine.endsWith("GET /LED=OFF")) {
        digitalWrite(LED_BUILTIN, HIGH); // Turn the LED off (HIGH level is the voltage level for the active LOW LED)
      }
    }
  }

  // Close the connection
  client.stop();
  Serial.println("Client Disconnected.");
}
