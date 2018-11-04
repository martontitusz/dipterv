
class SensorData:

    def __init__(self, deviceId0, deviceId1, deviceId2, packetId, temperatureId, temperature, humidityId, humidity):
        self.deviceId0      = deviceId0
        self.deviceId1      = deviceId1
        self.deviceId2      = deviceId2
        self.packetId       = packetId
        self.temperatureId  = temperatureId
        self.temperature    = temperature
        self.humidityId     = humidityId
        self.humidity       = humidity
