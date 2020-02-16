#include "Ardugram.h"

#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include "Utils.h"

inline void Ardugram::serialLog(String message)
{
#if DEBUG_MODE > 0
    Serial.print(message);
#endif
}

Ardugram::Ardugram(String token)
{
    botApiToken = token;
}

Ardugram::~Ardugram()
{
}

Ardugram::JsonDocument Ardugram::getMe()
{
    return callMethod("GET", "getMe", "");
    // JsonDocument jsonDoc;
    // JsonObject root = jsonDoc.to<JsonObject>();

    // root["sensor"] = "gps";
    // root["time"] = 1351824120;

    // JsonArray data = root.createNestedArray("data");
    // data.add(48.756080);
    // data.add(2.302038);

    // return jsonDoc;
}

Ardugram::JsonDocument Ardugram::callMethod(String httpMethod, String apiMethodName, String queryParams)
{
#if USE_HTTPS == 0 //work through http
    WiFiClient telegramWiFiClient;
    HTTPClient telegramHttpClient;
#else //work through https
    BearSSL::WiFiClientSecure telegramServer;
    telegramServer.setFingerprint(httpsFingerprint);
#endif

    JsonDocument jsonDoc;

    //String URL = httpMethod + " /telegram-api/bot" + botApiToken + (String) "/" + apiMethodName + (String) "?" + queryParams;

    String URL = (String) "http://" + TELEGRAM_IP + (String) "/telegram-api/bot" + botApiToken + (String) "/getMe";
    //URL = "http://176.223.142.19/telegram-api/bot571169334:AAEr3G6dtKkEtXMBRusBd9yAklLYw2QhgzY/getMe";

    if (telegramHttpClient.begin(telegramWiFiClient, URL))
    {
        int httpCode = telegramHttpClient.GET();

        if (httpCode > 0)
        {
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
            {
                deserializeJson(jsonDoc, telegramHttpClient.getString());
            }
            serialLog("OK\n");
        }
        else
        {
            //Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
            serialLog("NOT OK\n");
        }
        telegramHttpClient.end();
    }
    else
    {
        //Serial.printf("[HTTP} Unable to connect\n");
    }

#if false //CTBOT_CHECK_JSON == 0 //check JSON ???
    //return(telegramServer.readString());
#else
    return jsonDoc;
#endif
}
