#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Adhan";  // your own WiFi
const char* password = "wahyuramadhan";
const char* mqtt_server = "broker.emqx.io";  

WiFiClient espClient;
PubSubClient client(espClient);

// Light pin
#define Green 15
#define Red 13

// Pin Motor
#define ENA 16  // Enable Motor A
#define ENB 14  // Enable Motor B
#define MOTOR_A1 5
#define MOTOR_A2 4
#define MOTOR_B1 0
#define MOTOR_B2 2

//default speed (0-1023)
int speedValue = 255;
