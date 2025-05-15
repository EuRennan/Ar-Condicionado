#include <WiFi.h>
#include <esp_now.h>

// MAC do próximo ESP32 na cadeia
uint8_t enderecoProximoESP[] = {0x48, 0xCA, 0x43, 0x3E, 0x5E, 0xD8}; // Exemplo

// Estrutura compartilhada entre ESPs
typedef struct struct_message {
  char texto[20];
  bool valor;
} struct_message;

struct_message dadosRecebidos;

// Função chamada ao receber dados
void onReceiveData(const esp_now_recv_info_t* info, const uint8_t* incomingData, int len) {
  memcpy(&dadosRecebidos, incomingData, sizeof(dadosRecebidos));

  Serial.println("Dados recebidos:");
  Serial.print("Texto: ");
  Serial.println(dadosRecebidos.texto);
  Serial.print("Valor: ");
  Serial.println(dadosRecebidos.valor ? "true" : "false");

  // Opcional: modificar mensagem antes de reenviar
  strcpy(dadosRecebidos.texto, "Encaminhado");

  // Reenviar para outro ESP
  esp_now_send(enderecoProximoESP, (uint8_t*)&dadosRecebidos, sizeof(dadosRecebidos));
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  Serial.println("MAC atual: " + WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("Erro ao iniciar ESP-NOW");
    return;
  }

  // Registra função de recepção
  esp_now_register_recv_cb(onReceiveData);

  // Adiciona o próximo ESP como peer
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, enderecoProximoESP, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (!esp_now_add_peer(&peerInfo)) {
    Serial.println("Peer adicionado com sucesso");
  }
}

void loop() {
  // Nada no loop
}
