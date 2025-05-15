// REMETENTE - Envia uma string e um booleano via ESP-NOW

#include <WiFi.h>
#include <esp_now.h>

// MAC do ESP32 receptor
uint8_t enderecoReceptor[] = {0x48, 0xCA, 0x43, 0x3D, 0xE3, 0x70};

// Estrutura com mensagem e valor booleano
typedef struct struct_message {
  char texto[20];
  bool valor;
} struct_message;

struct_message dados;

void setup() {
  Serial.begin(115200);

  // Coloca o ESP em modo Station (necessário para ESP-NOW)
  WiFi.mode(WIFI_STA);
  Serial.print("MAC do Remetente: ");
  Serial.println(WiFi.macAddress());

  // Inicializa o ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Erro ao iniciar ESP-NOW");
    return;
  }

  // Registra o receptor como peer
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, enderecoReceptor, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (!esp_now_add_peer(&peerInfo)) {
    Serial.println("Peer registrado com sucesso");
  }

  // Callback opcional para feedback de envio
  esp_now_register_send_cb([](const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("Status de envio: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Sucesso" : "Falha");
  });
}

void loop() {
  // Alterna o valor booleano e define a mensagem
  dados.valor = !dados.valor;
  strcpy(dados.texto, "Ola ESP");

  // Envia a estrutura
  esp_err_t result = esp_now_send(enderecoReceptor, (uint8_t *)&dados, sizeof(dados));
  if (result == ESP_OK) {
    Serial.print("Mensagem enviada: ");
    Serial.print(dados.texto);
    Serial.print(" | Valor: ");
    Serial.println(dados.valor ? "true" : "false");
  } else {
    Serial.println("Erro ao enviar a mensagem");
  }

  delay(5000);
}
