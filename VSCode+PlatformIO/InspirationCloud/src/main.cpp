#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <ArduinoOTA.h>
#include <Led.h>
#include <MessageHandler.h>
#include <structures\ParsedMessage.h>
#include <LinkedList.h>
#include <utils\Utils.h>

#ifndef DEBUG_MODE
#define DEBUG_MODE 0
#endif

#ifndef SSID_MODE
#define SSID_MODE 0
#endif

#ifndef SSID_NAME
#define SSID_NAME "From_Siberia_With_Love"
#endif

#ifndef SSID_PASSWORD
#define SSID_PASSWORD "qwerty1480"
#endif

#ifndef BOT_TOKEN
#define BOT_TOKEN "571169334:AAEr3G6dtKkEtXMBRusBd9yAklLYw2QhgzY"
#endif

//https://github.com/esp8266/Arduino/issues/1032#issuecomment-285314332
#ifdef SSID_MODE

#if SSID_MODE == 1
extern "C"
{
#include "user_interface.h"
#include "wpa2_enterprise.h"
}

#ifndef SSID_USERNAME
#define SSID_USERNAME "user1"
#endif

#endif
#endif

#ifndef BOT_MTBS
#define BOT_MTBS = 1000
#endif

unsigned long bot_lasttime; // last time messages' scan has been done

Led leds;     //LED's controller
byte counter; //counter for LED animations

MessageHandler messageHandler;

int system_status = 0; //idle. Full list find in MessageHandler.cpp

// String getNamedOptionValue(LinkedList<Option> *options, String optionName)
// {
//   for (int i = 0; i < options->size(); i++)
//   {
//     if (options->get(i).option.equals(optionName))
//     {
//       return options->get(i).value;
//     }
//   }

//   return "";
// }

// unsigned int hexToDec(String hexString)
// {

//   if (hexString.equals(""))
//     return -1;

//   unsigned int decValue = 0;
//   int nextInt;

//   for (int i = 0; i < hexString.length(); i++)
//   {

//     nextInt = int(hexString.charAt(i));
//     if (nextInt >= 48 && nextInt <= 57)
//       nextInt = map(nextInt, 48, 57, 0, 9);
//     if (nextInt >= 65 && nextInt <= 70)
//       nextInt = map(nextInt, 65, 70, 10, 15);
//     if (nextInt >= 97 && nextInt <= 102)
//       nextInt = map(nextInt, 97, 102, 10, 15);
//     nextInt = constrain(nextInt, 0, 15);

//     decValue = (decValue * 16) + nextInt;
//   }

//   return decValue;
// }

