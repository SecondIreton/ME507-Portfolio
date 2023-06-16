import requests
import json
#from serial import Serial
import serial
import math
#import sys
import time
import threading


def topCamData():
    #Access cameras website (make sure connected to hotspot...joemama123)
    #response = requests.get('http://10.144.129.5:5000/info')
    #data = response.json()

    #Get balls and robots lists of dictionaries from data
    #balls = data['balls']
    #robots = data['robots']

    #Returns the balls list of dicts and robots list of dicts
    #return balls, robots

    balls = [{'color': 'green', 'x': 264, 'y': 218}, {'color': 'blue', 'x': 264, 'y': 218}, {'color': 'red', 'x': 280, 'y': 230}, 
             {'color': 'blue', 'x': 50, 'y': 70}, {'color': 'green', 'x': 200, 'y': 218}, {'color': 'yellow', 'x': 240, 'y': 28}]
    robots = [{'center': [195,180], 'decoded_info': '2', 'orientation': -90}, {'center': [26,30], 'decoded_info': '27', 'orientation': 170},
              {'center': [300,180], 'decoded_info': '5', 'orientation': 90}, {'center': [195,300], 'decoded_info': '8', 'orientation': 0}]
    return balls, robots


def trajectory(balls,robots):
#This method determines trajectory to closest ball
    for botDict in robots:
        #botDict = robots[i] #Temp storage of each dictionary in robots list
        if(botDict['decoded_info'] == '27'): #If the 'decoded_info' key equals our bots number, 27, its our hippo
            hippo = botDict #robots[i]
            X_hip, Y_hip = hippo['center']
            Ang_hip = hippo['orientation']
    
    dist = 10000
    i = 0
    closeBall = 0
    for ballDict in balls:
        #calculates the vector dist between each ball and hippo, then records shortest length and index of closest
        '''ballX = balls[j]['x']
        ballY = balls[j]['y']
        ballCol = balls[j]['color']'''
        ballX = ballDict['x']
        ballY = ballDict['y']
        ballCol = ballDict['color']
        if ballCol == 'blue': #Change to whatev color for match
            tempDist = math.sqrt((X_hip-ballX)**2 + (Y_hip-ballY)**2)
            #compares distances to determine which is smallest/closest ball and its index
            if(tempDist<dist):
                dist = tempDist
                closeBall = i
        
        i = i + 1

    reqdAngle = math.atan2((balls[closeBall]['y'] - Y_hip),(balls[closeBall]['x'] - X_hip)) #gives number b/t pi and -pi
    reqdAngle = math.degrees(reqdAngle) #should convert radian to degrees...hopefully

    return dist, reqdAngle, hippo, closeBall
    
    #Find angle between each bot and hippo, determine if that's similar to the reqd angle of ball within a few degrees, then 
    #extend it in the direction of the bots orientation to account for intended trajectory

    
'''def botCheck(robots, hippo, dist, reqdAngle):    
#checks to see if any bots are in the way of our trajectory. Finish last/if have enough time
    X_hip, Y_hip = hippo['center']
    for k in robots:
        if(robots[k]['decoded_info'] != '27'):
            Xbot,Ybot = robots[k]['center']
            botToBot = math.sqrt((X_hip-Xbot)**2 + (Y_hip-Ybot)**2)
            botAng = math.degrees(math.atan2((Ybot - Y_hip),(Xbot - X_hip)))
            if(reqdAngle>=(botAng-5) or reqdAngle<=(botAng+5)): #check this logic dumbass
                if(botToBot<=dist):
                     #SHIEET BRA WE GOT A BOT IN THE WAY
                     time.delay(1000) #don't have enough time to calculate trajectory, so wait for it to move


        #also put in code for going around center bucket

def arenaBounds()'''


def angController(reqdAngle, hippo):
#P controller for determining if angle of bot matches redq angle
    Ang_hip = hippo['orientation']
    diffAng = reqdAngle - Ang_hip
    diffAng = diffAng*10
    if(diffAng<0): #reqdAngle < Ang_hip
        #tell bot to rotate towards angle
        return 'C' #C = CCW
    elif(diffAng>0): #reqdAngle > Ang_hip
        return 'W' #W = CW
    else:
        return 'A' #A = At angle, stop rotating
    

'''def distController(hippo, atAng, closeBall, balls):
#P controller for checking dist from bot to ball and making sure is decreasing
#may not use this and instead will just drive towards ball until picked up
    X_hip, Y_hip = hippo['center']
    ballX = balls[closeBall]['x']
    ballY = balls[closeBall]['y']
    diffDist = math.sqrt((X_hip-ballX)**2 + (Y_hip-ballY)**2)
    if(diffDist > 0 and atAng == 'A'):
        return 'D' #D = Drive
    else:
        return 'B' #B = at ball, stop driving
    '''
    

