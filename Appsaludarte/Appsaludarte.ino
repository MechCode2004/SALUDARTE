#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Ticker.h>  // Librería para temporizadores no bloqueantes

#define PIN_D8 15  // D8 en NodeMCU es GPIO15

// Definir constantes
#define ANCHO_PANTALLA 128 // Ancho pantalla OLED
#define ALTO_PANTALLA 64   // Alto pantalla OLED
#define PIN_LED_OK 14  // D5 en NodeMCU es GPIO14
// Configuración WiFi
const char* ssid = "Saludarte 2.4";  
const char* password = "Saludarte1234";

// Variable para almacenar la IP anterior y evitar refrescos innecesarios
String ipAnterior = "";

// Objeto de la clase Adafruit_SSD1306
Adafruit_SSD1306 display(ANCHO_PANTALLA, ALTO_PANTALLA, &Wire, -1);

// Servidor Web
WiFiServer server(80);

// DFPlayer Mini
static const uint8_t PIN_MP3_TX = 13;  // D7 en NodeMCU
static const uint8_t PIN_MP3_RX = 12;  // D6 en NodeMCU
SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);
DFRobotDFPlayerMini player;

// Pines de sensores magnéticos
const int SENSOR_D0 = 16; // D0 en NodeMCU
const int SENSOR_D3 = 0;  // D3 en NodeMCU
const int SENSOR_D4 = 2;  // D4 en NodeMCU

int volumen = 25;

// Variables de estado de sensores
bool contandoD0 = false, contandoD3 = false, contandoD4 = false;
bool tiempoCumplidoD0 = false, tiempoCumplidoD3 = false, tiempoCumplidoD4 = false;

// Ticker para manejar temporizadores de los sensores
Ticker temporizadorD0, temporizadorD3, temporizadorD4;

// Callbacks que se ejecutan después de 10 minutos
void activarD0() { 
  Serial.println("Sensor D0 alcanzó los 10 minutos. Reproduciendo Audio 13.");
  tiempoCumplidoD0 = true;  

}

void activarD3() { 
  Serial.println("Sensor D3 alcanzó los 10 minutos. Reproduciendo Audio 14.");
  tiempoCumplidoD3 = true;
}

void activarD4() { 
  Serial.println("Sensor D4 alcanzó los 10 minutos. Reproduciendo Audio 15.");
  tiempoCumplidoD4 = true;
}

void setup() {
  Serial.begin(115200);
  softwareSerial.begin(9600);
  delay(1000);

  pinMode(PIN_D8, OUTPUT);
  digitalWrite(PIN_D8, LOW); // Asegurar que el RELE esté apagado al inicio

  pinMode(SENSOR_D0, INPUT_PULLUP);
  pinMode(SENSOR_D3, INPUT_PULLUP);
  pinMode(SENSOR_D4, INPUT_PULLUP);
  pinMode(PIN_LED_OK, OUTPUT);
  digitalWrite(PIN_LED_OK, LOW); // Apagar el LED inicialmente
  // Iniciar pantalla OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("No se encuentra la pantalla OLED");
    while (true);
  }

  // Limpiar pantalla OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  Serial.println("Conectando a WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConectado a WiFi!");
  Serial.print("IP del ESP8266: ");
  Serial.println(WiFi.localIP());

  // Mostrar IP en pantalla OLED
  actualizarPantalla(WiFi.localIP().toString());

  delay(1000);  // Esperar antes de inicializar DFPlayer Mini
  server.begin();

  // Iniciar DFPlayer Mini
  Serial.println("Iniciando DFPlayer Mini...");

  if (player.begin(softwareSerial)) {
    Serial.println("DFPlayer Mini conectado correctamente.");
    player.volume(25);  // Ajustar volumen (0 a 30)

    // ✅ Enciende el LED porque tanto WiFi como DFPlayer están OK
    digitalWrite(PIN_LED_OK, HIGH);
  } else {
    Serial.println("Error al conectar DFPlayer Mini. Verifica conexiones.");
    digitalWrite(PIN_LED_OK, LOW); // Mantén apagado si falla el DFPlayer
  }

}

