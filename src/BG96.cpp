
#include "Arduino.h"
#include "BG96.h"
#include <SoftwareSerial.h>

int _Tx;
int _Rx;
int _Reset;
int _Pin;
//SoftwareSerial BG96Serial(_Tx,_Rx);
// Constructor
BG96::BG96(int Tx, int Rx, int Reset ):	
	_Tx(Tx), 	
 	_Rx(Rx),
 	_Reset(Reset),
	BG96Serial(Tx, Rx)
{
}

void BG96::SetBaudRate(int BaudRate)
{
	//static int QuectelResetPin = 4;
	BG96Serial.listen();
	int i;
	String ReceivedMessage;
	Serial.begin(9600);
	BG96Serial.begin(9600);

	while (true)
	{
		Serial.println("ON/OFF");
		pinMode(_Reset, OUTPUT);
		digitalWrite(_Reset, LOW);
		delay(3000);
		pinMode(_Reset, INPUT);
		for (i = 0; i <= 2; i++)
		{
			BG96Serial.begin(115200);
			ReceivedMessage = BG96Serial.readString();
			BG96Serial.println("AT");
			ReceivedMessage = BG96Serial.readString();
			if (ReceivedMessage.indexOf("OK") > 0)
			{

				Serial.println("Started");
				Serial.print("\n changing  baudrates");
				BG96Serial.println("AT+IPR=9600");
				Serial.end();
				BG96Serial.end();
				Serial.begin(9600);
				BG96Serial.begin(9600);
				i = 0;
				goto done;
			}
			else
			{
				Serial.println("Retry at 115200" + i);
			};

			delay(1000);
		}
		i = 0;


		for (i = 0; i <= 2; i++)
		{
			
			BG96Serial.begin(BaudRate);
			BG96Serial.println("AT");
			//  p(F("AT")); 
			delay(1000);
			ReceivedMessage = BG96Serial.readString();
			if (ReceivedMessage.indexOf("OK") > 0)
			{
				Serial.println("Started");
				i = 0;
				goto done;
			}
			else
			{
				Serial.print("Retry at "); Serial.print(BaudRate); Serial.println(" " + i);
			};
		}
	}

done:
	
	delay(1000);
	BG96Serial.println("AT+IPR?");
	delay(400);
	Serial.print(BG96Serial.readString() + "\n");

	delay(200);
	Serial.print(BG96Serial.readString() + "\n");
	Serial.end();
	BG96Serial.end();
	Serial.begin(9600);

}

