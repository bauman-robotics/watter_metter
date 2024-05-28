/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-mysql-database-php/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

*/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "certs.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define SERIAL_PRINT_DEBUG_EN (1)
//#include <SoftwareSerial.h>; ///
///-------------------------------

//===========================
String user_id       = "Andrey";
String user_location = "Office";
String apiKeyValue   = "xxxxxx";
int cold             = -1;
int hot              = -1;
int alarm_interval   = 200; // 200 - 2 min, 00 sec 
//===========================

const char*    ssid       = "zzzzzz";
const char*    password   = "vvvvvv";
const char*    serverName = "baumanrobotics.ru";
const char*    scriptName = "/water_post.php";
const uint16_t serverPort = 443;  
//===========================
unsigned long wakeUpTime;
time_t now; 
struct tm timeinfo;
	
int readline(uint8_t readch, char *buffer, int len);
void Send_Post_Request(int cold, int hot, int alarm_interval);
bool ConnectToWifi(void);
void GetCurrTime(void);
bool ConnectToHTTPSserver(void);	
int GetCold(void);
int GetHot(void);
void Serial_Flush_Buffer();
//===========================

X509List certISRG(ISRG_Root_X1);
// Use WiFiClientSecure class to create TLS connection
WiFiClientSecure wifiClient;

//====================================================================

void setup() {
	bool conn = 0;
	char buf[100];
	Serial_Flush_Buffer();
	ConnectToWifi();
	GetCurrTime();
	conn = ConnectToHTTPSserver();	
	
	if (conn) {
		cold = GetCold();		
		delay(100);
		Serial_Flush_Buffer();
		hot = GetHot();
		
		Send_Post_Request(cold, hot, alarm_interval);
	}
	delay(100);
	Serial.print("set_time0");
	delay(100);
	//=========
	wakeUpTime = millis() / 1000;
	if (alarm_interval > wakeUpTime) {
		alarm_interval = alarm_interval - 100 + 60 - wakeUpTime;
	}
	//=========	
	if (alarm_interval < 100) {
		alarm_interval = 100;	// 1 min	
	}
 alarm_interval = 200;
	//=========	
	sprintf(buf, "%s%d", "set_alarm_time", alarm_interval);
	Serial.print(buf);
	//=========	
	delay(100);      
	Serial.print("esp_force_off");	
}
//====================================================================

void loop() {
}
//====================================================================

int readline(uint8_t readch, char *buffer, int len) {
    static int pos = 0;
    int rpos = 0;
	//===================
    if (readch > 0) {
		//===================
		if ((readch == 13) || (readch == 10)) {
			rpos = pos;
            buffer[pos] = 0;
            pos = 0;  // Reset position index ready for next time
            return rpos;			
		}
		//===================
		if ((readch >= '0') && (readch <= '9') && (pos < len-1)) {
			buffer[pos] = readch;
			pos++;		
		} 
		//===================	
		if ((readch < '0') && (readch > '9')) {
			pos = 0;
			buffer[pos] = 0;
			rpos = 0;
		}
		//===================
		if (pos >= len-1) {
			pos = 0;
			return 0;
		}			
		//===================
    }
    return 0;
}
//====================================================================

void Send_Post_Request(int cold, int hot, int alarm_interval) {
	String httpRequestData = "api_key=" + apiKeyValue + "&user_id=" + user_id
	+ "&user_location=" + user_location + "&cold=" + String(cold)
	+ "&hot=" + String(hot) + "&alarm_time=" + String(alarm_interval) + ""; 


	wifiClient.print(String("POST ") + scriptName + " HTTP/1.1\r\n" +
		"Host: " + serverName + "\r\n" +
		"Content-Type: application/x-www-form-urlencoded"+ "\r\n" +
		"Content-Length: " + httpRequestData.length() + "\r\n" + 
		"\r\n" + httpRequestData + "\r\n" +
		"Connection: close\r\n\r\n");	
}
//====================================================================

