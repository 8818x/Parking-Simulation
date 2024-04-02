#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* publish_topic = "@msg/led";

const char* ssid = "your_ssid";                                           // Wi-Fi SSID
const char* password = "your_password";                                 // Wi-Fi password
const char* mqtt_server = "your_broker";                      // MQTT broker server address
const int mqtt_port = "your_mqtt_port";                                        // MQTT broker port
const char* mqtt_Client = "your_mqtt_client";  // MQTT client ID
const char* mqtt_username = "your_mqtt_usernameT";    // MQTT username
const char* mqtt_password = "your_mqtt_password";    // MQTT password

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
int value = 0;
char msg[50];
char msg_fb[100];

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection…");
    if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) {  // Attempt to connect to MQTT broker
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      delay(5000);  // Wait for 5 seconds before retrying
    }
  }
}

void setup() {
  Serial.begin(115200);  // Initialize serial communication
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);              // Connect to Wi-Fi network
  while (WiFi.status() != WL_CONNECTED) {  // Wait for Wi-Fi connection
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());            // Print local IP address
  client.setServer(mqtt_server, mqtt_port);  // Set MQTT server and port
}

void loop() {
  if (!client.connected()) {
    reconnect();  // Reconnect to MQTT broker if connection is lost
  }
  client.loop();  // Maintain MQTT connection

  long now = millis();
  if (now - lastMsg > 5000) {  // Send data every 5 seconds
    lastMsg = now;

    client.publish(publish_topic, msg);  // Publish data to MQTT topic

    // Read values from Serial monitor
    String data = Serial.readStringUntil('\n');

    // Construct the payload
    String payload = "{\"data\":" + data + "}";
    Serial.println(payload);

    // Convert payload to char array and publish it
    payload.toCharArray(msg_fb, (payload.length() + 1));
    client.publish("@shadow/data/update", msg_fb);  // Publish payload to a specific MQTT topic
  }

  delay(10);  // Delay
}
