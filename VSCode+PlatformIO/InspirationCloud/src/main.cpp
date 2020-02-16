#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>

#include <Ardugram.h>

// extern "C" {
//   #include "user_interface.h"
//   #include "wpa2_enterprise.h"
// }

String ssid = "From_Siberia_With_Love";                         // REPLACE mySSID WITH YOUR WIFI SSID
String pass = "qwerty1480";                                     // REPLACE myPassword YOUR WIFI PASSWORD, IF ANY
String token = "571169334:AAEr3G6dtKkEtXMBRusBd9yAklLYw2QhgzY"; // REPLACE myToken WITH YOUR TELEGRAM BOT TOKEN

Ardugram myBot(token);

//https://github.com/esp8266/Arduino/issues/1032#issuecomment-285314332
// // SSID to connect to
// static const char* ssid = "MY_SSID";
// // Username for authentification
// static const char* username = "user1";
// // Password for authentification
// static const char* password = "secret";

void setup()
{
  // initialize the Serial
  Serial.begin(74880);

  // wifi_set_opmode(STATION_MODE);

  // struct station_config wifi_config;

  // memset(&wifi_config, 0, sizeof(wifi_config));
  // strcpy((char*)wifi_config.ssid, ssid);

  // wifi_station_set_config(&wifi_config);

  // wifi_station_clear_cert_key();
  // wifi_station_clear_enterprise_ca_cert();
  
  // wifi_station_set_wpa2_enterprise_auth(1);
  // wifi_station_set_enterprise_username((uint8*)username, strlen(username));
  // wifi_station_set_enterprise_password((uint8*)password, strlen(password));

  // wifi_station_connect();

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