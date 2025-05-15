#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>

const char* ssid = "Katia";
const char* password = "14122008@Rm";

WebServer server(80);

void handleDHT() {
  String temp = server.arg("temp");
  String hum  = server.arg("hum");
  
  Serial.println("Dados recebidos:");
  Serial.println("Temperatura: " + temp + " ºC");
  Serial.println("Umidade: " + hum + " %");
  
  server.send(200, "text/plain", "Dados recebidos com sucesso");
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nConectado!");
  Serial.print("Endereço IP: "); Serial.println(WiFi.localIP());

  server.on("/dht", handleDHT);
  server.begin();
  Serial.println("Servidor iniciado");
}

void loop() {
  server.handleClient();
}
