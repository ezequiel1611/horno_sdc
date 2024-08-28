#include <Arduino.h>

// pines
#define CONTROL 35  // Pin de lectura del potenciometro de ajuste
#define TRIGGER 18  // Pin de disparo del optoacoplador
#define INTR_SYNC 5 // Pin de detección de cruce por cero

// variables
int potencia, ge = 0;
float rms;

// timer
hw_timer_t *Timer0_Cfg = NULL;

// put function declarations here:
void IRAM_ATTR grado_electrico();
void IRAM_ATTR detector();

void setup() {
  // put your setup code here, to run once:
  pinMode(CONTROL, INPUT);
  pinMode(TRIGGER, OUTPUT);
  pinMode(INTR_SYNC, INPUT);
  attachInterrupt(INTR_SYNC, detector, CHANGE);
  Timer0_Cfg = timerBegin(0, 45, true);
  timerAttachInterrupt(Timer0_Cfg, &grado_electrico, true);
  timerAlarmWrite(Timer0_Cfg, 99, true);
  timerAlarmEnable(Timer0_Cfg);
  digitalWrite(TRIGGER, LOW);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  potencia = analogRead(CONTROL); 
  potencia = map(potencia,0,4095,0,179); // Conversión de tensión a grados eléctricos
  rms = (ge*100)/179.0;
  Serial.print("Potencia: ");
  Serial.print(rms);
  Serial.println("%");
}

// put function definitions here:
void IRAM_ATTR detector() {
  ge = 0; // En cada cruce por cero se resetea la cuenta de los grados electricos
}

void IRAM_ATTR grado_electrico(){
  ge++;
  if(potencia == ge){
    digitalWrite(TRIGGER, HIGH);
    digitalWrite(TRIGGER, LOW); // Si el optoacoplador no se dispara, introducir un delay
  }
}