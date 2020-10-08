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

            if (c == cmdStart)
            {
                bot.sendMessage(chat_id, buildMenu(bot.messages[i]), "Markdown");
                //currentParsedMessage.root = "/start";
                //currentParsedMessage.systemStatus = 0;
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