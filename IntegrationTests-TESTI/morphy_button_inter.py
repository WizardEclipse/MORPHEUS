import RPi.GPIO as GPIO
import os
import signal
from datetime import datetime, timedelta
import time

## GPIO.sEtup
GPIO.setmode(GPIO.BCM)
GPIO.setup(26, GPIO.IN, pull_up_down = GPIO.PUD_UP) #Button

GPIO.setup(5, GPIO.OUT, initial=GPIO.HIGH)
GPIO.setup(6, GPIO.OUT, initial=GPIO.HIGH)


## Event running in background with interrupts 
## but we don wan more than 1 button press every minute so this

last_ran = datetime.min

#Callback type beat
def button_press(channel):
    global last_ran
    if datetime.now() - last_ran > timedelta(minutes=0.1):
        print("Ye boi, Flashing")
        
        # Notifies Paulo of Flash start 
        GPIO.output(5, GPIO.LOW)
        time.sleep(1)
        GPIO.output(5, GPIO.HIGH)
        
        #Flashes
        #os.system("pio run --target clean && pio run --target upload")
        errcode = os.system("pio run --target upload")
        last_ran = datetime.now()
        
        #Notifies Paulo of Flash end
        if (errcode == 0): # Succesful flash
                GPIO.output(5, GPIO.LOW)
                time.sleep(0.1)
                GPIO.output(5, GPIO.HIGH)
        elif (errcode != 0): # Did not flash properly, notify different pin of fatal error
                GPIO.output(6, GPIO.LOW)
                time.sleep(0.1)
                GPIO.output(6, GPIO.HIGH)
    else:
        print('Chill fam')
            

#Interrupt type beat
GPIO.add_event_detect(26, GPIO.FALLING, callback=button_press, bouncetime=300)


##Go to sleep
try:
    signal.pause()
except KeyboardInterrupt:
    print('Bye')
finally:
    GPIO.cleanup()
