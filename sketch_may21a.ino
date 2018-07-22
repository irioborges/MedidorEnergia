//ÁREA DE DECLARAÇÃO DE BIBLIOTECAS
#include <EmonLib.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <Sim800l.h>

void setup() {
  Serial.begin(9600);
  Serial.print("Limpando SMS antigos...");
}

void loop() {
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(1000);
}
