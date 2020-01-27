#include <Arduino.h>
#include <CTBot.h>

CTBot myBot;

String ssid = "From_Siberia_With_Love";                         // REPLACE mySSID WITH YOUR WIFI SSID
String pass = "qwerty1480";                                     // REPLACE myPassword YOUR WIFI PASSWORD, IF ANY
String token = "571169334:AAEr3G6dtKkEtXMBRusBd9yAklLYw2QhgzY"; // REPLACE myToken WITH YOUR TELEGRAM BOT TOKEN

void setup()
{
  // initialize the Serial
  Serial.begin(74880);
  Serial.println("Starting TelegramBot...");

  // connect the ESP8266 to the desired access point
  myBot.wifiConnect(ssid, pass);
  Serial.println("Connected to WiFi");

  myBot.useDNS(false);

  // set the telegram bot token
  myBot.setTelegramToken(token);

  // check if all things are ok
  if (myBot.testConnection())
    Serial.println("\ntestConnection OK");
  else
    Serial.println("\ntestConnection NOK");
}

void loop()
{
  // a variable to store telegram message data
  TBMessage msg;

  // if there is an incoming message...
  if (myBot.getNewMessage(msg))
    // ...forward it to the sender
    myBot.sendMessage(msg.sender.id, msg.text);

  // wait 500 milliseconds
  delay(500);
}