void setup()
{
  // initialize the Serial
  Serial.begin(115200);

#if SSID_MODE == 0
// connect the ESP8266 to the desired access point
#if DEBUG_MODE == 1
  Serial.print("Connecting to WiFi: ");
  Serial.println(SSID_NAME);
#endif

  WiFi.mode(WIFI_STA);
  delay(500);
  WiFi.begin(SSID_NAME, SSID_PASSWORD);
  delay(500);

  if (WiFi.waitForConnectResult() == WL_CONNECTED)
  {
#if DEBUG_MODE == 1
    IPAddress ip = WiFi.localIP();
    Serial.print("Connected to WiFi. IP address: ");
    Serial.println(ip);
#endif
  }
  else
  {
#if DEBUG_MODE == 1
    Serial.println("Failed to connect to WiFi. Resetting ESP");
#endif
    delay(5000);
    ESP.restart();
  }
#else
//connect the ESP8266 to wpa2-enterprise
#if DEBUG_MODE == 1
  Serial.print("Connecting to WiFi: ");
  Serial.println(SSID_NAME);
#endif
  wifi_set_opmode(STATION_MODE);

  struct station_config wifi_config;

  memset(&wifi_config, 0, sizeof(wifi_config));
  strcpy((char *)wifi_config.ssid, (char *)SSID_NAME);

  wifi_station_set_config(&wifi_config);

  wifi_station_clear_cert_key();
  wifi_station_clear_enterprise_ca_cert();

  wifi_station_set_wpa2_enterprise_auth(1);
  wifi_station_set_enterprise_username((uint8 *)SSID_USERNAME, strlen(SSID_USERNAME));
  wifi_station_set_enterprise_password((uint8 *)SSID_PASSWORD, strlen(SSID_PASSWORD));

  wifi_station_connect();

#if DEBUG_MODE == 1
  // Wait for connection AND IP address from DHCP
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  IPAddress ip = WiFi.localIP();
  Serial.print("Connected to WiFi. IP address: ");
  Serial.println(ip);
#endif
#endif

  //Wireles firmare upload init
  ArduinoOTA.onStart([]() {
    Serial.println("Start of OTA update"); //  "Начало OTA-апдейта"
    leds.switchOff();
    //TODO: do LED notification
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd of OTA update"); //  "Завершение OTA-апдейта"
    //TODO: do LED notification
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    //TODO: do LED notification
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) //  "Ошибка при аутентификации"
    {
      Serial.println("Auth Failed");
      //TODO: do LED notification
    }
    else if (error == OTA_BEGIN_ERROR) //  "Ошибка при начале OTA-апдейта"
    {
      Serial.println("Begin Failed");
      //TODO: do LED notification
    }
    else if (error == OTA_CONNECT_ERROR) //  "Ошибка при подключении"
    {
      Serial.println("Connect Failed");
      //TODO: do LED notification
    }
    else if (error == OTA_RECEIVE_ERROR) //  "Ошибка при получении данных"
    {
      Serial.println("Receive Failed");
      //TODO: do LED notification
    }
    else if (error == OTA_END_ERROR) //  "Ошибка при завершении OTA-апдейта"
    {
      Serial.println("End Failed");
      //TODO: do LED notification
    }
  });
  ArduinoOTA.begin();

  pinMode(LED_BUILTIN, OUTPUT);
}

LinkedList<ParsedMessage> *myMessages;

void loop()
{
  ArduinoOTA.handle();

  if (millis() > bot_lasttime + BOT_MTBS)
  {

    LinkedList<ParsedMessage> *currentMessage;

#if DEBUG_MODE == 1
    Serial.println("Checking for updates");
#endif

    myMessages = messageHandler.handleMessages();

#if DEBUG_MODE == 1
    Serial.println("my messages size: " + String(myMessages->size()));

    for (int i = 0; i < myMessages->size(); i++)
    {
      Serial.print("! " + String(myMessages->get(i).root) + " ");
      Serial.print(String(myMessages->get(i).command) + " ");
      // Serial.print("(options size: "+ String(myMessages->get(i).options->size()) +")");
      // for (int j = 0; j < myMessages->get(i).options->size(); j++)
      // {
      //   Serial.print(myMessages->get(i).options->get(j).option + " " + myMessages->get(i).options->get(j).value);
      // }
      Serial.println();
    }
#endif

    if (myMessages->size() != 0)
    {
      for (int i = 0; i < myMessages->size(); i++)
      {
        if (myMessages->get(i).root.equals(CMD_LED))
        {
          if (myMessages->get(i).command.equals(FILL_ARG))
          {
            system_status = 1; //filled with color
          }
          if (myMessages->get(i).command.equals(RAINBOW_ARG))
          {
            system_status = 2; //playing rainbow
          }
        }
      }
    }

    bot_lasttime = millis();
    Serial.println("------------------------------");
    Serial.println("FreeRAM: " + String(ESP.getFreeHeap()));
  }

  switch (system_status)
  {
  case 1:
  {
    //leds.color(Utils::hexToDec(Utils::getNamedOptionValue(myMessages->get(0).options, COLOR_ARG))); //this may be a problem point when more then 1 message to work with
    break;
  }
  case 2:
  {
    leds.rainbow();
    break;
  }

  default:
    break;
  }

  // delete(myMessages);
  // if (myMessages->size() != 0) {
  //   myMessages->~LinkedList();
  // }
  // myMessages->~LinkedList();
  // myMessages->clear();
  // delete(myMessages);
}