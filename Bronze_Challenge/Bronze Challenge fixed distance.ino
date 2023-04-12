//Main Code
#include "Config.h"
#include "InfraredSensor.h"
#include "UltrasonicSensor.h"
#include "Setup.h"

void loop() 
{
  // Determine if the eye states have changed since last time
  infrared_sensor();
  // Adjust the motor speeds based on eye sensor states and distance readings
  InfraredMotorControl();
  UltrasonicMotorControl();

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
        infrared_sensor();
        InfraredMotorControl();
        new_distance();

          if(distance2 <= 20)
          {
            while(distance2 <=20)
            {
              digitalWrite(in1, LOW);
              digitalWrite(in2, LOW);
              digitalWrite(in3, LOW);
              digitalWrite(in4, LOW);
              
              if(distance2 != distance)
              {
                // print a warning message if the distance has changed since the last iteration
                client.println("Stopping for obstacle at " + String(distance2) + "cm distance");
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