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
#define DHTPIN1 9      // DHT TBS
#define DHTPIN2 8      // DHT TGN
#define DHTTYPE DHT22  // Sensor tipo DHT 22

DHT dht1(DHTPIN1, DHTTYPE);  // Define o sensor TBS
DHT dht2(DHTPIN2, DHTTYPE);  // Define o sensor TGN

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

// Botão ===============================================
const int buttonPin = 3;  // pushbutton habilita leitura = GND usa pullup interno
// variables will change:
int buttonState = 0;  // variable for reading the pushbutton status

void setup() {

  pinMode(buttonPin, INPUT_PULLUP);

  // Inicializa a porta serial 0
  Serial.begin(9600);
  // Inicializa a porta serial 3
  Serial3.begin(9600);

  Serial.println("==================================================");
  Serial.println("       Medição Temperatura, Vento e Umidade       ");
  Serial.println("==================================================");

  delay(500);

  // Inicializa os sensores DHT
  dht1.begin();
  dht2.begin();

  // Inicializa o LCD
  lcd.init();
  lcd.backlight();              // ativa a iluminação
  lcd.setCursor(0, 0);          // posiciona o cursor em 0,0
  lcd.print("Proj Conf Term");  // escreve mensagem
  delay(1000);

  lcd.setCursor(0, 1);           // posiciona o cursor em 0,1
  lcd.print("Temp Vento Umid");  // escreve mensagem
  delay(3000);
}

void loop() {

  // Aguarda leitura do botão de início
  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH) {
    
    // turn LED on:
    Serial.println("Chave = OFF");
    lcd.setCursor(0, 1);
    lcd.print("  Chave = OFF   ");
    delay(100);

  } else {

    lcd.setCursor(0, 1);
    lcd.print("Inciando leituras...");
    publicar("99"); // envia o código de paridade
    delay(1500);
    ler_termopares();
    delay(1500);
    ler_TBS();
    delay(1500);
    // ler_TGN(); // temp globo negro ainda falta montar 14-04-23
    // delay(1500);
    ler_KIMO();
    delay(1500);
  }
  
  delay(10000);

  if (Serial3.available() > 0) {
    int x22 = Serial3.read();
    Serial.write(x22);
    //Serial.write(Serial3.read());
  }
  if (Serial.available()) {
    int x32 = Serial.read();
    Serial3.write(x32);
    Serial.println(x32);
    // Serial3.write(Serial.read());
  }
}

void publicar(String dados) {
  Serial3.print(dados);
}

void ler_termopares() {

  Serial.println("########################################################");
  Serial.println("                 Leitura dos Termopares                 ");

  // Termopar 1
  Serial.print("S1 (C) = ");
  tk1 = thermocouple1.readCelsius();
  delay(300);
  publicar(String(tk1));
  Serial.println(tk1);
  delay(1000);

  // Termopar 2
  Serial.print("S2 (C) = ");
  tk2 = thermocouple2.readCelsius();
  delay(300);
  publicar(String(tk2));
  Serial.println(tk2);
  delay(1000);

  // Sensor 3
  Serial.print("S3 (C) = ");
  tk3 = thermocouple3.readCelsius();
  delay(300);
  publicar(String(tk3));
  Serial.println(tk3);
  delay(1000);

  // Sensor 4
  Serial.print("S4 (C) = ");
  tk4 = thermocouple4.readCelsius();
  delay(300);
  publicar(String(tk4));
  Serial.println(tk4);
  delay(1000);

  // Sensor 5
  Serial.print("S5 (C) = ");
  tk5 = thermocouple5.readCelsius();
  delay(300);
  publicar(String(tk5));
  Serial.println(tk5);
  delay(1000);

  // Calcula a média
  Serial.print("Temp. Média Termopar = ");
  temp_media_TK = (tk1 + tk2 + tk3 + tk4 + tk5) / 5;
  publicar(String(temp_media_TK));
  Serial.println(temp_media_TK);

  Serial.println("########################################################");
}

void ler_TBS() {

  // TDB - DHT1
  Serial.println("########################################################");

  float t1 = 0;
  float h1 = 0;

  // DHT22 sampling rate is 0.5HZ.
  delay(2000);
  t1 = dht1.readTemperature();
  h1 = dht1.readHumidity();

  // Verifica se os dados dos sensores são válidos
  if (isnan(t1) || isnan(h1)) {

    Serial.println("Erro de leitura do sensor TBS");
    t1 = 0;  // Envia o valor zero para t1
    h1 = 0;  // Envia o valor zero para h1
    publicar(String(t1));
    delay(1500);
    publicar(String(h1));

  } else {

    Serial.println("Leitura do Sensor DHT22 : bulbo seco ");
    Serial.print("TBS (C) = ");
    Serial.println(t1, 1);
    Serial.print("Umidade TBS (%) = ");
    Serial.print(h1, 1);
    Serial.println("########################################################");
    Serial.println("\t");
    publicar(t1);
    delay(1500);
    publicar(h1);
    delay(2000);
  }
}

