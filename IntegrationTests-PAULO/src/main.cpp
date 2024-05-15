#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define OLED_RESET 	-1    // This display does not have a reset pin accessible
#define BAUD_RATE 9600

Adafruit_SSD1306 display_handler(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup_display();
void flashInterrupt();
void flashFAILInterrupt();
void TEST1_UARTHandShake(void);
void commsEcho(int Code);
void setMultiplex(int muxChannel);
void TEST2_GPIO4plex_notfinal(void);

volatile int flashFlag;   // Flash Flag
// Shows mapping of pins to input ports A0, A1, A2, A3
int PortA0Pins[] = {PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7};
int PortA1Pins[] = {PA8, PA11, PA12, PA15, PB0, PB1, PB3, PB4};

/**
 * @brief main setup function.
*/
void setup() {  
  flashFlag = 0; //intial Flash State
  // Sets up mini display I2C
  setup_display();
  // Initiate Multiplex ABC Channel select pins
  pinMode(PB12, OUTPUT);
  pinMode(PB13, OUTPUT);
  pinMode(PB14, OUTPUT);
  // Initaialize 4 ports for MUX input
  pinMode(PA0, INPUT);
  pinMode(PA1, INPUT);
  pinMode(PA2, INPUT);
  pinMode(PA3, INPUT);
  // Initialize Flash Response pins
  pinMode(PB10, INPUT_PULLUP);
  pinMode(PB11, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PB10), flashInterrupt, FALLING); 
  attachInterrupt(digitalPinToInterrupt(PB11), flashFAILInterrupt, FALLING);  
}

/**
 * @brief Main loop
*/
void loop() {
  while (flashFlag != 2); // waits for flashing to complete
  flashFlag = 0;
  TEST1_UARTHandShake();
  delay(1000);            // wait before next test
  TEST2_GPIO4plex_notfinal();
  //commsEcho(102);
}

/**
 * @brief Sets up mini display I2C
*/
void setup_display() {
  display_handler.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display_handler.clearDisplay();
  display_handler.setTextSize(0.5);
  display_handler.setTextColor(SSD1306_WHITE);
  display_handler.setCursor(0,0);
  display_handler.println("STANDBY");
  display_handler.display();
}

/**
 * @brief ISR for signal from RPi indicating flashing.
 * @pre Connect Flash State wire to PB10 on Paulo.
*/
void flashInterrupt() {
  if (flashFlag == 0 || flashFlag == 400){ // Proceed to flashing from standby or Fail mode 
    display_handler.clearDisplay();
    display_handler.setCursor(0,0);
    display_handler.println("Flashing...");
    display_handler.display();
    flashFlag = 1;                   
  } else if (flashFlag == 1){
    display_handler.println("Done");
    display_handler.display();
    flashFlag = 2;
  }
}

/**
 * @brief ISR for signal from RPi indicating flash FAILURE.
 * @pre Connect Flash FAIL wire to PB11 on PAULO.
*/
void flashFAILInterrupt() {
  flashFlag = 400;
  display_handler.clearDisplay();
  display_handler.setCursor(0,0);
  display_handler.println("FLASHING ERROR");
  display_handler.println(flashFlag);
  display_handler.display();
}


/**
 * @brief UART HAndshake test, send test code to TEST, expect correct response within 3 seconds
*/
void TEST1_UARTHandShake(void) {

  Serial1.begin(BAUD_RATE);   // Start UART at 9600
  delay(1000);                // Giving TESTI time to setup UART
  Serial1.println(001);       // Send Handshake From Paulo to Testi
  display_handler.println("Start UART Test...");
  display_handler.display();

  delay(3000);  // 3 second UART timeout, if no message in UART pipeline by then test fail, UART TIMEOUT
  if (Serial1.available()) {
    int recievedMessage = Serial1.parseInt(); //read incoming
    if (recievedMessage == 002) {             // On succesfull Handshake
      display_handler.println("Handshake recieved");
      display_handler.println("UART Success");
      display_handler.display();
    } else { // Recievd incorrect bytes from UART, could identify faulty data line, or leakage
      display_handler.println("UART Corrupted");
      display_handler.display();
    }
  } else { // Did not recieve in 3 seconds (delay above)
    display_handler.println("UART Timeout");
    display_handler.display();
  }
}



