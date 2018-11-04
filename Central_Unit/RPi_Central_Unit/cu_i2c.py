def countIDWord(byte0, byte1, byte2, byte3):
    result = ((byte3<<24) | (byte2<<16) | (byte1<<8) | byte0)
    return result

def countID(block):
    word0 = countIDWord(block[0], block[1], block[2], block[3])
    word1 = countIDWord(block[4], block[5], block[6], block[7])
    word2 = countIDWord(block[8], block[9], block[10], block[11])
    result = hex((word2<<64) | (word1<<32) | word0)
    return result
    

def countTemperature(low, high):
    Temp = ((high * 256) + low)
    return (((float(Temp) / 65536) * 165) - 40)

def countHumidity(low, high):
    Hum = ((high * 256) + low)
    return ((float(Hum) / 65536) * 100)
