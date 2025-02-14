#include <WiFi.h>
#include <PubSubClient.h>

const char* mqtt_server = "broker.hivemq.com";
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
    WiFi.begin("yourSSID", "yourPASSWORD");
    client.setServer(mqtt_server, 1883);
}

void loop() {
    if (!client.connected()) {
        client.connect("ESP32Client");
    }
    char tempStr[10];
    dtostrf(temperature, 4, 2, tempStr);
    client.publish("machine/temperature", tempStr);
    delay(5000);
}
