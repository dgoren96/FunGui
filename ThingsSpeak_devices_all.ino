//Was specifically written for the ESP8266 D1 mini
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

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <stdio.h>
#include <ThingSpeak.h>
#include <Wire.h>
#include <Arduino.h>
#include <ctype.h> // for isNumber check
#include <Servo.h>
#include "pitches.h"
ESP8266WiFiMulti wifiMulti;
int noteDuration;
int pauseBetweenNotes = 0;
Servo myservo_servo_1; 
Servo myservo_servo_2; 



void UpdateDevices(char* code_arr);
void ResetDevices();
void ResetCode();
void tone2(uint8_t _pin, unsigned int frequency, unsigned long duration);
// WiFiClient client;

#define SERVO_1 D1
#define SERVO_2 D2
#define LIGHTS D3
#define SPEAKER D4
// ThingSpeak Settings

unsigned long myChannelNumber = you_channel_number; // Your ThingSpeak Channel number
const char * myWriteAPIKey = "yourWriteAPIkey";//change to yours
const char * myReadAPIKey = "yourREADAPIkey";//change to yours
const int servo_ON=120; // angle 1
const int servo_OFF=50; // angle 2
unsigned int code_FieldNumber = 1;// the field to read from
WiFiClient client;
void setup() {
  Serial.begin(9600);
  delay(50);
  // Wifi Settings
  wifiMulti.addAP("Name_1","Password_1");//you can add/ remove an AP
  wifiMulti.addAP("Name_2","Password_1");
  Serial.println();
  Serial.print("Starting...\n");
  myservo_servo_1.attach(D1);
  myservo_servo_2.attach(D2);
  pinMode(LIGHTS, OUTPUT);
  // We start by connecting to a WiFi network
  
  Serial.print("Connecting to ");

  
  while (wifiMulti.run() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
         
  }
  Serial.println("WiFi connected");
  Serial.println("");
  
  
  
  pinMode(LED_BUILTIN, OUTPUT); // Initialize the LED_BUILTIN pin as an output
  
  ResetDevices();
  // Initialize ThingSpeak
 
  ThingSpeak.begin(client);
  ResetCode();
 randomSeed(analogRead(0));
 tone2(SPEAKER,600 ,700);
}


void loop() {
  if(wifiMulti.run() != WL_CONNECTED)
  {
    Serial.println(".");
    delay(500);
  }
  else{
    
  // Wait a few seconds between measurements. 
  delay(100);
  
  int statusCode=0;
  
  int code = ThingSpeak.readIntField(myChannelNumber, code_FieldNumber,myReadAPIKey);
  statusCode = ThingSpeak.getLastReadStatus();
  
  Serial.println("Code:" );
  Serial.println(String(code));
  int i=0,temp=code;
  int code_arr[5]={1,0,0,0,0};
  for (i=0;i<4;i++)
  {
    code_arr[4-i]=temp%10;
    temp=temp/10;
  }
  
  if(statusCode == 200){ 
     UpdateDevices(code_arr);
  }
  else{
     Serial.println("Problem reading channel. HTTP error code " + String(statusCode)); 
   }
}
}
 /**********************************/

// Functions
void ResetDevices()
{
  Serial.println("Resetting Devices...");
  myservo_servo_1.write(servo_OFF);
  myservo_servo_2.write(servo_OFF);
  digitalWrite(LIGHTS, LOW);
}

void ResetCode()
{
  int code_init=10000;
ThingSpeak.setField(1,code_init);
ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey); 
delay(1000);

  
}

