#include "DHT.h"
#include "max6675.h"
#include "SPI.h"
#include <LiquidCrystal_I2C.h>

// Define o número de linhas e colunas do LCD
int lcdColumns = 16;
int lcdRows = 2;

// Define o endereço do LCD, número de linhas e colunas
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

// Determina os pinos e tipo dos sensores DHT
#define DHTPIN1 9  // DHT TBS
// #define DHTPIN2 8      // DHT TGN
#define DHTTYPE DHT22  // Sensor tipo DHT 22

DHT dht1(DHTPIN1, DHTTYPE);  // Define o sensor TBS
// DHT dht2(DHTPIN2, DHTTYPE);  // Define o sensor TGN

// Define os pinos do KIMO
const int pino_KIMO = A8;
float KIMO = 0;
float KIMO_temp = 0;

// Define os pino e variáveis do Wind Sensor
const int vento_WS = A1;  // wind sensor analog pin  hooked up to Wind P sensor "OUT" pin
const int temp_WS = A2;

float valor_ant = 0;
float valor_atual = 0;
float valorfinal = 0;  // seta variaveis
float valorfinal22 = 0;
int windADunits = 0;
float windMPH = 0;
float windMPS = 0;

// Define os sensores termopar tipo K MAX6675

// Termopar 1
int thermo1CLK = 22;  // sck: amarelo
int thermo1CS = 24;   // so: verde
int thermo1D0 = 26;   // miso: laranja
float tk1 = 0;
MAX6675 thermocouple1(thermo1CLK, thermo1CS, thermo1D0);

// Termopar 2
int thermo2CLK = 30;  // sck: amarelo
int thermo2CS = 32;   // so: verde
int thermo2D0 = 34;   // miso: laranja
float tk2 = 0;
MAX6675 thermocouple2(thermo2CLK, thermo2CS, thermo2D0);

// Termopar 3
int thermo3CLK = 5;  // sck: amarelo
int thermo3CS = 6;   // so: verde
int thermo3D0 = 7;   // miso: laranja
float tk3 = 0;
MAX6675 thermocouple3(thermo3CLK, thermo3CS, thermo3D0);

// Termopar 4
int thermo4CLK = 38;  // sck: amarelo
int thermo4CS = 40;   // so: verde
int thermo4D0 = 42;   // miso: laranja
float tk4 = 0;
MAX6675 thermocouple4(thermo4CLK, thermo4CS, thermo4D0);

// Termopar 5
int thermo5CLK = 45;  // sck: amarelo
int thermo5CS = 47;   // so: verde
int thermo5D0 = 49;   // miso: laranja
float tk5 = 0;
MAX6675 thermocouple5(thermo5CLK, thermo5CS, thermo5D0);

// Vetores da média móvel
const int index = 5;
int index_atual = 0;

float leituras_tk1[index];
float leituras_tk2[index];
float leituras_tk3[index];
float leituras_tk4[index];
float leituras_tk5[index];

// // Termopar 6 - Temperatura de globo negro (TGN) alterado de DHT22 -> Termopar
// int thermo6CLK = 23;  // sck: laranja
// int thermo6CS = 25;   // cs: marrom
// int thermo6D0 = 27;   // miso: verde
// float tgn = 0;
// MAX6675 thermocouple6(thermo6CLK, thermo6CS, thermo6D0);

// Botão ===============================================
const int buttonPin = 3;  // pushbutton habilita leitura = GND usa pullup interno
// variables will change:
int buttonState = 0;  // variable for reading the pushbutton status

void setup() {

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(DHTPIN1, INPUT_PULLUP);
  // pinMode(DHTPIN2, INPUT_PULLUP);

  // Inicializa a porta serial 0
  Serial.begin(9600);
  // Inicializa a porta serial 3
  Serial3.begin(9600);

  Serial.println("################################################");
  Serial.println("    Medição de Temperatura, Vento e Umidade     ");
  Serial.println("################################################");

  // Inicializa os sensores DHT
  dht1.begin();
  // dht2.begin();

  // Inicializa o LCD
  // lcd.init();
  // lcd.backlight();              // ativa a iluminação
  // lcd.setCursor(0, 0);          // posiciona o cursor em 0,0
  // lcd.print("Proj Conf Term");  // escreve mensagem
  // delay(1000);

  // lcd.setCursor(0, 1);           // posiciona o cursor em 0,1
  // lcd.print("Temp Vento Umid");  // escreve mensagem
  // delay(3000);
}

