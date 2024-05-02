# MORPHEUS
## Blue Pill Diagnostics Tool

Morpheus is a stand-alone diagnostic tool for the STM32 "bluepill" microcontroller. The purpouse of this project is to develop a device, running on an STM32 that can validate the operation of other STM32 boards.

#### Current Design:
Morpheus will run on an STM32 called Paulo. Paulo will have a one-to-one connection to the testboard. It will:
- Validate GPIO pins
- Validate PWM functionality
- Validate ADC functionality
- Have visual indicators of the progress and functionality of the testboard. This can potentially include: and OLED/LCD screen displaying progress of tests, and error codes when a fault is found. LED lights to visualise the operation of the GPIO pins. A buzzer to verify the operation of the PWM pins, some kind of visual guide for ADC functinality.

### **Todo:**
- Make sequence diagram
- Make circuit diagram
