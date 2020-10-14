#pragma once

//https://api.slack.com/authentication/basics

#ifndef ArduSlack_h
#define ArduSlack_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WebSocketsClient.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

#define SLACK_URL "https://slack.com/api/rtm.connect?token="

class ArduSlack
{
public:
    ArduSlack(const String &token);

protected:
    bool connected = false;

private:
    long nextCmdId = 1;

    String _token;
    WiFiClientSecure *client;

    WebSocketsClient webSocket;

    bool connectToSlack();
    void webSocketEvent(WStype_t type, uint8_t *payload, size_t len);
    void sendPing();
};

#endif