void loop() {

  // Aguarda leitura do botão de início
  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH) {

    // turn LED on:
    Serial.println("Chave = OFF");

    // lcd.clear();
    // lcd.setCursor(0, 0);
    // lcd.print("Proj Conf Term");
    // lcd.setCursor(0, 1);
    // lcd.print("Chave = OFF");
    delay(100);

  } else {

    // lcd.clear();
    // lcd.setCursor(0, 0);
    // lcd.print("Temp Vento Umid");  // escreve mensagem
    // lcd.setCursor(0, 1);
    // lcd.print("Exec. leituras");

    publicar("99");  // envia o código de paridade
    delay(1500);

    // lcd.clear();
    // lcd.setCursor(0, 0);
    // lcd.print("Exec. leituras");
    // lcd.setCursor(0, 1);
    // lcd.print("Termopares");

    ler_termopares();

    index_atual++;

    if (index_atual >= index) {
      index_atual = 0;
    }
    
    delay(1500);

    // lcd.clear();
    // lcd.setCursor(0, 0);
    // lcd.print("Exec. leituras");
    // lcd.setCursor(0, 1);
    // lcd.print("Sensor TBS");

    ler_TBS();
    delay(1500);

    // lcd.clear();
    // lcd.setCursor(0, 0);
    // lcd.print("Exec. leituras");
    // lcd.setCursor(0, 1);
    // lcd.print("Sensor TGN");

    // ler_TGN();
    publicar(String(0.00));
    delay(1500);

    // lcd.clear();
    // lcd.setCursor(0, 0);
    // lcd.print("Exec. leituras");
    // lcd.setCursor(0, 1);
    // lcd.print("Sensor Kimo");

    ler_KIMO();
    delay(1500);

    // lcd.clear();
    // lcd.setCursor(0, 0);
    // lcd.print("Leituras OK");

    delay(10000);
  }

  delay(10000);

  if (Serial3.available() > 0) {
    int x22 = Serial3.read();
    Serial.write(x22);
    //Serial.write(Serial3.read());
  }
  if (Serial.available() > 0) {
    int x32 = Serial.read();
    Serial3.write(x32);
    // Serial3.write(Serial.read());
  }
}

void publicar(String dados) {
  Serial3.print(dados);
}

void ler_termopares() {

  Serial.println("------------------------------------------------");
  Serial.println("             Leitura dos Termopares             ");
  Serial.println("------------------------------------------------");

  // Termopar 1
  float total_tk1 = 0;
  float total_tk2 = 0;
  float total_tk3 = 0;
  float total_tk4 = 0;
  float total_tk5 = 0;

  for (int i = 0; i < 5; i++) {
    float valor_tk1 = thermocouple1.readCelsius();
    total_tk1 += valor_tk1;
    float valor_tk2 = thermocouple2.readCelsius();
    total_tk2 += valor_tk2;
    float valor_tk3 = thermocouple3.readCelsius();
    total_tk3 += valor_tk3;
    float valor_tk4 = thermocouple4.readCelsius();
    total_tk4 += valor_tk4;
    float valor_tk5 = thermocouple5.readCelsius();
    total_tk5 += valor_tk5;
    delay(250);
  }

  Serial.print("MM Tk1 (C) = ");
  float media_tk1 = (total_tk1 / 5) + 0.85;
  float mm_tk1 = calcula_mm_tk(leituras_tk1, media_tk1);
  Serial.println(mm_tk1);
  Serial.print("Leitura atual Tk1: ");
  Serial.println(media_tk1);
  publicar(String(mm_tk1));
  delay(1200);

  Serial.print("MM Tk2 (C) = ");
  float media_tk2 = total_tk2 / 5;
  float mm_tk2 = calcula_mm_tk(leituras_tk2, media_tk2);
  Serial.println(mm_tk2);
  Serial.print("Leitura atual Tk2: ");
  Serial.println(media_tk2);
  publicar(String(mm_tk2));
  delay(1200);

  Serial.print("MM Tk3 (C) = ");
  float media_tk3 = (total_tk3 / 5) - 0.2;
  float mm_tk3 = calcula_mm_tk(leituras_tk3, media_tk3);
  Serial.println(mm_tk3);
  Serial.print("Leitura atual Tk3: ");
  Serial.println(media_tk3);
  publicar(String(mm_tk3));
  delay(1200);

  Serial.print("MM tk4 (C) = ");
  float media_tk4 = (total_tk4 / 5) - 0.3;
  float mm_tk4 = calcula_mm_tk(leituras_tk4, media_tk4);
  Serial.println(mm_tk4);
  Serial.print("Leitura atual Tk4: ");
  Serial.println(media_tk4);
  publicar(String(mm_tk4));
  delay(1200);

  Serial.print("MM tk5 (C) = ");
  float media_tk5 = total_tk5 / 5;
  float mm_tk5 = calcula_mm_tk(leituras_tk5, media_tk5);
  Serial.println(mm_tk5);
  Serial.print("Leitura atual Tk5: ");
  Serial.println(media_tk5);
  publicar(String(mm_tk5));
  delay(1200);

  // Calcula a média
  Serial.println("\t");
  Serial.print("Média Termopar = ");
  float temp_mm_TK = (mm_tk1 + mm_tk2 + mm_tk3 + mm_tk4 + mm_tk5) / 5;
  publicar(String(temp_mm_TK));
  Serial.println(temp_mm_TK);

  Serial.print("Index atual: ");
  Serial.println(index_atual);

  Serial.println("\t");
}

