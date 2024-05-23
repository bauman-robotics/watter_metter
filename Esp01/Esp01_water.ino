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

#include <SoftwareSerial.h>; ///

#define BUFFER_ANSWER (3)
#define SEALEVELPRESSURE_HPA (1013.25)

//SoftwareSerial mySerial(0, 2); // 0 - к TX сенсора, 2 - к RX  ///
//SoftwareSerial mySerial(2, 3); // 2 - к TX сенсора, 3 - к RX  ///

byte cmd[] = "get_cold"; 
unsigned char response[9]; ///
unsigned int ppm = 0;
///-------------------------------

// Replace with your network credentials
const char* ssid     = "Bauman_Robotics";
const char* password = "******";

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "http://84.252.143.212/watter_post.php";
 
// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key 

String user_id = "Andrey";
String user_location = "Home";
String apiKeyValue = "******";
int cold = 1;
int hot = 1;
int alarm_time = 1245;

uint8_t readStatus = 0;

void setup() {
  Serial.begin(9600);
  //mySerial.begin(9600);   
  
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

void loop() {
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;
    
    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    //-----------------------------
    Serial.write(cmd, strlen(cmd));
    memset(response, 0, BUFFER_ANSWER);
    Serial.readBytes(response, BUFFER_ANSWER);

	//Serial.println("Ok");
    //------------------------------
    
    // Prepare your HTTP POST request data
	
    String httpRequestData = "api_key=" + apiKeyValue + "&user_id=" + user_id
                          + "&user_location=" + user_location + "&cold=" + String(cold)
                          + "&hot=" + String(hot) + "&alarm_time=" + String(alarm_time) + "";
                          
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
	
	cold ++;
	hot ++;
	alarm_time ++;
    
    // You can comment the httpRequestData variable above
    // then, use the httpRequestData variable below (for testing purposes without the BME280 sensor)
    //String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&location=Office&value1=24.75&value2=49.54&value3=1005.14";

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
     
    // If you need an HTTP request with a content type: text/plain
    //http.addHeader("Content-Type", "text/plain");
    //int httpResponseCode = http.POST("Hello, World!");
    
    // If you need an HTTP request with a content type: application/json, use the following:
    //http.addHeader("Content-Type", "application/json");
    //int httpResponseCode = http.POST("{\"value1\":\"19\",\"value2\":\"67\",\"value3\":\"78\"}");
        
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }

  //Send an HTTP POST request every 10 seconds
  delay(10000); 
}
