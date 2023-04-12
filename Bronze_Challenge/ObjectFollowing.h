//PID constants
double Kp = 0.07;
double Ki = 0.0008;
double Kd = 0.6;

//error and time variables for PID control
int P;
int I;
int D;

int last_error = 0;

const int max_speed = 255;
const int base_speed = 170;

//setpoints for distance from object
double setpoint = 30.0; //the distance the buggy should maintain from the object


void ObjectFollowing() {
  //measure the distance to the object
  double distance = measure_distance();

  //calculate the error for the PID control
  double error = setpoint - distance;

  //calculate the integral and derivative terms for the PID control
  P = error;
  I = I + error;
  D = error - last_error;
  last_error = error;
  int motor_speed = P*Kp + I*Ki + D*Kd;

  //calculate the motor speed using PID control
  int corrected_speed = base_speed + motor_speed;

  //adjust the motor speed based on the distance to the object
  if (distance < 20) {
    //stop the motors if the buggy is too close to the object
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    analogWrite(enA, 0);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    analogWrite(enB, 0);
  }
  else if(corrected_speed > max_speed)
  {
    corrected_speed = max_speed;

    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enA, corrected_speed);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(enB, corrected_speed);
  }
}

double measure_distance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  double distance = duration * 0.034 / 2;

  return distance;
}