void UpdateDevices(int code_arr[]){

//check SERVO_1 status
  if(code_arr[1]==0)
  {
    Serial.println("SERVO_1 is off ");
    myservo_servo_1.write(servo_OFF); 
  }
  else if(code_arr[1]==1)
    {
    Serial.println("SERVO_1 is on ");
    myservo_servo_1.write(servo_ON); 
      }
  else{
    Serial.println("problem in SERVO_1");
  } 
 
//check SERVO_2 status
  if(code_arr[2]==0)
  {
    Serial.println("SERVO_2 is off ");
     myservo_servo_2.write(servo_OFF); 
    
  }
  else if(code_arr[2]==1)
    {
    Serial.println("SERVO_2 is on ");
    myservo_servo_2.write(servo_ON);
     
    }
  else{
    Serial.println("problem in SERVO_2");
  }
  //check LIGHTS status
  if(code_arr[3]==0)
  {
    Serial.println("Lights Off");
    digitalWrite(LIGHTS, LOW);
  }
  else if(code_arr[3]==1)
    {
    Serial.println("Lights On");
   digitalWrite(LIGHTS, HIGH);
      }
  else{
    Serial.println("problem in Lights");
  }
 //check MUSIC status
  if(code_arr[4]==0)
  {
    Serial.println("Music Off ");
   
  }
  else if(code_arr[4]==1)
    {
    Serial.println("Music On ");
    playMusic();
    }
  else{
    Serial.println("problem in Music");
  } 
}

// Print Wifi Status

void printWifiStatus() {
// print the SSID of the network you're attached to:
Serial.print("SSID: ");
Serial.println(WiFi.SSID());

// print your WiFi shield's IP address:
IPAddress ip = WiFi.localIP();
Serial.print("IP Address: ");
Serial.println(ip);

// print the received signal strength:
long rssi = WiFi.RSSI();
Serial.print("signal strength (RSSI):");
Serial.print(rssi);
Serial.println(" dBm");
}
//////////////////////////////////////////////////////////////////////////////////
void playMusic()
{
 
long  randNumber=random(0,4);
 Serial.println(randNumber);
  if (randNumber==0||randNumber==2)
    {
      POTC();
    }
  if (randNumber==1||randNumber==3||randNumber==4)
  {
    Despacito();
    delay(200);
    Despacito();
  }

}
///////////////////////////////////////////////////
void tone2(uint8_t _pin, unsigned int frequency, unsigned long duration) {
  pinMode (_pin, OUTPUT );
  analogWriteFreq(frequency);
  analogWrite(_pin,500);
  delay(duration);
  analogWrite(_pin,0);
}
///////////////////////////////////////////////////
void POTC()
{ 
  int notes[] = {       //Note of the song, 0 is a rest/pulse
   NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0, 
   NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0, 
   NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
   NOTE_A4, NOTE_G4, NOTE_A4, 0,
   
   NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0, 
   NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0, 
   NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
   NOTE_A4, NOTE_G4, NOTE_A4, 0,
   
   NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0, 
   NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, 0, 
   NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, 0,
   NOTE_E5, NOTE_D5, NOTE_E5, NOTE_A4, 0,
   
   NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0, 
   NOTE_D5, NOTE_E5, NOTE_A4, 0, 
   NOTE_A4, NOTE_C5, NOTE_B4, NOTE_B4, 0,
   NOTE_C5, NOTE_A4, NOTE_B4, 0,

   
   NOTE_E5, 0, 0, NOTE_F5, 0, 0,
   NOTE_E5, NOTE_E5, 0, NOTE_G5, 0, NOTE_E5, NOTE_D5, 0, 0,
   NOTE_D5, 0, 0, NOTE_C5, 0, 0,
   NOTE_B4, NOTE_C5, 0, NOTE_B4, 0, NOTE_A4,

   
};
//*************
int duration[] = {         //duration of each note (in ms) Quarter Note is set to 250 ms
  125, 125, 250, 125, 125, 
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 125, 
  
  125, 125, 250, 125, 125, 
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 125, 
  
  125, 125, 250, 125, 125, 
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 125, 250, 125,

  125, 125, 250, 125, 125, 
  250, 125, 250, 125, 
  125, 125, 250, 125, 125,
  125, 125, 375, 375,
  
  250, 125, 375, 250, 125, 375,
  125, 125, 125, 125, 125, 125, 125, 125, 375,
  250, 125, 375, 250, 125, 375,
  125, 125, 125, 125, 125, 500,

  
  };
   float songspeed = 0.8;
   Serial.print("Pirates of the carribian\n");
   for (int i=0;i<(sizeof(duration))/sizeof(int);i++)
   {              //203 is the total number of music notes in the song
    float wait = duration[i] * songspeed;
    tone2(SPEAKER,notes[i],wait);          //tone(pin,frequency,duration)
    delay(wait);}  
}

