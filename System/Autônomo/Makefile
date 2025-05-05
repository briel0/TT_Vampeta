build:
	~/.platformio/penv/bin/platformio run

upload:
	~/.platformio/penv/bin/platformio run --target upload

clean:
	~/.platformio/penv/bin/platformio run --target clean

monitor:
	~/.platformio/penv/bin/platformio device monitor

erase:
	python -m esptool --chip esp32 erase_flash
