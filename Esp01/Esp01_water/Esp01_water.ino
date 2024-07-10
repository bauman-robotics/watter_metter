/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-mysql-database-php/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

*/

#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define SERIAL_PRINT_DEBUG_EN (1)

//#include <SoftwareSerial.h>; ///
///-------------------------------

// Replace with your network credentials
const char* ssid     = "zzzzzz";
const char* password = "vvvvvv";

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "http://xx.xx.xx.xx/watter_post.php";
 
// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key 

String user_id = "Andrey";
String user_location = "Home";
String apiKeyValue = "xxxxxx";
int cold = -1;
int hot = -1;
int alarm_interval   = 200; // 200 - 2 min, 00 sec
		
char buf_cold[80];
char buf_hot[80];

unsigned long wakeUpTime;
time_t now; 
struct tm timeinfo;

WiFiClient client;
HTTPClient http;

int readline(uint8_t readch, char *buffer, int len);
int GetCold(void); 
int GetHot(void);
void Serial_Flush_Buffer(void);
int Send_Post_Request(int cold, int hot, int alarm_interval);
bool ConnectToWifi(void);
//====================================================================

void setup() {
	bool conn = 0;
	char buf[100];
	Serial_Flush_Buffer();
	conn = ConnectToWifi();
	int httpResponseCode = 0;
	if (conn) {
		cold = GetCold();		
		delay(100);
		Serial_Flush_Buffer();
		hot = GetHot();
		
		httpResponseCode = Send_Post_Request(cold, hot, alarm_interval);		
		
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

bool ConnectToWifi(void) {
	bool conn = 1;
	
	if (SERIAL_PRINT_DEBUG_EN) {
		Serial.begin(9600);
		Serial.println();
		Serial.print("Connecting to ");
		Serial.println(ssid);	
	}
	
	WiFi.begin(ssid, password);
	
	while(WiFi.status() != WL_CONNECTED) { 
		delay(500);
		if (SERIAL_PRINT_DEBUG_EN) {
			Serial.print(".");
		}
	}
	
	if (SERIAL_PRINT_DEBUG_EN) {	
		Serial.println("");
		Serial.print("Connected to WiFi network with IP Address: ");
		Serial.println(WiFi.localIP());
	}
	
	conn = WiFi.status();
	return conn;
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
//====================================================================

int Send_Post_Request(int cold, int hot, int alarm_interval) {
	int httpResponseCode = 200;
	
	// Your Domain name with URL path or IP address with path
	http.begin(client, serverName);

	// Specify content-type header
	http.addHeader("Content-Type", "application/x-www-form-urlencoded");

	//======================================

	// Prepare your HTTP POST request data
	String httpRequestData = "api_key=" + apiKeyValue + "&user_id=" + user_id
				  + "&user_location=" + user_location + "&cold=" + String(cold)
				  + "&hot=" + String(hot) + "&alarm_time=" + String(alarm_interval) + ""; 
				  
	// Send HTTP POST request
	httpResponseCode = http.POST(httpRequestData); 

	//======================================

	http.end();

	return httpResponseCode;

}
//====================================================================
