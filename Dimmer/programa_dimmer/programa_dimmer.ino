#include<TimerOne.h>
#define control A1
#define opto 8
#define cruce_cero 2
int tension_control,ge=0;


void setup() {
  pinMode(control,INPUT);
  pinMode(opto,OUTPUT);
  pinMode(cruce_cero,INPUT);
  attachInterrupt(digitalPinToInterrupt(cruce_cero),detector,CHANGE);
  Timer1.initialize(55); //Tiempo entre cada grado electrico
  Timer1.attachInterrupt(grado_electrico);
  digitalWrite(opto,LOW);
  Serial.begin(9600);
}

void loop() {
  tension_control=analogRead(control);               //Convirtiendo la tension a
  tension_control=map(tension_control,0,1023,0,179); //grados electricos
  Serial.println(ge);
}

void detector(){
  ge=0; //Cada vez que hay un cruce por cero se resetea la cuenta
}

void grado_electrico(){
  ge++;
  if(tension_control==ge){   //Cuando la senal este en el angulo que coincide 
    digitalWrite(opto,HIGH); //con la tension de control se hace el disparo.
    //Serial.println("ON");
    digitalWrite(opto,LOW);  //Con poner el pin en alto y luego en bajo ya
    //Serial.println("OFF");
  }                          //genero un pulso suficiente para disparar el opto
}
