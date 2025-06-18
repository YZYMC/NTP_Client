.PHONY: compile
compile:
	mkdir ./bin
	g++ -std=c++17 -o ./bin/ntp_client ./NTP_Client/main.cpp

CONFIG_FILE = ./bin/config.ini
config.ini:
	@echo "Creating default config file..."
	@echo "; NTP client configuration" > $(CONFIG_FILE)
	@echo "[config]" >> $(CONFIG_FILE)
	@echo "server = yzynetwork.xyz" >> $(CONFIG_FILE)
	@echo "; Sync interval (second)" >> $(CONFIG_FILE)
	@echo "interval = 240" >> $(CONFIG_FILE)