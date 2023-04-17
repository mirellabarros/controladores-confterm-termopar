#include "DHT.h"

#define DHTPIN1 2      // pino que estamos conectado
#define DHTPIN2 3      // pino que estamos conectado
#define DHTTYPE DHT11  // DHT 11

DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);

void setup() {

  // Inicializa a porta serial 0
  Serial.begin(9600);

  Serial.println("==================================================");
  Serial.println("Iniciando...");
  Serial.println("==================================================");

  // Inicializa a porta serial 2
  Serial2.begin(9600);

  // Inicializa os sensores DHT
  dht1.begin();
  dht2.begin();
}

void loop() {  // run over and over

  delay(2000);

  // Envia o código de paridade
  Serial2.print(99);

  delay(2000);

  float h1 = dht1.readHumidity();
  float t1 = dht1.readTemperature();

  float h2 = dht2.readHumidity();
  float t2 = dht2.readTemperature();

  // testa se retorno é valido, caso contrário algo está errado.
  if (isnan(t1) || isnan(h1)) {
    Serial.println("Failed to read from DHT");
  } else {
    Serial.print("Umidade 1: ");
    Serial.print(h1);
    Serial.print(" %\t");
    Serial.print("Temperatura 1: ");
    Serial.print(t1);
    Serial.println(" *C");
    Serial2.print(t1);
  }
  delay(1500);
  // testa se retorno é valido, caso contrário algo está errado.
  if (isnan(t2) || isnan(h2)) {
    Serial.println("Failed to read from DHT");
  } else {
    Serial.print("Umidade 2: ");
    Serial.print(h2);
    Serial.print(" %\t");
    Serial.print("Temperatura 2: ");
    Serial.print(t2);
    Serial.println(" *C");
    Serial.println("-----");
    Serial2.print(t2);
  }

  delay(10000);

  if (Serial2.available()) {
    int x22 = Serial2.read();
    Serial.write(x22);
    //Serial.write(Serial2.read());
  }
  if (Serial.available()) {
    int x32 = Serial.read();
    Serial2.write(x32);
    // Serial2.write(Serial.read());
  }
}