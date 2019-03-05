import wiringpi
import time
from lcdlib_data import *

RST = 6
DC = 5
SS = 10
OUTPUT = 1
Channel = 0
Speed = 500000

NOP = 0x00cd 
Function_Set = 0x20
Display_Control = 0x08
Set_Y_Address = 0x40
Set_X_Address = 0x80
Set_V0 = 0x80
Set_Test_Mode = 0x30

wiringpi.wiringPiSetup()
wiringpi.pinMode(RST, OUTPUT)
wiringpi.pinMode(DC, OUTPUT)
wiringpi.pinMode(SS, OUTPUT)
wiringpi.wiringPiSPISetup(Channel, Speed)

def send_cmds(cmds):
    wiringpi.digitalWrite(SS, 0)    
    wiringpi.digitalWrite(DC, 0)
    wiringpi.wiringPiSPIDataRW(Channel, bytes(cmds))
    wiringpi.digitalWrite(SS, 1)

def send_dats(dats):
    wiringpi.digitalWrite(SS, 0)
    wiringpi.digitalWrite(DC, 1)
    wiringpi.wiringPiSPIDataRW(Channel, bytes(dats))
    wiringpi.digitalWrite(SS, 1)

def LCD_Set_XY(x, y):
    send_cmds([Set_X_Address|x, Set_Y_Address|y])

def cls():
    LCD_Set_XY(0, 0)
    send_dats([0x00]*960)

def show_bmp(BMP):
    for y in range(7):
        for x in range(96):
            LCD_Set_XY(x, y)
            send_dats([BMP[x + y*96]])
            
wiringpi.digitalWrite(RST, 0)
time.sleep(0.01)
wiringpi.digitalWrite(RST, 1)
time.sleep(0.01)
wiringpi.digitalWrite(SS, 1)

cmds = [Function_Set|0x01, Set_V0|0x30, Set_Test_Mode|0x02,
        Function_Set|0x00, Display_Control|0x04]
send_cmds(cmds)
cls()

show_bmp(A)

