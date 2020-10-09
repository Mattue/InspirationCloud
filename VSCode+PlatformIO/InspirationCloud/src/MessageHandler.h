#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <UniversalTelegramBot.h>
#include <SimpleCLI.h>
#include <structures\ParsedMessage.h>
#include <LinkedList.h>

#define DEFAULT_PARSE_MODE "Markdown"

#define HELP_ARG "help"
#define FILL_ARG "fill"
#define COLOR_ARG "color"
#define BLINK_ARG "blink"
#define RAINBOW_ARG "rainbow"
#define COUNT_ARG "count"
#define DEFAULT_OPTIONAL "-1"

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

    static String buildMenu(telegramMessage currentMessage);
    static String buildLedHelp();

public:
    MessageHandler();
    LinkedList<ParsedMessage> * handleMessages();
    // void handleMessages(LinkedList<int> * parsedMessages);
};