// REMETENTE - Envia uma string e um booleano via ESP-NOW

#include <WiFi.h>
#include <esp_now.h>
#include <DHT.h>

#define PINO_INSUFLAMENTO 48
#define PINO_RETORNO      47
#define DHTTYPE DHT11

DHT dhtInsuflamento (PINO_INSUFLAMENTO,DHTTYPE);
DHT dhtRetorno      (PINO_RETORNO,DHTTYPE);

// MAC do ESP32 receptor (StamPLC)
uint8_t enderecoReceptor[] = {0xB0, 0x81, 0x84, 0x96, 0x78, 0xA4};

// Estrutura com mensagem e valor booleano
typedef struct struct_message
{
  char texto[20];
  float insuflamento;
  float retorno;
} struct_message;

struct_message dados;

void setup()
{
  Serial.begin(115200);

  // Coloca o ESP em modo Station (necessário para ESP-NOW)
  WiFi.mode(WIFI_STA);
  Serial.print("MAC do Remetente: ");
  Serial.println(WiFi.macAddress());

  // Inicializa o ESP-NOW
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Erro ao iniciar ESP-NOW");
    return;
  }

  // Registra o receptor como peer
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, enderecoReceptor, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (!esp_now_add_peer(&peerInfo))
  {
    Serial.println("Peer registrado com sucesso");
  }

  // Callback opcional para feedback de envio
  esp_now_register_send_cb([](const uint8_t *mac_addr, esp_now_send_status_t status)
  {
    Serial.print("Status de envio: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Sucesso" : "Falha");
  });
  dhtInsuflamento.begin();
  dhtRetorno.begin();
}

void loop()
{
  leituraDHT();
  strcpy(dados.texto, "Ola ESP");

  // Envia a estrutura
  esp_err_t result = esp_now_send(enderecoReceptor, (uint8_t *)&dados, sizeof(dados));
  if (result == ESP_OK) {
    Serial.print("Mensagem enviada: ");
    Serial.print(dados.texto);
    Serial.print("Insuflamento: ");
    Serial.println(dados.insuflamento);
    Serial.print("Retorno: ");
    Serial.println(dados.retorno);
  } else {
    Serial.println("Erro ao enviar a mensagem");
  }

  delay(5000);
}

void leituraDHT()
{
  dados.insuflamento = dhtInsuflamento.readTemperature();
  dados.retorno      = dhtRetorno.readTemperature();
}
