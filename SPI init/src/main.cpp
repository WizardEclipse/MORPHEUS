#include <SPI.h>

#define SS_PIN PA4  // Define the Slave Select pin
#define LED_PIN PC13  // Define LED pin (built-in LED on STM32 Blue Pill)

void setup() {
  pinMode(SS_PIN, OUTPUT); // Set the SS pin as an output
  pinMode(LED_PIN, OUTPUT); // Set the LED pin as an output
  digitalWrite(SS_PIN, HIGH); // Start with the slave deselected
  digitalWrite(LED_PIN, LOW); // Ensure LED is off initially
  
  Serial.begin(115200); // Start serial communication at 115200 baud
  SPI.begin(); // Initialize SPI communication
  SPI.setClockDivider(SPI_CLOCK_DIV16); // Set clock divider for appropriate SPI speed
}

void loop() {
  digitalWrite(SS_PIN, LOW); // Select the slave
  byte sentData = 0x35; // Data to send
  byte receivedData = SPI.transfer(sentData); // Send data to the slave and receive response
  digitalWrite(SS_PIN, HIGH); // Deselect the slave

  // Check if the received data is what we expect (0x12)
  if (receivedData == 0x12) {
    digitalWrite(LED_PIN, HIGH); // Turn LED on
    delay(100); // Keep it on for 100 ms
    digitalWrite(LED_PIN, LOW); // Turn LED off
  }

  Serial.print("Sent: ");
  Serial.print(sentData, HEX); // Print sent data in hexadecimal
  Serial.print(", Received: ");
  Serial.println(receivedData, HEX); // Print received data in hexadecimal
  delay(2000); // Wait for two seconds
}




// #include <SPI.h>

// #define SS_PIN PA4  // Define the Slave Select pin
// #define LED_PIN PC13  // Define LED pin (built-in LED on STM32 Blue Pill)

// volatile byte receivedData;
// volatile bool newData = false;

// void setup() {
//   pinMode(SS_PIN, INPUT); // Set the SS pin as an input to detect master's selection
//   pinMode(MISO, OUTPUT); // Set MISO as output
//   pinMode(LED_PIN, OUTPUT); // Set the LED pin as an output
//   digitalWrite(LED_PIN, LOW); // Ensure LED is off initially
//   Serial.begin(115200); // Start serial communication

//   SPI.begin(); // Start SPI in slave mode
//   SPI.attachInterrupt(); // Enable SPI interrupt to handle incoming data
// }

// void SPI_ISR() {
//   receivedData = SPI.transfer(0x12); // Send back 0x12 whenever data is received
//   newData = true; // Set flag when new data is received
//   digitalWrite(LED_PIN, HIGH); // Turn LED on
//   delay(100); // Keep it on for 100 ms
//   digitalWrite(LED_PIN, LOW); // Turn LED off
// }

// void loop() {
//   if (newData) {
//     Serial.print("Received Data: ");
//     Serial.println(receivedData, HEX); // Print received data
//     newData = false; // Reset the flag
//   }
// }
