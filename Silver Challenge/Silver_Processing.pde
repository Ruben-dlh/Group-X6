// Import the network library and the ControlP5 library
import processing.net.*;
import controlP5.*;

// Create ControlP5 and font objects and message variable
ControlP5 cp5;
PFont font;
String message;

Slider buggyVelSlider;
Slider objRelVelSlider;
Slider distanceSlider;

// Create a client object to connect to the Arduino
Client arduino;

void setup() {
  // Create the canvas
  size(800, 500);
  
  // Initialize the ControlP5 object and create a connection to the Arduino using the IP address and port number
  cp5 = new ControlP5(this);
  arduino = new Client(this, "192.168.4.1", 5200);
  
  // Create a font for the buttons
  font = createFont("arial", 40);
  
  // Add two buttons for turning the Arduino ON and OFF
  cp5.addButton("ON")
     .setPosition(150, 110)
     .setSize(240, 140)
     .setFont(font);
  cp5.addButton("OFF")
     .setPosition(400, 110)
     .setSize(240, 140)
     .setFont(font);
     
     // Add a slider for linear velocity
  buggyVelSlider = cp5.addSlider("Buggy Linear Velocity (m/s)")
     .setPosition(150, 370)
     .setSize(500, 50)
     .setRange(0, 1)
     .setValue(0);
     
     buggyVelSlider.getCaptionLabel().toUpperCase(true).setFont(font).setSize(15).align(ControlP5.TOP, ControlP5.LEFT).setPaddingX(0);
     buggyVelSlider.getValueLabel().toUpperCase(true).setFont(font).setSize(20);
     
     
  // Add a slider for object relative velocity
 objRelVelSlider = cp5.addSlider("Object Relative Velocity (m/s)")
     .setPosition(150, 440)
     .setSize(500, 50)
     .setRange(0, 1)
     .setValue(0);
     
     objRelVelSlider.getCaptionLabel().toUpperCase(true).setFont(font).setSize(15).align(ControlP5.TOP, ControlP5.LEFT).setPaddingX(0);
     objRelVelSlider.getValueLabel().toUpperCase(true).setFont(font).setSize(20);
     
  distanceSlider = cp5.addSlider("Object Distance (m)")
     .setPosition(150, 300)
     .setSize(500, 50)
     .setRange(0, 100)
     .setValue(0);
     
     distanceSlider.getCaptionLabel().toUpperCase(true).setFont(font).setSize(15).align(ControlP5.TOP, ControlP5.LEFT).setPaddingX(0);
     distanceSlider.getValueLabel().toUpperCase(true).setFont(font).setSize(20);
}

void draw() {
  // Set the background color to black
  background(0);
  
  // Set the text size and display the text "Arduino Control Pad"
  textSize(60);
  text("Arduino Control Pad", 142, 80);
  
  // Read the message from the Arduino
  message = arduino.readString();
  
  String[] a = split(message, ',');
  
  // If the message is not null, print it to the console
if (message != null) {
    
    String linearVelocity = a[0];
    String objectRelativeVelocity = a[1];
    String distance = a[2];
    
    print("Buggy Velocity: " + linearVelocity + " m/s   ");
    print("Object relative velocity: " + objectRelativeVelocity.substring(0, 4) + " m/s    ");
    print("Distance: " + distance.substring(0, 4) + " cm");
    println();
    
    float linvel = Float.parseFloat(linearVelocity);
    float objvel = Float.parseFloat(objectRelativeVelocity.substring(0, 4));
    float dist = Float.parseFloat(distance.substring(0, 4));
    
    buggyVelSlider.setValue(linvel);
    objRelVelSlider.setValue(objvel);
    distanceSlider.setValue((dist)/100);
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
