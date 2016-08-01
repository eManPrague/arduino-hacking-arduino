#include <Arduino.h>

#include <SPI.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

/* CONFIG */
char WIFI_SSID[]       = ""; // your wifi ssid
char WIFI_PASS[]       = ""; // your wifi pass
char CLIENT_LOGIN[]    = ""; // your MQTT client id
char CLIENT_PASS[]     = ""; // your MQTT client password
IPAddress MQTT_SERVER(192, 168, 1, 2); // IP address of the MQTT server to connect to
int MQTT_SERVER_PORT   = 1883; // MQTT port, 1883 is default in most cases
/* /CONFIG */

char CLIENT_ID[]       = "arduino";

char CONNECT_MESSAGE[]    = {0x01};
char LAST_WILL_MESSAGE[]  = {0x00};
int QOS                   = 0;
int RETAIN                = 0;

/* Arduino status topic - sends 0x01 on connect, 0x00 on disconnect */
char TOPIC_STATUS[] = "arduino/arduino-uno/status";
/* LED topics */
char TOPIC_R[]      = "arduino/arduino-uno/red";
char TOPIC_G[]      = "arduino/arduino-uno/green";
char TOPIC_B[]      = "arduino/arduino-uno/blue";
char TOPIC_Y[]      = "arduino/arduino-uno/yellow";
/* STREAM topic, used via Websocket server to accepts strings such as RGBYRGB_YRBG */
char TOPIC_STREAM[] = "arduino/arduino-uno/stream";

// PINs
const int R = 5;
const int G = 4;
const int B = 3;
const int Y = 2;
const int pins[] = {R, G, B, Y};

int status = WL_IDLE_STATUS;
char *currentTopic;

void messageReceived(char* topic, byte* payload, unsigned int length) {
  currentTopic = topic;

  // print message & topic
  Serial.print("Message: ");
  for (int i=0; i<length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.print(" from topic: ");
  Serial.println(topic);

  /*
  TOPIC_STREAM -> expects sequence of RGBY characters, all other characters switch all the PINs to LOW (visual "break")
  other topics -> expect 0x01 (HIGH) and 0x00 (LOW) values. All other characters also mean LOW.
  */
  if (isTopic(TOPIC_STREAM)) {
    // 1) remember the states of all PINs and switch them off
    int pinCount = sizeof(pins);
    int initialPinStatus[pinCount];
    for (int i = 0; i < pinCount; i++ ) {
      initialPinStatus[i] = digitalRead(pins[i]);
      digitalWrite(pins[i], LOW);
    }

    // 2) write the sequence
    for (int i=0; i<length; i++) {
      char color = (char)payload[i];
      switch (color) {
        case 'R':
          digitalWrite(R, HIGH);
          break;
        case 'G':
          digitalWrite(G, HIGH);
          break;
        case 'B':
          digitalWrite(B, HIGH);
          break;
        case 'Y':
          digitalWrite(Y, HIGH);
          break;
        default:
          for (int i = 0; i < pinCount; i++ ) {
            digitalWrite(pins[i], LOW);
          }
      }
      delay(250);
      for (int i = 0; i < pinCount; i++ ) {
        digitalWrite(pins[i], LOW);
      }
    }

    // 3) restore state of all PINs
    for (int i = 0; i < pinCount; i++ ) {
      digitalWrite(pins[i], initialPinStatus[i]);
    }
  } else {
    bool state = payload[0] == 0x01;

    if (isTopic(TOPIC_R)) {
      digitalWrite(R, state);
    } else if (isTopic(TOPIC_G)) {
      digitalWrite(G, state);
    } else if (isTopic(TOPIC_B)) {
      digitalWrite(B, state);
    } else if (isTopic(TOPIC_Y)) {
      digitalWrite(Y, state);
    }
  }
}

WiFiClient wifiClient;
PubSubClient mqttClient(MQTT_SERVER, MQTT_SERVER_PORT, messageReceived, wifiClient);

void setup()
{
  for (int i = 0; i < (int)sizeof(pins); i++ ) {
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], LOW);
  }

  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(WIFI_SSID);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(WIFI_SSID, WIFI_PASS);
  }

  // connected to wifi, print info:
  Serial.println("You're connected to the network");
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void loop()
{
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();
}

void reconnect() {
  //lLoop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect(
      CLIENT_ID,
      CLIENT_LOGIN,
      CLIENT_PASS,
      TOPIC_STATUS,
      QOS,
      RETAIN,
      LAST_WILL_MESSAGE
    )) {
      // visually show we're connected to the broker
      for (int i = 0; i < sizeof(pins); i++) {
        digitalWrite(pins[i], HIGH);
        delay(250);
        digitalWrite(pins[i], LOW);
      }

      Serial.println("connected");
      // once connected, publish an announcement...
      mqttClient.publish(TOPIC_STATUS, CONNECT_MESSAGE, RETAIN);
      // ...and subscribe to all topics

      if (
        mqttClient.subscribe(TOPIC_R, QOS)
        && mqttClient.subscribe(TOPIC_G, QOS)
        && mqttClient.subscribe(TOPIC_B, QOS)
        && mqttClient.subscribe(TOPIC_Y, QOS)
        && mqttClient.subscribe(TOPIC_STREAM, QOS)
      ) {
        Serial.println("Subbed successfully");
      } else {
        Serial.println("Sub failed");
      }
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 1 second");
      // Wait 1 second before retrying
      delay(1000);
    }
  }
}

// helper function for identifying the topic
bool isTopic(char* topicName) {
  return strcmp(currentTopic, topicName) == 0;
}
