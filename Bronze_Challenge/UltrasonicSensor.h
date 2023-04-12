int new_distance(){
  // measure the distance using the ultrasonic sensor and save the value to a global variable
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  long new_duration = pulseIn(echoPin, HIGH);
  distance2 = (new_duration * 0.0343) / 2;
  delay(50);
}


void UltrasonicMotorControl()
{
    new_distance();

    if(distance2 <= 20){
    while(distance2 <=20){
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
  if(distance2 != distance){
    // print a warning message if the distance has changed since the last iteration
      Serial.println("Stopping for obstacle at " + String(distance2) + "cm distance");
  }
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);

    digitalWrite(trigPin, LOW);

    long new_duration = pulseIn(echoPin, HIGH);
    distance2 = (new_duration * 0.0343) / 2;
  }
  }
  // save the current distance for comparison in the next iteration
  distance = distance2;
}

