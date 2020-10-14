#include <ArduSlack.h>

// ArduSlack::ArduSlack(const String &token)
// {
//     _token = token;
// }

// void sendPing()
// {
//     DynamicJsonBuffer jsonBuffer;
//     JsonObject &root = jsonBuffer.createObject();
//     root["type"] = "ping";
//     root["id"] = nextCmdId++;
//     String json;
//     root.printTo(json);
//     webSocket.sendTXT(json);
// }

// void webSocketEvent(WStype_t type, uint8_t *payload, size_t len)
// {
//     switch (type)
//     {
//     case WStype_DISCONNECTED:
//     {
//         Serial.printf("[WebSocket] Disconnected :-( \n");
//         connected = false;
//         break;
//     }

//     case WStype_CONNECTED:
//     {
//         Serial.printf("[WebSocket] Connected to: %s\n", payload);
//         sendPing();
//         break;
//     }

//     case WStype_TEXT:
//     {
//         Serial.printf("[WebSocket] Message: %s\n", payload);
//         processSlackMessage((char *)payload);
//         break;
//     }

//     default:
//         break;
//     }
// }

// bool ArduSlack::connectToSlack()
// {
//     client = new WiFiClientSecure();

//     bool mfln = client->probeMaxFragmentLength(SLACK_URL, 443, 1024);

// #if DEBUG_MODE == 1
//     Serial.println("\nINFO: Connecting to " + String(SLACK_URL SLACK_BOT_TOKEN));
//     Serial.printf("INFO: Maximum fragment Length negotiation supported: %s\n", mfln ? "yes" : "no");
// #endif

//     if (mfln)
//     {
//         client->setBufferSizes(1024, 1024);
//     }

//     client->setFingerprint(SLACK_SSL_FINGERPRINT);

// #if DEBUG_MODE == 1
//     Serial.println("DEBUG: [HTTPS] begin...");
// #endif

// // Step 1: Find WebSocket address via RTM API (https://api.slack.com/methods/rtm.connect)

//     HTTPClient https;

//     if (https.begin(*client, String(SLACK_URL SLACK_BOT_TOKEN)))
//     {

// #if DEBUG_MODE == 1
//         Serial.print("DEBUG: [HTTPS] GET...\n");
// #endif

//         // start connection and send HTTP header
//         int httpCode = https.GET();
//         if (httpCode > 0)
//         {
//             if (httpCode == HTTP_CODE_OK)
//             {
//                 client->find("wss:\\/\\/");
//                 String host = client->readStringUntil('\\');
//                 String path = client->readStringUntil('"');
//                 path.replace("\\/", "/");

//                 // Step 2: Open WebSocket connection and register event handler
//                 Serial.println("WebSocket Host=" + host + " Path=" + path);
//                 // webSocket.beginSSL(host, 443);
//                 //webSocket.beginSSL(host, 443, path, "", "");
//                 //webSocket.onEvent(webSocketEvent);
//                 return true;
//             }
//         }
//         else
//         {

// #if DEBUG_MODE == 1
//             Serial.printf("ERROR: [HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
// #endif

//             return false;
//         }
//     }
//     else
//     {
// #if DEBUG_MODE == 1
//         Serial.printf("ERROR: ArduSlack: Unable to connect\n");
// #endif
//         return false;
//     }
// }