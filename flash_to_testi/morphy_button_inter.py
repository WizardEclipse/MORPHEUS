import RPi.GPIO as GPIO
import os
import signal
from datetime import datetime, timedelta

## GPIO.sEtup
GPIO.setmode(GPIO.BCM)
GPIO.setup(21, GPIO.IN, pull_up_down = GPIO.PUD_UP)


## Event running in background with interrupts 
## but we don wan more than 1 button press every minute so this

last_ran = datetime.min

#Callback type beat
def button_press(channel):
    global last_ran
    if datetime.now() - last_ran > timedelta(minutes=1):
        print("Ye boi, Flashing")
        os.system("pio run --target clean && pio run --target upload")
        last_ran = datetime.now()
    else:
        print('Chill fam')
            

#Interrupt type beat
GPIO.add_event_detect(21, GPIO.FALLING, callback=button_press, bouncetime=300)


##Go to sleep
try:
    signal.pause()
except KeyboardInterrupt:
    print('Bye')
finally:
    GPIO.cleanup()
