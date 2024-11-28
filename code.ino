
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define ssid      "Driver"       // WiFi SSID
#define password  "project123"  // WiFi password
#include "MAX30100_PulseOximeter.h"

long GPS;
long GPS1;
int AcX, AcY, AcZ, fl, y, x, f2, ge, te, hb, hb1, sdnn, rmssd, sp, al;
//SoftwareSerial mySerial(15, 13);
LiquidCrystal_I2C lcd(0x27, 16, 2);

String   t = "NULL";
String   t1 = "NULL";
String   t2 = "NULL";
String   t3 = "NULL";
char   p ;
String  etatLed = "";
#define button1 16
WebServer server ( 80 );

String getPage() {
  String page = "<html lang=fr-FR><head><meta http-equiv='refresh' content='2'/>";
  page += "<title>DRIVER SLEEP MONITORING SYSTEM</title>";
  page += "<style> body { background-color: #9AFEFF; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }</style>";
  page += "</head><body><h1>DRIVER SLEEP MONITORING SYSTEM</h1>";


  page += "<ul><li>DRIVER STATUS: ";

  page += t1;
  page += "</li></ul>";


  page += "<ul><li>ALCOHOL STATUS: ";

  page += t2;
  page += "</li></ul>";

 
  return page;

}


void handleRoot() {
  if ( server.hasArg("LED") ||  server.hasArg("LED1")) {
    handleSubmit();
  } else {
    server.send ( 200, "text/html", getPage() );
  }
}


void handleSubmit() {
  // Actualise le GPIO / Update GPIO
  String LEDValue, LEDValue1;
  LEDValue = server.arg("LED");
  LEDValue1 = server.arg("LED1");

  //Serial.println("Set GPIO "); Serial.print(LEDValue);
  if ( LEDValue == "0" ) {
    //  Serial.println ("B");
    digitalWrite(14, LOW);
    t = "MOTOR OFF";

    server.handleClient();
    delay(2000);
    etatLed = "OFF";
    server.send ( 200, "text/html", getPage() );
  } else if ( LEDValue == "1" ) {
    // Serial.println ("A");
    digitalWrite(14, HIGH);
    etatLed = "ON";
    t = "MOTOR ON";
    //     hb = random(3,5);
    //lcd.setCursor(0,10);
    //  lcd.print("I:");
    //  lcd.print(hb);
    server.handleClient();
    delay(2000);

    server.send ( 200, "text/html", getPage() );
  } else {
    // Serial.println("Err Led Value");
  }


}



void setup() {

  Serial.begin(9600);
  server.handleClient();
    WiFi.softAP(ssid, password);
    IPAddress myIP = WiFi.softAPIP();
    // On branche la fonction qui gère la premiere page / link to the function that manage launch page
    server.on ( "/", handleRoot );
    server.begin();
serialEvent();
 
  lcd.init();
  // turn on LCD backlight
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("DRIVER");
  lcd.setCursor(0, 1);
  lcd.print("DROWSINESS SYSTM");

 
  delay(200); // Pause for 2 seconds

 
init_modem();
  // mySerial.begin(9600);

  // pinMode(2, OUTPUT);
  pinMode(12, INPUT_PULLUP);
  pinMode(23, OUTPUT);
  pinMode(14, OUTPUT);

  digitalWrite(23, LOW);
  digitalWrite(14, LOW);
  randomSeed(analogRead(0));

  //digitalWrite(2, LOW);
  delay(1000);

  //digitalWrite(12, LOW);
  lcd.clear();
}


void init_modem()
{
      Serial.print("AT\n\r");
    delay(300);
    Serial.print("ATE0\n\r");
    delay(300);
    Serial.print("AT+CREG=1\n\r");
    delay(300);
    Serial.print("AT+CSMS=1\n\r");
    delay(300);
    Serial.print("AT+CSCS=\"GSM\"\n\r");
    delay(300);
    Serial.print("AT+CMGF=1\n\r");
    delay(300);
    Serial.print("AT+CNMI=2,2,0,0,0\n\r");
       delay(300);
      Serial.print("AT+CNMI=1,2,0,0,0");
    delay(300);
    
}
void send_sms1()
{
   
     init_modem();
  Serial.println("AT");
    delay(300);
    Serial.println("ATE0");
    delay(300);
    
    Serial.println("AT+CMGF=1");
    delay(300);
    Serial.println("AT+CMGS=\"9025752023\"\r"); // Replace x with mobile number
    delay(300);
    Serial.print("DRIVER SLEPT ALERT");
//     Serial.print("11.");
//      Serial.println(GPS); 
//      Serial.print("N"); 
//      Serial.print("78.");
//      Serial.println(GPS1);
//      Serial.print("E"); 
//      Serial.print("Temp.");
//      Serial.println(c1); 
//      Serial.print("HB.");
//      Serial.println(c2); 
      delay(500);  
 
  Serial.write((byte)0x1A);
    
    
    delay(3000);
    lcd.clear();
    lcd.print("sms sent1");
     delay(2000);
 }

void send_sms2()
{
   
     init_modem();
  Serial.println("AT");
    delay(300);
    Serial.println("ATE0");
    delay(300);
    
    Serial.println("AT+CMGF=1");
    delay(300);
    Serial.println("AT+CMGS=\"9025752023\"\r"); // Replace x with mobile number
    delay(300);
    Serial.print("ALCOHOL DRUKEN ALERT");
//     Serial.print("11.");
//      Serial.println(GPS); 
//      Serial.print("N"); 
//      Serial.print("78.");
//      Serial.println(GPS1);
//      Serial.print("E"); 
//      Serial.print("Temp.");
//      Serial.println(c1); 
//      Serial.print("HB.");
//      Serial.println(c2); 
      delay(500);  
 
  Serial.write((byte)0x1A);
    
    
    delay(3000);
    lcd.clear();
    lcd.print("sms sent2");
     delay(2000);
 }



void serialEvent()
{
  while (Serial.available())
  {
    char inChar = (char)Serial.read();
    //Serial.print(inChar);

    if (inChar == 'A')
    {
      t1 = "SLEPT-->> EMERGENCY ALERT";
      t2 = "NORMAL";
      digitalWrite(23, HIGH);
      lcd.setCursor(0, 0);
      lcd.print("DRIVER ");
      lcd.setCursor(0, 1);
      lcd.print("SLEPT.");
      //server.handleClient();
      delay(10000);
       send_sms1();
      delay(10000);
      delay(10000);
      lcd.clear();
    }
    else
    {
       t1 = "SLEPT-->> EMERGENCY ALERT";
      t2 = "NORMAL";
      digitalWrite(23, HIGH);
      lcd.setCursor(0, 0);
      lcd.print("DRIVER ");
      lcd.setCursor(0, 1);
      lcd.print("SLEPT");
      //server.handleClient();
      delay(10000);
       send_sms1();
      delay(10000);
      delay(10000);
      lcd.clear();
    }
  }
}


void loop()
{
 al = analogRead(34);
   serialEvent();
   if (al<200)
   {
    serialEvent();
     t2 = "ALCOHOL DRUNKEN-->> EMERGENCY ALERT";
      t1 = "NORMAL";
  lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ALCOHOL");
    lcd.setCursor(0, 1);
    lcd.print("DRIVING");
    server.handleClient();
    delay(500);
    digitalWrite(23, HIGH);
    send_sms2();
    delay(10000);
    delay(10000);
   }
   else
   {serialEvent();
     t2 = "NORMAL";
      t1 = "NORMAL";
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("NORMAL");
    server.handleClient();
    digitalWrite(23, LOW);
    delay(500);
   }
   
  


}
