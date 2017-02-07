#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// wifi variables
#define wifi_ssid "here your ssid"
#define wifi_password "here your wpa key"

// mqtt variables
#define mqtt_server "here your mqtt server"
#define mqtt_port 10395
#define mqtt_user "here your mqtt user"
#define mqtt_password "here your mqtt password"

//local varables
int counter = 0;
int previousReading = LOW;

WiFiClient espClient;
PubSubClient client(espClient);

void setup(){
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server,mqtt_port);
  pinMode(4, INPUT); // on nodemcu this is D2
  }

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect(){
    // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (client.connect("ESP8266Client")) {
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  int reading = digitalRead(4);
  if (previousReading == LOW && reading == HIGH) {
    counter++;
    client.publish("sensor/frontdoor", "ring");  
    Serial.print("Triggered ");
    Serial.print(counter);
    Serial.print("x Times! ");
    delay(10000);
    client.publish("sensor/frontdoor", "noring"); 
  }
  previousReading = reading;
}

