#include <WiFiNINA.h>

// Please enter your sensitive data in the Secret tab
char ssid[] = "Airtel_HOME";                // Your network SSID (name)
char pass[] = "Mango@784";             // Your network password (use for WPA, or use as a key for WEP)
int status = WL_IDLE_STATUS;           // The Wi-Fi radio's status
WiFiClient client;

char HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME   = "/trigger/email_sender_timer/json/with/key/e1uY0KHocCfIjhsoLzos43D_vnaLhqw6BXvj8CuCoF9"; // Change your EVENT-NAME and YOUR-KEY
String queryString = "?value1=57&value2=25";

unsigned long startTime;
unsigned long timerDuration = 7200000;  // 2 hours in milliseconds
bool timerStarted = false;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Set the LED as an output
  pinMode(LED_BUILTIN, OUTPUT);

  // Attempt to connect to Wi-Fi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to network: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // Wait 10 seconds for connection:
    delay(10000);
  }

  // You're connected now, so print out the data:
  Serial.println("You're connected to the network");
  Serial.println("---------------------------------------");
}

void loop() {
  if (!timerStarted) {
    startTime = millis();
    timerStarted = true;
  }
  
  if (Serial.read() == 's') {
    if (client.connect(HOST_NAME, 80)) {
      Serial.println("Connected to server");
    
      client.println("GET " + PATH_NAME + queryString + " HTTP/1.1");
      client.println("Host: " + String(HOST_NAME));
      client.println("Connection: close");
      client.println(); // end HTTP header

      while (client.connected()) {
        if (client.available()) {
          // read an incoming byte from the server and print it to the serial monitor:
          char c = client.read();
          Serial.print(c);
        }
      }
    
      client.stop();
      Serial.println();
      Serial.println("disconnected");
      delay(10000);
      unsigned long currentTime = millis();
      unsigned long elapsedTime = currentTime - startTime;
      if (elapsedTime >= timerDuration) {
        if (client.connect(HOST_NAME, 80)) {
          // if connected:
          Serial.println("Connected to server");
      
          client.println("GET " + PATH_NAME + queryString + " HTTP/1.1");
          client.println("Host: " + String(HOST_NAME));
          client.println("Connection: close");
          client.println(); // end HTTP header

          while (client.connected()) {
            if (client.available()) {
              // read an incoming byte from the server and print it to the serial monitor:
              char c = client.read();
              Serial.print(c);
            }
          }

          timerStarted = false;
        }
        
        client.stop();
        Serial.println();
        Serial.println("disconnected");
      }
    }
  }
}
