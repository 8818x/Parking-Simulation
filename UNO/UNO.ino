int ledPins[] = { 13, 12, 11 };   // Array to store LED pin numbers
int digitalPins[] = { 7, 6, 5 };  // Array to store IR Sensor pin numbers
int val[] = { 0, 0, 0 };          // Array to store initial sensor values

// Function to convert integer to string
String i2s(int val) {
  int num = val;
  char buffer[10];
  sprintf(buffer, "%d", num);
  return String(buffer);
}

void setup() {
  Serial.begin(115200);  // Initialize serial communication
  for (int i = 0; i < 3; i++) {
    pinMode(ledPins[i], OUTPUT);     // Set LED pins as output mode
    pinMode(digitalPins[i], INPUT);  // Set IR Sensor pins as input mode
  }
}

void loop() {
  // Loop through each sensor
  for (int i = 0; i < 3; i++) {
    val[i] = digitalRead(digitalPins[i]);  // Read sensor values
    if (val[i] == 0) {
      digitalWrite(ledPins[i], HIGH);  // Turn off LED if vehicle detected
    } else {
      digitalWrite(ledPins[i], LOW);  // Turn on LED
    }
  }

  // Convert sensor values to strings
  String c0 = i2s(val[0]);
  String c1 = i2s(val[1]);
  String c2 = i2s(val[2]);

  // Construct JSON string with sensor values
  String data = "{\"value1\":\"" + c0 + "\",\"value2\":\"" + c1 + "\",\"value3\":\"" + c2 + "\"}";

  // Print JSON string to serial monitor
  Serial.println(data);

  delay(5000);  // Delay for 5 seconds
}