void BG96::Connect(int pin)
{
	BG96Serial.listen();
	//saves pin for use in other functions
	_Pin = pin;
//	SoftwareSerial BG96Serial(_Tx,_Rx); // rx , tx
	Serial.begin(9600);
	BG96Serial.begin(9600);

	String ReceivedMessage;

	BG96Serial.listen();
	BG96Serial.println("AT");
	Serial.println("AT sent");

	Serial.print(BG96Serial.readString());
	Serial.flush();
	BG96Serial.flush();
	delay(100);
	/// Cpin set the pin of the sim card, If the sim is already unlocked it will reply with error, this needs to be set to the front of the SetBG96 function so it can do the stuff below
	BG96Serial.print("AT+CPIN = "); BG96Serial.println(pin);
	while (BG96Serial.available() == 0) {}
	Serial.print(BG96Serial.readString());
	Serial.flush();
	BG96Serial.flush();
	delay(100);
	BG96Serial.println("ATV1");

	while (BG96Serial.available() == 0) {}
	Serial.print(BG96Serial.readString());
	Serial.flush();
	BG96Serial.flush();
	delay(100);

	// Set Echo 1 = echo on 0 = off
	BG96Serial.println("ATE0");
	Serial.print(BG96Serial.readString());
	Serial.flush();
	BG96Serial.flush();
	delay(100);

	BG96Serial.println("AT+CMEE=2");
	while (BG96Serial.available() == 0) {}
	Serial.print(BG96Serial.readString());
	Serial.flush();
	BG96Serial.flush();
	delay(100);


	BG96Serial.println("AT+GSN");
	while (BG96Serial.available() == 0) {}
	Serial.print(BG96Serial.readString());
	Serial.flush();
	BG96Serial.flush();
	delay(100);


	BG96Serial.println("ATI");
	while (BG96Serial.available() == 0) {}
	Serial.print(BG96Serial.readString());
	Serial.flush();
	BG96Serial.flush();
	delay(100);


	BG96Serial.println("AT+COPS");
	while (BG96Serial.available() == 0) {}
	Serial.print(BG96Serial.readString());
	Serial.flush();
	BG96Serial.flush();
	delay(100);


	BG96Serial.println("AT+QCCID");
	while (BG96Serial.available() == 0) {}
	Serial.print(BG96Serial.readString());
	Serial.flush();
	BG96Serial.flush();
	delay(100);

	BG96Serial.println("AT+COPS?");

	ReceivedMessage = BG96Serial.readString();
	Serial.print(ReceivedMessage);	
	if (ReceivedMessage.indexOf("Vod") > 0)
	{
		Serial.println("Connected");
	}
	else
	{
//		Serial.println("Not connected");
		Connect(_pin);
	};
		

	ReceivedMessage = BG96Serial.readString();
	Serial.print(BG96Serial.readString());
	Serial.flush();
	BG96Serial.flush();
	delay(100);

	BG96Serial.println("AT+CSQ");
	while (BG96Serial.available() == 0) {}
	Serial.print(BG96Serial.readString());
	Serial.flush();
	BG96Serial.flush();
	delay(100);


	// registers on the network
	BG96Serial.println("AT+CREG?");
	while (BG96Serial.available() == 0) {}
	Serial.print(BG96Serial.readString());
	Serial.flush();
	BG96Serial.flush();
	delay(100);


	BG96Serial.println("AT+QIACT=1");
	while (BG96Serial.available() == 0) {}
	Serial.print(BG96Serial.readString());
	Serial.flush();
	BG96Serial.flush();
	delay(100);

	// turns header request on allowing to read the http responce codes
	BG96Serial.println("AT+QHTTPCFG = \"requestheader\",1");
	//    Serial.print("AT+QHTTPCFG = \"requestheader\",1\n");
	while (BG96Serial.available() == 0) {}
	Serial.print(BG96Serial.readString() + "\n\n\n");
	Serial.flush();
	delay(100);
// turns on GPS always
	BG96Serial.println("AT+QGPS=1");
	delay(300);
	Serial.print(BG96Serial.readString());
	Serial.flush();
	BG96Serial.flush();
	delay(100);
	Serial.println("GPS ON");
	

	Serial.print("\n**********\n CONNECTED\n************\n");
}

void BG96::SetPins(int Tx, int Rx, int Reset)
{
 _Tx = Tx;
 _Rx = Rx;
 _Reset = Reset;
}

void BG96::CheckMessage(String ctemp)
{
int timer = 0;
bool receivedConnect = false;
  for (timer = 0; timer < 30 && receivedConnect == false; timer++)
  {
    Serial.print(String(timer));
    if (BG96Serial.available() > 0)
    {
      String httpCon = BG96Serial.readString();
      Serial.print(httpCon);
      String networkfail = "+CME ERROR: Http network open fail";
      httpCon.trim();
      if (httpCon.equals(ctemp))
      {
        receivedConnect = true; 
      }   
         if (httpCon.equals(networkfail))
      {
      Connect(_pin);
      }   
    }
    if (timer > 58)
    {
      Connect(_pin);
    }
    delay(1000);
  }
}

void BG96::PutURL(char URL[])
{

Serial.println (strlen(URL));


BG96Serial.print("AT+QHTTPURL=");BG96Serial.print(strlen(URL));BG96Serial.println(",10");
Serial.print("AT+QHTTPURL=");Serial.print(strlen(URL));Serial.println(",10");

//Serial.print("AT+QHTTPURL=95,10");

CheckMessage("CONNECT");
Serial.println(URL);
BG96Serial.println(URL);
//BG96Serial.println("https://PrototypeIOTHUB.azure-devices.net/devices/BigBox/messages/events?api-version=2016-02-03");
CheckMessage("OK");
}

