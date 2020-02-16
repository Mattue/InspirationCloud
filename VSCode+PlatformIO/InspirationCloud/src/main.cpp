#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>

#include <Ardugram.h>

String ssid = "From_Siberia_With_Love";                         // REPLACE mySSID WITH YOUR WIFI SSID
String pass = "qwerty1480";                                     // REPLACE myPassword YOUR WIFI PASSWORD, IF ANY
String token = "571169334:AAEr3G6dtKkEtXMBRusBd9yAklLYw2QhgzY"; // REPLACE myToken WITH YOUR TELEGRAM BOT TOKEN

Ardugram myBot(token);

void setup()
{
  // initialize the Serial
  Serial.begin(74880);

  // connect the ESP8266 to the desired access point
  Serial.print("Connecting to WiFi ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  delay(500);
  WiFi.begin(ssid, pass);
	delay(500);

  if (WiFi.waitForConnectResult() == WL_CONNECTED) {
    IPAddress ip = WiFi.localIP();
		Serial.print("Connected to WiFi. IP address: ");
    Serial.println(ip);
  } else {
    Serial.println("Failed to connect to WiFi. Resetting ESP");
    delay(5000);
    ESP.restart();
  }

  // myBot.useDNS(false);

  // // set the telegram bot token
  // myBot.setTelegramToken(token);

  // // check if all things are ok
  // if (myBot.testConnection())
  //   Serial.println("\ntestConnection OK");
  // else
  //   Serial.println("\ntestConnection NOK");
}

void loop()
{
  // // a variable to store telegram message data
  // TBMessage msg;

  // // if there is an incoming message...
  // if (myBot.getNewMessage(msg))
  //   // ...forward it to the sender
  //   myBot.sendMessage(msg.sender.id, msg.text);

  // // wait 500 milliseconds
  // delay(500);

  String output;
  serializeJson(myBot.getMe(), output);

  //myBot.getMe();

  Serial.println(output);
  delay(3000);
}