#include<LiquidCrystal.h>
#include<SoftwareSerial.h>
LiquidCrystal lcd(12,11,5,4,3,2);
const int button=10;
const int IR1=6;
const int IR2=7;
const byte tx=8,rx=9;
SoftwareSerial esp8266(rx,tx);
String ssid="Nokia 8";
String password="1234567801";
boolean found= false;
int value;
String api ="8AXZT5J931R8M2K9";
String host="api.thingspeak.com";
String port="80";
int count=0;

void setup() {
  pinMode(button,INPUT);
  lcd.begin(16,2);
  lcd.print("VISITORS COUNT=");
  pinMode(IR1,INPUT); 
  pinMode(IR2,INPUT); 
  Serial.begin(9600);
  esp8266.begin(115200);
  Serial.println("IOT PROJECT");
  connectwifi();
  
  // put your setup code here, to run once:

}

void loop() {
  while(digitalRead(button)==0) //activates the monitoring systems
  {
    if(digitalRead(IR1)==0)
    {
      if(digitalRead(IR2)==0)
      {
        count=count+1;
        senddata();
        Send_sms("VISITOR COUNT: ",count,"6362054291");
        S_print(count);
      }
      else
      {
        senddata();
      }
      
    }
    else
    {
      senddata();
    }
    
  }
  delay(2000);
  
    
}
void sendcommand(String command,int maxtime,char readreply[])
{
  Serial.print(".at command=>");
  Serial.print(command);
  Serial.print(" ");
  while(maxtime--)
  {
    esp8266.println(command);
    if (esp8266.find(readreply))
    {
      found=true;break;
    }
  }
  if(found)
  {
    Serial.println("OK DONE");
  }
  else
  {
    Serial.println("FAIL");
  }
  found=false;
}
void connectwifi()
{
  sendcommand("AT",5,"OK");
  sendcommand("AT+CWMODE=1",5,"OK");
  sendcommand("AT+CWJAP=\"" + ssid +"\",\""+ password +"\"",20,"OK");
}
void senddata()
{
  String getData1 = "GET /update?api_key="+ api;
  String getData2 = "&field1="+String(count);
  sendcommand("AT+CIPSTART=\"TCP\",\""+host +"\","+ port, 15, "OK"); 
  sendcommand("AT+CIPSEND=" + String (getData1.length()+ getData2.length()+2), 7, ">");
  delay(10);
  esp8266.println(getData1+getData2); Serial.println(getData1 + getData2);
  delay(1000);
  sendcommand("AT+CIPCLOSE=0",5, "OK");
  Serial.println(".................................");
}
void Send_sms(String str,int count, String num)
{
  Serial.println("AT");
  delay(500);
  Serial.println("AT+CMGF=1");
  delay(500);
  Serial.println("AT+CMGS=\"");
  Serial.print(num);
  Serial.print("\"");
  delay(500);
  Serial.print(str);
  Serial.println(count);
  delay(500);
  Serial.write(0x1a);
}
void S_print(int count)
{
  Serial.print("CURRENT VISITOR COUNT: ");
  Serial.print(count);
  lcd.setCursor(1,1);
  lcd.print(count);
  delay(1000);
}
