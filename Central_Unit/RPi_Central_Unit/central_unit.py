# import for I2C:
from smbus2 import *
# import for GUI:
from tkinter import *
import tkinter.font
import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BCM)
# import for MQTT:
import paho.mqtt.publish as publish


## my modules ##
from cu_i2c import *

## I2C DEFINITIONS ##
slaveAddress = 17
packetLength = 22
offset = 0

## MQTT DEFINITONS ##
host = "test.mosquitto.org"
topic = "tituszdipterv/test"

## GPIO DEFINITIONS ##
GPIO_I2CBUSY_PIN = 18
GPIO.setup(GPIO_I2CBUSY_PIN, GPIO.IN)

## GUI DEFINITIONS ##
window = Tk()
window.title("Sensor Network")
window.configure(background = "black")
window.geometry("480x320")
myFont = tkinter.font.Font(family = 'Helvetica', size = 12, weight = "bold")

## LABELS ##
IDLabel = Label(window, text="ID", bg="grey", fg="white", font="none 12 bold", width="14")
IDLabel.grid(row=0, column=0, sticky=W)
TempLabel = Label(window, text="Temp", bg="grey", fg="white", font="none 12 bold", width="14")
TempLabel.grid(row=0, column=1, sticky=W)
HumLabel = Label(window, text="Hum", bg="grey", fg="white", font="none 12 bold", width="14")
HumLabel.grid(row=0, column=2, sticky=W)

## OUTPUT TEXT BOX ##
IDOutput = Text(window, width=22, height=3, wrap=WORD, background="black", foreground="white")
IDOutput.grid(row=1, column=0, sticky=W)
TempOutput = Text(window, width=22, height=3, wrap=WORD, background="black", foreground="white")
TempOutput.grid(row=1, column=1, sticky=W)
HumOutput = Text(window, width=22, height=3, wrap=WORD, background="black", foreground="white")
HumOutput.grid(row=1, column=2, sticky=W)

## EVENT FUNCTIONS ##
def I2C_GetDataFunction():
    with SMBusWrapper(1) as bus:
        block = bus.read_i2c_block_data(slaveAddress, offset, packetLength)
        #print(block)

        ID0             = hex(getID0(block))
        ID1             = hex(getID1(block))
        ID2             = hex(getID2(block))
        PackageID       = getPackageID(block)
        TemperatureID   = getTemperatureID(block)
        Temperature     = getTemperature(block)
        HumidityID      = getHumidityID(block)
        Humidity        = getHumidity(block)

        fillTextbox(ID0, ID1, ID2, Temperature, Humidity)
        data = (","+ID0+","+ID1+","+ID2+","+str(PackageID)+","+str(TemperatureID)+","+str(Temperature)+","+str(HumidityID)+","+str(Humidity)+",")
        
        #publishing data via MQTT
        #topic, payload, hostname
        publish.single(topic, data, hostname=host)
        print("Data published.\n")
        

def fillTextbox(id0, id1, id2, temperature, humidity):
    IDOutput.delete(0.0, END)
    TempOutput.delete(0.0, END)
    HumOutput.delete(0.0, END)
    
    IDOutput.insert(END, id0)
    IDOutput.insert(END, "\n")
    IDOutput.insert(END, id1)
    IDOutput.insert(END, "\n")
    IDOutput.insert(END, id2)
    
    TempOutput.insert(END, temperature)
    TempOutput.insert(END, "°C")
    HumOutput.insert(END, humidity)
    HumOutput.insert(END, "%")

def closeFunction():
    GPIO.cleanup()
    window.destroy()

def gpioEventCallbackFunction(input_int):
    I2C_GetDataFunction()

GPIO.add_event_detect(GPIO_I2CBUSY_PIN, GPIO.RISING)
GPIO.add_event_callback(GPIO_I2CBUSY_PIN, gpioEventCallbackFunction)

## WIDGETS ##
I2CButton = Button(window, text = 'Get Data', font = myFont, command = I2C_GetDataFunction, bg = 'bisque2', height = 1, width = 14)
I2CButton.grid(row=2, column=0)

exitButton = Button(window, text = 'Exit', font = myFont, command = closeFunction, bg = 'red', height = 1, width = 14)
exitButton.grid(row=2, column=2)

window.protocol("WM_DELETE_WINDOW", closeFunction) # Exit cleanly

window.mainloop() # Loop forever
