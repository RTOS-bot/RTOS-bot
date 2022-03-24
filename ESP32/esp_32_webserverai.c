// Load Wi-Fi library
#include <WiFi.h>

#define RXD2 16
#define TXD2 17

// Replace with your network credentials
const char* ssid = "redmi_123";
const char* password = "abcd1234";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String response, ip_address;

// Auxiliar variables to store the current output state
String output26State = "off";

// Assign output variables to GPIO pins
const int output26 = 26;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;
int wait30 = 30000; // time to reconnect when connection is lost.

// This is your Static IP
IPAddress local_IP(192, 168, 43, 211); 
// Gateway IP address
IPAddress gateway(192, 168, 43, 1);
IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4); 

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  // Initialize the output variables as outputs
  pinMode(output26, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output26, LOW);

  //Configure Static IP
  if(!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
  {
    Serial.println("Static IP failed to configure");
  }

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  ip_address = WiFi.localIP().toString();
  Serial.println(ip_address);
  server.begin();
}

void loop() {

// If disconnected, try to reconnect every 30 seconds.
  if ((WiFi.status() != WL_CONNECTED) && (millis() > wait30)) {
    Serial.println("Trying to reconnect WiFi...");
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    wait30 = millis() + 30000;
  } 
  // Check if a client has connected..
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
   
  Serial.print("New client: ");
  Serial.println(client.remoteIP());
   
  // Espera hasta que el cliente envíe datos.
  // while(!client.available()){ delay(1); }

  /////////////////////////////////////////////////////
  // Read the information sent by the client.
  String req = client.readStringUntil('\r');
  Serial.println(req);

  // Make the client's request.
  if(req.indexOf("status") != -1)
  {
    response = "WiFi Connected: " + ip_address;
  }
  if(req.indexOf("onRed") != -1)
  {
    digitalWrite(output26, HIGH);
    response = "RED LED ON";
    Serial2.write(0x31);
  }
  if(req.indexOf("offRed") != -1)
  {
    digitalWrite(output26, LOW);
    response = "RED LED OFF";
    Serial2.write(0x30);
  }  
  if(req.indexOf("onGreen") != -1)
  {
    digitalWrite(output26, HIGH);
    response = "GREEN LED ON";
    Serial2.write(0x33);
  }
  if(req.indexOf("offGreen") != -1)
  {
    digitalWrite(output26, LOW);
    response = "GREEN LED OFF";
    Serial2.write(0x32);
  }
  if(req.indexOf("onBlue") != -1)
  {
    digitalWrite(output26, HIGH);
    response = "BLUE LED ON";
    Serial2.write(0x35);
  }
  if(req.indexOf("offBlue") != -1)
  {
    digitalWrite(output26, LOW);
    response = "BLUE LED OFF";
    Serial2.write(0x34);
  }
  if (req.indexOf("turnLeft") != -1)
  {
    digitalWrite(output26, HIGH);
    response = "TURNING LEFT";
    Serial2.write(0x40);
  }
  if (req.indexOf("turnRight") != -1)
  {
    digitalWrite(output26, HIGH);
    response = "TURNING RIGHT";
    Serial2.write(0x41);
  }
  if (req.indexOf("goFront") != -1)
  {
    digitalWrite(output26, HIGH);
    response = "GOING FORWARD";
    Serial2.write(0x42);
  }
  if (req.indexOf("goBack") != -1)
  {
    digitalWrite(output26, HIGH);
    response = "REVERSING";
    Serial2.write(0x43);
  }
  if (req.indexOf("stop") != -1)
  {
    digitalWrite(output26, HIGH);
    response = "STOPPING";
    Serial2.write(0x44);
  }
  /*
       if (req.indexOf("on12") != -1) {digitalWrite(LED12, HIGH); estado = "LED12 ON";}
       if (req.indexOf("off12") != -1){digitalWrite(LED12, LOW); estado = "LED12 OFF";}
       if (req.indexOf("on14") != -1) {digitalWrite(LED14, HIGH); estado = "LED14 ON";}
       if (req.indexOf("off14") != -1){digitalWrite(LED14, LOW); estado = "LED14 OFF";}
       if (req.indexOf("consulta") != -1){
           estado ="";
           if (digitalRead(LED12) == HIGH) {estado = "LED12 ON,";} else {estado = "LED12 OFF,";}
           if (digitalRead(LED14) == HIGH) {estado = estado + "LED14 ON";} else {estado = estado + "LED14 OFF";}
           }*/
           

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); 
  client.println(response); //  Return status.

  client.flush();
  client.stop();
  Serial.println("Client disconnected.");
}