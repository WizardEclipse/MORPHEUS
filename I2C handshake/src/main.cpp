
// //PAULO

#include <Wire.h>
#include <Arduino.h>

const int LED_PIN = PC13;
const int I2C_SLAVE_ADDR = 9;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  Wire.begin();  // Start I2C as master
}

void blinkError(int code) {
  for (int i = 0; i < code; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(300);
    digitalWrite(LED_PIN, LOW);
    delay(300);
  }
  delay(500);  // Pause between error codes
}

void loop() {
  Serial.println(004);  // Send code 004 to Testi via UART
  delay(500);  // Wait for Testi to process
  
  // Wait for response code 005 via UART
  unsigned long startTime = millis();
  while (!Serial.available() && millis() - startTime < 2750);
  if (Serial.available()) {
    int code = Serial.parseInt();
    if (code == 005) {
      // Send 005 to Testi via I2C
      Wire.beginTransmission(I2C_SLAVE_ADDR);
      Wire.write(005);
      Wire.endTransmission();

      // Wait for response 004 via I2C
      Wire.requestFrom(I2C_SLAVE_ADDR, 1);
      startTime = millis();
      while (!Wire.available() && millis() - startTime < 2750);
      if (Wire.available()) {
        int response = Wire.read();
        if (response == 004) {
          // Success, blink LED quickly three times
          blinkError(3);
        } else {
          // Error code 008
          blinkError(8);
        }
      } else {
        // No response received, error code 008
        blinkError(8);
      }
    } else {
      // Incorrect or no code received, blink LED error 5 times
      blinkError(5);
    }
  } else {
    // Timeout waiting for 005, blink LED error 8 times
    blinkError(8);
  }
  
  delay(5000);  // Delay before next loop iteration
}



// Testi


// #include <Wire.h>
// #include <Arduino.h>

// const int LED_PIN = PC13;
// const int I2C_SLAVE_ADDR = 9;

// void receiveEvent(int howMany) {
//   while (Wire.available()) {
//     int code = Wire.read();
//     if (code == 005) {
//       // Send 004 back to Paulo if received 005
//       Wire.write(004);
//     }
//   }
// }

// void requestEvent() {
//   Wire.write(005);  // Send 005 when requested
// }

// void setup() {
//   pinMode(LED_PIN, OUTPUT);
//   Serial.begin(9600);
//   Wire.begin(I2C_SLAVE_ADDR);  // Start I2C as slave on address 9
//   Wire.onReceive(receiveEvent);
//   Wire.onRequest(requestEvent);
// }

// void blinkLED(int times) {
//   for (int i = 0; i < times; i++) {
//     digitalWrite(LED_PIN, HIGH);
//     delay(200);
//     digitalWrite(LED_PIN, LOW);
//     delay(200);
//   }
// }


// void loop() {
//   if (Serial.available()) {
//     int code = Serial.parseInt();
//     if (code == 004) {
//       // Received 004 from Paulo, send 005 via UART
//       Serial.println(005);
//       blinkLED(2);  // Blink to show received 004
//     }
//   }
// }
