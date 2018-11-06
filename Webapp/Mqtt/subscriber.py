import sqlite3
import paho.mqtt.client as mqtt

_host           = "test.mosquitto.org"
_port           = 1883
_keepalive      = 60
_bind_address   = ""
_topic          = "tituszdipterv/test"


def insertIntoDatabase(_deviceId0, _deviceId1, _deviceId2, _packetId, _temperatureId, _temperature, _humidityId, _humidity):
    conn = sqlite3.connect('sensor.db')
    c = conn.cursor()
    with conn:
        c.execute("""INSERT INTO sensors (deviceId0, deviceId1, deviceId2, packetId, temperatureId, temperature, humidityId, humidity)
            VALUES (:deviceId0, :deviceId1, :deviceId2, :packetId, :temperatureId, :temperature, :humidityId, :humidity)""", 
            {   'deviceId0'     : _deviceId0,
                'deviceId1'     : _deviceId1,
                'deviceId2'     : _deviceId2,
                'packetId'      : _packetId,
                'temperatureId' : _temperatureId,
                'temperature'   : _temperature,
                'humidityId'    : _humidityId,
                'humidity'      : _humidity
            })

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() - if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe(topic=_topic)

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))

    splitted = str(msg.payload).split(",")
    print(splitted)

    insertIntoDatabase(splitted[1], splitted[2], splitted[3], splitted[4], splitted[5], splitted[6], splitted[7], splitted[8])
    print("Data inserted.")

# Create an MQTT client and attach our routins to it.
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect(host=_host, port=_port, keepalive=_keepalive, bind_address=_bind_address)

client.loop_forever()