def main():
    #Set up Serial Comms [use BT connected Port]
    ser = serial.Serial()
    ser.baudrate = 115200
    ser.port = 'COM17' #Rename to COM whatever the BT COM number is
    ser.open()

    
    #ser.write(str(ColorInput).encode('ascii'))
    #ser.write(b'B')

    #gets initial posisition of hippo and records it so it can later go home
    #userInput = input()
    #ser.write(str(userInput).encode('ascii'))
    sepuku = True
    home = True
    ballData, botData = topCamData()
    dist, reqdAngle, hippo, closeBall = trajectory(ballData, botData)
    print(closeBall)
    col = ballData[closeBall]['color']
    print(col)
    X_home,Y_home = hippo['center']
    Ang_home = hippo['orientation']

    #continues to run until KILL command inserted
    while(sepuku==True):
        try:    
            #gets balls and robots location data for processing, then finds nearest ball 
            balls, robots = topCamData()
            dist, reqdAngle, hippo, closeBall = trajectory(balls, robots)
            #print(dist)
            #print(reqdAngle)

            #try:
            #calculate trajectory to nearest ball and send driving commands
            command = angController(reqdAngle, hippo)
            #print(command)
            if(command == 'A'):
                #command = distController(hippo, command, closeBall, balls)
                command = 'D'
                print(command)
                #ser.write(str(command).encode('ascii'))
        except KeyboardInterrupt:
            print("Interrupt")
            userInput = input()

            if(userInput == 'K'):
                #Exits while loop and kills all motors
                sepuku = False
                command = 'Z' #Z = kill all motors
                print(command)
            elif(userInput == 'H'):
            #sendd bot home and exits while loop and kills motors if at home
                while(home == True):
                    X_hip,Y_hip = hippo['center']
                    homeAngle = math.atan2((Y_home - Y_hip),(X_home - X_hip)) #gives number b/t pi and -pi
                    homeAngle = math.degrees(reqdAngle)
                    command = angController(homeAngle, hippo)
                    if(command == 'A' and X_hip != X_home and Y_hip != Y_home):
                        command = 'D'
                    elif(X_hip == X_home and Y_hip == Y_home):
                        sepuku = False
                        command = 'Z'
                        home = False
                    print(command)
                    #ser.write(str(command).encode('ascii'))
                    

'''#gets user input if any entered to check if 'KILL' or 'HOME' entered
        userInput = input()

        if(userInput):
            if(userInput == 'KILL'):
                #Exits while loop and kills all motors
                sepuku = False
                command = 'Z' #Z = kill all motors
            elif(userInput == 'HOME'):
            #sendd bot home and exits while loop and kills motors if at home
                X_hip,Y_hip = hippo['center']
                homeAngle = math.atan2((Y_home - Y_hip),(X_home - X_hip)) #gives number b/t pi and -pi
                homeAngle = math.degrees(reqdAngle)
                command = angController(homeAngle, hippo)
                if(command == 'A' and X_hip != X_home and Y_hip != Y_home):
                    command = 'D'
                elif(X_hip == X_home and Y_hip == Y_home):
                    sepuku = False
                    command = 'Z'
            else:
                userInput = False
        else:
            #calculate trajectory to nearest ball and send driving commands
            command = angController(reqdAngle, hippo)
            if(command == 'A'):
                #command = distController(hippo, command, closeBall, balls)
                command = 'D' '''

'''#gets balls and robots location data for processing, then finds nearest ball 
        balls, robots = topCamData()
        dist, reqdAngle, hippo, closeBall = trajectory(balls, robots)
        print(dist)
        print(reqdAngle)

        if(userInput == 'KILL'):
            #Exits while loop and kills all motors
            sepuku = False
            command = 'Z' #Z = kill all motors
        elif(userInput == 'HOME'):
            #sendd bot home and exits while loop and kills motors if at home
            X_hip,Y_hip = hippo['center']
            homeAngle = math.atan2((Y_home - Y_hip),(X_home - X_hip)) #gives number b/t pi and -pi
            homeAngle = math.degrees(reqdAngle)
            command = angController(homeAngle, hippo)
            if(command == 'A' and X_hip != X_home and Y_hip != Y_home):
                command = 'D'
            elif(X_hip == X_home and Y_hip == Y_home):
                sepuku = False
                command = 'Z'
        else:
            #calculate trajectory to nearest ball and send driving commands
            command = angController(reqdAngle, hippo)
            if(command == 'A'):
                #command = distController(hippo, command, closeBall, balls)
                command = 'D' '''

        #send command for driving to blackpill mastermind
        #print(command)
        #ser.write(str(command).encode('ascii'))
    

if __name__ == '__main__':
        print('running')
        main()
        #print('running')
