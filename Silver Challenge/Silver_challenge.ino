#include <WiFiNINA.h>   // include the WiFiNINA library for using WiFi
#include <PID_v1.h>

#define ENCODEROUTPUT 8

char ssid[] = "X6";   // WiFi network name
char password[] = "2E10PROJECT";   // WiFi network password
WiFiServer server(5200);

//PID constants
double Kp = 10;
double Ki = 4;
double Kd = 0.02;

double setpoint, distance, oldDistance, speed, safety_distance, object_velocity, relative_object_speed, object_speed;

PID myPID(&distance, &speed, &setpoint, Kp, Ki, Kd, DIRECT);


const int LEYE = 4;   // left light sensor pin
const int REYE = 8;   // right light sensor pin

const int encoder_A = 9; // Encoder_A output pin

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

float ang_velocity = 0;
float buggy_linear_velocity = 0;
int rpm = 0;

const float rpm_to_radians = 0.10471975512;

volatile long encoderValue = 0;

int interval = 1000;
long previousMillis = 0;
long currentMillis = 0;
long currentMillis2 = 0;
long previousMillis2 = 0;
float deltaT = 0;


void setup() {
  // put your setup code here, to run once:
   // put your setup code here, to run once:
  Serial.begin(9600);

  //PinMode for light sensors
  pinMode( LEYE, INPUT );   // set left light sensor pin as input
  pinMode( REYE, INPUT );   // set right light sensor pin as input

  pinMode(enA, OUTPUT);   // set enable pin for motor A as output
  pinMode(enB, OUTPUT);   // set enable pin for motor B as output
  pinMode(in1, OUTPUT);   // set input 1 pin for motor A as output
  pinMode(in2, OUTPUT);   // set input 2 pin for motor A as output
  pinMode(in3, OUTPUT);   // set input 3 pin for motor B as output
  pinMode(in4, OUTPUT);   // set input 4 pin for motor B as output

  //PinMode for Ultrasonic sensor
  pinMode(trigPin, OUTPUT);   // set trigger pin for ultrasonic sensor as output
  pinMode(echoPin, INPUT);   // set echo pin for ultrasonic sensor as input

  pinMode(encoder_A , INPUT_PULLUP);
  EncoderInit();//Initialize the module

  // Initialize motors off
  digitalWrite(in1, LOW);   // set input 1 pin for motor A as low (off)
  digitalWrite(in2, LOW);   // set input 2 pin for motor A as low (off)
  digitalWrite(in3, LOW);   // set input 3 pin for motor B as low (off)
  digitalWrite(in4, LOW);   // set input 4 pin for motor B as low (off)

  //distance = measure_distance();
  setpoint = -25;
  myPID.SetMode(AUTOMATIC);

  encoderValue = 0;
  previousMillis = millis();

   // Connect to WiFi network
  WiFi.beginAP(ssid, password);   // begin WiFi connection with given network name and password
  IPAddress ip = WiFi.localIP();   // get the local IP address of the WiFi network
  Serial.println(ip);   // print the local IP address to the serial monitor
  server.begin();
}


void loop() {
  // put your main code here, to run repeatedly:
  distance = -measure_distance();

  myPID.Compute();

  infrared_sensor();
  InfraredMotorControl();
  buggy_velocity();
  object_velocity_calc();


  Serial.print("Linear Velocity: ");
  Serial.print(buggy_linear_velocity);
  Serial.print(" Object relative Velocity: ");
  Serial.println(relative_object_speed);

  if(-distance < 20)
  {
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        digitalWrite(in3, LOW);
        digitalWrite(in4, LOW);
  }



  // Wait for a client to connect to the WiFi server
  WiFiClient client = server.available();
  int val = client.read();

  // While the client is connected...
  while (client.connected())
  {
    // If the client sends a "1" command, send obstacle detection messages until a "2" command is received
    if(val==1)
    {
      while(val != 2)
      {
        distance = -measure_distance();

        myPID.Compute();

        infrared_sensor();
        InfraredMotorControl();
        buggy_velocity();
        object_velocity_calc();

        if(-distance < 20)
        {
          digitalWrite(in1, LOW);
          digitalWrite(in2, LOW);
          digitalWrite(in3, LOW);
          digitalWrite(in4, LOW);
        }

          client.print(String(buggy_linear_velocity) + "," + String(object_speed) + "," + String(-distance));
          
          val = client.read();
      }
    }
    // If the client sends a "2" command, stop the robot until a "1" command is received
    else if(val==2)
    {
      while(val != 1)
      {
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        digitalWrite(in3, LOW);
        digitalWrite(in4, LOW);
        val = client.read();
      }     
    }
  }
}
  



void infrared_sensor() {

    NewState_L = digitalRead(LEYE);
    NewState_R = digitalRead(REYE); 
}



void InfraredMotorControl() {

  // Controls Right motor based on sensor readings
  if (NewState_R == HIGH ) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enA, speed*0.75);
  } else if (NewState_R == LOW) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);   
  }
  //controls Left motor based on sensor readings
  if (NewState_L == HIGH) {
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(enB, speed*0.75);
  } else if (NewState_L == LOW) {
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
  }
}



double measure_distance() 
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  double distance = duration * 0.0343 / 2;

  return distance;
}


void buggy_velocity() {

  // Update RPM value on every second
  currentMillis = millis();
  if (currentMillis - previousMillis > interval)
  {
    
    previousMillis = currentMillis;
    
    // Revolutions per minute (RPM) =
    // (total encoder pulse in 1s / motor encoder output) x 60s
    rpm = (float)(encoderValue * 60 / ENCODEROUTPUT);
    ang_velocity = rpm * rpm_to_radians;
    buggy_linear_velocity = (ang_velocity * 0.034)/60;

    
    encoderValue = 0;  
  }
}

void object_velocity_calc()
{
    currentMillis2 = millis();
  if (currentMillis2- previousMillis2 >= 500){
   deltaT = (currentMillis2 - previousMillis2) / 1000;
   object_velocity = (distance - oldDistance)/0.5 ;
   previousMillis2 = currentMillis2;
   oldDistance = distance;
  }

  relative_object_speed = (buggy_linear_velocity + object_velocity)/100;


    if (relative_object_speed<0){
      relative_object_speed = 0;
    }

    object_speed = relative_object_speed;
}



void EncoderInit()
{
 // Attach interrupt at hall sensor A on each rising signal
  attachInterrupt(digitalPinToInterrupt(encoder_A), updateEncoder, RISING);
}


void updateEncoder()
{
  // Add encoderValue by 1, each time it detects rising signal
  // from encoder A
  encoderValue++;
}