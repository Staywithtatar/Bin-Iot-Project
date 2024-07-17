#include <Servo.h> // Include the Servo library
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const int trigPin = D1;    // Trig pin of the Ultrasonic Sensor
const int echoPin = D2;    // Echo pin of the Ultrasonic Sensor
const int servoPin = D4;   // Pin connected to the Servo Motor
const int redPin = D7;     // Pin for the red LED
const int greenPin = D8;   // Pin for the green LED

Servo myServo;  // Servo Motor object

long duration;
int distance;

// Add Line Notify library
#include <TridentTD_LineNotify.h>

#define SSID        "Kuri"                                     // Insert your WiFi SSID
#define PASSWORD    "55555555"                                 // Insert your WiFi password
#define LINE_TOKEN  "Xx2MBPpo3kO6S7y3aeIkYbSgOjEPFXoudkFR8j6DG4I" // Insert your Line Notify token

void setup() {
  Serial.begin(9600);
  Serial.println(LINE.getVersion());

  // Connect to WiFi
  WiFi.begin(SSID, PASSWORD);
  Serial.printf("WiFi connecting to %s\n",  SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(400);
  }
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP());

  // Initialize Line Notify with token
  LINE.setToken(LINE_TOKEN);

  // Initialize pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  myServo.attach(servoPin);
}

void loop() {
  // Send Ultrasonic signal to measure distance
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH); // Receive the time taken for the signal to return
  distance = duration * 0.034 / 2;   // Calculate distance in centimeters

  // Check the distance
  if (distance < 20) {
    openLid();  // Open the trash can lid
    Serial.println("Lid opened."); // Print message to Serial Monitor indicating the lid is open
    digitalWrite(redPin, LOW); // Turn off the red LED
    digitalWrite(greenPin, HIGH); // Turn on the green LED
    sendLineNotification("Trash can lid is open."); // Send Line Notify notification
    sendStatusToServer(true); // Send lid status to server
  } else {
    closeLid(); // Close the trash can lid
    Serial.println("Lid closed."); // Print message to Serial Monitor indicating the lid is closed
    digitalWrite(redPin, HIGH); // Turn on the red LED
    digitalWrite(greenPin, LOW); // Turn off the green LED
    sendLineNotification("Trash can lid is closed."); // Send Line Notify notification
    sendStatusToServer(false); // Send lid status to server
  }
  delay(100); // Delay for stability
}

// Function to open the trash can lid
void openLid() {
  myServo.write(180); // Rotate Servo Motor to open the lid
  delay(100); // Wait for the lid to fully open
}

// Function to close the trash can lid
void closeLid() {
  myServo.write(0); // Rotate Servo Motor to close the lid
  delay(100); // Wait for the lid to fully close
}

// Function to send Line Notify notification
void sendLineNotification(String message) {
  LINE.notify(message);
}

// Function to send lid status to server
void sendStatusToServer(bool lidOpen) {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;
    http.begin(client, "http://192.168.81.44/workhard/distance.php"); // Change URL accordingly
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpCode = http.POST("lidOpen=" + String(lidOpen) + "&time=" + getTimeStamp()); // Send lid status and time value
    Serial.printf("HTTP Response code: %d\n", httpCode);
    http.end();
  } else {
    Serial.println("WiFi Disconnected. Unable to send HTTP request.");
  }
  
}
String getTimeStamp() {
  unsigned long currentMillis = millis();
  unsigned long seconds = currentMillis / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  unsigned long days = hours / 24;
  
  String timeStamp = String(days) + "-" + String(hours % 24) + ":" + String(minutes % 60) + ":" + String(seconds % 60);
  return timeStamp;
}