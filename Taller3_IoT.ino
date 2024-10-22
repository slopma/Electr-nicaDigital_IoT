#include "Ubidots.h"

const char* UBIDOTS_TOKEN = "BBUS-Q4IG2032U3rht6lMoxHTPBHxwdaqGq";
const char* WIFI_SSID = "IoT-B19";
const char* WIFI_PASS = "lcontrol2020*";
Ubidots* ubidots{nullptr};


const int IN3 = 33;
const int IN4 = 32;
const int LM35 = 35;
const int POT = 34;

void setup() {
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  Serial.begin(115200);
  Ubidots::wifiConnect(WIFI_SSID, WIFI_PASS);
  ubidots = new Ubidots(UBIDOTS_TOKEN, UBI_HTTP);
}


void loop() {

  float temperatura = leerTemperatura();
  float voltaje = leerPotenciometro();
  int estadoMotor = ubidots->get("6717bcf77ff59834620f6fdf", "estadomotor");
  int velocidadMotor = ubidots->get("6717bcfd8a3f1a306eef3a15", "velocidadmotor");
  
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" °C");

  Serial.print("Valor Potenciómetro: ");
  Serial.print(voltaje);
  Serial.println(" V");


  ubidots->add("temperatura-lm35", temperatura);
  ubidots->add("voltaje-potenciometro", voltaje);
  ubidots->send();


  girarMotor(estadoMotor, velocidadMotor);
}


float leerTemperatura() {
  int valor = analogRead(LM35);
  float temperatura = (valor * 5.0 * 100.0) / 4095.0;
  return temperatura;
}

float leerPotenciometro() {
  int valor = analogRead(POT);
  float voltaje = (valor / 4095.0) * 3.3;
  Serial.println(valor);
  return voltaje;
}

void girarMotor(int estadoMotor, int velocidadMotor) {
  if (estadoMotor == 0) {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  } else {
    girarMotor(velocidadMotor);
  }
}
void girarMotor(int velocidadMotor) {
  if (velocidadMotor == 0) {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  } else if (velocidadMotor != 0) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
}