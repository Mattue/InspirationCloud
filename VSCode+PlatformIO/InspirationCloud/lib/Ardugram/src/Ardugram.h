#pragma once
#ifndef Ardugram_h
#define Ardugram_h

#define ARDUINOJSON_USE_LONG_LONG 1 //using int_64 data

//clean includes
#include <Arduino.h>
#include <ArduinoJson.h>

#include "ArdugramDataStructures/SendMessageRequest.h"

#define CONFIG_JSON_SIZE 200 //Size json config ----
#define DEBUG_MODE 1 // enable/disable debug mode. Pring debug data to serial port
#define USE_HTTPS 0 // enable/disable https/http connection. For HTTPS fingerprint must be provided
#define TELEGRAM_API_URI /telegram-api/bot

class Ardugram
{
    using JsonDocument = StaticJsonDocument<200>;

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