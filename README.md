# ME507-Portfolio
Details for ME507 Term Project completed by Gabriel Ahern and Trevor Foley, Spring 2023, Cal Poly SLO

## Overview
The term project assigned in ME 507 for Spring Quarter of 2023 consisted of designing and manufactuing a robot for a large scale version of the game Hungry Hungry Hippos. In this game, our bot was required to start at one of four colored stations, then find and collect balls of the same color.
It was then supposed to deposit the collected balls at either its home station or launch them into a center bucket. The constraints and criteria for the project are detailed below:


## Electrical Criteria:
- A custom PCB designed around an STM32F411 MCU programmed in either C or C++.
- Two or more motors which drive the robot, actuated by suitable motor drivers.
- One or more ball detection sensors.
- One or more line detection sensors.
- At least one additional type of sensor of your choosing.
- An integrated 'kill' switch over radio or Bluetooth

## Mechanical/Manufacturing Criteria:
- Mass (Maximum) 1.5 kg
- Bounding Box (Maximum) 250 mm × 250 mm × 300 mm
- Battery Voltage (Maximum) 12.6 V (3S LiPo)
- Battery Capacity (Maximum) 19 W h
- Motor Size (Maximum, Can Only) 25 mm × 32 mm
- Motor Quantity (Maximum) 4
- Motor Power (Maximum, Combined) 50 W

## Major Hardware:
### MAJOR HARDWARE – MOTORS/ACTUATORS:
Our intention when creating the mechanical design for our hippo was to have an agile robot that could easily change directions to either pick up balls or avoid other bots. In order to meet this criterion, we chose to use four DC gearbox motors (“TT Motors”) to drive a set of mecanum wheels. This gave our bot the ability to strafe, move diagonally, and easily rotate in place.
In order to collect balls, we decided to use a vacuum to suck up any ball in front of our bot. This vacuum was located at the back of our bot, its entrance at the front, and designed to pull the balls first to a sorting junction with a color sensor and then up into the collection tank if they were of the correct color.
To control the flow and processing of balls inside of our robot, we used 2 servo motors. At the color sensor inside of our bot, one servo was designed to either push the ball out or allow it further into our collection tank, while the other ensured only 1 ball was allowed in front of the color sensor at a time.
### MAJOR HARDWARE – SENSORS:
While we initially intended on using a color sensor, IR sensors, motor encoders, and an accelerometer to perform line following, ball/bot/arena detection, and keep track of our bot’s location in the arena, we ended up deciding to use the overhead camera and its visual processing code – provided by Nathan – and proximity sensors for movement and location tracking. We did keep the color sensor, however, and aimed to use it to confirm that all balls collected were the correct color.
### MAJOR HARDWARE – PCB:
Our PCB design consists of 4 layers: 2 interiors for GND and 3v3 Power, a top layer for the STM MCU and its necessary components, and a bottom layer for power regulation (converting a 12V input down to 5V for sensor/motor rails and down again to 3v3 for the STM and other sensors). The reason why we put components on both the top and bottom layers was to reduce the overall dimensions of the board and conserve space inside the robot. While this did yield a much smaller footprint, it made it difficult to solder components as we could not just use a template for all; we were able to use a template for the components on the top layer, but had to hand-solder the bottom layer components. We also chose to shape it like the United States for aesthetics.
After all our components came in, we realized that we were missing one of our resistors. Due to lack of time, we were unable to order a replacement. However, we were able to use one of the through-hole resistors in the mechatronics lab as a replacement; we soldered on a through-hole resistor to the SMD pads as an emergency fix. 
After soldering our components to our pcb we found that the 5V and 3v3 rails were outputting the correct voltage, but the Power LED (Orange LED on the top of the board designed to indicate that power is connected) was not on. While probing the components on the power side of the board, we forgot to disconnect the power supply and accidentally caused a short in the board. Although we replaced the switching and linear regulators (the switching was shorted during probing and the linear was suspected of potentially not being entirely connected since the Power LED was not lit) and found that all power regulators were continuous (checked using the continuity feature on a multimeter), we were unable to find and correct the short in time. We suspect that either the board itself became damaged or that one of the other SMD components was shorted. Because of this, we switched back to the Blackpill for the remainder of the project. For images of the complete pcb and of its design in Fusion360 see appendix B. For details on the calculations of components for the pcb, see appendix X.
### MAJOR HARDWARE – ROBOT DESIGN:

## Software and Data:
### SOFTWARE – DRIVERS:
#### Color Sensor Driver:

#### Mecanum Wheels/Motor Driver:

