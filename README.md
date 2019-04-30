# ThingSpeak-Multi-Devices
Operate Devices using ThingSpeak and ESP8266 (D1 mini)


//this program takes a "code" from your Thingspeak channel and operates a device according to his "bit"

// code has 5 bits


// 1---- is for testing and always '1'

// -X--- for SERVO_1

// --X-- for SERVO_2

// ---X- for LED (LIGHTS)

// ----X for Music 

//servos switching between 2 angle states (servo_on,servo_off)

// has 2 songs,(has 2 short-form songs, Despacito, POTC(Pirates of the Caribbean), it chooses randomly  which song to play when it bit is '1'


// every time the system is powering up, it turn off all devices and send the code "10000" to your Thingspeak channel

// also writes the mode in the Serial Monitor

// connects to the strongest signal (WiFi) from the wifiMulti AP added in the "void setup" section
