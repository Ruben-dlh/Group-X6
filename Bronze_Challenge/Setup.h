void setup() {
  Serial.begin(9600);   // initialize serial communication at 9600 bits per second

  //PinMode for light sensors
  pinMode( LEYE, INPUT );   // set left light sensor pin as input
  pinMode( REYE, INPUT );   // set right light sensor pin as input

  //PinMode for DC Motor Control
  pinMode(enA, OUTPUT);   // set enable pin for motor A as output
  pinMode(enB, OUTPUT);   // set enable pin for motor B as output
  pinMode(in1, OUTPUT);   // set input 1 pin for motor A as output
  pinMode(in2, OUTPUT);   // set input 2 pin for motor A as output
  pinMode(in3, OUTPUT);   // set input 3 pin for motor B as output
  pinMode(in4, OUTPUT);   // set input 4 pin for motor B as output

  //PinMode for Ultrasonic sensor
  pinMode(trigPin, OUTPUT);   // set trigger pin for ultrasonic sensor as output
  pinMode(echoPin, INPUT);   // set echo pin for ultrasonic sensor as input

  // Initialize motors off
  digitalWrite(in1, LOW);   // set input 1 pin for motor A as low (off)
  digitalWrite(in2, LOW);   // set input 2 pin for motor A as low (off)
  digitalWrite(in3, LOW);   // set input 3 pin for motor B as low (off)
  digitalWrite(in4, LOW);   // set input 4 pin for motor B as low (off)

  // Connect to WiFi network
  WiFi.beginAP(ssid, password);   // begin WiFi connection with given network name and password
  IPAddress ip = WiFi.localIP();   // get the local IP address of the WiFi network
  Serial.println(ip);   // print the local IP address to the serial monitor
  server.begin();
}