#define USE_AVG
#define N 50

const int sharpLEDPin = 8;
const int sharpVoPin = A1;  
static unsigned long VoRawTotal = 0;
static int VoRawCount = 0; 
static float Voc = 0.6;
const float K = 0.5;


void printFValue(String text, float value, String units, bool isLast = false) {
  Serial.print(text+"="+value+units);
  if (!isLast) {
    Serial.print(", ");
  }
}

void CalculateDust(){
  digitalWrite(sharpLEDPin, LOW);
  delayMicroseconds(280);
  int VoRaw = analogRead(sharpVoPin);
  digitalWrite(sharpLEDPin, HIGH);
  delayMicroseconds(9620);
 
  float Vo = VoRaw;
  VoRawTotal += VoRaw;
  VoRawCount++;
  if ( VoRawCount >= N ) {
    Vo = 1.0 * VoRawTotal / N;
    VoRawCount = 0;
    VoRawTotal = 0;
  } else {
    return;
  }
  Vo = Vo / 1024.0 * 5.0;
  float dV = Vo - Voc;
  if ( dV < 0 ) {
    dV = 0;
    Voc = Vo;
  }
  float dustDensity = dV / K * 100.0;
  printFValue("DustDensity", dustDensity, "ug/m3", true);
  Serial.println("");
}


// Arduino setup function.
void setup() {
  pinMode(sharpLEDPin, OUTPUT);
  Serial.begin(9600);
}

// Arduino main loop.
void loop() {  
  CalculateDust();
} // END PROGRAM
