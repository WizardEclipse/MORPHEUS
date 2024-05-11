New Linux install,
apt install all the basics
install python(>3.6),pip,pyenv and other python basics (not needed but helpful)
intall stlink-tools to see into blupills memory 
(for Raspberry Pi, use the built in Add/Remove Software tool from the Preferences tab top right, DO NOT MAKE yourself unless highly skilled or want to waste all the beutiful sunlight)
install PlatformIO Core (CLI):
	curl -fsSL -o get-platformio.py https://raw.githubusercontent.com/platformio/platformio-core-installer/master/get-platformio.py
	python3 get-platformio.py

	install Shell Commands:
	
	mkdir -p /usr/local/bin
	ln -s ~/.platformio/penv/bin/platformio /usr/local/bin/platformio
	ln -s ~/.platformio/penv/bin/pio /usr/local/bin/pio
	ln -s ~/.platformio/penv/bin/piodebuggdb /usr/local/bin/piodebuggdb

	install 99-platformio-udev.rules
	curl -fsSL https://raw.githubusercontent.com/platformio/platformio-core/develop/platformio/assets/system/99-platformio-udev.rules | sudo tee /etc/udev/rules.d/99-platformio-udev.rules
	sudo service udev restart
	sudo usermod -a -G dialout $USER
	sudo usermod -a -G plugdev $USER


TODO: make requirements.txt and make this more step-by-step 

Setting up project:

make project directory
cd into project 
pio project init --board bluepill_f103c8

paste code that we want to go into testi in src
make sure to configure platform.ini well, here is what works:

	[env:bluepill_f103c8]
	platform = ststm32
	board = bluepill_f103c8
	framework = arduino
	upload_flags = -c set CPUTAPID 0x2ba01477
	lib_deps = 	Adafruit SSD1306
			AdafruitBusID
	upload_protocol = stlink
	debug_tool = stlink

In above example external libraries were used in lib deps, make sure they are seperated by different lines and properly tabbed (pythonic)


pio run <- only need once during project inti, will build the environment, and install libraries/frameworks as needed based on .ini
pio run --target upload  <- will upload code to bluepill


Python button automation:
Made interrupt based program, uses gpio pin 21 (bottom right), as input pullup resistor. Debouncing in place. Also 1 minute 'cool down' so kids arent spamming the 'diagnose' button when they're frustrated.

everytime button is pressed the build files are cleared and rebuilt then reflashed onto bluepill. Takes about 50 secs in my tests. We can remove this so that we are'nt necessarily rebuilding from scratch all the time. (probably good to do once we have all the proper testi code figured out)

So far all works great, BUT you have to fist run the python file by going: python3 morphy_button_inter.py

Creating System md service to run on startup and background:

sudo nano /etc/systemd/system/morpheus_flash.service

[Unit]
Description=Run Python script to flash blue pill on Button Press
After=multi-user.target

[Service]
Type=simple
User=pi
ExecStart=/usr/bin/python3 /home/pi/Desktop/flash_to_testi/morphy_button_inter.py

[Install]
WantedBy=multi-user.target

sudo systemctl daemon-reload
sudo systemctl enable morpheus_flash.service
sudo systemctl start morpheus_flash.service


