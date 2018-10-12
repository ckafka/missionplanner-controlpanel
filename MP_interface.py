import sys
sys.path.append(r"c:\Python27\Lib\site-packages")
sys.path.append(r"c:\Python27\Lib")
import MissionPlanner
import MAVLink
import serial

topRowButtons       = ["FBWA", "Guided", "Manual", ""]
bottomRowButtons    = ["Loiter", "Stabilize", "Auto", ""]
auxSwitch           = ["RTL"] 
actions             = bottomRowButtons + topRowButtons + auxSwitch
armMessages         = ["disarmed", "armed"]

ser = serial.Serial('COM4', 9600, timeout=15) #edit COM port here
ser.xonxoff=1
boxInput = ""
lastMode = ""
armed = False
commandArm = False 
armMessage = 8
action = 0

while(1):
    boxInput = ser.read_until().split('.')
    commandArm = int(boxInput[0])
    flightMode = actions[int(boxInput[1])]

    print ("flight mode:  " + flightMode)
    print ("arm status: " + armMessages[commandArm])

    if commandArm and not armed: 
        MAV.doARM(True)
        armed = True
    elif not commandArm and armed:
        MAV.doARM(False)
        armed = False
    if lastMode != flightMode:
        Script.ChangeMode(flightMode)
    lastMode = flightMode 


