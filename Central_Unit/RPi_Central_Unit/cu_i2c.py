def getID0(_block):
    _id0 = ((_block[3]<<24) | (_block[2]<<16) | (_block[1]<<8) | _block[0])
    return _id0

def getID1(_block):
    _id1 = ((_block[7]<<24) | (_block[6]<<16) | (_block[5]<<8) | _block[4])
    return _id1

def getID2(_block):
    _id2 = ((_block[11]<<24) | (_block[10]<<16) | (_block[9]<<8) | _block[8])
    return _id2
    
def getPackageID(_block):
    _packageid = ((_block[15]<<24) | (_block[14]<<16) | (_block[13]<<8) | (_block[12]))
    return (_packageid)

def getTemperatureID(_block):
    return (_block[16])

def getHumidityID(_block):
    return (_block[19])

def getTemperature(_block):
    low  = _block[17]
    high = _block[18]
    temp = ((high * 256) + low)
    return (((float(temp) / 65536) * 165) - 40)

def getHumidity(_block):
    low  = _block[20]
    high = _block[21]
    Hum  = ((high * 256) + low)
    return ((float(Hum) / 65536) * 100)
