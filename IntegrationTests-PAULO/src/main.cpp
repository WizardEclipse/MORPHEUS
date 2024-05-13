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
  if (flashFlag == 0 || flashFlag == 400){
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
  display_handler.print(rec % 100);
  display_handler.print(" ");
  display_handler.display();
}


void setMultiplex(int pinIndex) {
  // B12 B13 B14 -> A B C
  digitalWrite(PB12, (pinIndex & 1) ? HIGH : LOW);
  digitalWrite(PB13, (pinIndex & 2) ? HIGH : LOW);
  digitalWrite(PB14, (pinIndex & 4) ? HIGH : LOW);
}

int PortA0Pins[] = {PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7};
int PortA1Pins[] = {PA8, PA11, PA12, PA15, PB0, PB1, PB3, PB4};

void testGPIO4plex(void) {
  pinMode(PA0, INPUT);
  pinMode(PA1, INPUT);
  pinMode(PA2, INPUT);
  pinMode(PA3, INPUT);
  
  display_handler.clearDisplay();
  display_handler.setCursor(0,0);
  
  for (int i = 0; i < 8; i++) { // 8 on the multiplex
    setMultiplex(i);
    
    sendCode(100 + i);
    display_handler.print(digitalRead(PA0));
    display_handler.print(" ");
    sendCode(100 + 8 + i);
    display_handler.print(digitalRead(PA1));
    display_handler.print(" ");
    sendCode(100 + 16 + i);
    display_handler.print(digitalRead(PA2));
    display_handler.print(" ");
    sendCode(100 + 24 + i);
    display_handler.print(digitalRead(PA3));
    display_handler.println("");
    display_handler.display();
    
    delay(500);
  }
}
void setup() {
  
  flashFlag = 0;

  // Sets up mini display I2C
  display_handler.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display_handler.clearDisplay();
  display_handler.setTextSize(0.5);
  display_handler.setTextColor(SSD1306_WHITE);
  display_handler.setCursor(0,0);
  display_handler.println("STANDBY");
  display_handler.display();

  //Initiate Multiplex pins
  pinMode(PB12, OUTPUT);
  pinMode(PB13, OUTPUT);
  pinMode(PB14, OUTPUT);

  // Initialize Flash Response pins
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
  
  testGPIO4plex();
  
};

// void setMultiplex(int pinIndex) {
//   // B12 B13 B14 -> A B C
//   digitalWrite(PB12, (pinIndex & 1) ? HIGH : LOW);
//   digitalWrite(PB13, (pinIndex & 2) ? HIGH : LOW);
//   digitalWrite(PB14, (pinIndex & 4) ? HIGH : LOW);
// }

// int PortA0Pins[] = {PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7};
// int PortA1Pins[] = {PA8, PA11, PA12, PA15};

// void testGPIO(void) {
//   pinMode(PA0, INPUT);
//   pinMode(PA1, INPUT);
//   display_handler.clearDisplay();
//   display_handler.setCursor(0,0);
//   for (int i = 0; i < 8; i++) {
//     setMultiplex(i);
//     sendCode(100 + i);
//     sendCode(100 + 8 + i);
//     display_handler.print(" ");
//     display_handler.println(digitalRead(PA0));
//     display_handler.println(digitalRead(PA1));
//     display_handler.display();
//     delay(400);
//   }
//}