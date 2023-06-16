//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial

#include "BluetoothSerial.h"
#include <HardwareSerial.h>

#define RX0 12
#define TX0 13
HardwareSerial SerialPort(0);

#define USE_PIN // Uncomment this to use PIN during pairing. The pin is specified on the line below
const char *pin = "8008"; // Change this to more secure PIN.
//#define STM32_Pin 12
//#define testPin 13
String command;
String device_name = "Fuck Me";

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin(device_name); //Bluetooth device name
  Serial.printf("\"%s\" is started.\nPair it with Bluetooth!\n", device_name.c_str());
  //Serial.printf("The device with name \"%s\" and MAC address %s is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str(), SerialBT.getMacString()); // Use this after the MAC method is implemented
  #ifdef USE_PIN
    SerialBT.setPin(pin);
    Serial.println("Using PIN");
  #endif

  SerialPort.begin(115200,SERIAL_8N1,RX0,TX0);
}

void loop() {
  // Read received messages (LED control command)
  if (SerialBT.available()){
    char phoneChar = SerialBT.read();
    if (phoneChar != '\n'){
      command += String(phoneChar);
    }
    else{
      command = "";
    }
    Serial.write(phoneChar);  
  }
  // Check received message and control output accordingly
  /*if (command =="stop" || command == "Stop" || command == "STOP"){
    digitalWrite(STM32_Pin, HIGH);
  }
  else if (command =="test" || command == "Test" || command == "TEST"){
    digitalWrite(testPin, HIGH);
  }*/
  //Serial.println(command);
  //SerialPort.write('B');
  SerialPort.print(command);
  delay(10);
  Serial.println(SerialPort.read());
  delay(100);

}
