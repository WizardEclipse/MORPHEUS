#include "Arduino.h"

#define LED_PIN PC13   // LED connected to PC13
#define BAUD_RATE 9600 // UART baud rate

//~ PinName pwmPins[] = {PA_0, PA_1, PA_2, PA_3, PA_6, PA_7, PA_8, PB_0, PB_1, PB_6, PB_7, PB_8, PB_9};

int gpioPins[] = {PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7, PA8, PA11, PA12, PA13, PA14, PA15,
                 PB0, PB1, PB3, PB4, PB5, PB6, PB7, PB8, PB9, PB10, PB11, PB12, PB13, PB14, PB15};

int numGPIOPins = sizeof(gpioPins) / sizeof(gpioPins[0]);  // Number of pins
//~ int numPWMPins = sizeof(pwmPins) / sizeof(pwmPins[0]);  // Number of pins



void setup() {

  pinMode(LED_PIN, OUTPUT);  // Set LED pin as output
  digitalWrite(LED_PIN, LOW);  // Turn off LED initially
  Serial1.begin(BAUD_RATE);  // Start UART communication at 9600 baud rate
  
  while (!Serial1.available());  // Wait for handshake initiation from PAULO
	  int receivedMessage = Serial1.parseInt(); // Read the incoming data
	  if (receivedMessage== 001) {
		  digitalWrite(LED_PIN, HIGH); // Turn LED on
		  Serial1.println(002);  // Send back handshake confirmation
		  delay(250);
		  //digitalWrite(LED_PIN, LOW);  // Turn LED off
	  }

}


void testGPIO(int index) {
	int pin = gpioPins[index];
	pinMode(pin, OUTPUT);
	digitalWrite(pin, HIGH);
	Serial1.println(100+index);
	delay(1000);
	digitalWrite(pin,LOW);
	Serial1.flush();
}


//~ void testPWM(int index) {
	//~ int pin = gpioPins[index];
	//~ pinMode(pin, OUTPUT);  // Set LED pin as output
	//~ digitalWrite(pin, HIGH);
	//~ Serial1.println(100+index);
	//~ delay(50);
	//~ digitalWrite(pin,LOW);
	
//~ }

void loop() {
	while (!Serial1.available()); // Wait for Paulo to initiate tests
	int testCode = Serial1.parseInt(); // Read incoming test
	
	int group = testCode / 100; // Break down Comms code into groups (GPIO tests, PWM tests etc)
	int pinNum = testCode % 100; // Gets excact pin number
	
	switch (group)
	{
	case 1:
	{
		for (int i = 0; i < numGPIOPins; i++) {
			pinMode(gpioPins[i], OUTPUT);
		}
		for (int i = 0; i < numGPIOPins; i++) {
			digitalWrite(gpioPins[i], LOW);  // Turn LED off
		}
		testGPIO(pinNum);
		break;
	}
	case 2:
		break;
	default:
		break;
	}
	
}

//~ for (int i = 0; i < numGPIOPins; i++)
