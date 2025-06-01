# **PS3 Controlled IoT Car Using ESP8266 and MQTT**

<img src="https://github.com/user-attachments/assets/b21a6400-ddb0-4c83-8a0e-cda4db1e1db6" width="450" height="auto">

## **Project Overview**
This project enables a user to control a robot car using a PlayStation 3 (PS3) controller via an MQTT protocol. The system consists of a PS3 controller, a computer running a Python script to read joystick inputs and send MQTT messages, and an ESP8266-based car that receives and processes the MQTT messages to move accordingly.

## **Required Components**
1. **ESP8266 NodeMCU** (to control the motors and communicate via MQTT)
2. **L298N Motor Driver Module** (to control the DC motors)
3. **Two DC Motors** (for movement)
4. **Power Supply** (Battery pack for the motors and ESP8266)
5. **PlayStation 3 Controller** (to control the car)
6. **Computer** (to run the Python script for joystick input and MQTT communication)
7. **Jumper Wires** (to connect all components)

## **Building the Hardware**
### **Connections**
| ESP8266 Pin | Motor Driver Pin | Description |
|------------|----------------|-------------|
| D0 (16) | ENA | Motor A Speed Control |
| D5 (14) | ENB | Motor B Speed Control |
| D1 (5) | MOTOR_A1 | Motor A Direction |
| D2 (4) | MOTOR_A2 | Motor A Direction |
| D3 (0) | MOTOR_B1 | Motor B Direction |
| D4 (2) | MOTOR_B2 | Motor B Direction |
| D7 (13) | Red LED | Indicating Stop |
| D8 (15) | Green LED | Indicating Movement |

- **Connect ESP8266 to L298N motor driver** using the above table.
- **Connect the PS3 controller to the computer via USB or Bluetooth.**
- **Power the ESP8266 using a battery pack (5V recommended).**

## **Image**
<img src="https://github.com/user-attachments/assets/9e79ed9e-31da-48c2-b145-5565c09e69a3" width="600" height="auto">


## **Software Overview**
This project consists of two main parts:
1. **Python Code** (on the computer) to read PS3 joystick input and publish MQTT messages.
2. **C++ Code (Arduino)** (on the ESP8266) to receive and process MQTT messages to control the motors.

### **Python Code (PS3 Controller to MQTT)**
#### **1. Import Required Libraries**
```python
import paho.mqtt.client as mqtt
import hid  # To read input from PS3 Controller
import time
```
- `paho.mqtt.client`: Handles MQTT communication.
- `hid`: Reads input from the PS3 controller.
- `time`: Provides delay functionality.

#### **2. Define MQTT Broker and Topic**
```python
BROKER = "broker.emqx.io"
TOPIC = "IoT/example"
```
- `BROKER`: The MQTT broker address.
- `TOPIC`: The topic to which movement commands are published.

#### **3. Define MQTT Disconnection Handling**
```python
def on_disconnect(client, userdata, rc):
    print("Disconnected, trying to reconnect...")
    while True:
        try:
            client.reconnect()
            break
        except:
            time.sleep(5)
```
- This function ensures that if the MQTT connection is lost, the client will continuously attempt to reconnect every 5 seconds.

#### **4. Initialize MQTT Client**
```python
client = mqtt.Client()
client.on_disconnect = on_disconnect
client.connect(BROKER, 1883, 60)
client.loop_start()  # start loop MQTT
```
- Creates an MQTT client.
- Assigns the `on_disconnect` function.
- Connects to the broker on port `1883` with a keep-alive of `60` seconds.
- Starts the MQTT loop to keep the connection alive.

### **C++ Code (ESP8266 as MQTT Subscriber)**
Below is a detailed explanation of each part of the C++ code running on the ESP8266:

#### **1. Import Required Libraries**
```cpp
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
```
- `ESP8266WiFi.h`: Connects ESP8266 to the WiFi network.
- `PubSubClient.h`: Enables MQTT communication with the broker.

#### **2. Define WiFi and MQTT Parameters**
```cpp
const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";
const char* mqtt_server = "broker.emqx.io";
```
- `ssid`: WiFi network name.
- `password`: WiFi password.
- `mqtt_server`: MQTT broker address.

#### **3. Define Motor and LED Pins**
```cpp
#define MOTOR_A1 D1
#define MOTOR_A2 D2
#define MOTOR_B1 D3
#define MOTOR_B2 D4
#define ENA D0
#define ENB D5
#define LED_STOP D7
#define LED_MOVE D8
```
- Defines the pins used to control motors and indicator LEDs.

#### **4. Setup WiFi and MQTT Connection**
```cpp
WiFiClient espClient;
PubSubClient client(espClient);
```
- Creates WiFi and MQTT client objects.

#### **5. Function to Connect to WiFi**
```cpp
void setup_wifi() {
    delay(10);
    Serial.println("Connecting to WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected");
}
```
- Initiates connection to the WiFi network and waits until it is successfully connected.

#### **6. MQTT Callback Function for Receiving Messages**
```cpp
void callback(char* topic, byte* payload, unsigned int length) {
    String message = "";
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    if (message == "forward") { moveForward(); }
    else if (message == "backward") { moveBackward(); }
    else if (message == "left") { turnLeft(); }
    else if (message == "right") { turnRight(); }
    else { stopMotor(); }
}
```
- Receives commands from MQTT and executes the corresponding movement function.

#### **7. Setup Function**
```cpp
void setup() {
    Serial.begin(115200);
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}
```
- Initializes serial communication, WiFi connection, and MQTT setup.

#### **8. Loop to Maintain MQTT Connection**
```cpp
void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
}
```
- Ensures ESP8266 remains connected to the MQTT broker and processes incoming messages.

## **Future Improvements**
- Implement WebSocket control.
- Add voice command support.
- Add a camera module for remote vision.


This is my university project that me and my friend build to complite the assigment! </br>
note: feel free to contribute👌

