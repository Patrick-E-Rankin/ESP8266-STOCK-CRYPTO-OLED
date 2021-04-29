<!DOCTYPE html>
<html>
  <head>

  <body>
    <h1>ESP8266 Stock/Crypto OLED</h1>
    <img src="OLEDExample.gif"></img>
    <h3><u>Summary</u></h3>
    <p>ESP8266 based stock price display (not really a ticker because it doesn't move) that uses an OLED (SH1106), that receives the ticker symbol from
      self served webpage.</p>
    <p>I am currently using Yahoo Finance for price info every 5 seconds but you
      can change that to the API of your choice along with the update frequency.</p>
  <p>The ESP8266 needs the RSA fingerprint method, and it get updated randomly. You can get the fingerprint <a href="https://www.grc.com/fingerprints.htm">here</a>.</p>
    <p>I am not a developer or programmer, so my code is not pretty, but I'm working on it.</p>
    <p>This code is compatible with Arduino.</p>
    <h3><u>Libraries Used:</u></h3>
    <p><a href="https://github.com/olikraus/u8g2" target="_blank">U8x8 (it's part of U8G2)</a>
      by <a href="https://github.com/olikraus" target="_blank">Olikraus</a><br>
      <a href="https://github.com/me-no-dev/ESPAsyncWebServer" target="_blank">ESPAsyncWebServer</a>
      by <a href="https://github.com/me-no-dev" target="_blank">Me No Dev</a><br>
  <a href="https://github.com/bblanchon/ArduinoJson">ArduinoJson</a></p>
    <h3><u>Setup</u></h3>
    <p>Connect your ESP8266 to your i2c OLED. I use the standard D1(SCL) and D2 (SDA).</p>
    <p>You must insert your own SSID and password for your Wifi, so it can
      connect. </p>
    <p>Upon getting an IP address it will display it continually on the OLED until it receives its first ticker symbol.</p>
    <p>From any browser input the IP address where you will be presented with a
      ticker input. </p>
    <p>The program will automatically check for a new price every 5 seconds.</p>
    <p>It also supports crypto, but you must use the Yahoo Finance format, for
      example BTC-USD for Bitcoin or DOGE-USD for Doge. Other currencies are supported.</p>
    <p>Any symbol that works on Yahoo Finance should technically work.</p>
    <p>It also calculates the percentage gain or loss from the previous close.</p>
    <br>
  <h3><u>FAQ</u></h3>
  <p><b>What happens when I put a bad ticker? Like "None"?</b><br>
  An error will display on the OLED.</p>
  <h3><u>TODO</u></h3>
  1) Multiple ticker support
 
  </body>
</html>
