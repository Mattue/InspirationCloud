#include <MessageHandler.h>

/*
returning statuses:
    0 - idle
    1 - led runing operations
*/

MessageHandler::MessageHandler()
{
    cmdStart = cli.addCmd("/start");
    cmdStart.setDescription("Start communication with bot");

    cmdHelp = cli.addCmd("/help");
    cmdHelp.setDescription("Show help for this system");

    cmdLed = cli.addCmd("/led");
    cmdLed.setDescription("/led is for playing with LED in inspiration cloud.");
    // cmdLed.addArg("blink");
    cmdLed.addFlagArg("fill");
    // cmdLed.addArg("rainbow");
    cmdLed.addFlagArg(HELP_ARG);
    // cmdLed.addArg("c/color");
    // cmdLed.addArg("n/count");

    clientSecure.setInsecure();
}

void MessageHandler::handleMessages()
{

    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

#if DEBUG_MODE == 1
    Serial.print("Number of messages to handle: ");
    Serial.println(String(numNewMessages));
#endif

    for (int i = 0; i < numNewMessages; i++)
    {
#if DEBUG_MODE == 1
        Serial.println("got response");
#endif

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

            if (c == cmdStart || c == cmdHelp)
            {
                bot.sendMessage(chat_id, buildMenu(bot.messages[i]), DEFAULT_PARSE_MODE);
                //currentParsedMessage.root = "/start";
                //currentParsedMessage.systemStatus = 0;
            }
            else if (c == cmdLed)
            {
                if (c.getArg(HELP_ARG).isSet() || argNum == 2)
                {
                    bot.sendMessage(chat_id, buildLedHelp(), DEFAULT_PARSE_MODE);
                }
            }
            else
            {
                bot.sendMessage(chat_id, "Unknown command.\nRun /start or /help to get list of valid commands.", "Markdown");
                //currentParsedMessage.root = "unknown";
                //currentParsedMessage.systemStatus = 0;
            }
        }

        if (cli.errored())
        {
            CommandError cmdError = cli.getError();

            String errorMessage = "ERROR: " + cmdError.toString() + "\n";

            if (cmdError.hasCommand())
            {
                errorMessage += "Did you mean \"" + cmdError.getCommand().toString() + "\"?\n";
            }
        }
    }
}

String MessageHandler::buildMenu(telegramMessage currentMessage)
{
    String welcome = "Hello there " + currentMessage.from_name + "!\n";
    welcome += "Welcome to Inspiration cloud.\n";
    welcome += "\n\n";
    welcome += "/led : for led commands\n";
    welcome += "/status : Returns current status of inspiration cloud\n";

    return welcome;
}

String MessageHandler::buildLedHelp()
{
    String ledHelp = "/led is for playing with LED in inspiration cloud.\n";
    ledHelp += "Usage:\n";
    ledHelp += "/led [command] [option] [value]\n\n";
    ledHelp += "Commands:\n";
    ledHelp += "-blink   blink with specified color specified numbmer of times. Options: -color, -count\n";
    ledHelp += "-fill   switch on LED with specified color. Options: -color\n";
    ledHelp += "-rainbow switch on or off LED rainbow.\n";
    ledHelp += "-help    show this help\n\n";
    ledHelp += "Options:\n";
    ledHelp += "-color  HEX, or text color value\n";
    ledHelp += "-count  integer number\n";

    return ledHelp;
}