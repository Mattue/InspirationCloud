//https://github.com/urish/arduino-slack-bot/blob/master/slackbot/slackbot.ino
#define FASTLED_ESP8266_RAW_PIN_ORDER

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <FastLED.h>

#define NUM_LEDS 73
#define PIN6 12
#define PIN7 13
#define PIN8 15

// замените значения в этих константах на те,
// что соответствуют вашей сети:
const char* ssid = "From_Siberia_With_Love";
const char* password = "qwerty1480";

CRGB leds[NUM_LEDS];
byte counter;

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");  //  "Загрузка"
  
  //LED strips init
  FastLED.addLeds<WS2812B, PIN6, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<WS2812B, PIN7, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<WS2812B, PIN8, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );

  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB::Black;
  }
    
  FastLED.setBrightness(70);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    //  "Соединиться не удалось! Перезагрузка..."
    delay(5000);
    ESP.restart();
  }

  //Wireles firmare upload init
  ArduinoOTA.onStart([]() {
    Serial.println("Start");  //  "Начало OTA-апдейта"
    
    for(int i = 0; i < NUM_LEDS; i++){
      leds[i] = CRGB::Black;
    }
    
    FastLED.show();
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");  //  "Завершение OTA-апдейта"
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    //  "Ошибка при аутентификации"
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    //  "Ошибка при начале OTA-апдейта"
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    //  "Ошибка при подключении"
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    //  "Ошибка при получении данных"
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
    //  "Ошибка при завершении OTA-апдейта"
  });
  ArduinoOTA.begin();
  Serial.print("WiFi status: ");
  Serial.println(WiFi.status());
  Serial.println("Ready");  //  "Готово"
  Serial.print("IP address: ");  //  "IP-адрес: "
  Serial.println(WiFi.localIP());
  
  pinMode(LED_BUILTIN, OUTPUT);
}
 
void loop() {
  ArduinoOTA.handle();
  
  for (int i = 0; i < NUM_LEDS; i++ ) {         // от 0 до первой трети
    leds[i] = CHSV(counter + i * 2, 255, 255);  // HSV. Увеличивать HUE (цвет)
//    // умножение i уменьшает шаг радуги
  }
  counter++;        // counter меняется от 0 до 255 (тип данных byte)
  FastLED.show();
  
  delay(5);         // скорость движения радуги
}void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
