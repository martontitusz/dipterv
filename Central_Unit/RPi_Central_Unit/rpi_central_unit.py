import sys
import time
from smbus2 import *
from tkinter import *
import tkinter.font
import tkinter.ttk
import RPi.GPIO as GPIO
import paho.mqtt.publish as publish

#---my modules---
from rpi_cu_i2c import *
from rpi_database import *

#---SENSOR DEFINITIONS---
sensorId1 = ["0x42003c", "0x5246500e", "0x20383352"]
sensorId2 = ["0x4a002e", "0x5246500e", "0x20383352"]
sensor1IdFull = sensorId1[0]+sensorId1[1]+sensorId1[2]
sensor2IdFull = sensorId2[0]+sensorId2[1]+sensorId2[2]

#---I2C DEFINITIONS---
slaveAddress    = 17
packetLength    = 22
offset          = 0

#---MQTT DEFINITONS---
host    = "test.mosquitto.org"
topic   = "tituszdipterv/test"

#---GPIO DEFINITIONS---
GPIO.setmode(GPIO.BCM)
GPIO_I2CBUSY_PIN = 18
GPIO.setup(GPIO_I2CBUSY_PIN, GPIO.IN)

#---GUI---
#---WINDOW---
window = Tk()
window.title("Sensor Network")
window.configure(background = "ghost white")
window.geometry("480x320")
#window.overrideredirect(1)

#---CANVAS---
canvas = Canvas(window, width=480, height=320, background="ghost white")
canvas.create_line(  0,  75, 480,  75)
canvas.create_line(  0, 100, 480, 100)
canvas.create_line(  0, 130, 480, 130)
canvas.create_line(  0, 158, 480, 158)
canvas.create_line(  0, 265, 480, 265)
canvas.create_line(  0, 290, 480, 290)
canvas.create_line(240,  75, 240, 100)
canvas.create_line(240, 130, 240, 265)
canvas.create_line(240, 290, 240, 320)
canvas.grid(row=0, column=0, rowspan=7, columnspan=2, sticky=W+E+N+S)

#---LABELS---
labelCurrentTime        = Label(window,
                                bg         ="ghost white",
                                fg         ="black",
                                font       ="none 20 bold",
                                width      ="25",
                                height     ="2",
                                borderwidth="0",
                                relief     ="solid")
labelCurrentTime.grid(row=0, column=0, columnspan=2, sticky=W+E+N+S)

labelSensor1Name        = Label(window,
                                bg         ="ghost white",
                                fg         ="black",
                                font       ="none 10 bold",
                                width      ="24",
                                borderwidth="0",
                                relief     ="solid")
labelSensor1Name.config(text="SM #1")
labelSensor1Name.grid(row=1, column=0)

labelSensor2Name        = Label(window,
                                bg         ="ghost white",
                                fg         ="black",
                                font       ="none 10 bold",
                                width      ="24",
                                borderwidth="0",
                                relief     ="solid")
labelSensor2Name.config(text="SM #2")
labelSensor2Name.grid(row=1, column=1)

labelLastUpdate         = Label(window,
                                bg         ="ghost white",
                                fg         ="black",
                                font       ="none 10 bold",
                                width      ="24",
                                borderwidth="0",
                                relief     ="solid")
labelLastUpdate.config(text="Last update:")
labelLastUpdate.grid(row=2, column=0, columnspan=2, sticky=W+E)

labelSensor1Update      = Label(window,
                                bg         ="ghost white",
                                fg         ="black",
                                font       ="none 10 bold",
                                width      ="24",
                                borderwidth="0",
                                relief     ="solid")
lastTimeStamp1 = getLastTimeStampByDeviceId(sensorId1[0],sensorId1[1],sensorId1[2])
labelSensor1Update.config(text=lastTimeStamp1[0])
labelSensor1Update.grid(row=3, column=0)

labelSensor2Update      = Label(window,
                                bg         ="ghost white",
                                fg         ="black",
                                font       ="none 10 bold",
                                width      ="24",
                                borderwidth="0",
                                relief     ="solid")
lastTimeStamp2 = getLastTimeStampByDeviceId(sensorId2[0],sensorId2[1],sensorId2[2])
labelSensor2Update.config(text=lastTimeStamp1[0])
labelSensor2Update.grid(row=3, column=1)

labelSensor1Data        = Label(window,
                                bg         ="ghost white",
                                fg         ="black",
                                font       ="none 20 bold",
                                width      ="12",
                                height     ="3",
                                borderwidth="0",
                                relief     ="solid")
lastTemp1    = getLastTemperatureByDeviceId(sensorId1[0],sensorId1[1],sensorId1[2])
lastHum1     = getLastHumidityByDeviceId(sensorId1[0],sensorId1[1],sensorId1[2])
labelSensor1Data.config(text=str(lastTemp1[0])+"°C\n"+str(lastHum1[0])+"%")
labelSensor1Data.grid(row=4, column=0)

labelSensor2Data        = Label(window,
                                bg         ="ghost white",
                                fg         ="black",
                                font       ="none 20 bold",
                                width      ="12",
                                height     ="3",
                                borderwidth="0",
                                relief     ="solid")
