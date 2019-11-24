#include <SPI.h>
#include <Ethernet.h>
 

byte mac[]      = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[]       = { 192, 168, 1, 168 };
byte gateway[]  = { 192, 168, 1, 1 };
byte subnet[]   = { 255, 255, 255, 0 };

EthernetServer server(80); 

int pinArray[54]; 
int count;
int posInterrogacao;
int posIgual;
String pinLed;
String statusLed;
int posEspaco;

void setup() {
  Ethernet.begin(mac, ip, gateway, subnet); // INICIALIZA A CONEXÃO ETHERNET
  
  for (count = 2; count <= 54; count++) {
    pinMode(count, OUTPUT);
  }

  Serial.begin(9600);
}

String readString = String(30);

void toggleLED(int LEDPin) {
  digitalWrite(LEDPin, !digitalRead(LEDPin));
}

void loop(){
  EthernetClient client = server.available(); // CRIA UMA VARIÁVEL CHAMADA client
  
  if (client) { //SE EXISTE CLIENTE
    while (client.connected()) { // ENQUANTO  EXISTIR CLIENTE CONECTADO
    if (client.available()) { // SE EXISTIR CLIENTE HABILITADO
      char caractere = client.read(); // CRIA A VARIÁVEL c
      
      if (readString.length() < 100) // SE O ARRAY FOR MENOR QUE 100
      {
        readString += caractere; // "readstring" VAI RECEBER OS CARACTERES LIDO
      }
      if (caractere == '\n') { // SE ENCONTRAR "\n" É O FINAL DO CABEÇALHO DA REQUISIÇÃO HTTP
        if (readString.indexOf("?") <0) //SE NÃO ENCONTRAR O CARACTER "?"
        {
        }
        else // SENÃO
          posInterrogacao = readString.indexOf("?");
          posIgual = readString.indexOf("=");
          posEspaco = readString.indexOf(" H");
          pinLed = readString.substring(posInterrogacao+1, posIgual);
          statusLed = readString.substring(posIgual+1, posEspaco);
          
          if(statusLed == "true"){ // SE ENCONTRAR O PARÂMETRO "TRUE"
            digitalWrite(pinLed.toInt(), HIGH); // ENERGIZA A PORTA "ledPin" 
            Serial.print("PinLed: ");
            Serial.println(pinLed);            
            //LEDON = true; 
          }
          if(statusLed == "false"){ // SE ENCONTRAR O PARÂMETRO "L=1"
            digitalWrite(pinLed.toInt(), LOW); // ENERGIZA A PORTA "ledPin" 
            //LEDON = true; 
          }

          Serial.print("Recebi: ");
          Serial.println(readString);
          Serial.print("statusLed: ");
          Serial.println(statusLed);
        
          readString="";
          client.println("HTTP/1.1 200 OK"); // ESCREVE PARA O CLIENTE A VERSÃO DO HTTP
          client.println("Content-Type: text/html"); // ESCREVE PARA O CLIENTE O TIPO DE CONTEÚDO(texto/html)
          client.println();
          client.stop(); // FINALIZA A REQUISIÇÃO HTTP
        }else
          client.println("HTTP/1.1 400 Bad Request"); // ESCREVE PARA O CLIENTE A VERSÃO DO HTTP
          client.println("Content-Type: text/html"); // ESCREVE PARA O CLIENTE O TIPO DE CONTEÚDO(texto/html)
          client.println();
      }
    }
  }
}
