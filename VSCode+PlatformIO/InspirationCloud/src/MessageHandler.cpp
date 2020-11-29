#include <MessageHandler.h>

MessageHandler::MessageHandler()
{
    bot.waitForResponse = 500;

    cmdStart = cli.addCmd(CMD_START);

    cmdHelp = cli.addCmd(CMD_HELP);

    cmdLed = cli.addCmd(CMD_LED);

    cmdLed.addFlagArg(BLINK_ARG);
    cmdLed.addFlagArg(FILL_ARG);
    cmdLed.addFlagArg(RAINBOW_ARG);
    cmdLed.addFlagArg(STOP_ARG);
    cmdLed.addFlagArg(HELP_ARG);

    cmdLed.addArg(COLOR_ARG, DEFAULT_CMD_VALUE);
    cmdLed.addArg(COUNT_ARG, DEFAULT_CMD_VALUE);

    clientSecure.setInsecure();
}

bool MessageHandler::isArgsNotSet(Command *cmd)
{

#if DEBUG_MODE == 1
    Serial.println("****");
#endif

    for (int j = 0; j < cmd->countArgs(); j++)
    {

#if DEBUG_MODE == 1
        Serial.println("DEBUG: Is Set: " + String(cmd->getArg(j).isSet()));
        Serial.println("DEBUG: Is Default value:" + String(cmd->getArg(j).getValue() == DEFAULT_CMD_VALUE) + " ; " + String(cmd->getArg(j).getValue()));
        Serial.println("DEBUG: Is Value NULL: " + String(cmd->getArg(j).getValue() == NULL) + " or just empty: " + String(cmd->getArg(j).getValue().isEmpty()));
#endif

        if (cmd->getArg(j).getValue().isEmpty())
        {
            if (cmd->getArg(j).isSet())
            {
                return false;
                break;
            }
        }
        else
        {
            if (cmd->getArg(j).getValue() != DEFAULT_CMD_VALUE)
            {
                return false;
                break;
            }
        }
    }

#if DEBUG_MODE == 1
    Serial.println("****");
#endif

    return true;
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

        if (cli.errored())
        {
            CommandError cmdError = cli.getError();

            String errorMessage = "ERROR: " + cmdError.toString() + "\n";

            // if (cmdError.hasCommand())
            // {
            //     errorMessage += "Did you mean \"" + cmdError.getCommand().toString() + "\"?\n";
            // }

            bot.sendMessage(chat_id, errorMessage, DEFAULT_PARSE_MODE);
            Utils::deleteParsedMessage(parsedMessage);
            break;
        }

        if (cli.available())
        {
            Command c = cli.getCmd();

#if DEBUG_MODE == 1
            Serial.print("DEBUG: > ");
            Serial.print(c.getName());
            Serial.print(' ');

            for (int i = 0; i < c.countArgs(); ++i)
            {
                Argument arg = c.getArgument(i);
                Serial.print(arg.toString());
                Serial.print(' ');
            }

            Serial.println();
#endif

            parsedMessage->root = c.getName();

            if (c == cmdStart || c == cmdHelp) // /start or /help is called
            {
                bot.sendMessage(chat_id, buildMenu(&bot.messages[i]), DEFAULT_PARSE_MODE);
                Utils::deleteParsedMessage(parsedMessage);
                break;
            }
            else if (c == cmdStatus) // /status is called
            {
                Utils::deleteParsedMessage(parsedMessage);
                //TODO: get current status logic
                break;
            }
            else if (c == cmdLed) // /led is called
            {
                if (c.getArg(HELP_ARG).isSet() || isArgsNotSet(&c)) // -help or no args is called
                {
                    bot.sendMessage(chat_id, buildLedHelp(), DEFAULT_PARSE_MODE);
                    Utils::deleteParsedMessage(parsedMessage);
                    break;
                }
                else if (c.getArg(FILL_ARG).isSet()) // -fill is called
                {

                    parsedMessage->command = FILL_ARG;

                    Argument fillColor = c.getArg(COLOR_ARG);
                    if (fillColor.getValue() != DEFAULT_CMD_VALUE)
                    {
                        Option colorOption;
                        colorOption.option = COLOR_ARG;
                        colorOption.value = fillColor.getValue();

                        parsedMessage->options = new LinkedList<Option>();
                        parsedMessage->options->add(colorOption);
                        parsedMessage->systemStatus = 1;

                        bot.sendMessage(chat_id, "LED will be filled with " + fillColor.getValue() + " color", DEFAULT_PARSE_MODE);
                    }
                    else
                    {
                        bot.sendMessage(chat_id, "Please provide color to fill with -" + String(COLOR_ARG) + " option.\nExample: " + String(CMD_LED) + " -" + String(FILL_ARG) + " -" + String(COLOR_ARG) + " RED", DEFAULT_PARSE_MODE);
                        Utils::deleteParsedMessage(parsedMessage);
                        break;
                    }
                }
                else if (c.getArg(RAINBOW_ARG).isSet()) // -rainbow is called
                {
                    parsedMessage->command = RAINBOW_ARG;
                    parsedMessage->systemStatus = 2;
                    bot.sendMessage(chat_id, "RAINBOW!");
                }
                else if (c.getArg(STOP_ARG).isSet()) // -stop is called
                {
                    parsedMessage->command = STOP_ARG;
                    parsedMessage->systemStatus = 0;
                    bot.sendMessage(chat_id, "Switching LEDs off.");
                }
            }
            else
            {
                bot.sendMessage(chat_id, "Unknown command.\nRun " + String(CMD_START) + " or " + String(CMD_HELP) + " to get list of valid commands.", DEFAULT_PARSE_MODE);
                Utils::deleteParsedMessage(parsedMessage);
                break;
            }

#if DEBUG_MODE == 1
            Serial.println("DEBUG: Adding to List: " + parsedMessage->root);
#endif

            parsedMessages->add(*parsedMessage);

#if DEBUG_MODE == 1
            Serial.println("DEBUG: parsedMessages list size: " + String(parsedMessages->size()));
#endif
        }
    }

    if (parsedMessages->size() == 0)
    {
        Utils::deleteParsedMessageList(parsedMessages);
        return NULL;
    }

    return parsedMessages;
}

