#include <WiFi.h>
#include <esp_now.h>

// Estrutura recebida do remetente
typedef struct struct_message {
  char texto[20];
  bool valor;
  float insuflamento;
  float retorno;
} struct_message;

struct_message dadosRecebidos;

// Struct para resposta ao remetente
typedef struct resposta_message {
  bool estadoLED;
  char resposta[20];
} resposta_message;

resposta_message resposta;

// Endereço MAC do remetente (substitua pelos valores reais)
uint8_t enderecoRemetente[] = {0x48, 0xCA, 0x43, 0x3E, 0x5E, 0xD8}; // EXEMPLO

void onReceiveData(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&dadosRecebidos, incomingData, sizeof(dadosRecebidos));

  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

  Serial.println("=== Dados recebidos ===");
  Serial.print("De: "); Serial.println(macStr);
  Serial.print("Texto: "); Serial.println(dadosRecebidos.texto);
  Serial.print("Booleano: "); Serial.println(dadosRecebidos.valor ? "true" : "false");
  Serial.print("Insuflamento: "); Serial.println(dadosRecebidos.insuflamento);
  Serial.print("Retorno: "); Serial.println(dadosRecebidos.retorno);

  // Prepara resposta
  resposta.estadoLED = !resposta.estadoLED;
  strcpy(resposta.resposta, "LED atualizado");

  // Envia de volta ao remetente
  esp_now_send(mac, (uint8_t *)&resposta, sizeof(resposta));
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  Serial.print("MAC do Receptor: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("Erro ao iniciar ESP-NOW");
    return;
  }

  // Registra o remetente como peer
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, enderecoRemetente, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  esp_now_add_peer(&peerInfo);

  // Registra callback de recebimento
  esp_now_register_recv_cb(onReceiveData);
}

void loop() {
  // Nada no loop principal
}