float calcula_mm_tk(float vetor[index], float dado) {

  float total = 0;

  for (int i = 0; i < index; i++) {
    total += + vetor[i];
    // Serial.println(vetor[i]);
  }

  vetor[index_atual] = dado;

  return total / index;
}

void ler_TBS() {

  // TBS - DHT1
  Serial.println("------------------------------------------------");
  Serial.println("      Leitura do Sensor DHT22: Bulbo Seco       ");
  Serial.println("------------------------------------------------");

  // DHT22 sampling rate is 0.5HZ.
  delay(2000);
  float t1 = dht1.readTemperature();
  float h1 = dht1.readHumidity();

  // Verifica se os dados dos sensores são válidos
  if (isnan(t1) || isnan(h1)) {

    Serial.println("Erro de leitura do sensor TBS");
    t1 = 0;  // Envia o valor zero para t1
    h1 = 0;  // Envia o valor zero para h1
    publicar(String(t1));
    delay(1500);
    publicar(String(h1));
    delay(1500);

  } else {

    Serial.print("TBS (C) = ");
    Serial.println(t1, 1);
    Serial.print("Umidade TBS (%) = ");
    Serial.println(h1, 1);
    publicar(String(t1));
    delay(1500);
    publicar(String(h1));
    delay(1500);
  }

  Serial.println("\t");
}

// void ler_TGN() {

//   // TGN - Termopar Tk
//   Serial.println("------------------------------------------------");
//   Serial.println("     Leitura do Sensor Termopar: Globo Negro    ");
//   Serial.println("------------------------------------------------");

//   tgn = thermocouple6.readCelsius();
//   delay(300);
//   Serial.print("TGN (C) = ");
//   publicar(String(tgn));
//   Serial.println(tgn);
//   delay(1500);

//   // Verifica se o dado do sensor é válido
//   // if (isnan(tgn)) {

//   //   Serial.println("Erro de leitura do sensor TGN");
//   //   tgn = 0;  // Envia o valor zero para t2
//   //   publicar(String(tgn));
//   //   delay(1500);

//   // } else {


//   // }

//   Serial.println("\t");
// }

void ler_KIMO() {

  // Sensor de vento KIMO
  Serial.println("------------------------------------------------");
  Serial.println("            Leitura Anemômetro KIMO             ");
  Serial.println("------------------------------------------------");

  // Leitura do KIMO : 1V = 1m/s

  for (int i = 0; i <= 99; i++) {

    KIMO_temp = analogRead(pino_KIMO);
    float KIMO2 = (KIMO_temp / 1024) * 4.97;  // 4.71 = Aref com cabo USB do notebook
    valor_atual = valor_atual + KIMO2;        //valor_atual;
  }

  valorfinal = valor_atual / 100;

  KIMO = valorfinal;  //(valorfinal/1023)*5; // transformar conversão 10 bits para Volts

  Serial.print("KIMO (1V = 1m/s) (m/s) = ");
  Serial.println(KIMO);
  publicar(String(KIMO));

  valor_atual = 0;

  Serial.println("\t");
}