/**
 * @brief Send, then Recieve Code to TESTI through UART, see internal error/comms code for reference.
 * @pre Connect P9 (RX) -> P10 (TX), P10 (TX) -> P9 (RX).
 * 
 * @param int Code integer Code value, see internal error/comms code for reference.
 * @retval none
*/
void commsEcho(int Code) {
  Serial1.println(Code);          //Send code through UART configured as Serial1
  while (!Serial1.available());   //Recieve code through UART
  int rec = Serial1.parseInt();   //Expect only integer codes
  display_handler.print(rec);
  //display_handler.print("");
  display_handler.display();
  Serial1.flush();
}

/**
 * @brief Set control pins A,B,C on the 4 mutliplexers that output to the input pins on PAULO
 * 
 * @details 
 * 
 * 4, 8:1 MUX are connected to 30 pins on TESTI, MUX output lines are connected to PAULO.
 *          MUX0 out -> PA0 on PAULO
 *          MUX1 out -> PA1 on PAULO
 *          MUX2 out -> PA0 on PAULO
 *          MUX3 out -> PA1 on PAULO
 *          
 * Channel selection on 4 MUXs are set to use the same A, B and C values, driven by PAULO lines PB12, PB13, PB14.
 *          MUX0 A, MUX1 A, MUX2 A, MUX3 A ->  PB12 on PAULO
 *          MUX0 B, MUX1 B, MUX2 B, MUX3 B ->  PB13 on PAULO
 *          MUX0 C, MUX1 C, MUX2 C, MUX3 C ->  PB14 on PAULO
 *         
 * @param int muxChannel integer [0, 8] corresponding to the 8:1 multiplexer.
 * @retval none
*/
void setMultiplex(int muxChannel) {
  // B12 B13 B14 -> A B C
  digitalWrite(PB12, (muxChannel & 1) ? HIGH : LOW);
  digitalWrite(PB13, (muxChannel & 2) ? HIGH : LOW);
  digitalWrite(PB14, (muxChannel & 4) ? HIGH : LOW);
}


/**
 * @brief runs through GPIO testing 30 pins on TESTI using 4, 8 channel Multiplexers.
 * 
 * 
 * @details 
 * 4, 8:1 MUX are connected to 30 pins on TESTI, MUX output lines are connected to PAULO.
 *          MUX0 out -> PA0 on PAULO
 *          MUX1 out -> PA1 on PAULO
 *          MUX2 out -> PA0 on PAULO
 *          MUX3 out -> PA1 on PAULO
 *          
 * Channel selection on 4 MUXs are set to use the same A, B and C values, driven by PAULO lines PB12, PB13, PB14.
 *          MUX0 A, MUX1 A, MUX2 A, MUX3 A ->  PB12 on PAULO
 *          MUX0 B, MUX1 B, MUX2 B, MUX3 B ->  PB13 on PAULO
 *          MUX0 C, MUX1 C, MUX2 C, MUX3 C ->  PB14 on PAULO

*/
void TEST2_GPIO4plex_notfinal(void) {
  display_handler.clearDisplay();
  display_handler.setCursor(0,0);
  
  for (int i = 0; i < 8; i++) { // go through all 8 MUX channels
    
    setMultiplex(i);    // set A,B,C on MUXs
    
    commsEcho(100 + i); // TEST turns pin high when recieving 100 + x + i
    display_handler.print(digitalRead(PA0)); // Expect 1
    display_handler.print(" ");
    commsEcho(100 + 8 + i);
    display_handler.print(digitalRead(PA1)); // Expect 1
    display_handler.print(" ");
    commsEcho(100 + 16 + i);
    display_handler.print(digitalRead(PA2)); //Expect 1
    display_handler.print(" ");
    commsEcho(100 + 24 + i);
    display_handler.print(digitalRead(PA3)); //Expect 1
    display_handler.println("");
    display_handler.display();
    
    delay(500);
  }
}

