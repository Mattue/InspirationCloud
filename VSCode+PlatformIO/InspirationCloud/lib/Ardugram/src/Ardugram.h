#pragma once
#ifndef Ardugram_h
#define Ardugram_h

#define ARDUINOJSON_USE_LONG_LONG 1 //using int_64 data

//clean includes
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Socks5Proxy.h>

//#ifndef UNSECURE_CLIENT
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
//#define UNSECURE_CLIENT WiFiClient
//#endif

#include "Utils.h"

#include "ArdugramDataStructures/SendMessageRequest.h"

#ifndef CONFIG_JSON_SIZE
#define CONFIG_JSON_SIZE 200 //Size json config ----
#endif

#ifndef DEBUG_MODE
#define DEBUG_MODE 1 // enable/disable debug mode. Pring debug data to serial port
#endif

#ifndef USE_HTTPS
#define USE_HTTPS 0 // enable/disable https/http connection. For HTTPS fingerprint must be provided
#endif

#ifndef USE_SOCKS5
#define USE_SOCKS5 1 //enable/disable socks5 proxy
#endif

#ifndef TELEGRAM_API_URI
#define TELEGRAM_API_URI "/telegram-api/bot"
#endif

#ifndef TELEGRAM_URL
#define TELEGRAM_URL "http://176.223.142.19" //IP of VPS nginx proxy. In soviet Russia working like this. Thiking of socks5 proxy...
#endif

#ifndef TELEGRAM_IP
#define TELEGRAM_IP "176.223.142.19" //IP of VPS nginx proxy. In soviet Russia working like this. Thiking of socks5 proxy...
#endif

#ifndef TELEGRAM_PORT
#define TELEGRAM_PORT 80
#endif

class Ardugram
{
    using JsonDocument = StaticJsonDocument<CONFIG_JSON_SIZE>;

public:
    // default constructor
    Ardugram(String token);
    // default destructor
    ~Ardugram();

    // test the connection between ESP8266 and the telegram server
    // returns true if no error occurred
    bool testConnection(void);

    // get information about the bot
    // returns JsonDocument of /getMe responce or empty JsonDoc in case of error
    JsonDocument getMe();

    // send message from bot
    // returns JsonDocument of /sendMessage responce or empty JsonDoc in case of error
    //JsonDocument sendMessage(SendMessageRequest sendMessageRequest);

    // set the new Telegram API server fingerprint overwriting the default one.
	// It can be obtained by this service: https://www.grc.com/fingerprints.htm
	// quering api.telegram.org
	// params:
	//    newFingerprint: the array of 20 bytes that contains the new fingerprint
	void setFingerprint(const uint8_t *newFingerprint);

private:

    String botApiToken; //telegram bot api token
    uint8_t   httpsFingerprint[20]; //telegram api https fingerprint https://www.grc.com/fingerprints.htm

    // send data to the serial port. It work only if the CTBOT_DEBUG_MODE is enabled.
	// message: the message to send
	inline void serialLog(String message);

    // send commands to the telegram server. For info about commands, check the telegram api https://core.telegram.org/bots/api
    // returns JsonDocument of methodName responce or null in case of error
	JsonDocument callMethod(String httpMethod, String apiMethodName, String queryParams);
};

#endif