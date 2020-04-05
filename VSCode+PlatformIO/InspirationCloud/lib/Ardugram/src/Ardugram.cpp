#include "Ardugram.h"

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
}

Ardugram::JsonDocument Ardugram::callMethod(String httpMethod, String apiMethodName, String queryParams)
{

    String URL = apiMethodName + (String) " /bot" + botApiToken + (String) "/" + apiMethodName;
    WiFiClientSecure telegramWiFiClient;

    HTTPClient telegramHttpClient;
    JsonDocument jsonDoc;

#if USE_SOCKS5 == 1 //work through socks5 proxy

    IPAddress proxyIP(176, 223, 142, 19);
    u_int proxyPort = 1080;
    // Socks5Proxy proxy(proxyIP, proxyPort, telegramWiFiClient);

    //URL = (String) "api.telegram.org/telegram-api/bot" + botApiToken + (String) "/getMe";
#else // basic http/https
// #if USE_HTTPS == 0 //work through http
//     HTTPClient telegramHttpClient;
// #else              //work through https
//     BearSSL::WiFiClientSecure telegramServer;
//     telegramServer.setFingerprint(httpsFingerprint);
//     telegramServer
// #endif
#endif

    serialLog("\nConnecting to proxy using host name\n");
    if (telegramWiFiClient.connect("0x050x010x000x03proxy_user:qwerty1480@176.223.142.19", 1080))
    {
        serialLog("\nConnected using host name\n");
    }
    else
    {
        serialLog("In else");
    }

    telegramWiFiClient.println("GET 149.154.167.198/bot123456:ABC-DEF1234ghIkl-zyx57W2v1u123ew11/getMe433");
    Serial.println(telegramWiFiClient.readString());

// serialLog("\nConnecting using host name\n");
//     uint16_t port = 443;
//     if (proxy.connect("api.telegram.org", port))
//     {
//         serialLog("\nConnected using host name\n");
//     }
//     else
//     {
//         serialLog("\nFailed to connect using host name\n");
//         serialLog("\nConnecting to connect using IP address\n");
//         // no way, try to connect with fixed IP
//         IPAddress telegramServerIP;
//         telegramServerIP.fromString("149.154.167.198");
//         if (proxy.connect(telegramServerIP, port))
//         {
//             serialLog("\nConnected using fixed IP\n");
//         }
//         else
//         {
//             serialLog("\nUnable to connect to Telegram server\n");
//         }
//     }

    // telegramHttpClient

    // if (telegramHttpClient.begin(telegramWiFiClient, URL))
    // {
    //     int httpCode = telegramHttpClient.GET();

    //     if (httpCode > 0)
    //     {
    //         Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    //         // file found at server
    //         if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
    //         {
    //             deserializeJson(jsonDoc, telegramHttpClient.getString());
    //         }
    //         serialLog("OK\n");
    //     }
    //     else
    //     {
    //         Serial.printf("[HTTP] GET... failed, error: %s\n", telegramHttpClient.errorToString(httpCode).c_str());
    //         serialLog("NOT OK\n");
    //     }
    //     telegramHttpClient.end();
    // }
    // else
    // {
    //     Serial.printf("[HTTP] Unable to connect\n");
    // }

#if false //CTBOT_CHECK_JSON == 0 //check JSON ???
    //return(telegramServer.readString());
#else
        return jsonDoc;
#endif
}
