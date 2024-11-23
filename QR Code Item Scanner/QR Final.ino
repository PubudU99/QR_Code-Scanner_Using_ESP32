#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
#include <ESP32QRCodeReader.h>

#define ApplePrice 10.00
#define BiscuitPrice 15.00
#define SoapPrice 20.00

const char* ssid = "PUBUDU";
const char* password = "7K6328h%";
String webPage = "";

ESP32QRCodeReader reader(CAMERA_MODEL_AI_THINKER);
void onQrCodeTask(void *pvParameters);
void updateHTML(int Applecounter, int BiscuitCounter, int SoapCounter, int TotalItemCount);

WebServer server(80);




void onQrCodeTask(void *pvParameters)
{
  struct QRCodeData qrCodeData;
  int Applecounter = 0;
  int BiscuitCounter = 0;
  int SoapCounter = 0;
  int TotalItemCount = 0;

  while (true)
  {
    if (reader.receiveQrCode(&qrCodeData, 100))
    {
      Serial.println("Found QRCode");
      if (qrCodeData.valid)
      {
        String payloadString = String((char*)qrCodeData.payload);
        if (payloadString.equals("Apple"))
        {
          Serial.print("Payload1: ");
          Serial.println((const char *)qrCodeData.payload);
          Applecounter++;
          TotalItemCount++;
        }
        if (Applecounter >= 2)
        {
          Serial.println("Item removed (Apple)!");
          TotalItemCount = TotalItemCount - 1;
          Applecounter = 0;
        }
        if (payloadString.equals("biscuit"))
        {
          Serial.print("Payload2: ");
          Serial.println((const char *)qrCodeData.payload);
          BiscuitCounter++;
          TotalItemCount++;
        }
        if (BiscuitCounter >= 2)
        {
          Serial.println("Item removed (Biscuit)!");
          TotalItemCount = TotalItemCount - 1;
          BiscuitCounter = 0;
        }
        if (payloadString.equals("soap"))
        {
          Serial.print("Payload3: ");
          Serial.println((const char *)qrCodeData.payload);
          SoapCounter++;
          TotalItemCount++;
        }
        if (SoapCounter >= 2)
        {
          Serial.println("Item removed (Soap)!");
          TotalItemCount = TotalItemCount - 1;
          SoapCounter = 0;
        }

        Serial.print("Total Item Count: ");
        Serial.println(TotalItemCount);
        Serial.print("Apple Count: ");
        Serial.println(Applecounter);
        Serial.print("Biscuit Count: ");
        Serial.println(BiscuitCounter);
        Serial.print("Soap Count: ");
        Serial.println(SoapCounter);

        
        updateHTML(SoapCounter,BiscuitCounter,Applecounter,TotalItemCount-1);

      }
      else
      {
        Serial.print("Invalid: ");
        Serial.println((const char *)qrCodeData.payload);
        Serial.println("Move backward and Forward");
      }
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void updateHTML(int Applecounter, int BiscuitCounter, int SoapCounter, int TotalItemCount)
{
  
  String updatedHTML = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Supermarket Calculator</title><style>table{width:100%;border-collapse:collapse;margin-bottom:20px;}table,th,td{border:1px solid #ddd;}th,td{padding:12px;text-align:left;}th{background-color:#f2f2f2;}</style></head><body><h2>Supermarket Items</h2><table><thead><tr><th>Item Name</th><th>Count</th><th>Price</th></tr></thead><tbody>";

  
  updatedHTML += "<tr><td>Apple</td><td class=\"count\">" + String(Applecounter) + "</td><td class=\"price\">" + String(Applecounter * ApplePrice) + "</td></tr>";
  updatedHTML += "<tr><td>Biscuit</td><td class=\"count\">" + String(BiscuitCounter) + "</td><td class=\"price\">" + String(BiscuitCounter * BiscuitPrice) + "</td></tr>";
  updatedHTML += "<tr><td>Soap</td><td class=\"count\">" + String(SoapCounter) + "</td><td class=\"price\">" + String(SoapCounter * SoapPrice) + "</td></tr>";


  updatedHTML += "</tbody><tfoot><tr><td>Total</td><td>" + String(TotalItemCount) + "</td><td id=\"total\">" + String((Applecounter * ApplePrice) + (BiscuitCounter * BiscuitPrice) + (SoapCounter * SoapPrice)) + "</td></tr></tfoot></table><script>window.onload=function(){calculateTotal();};function calculateTotal(){var prices=document.getElementsByClassName(\"price\");var total=0;for(var i=0;i<prices.length;i++){total+=parseFloat(prices[i].textContent);}document.getElementById(\"total\").textContent=total.toFixed(2);}</script></body></html>";

  webPage = updatedHTML;
}

void handleRoot() {
  server.send(200, "text/html", webPage);
}

void handleNotFound(){
  server.send(404, "text/plain", "Not found");
}

void setup()
{
  Serial.begin(115200);
  Serial.println();

  reader.setup();

  Serial.println("Setup QRCode Reader");

  reader.beginOnCore(1);

  Serial.println("Begin on Core 1");

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(250);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");

  // Setup MDNS
  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  
  server.on("/", HTTP_GET, handleRoot);
  server.onNotFound(handleNotFound);

  server.begin();'9'

  Serial.println("HTTP server started");

  Serial.println("Use this URL to connect: " + WiFi.localIP().toString());

  // Start QRCode task
  xTaskCreate(onQrCodeTask, "onQrCode", 4 * 1024, NULL, 4, NULL);
}

void loop()
{
  server.handleClient();
  // MDNS.update();
}



