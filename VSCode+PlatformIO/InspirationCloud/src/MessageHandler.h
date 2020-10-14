#pragma once

#ifndef MessageHandler_h
#define MessageHandler_h

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <UniversalTelegramBot.h>
#include <SimpleCLI.h>
#include <structures\ParsedMessage.h>
#include <LinkedList.h>
#include <utils\Utils.h>

#define DEFAULT_PARSE_MODE "Markdown"

#define CMD_START "/start"
#define CMD_LED "/led"
#define CMD_HELP "/help"
#define CDM_STATUS "/status" //TODO

#define HELP_ARG "help"
#define BLINK_ARG "blink" //TODO
#define FILL_ARG "fill"
#define STOP_ARG "stop"
#define RAINBOW_ARG "rainbow"

#define COLOR_ARG "color"
#define COUNT_ARG "count"

#define DEFAULT_CMD_VALUE "value"

class MessageHandler
{

private:
    WiFiClientSecure clientSecure;
    UniversalTelegramBot bot = UniversalTelegramBot(BOT_TOKEN, clientSecure);

    SimpleCLI cli;
    Command cmdStart;
    Command cmdHelp;
    Command cmdLed;
    Command cmdStatus;

    static String buildMenu(telegramMessage *currentMessage);
    static String buildLedHelp();
    static bool isArgsNotSet(Command *cmd);

public:
    MessageHandler();
    LinkedList<ParsedMessage> *handleMessages();
};

#endif