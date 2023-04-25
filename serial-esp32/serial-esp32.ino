
#include <WiFi.h>
#include <HTTPClient.h>

// Credenciais da conexão Wifi
const char* ssid = "Conforto-Termico";
const char* password = "master1466";

// Endereço da interface com o banco de dados
const char* serverName = "http://10.0.0.50/confterm-termopar/post-sensores.php";

// Chave para acesso à interface com o banco de dados (API)
String apiKeyValue = "tPmAT5Ab3j7F9";

const int BUFFER_SIZE = 10;
char buf[BUFFER_SIZE];
int count = 0;
String paridade;
String dados[10];
String s;

void setup() {

  delay(2000);
  
  // Inicializa a porta serial 0
  Serial.begin(9600);
  // Inicializa a porta serial 2
  Serial2.begin(9600);

  Serial.println("==================================================");
  Serial.println("Iniciando...");
  Serial.println("==================================================");

  // Inicializa a conexão Wifi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Conectando-se a rede Wifi...");
  }

  // Confirma a conexão Wifi e obtem o endereço IP
  Serial.print("Conectado a rede Wifi ");
  Serial.println(ssid);
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  Serial.println("==================================================");

  // Envia uma mensagem de teste para o Arduino Mega
  delay(1000);
  // Serial2.println("Olá, Mega.");
}

void loop() {
  if (Serial2.available()) {
    
    // Recebe os dados da serial, salva em buf e retorna o seu tamanho
    int rlen = Serial2.readBytes(buf, BUFFER_SIZE);

    // Dado recebido é armazenado em buffer
    s = String(buf);

    Serial2.println(s);
    
    if (s == "99") {
      paridade = s;
    }

    // Limpa o buffer
    memset(buf, 0, sizeof(buf));

    if (paridade == "99") {
      if (rlen > 1 && s != "99") {
        Serial.print("Posição do vetor: ");
        Serial.println(count);
        Serial.print("Tamanho do buffer: ");
        Serial.println(rlen);
        Serial.print("Dado recebido: ");
        Serial.println(s);
        Serial.println("-----");
        dados[count] = s;
        count++;

        if (count >= 10) {
          Serial.println("Enviando leituras para o banco de dados...");
          conexao();
          count = 0;
          paridade = "0";
        }
      }
    }

    // int y22 = Serial2.read();
    // delay(300);
    // Serial.println(y22);
    // Serial.write(y22);
    // Serial2.write(Serial.read());
  }
  if (Serial.available() > 0) {
    int y32 = Serial.read();
    //delay(300);
    Serial.println(y32);
    Serial2.write(y32);
    // Serial2.write(Serial.read());
  }
}

void conexao() {

  // Verifica se a conexao Wifi está ativa
  if (WiFi.status() == WL_CONNECTED) {

    WiFiClient client;
    HTTPClient http;

    // Inicializa a requisição HTTP
    http.begin(client, serverName);

    // Especifica o cabeçalho content-type
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Prepara os parâmetros para a requisção do método POST
    String httpRequestData = "api_key=" + apiKeyValue + "&tk1=" + dados[0] + 
    "&tk2=" + dados[1] + "&tk3=" + dados[2] + "&tk4=" + dados[3] + 
    "&tk5=" + dados[4] + "&media=" + dados[5] + "&t_tdb=" + dados[6] + 
    "&h_tdb=" + dados[7] + "&t_tgn=" + dados[8] + "&kimo=" + dados[9] + "";

    Serial.print("Dados: ");
    Serial.println(httpRequestData);

    // Realiza a requisição do método POST e retorna o resultado em httpResponseCode
    int httpResponseCode = http.POST(httpRequestData);

    // Analisa e informa o status da requisição
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Encerra a requisição HTTP
    http.end();
    Serial.println("");
    Serial.println("==================================================");
    Serial.println("");

  } else {

    Serial.println("Não há conexão Wifi disponível.");
  }
}