void loop() {
  // Actualizar IP en pantalla si cambió
  String ipActual = WiFi.localIP().toString();
  if (ipActual != ipAnterior) {
    actualizarPantalla(ipActual);
    ipAnterior = ipActual;
  }

  // Verificar sensores
 // verificarSensores();

  // Manejo de clientes HTTP
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Cliente conectado.");
    String request = client.readStringUntil('\r');
    Serial.println("Comando recibido: " + request);

    // Enviar una respuesta HTTP válida al cliente
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
    client.println("OK");

    // Encender el LED antes de reproducir

    if (request.indexOf("/AUDIO1") >= 0) reproducirAudio(1);
    else if (request.indexOf("/AUDIO2") >= 0) reproducirAudio(2);
    else if (request.indexOf("/AUDIO3") >= 0) reproducirAudio(3);
    else if (request.indexOf("/AUDIO4") >= 0) reproducirAudio(4);
    else if (request.indexOf("/AUDIO5") >= 0) reproducirAudio(5);
    else if (request.indexOf("/AUDIO6") >= 0) reproducirAudio(6);
    else if (request.indexOf("/AUDIO7") >= 0) reproducirAudio(7);
    else if (request.indexOf("/AUDIO8") >= 0) reproducirAudio(8);
    else if (request.indexOf("/AUDIO9") >= 0) reproducirAudio(9);
    else if (request.indexOf("/AUDIOA") >= 0) reproducirAudio(10);
    else if (request.indexOf("/AUDIOB") >= 0) reproducirAudio(11);
/*

    if request.indexOf("/1") volumen = 1;
    if request.indexOf("/2") ) volumen = 2;
    if request.indexOf("/3") ) volumen = 3;
    if request.indexOf("/4") ) volumen = 4;
    if request.indexOf("/5") ) volumen = 5;
    if request.indexOf("/6") ) volumen = 6;
    if request.indexOf("/7") ) volumen = 7;
    if request.indexOf("/8") ) volumen = 8;
    if request.indexOf("/9") ) volumen = 9;
    if request.indexOf("/10") ) volumen = 10;
   
    if (request.indexOf("/11") ) volumen = 11;
    if (request.indexOf("/12") ) volumen = 12;
    if (request.indexOf("/13") ) volumen = 13;
    if (request.indexOf("/14")) volumen = 14;
    if (request.indexOf("/15") ) volumen = 15;
    if (request.indexOf("/16") ) volumen = 16;
    if (request.indexOf("/17") ) volumen = 17;
    if (request.indexOf("/18") ) volumen = 18;
    if (request.indexOf("/19") ) volumen = 19;
    if (request.indexOf("/20") ) volumen = 20;  

    if (request.indexOf("/21") ) volumen = 21;
    if (request.indexOf("/22") ) volumen = 22;
    if (request.indexOf("/23") ) volumen = 23;
    if (request.indexOf("/24") ) volumen = 24;
    if (request.indexOf("/25") ) volumen = 25;
    if (request.indexOf("/26") ) volumen = 26;
    if (request.indexOf("/27") ) volumen = 27;
    if (request.indexOf("/28") ) volumen = 28;
    if (request.indexOf("/29") ) volumen = 29;
    if (request.indexOf("/30") ) volumen = 30;

*/


if (request.indexOf("/10") != -1) {
  volumen = 10;
}
else if (request.indexOf("/11") != -1) {
  volumen = 11;
}
else if (request.indexOf("/12") != -1) {
  volumen = 12;
}
else if (request.indexOf("/13") != -1) {
  volumen = 13;
}
else if (request.indexOf("/14") != -1) {
  volumen = 14;
}
else if (request.indexOf("/15") != -1) {
  volumen = 15;
}
else if (request.indexOf("/16") != -1) {
  volumen = 16;
}
else if (request.indexOf("/17") != -1) {
  volumen = 17;
}
else if (request.indexOf("/18") != -1) {
  volumen = 18;
}
else if (request.indexOf("/19") != -1) {
  volumen = 19;
}
else if (request.indexOf("/20") != -1) {
  volumen = 20;
}
else if (request.indexOf("/21") != -1) {
  volumen = 21;
}
else if (request.indexOf("/22") != -1) {
  volumen = 22;
}
else if (request.indexOf("/23") != -1) {
  volumen = 23;
}
else if (request.indexOf("/24") != -1) {
  volumen = 24;
}
else if (request.indexOf("/25") != -1) {
  volumen = 25;
}
else if (request.indexOf("/26") != -1) {
  volumen = 26;
}
else if (request.indexOf("/27") != -1) {
  volumen = 27;
}
else if (request.indexOf("/28") != -1) {
  volumen = 28;
}
else if (request.indexOf("/29") != -1) {
  volumen = 29;
}
else if (request.indexOf("/30") != -1) {
  volumen = 30;
}

    client.stop();
    Serial.println("Cliente desconectado.");
  }

  delay(500);
}

