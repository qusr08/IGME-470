// https://howtomechatronics.com/tutorials/arduino/ultrasonic-sensor-hc-sr04/

#define TRIG_PIN 52
#define ECHO_PIN 53

#define RANGE_MAX 2000
#define RANGE_MIN 300

int distance;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  Serial.begin(9600);
}

void loop() {
  // Clears the trig pin
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  // Sets the trig pin on HIGH state for 10 microseconds
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Reads the echo pin, returns the sound wave travel time in microseconds
  // The value read can just be used for the distance though
  distance = pulseIn(ECHO_PIN, HIGH);
  // Make sure the value read by the sensor is valid
  if (distance < RANGE_MIN || distance > RANGE_MAX) {
    distance = -1;
  }
  
  Serial.flush();
  Serial.println(distance);

  delay(20);
}
