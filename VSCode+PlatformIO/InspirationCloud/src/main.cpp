#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <ArduinoOTA.h>
#include <Led.h>
#include <MessageHandler.h>
#include <Ticker.h>
#include <LinkedList.h>
#include <structures\ParsedMessage.h>
// #include <structures\Status.h>
#include <utils\Utils.h>
// #include <ArduSlack.h>

#ifndef TELEGRAM_DEBUG
#define TELEGRAM_DEBUG 0
#endif

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

#ifndef TELEGRAM_BOT_TOKEN
#define TELEGRAM_BOT_TOKEN "571169334:AAEr3G6dtKkEtXMBRusBd9yAklLYw2QhgzY"
#endif

#ifndef SLACK_BOT_TOKEN
#define SLACK_BOT_TOKEN "571169334:AAEr3G6dtKkEtXMBRusBd9yAklLYw2QhgzY"
#endif

#ifndef SLACK_SSL_FINGERPRINT
#define SLACK_SSL_FINGERPRINT ""
#endif

//https://github.com/esp8266/Arduino/issues/1032#issuecomment-285314332
#ifdef SSID_MODE //1 - WPA2-Enterprize, 0 - WPA2-Personal
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

#ifndef BOT_MTBS //bot refresh speed in ms
#define BOT_MTBS = 1000
#endif

unsigned long lastMillis = millis(); // last time messages' scan has been done

Led leds; //LED's controller

Ticker rainbowTicker; //rainbow interruption

MessageHandler messageHandler;

bool runCommand = true; //true if there is commant to run, false otherwise

LinkedList<ParsedMessage> *lastMessages = NULL; //holds last message from bot

// Status currentStatus;

