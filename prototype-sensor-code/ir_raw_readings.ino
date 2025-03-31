#define T1 16
#define T2 15
#define T3 14
#define T4 13 
#define T5 12
#define T6 11
#define T7 10
#define T8 9
#define T9 8
#define T10 7
#define T11 6
#define T12 5
#define T13 4
#define T14 3
#define T15 2
#define T16 1

int sensorPins[16] = {T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16};

void setup() {
  // Setting up computer serial
  Serial.begin(115200);
  pinMode(45, OUTPUT);
  // Displaying LED to show running
  digitalWrite(45, HIGH);

  // Setting up IR pins as input
  for (int i=0; i<16; i++) {
    pinMode(sensorPins[i], INPUT);
  }

}

void loop() {
  // getting readings for every 5ms
  unsigned long startTime = millis();
  int readings[16] = {};
  while(millis()-startTime < 5) {
    for (int i=0; i<16; i++) {
      readings[i] += !digitalRead(sensorPins[i]);
    }
  }

  for (int i=0; i<16; i++) {
    Serial.print("Sensor T");
    Serial.print(i+1);
    Serial.print(": ");
    Serial.println(readings[i]);
  }
  Serial.println("");
  delay(50);
}
