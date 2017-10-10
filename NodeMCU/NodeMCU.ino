#include <ESP8266WiFi.h>
 
const char* ssid = "AccessPointX";
const char* password = "Rj@0750392151";
 
int ledPin1 = 13; // GPIO13
int ledPin2 = 14; // GPIO13
int ledPin3 = 2; // GPIO13
int ledPin4 = 0; // GPIO13


int wifi_ind  = 12;
WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);
  delay(10);
 
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  
  pinMode(wifi_ind, OUTPUT);
  
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
  digitalWrite(ledPin4, LOW);
  
  digitalWrite(wifi_ind,LOW);
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  digitalWrite(wifi_ind,HIGH);
 
}
 
void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Match the request

  if (request.indexOf("/LED1=ON") != -1)  {
    digitalWrite(ledPin1, HIGH);
  }
  if (request.indexOf("/LED1=OFF") != -1)  {
    digitalWrite(ledPin1, LOW);
  }
  if (request.indexOf("/LED2=ON") != -1)  {
    digitalWrite(ledPin2, HIGH);
  }
  if (request.indexOf("/LED2=OFF") != -1)  {
    digitalWrite(ledPin2, LOW);
  }
  if (request.indexOf("/LED3=ON") != -1)  {
    digitalWrite(ledPin3, HIGH);
  }
  if (request.indexOf("/LED3=OFF") != -1)  {
    digitalWrite(ledPin3, LOW);
  }
  if (request.indexOf("/LED4=ON") != -1)  {
    digitalWrite(ledPin4, HIGH);
  }
  if (request.indexOf("/LED4=OFF") != -1)  {
    digitalWrite(ledPin4, LOW);
  }
 

 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
  client.print("Led pin is now: ");
 
//  if(value == HIGH) {
//    client.print("On");
//  } else {
//    client.print("Off");
//  }
  client.println("<br><br>");
  client.println("<a href=\"/LED1=ON\"\"><button>Turn Light1 On </button></a>");
  client.println("<a href=\"/LED1=OFF\"\"><button>Turn Light1 Off </button></a><br />"); 
  client.println("<a href=\"/LED2=ON\"\"><button>Turn Light2 On </button></a>");
  client.println("<a href=\"/LED2=OFF\"\"><button>Turn Light2 Off </button></a><br />"); 
  client.println("<a href=\"/LED3=ON\"\"><button>Turn Light3 On </button></a>");
  client.println("<a href=\"/LED3=OFF\"\"><button>Turn Light3 Off </button></a><br />"); 
  client.println("<a href=\"/LED4=ON\"\"><button>Turn Light4 On </button></a>");
  client.println("<a href=\"/LED4=OFF\"\"><button>Turn Light4 Off </button></a><br />");  
  client.println("<a href=\"/LED4=ON\"\"><button>Turn Light4 On </button></a>");

  
  client.println("<a href=\"/LED4=OFF\"\"><button>AC increase  </button></a><br />"); 
  client.println("<a href=\"/LED4=ON\"\"><button>AC Decrease  </button></a>");
  client.println("<a href=\"/LED4=OFF\"\"><button>AC Off </button></a><br />"); 
   client.println("<a href=\"/LED4=OFF\"\"><button>AC  on </button></a><br />"); 
client.println("<a href=\"/LED4=OFF\"\"><button>AC  fan rotation </button></a><br />"); 
  
  client.println("Temprature - 27C and Humidity - 75%"); 
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
 
}
