#include <Servo.h>

Servo myservo;

// IR Sensor Pins
#define ir_enter 2
#define ir_back  4
#define ir_car1 5
#define ir_car2 6
#define ir_car3 7
#define ir_car4 8
#define ir_car5 9

// LED Pins
#define greenLED 11
#define redLED 10 // Updated red LED pin to 10

// Variables to track parking slot status
int S1 = 0, S2 = 0, S3 = 0, S4 = 0, S5 = 0;
int slot = 5;  // Total number of slots

void setup() {
  Serial.begin(9600);

  // Configure IR sensor pins
  pinMode(ir_car1, INPUT);
  pinMode(ir_car2, INPUT);
  pinMode(ir_car3, INPUT);
  pinMode(ir_car4, INPUT);
  pinMode(ir_car5, INPUT);
  pinMode(ir_enter, INPUT);
  pinMode(ir_back, INPUT);

  // Configure LED pins
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  // Initialize servo
  myservo.attach(3);
  myservo.write(90); // Initial gate position

  // Initialize LEDs
  Read_Sensor();
  updateLEDs();
}

void loop() {
  Read_Sensor();
  updateLEDs();

  // Handle car entering the parking lot
  if (digitalRead(ir_enter) == LOW) {
    if (S1 + S2 + S3 + S4 + S5 < slot) {
      myservo.write(180); // Open gate for entry
      delay(3000);        // Allow time for the car to pass
      myservo.write(90);  // Close gate
    } else {
      Serial.println("Parking Full");
    }
  }

  // Handle car exiting the parking lot
  if (digitalRead(ir_back) == LOW) {
    if (S1 + S2 + S3 + S4 + S5 > 0) { // If there are cars parked
      myservo.write(180); // Open gate for exit
      delay(3000);        // Allow time for the car to leave
      myservo.write(90);  // Close gate
    }
  }

  delay(100);
}

void Read_Sensor() {
  // Update slot status based on IR sensors
  S1 = digitalRead(ir_car1) == LOW ? 1 : 0;
  S2 = digitalRead(ir_car2) == LOW ? 1 : 0;
  S3 = digitalRead(ir_car3) == LOW ? 1 : 0;
  S4 = digitalRead(ir_car4) == LOW ? 1 : 0;
  S5 = digitalRead(ir_car5) == LOW ? 1 : 0;
}

void updateLEDs() {
  if (S1 == 0 || S2 == 0 || S3 == 0 || S4 == 0 || S5 == 0) {
    digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, LOW);
  } else {
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, HIGH);
  }
}
