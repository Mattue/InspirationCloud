#pragma once
#ifndef Arduslack_h
#define Arduslack_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WebSocketsClient.h>

class Arduslack
{
    using JsonDocument = StaticJsonDocument<CONFIG_JSON_SIZE>;

public:
    // default constructor
    Arduslack(String token);
    // default destructor
    ~Arduslack();

    // set the new Telegram API server fingerprint overwriting the default one.
	// It can be obtained by this service: https://www.grc.com/fingerprints.htm
	// quering api.telegram.org
	// params:
	//    newFingerprint: the array of 20 bytes that contains the new fingerprint
	void setFingerprint(const uint8_t *newFingerprint);

    bool connectToSlack();

private:

    String botApiToken; //telegram bot api token
    uint8_t   httpsFingerprint[20]; //telegram api https fingerprint https://www.grc.com/fingerprints.htm

    // send data to the serial port. It work only if the CTBOT_DEBUG_MODE is enabled.
	// message: the message to send
	inline void serialLog(String message);

    // // send commands to the telegram server. For info about commands, check the telegram api https://core.telegram.org/bots/api
    // // returns JsonDocument of methodName responce or null in case of error
	// JsonDocument callMethod(String httpMethod, String apiMethodName, String queryParams);
};

#endif