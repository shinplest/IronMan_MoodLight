int dustPin = A5;   
int ledP = 10;   
float voMeasured = 0;  
float calcVoltage = 0;  
float dustDensity = 0;  
void setup(){  
 Serial.begin(115200);  
 pinMode(ledP,OUTPUT);  
}  
  
void loop(){  
 digitalWrite(ledP,LOW); // power on the LED  
 delayMicroseconds(280);  
 voMeasured = analogRead(dustPin); // read the dust value  
 delayMicroseconds(40);  
 digitalWrite(ledP,HIGH); // turn the LED off  
 delayMicroseconds(9680);  
 // 0 - 5V mapped to 0 - 1023 integer values  
 calcVoltage = voMeasured * (5.0 / 1024.0);  
 dustDensity = 0.17 * calcVoltage - 0.1;  
 dustDensity = dustDensity*1000; // ug/m3  
 Serial.println(dustDensity);    
 delay(1000);  
}  int dustPin = A5;   
int ledP = 10;   
float voMeasured = 0;  
float calcVoltage = 0;  
float dustDensity = 0;  
void setup(){  
 Serial.begin(115200);  
 pinMode(ledP,OUTPUT);  
}  
  
void loop(){  
 digitalWrite(ledP,LOW); // power on the LED  
 delayMicroseconds(280);  
 voMeasured = analogRead(dustPin); // read the dust value  
 delayMicroseconds(40);  
 digitalWrite(ledP,HIGH); // turn the LED off  
 delayMicroseconds(9680);  
 // 0 - 5V mapped to 0 - 1023 integer values  
 calcVoltage = voMeasured * (5.0 / 1024.0);  
 dustDensity = 0.17 * calcVoltage - 0.1;  
 dustDensity = dustDensity*1000; // ug/m3  
 Serial.println(dustDensity);    
 delay(1000);  
}  
