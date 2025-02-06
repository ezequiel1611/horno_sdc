#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
/*
  Incluir estas dos librerias para usar el sensor DHT11 o DHT22
*/
//#include "DHT.h"
//#include <Adafruit_Sensor.h>
/*
  Incluir estas tres librerias para usar el BMP180 o BMP085
*/
#include <Wire.h>
#include <SPI.h>
//#include <Adafruit_BMP085.h>

// datos de red WiFi
#define WIFI_SSID "nombre_red"
#define WIFI_PASSWORD "clave_red"

// firebase API key
#define API_KEY "clave_api_firebase"

// url a la base de datos
#define DATABASE_URL "rtdb_url"

// datos de autenticación
#define USER_EMAIL "email_user"
#define USER_PASSWORD "clave_user"

// pines
#define TRIGGER 4  // Pin de disparo del optoacoplador
#define INTR_SYNC 18 // Pin de detección de cruce por cero
#define START 21    // Pin de encendido
//Adafruit_BMP085 bmp;  // GPIO22(D22)=SCL GPIO21(D21)=SDA
//#define DHTPIN 22   // Pin de lectura del sensor de temperatura
//#define DHTTYPE DHT11

// inicialización de la base de datos
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// inicialización del sensor DHT
//DHT dht(DHTPIN, DHTTYPE);

// variables
int potencia, disparo, ge = 0;
float t=30;
volatile bool onePulse = false;     // Bandera para segurar un disparo por semiciclo
unsigned long sendDataPrevMillis = 0;
bool startFlag = false; 

// timer
hw_timer_t *Timer0_Cfg = NULL;
hw_timer_t *Timer1_Cfg = NULL;

// declaracion de funciones
void IRAM_ATTR grado_electrico();
void IRAM_ATTR detector();

void setup() {
  // conexión a wifi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while(WiFi.status() != WL_CONNECTED){
    delay(300);
  }
  // configuración base de datos firebase
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);
  config.token_status_callback = tokenStatusCallback;
  config.max_token_generation_retry = 5;
  Firebase.begin(&config, &auth);
  // configuración de pines e interrupción externa
  pinMode(START, OUTPUT);
  pinMode(TRIGGER, OUTPUT);
  pinMode(INTR_SYNC, INPUT);
  attachInterrupt(INTR_SYNC, detector, CHANGE);
  // configuración del timer 0 para incrementar los grados eléctricos
  Timer0_Cfg = timerBegin(0, 45, true);
  timerAttachInterrupt(Timer0_Cfg, &grado_electrico, true);
  timerAlarmWrite(Timer0_Cfg, 99, true);
  digitalWrite(TRIGGER, LOW);
  // inicialización de la comunicación SPI con el BMP180
  //bmp.begin();
  // inicializacion del sensor DHT
  //dht.begin();
}

void loop() {
  // lectura del sensor de temperatura
  //t = bmp.readTemperature();
  //t = dht.readTemperature();
  // hago una petición a la base de datos
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    // envio de datoa a la base de datos
    Firebase.RTDB.setFloat(&fbdo, "READINGS/temperature", t);
    // toma de datos de la base de datos
    if(Firebase.RTDB.getInt(&fbdo, "/CONTROL/power/")){
      if(fbdo.dataType() == "int"){
        disparo = fbdo.intData();
      }
    }
    if(Firebase.RTDB.getBool(&fbdo, "/CONTROL/start/")){
      if(fbdo.dataType() == "boolean"){
        startFlag = fbdo.boolData();
        if(startFlag == true){digitalWrite(START, HIGH);}
        else{digitalWrite(START,LOW);}
      }
    }
  }
}

// put function definitions here:
void IRAM_ATTR detector() {
  ge = 0; // En cada cruce por cero se resetea la cuenta de los grados electricos
  onePulse = true;
  timerAlarmEnable(Timer0_Cfg);
}

void IRAM_ATTR grado_electrico(){
  ge++;
  if(disparo == ge && onePulse == true){
    onePulse = false;
    for(int i=0;i<20;i++) {
      digitalWrite(TRIGGER, HIGH); // Este for asegura que el pulso tenga la duración necesaria para activar el triac
    }
    digitalWrite(TRIGGER, LOW);
  }
}
