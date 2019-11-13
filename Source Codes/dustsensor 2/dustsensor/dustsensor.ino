#define dustAnalog A1
#define dustLed 10

#define Voc 0.4


int data[10] = {40,40,40,40,40,40,40,40,40,40};
float avg(int x);
float DustResult = 0;

void setup() {
  Serial.begin(9600);
  pinMode(dustLed, OUTPUT);
}

void loop() {
  digitalWrite(dustLed, LOW);
  delayMicroseconds(280);
  float readDust = analogRead(dustAnalog);
  delayMicroseconds(40);
  digitalWrite(dustLed, HIGH);
  delayMicroseconds(9680); 

  float voltage = readDust * (5.0 / 1024.0);
  Serial.println(voltage);
  float dust = (voltage - Voc) / 0.005;
  DustResult = avg(dust);
  

  if(DustResult > 100) { 
     Serial.println("Bad");
     
  }
  else if(DustResult > 50) {
    Serial.println("Normal");
  }
  else {
    Serial.println("Good");
  }
    
  Serial.print("Dust : ");
  Serial.print(DustResult);
  Serial.println("[ug/m3]");
  delay(1000);
}



float avg(int x){
    unsigned char i;
    float sum=0;
    float average=0;
    for(i = 0; i < 10-1; i++){
      data[i]=data[i+1];
    }
    data[10-1]=x;
    
    for(i=0;i<10;i++){
      sum += data[i];
    }
    average = sum / 10;
    return average;
  }