void BG96::CheckConnected()
{
int timer = 0;
bool receivedConnect = false;
  for (timer = 0; timer < 30 && receivedConnect == false; timer++)
  {
    Serial.print(String(timer));
    if (BG96Serial.available() > 0)
    {
	String ctemp = "CONNECT";
      String httpCon = BG96Serial.readString();
      Serial.print(httpCon);
      String networkfail = "+CME ERROR: Http network open fail";
      httpCon.trim();
      if (httpCon.equals(ctemp))
      {
        receivedConnect = true; 
      }   
         if (httpCon.equals(networkfail))
      {
      Connect(_pin);
      }   
    }
    if (timer > 58)
    {
      Connect(_pin);
    }
    delay(1000);
  }
}


void BG96::GPS()
{
Serial.print("*****GPS****");


	

	BG96Serial.println("AT+QGPSLOC?");
	delay(300);
	Serial.print(BG96Serial.readString());
	Serial.flush();
	BG96Serial.flush();
	delay(100);

	BG96Serial.println("AT+QGPSCFG=?");
	delay(300);
	Serial.print(BG96Serial.readString());
	Serial.flush();
	BG96Serial.flush();
	delay(100);
}

void BG96::OpenTCP(char URL[],int Port)
{
BG96Serial.print("AT+QIOPEN=1,0,\"TCP\",");  BG96Serial.print(URL); BG96Serial.print( ","); BG96Serial.println(Port);
delay(300);
Serial.print(BG96Serial.readString());
}

void BG96::HTTPPost(char POSTURL[], char Host[], char SASKey[], char Content[])
{
static char OpeningPost[] = "POST ";
static char EndingPost[] = " HTTP/1.1\n";
static char BoilerHost[] = "Host: "; 
static char BoilerLines[] = "\nAccept: */*\nUser-Agent: QUECTEL_MODULE\nConnection: Keep-Alive\nContent-Type: application/json\nAuthorization: "; 
static char BoilerContent[] = "\nCache-Control: no-cache\nContent-Length: "; 
 int ContentLength = 2;
if (sizeof(Content) < 10)
{
 ContentLength = 1;
}
if (sizeof(Content) > 9)
{
 ContentLength = 2;
}
if (sizeof(Content) > 99)
{
 ContentLength = 3;
}
Serial.print("AT+QHTTPPOST="); Serial.print(strlen(Content) + strlen(OpeningPost)  + strlen(EndingPost)  + strlen(BoilerHost)  + strlen(Host) +  strlen(POSTURL) + strlen(BoilerLines) + strlen(SASKey) + strlen(BoilerContent)  +5 + ContentLength);     Serial.println(",25, 25");
BG96Serial.print("AT+QHTTPPOST="); BG96Serial.print(strlen(Content) + strlen(OpeningPost)  + strlen(EndingPost)  + strlen(BoilerHost)  + strlen(Host) + strlen(POSTURL)+ strlen(BoilerLines)  + strlen(SASKey)  + strlen(BoilerContent)  +5 + ContentLength) ; BG96Serial.println(",25, 25");

while (BG96Serial.available() == 0) {}
Serial.print(BG96Serial.readString());

Serial.print(BG96Serial.readString());

Serial.print(OpeningPost);
Serial.print(POSTURL);
Serial.print(EndingPost);
Serial.print(BoilerHost);
Serial.print(Host);
Serial.print(BoilerLines);
Serial.print(SASKey);
Serial.print(BoilerContent);
Serial.print(strlen(Content));
Serial.print("\n\n");
Serial.println(Content);
Serial.print("\n\n");

BG96Serial.print(OpeningPost);
BG96Serial.print(POSTURL);
BG96Serial.print(EndingPost);
BG96Serial.print(BoilerHost);
BG96Serial.print(Host);
BG96Serial.print(BoilerLines);
BG96Serial.print(SASKey);
BG96Serial.print(BoilerContent);
BG96Serial.print(strlen(Content));
BG96Serial.print("\n\n");
BG96Serial.println(Content);
BG96Serial.print("\n\n");

while (BG96Serial.available() == 0) {}
String ans = BG96Serial.readString();

if (ans.indexOf("+QHTTPPOST: 0,204,0") > 0)
			{
Serial.println("OK Got");
			}
Serial.println(BG96Serial.readString());

}
// turn off AT COMMAND
//AT+QPOWD