String MessageHandler::buildMenu(telegramMessage *currentMessage)
{
    String welcome = "Hello there " + currentMessage->from_name + "!\n";
    welcome += "Welcome to Inspiration cloud.\n";
    welcome += "\n\n";
    welcome += String(CMD_LED) + " : for led commands\n";
    welcome += String(CDM_STATUS) + " : Returns current status of inspiration cloud\n";
    welcome += String(CMD_START) + " or " + String(CMD_HELP) + " : to show this menu";

    return welcome;
}

String MessageHandler::buildLedHelp()
{
    String ledHelp = String(CMD_LED) + " is for playing with LED in inspiration cloud.\n";
    ledHelp += "Usage:\n";
    ledHelp += String(CMD_LED) + " \\[command] \\[option] <value>\n\n";
    ledHelp += "Commands:\n";
    //ledHelp += "-blink   blink with specified color specified numbmer of times. Options: -color, -count\n";
    ledHelp += "-" + String(FILL_ARG) + " switch on LED with specified color. Options: -" + String(COLOR_ARG) + "\n";
    ledHelp += "-" + String(RAINBOW_ARG) + " switch on or off LED rainbow.\n";
    ledHelp += "-" + String(STOP_ARG) + " switch off LEDs or stop " + String(CMD_LED) + " action.\n";
    ledHelp += "-" + String(HELP_ARG) + " show this help\n\n";
    ledHelp += "Options:\n";
    ledHelp += "-" + String(COLOR_ARG) + " HEX or text color value\n";
    //ledHelp += "-count   integer number\n";

    return ledHelp;
}