import sqlite3
from Database.sensordata import *

sensorId1 = ["0x42003c", "0x5246500e", "0x20383352"]
sensorId2 = ["0x4a002e", "0x5246500e", "0x20383352"]

### Function definitions ###
def createTable():
    conn = sqlite3.connect('sensor.db')
    c = conn.cursor()
    with conn:
        c.execute("""CREATE TABLE IF NOT EXISTS sensors
        (
            ID              integer PRIMARY KEY AUTOINCREMENT,
            deviceId0       integer,
            deviceId1       integer,
            deviceId2       integer,
            packetId        integer,
            temperatureId   integer,
            temperature     real,
            humidityId      integer,
            humidity        real
        )""")

# def insertIntoDatabase(data):
#     conn = sqlite3.connect('sensor.db')
#     c = conn.cursor()
#     with conn:
#         c.execute("""INSERT INTO sensors (deviceId0, deviceId1, deviceId2, packetId, temperatureId, temperature, humidityId, humidity)
#             VALUES (:deviceId0, :deviceId1, :deviceId2, :packetId, :temperatureId, :temperature, :humidityId, :humidity)""", 
#             {   'deviceId0'     : data.deviceId0,
#                 'deviceId1'     : data.deviceId1,
#                 'deviceId2'     : data.deviceId2,
#                 'packetId'      : data.packetId,
#                 'temperatureId' : data.temperatureId,
#                 'temperature'   : data.temperature,
#                 'humidityId'    : data.humidityId,
#                 'humidity'      : data.humidity
#             })

def getAllFromDatabase():
    conn = sqlite3.connect('sensor.db')
    c = conn.cursor()
    with conn:
        c.execute("SELECT * FROM sensors")
    return c.fetchall()

def getTemperatureFromDatabase():
    conn = sqlite3.connect('sensor.db')
    c = conn.cursor()
    with conn:
        c.execute("SELECT ID, temperature  FROM sensors")
        return c.fetchall()

def getHumidityFromDatabase():
    conn = sqlite3.connect('sensor.db')
    c = conn.cursor()
    with conn:
        c.execute("SELECT ID, humidity FROM sensors")
        return c.fetchall()

def getDataByDeviceId(devId0, devId1, devId2):
    conn = sqlite3.connect('sensor.db')
    c = conn.cursor()
    with conn:
        c.execute("SELECT * FROM sensors WHERE deviceId0=:deviceId0 AND deviceId1=:deviceId1 AND deviceId2=:deviceId2",{'deviceId0': devId0,'deviceId1': devId1,'deviceId2': devId2})
        return c.fetchall()

def droptable():
    conn = sqlite3.connect('sensor.db')
    c = conn.cursor()
    with conn:
        c.execute("""DROP TABLE sensors""")

# droptable()
# createTable()