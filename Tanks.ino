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
#define L_MOTOR 1
#define R_MOTOR 2

//Pipe address
const uint64_t pipeIn =  0xB00B1E5000LL;

//Pins for the radio
RF24 radio(7, 8);

/* Struct to hold variables from receiver
The size of this struct should not exceed 32 bytes */
struct packetdata {
  byte L_UD; //Left stick Up-Down
  byte L_LR; //Left stick Left-Right
  byte R_UD; //Right stick Up-Down
  byte L_LR; //Right stick Left-Right
  byte dial1; //Left dial
  byte dial2; // Right dial
  byte switch1; //Left switch
  byte switch2; //Right switch
};

//Data object
packetdata data;

//Left ESC servo object
Servo left_servo;
//Right ESC servo object
Servo right_servo;

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
  left_servo.attach(L_MOTOR);
  left_servo.attach(R_MOTOR);

  //Initialize motors, so they don't spin around.
  motor1.drive(0);
  motor2.drive(0);
}

void loop() {

  if ( radio.available() ) {
  radio.read(&data, sizeof(packetdata));

  //DRIVE
  //FORWARD
  if (data.throttle > 125) {

  int mapd_forward = map(data.throttle, 125, 255, 0, 255);
  motor1.drive(mapd_forward);

    delay(10);
  }else if (data.throttle >= 125){
  motor1.drive(0);
  }
 delay(10);

  //BACKWARD
  if (data.throttle < 118) {
  int mapd_backward = map(data.throttle, 118, 0, 0, -255);
  motor1.drive(mapd_backward);
  }

  //FORKLIFT ARMS
  //LIFT
  if(data.pitch > 140) {

    motor2.drive(data.pitch/lSpeed);

  }else if (data.pitch >= 140){
   motor2.drive(0);
  }else{
  motor2.drive(0);
  }
  //LOWER
 if (data.pitch < 135) {
  int mapped = map(data.pitch, 135, 0, 0, -255);

  motor2.drive(mapped/lSpeed);
  }

  //SERVO
  //Map steering input to fit servo
  int degree = map(data.roll, 0, 255, 91, 69);
  //Write the mapped data to the servo
  steering.write(degree);

  }else{
  //Reset truck to zero pos.
  motor1.drive(0);
  motor2.drive(0);
  steering.write(88);
  }
