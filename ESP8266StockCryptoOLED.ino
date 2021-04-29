//This program came from: https://github.com/Patrick-E-Rankin/ESP8266-STOCK-CRYPTO-OLED
//Please feel free to modify as you please
//This program displays stock and crypto prices on an OLED using Yahoo Finance as a source.
//Cryptos need to use the Yahoo format of symbol and currency, which for example is: BTC-USD
//It also shows the percentage difference from yesterday's price.
//When it first starts up it will show you what SSID it's connecting to and the IP address it wants you to go to for input.

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <U8x8lib.h> //Very fast library (characters only), you don't have to use this one, substitute whatever you like
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>

AsyncWebServer server(80); //Start the webserver

U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE); //U8X8 OLED declaration, needs to match your LCD


const char* ssid     = "YOUR SSID";     // your network SSID (name of wifi network)
const char* password = "YOUR SSID PASSWORD"; // your network password

const char* yahoo = "https://query1.finance.yahoo.com/v8/finance/chart/";  //First part of Yahoo url
const char* interval = "?interval=1d"; //Third part of Yahoo url
const char* symbol; //JSON value of ticker symbol
float marketprice; //JSON value of ticker price
float comparemarketprice; //Previous ticker price for comparison
float previousclose; //Json value of ticker price on previous day of trading 
float percent; //Using the previous close and today's price, a percent can be calculated
int httpresponseCode; //Common http response codes, such as 404 (missing webpage) or 200 (all ok)
const char* PARAM_INPUT_1 = "input1"; //User input value on the webpage generated
String ticker1 = ""; //Temporary holder for the user input as long as it is valid
const char Index[] = "<!DOCTYPE HTML><html><head><title>ESP8266 STOCK/CRYPTO LCD</title><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><body><form action=\"/get\">Ticker Symbol: <input type=\"text\" name=\"input1\"><input type=\"submit\" value=\"Submit\"></form></body></html>"; //The webpage for user input

void notFound(AsyncWebServerRequest *request) //Our webserver 404 error response
{
  request->send(404, "text/plain", "Not found");
}

void badstatus(void) //Display on OLED if ticker is invalid or maybe connection lost?
{
  u8x8.setFont(u8x8_font_8x13B_1x2_f);
  u8x8.clear();
  u8x8.print("Bad Connection?");
  u8x8.setCursor(0,10);
  u8x8.print("Invalid Ticker?");
}

void connectionstatus(void) //Display on OLED what SSID it's trying to connect to. If it's too long it'll overwrite itself, consider using a smaller font.
{
  u8x8.setFont(u8x8_font_8x13B_1x2_f);
  u8x8.clear();
  u8x8.print("Searching for:");
  u8x8.setCursor(0,10);
  u8x8.print(ssid);
}

void showip(void) //Display on OLED the acquired IP Address
{
  u8x8.setCursor(0,0);
  u8x8.setFont(u8x8_font_8x13B_1x2_f);
  u8x8.clear();
  u8x8.print("Connect to:");
  u8x8.setCursor(0,10);
  u8x8.print(WiFi.localIP());
}

void showprice(void) //Display on OLED the ticker, possible percent change, and price.
{
  u8x8.setFont(u8x8_font_8x13B_1x2_f);    
  u8x8.clear();
  u8x8.inverse();
  u8x8.print(symbol);
  if (percent > 0)
   { 
    u8x8.setCursor((strlen(symbol)+1),0); //Move the cursor two spaces after the ticker name
    u8x8.noInverse();
    u8x8.print("+");
    u8x8.print(percent);
    u8x8.print("%");
   }
  else if (percent < 0)
    {
      u8x8.setCursor((strlen(symbol)+1),0); //Move the cursor two spaces after the ticker name
      u8x8.noInverse();
      u8x8.print(percent);
      u8x8.print("%");
    }
  u8x8.setFont(u8x8_font_profont29_2x3_f);
  u8x8.noInverse();
  u8x8.setCursor(0,3);
  u8x8.print(marketprice);
}

void webserver(void) //Starts the webserver for user input
{
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
   request->send(200, "text/html", Index);
  });
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) 
  {
    String inputMessage;
    String inputParam;
    if (request->hasParam(PARAM_INPUT_1)) 
    {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      ticker1 = inputMessage;
    }
      request->redirect("/"); //After user input, sends user back to beginning
  });
    server.onNotFound(notFound);
    server.begin();
}

void getinfo(void) //Connects to Yahoo Finance, downloads JSON, parses info, generates possible percent change
{
  if (WiFi.status()==WL_CONNECTED) 
  {
    HTTPClient http;
    http.begin(yahoo+ticker1+interval,"42:0A:3F:BB:EF:F1:FC:D9:FA:0F:23:8F:96:AA:B9:AA:4D:4E:85:15"); //HTTPS fingerprint
    httpresponseCode = http.GET();

    if (httpresponseCode > 0) {
      Serial.print("HTTP Response Code: ");
      Serial.println(httpresponseCode);
      String payload = http.getString();

      DynamicJsonDocument doc(2048);
      DeserializationError error = deserializeJson(doc, payload);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
        }
       JsonObject chart_result_0 = doc["chart"]["result"][0];
       JsonObject chart_result_0_meta = chart_result_0["meta"];
       const char* tempsymbol = chart_result_0_meta["symbol"];
       float tempmarketprice = chart_result_0_meta["regularMarketPrice"];
       float temppreviousclose = chart_result_0_meta["chartPreviousClose"];
       marketprice = tempmarketprice;
       symbol = tempsymbol;
       previousclose = temppreviousclose;
       percent = (((marketprice / previousclose)-1)*100);
       
      Serial.println(symbol);
      Serial.println(marketprice);
      
    }
    else {
      Serial.print("Error Code: ");
      Serial.println(httpresponseCode);
    }
   }
}

void setup() 
{
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  delay(100);
  u8x8.begin();

  //HTTPClient http;
  connectionstatus();
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("Connected to ");
  Serial.println(ssid);
  showip();
  webserver();
}

void loop() 
{
  while (ticker1 != "")
   {delay(5000); //5 second delay on getting info
   getinfo();
   if (httpresponseCode == 404)
   {
    badstatus();
   }
   else
   {
    if (comparemarketprice != marketprice)
      {
        showprice();
        comparemarketprice = marketprice;
      }
   }
   }
}
