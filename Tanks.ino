/*
3D printed tanks 2020
Main reveiver sketch

29.03.2020
Marcus Eide
*/

//Libraries
//Servo library
#include <Servo.h>
//SPI library
#include <SPI.h>
//nRF24L01 libraries
#include <nRF24L01.h>
#include <RF24.h>
//printf.h sketch for printing out nRF24l01 information
#include "printf.h"

//Servo pinout
#define L_MOTOR 5
#define R_MOTOR 6

//Radio pinout
#define RADIO_1 7
#define RADIO_2 8

//Pipe address
const uint64_t pipeIn =  0xB00B1E5000LL;

//Define radio
RF24 radio(RADIO_1, RADIO_2);

/* Struct to hold variables from receiver
The size of this struct should not exceed 32 bytes */
struct packetdata {
  byte L_UD; //Left stick Up-Down
  byte L_LR; //Left stick Left-Right
  byte R_UD; //Right stick Up-Down
  byte R_LR; //Right stick Left-Right
  byte dial1; //Left dial
  byte dial2; // Right dial
  byte switch1; //Left switch
  byte switch2; //Right switch
};

//Data object
packetdata data;

//Servo ESC objects
Servo left_motor;
Servo right_motor;

void setup()
{
   Serial.begin(9600);
   printf_begin();

  // Set up radio module
  radio.begin();
  radio.setDataRate(RF24_250KBPS); // Both endpoints must have this set the same
  radio.setAutoAck(false);
  radio.openReadingPipe(1,pipeIn);
  radio.startListening();
  radio.printDetails();

  //Attach servos
  left_motor.attach(L_MOTOR);
  right_motor.attach(R_MOTOR);
  // Reset values to zero
  data.L_UD = 90;
  data.L_LR = 90;
  delay(5000); // Wait for ESC to boot
}

void loop() {

  if ( radio.available() ) {
  radio.read(&data, sizeof(packetdata));



  //DRIVE
  //LEFT FORWARD
  if (data.L_UD > 125) {
    int map_L_U = map(data.L_UD, 125, 237, 97, 130);
    left_motor.write(map_L_U);

Serial.println("FWD");
Serial.println(map_L_U);
  }

//delay(10);


   //LEFT BACKWARD
     if (data.L_UD < 118) {
     int map_L_D = map(data.L_UD, 118, 0, 90, 50);
     left_motor.write(map_L_D);
Serial.println("BWD");
Serial.println(map_L_D);
     }


     //RIGHT FORWARD
     if (data.R_UD > 125) {
       int map_R_U = map(data.R_UD, 125, 255, 0, 255);
       left_motor.write(map_R_U);

   delay(10);
 }else if (data.R_UD >= 125){
       right_motor.write(0);
       }
   delay(10);

      //RIGHT BACKWARD
        if (data.R_UD < 118) {
        int map_R_D = map(data.R_UD, 118, 0, 0, -255);
        right_motor.write(map_R_D);
        }

  }
}
