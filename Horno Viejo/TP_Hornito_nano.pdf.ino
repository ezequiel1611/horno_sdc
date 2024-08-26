/*   Escalon al 60% 
     
     This example code is in the public domain.
*/

// These constants won't change.  They're used to give names
// to the pins used:
const int analogInPin = A0;  // Entrada analógica de la Temperatura
const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin
const int OutTriacPin =  4;  // D4 - Pin 20 para disparar el TRIAC
const int InCruceZero =  3;  // D3 - Pin 21 lee el detector de cruce por cero

int sensorValue = 0;         // value read from the LM335
int contador = 1;            // contador de lecturas
int buttonState = 0;         // variable for reading the pushbutton status
int parpadeo = 100;          // tiempo entre parpadeo del led
int Cruce0State = 0;         // Estado del cruce por cero

void setup() {
  // initialize serial communications at 115200 bps:
  Serial.begin(115200);
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  // inicializa el disparo del TRIAC en bajo
  pinMode(OutTriacPin, OUTPUT);
  // inicializa la lectura del cruce por cero
  pinMode(InCruceZero, INPUT);
  // uso 3V3 con resistencias en serie para ajustar a 1V AREF
  analogReference(EXTERNAL);
  // activo la interrupción que dispara el cruce por cero.
  attachInterrupt(digitalPinToInterrupt (InCruceZero), cruzo, RISING);
}


// *************************************************
// se hace una especie de PWM sobre la base del
// período de tiempo de limiteOn + limiteOff
//
// limiteOn semiciclos encendido
// limiteOff semiciclos apagados
// limiteOn = limOn[k0];
// limiteOff = 5 - limOn[k0];
// cantRep es la cantidad de veces que repetimos PWM
// Tiempo del experimento es:
// cantRep*(limiteOn + limiteOff)/100 [segundos]

float cantRep[] = {1, 48000}; // TIEMPO QUE ESTARÁ ENCENSIDO en minutos
// 1200 es el factor de escala
int limOn[]   = {0, 1};  // INTENSIDAD DEL ENCENDIDO DEL FOCO DE 0 A 5
int CantElem  = 2;                          // CANTIDAD DE ESCALONES
int limiteOn;
int limiteOff;
int contOn = 0;
int contOff = 0;

bool encender = false;
void cruzo (void) {
  if (encender) {
    if (contOn < limiteOn) {
      digitalWrite(OutTriacPin, HIGH); // Prender Triac
      contOn ++;
      contOff = 0;
    }
    else {
      digitalWrite(OutTriacPin, LOW); // Apagar Triac
      if (contOff < limiteOff) {
        contOff ++;
      }
      else {
        contOn = 0;
      }
    }
  }
}

void loop() {
  int sumaCont = 0, sumaLim = 5, sumaPrev = 0;

  // parpadea el led esperado inicio con botón
  buttonState = digitalRead(buttonPin);
  while (buttonState == LOW) {
    digitalWrite(ledPin, LOW);
    delay (parpadeo / 4);
    buttonState = digitalRead(buttonPin);
    digitalWrite(ledPin, HIGH);
    delay (parpadeo / 4);
  }
  for (int k = 0; k < 5; k++) {
    digitalWrite(ledPin, LOW);
    delay (parpadeo);
    digitalWrite(ledPin, HIGH);
    delay (parpadeo);
  }
  digitalWrite(ledPin, LOW);
  buttonState = LOW;

  // *******************************************
  for (int k0 = 0; k0 < CantElem; k0++) {
    limiteOn = limOn[k0];
    limiteOff = 5 - limOn[k0];

    encender = true;
    for (int k = 0; k < cantRep[k0]; k++) {
      contOn = 0;
      contOff = 0;
      sumaCont = contOn + contOff;
  
      while ( sumaCont < sumaLim)
      {
        //  leo temperatura y transmito
        if (sumaCont != sumaPrev) {
          sensorValue = analogRead(analogInPin);
          Serial.print(limiteOn);
          Serial.print(" ");
          Serial.println(sensorValue);
          sumaPrev = sumaCont;
        }
        buttonState = digitalRead(buttonPin);
        if (buttonState != LOW) {
          Serial.print(0);
          Serial.print(" ");
          Serial.println(0);
          encender = false;
          digitalWrite(OutTriacPin, LOW); // Apagar Triac
          break;
       
      }
      sumaCont = contOn + contOff;
    }
  }
  encender = false;
  digitalWrite(OutTriacPin, LOW); // Apagar Triac
}
// *******************************************

while (1 < 2) {
  Serial.print(0);
  Serial.print(" ");
  Serial.println(0);
  delay(1);
}
}

