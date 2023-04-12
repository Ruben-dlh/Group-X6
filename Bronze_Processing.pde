// Import the network library and the ControlP5 library
import processing.net.*;
import controlP5.*;

// Create ControlP5 and font objects and message variable
ControlP5 cp5;
PFont font;
String message;

// Create a client object to connect to the Arduino
Client arduino;

void setup() {
  // Create the canvas
  size(600, 300);
  
  // Initialize the ControlP5 object and create a connection to the Arduino using the IP address and port number
  cp5 = new ControlP5(this);
  arduino = new Client(this, "192.168.4.1", 5200);
  
  // Create a font for the buttons
  font = createFont("arial", 40);
  
  // Add two buttons for turning the Arduino ON and OFF
  cp5.addButton("ON")
     .setPosition(50, 130)
     .setSize(240, 140)
     .setFont(font);
  cp5.addButton("OFF")
     .setPosition(300, 130)
     .setSize(240, 140)
     .setFont(font);
}

void draw() {
  // Set the background color to black
  background(0);
  
  // Set the text size and display the text "Arduino Control Pad"
  textSize(40);
  text("Arduino Control Pad", 130, 100);
  
  // Read the message from the Arduino
  message = arduino.readString();
  
  // If the message is not null, print it to the console
  if (message != null) {
    println(message);
  }
}

// Function for the ON button
void ON() {
  // Send the value 1 to the Arduino
  arduino.write(1);
  // Print a message to the console
  println("Arduino ON");
}

// Function for the OFF button
void OFF() {
  // Send the value 2 to the Arduino
  arduino.write(2);
  // Print a message to the console
  println("Arduino OFF");
}