lastTemp2    = getLastTemperatureByDeviceId(sensorId2[0],sensorId2[1],sensorId2[2])
lastHum2     = getLastHumidityByDeviceId(sensorId2[0],sensorId2[1],sensorId2[2])
labelSensor2Data.config(text=str(lastTemp2[0])+"°C\n"+str(lastHum2[0])+"%")
labelSensor2Data.grid(row=4, column=1)

labelReceivedPackages   = Label(window,
                                bg         ="ghost white",
                                fg         ="black",
                                font       ="none 10 bold",
                                width      ="24",
                                height     ="1",
                                borderwidth="0",
                                relief     ="solid")
labelReceivedPackages.config(text="Received packages")
labelReceivedPackages.grid(row=5, column=0, columnspan=2, sticky=W+E)

labelSensor1Packages    = Label(window,
                                bg         ="ghost white",
                                fg         ="black",
                                font       ="none 10 bold",
                                width      ="24",
                                height     ="1",
                                borderwidth="0",
                                relief     ="solid")
packets1     = getNumberOfPacketsByDeviceId(sensorId1[0],sensorId1[1],sensorId1[2])
labelSensor1Packages.config(text=str(packets1[0]))
labelSensor1Packages.grid(row=6, column=0)

labelSensor2Packages    = Label(window,
                                bg         ="ghost white",
                                fg         ="black",
                                font       ="none 10 bold",
                                width      ="24",
                                height     ="1",
                                borderwidth="0",
                                relief     ="solid")
packets2     = getNumberOfPacketsByDeviceId(sensorId2[0],sensorId2[1],sensorId2[2])
labelSensor2Packages.config(text=str(packets2[0]))
labelSensor2Packages.grid(row=6, column=1)

#---FUNCTIONS---
def tick():
    time_string = time.strftime("%Y-%m-%d %H:%M:%S")
    labelCurrentTime.config(text=time_string)
    labelCurrentTime.after(200, tick)

def closeFunction():
    GPIO.cleanup()
    window.destroy()

def gpioEventCallbackFunction(input_int):
    I2C_GetDataFunction()

def getArrowSymbol(old, new):
    if old < new:
        return "⇑ "
    elif old > new:
        return "⇓ "
    else:
        return "  "

def I2C_GetDataFunction():
    with SMBusWrapper(1) as bus:
        block = bus.read_i2c_block_data(slaveAddress, offset, packetLength)

        time_string     = time.strftime("%Y-%m-%d %H:%M:%S")
        ID0             = hex(getID0(block))
        ID1             = hex(getID1(block))
        ID2             = hex(getID2(block))
        PackageID       = getPackageID(block)
        TemperatureID   = getTemperatureID(block)
        Temperature     = getTemperature(block)
        HumidityID      = getHumidityID(block)
        Humidity        = getHumidity(block)

        id_string       = ID0+ID1+ID2

        data = ""
        data = data+","+time_string
        data = data+","+ID0+","+ID1+","+ID2
        data = data+","+str(PackageID)
        data = data+","+str(TemperatureID)
        data = data+","+str(Temperature)
        data = data+","+str(HumidityID)
        data = data+","+str(Humidity)
        data = data+","
        
        #refresh labels
        if id_string == sensor1IdFull:
            lastTemp    = getLastTemperatureByDeviceId(sensorId1[0],sensorId1[1],sensorId1[2])
            lastHum     = getLastHumidityByDeviceId(sensorId1[0],sensorId1[1],sensorId1[2])
            arrowTemp   = getArrowSymbol(lastTemp, Temperature)
            arrowHum    = getArrowSymbol(lastHum, Humidity)
            packets     = getNumberOfPacketsByDeviceId(sensorId1[0],sensorId1[1],sensorId1[2])

            labelSensor1Update.config(text=time_string)
            labelSensor1Data.config(text=arrowTemp+str(Temperature)+"°C\n"+arrowHum+str(Humidity)+"%")
            labelSensor1Packages.config(text=str(packets))
            
        elif id_string == sensor2IdFull:
            lastTemp    = getLastTemperatureByDeviceId(sensorId2[0],sensorId2[1],sensorId2[2])
            lastHum     = getLastHumidityByDeviceId(sensorId2[0],sensorId2[1],sensorId2[2])
            arrowTemp   = getArrowSymbol(lastTemp, Temperature)
            arrowHum    = getArrowSymbol(lastHum, Humidity)
            packets     = getNumberOfPacketsByDeviceId(sensorId2[0],sensorId2[1],sensorId2[2])

            labelSensor2Update.config(text=time_string)
            labelSensor2Data.config(text=arrowTemp+str(Temperature)+"°C\n"+arrowHum+str(Humidity)+"%")
            labelSensor2Packages.config(text=str(packets))
            
        else:
            print("Invalid SensorId")

        #publishing data via MQTT
        publish.single(topic, data, hostname=host)

        #push data to the database
        newdata = SensorData(time_string, ID0, ID1, ID2, PackageID, TemperatureID, Temperature, HumidityID, Humidity)
        insertIntoDatabase(newdata)
        
GPIO.add_event_detect(GPIO_I2CBUSY_PIN, GPIO.RISING)
GPIO.add_event_callback(GPIO_I2CBUSY_PIN, gpioEventCallbackFunction)

createTable()
tick()
window.protocol("WM_DELETE_WINDOW", closeFunction) # Exit cleanly
window.mainloop() # Loop forever
