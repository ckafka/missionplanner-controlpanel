import sys
import MissionPlanner
import MAVLink
import serial

topRowButtons       = ["FBWA", "Guided", "Manual", ""]
bottomRowButtons    = ["Loiter", "Stabilize", "Auto", ""]
auxSwitch           = ["RTL"] 
actions             = bottomRowButtons + topRowButtons + auxSwitch

ser = serial.Serial('COM4', 9600, timeout=15) #edit COM port here
boxInput = ""
lastMode = ""
armStatus = False
commandArm = False 
armMessage = 8

while(1):
    armInput = ser.read()
    action   = ser.read()
    flightMode = actions[(int)action]
    commandArm = True if armInput == armMessage else commandArm = false
    print("flight mode:  " + flightMode)
    print("arm status :  armed \n") if armStatus else print("arm status : disarmed \n")
    if commandArm and not armed: 
        MAV.doARM(True)
        armStatus = True
    elif not commandArm and armed:
        MAV.doARM(False)
        armStatus = False
    if lastMode != flightMode:
        Script.ChangeMode(flightMode)
    lastMode = flightMode


