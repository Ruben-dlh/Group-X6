void infrared_sensor() {

    NewState_L = digitalRead(LEYE);
    NewState_R = digitalRead(REYE);

  // check if the state of either sensor has changed
  if (NewState_L != OldState_L || NewState_R != OldState_R) {
    // print the current state of the sensors
    if (NewState_L == HIGH && NewState_R == HIGH) {
      Serial.println("high  high");
    } else if (NewState_L == HIGH && NewState_R == LOW) {
      Serial.println("high  low");
    } else if (NewState_L == LOW && NewState_R == HIGH) {
      Serial.println("low  high");
    } else if (NewState_L == LOW && NewState_R == LOW) {
      Serial.println("low  low");
    }
  }
  // save the current state of the sensors for comparison in the next iteration
  OldState_L = NewState_L;
  OldState_R = NewState_R;  
}

void InfraredMotorControl() {

  // Controls Right motor based on sensor readings
  if (NewState_R == HIGH ) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enA, 240);
  } else if (NewState_R == LOW) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    analogWrite(enA, 100);
  }
  //controls Left motor based on sensor readings
  if (NewState_L == HIGH) {
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(enB, 240);
  } else if (NewState_L == LOW) {
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    analogWrite(enB, 100);
  }
}