void ler_TGN() {

  // TGN - DHT2
  Serial.println("########################################################");

  float t2 = 0;
  float h2 = 0;

  // DHT22 sampling rate is 0.5HZ.
  delay(2000);

  t2 = dht2.readTemperature();
  h2 = dht2.readHumidity();

  // Verifica se os dados dos sensores são válidos
  if (isnan(t2) || isnan(h2)) {

    Serial.println("Erro de leitura do sensor TGN");
    t2 = 0;  // Envia o valor zero para t2
    h2 = 0;  // Envia o valor zero para h2
    publicar(t2);
    delay(1500);
    publicar(h2);

  } else {

    Serial.println("Leitura do Sensor DHT22 : globo negro");
    Serial.print("TGN (C) = ");
    Serial.println(t2, 1);
    Serial.print("Umidade TGN (%) = ");
    Serial.print(h2, 1);
    Serial.println("########################################################");
    Serial.println("\t");
    publicar(t2);
    delay(1500);
    publicar(h2);
    delay(2000);
  }
}

void ler_KIMO() {

  // Sensor de vento KIMO
  Serial.println("########################################################");

  // Leitura do KIMO : 1V = 1m/s

  Serial.println("                  Leitura anemômetro KIMO.              ");

  for (int i = 0; i <= 99; i++) {

    KIMO_temp = analogRead(pino_KIMO);
    float KIMO2 = (KIMO_temp / 1024) * 4.97;  // 4.71 = Aref com cabo USB do notebook
    valor_atual = valor_atual + KIMO2;        //valor_atual;
  }

  valorfinal = valor_atual / 100;

  KIMO = valorfinal;  //(valorfinal/1023)*5; // transformar conversão 10 bits para Volts

  Serial.print("KIMO (1V = 1m/s) (m/s) = ");
  Serial.println(KIMO);
  Serial.println(" ");
  Serial.println("########################################################");
  publicar(KIMO);

  valor_atual = 0;
}

// ***** Temperatura do Wind Sensor Modern Device *****
// MB função wind_WS() não revisada (17/04/2023 - MB)

void wind_WS() {
                 // selecionando coluna 0 e linha 0
    // Print a message to the LCD.
   // lcd.display();
   delay(300);

     for (int j=0; j <= 4; j++){
    
    for (int i=0; i <= 99; i++){   //  cria laço de 100 voltas
         
    ///////       read wind   ////////////
     
    int valorA0 = analogRead(vento_WS);

    windMPH =  pow((((float)valorA0 - 264.0) / 85.6814), 3.36814);
    // pow(X,Y) eleva X ((float)valorA0 - 264.0) / 85.6814))por Y (3.36814)
    
   // Here’s the equation derived from the regression for the Rev P wind sensor.
   // WS_MPH = (((Volts – ZeroWind_V) / (3.038517 * (Temp_C ^ 0.115157 ))) / 0.087288 ) ^ 3.009364
    
    //You’ll need to code this up in C. Most people will be using microcontrollers 
    //with ADC’s so you’ll need to input the reference voltage of your microcontroller
    //and the bit depth of the ADC. For example with an Arduino UNO or Mega 
    //running at 5.0 volts, which have a 10-bit ADC, you would substitute 
    //the following expression for the term “Volts”  “(float)analogRead(A0) * 5.0 / 1023.0”

    
    windMPS = (windMPH/2.237); //3.95; // conversão milha/h para m/s
   
    valor_atual = valor_atual + windMPS ; //valor_atual;
    //valor_atual = valor_atual + valor_ant;
    
    //valor_ant = valor_atual;
    //Serial.print("Valor total = "); Serial.println(windMPS);
    delay(2);
    }

    valorfinal= valor_atual / 100;
    valor_atual=0;  
    
  }
    
     valorfinal22 = valorfinal/5;   // faz 100 leituras e depois repete mais 5x = 500 leituras!!!
           
    // wind formula derived from a wind tunnel data, annemometer and some fancy Excel regressions
    // this scalin doesn't have any temperature correction in it yet
   // float windMPH =  pow((((float)windADunits - 264.0) / 8.56814), 3.36814);
    //float windMPS = windMPH/2.237; // conversão milha/h para m/s
    
    Serial.print("Valor Medio = "); Serial.print(valorfinal22); 
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



