import sqlite3
from rpi_sensordata import *


#---FUNCTION DEFINITIONS---
def createTable():
    conn = sqlite3.connect('rpi_sensor.db')
    c = conn.cursor()
    with conn:
        c.execute("""CREATE TABLE IF NOT EXISTS sensors
        (
            ID              integer PRIMARY KEY AUTOINCREMENT,
            timestamp       text,
            deviceId0       integer,
            deviceId1       integer,
            deviceId2       integer,
            packetId        integer,
            temperatureId   integer,
            temperature     real,
            humidityId      integer,
            humidity        real
        )""")

def insertIntoDatabase(data):
    conn = sqlite3.connect('rpi_sensor.db')
    c = conn.cursor()
    with conn:
        c.execute("""INSERT INTO sensors (timestamp, deviceId0, deviceId1, deviceId2, packetId, temperatureId, temperature, humidityId, humidity)
                     VALUES (:timestamp, :deviceId0, :deviceId1, :deviceId2, :packetId, :temperatureId, :temperature, :humidityId, :humidity)""",
                  { 'timestamp'     : data.timestamp,
                    'deviceId0'     : data.deviceId0,
                    'deviceId1'     : data.deviceId1,
                    'deviceId2'     : data.deviceId2,
                    'packetId'      : data.packetId,
                    'temperatureId' : data.temperatureId,
                    'temperature'   : data.temperature,
                    'humidityId'    : data.humidityId,
                    'humidity'      : data.humidity
                    })

def getLastTimeStampByDeviceId(devId0, devId1, devId2):
    conn = sqlite3.connect("rpi_sensor.db")
    c = conn.cursor()
    with conn:
        c.execute("""SELECT timestamp FROM sensors
                     WHERE deviceId0=:deviceId0 AND deviceId1=:deviceId1 AND deviceId2=:deviceId2
                     ORDER BY ID DESC
                     LIMIT 1""",
                  {'deviceId0':devId0,
                   'deviceId1':devId1,
                   'deviceId2':devId2})
        return c.fetchone()

def getLastTemperatureByDeviceId(devId0, devId1, devId2):
    conn = sqlite3.connect("rpi_sensor.db")
    c = conn.cursor()
    with conn:
        c.execute("""SELECT temperature FROM sensors
                     WHERE deviceId0=:deviceId0 AND deviceId1=:deviceId1 AND deviceId2=:deviceId2
                     ORDER BY ID DESC
                     LIMIT 1""",
                  {'deviceId0':devId0,
                   'deviceId1':devId1,
                   'deviceId2':devId2})
        return c.fetchone()

def getLastHumidityByDeviceId(devId0, devId1, devId2):
    conn = sqlite3.connect("rpi_sensor.db")
    c = conn.cursor()
    with conn:
        c.execute("""SELECT humidity FROM sensors
                     WHERE deviceId0=:deviceId0 AND deviceId1=:deviceId1 AND deviceId2=:deviceId2
                     ORDER BY ID DESC
                     LIMIT 1""",
                  {'deviceId0':devId0,
                   'deviceId1':devId1,
                   'deviceId2':devId2})
        return c.fetchone()

def getNumberOfPacketsByDeviceId(devId0, devId1, devId2):
    conn = sqlite3.connect("rpi_sensor.db")
    c = conn.cursor()
    with conn:
        c.execute("""SELECT count(*) FROM sensors
                     WHERE deviceId0=:deviceId0 AND deviceId1=:deviceId1 AND deviceId2=:deviceId2""",
                  {'deviceId0':devId0,
                   'deviceId1':devId1,
                   'deviceId2':devId2})
        return c.fetchone()

def droptable():
    conn = sqlite3.connect('rpi_sensor.db')
    c = conn.cursor()
    with conn:
        c.execute("""DROP TABLE IF EXISTS sensors""")

def getAll():
    conn = sqlite3.connect('rpi_sensor.db')
    c = conn.cursor()
    with conn:
        c.execute("""SELECT * FROM sensors""")
        return c.fetchall()

#droptable()
#createTable()

#newdata1 = SensorData("2018-11-19 15:47:59", "0x42003c", "0x5246500e", "0x20383352", 1, 1, 28.2, 2, 62.2)
#newdata2 = SensorData("2018-11-18 14:36:49", "0x4a002e", "0x5246500e", "0x20383352", 1, 1, 25.8, 2, 40.1)

#insertIntoDatabase(newdata1)
#insertIntoDatabase(newdata2)

#print(getAll())

