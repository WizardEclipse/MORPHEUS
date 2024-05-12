#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 	-1 // This display does not have a reset pin accessible
#define BAUD_RATE 9600

Adafruit_SSD1306 display_handler(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
volatile int flashFlag; // Flash Flag

// Interrupt handler for intial button press
void flashInterrupt() {
  if (flashFlag == 0){
    display_handler.clearDisplay();
    display_handler.setCursor(0,0);
    display_handler.println("Flashing...");
    display_handler.display();
    flashFlag = 1;
  }
  else if (flashFlag == 1)
  {
    display_handler.println("Done");
    display_handler.display();
    flashFlag = 2;
  }
}

void flashFAILInterrupt() {
  flashFlag = 400;
  display_handler.clearDisplay();
  display_handler.setCursor(0,0);
  display_handler.println("FLASHING ERROR");
  display_handler.println(flashFlag);
  display_handler.display();

}

void sendCode(int Code) {
  Serial1.println(Code);
  while (!Serial1.available());
  int rec = Serial1.parseInt();
  display_handler.println(rec);
  display_handler.display();
}

void setup() {
  
  flashFlag = 0;

  // Sets up mini display I2C
  display_handler.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display_handler.clearDisplay();
  display_handler.setTextSize(1);
  display_handler.setTextColor(SSD1306_WHITE);
  display_handler.setCursor(0,0);
  display_handler.println("STANDBY");
  display_handler.display();

  // Initialize Button Press pin
  pinMode(PB10, INPUT_PULLUP);
  pinMode(PB11, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PB10), flashInterrupt, FALLING); 
  attachInterrupt(digitalPinToInterrupt(PB11), flashFAILInterrupt, FALLING);  
}

void loop() {
  
  while (flashFlag != 2); // waits for flashing to complete
  flashFlag = 0;
  
  Serial1.begin(BAUD_RATE); // Start UART at 9600
  delay(1000); // Giving testi a lil bit of time

  Serial1.println(001); // Send Handshake From Paulo to Testi
  display_handler.println("Start UART Test...");
  display_handler.display();

  delay(3000);

  if (Serial1.available()) {
    int recievedMessage = Serial1.parseInt(); //read incoming
    if (recievedMessage == 002) {
      display_handler.println("Handshake recieved");
      display_handler.println("UART Success");
      display_handler.display();
    }
    else {
      display_handler.println("UART Corrupted");
      display_handler.display();
    }
  } else {
    display_handler.println("UART Timeout");
    display_handler.display();
  }
  
  delay(1000);
  sendCode(102);

  // B12 B13 B14 -> A B C
  pinMode(PB12, OUTPUT);
  digitalWrite(PB12, LOW);
  
  pinMode(PB13, OUTPUT);
  digitalWrite(PB13, LOW);
  
  pinMode(PB14, OUTPUT);
  digitalWrite(PB14, LOW);

  pinMode(PA0, INPUT);
  sendCode(101);

  display_handler.println(digitalRead(PA0));
  display_handler.display();
  
};

// void multiPlex(int pinIndex) {
//   digitalWrite(PB12, (pinIndex & 1) ? HIGH : LOW);
//   digitalWrite(PB13, (pinIndex & 1) ? HIGH : LOW);
//   digitalWrite(PB14, (pinIndex & 1) ? HIGH : LOW);

// }