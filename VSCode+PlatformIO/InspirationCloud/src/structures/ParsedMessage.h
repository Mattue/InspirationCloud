#pragma once
#ifndef ParsedMessage_h
#define ParsedMessage_h

#include <Arduino.h>
#include <UniversalTelegramBot.h>
#include <structures\Option.h>
#include <LinkedList.h>

struct ParsedMessage {
    String root;
    String command;
    LinkedList<Option> * options;
    int systemStatus;
};

#endif