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

// extern "C" {
//   #include "user_interface.h"
//   #include "wpa2_enterprise.h"
// }

String ssid = "From_Siberia_With_Love";                         // REPLACE mySSID WITH YOUR WIFI SSID
String pass = "qwerty1480";                                     // REPLACE myPassword YOUR WIFI PASSWORD, IF ANY
String token = "571169334:AAEr3G6dtKkEtXMBRusBd9yAklLYw2QhgzY"; // REPLACE myToken WITH YOUR TELEGRAM BOT TOKEN

WiFiClientSecure secured_client;
UniversalTelegramBot bot(token, secured_client);

//https://github.com/esp8266/Arduino/issues/1032#issuecomment-285314332
// // SSID to connect to
// static const char* ssid = "MY_SSID";
// // Username for authentification
// static const char* username = "user1";
// // Password for authentification
// static const char* password = "secret";

const unsigned long BOT_MTBS = 1000; // mean time between scan messages
unsigned long bot_lasttime;          // last time messages' scan has been done

CRGB leds[NUM_LEDS];

void handleNewMessages(int numNewMessages)
{
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

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
      String welcome = "Welcome to Universal Arduino Telegram Bot library, " + from_name + ".\n";
      welcome += "This is Flash Led Bot example.\n\n";
      welcome += "/ledon : to switch the Led ON\n";
      welcome += "/ledoff : to switch the Led OFF\n";
      welcome += "/status : Returns current status of LED\n";
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

  if (WiFi.waitForConnectResult() == WL_CONNECTED)
  {
    IPAddress ip = WiFi.localIP();
    Serial.print("Connected to WiFi. IP address: ");
    Serial.println(ip);
  }
  else
  {
    Serial.println("Failed to connect to WiFi. Resetting ESP");
    delay(5000);
    ESP.restart();
  }

  //Wireles firmare upload init
  ArduinoOTA.onStart([]() {
    Serial.println("Start"); //  "Начало OTA-апдейта"

    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = CRGB::Black;
    }

    FastLED.show();
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd"); //  "Завершение OTA-апдейта"
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

  pinMode(LED_BUILTIN, OUTPUT);

  secured_client.setInsecure();
}

void loop()
{

  ArduinoOTA.handle();

  if (millis() > bot_lasttime + BOT_MTBS)
  {
    Serial.println("Cheking for updates");
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages)
    {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    bot_lasttime = millis();
  }
}