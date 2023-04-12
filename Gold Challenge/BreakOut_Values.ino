
#include <WiFiNINA.h>   // include the WiFiNINA library for using WiFi
#include <Arduino_LSM6DS3.h>

char ssid[] = "X6";   // WiFi network name
char password[] = "2E10PROJECT";   // WiFi network password
WiFiServer server(5200);

float x, y, z;

void setup() {
  Serial.begin(9600);
  

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");

    while (1);
  }

  WiFi.beginAP(ssid, password);   // begin WiFi connection with given network name and password
  IPAddress ip = WiFi.localIP();   // get the local IP address of the WiFi network
  Serial.println(ip);   // print the local IP address to the serial monitor
  server.begin();

}

void loop() {

  WiFiClient client = server.available();
  int val = client.read();


 

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);

    Serial.println(x);
  }

  // While the client is connected...
  while (client.connected())
  {

    client.print(x);

    val = client.read();
  

  }
  
}
