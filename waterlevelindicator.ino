#include "UbidotsESPMQTT.h" 
#include <ESP8266WiFi.h> 
#include <ESP8266WebServer.h>
#define TOKEN "BBFF-G2XGEG7is3woJ4HZ0OtCmoAiAy****" // Your Ubidots TOKEN
Ubidots client(TOKEN); 
ESP8266WebServer server; 
char* ssid = "mi"; //wifi ssid
char* password = "********"; //wifi password 
const int trigPin = 2; //D4
const int echoPin = 0; //D3 
int inputPin = 10; //SDD3 
long duration, cm, inches; 
int distance;
String page="";


void callback(char* topic, byte* payload, unsigned int length)
{
Serial.print("Message arrived ["); 
Serial.print(topic);
Serial.print("] ");
for (int i = 0; i < length; i++) 
{ 
Serial.print((char)payload[i]);
}
Serial.println();
}


void setup() {
// put your setup code here, to run once:

WiFi.begin(ssid,password);
Serial.begin(9600); // Starts the serial communication 
while(WiFi.status()!=WL_CONNECTED)
{
Serial.print("."); delay(500);
}

Serial.println("IP Address:");
Serial.print(WiFi.localIP()); 
server.begin();
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output 
pinMode(echoPin, INPUT); // Sets the echoPin as an Input

client.setDebug(true);  // Pass a true or false bool value to activate debug messages
client.wifiConnection(ssid, password); 
client.begin(callback);
}

void loop() {
// put your main code here, to run repeatedly:

if (!client.connected()) 
{ 
client.reconnect();
}

digitalWrite(trigPin, LOW); 
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds 
digitalWrite(trigPin, HIGH);
delayMicroseconds(10); 
digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);

// Calculating the distance 
distance= duration*0.034/2; 
cm = (duration/2) / 29.1; 
inches = (duration/2) / 74;

Serial.print("Distance: ");
Serial.print(inches); 
Serial.print("in, "); 
Serial.print(cm); 
Serial.print("cm"); 
Serial.println();

delay(10000);

float level = inches; // Read the ADC channel
client.add("level", level); // Variable for the water heater sensor assigned the ADC value.
//This will show up in Ubidots within the water-
sensor device

client.ubidotsPublish("waterlevel");
// Device name for Ubidots. Make sure Ubidots is setup prior to loading and running this application.

client.loop();

}