//////////////////////
void Despacito(){
 int negra=400;
  Serial.print("Despacito\n");
     notars(negra, NOTE_D5);
     notars(negra, NOTE_CS5);   
// (1)
     notars(negra/2, NOTE_B4);
     notars(negra/4, NOTE_FS4);
// (2)
     delay(negra/4);
// (3)
     notars(negra/4, NOTE_FS4);
     notars(negra/4, NOTE_FS4);
// (4)
     notars(negra/4, NOTE_FS4);
     notars(negra/4, NOTE_FS4);  //  (1) 
     notars(negra/4, NOTE_FS4);
     notars(negra/4, NOTE_B4);
     notars(negra/4, NOTE_B4);
//  (2) 
     notars(negra/4, NOTE_B4);
//  (3)  
     notars(negra/2, NOTE_B4);
//  (4)   
     notars(negra/4, NOTE_A4);            
     notars(negra/4, NOTE_B4);
//  (1) 
     delay(negra/4); 
     delay(negra/4); 
     notars(negra/4, NOTE_G4);
//  (2) 
     delay(negra/4);
//  (3) 
//......
     notars(negra/4, NOTE_G4);  
     notars(negra/4, NOTE_G4);
//  (4)   
     notars(negra/4, NOTE_G4);  
     notars(negra/4, NOTE_G4);
//  (1)   
     notars(negra/4, NOTE_G4);  
     notars(negra/4, NOTE_B4);  
     notars(negra/4, NOTE_B4);
//  (2) 
     notars(negra/4, NOTE_B4);
//  (3) 
     notars(negra/2, NOTE_B4);
//  (4)      
     notars(negra/4, NOTE_CS5);    
     notars(negra/4, NOTE_D5);
 //  (1) 
     delay(negra/4);
     delay(negra/4);
     notars(negra/4, NOTE_A4);
//  (2)      
     delay(negra/4);
//  (3)      
     notars(negra/4, NOTE_A4);     
     notars(negra/4, NOTE_A4);
     
//  (4)           
     notars(negra/4, NOTE_A4);               
     notars(negra/4, NOTE_A4);
//  (1)                     
     notars(negra/4, NOTE_D5);          
     notars(negra/4, NOTE_CS5);               
     notars(negra/4, NOTE_D5);
//  (2)           
     notars(negra/4, NOTE_CS5);
//  (3)          
     notars(negra/2, NOTE_D5);
//  (4)                    
     notars(negra/4, NOTE_E5);    
     notars_jai(negra/2, NOTE_E5,negra/4);  // (1)
//     notars(negra/2, NOTE_E5);  

    //-----------------------------------------------
    
    delay(negra/8);
      tone2(SPEAKER,NOTE_CS5 ,negra*3/4);    
  Serial.print("end...\n");
  delay(negra/4);
}
////////////////////////////////////////
void notars(int noteDuration,int LaNota ){

    //noteDuration=negra/4;
  tone2(SPEAKER,LaNota ,noteDuration*0.9);
//      pauseBetweenNotes = noteDuration * 1.30;
      pauseBetweenNotes = noteDuration * 1.1;
      delay(pauseBetweenNotes);
      delay(3);  


}
void notars_jai(int noteDuration,int LaNota ,int Desfase){

    //noteDuration=negra/4;
  tone2(SPEAKER,LaNota ,noteDuration*0.9);
//      pauseBetweenNotes = noteDuration * 1.30;
      pauseBetweenNotes = noteDuration * 1.1;
      delay(Desfase);
      //  (1)   
      
        delay(pauseBetweenNotes-Desfase);
      noTone(3);  


}


/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/ 
/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/
