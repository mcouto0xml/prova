#include <WiFi.h>
#include <HTTPClient.h>

#define led_verde 2 // Pino utilizado para controle do led verda
#define led_vermelho 1 // Pino utilizado para controle do led vermelho
#define led_amarelo 9 // Pino utilizado para controle do led amarelo

const int botao_pin = 18;  // Pino utilizado para o controle do botão
int estado_botao = 0;  // Variável para leitura do estado do botão

const int ldr_pin = 4;  // Pino utilizado para o controle do LDR
int luminosidade_max=600;

void setup() {

  // Configuração inicial dos pinos para controle dos leds como OUTPUTs (saídas) do ESP32
  pinMode(led_amarelo,OUTPUT);
  pinMode(led_verde,OUTPUT);
  pinMode(led_vermelho,OUTPUT);

  // Inicialização das entradas
  pinMode(botao_pin, INPUT); // Inicializando o botão como entrada

  digitalWrite(led_amarelo, LOW);
  digitalWrite(led_verde, LOW);
  digitalWrite(led_vermelho, LOW);

  Serial.begin(9600); // Configuração para debug por interface serial entre ESP e computador com baud rate de 9600

  WiFi.begin("iPhone", "murilo123"); // Conexão à rede WiFi aberta com SSID Wokwi-GUEST

  while (WiFi.status() == WL_CONNECT_FAILED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("Conectado ao WiFi com sucesso!"); // Considerando que saiu do loop acima, o ESP32 agora está conectado ao WiFi (outra opção é colocar este comando dentro do if abaixo)

  if(WiFi.status() != WL_CONNECT_FAILED){ // Se o ESP32 estiver conectado à Internet
    HTTPClient http;

    String serverPath = "http://www.google.com.br/"; // Endpoint da requisição HTTP

    http.begin(serverPath.c_str());

    int httpResponseCode = http.GET(); // Código do Resultado da Requisição HTTP

    if (httpResponseCode>0) {
      Serial.print("Código de resposta HTTP: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
      }
    else {
      Serial.print("Código de erro: ");
      Serial.println(httpResponseCode);
      }
      http.end();
    }

  else {
    Serial.println("WiFi desconectado :(");
  }
}

void modoNoite() {
  digitalWrite(led_vermelho, LOW);
  digitalWrite(led_amarelo, LOW);
  digitalWrite(led_verde, LOW);
  delay(1000);
  digitalWrite(led_amarelo, HIGH);
}


// Cria a classe Day
class Dia {
  public:
    Dia();
    // Define o método a ser construído
    void semaforoFunciona();
  private:
    unsigned long ultimaMudanca = 0;
    const unsigned long vermelhoTempo = 5000;
    const unsigned long verdeTempo = 3000;
    const unsigned long amareloTempo = 2000;
    char ledAtual = 'r';
};

// Construtor da classe Day
Dia::Dia() {}

// Cria o método semaforoFunciona da classe Day
void Dia::semaforoFunciona() {
  // Realiza o funcionamento do farol de acordo com a regra de negócios e envia o estado e cor atual
  unsigned long tempoAtual = millis();
  if (ledAtual == 'r' && tempoAtual - ultimaMudanca >= vermelhoTempo) {
    ultimaMudanca = tempoAtual;
    ledAtual = 'g';
    digitalWrite(led_vermelho, LOW);
    digitalWrite(led_verde, HIGH);
    
  } else if (ledAtual == 'g' && tempoAtual - ultimaMudanca >= verdeTempo) {
    ultimaMudanca = tempoAtual;
    ledAtual = 'y';
    digitalWrite(led_verde, LOW);
    digitalWrite(led_amarelo, HIGH);

  } else if (ledAtual == 'y' && tempoAtual - ultimaMudanca >= amareloTempo) {
    ultimaMudanca = tempoAtual;
    ledAtual = 'r';
    digitalWrite(led_amarelo, LOW);
    digitalWrite(led_vermelho, HIGH);
    
  }
}

Dia dia;

int val;
int val2;
int buttonState;

void loop() {

  int ldrstatus=analogRead(ldr_pin);

  // Verifica estado do botão
  estado_botao = digitalRead(botao_pin);

  // Leitura contra debounce
  val = digitalRead(botao_pin);      
  delay(10);                        
  val2 = digitalRead(botao_pin);    
  if (val == val2) {                
    if (val != estado_botao) {          
      if (val == HIGH) {
        botao_apertado = HIGH
      }
    }
  }
  // Comparação dos valores lidos pelo LDR
  if(ldrstatus<=luminosidade_max){
    Serial.print("Está escuro, ligando Led");
    Serial.println(ldrstatus);
    modoNoite();


  }else{
    Serial.print("Está claro, ligando Led");
    Serial.println(ldrstatus);
    dia.semaforoFunciona();

    if (digitalRead(led_vermelho) == HIGH) {
      Serial.print("Led Vermelho Ligado");
      if (estado_botao == HIGH){
        Serial.println("Botão pressionado");
        contador = contador + 1;
        delay(1000);
        digitalWrite(led_vermelho, LOW);
        digitalWrite(led_verde, HIGH);
        delay(3000);
        dia.semaforoFunciona();
        
        if (contador == 3) {
          Serial.println("Manda mensagem HTTP");
        }

      }
    }
  }
}