#### Communication:
Because of the use of Nathan’s overhead camera as our main sensor for controlling our robot, we developed two external files (one in Python using VSS and another in C++ using the Arduino IDE) to access the camera data, process it to plot a trajectory to the nearest ball of the correct color, then send commands to the Master Mind/main file on the STM MCU over first Bluetooth serial (from our laptop to the ESP32) and then through USART (from the ESP32 to STM). The code for the Bluetooth Serial communication on the ESP32 is simple and is set up to grab the characters sent to it from the laptop and echo them to the STM. This code – and the code for the camera data processor - can be seen in Appendix X, and a deeper dive into the data processing and its setup occurs in the Data Interpretation section.

### SOFTWARE – MASTERMIND/MAIN LOOP:
 For our main loop/Master Mind file in the CubeIDE, we set it up so that it first checks the proximity sensors, then reads the commands from the topCamData file on our laptop to move accordingly, and finally checks the color sensor to see if a ball has been detected. 
For the proximity sensor state, we first determine which of the two front sensors has been activated, then strafe in the opposite direction to avoid collision. If both have been activated, we then wait a second before checking again (in this case it is assumed that a bot has been detected in front of ours). Ideally, this state would confirm with the upper camera whether the object being detected is the central bucket or a robot and move in a more complex manner than just strafing. However, due to time constraints, we decided it would be best just to implement a strafing or waiting mechanism to take care of most potential collisions. This state ends with a delay before continuing to check movement so as to ensure its strafing has occurred.
After the sensor has been activated, the main loop checks the character received through USART from the topCamData file and moves accordingly (either rotating, driving, or disabling motors/stopping).
At the end of this loop, the color sensor is read and, if a color is detected (by checking its level compared to the empty tube calibration value), it then determines whether it’s the correct color or not. If it is, the servo used to hold the balls back is turned on to allow the ball to continue up to storage. If not, the hold servo is still called to allow it forward, but the flapper servo is also called to kick the ball out of the bot.
Because we were unable to finish putting our hippo together, and were focused primarily on that towards the end, this file is mainly a schematic; the file was created as a structure that details all the states and their conditions and specifies what occurs in each (using comments) but is not populated with the code to call the completed drivers. It does have all the pins and timers set up correctly to control each component, as well as the code for processing USART values (which has been tested and confirmed to be working correctly).
Our strategy when creating our code was to first create all the necessary drivers for controlling each sensor and motor, then develop and test the code for communication and robot/ball position processing, and only then create the main loop/Master Mind file to integrate everything. 
With the drivers, the main strategy was to break up its necessary functionality into small methods that could be called either in a main loop (as in the case of the topCamData file for position/trajectory processing and communication) or other methods within the driver.
For the Master Mind file, an FSM was used to plan each necessary state and the flow for controlling the hippo. Its details – as well as its FSM - are in the previous section.

### DATA INTERPRETATION:
#### Top Camera Data Processor:

In the Python file (which was run on our laptop) we first pull data from Nathan’s overhead camera which gives us two lists filled with dictionaries: one for all the balls, and another for the robots. We then take this data and run it through a method named ‘trajectory’, which returns the distance to the nearest, correctly colored ball, the angle from our bot to the ball, the dictionary holding our bot’s data, and the index in the balls list for the nearest ball. To determine the closest ball (of the desired color) we first check the color of every ball in the balls data list (through a for-loop) and, if it is the correct color, find the distance between our bot and it. The equation used was the vector distance equation and is below.

> math.sqrt(Xhipp – ballX)^2 +(Yhipp – ballY)^2)

We store this distance value as well as the index of the ball in the balls list in variables dist and closeball, respectively, then repeat the process, updating these values every time a ball is found to be closer.
The next step in the method is to find the angle from our bot to ball. Because we know the coordinates of both the ball and our bot, we use the arctan function (where the numerator is the difference in Y positions and the denominator is the difference in X), then convert its radian output to degrees. The distance equation is below.

> math.atan2((balls[closeBall]['y'] - Y_hip),(balls[closeBall]['x'] - X_hip)) #gives number b/t pi and -pi

After determining the angle from our bot to the ball, we run a method named ‘angController’, which returns letter values corresponding to commands for rotating the bot (sent through the ESP32 to Master Mind) until its orientation matches that of the required angle to the ball. It does so by first finding the difference between the angles, then determining if said difference is positive, negative, or zero (where positive and negative values correlate to clockwise and counterclockwise rotation commands and zero tells the bot it is in line). If the bot is orientated toward the ball, a command is then sent to the STM to have it drive forward.

An important note about this driver is that there is no command to stop the robot when it reaches the ball; once it reaches the ball and sucks it up, it should immediately find the next closest, correctly colored ball and repeat the above process.