// Función para verificar sensores y gestionar Ticker
void verificarSensores() {

  /*
  if (digitalRead(SENSOR_D0) == LOW) {
  if (!contandoD0) {
    contandoD4 = true;
    temporizadorD4.attach(15, activarD0); // 600 segundos = 10 min
    Serial.println("Sensor D4 activado. Contando 10 minutos...");
  }
} else {
  if (contandoD0) {
    contandoD0 = false;
    temporizadorD4.detach(); // Detiene el temporizador si se desactiva el sensor
    Serial.println("Sensor D4 desactivado. Temporizador detenido.");

  }

}

  if (digitalRead(SENSOR_D3) == LOW) {
  if (!contandoD4) {
    contandoD4 = true;
    temporizadorD4.attach(15, activarD4); // 600 segundos = 10 min
    Serial.println("Sensor D4 activado. Contando 10 minutos...");
  }
} else {
  if (contandoD3) {
    contandoD4 = false;
    temporizadorD4.detach(); // Detiene el temporizador si se desactiva el sensor
    Serial.println("Sensor D4 desactivado. Temporizador detenido.");

  }

}*/



/*



if (digitalRead(SENSOR_D4) == LOW) {
  if (!contandoD4) {
    contandoD4 = true;
    temporizadorD4.attach(600, activarD4); // 600 segundos = 10 min
    Serial.println("Sensor D4 activado. Contando 10 minutos...");
  }
} else {
  if (contandoD4) {
    contandoD4 = false;
    temporizadorD4.detach(); // Detiene el temporizador si se desactiva el sensor
    Serial.println("Sensor D4 desactivado. Temporizador detenido.");
  }
}
  if (tiempoCumplidoD0) {
    reproducirAudio(13);
    tiempoCumplidoD0 = false;
    ESP.restart();
  }

  if (tiempoCumplidoD3) {
    reproducirAudio(14);
    tiempoCumplidoD3 = false;
    ESP.restart();
  }

  if (tiempoCumplidoD4) {
    reproducirAudio(15);
    tiempoCumplidoD3 = false;
    ESP.restart();
  }
  */
}

void actualizarPantalla(String ip) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(10, 20);
  display.println("CONECTADO A LA RED");

  display.setCursor(0, 40);
  display.print("IP: ");
  display.println(ip);

  display.display();
}

void reproducirAudio(int numero) {

  player.volume(volumen); 

  Serial.println(volumen);
  delay(200);

    digitalWrite(PIN_D8, HIGH);
    delay(500); 


  digitalWrite(PIN_D8, HIGH);
  Serial.printf("Reproduciendo Audio %d\n", numero);
  delay(100);
  player.play(numero);
  delay(9000);
  digitalWrite(PIN_D8, LOW); 

}