// ***** Temperatura do Wind Sensor Modern Device *****
// MB função wind_WS() não revisada (17/04/2023 - MB)

void wind_WS() {
  // selecionando coluna 0 e linha 0
  // Print a message to the LCD.
  // lcd.display();
  delay(300);

  for (int j = 0; j <= 4; j++) {

    for (int i = 0; i <= 99; i++) {  //  cria laço de 100 voltas

      ///////       read wind   ////////////

      int valorA0 = analogRead(vento_WS);

      windMPH = pow((((float)valorA0 - 264.0) / 85.6814), 3.36814);
      // pow(X,Y) eleva X ((float)valorA0 - 264.0) / 85.6814))por Y (3.36814)

      // Here’s the equation derived from the regression for the Rev P wind sensor.
      // WS_MPH = (((Volts – ZeroWind_V) / (3.038517 * (Temp_C ^ 0.115157 ))) / 0.087288 ) ^ 3.009364

      //You’ll need to code this up in C. Most people will be using microcontrollers
      //with ADC’s so you’ll need to input the reference voltage of your microcontroller
      //and the bit depth of the ADC. For example with an Arduino UNO or Mega
      //running at 5.0 volts, which have a 10-bit ADC, you would substitute
      //the following expression for the term “Volts”  “(float)analogRead(A0) * 5.0 / 1023.0”


      windMPS = (windMPH / 2.237);  //3.95; // conversão milha/h para m/s

      valor_atual = valor_atual + windMPS;  //valor_atual;
      //valor_atual = valor_atual + valor_ant;

      //valor_ant = valor_atual;
      //Serial.print("Valor total = "); Serial.println(windMPS);
      delay(2);
    }

    valorfinal = valor_atual / 100;
    valor_atual = 0;
  }

  valorfinal22 = valorfinal / 5;  // faz 100 leituras e depois repete mais 5x = 500 leituras!!!

  // wind formula derived from a wind tunnel data, annemometer and some fancy Excel regressions
  // this scalin doesn't have any temperature correction in it yet
  // float windMPH =  pow((((float)windADunits - 264.0) / 8.56814), 3.36814);
  //float windMPS = windMPH/2.237; // conversão milha/h para m/s

  Serial.print("Valor Medio = ");
  Serial.print(valorfinal22);
  //Serial.print(" MPH\t");
  Serial.println(" MPS\t");
}

// ***** Temperatura do Wind Sensor Modern Device *****
// MB função temp_wind_WS() não revisada (17/04/2023 - MB)

void temp_wind_WS() {
  //lcd.begin(16, 2);
  delay(300);
  // lcd.setCursor(0, 0);
  // escrever no LCD a velocidade do vento
  // lcd.print("Vento=");
  //  lcd.setCursor(8, 0);
  //  lcd.print(valorfinal22);
  //  lcd.setCursor(13, 0);
  //  lcd.print("m/s");

  // temp routine and print raw and temp C
  //analogReadResolution(10);
  int tempRawAD = analogRead(temp_WS);
  // Serial.print("RT ");    // print raw A/D for debug
  // Serial.print(tempRawAD);
  // Serial.print("\t");

  // convert to volts then use formula from datatsheet
  // Vout = ( TempC * .0195 ) + .400
  // tempC = (Vout - V0c) / TC   see the MCP9701 datasheet for V0c and TC

  float tempC = ((((float)tempRawAD * 5.0) / 1024.0) - 0.400) / .0195;
  Serial.print(tempC);
  Serial.println(" C");
  delay(150);

  // delay(100);
  // lcd.setCursor(0, 1);
  // lcd.print("Temp=");
  // lcd.setCursor(8, 1);
  // lcd.print(tempC);
  // lcd.setCursor(13, 1);
  // lcd.print(" C");
}
