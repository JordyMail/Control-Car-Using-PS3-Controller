import paho.mqtt.client as mqtt
import hid  # To read input from PS3 Controller
import time

BROKER = "broker.emqx.io"
TOPIC = "IoT/group8"

# Callback while disconnected
def on_disconnect(client, userdata, rc):
    print("Disconnected, trying to reconnect...")
    while True:
        try:
            client.reconnect()
            break
        except:
            time.sleep(5)

# Initiate MQTT Client
client = mqtt.Client()
client.on_disconnect = on_disconnect
client.connect(BROKER, 1883, 60)
client.loop_start()  # start loop MQTT

# Initiate PS3 Controller
PS3_VENDOR_ID = 0x1949
PS3_PRODUCT_ID = 0x402

try:
    gamepad = hid.device()
    gamepad.open(PS3_VENDOR_ID, PS3_PRODUCT_ID)
    gamepad.set_nonblocking(True)
    print("PS3 Controller Connected!")

    while True:
        data = gamepad.read(64)
        if data:
            lx = data[3]  # Joystick left (X-axis)
            ly = data[4]  # Joystick left (Y-axis)

            if ly < 100:  
                print("Forward")
                client.publish(TOPIC, "forward")
            elif ly > 150:
                print("Backward")
                client.publish(TOPIC, "backward")
            elif lx < 100:
                print("Left")
                client.publish(TOPIC, "left")
            elif lx > 150:
                print("Right")
                client.publish(TOPIC, "right")
            else:
                print("Stop");
                client.publish(TOPIC, "stop")
        time.sleep(0.2)  
except Exception as e:
    print("Error:", e)

finally:
 gamepad.close()
 client.loop_stop()
 client.disconnect()