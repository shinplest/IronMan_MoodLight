#include <SoftwareSerial.h>
 
int Tx=6; //전송
int Rx=7; //수신
 
SoftwareSerial btSerial(Tx, Rx);
 
void setup() 
{
  Serial.begin(9600);
  btSerial.begin(9600);
  pinMode(12, OUTPUT);
}
 
void loop()
{
  if (btSerial.available()) {  
    char data = btSerial.read();  
    Serial.write(data);
    if(data == "1")
    {
      digitalWrite(12, HIGH);
    }
    if(data == "2")
    {
      digitalWrite(12, LOW);
    }
    
  }
 

}