bool ConnectToWifi(void) {
	bool conn = 1;
	if (SERIAL_PRINT_DEBUG_EN) {
		Serial.begin(9600);
		Serial.println();
		Serial.print("Connecting to ");
		Serial.println(ssid);
	}
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);   // ========== <<---
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		if (SERIAL_PRINT_DEBUG_EN) {
			Serial.print(".");
		}
	}
	if (SERIAL_PRINT_DEBUG_EN) {
		Serial.println("");
		delay(10);
		Serial.println("WiFi connected");
		delay(10);
		Serial.println("IP address: ");
		Serial.println(WiFi.localIP());
	}
	conn = WiFi.status();
	return conn;
}
//====================================================================

void GetCurrTime(void) {
	//==========
	if (SERIAL_PRINT_DEBUG_EN) {
		delay(100);
	}
	//==========
	// Set time via NTP, as required for x.509 validation
	configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
	//==========
	
	if (SERIAL_PRINT_DEBUG_EN) {
		Serial.print("Waiting for NTP time sync: ");
	}	
	//==========

	now = time(nullptr);
	
	while (now < 8 * 3600 * 2) {
		delay(500);
		if (SERIAL_PRINT_DEBUG_EN) {
			Serial.print(".");
		}
		now = time(nullptr);
	}	
	//==========	
	if (SERIAL_PRINT_DEBUG_EN) {	
		Serial.println("");
	}
	//==========
	gmtime_r(&now, &timeinfo);	

	//==========	
	if (SERIAL_PRINT_DEBUG_EN) {
		delay(100);			
		Serial.println("");
		delay(100);
		Serial.print(asctime(&timeinfo));  // ========== <<---
	} else {
		asctime(&timeinfo);		          // ========== <<---
	}
	//==========	
}
//====================================================================

bool ConnectToHTTPSserver(void) {
	bool isServerConnect = 1;
	//==========
	if (SERIAL_PRINT_DEBUG_EN) {
		delay(100);
		Serial.printf("Using certificate: %s\n", ISRG_Root_X1);
	}
	//==========
	wifiClient.setTrustAnchors(&certISRG);
	//==========
	if (SERIAL_PRINT_DEBUG_EN) {
		delay(100);
		Serial.print("Connecting to ");
		Serial.println(serverName);
		delay(100);	
	}
	//==========
    isServerConnect = wifiClient.connect(serverName, serverPort); // ========== <<---
	if (SERIAL_PRINT_DEBUG_EN) {
		if (isServerConnect == 0) {
			Serial.println("Connection failed");
		} 
		else {
			Serial.println("Connection Ok");				
		}
		delay(100);
	}
	//==========

	return  isServerConnect;
}
//====================================================================

int GetCold(void) {
	char buf_cold[80] = {0};
	int cold = -1;
	//==================
	Serial_Flush_Buffer();
	//==================
	for (int n=0; n<10; n++) {
		Serial.print("get_cold");
		delay(100);
		if (readline(Serial.read(), buf_cold, 80) > 0) {
			cold  = atoi(buf_cold);
			//==================
			if (SERIAL_PRINT_DEBUG_EN) {
				Serial.print(cold);
			}
			//==================
			break;
		}	
	}
	return cold;
}
//====================================================================

int GetHot(void) {
	char buf_hot[80] = {0};
	int hot = -1;
	//==================
	Serial_Flush_Buffer();
	//==================	
	for (int n=0; n<10; n++) {
		Serial.print("get_hot");
		delay(100);
		if (readline(Serial.read(), buf_hot, 80) > 0) {
			hot  = atoi(buf_hot);
			//==================
			if (SERIAL_PRINT_DEBUG_EN) {
				Serial.print(hot);
			}
			//==================
			break;
		}	
	}
	return hot;
}
//====================================================================
void Serial_Flush_Buffer(void) {
    while (Serial.available()){
		  Serial.read();
    }
}
