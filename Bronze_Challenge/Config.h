#include <WiFiNINA.h>   // include the WiFiNINA library for using WiFi

char ssid[] = "X6";   // WiFi network name
char password[] = "2E10PROJECT";   // WiFi network password
WiFiServer server(5200);

const int LEYE = 4;   // left light sensor pin
const int REYE = 8;   // right light sensor pin

// RIght connections
const int enA = A2;   // enable pin for motor A
const int in1 = A0;   // input 1 pin for motor A
const int in2 = A1;   // input 2 pin for motor A

// Motor B connections
const int enB = A3;   // enable pin for motor B
const int in3 = A6;   // input 3 pin for motor B
const int in4 = A5;   // input 4 pin for motor B

bool OldState_L = false;   // previous state of left light sensor
bool NewState_L = false;   // current state of left light sensor
bool OldState_R = false;   // previous state of right light sensor
bool NewState_R = false;   // current state of right light sensor

//ultrasonic sensor connections
const int trigPin = 6;   // trigger pin for ultrasonic sensor
const int echoPin = 5;   // echo pin for ultrasonic sensor

int distance;   // previous distance reading from ultrasonic sensor
int distance2 = 0;   // current distance reading from ultrasonic sensor
