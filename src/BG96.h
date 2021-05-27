#pragma once
#ifndef BG96_h
#define BG96_h
#include "Arduino.h"
#include <SoftwareSerial.h>

class BG96
{
	public:
		BG96(int Tx, int Rx, int Reset);
		void Connect(int pin);
		void SetBaudRate(int BaudRate);
		void SetPins(int Tx, int Rx, int Reset);
		void PutURL(char URL[]);
		void CheckMessage(String ctemp);
		void CheckConnected();
		void GPS();
		void OpenTCP(char URL[], int Port);
		void HTTPPost(char POSTURL[],char Host[], char SASKey[], char Content[]);
	
	private:
		
		int _Tx;
		int _Rx;
		int _Reset;
		int _pin;
		int _Pin;
		SoftwareSerial BG96Serial;		
};



#endif // BG96_h



