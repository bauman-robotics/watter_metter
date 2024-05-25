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
int alarm_time = 1245;
		
char buf_cold[80];
char buf_hot[80];

int readline(uint8_t readch, char *buffer, int len);
//====================================================================

void setup() {
  Serial.begin(9600);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}
//====================================================================

void loop() {
	//Check WiFi connection status
	if ( WiFi.status() == WL_CONNECTED ) {
		WiFiClient client;
		HTTPClient http;

		// Your Domain name with URL path or IP address with path
		http.begin(client, serverName);

		// Specify content-type header
		http.addHeader("Content-Type", "application/x-www-form-urlencoded");
		//======================================
		cold = -1;
		hot = -1;
		
		int httpResponseCode = 200;
		
		delay(1000);
		for (int n=0; n<10; n++) {
			Serial.write("get_cold", 8);
			if (readline(Serial.read(), buf_cold, 80) > 0) {
				cold  = atoi(buf_cold);
				//Serial.write(buf, strlen(buf));
				break;
			}	
			delay(100);
		}

		delay(1000);

		for (int n=0; n<10; n++) { 
			Serial.write("get_hot", 7);
			if (readline(Serial.read(), buf_hot, 80) > 0) {
				hot  = atoi(buf_hot);
				//Serial.write(buf, strlen(buf));
				break;
			}
			delay(100);
		}

		//======================================
		if ((cold != -1) && (hot != -1)) {
			// Prepare your HTTP POST request data
			String httpRequestData = "api_key=" + apiKeyValue + "&user_id=" + user_id
						  + "&user_location=" + user_location + "&cold=" + String(cold)
						  + "&hot=" + String(hot) + "&alarm_time=" + String(alarm_time) + ""; 
						  
			// Send HTTP POST request
			httpResponseCode = http.POST(httpRequestData); 
			  
			alarm_time ++;     

			//======================================

			http.end();
		}
		
		
		if (httpResponseCode==200) {
			delay(1000);
			Serial.write("set_time0", 9);
			delay(1000);
			Serial.write("set_alarm_time100", 17);
			delay(1000);			
			Serial.write("esp_force_off", 13);
		} 
		
	}
	else {
		Serial.println("WiFi Disconnected");
	}

	//Send an HTTP POST request every 10 seconds
	delay(5000); 
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
		if (pos >= len-1) {
			pos = 0;
			return 0;
		}			
		//===================
    }
    return 0;
}