void setup()
{
  // initialize the Serial
  Serial.begin(115200);

#if SSID_MODE == 0 //WPA2-Personal //TODO: rework to ifndef
// connect the ESP8266 to the desired access point
#if DEBUG_MODE == 1
  Serial.println("\nINFO: Connecting to WiFi: " + String(SSID_NAME));
#endif

  WiFi.mode(WIFI_STA);
  delay(500);
  WiFi.begin(SSID_NAME, SSID_PASSWORD);
  delay(500);

  if (WiFi.waitForConnectResult() == WL_CONNECTED)
  {
    // currentStatus.ip = WiFi.localIP().toString();

#if DEBUG_MODE == 1
    IPAddress ip = WiFi.localIP();
    Serial.println("INFO: Connected to WiFi. IP address: " + ip.toString());
#endif
  }
  else
  {
#if DEBUG_MODE == 1
    Serial.println("ERROR: Failed to connect to WiFi. Resetting ESP");
#endif
    delay(5000);
    ESP.restart();
  }
#else
//connect the ESP8266 to wpa2-enterprise
//TODO: cert must be applied here
#if DEBUG_MODE == 1
  Serial.print("INFO: Connecting to WiFi: ");
  Serial.println(SSID_NAME);
#endif

  static const char *ssid = SSID_NAME;
  // Username for authentification
  static const char *username = SSID_USERNAME;
  // Password for authentication
  static const char *password = SSID_PASSWORD;

  Serial.println(ssid);
  Serial.println(username);
  Serial.println(password);

  wifi_set_opmode(STATION_MODE);
  struct station_config wifi_config;
  memset(&wifi_config, 0, sizeof(wifi_config));
  strcpy((char *)wifi_config.ssid, ssid);
  wifi_station_set_config(&wifi_config);
  wifi_station_clear_cert_key();
  wifi_station_clear_enterprise_ca_cert();
  wifi_station_set_wpa2_enterprise_auth(1);
  wifi_station_set_enterprise_identity((uint8 *)username, strlen(username));
  wifi_station_set_enterprise_username((uint8 *)username, strlen(username));
  wifi_station_set_enterprise_password((uint8 *)password, strlen(password));
  wifi_station_connect();

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Done");

  //   disable_extra4k_at_link_time();

  //   wifi_set_opmode(STATION_MODE);

  //   struct station_config wifi_config;

  //   memset(&wifi_config, 0, sizeof(wifi_config));
  //   strcpy((char *)wifi_config.ssid, (char *)SSID_NAME);

  //   wifi_station_set_config(&wifi_config);

  //   wifi_station_clear_cert_key();
  //   wifi_station_clear_enterprise_ca_cert();

  //   wifi_station_set_wpa2_enterprise_auth(1);
  //   wifi_station_set_enterprise_identity((uint8 *)SSID_USERNAME, strlen(SSID_USERNAME));
  //   wifi_station_set_enterprise_username((uint8 *)SSID_USERNAME, strlen(SSID_USERNAME));
  //   wifi_station_set_enterprise_password((uint8 *)SSID_PASSWORD, strlen(SSID_PASSWORD));

  //   wifi_station_connect();

  //   // Wait for connection AND IP address from DHCP
  //   while (WiFi.status() != WL_CONNECTED)
  //   {
  // #if DEBUG_MODE == 1
  //     // if (millis() > lastMillis + 500)
  //     // {
  //     //   Serial.print(".");
  //     //   lastMillis = millis();
  //     // }
  //     // delay(500);
  //     // Serial.println(WiFi.status());
  // #endif
  //   }

  // #if DEBUG_MODE == 1
  //   IPAddress ip = WiFi.localIP();
  //   Serial.print("INFO: Connected to WiFi. IP address: ");
  //   Serial.println(ip);
  // #endif

#endif

  //Wireles firmare upload init
  ArduinoOTA.setRebootOnSuccess(true);
  ArduinoOTA.onStart([]() {
    Serial.println("\nINFO: Start of OTA update"); //  "Begin of OTA-update"
    rainbowTicker.detach();

    leds.color("Yellow");
    delay(1000);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nINFO: End of OTA update"); //  "End of OTA-update"
    delay(2000);
    leds.switchOff();
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    leds.setColorByPercent((progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    leds.color("Red");
    delay(1000);

    Serial.printf("ERROR: Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) //  "Authentication error"
    {
      Serial.println("Auth Failed");
      //TODO: do LED notification
    }
    else if (error == OTA_BEGIN_ERROR) //  "Error in begin of OTA-update"
    {
      Serial.println("Begin Failed");
      //TODO: do LED notification
    }
    else if (error == OTA_CONNECT_ERROR) //  "Connection error"
    {
      Serial.println("Connect Failed");
      //TODO: do LED notification
    }
    else if (error == OTA_RECEIVE_ERROR) //  "Data receive error"
    {
      Serial.println("Receive Failed");
      //TODO: do LED notification
    }
    else if (error == OTA_END_ERROR) //  "Error in end of OTA-update"
    {
      Serial.println("End Failed");
      //TODO: do LED notification
    }
  });
  ArduinoOTA.begin();

  // currentStatus.status = 0;

  pinMode(LED_BUILTIN, OUTPUT);
}

void rainbowCallBack()
{ //TODO: think about Lambda function
  leds.rainbow();
}

void loop()
{
  ArduinoOTA.handle();

  if (millis() > lastMillis + BOT_MTBS)
  {

    LinkedList<ParsedMessage> *currentMessage;

#if DEBUG_MODE == 1
    Serial.println("INFO: Checking for updates");
#endif

    currentMessage = messageHandler.handleMessages();

    if (currentMessage != NULL)
    {
      if (currentMessage->size() != 0)
      {
        Utils::deleteParsedMessageList(lastMessages);
        lastMessages = currentMessage;
        //system_status = lastMessages->get(0).systemStatus; //this may be a problem point when more then 1 message to work with
      }

#if DEBUG_MODE == 1
      Serial.println("DEBUG: currentMessages size: " + String(currentMessage->size()));

      for (int i = 0; i < currentMessage->size(); i++)
      {
        Serial.print("DEBUG: ! " + String(currentMessage->get(i).root) + " ");
        Serial.print(String(currentMessage->get(i).command) + " ");
        if (currentMessage->get(i).options != NULL)
        {
          for (int j = 0; j < currentMessage->get(i).options->size(); j++)
          {
            Serial.print(currentMessage->get(i).options->get(j).option + " " + currentMessage->get(i).options->get(j).value);
          }
        }
        Serial.println();
      }
#endif

      runCommand = true;
    }
    else
    {
#if DEBUG_MODE == 1
      Serial.println("INFO: currentMessages is NULL");
#endif
    }

    lastMillis = millis();

#if DEBUG_MODE == 1
    Serial.println("DEBUG: FreeRAM: " + String(ESP.getFreeHeap()));
    Serial.println("------------------------------");
#endif
  }

  if (lastMessages != NULL && runCommand)
  {
    rainbowTicker.detach();
    switch (lastMessages->get(0).systemStatus) //this may be a problem point when more then 1 message to work with
    {
    case 0:
    {
      leds.switchOff();
      break;
    }
    case 1:
    {
      leds.color(Utils::getNamedOptionValue(lastMessages->get(0).options, COLOR_ARG));
      break;
    }
    case 2:
    {
      rainbowTicker.attach_ms((uint32_t) (RAINBOW_SPEED), rainbowCallBack);
      break;
    }

    default:
      break;
    }

    runCommand = false;
  }
}