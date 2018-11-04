import sqlite3
from Database.sensordata import *

conn = sqlite3.connect('sensor.db')
c = conn.cursor()

sensorId1 = [1111111111, 1111111111, 1111111111]
sensorId2 = [2222222222, 2222222222, 2222222222]

### Function definitions ###
def createTable():
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

def insertIntoDatabase(data):
    with conn:
        c.execute("""INSERT INTO sensors (deviceId0, deviceId1, deviceId2, packetId, temperatureId, temperature, humidityId, humidity)
            VALUES (:deviceId0, :deviceId1, :deviceId2, :packetId, :temperatureId, :temperature, :humidityId, :humidity)""", 
            {   'deviceId0'     : data.deviceId0,
                'deviceId1'     : data.deviceId1,
                'deviceId2'     : data.deviceId2,
                'packetId'      : data.packetId,
                'temperatureId' : data.temperatureId,
                'temperature'   : data.temperature,
                'humidityId'    : data.humidityId,
                'humidity'      : data.humidity
            })

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

######

# data0 = SensorData(sensorId1[0], sensorId1[1], sensorId1[2], 1, 1, 22.1, 2, 39.5)
# data1 = SensorData(sensorId1[0], sensorId1[1], sensorId1[2], 2, 1, 23.1, 2, 38.5)
# data2 = SensorData(sensorId1[0], sensorId1[1], sensorId1[2], 3, 1, 24.1, 2, 37.5)
# data3 = SensorData(sensorId1[0], sensorId1[1], sensorId1[2], 4, 1, 23.1, 2, 36.5)
# data4 = SensorData(sensorId1[0], sensorId1[1], sensorId1[2], 5, 1, 22.1, 2, 34.5)
# data5 = SensorData(sensorId1[0], sensorId1[1], sensorId1[2], 6, 1, 21.1, 2, 33.5)
# data6 = SensorData(sensorId1[0], sensorId1[1], sensorId1[2], 7, 1, 20.1, 2, 35.5)
# data7 = SensorData(sensorId1[0], sensorId1[1], sensorId1[2], 8, 1, 21.1, 2, 34.5)
# data8 = SensorData(sensorId1[0], sensorId1[1], sensorId1[2], 9, 1, 22.1, 2, 32.5)
# data9 = SensorData(sensorId1[0], sensorId1[1], sensorId1[2], 10, 1, 23.1, 2, 31.5)

# data10 = SensorData(sensorId2[0], sensorId2[1], sensorId2[2], 1, 1, 42.1, 2, 21.5)
# data11 = SensorData(sensorId2[0], sensorId2[1], sensorId2[2], 2, 1, 41.1, 2, 24.5)
# data12 = SensorData(sensorId2[0], sensorId2[1], sensorId2[2], 3, 1, 42.1, 2, 28.5)
# data13 = SensorData(sensorId2[0], sensorId2[1], sensorId2[2], 4, 1, 43.1, 2, 22.5)
# data14 = SensorData(sensorId2[0], sensorId2[1], sensorId2[2], 5, 1, 46.1, 2, 21.5)
# data15 = SensorData(sensorId2[0], sensorId2[1], sensorId2[2], 6, 1, 45.1, 2, 22.5)
# data16 = SensorData(sensorId2[0], sensorId2[1], sensorId2[2], 7, 1, 44.1, 2, 21.5)
# data17 = SensorData(sensorId2[0], sensorId2[1], sensorId2[2], 8, 1, 43.1, 2, 24.5)
# data18 = SensorData(sensorId2[0], sensorId2[1], sensorId2[2], 9, 1, 42.1, 2, 23.5)
# data19 = SensorData(sensorId2[0], sensorId2[1], sensorId2[2], 10, 1, 41.1, 2, 29.5)
# data20 = SensorData(sensorId2[0], sensorId2[1], sensorId2[2], 11, 1, 40.1, 2, 30.5)
 

# createTable()

# insertIntoDatabase(data0)
# insertIntoDatabase(data1)
# insertIntoDatabase(data2)
# insertIntoDatabase(data3)
# insertIntoDatabase(data4)
# insertIntoDatabase(data5)
# insertIntoDatabase(data6)
# insertIntoDatabase(data7)
# insertIntoDatabase(data8)
# insertIntoDatabase(data9)
# insertIntoDatabase(data10)
# insertIntoDatabase(data11)
# insertIntoDatabase(data12)
# insertIntoDatabase(data13)
# insertIntoDatabase(data14)
# insertIntoDatabase(data15)
# insertIntoDatabase(data16)
# insertIntoDatabase(data17)
# insertIntoDatabase(data18)
# insertIntoDatabase(data19)
# insertIntoDatabase(data20)

#  datas = getAllFromDatabase()
#  print(datas)


conn.close()
