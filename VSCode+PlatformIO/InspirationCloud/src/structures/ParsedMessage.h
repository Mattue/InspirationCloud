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
    int systemStatus; //TODO: document all avaliable statuses

    /*
    returning statuses:
        0 - idle
        1 - led filled
        2 - led rainbow operating
    */
};

#endif