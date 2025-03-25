#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "yourWifiName";  // your own WiFi
const char* password = "yourWIfiPassword";
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

void setup() {
    Serial.begin(115200);
    
    // Inisialisasi WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi");

    // MQTT
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    
    reconnect();

    // Setup pin Light
    pinMode(Green, OUTPUT);
    pinMode(Red, OUTPUT);

    // Setup pin motor
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(MOTOR_A1, OUTPUT);
    pinMode(MOTOR_A2, OUTPUT);
    pinMode(MOTOR_B1, OUTPUT);
    pinMode(MOTOR_B2, OUTPUT);

  //set intiial speed
 analogWrite(ENA, speedValue);
 analogWrite(ENB, speedValue);
}
void moveForward() {
    analogWrite(ENA, speedValue);
    analogWrite(ENB, speedValue);
    digitalWrite(MOTOR_A1, HIGH);
    digitalWrite(MOTOR_A2, LOW);
    digitalWrite(MOTOR_B1, HIGH);
    digitalWrite(MOTOR_B2, LOW);
}
void moveBackward() {
    analogWrite(ENA, speedValue);
    analogWrite(ENB, speedValue);
    digitalWrite(MOTOR_A1, LOW);
    digitalWrite(MOTOR_A2, HIGH);
    digitalWrite(MOTOR_B1, LOW);
    digitalWrite(MOTOR_B2, HIGH);
}
void turnLeft() {
 analogWrite(ENA, speedValue);
 analogWrite(ENB, speedValue);
 digitalWrite(MOTOR_A1, LOW);
 digitalWrite(MOTOR_A2, HIGH);
 digitalWrite(MOTOR_B1, HIGH);
 digitalWrite(MOTOR_B2, LOW);
}

void turnRight() {
    analogWrite(ENA, speedValue);
    analogWrite(ENB, speedValue);
    digitalWrite(MOTOR_A1, HIGH);
    digitalWrite(MOTOR_A2, LOW);
    digitalWrite(MOTOR_B1, LOW);
    digitalWrite(MOTOR_B2, HIGH);
}

void stopCar() {
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
    digitalWrite(MOTOR_A1, LOW);
    digitalWrite(MOTOR_A2, LOW);
    digitalWrite(MOTOR_B1, LOW);
    digitalWrite(MOTOR_B2, LOW);
}

// Callback when MQTT message is received
void callback(char* topic, byte* payload, unsigned int length) {
    String message = "";
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    Serial.println("message received: " + message);

    if (message == "forward") {
        moveForward();
        digitalWrite(Green, HIGH);
        digitalWrite(Red, LOW);
    } else if (message == "backward") {
        moveBackward();
        digitalWrite(Green, HIGH);
        digitalWrite(Red, LOW);
    } else if (message == "left") {
        turnLeft();
        digitalWrite(Green, HIGH);
        digitalWrite(Red, LOW);
    } else if (message == "right") {
        turnRight();
        digitalWrite(Green, HIGH);
        digitalWrite(Red, LOW);
    } else if (message == "stop") {
        stopCar();
        digitalWrite(Red, HIGH);
        digitalWrite(Green, LOW);
    } else if (message.startsWith("speed:")) {
        speedValue = message.substring(6).toInt();
        analogWrite(ENA, speedValue);
        analogWrite(ENB, speedValue);
        Serial.println("Speed ​​is changed to: " + String(speedValue));
    }
}

// Function to ensure the connection to MQTT remains active
void reconnect() {
    while (!client.connected()) {
        Serial.print("Connecting to MQTT...");
        if (client.connect("mqttx_edd67096")) {
            Serial.println("Connected");
            client.subscribe("IoT/example");
        } else {
            Serial.print("Failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds....");
            delay(5000);
        }
    }
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
}
