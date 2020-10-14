#include <MessageHandler.h>

/*
returning statuses:
    0 - idle
    1 - led runing operations
*/

MessageHandler::MessageHandler()
{
    bot.waitForResponse = 500;

    cmdStart = cli.addCmd("/start");
    cmdStart.setDescription("Start communication with bot");

    cmdHelp = cli.addCmd("/help");
    cmdHelp.setDescription("Show help for this system");

    cmdLed = cli.addCmd("/led");
    cmdLed.setDescription("/led is for playing with LED in inspiration cloud.");
    cmdLed.addFlagArg(BLINK_ARG);
    cmdLed.addFlagArg(FILL_ARG);
    cmdLed.addFlagArg(RAINBOW_ARG);
    cmdLed.addFlagArg(HELP_ARG);
    cmdLed.addArg(COLOR_ARG, DEFAULT_OPTIONAL);
    cmdLed.addArg(COUNT_ARG, DEFAULT_OPTIONAL);

    clientSecure.setInsecure();
}

LinkedList<ParsedMessage> *MessageHandler::handleMessages()
{

    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

#if DEBUG_MODE == 1
    Serial.print("Number of messages to handle: ");
    Serial.println(String(numNewMessages));
#endif

    LinkedList<ParsedMessage> *parsedMessages = new LinkedList<ParsedMessage>();

    for (int i = 0; i < numNewMessages; i++)
    {

#if DEBUG_MODE == 1
        Serial.println("got response");
#endif

        ParsedMessage *parsedMessage = new ParsedMessage();

        String chat_id = String(bot.messages[i].chat_id);
        String text = bot.messages[i].text;
        text.trim();

        String from_name = bot.messages[i].from_name;
        if (from_name == "")
            from_name = "Guest";

        cli.parse(text);

        if (cli.available())
        {
            Command c = cli.getCmd();

            int argNum = c.countArgs();

#if DEBUG_MODE == 1
            Serial.print("> ");
            Serial.print(c.getName());
            Serial.print(' ');

            for (int i = 0; i < argNum; ++i)
            {
                Argument arg = c.getArgument(i);
                Serial.print(arg.toString());
                Serial.print(' ');
            }

            Serial.println();
#endif

            parsedMessage->root = c.getName();

            if (c == cmdStart || c == cmdHelp)
            {
                bot.sendMessage(chat_id, buildMenu(bot.messages[i]), DEFAULT_PARSE_MODE);
                // parsedMessage.systemStatus = 0;
            }
            else if (c == cmdLed)
            {
                if (c.getArg(HELP_ARG).isSet()) // -help is called
                {
                    bot.sendMessage(chat_id, buildLedHelp(), DEFAULT_PARSE_MODE);
                    parsedMessage->command = HELP_ARG;
                }
                else if (c.getArg(FILL_ARG).isSet()) // -fill is called
                {

                    parsedMessage->command = FILL_ARG;

                    Argument fillColor = c.getArg(COLOR_ARG);
                    if (fillColor.getValue() != DEFAULT_OPTIONAL)
                    {
                        Option colorOption;
                        colorOption.option = COLOR_ARG;
                        colorOption.value = fillColor.getValue();

                        parsedMessage->options = new LinkedList<Option>();
                        parsedMessage->options->add(colorOption);

                        bot.sendMessage(chat_id, "LED will be filled with " + fillColor.getValue() + " color", DEFAULT_PARSE_MODE);
                    }
                    else
                    {
                        bot.sendMessage(chat_id, "Please provide color to fill with -color option.\nExample: /led -fill -color RED", DEFAULT_PARSE_MODE);
                    }
                }
                else if (c.getArg(RAINBOW_ARG).isSet())
                {
                    parsedMessage->command = RAINBOW_ARG;
                    bot.sendMessage(chat_id, "RAINBOW!");
                }
            }
            else
            {
                bot.sendMessage(chat_id, "Unknown command.\nRun /start or /help to get list of valid commands.", DEFAULT_PARSE_MODE);
                parsedMessage->root = "unknown";
                // currentParsedMessage.systemStatus = 0;
            }

            Serial.println("Adding to List: " + parsedMessage->root);

            parsedMessages->add(*parsedMessage);
            delete (parsedMessage);

            Serial.println("parsedMessages list size: " + String(parsedMessages->size()));
        }

        if (cli.errored())
        {
            CommandError cmdError = cli.getError();

            String errorMessage = "ERROR: " + cmdError.toString() + "\n";

            // if (cmdError.hasCommand())
            // {
            //     errorMessage += "Did you mean \"" + cmdError.getCommand().toString() + "\"?\n";
            // }

            bot.sendMessage(chat_id, errorMessage, DEFAULT_PARSE_MODE);
        }
    }

    //     if(parsedMessages->size() == 0) {
    //         delete(parsedMessages);
    //         //parsedMessages.~LinkedList();
    //     }

    Serial.println("parsedMessages list size before return: " + String(parsedMessages->size()));

    return parsedMessages;
}

String MessageHandler::buildMenu(telegramMessage currentMessage)
{
    String welcome = "Hello there " + currentMessage.from_name + "!\n";
    welcome += "Welcome to Inspiration cloud.\n";
    welcome += "\n\n";
    welcome += "/led : for led commands\n";
    welcome += "/status : Returns current status of inspiration cloud\n";
    welcome += "/start : to show this menu";

    return welcome;
}

String MessageHandler::buildLedHelp()
{
    String ledHelp = "/led is for playing with LED in inspiration cloud.\n";
    ledHelp += "Usage:\n";
    ledHelp += "/led \\[command\\] \\[option\\] <value>\n\n";
    ledHelp += "Commands:\n";
    //ledHelp += "-blink   blink with specified color specified numbmer of times. Options: -color, -count\n";
    ledHelp += "-fill    switch on LED with specified color. Options: -color\n";
    ledHelp += "-rainbow switch on or off LED rainbow.\n";
    ledHelp += "-help    show this help\n\n";
    ledHelp += "Options:\n";
    ledHelp += "-color   HEX or text color value\n";
    //ledHelp += "-count   integer number\n";

    return ledHelp;
}