#include <Arduino.h>

#define FASTLED_ESP8266_RAW_PIN_ORDER

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <ArduinoOTA.h>
#include <FastLED.h>

#define NUM_LEDS 73
#define PIN6 12
#define PIN7 13
#define PIN8 15

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

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

//const unsigned long BOT_MTBS = 1000; // mean time between scan messages
unsigned long bot_lasttime;          // last time messages' scan has been done

CRGB leds[NUM_LEDS];

void handleNewMessages(int numNewMessages)
{
#if DEBUG_MODE == 1
  Serial.print("Number of messages to handle: ");
  Serial.println(String(numNewMessages));
#endif

  for (int i = 0; i < numNewMessages; i++)
  {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "")
      from_name = "Guest";

    bot.sendMessage(chat_id, "Hello there: " + text, "");

    if (text == "/start")
    {
      String welcome = "Welcome to Inspiration cloud, " + from_name + ".\n";
      welcome += "\n\n";
      welcome += "/led : for led commands\n";
      welcome += "/status : Returns current status of inspiration cloud\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
  }
}

void setup()
{
  // initialize the Serial
  Serial.begin(115200);

  //LED strips init
  FastLED.addLeds<WS2812B, PIN6, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812B, PIN7, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812B, PIN8, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB::Black;
  }

  FastLED.setBrightness(70);

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

    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = CRGB::Black;
    }

    FastLED.show();
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd of OTA update"); //  "Завершение OTA-апдейта"
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
      Serial.println("Auth Failed");
    //  "Ошибка при аутентификации"
    else if (error == OTA_BEGIN_ERROR)
      Serial.println("Begin Failed");
    //  "Ошибка при начале OTA-апдейта"
    else if (error == OTA_CONNECT_ERROR)
      Serial.println("Connect Failed");
    //  "Ошибка при подключении"
    else if (error == OTA_RECEIVE_ERROR)
      Serial.println("Receive Failed");
    //  "Ошибка при получении данных"
    else if (error == OTA_END_ERROR)
      Serial.println("End Failed");
    //  "Ошибка при завершении OTA-апдейта"
  });
  ArduinoOTA.begin();

  //pinMode(LED_BUILTIN, OUTPUT);

  secured_client.setInsecure();
}

void loop()
{

  ArduinoOTA.handle();

  if (millis() > bot_lasttime + BOT_MTBS)
  {
#if DEBUG_MODE == 1
    Serial.println("Cheking for updates");
#endif

    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    for (int i = 0; i < numNewMessages; i++)
    {
#if DEBUG_MODE == 1
      Serial.println("got response");
#endif

      handleNewMessages(numNewMessages);
    }

    bot_lasttime = millis();
  }
}