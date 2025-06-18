.PHONY: compile
compile:
	mkdir ./bin
	g++ -std=c++17 -o ./bin/ntp_client ./NTP_Client/main.cpp