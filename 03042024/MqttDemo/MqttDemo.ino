#include <WiFi.h>
#include <PubSubClient.h>

const char *ssid = "";
const char *password = "";
const char *mqtt_username = "";
const char *mqtt_password = "";
const char *mqtt_server = "";

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [" + String(topic) + "]: ");
  String message = String(payload, length);
  Serial.println(message);
}

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  client.connect("ESP32", mqtt_username, mqtt_password);
  client.subscribe("esp32/response");
}

byte i = 0;

void loop()
{
  String message = String(i);
  Serial.println("publishing");
  client.publish("esp32/test", message.c_str());
  delay(1000);
  i += 1;
  if (i > 10)
    i = 0;
  client.loop();
}