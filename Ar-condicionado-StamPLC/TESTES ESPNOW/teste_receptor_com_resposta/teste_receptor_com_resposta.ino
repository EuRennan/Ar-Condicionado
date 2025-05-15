#include <WiFi.h>
#include <esp_now.h>

// Estrutura esperada
typedef struct struct_message {
  char texto[20];
  bool valor;
  float insuflamento;
  float retorno;
} struct_message;

struct_message dadosRecebidos;

// MAC do próximo destino (ESP32 #3)
uint8_t enderecoProximo[] = {0x48, 0x27, 0xE2, 0xA1, 0xB0, 0x55}; // <== Substitua pelo MAC real do ESP32 #3

// Função de recebimento
void onReceiveData(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&dadosRecebidos, incomingData, sizeof(dadosRecebidos));

  Serial.println("Dados recebidos:");
  Serial.print("Texto: "); Serial.println(dadosRecebidos.texto);
  Serial.print("Valor booleano: "); Serial.println(dadosRecebidos.valor ? "true" : "false");
  Serial.print("Insuflamento: "); Serial.println(dadosRecebidos.insuflamento);
  Serial.print("Retorno: "); Serial.println(dadosRecebidos.retorno);
  Serial.println("------------------------------------");

  // Repassa para o próximo ESP32
  esp_err_t result = esp_now_send(enderecoProximo, (uint8_t *)&dadosRecebidos, sizeof(dadosRecebidos));
  if (result == ESP_OK) {
    Serial.println("Dados encaminhados com sucesso!");
  } else {
    Serial.println("Erro ao encaminhar os dados.");
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  Serial.print("MAC do Intermediário: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("Erro ao iniciar ESP-NOW");
    return;
  }

  // Registrar callback de recebimento
  esp_now_register_recv_cb(onReceiveData);

  // Adiciona peer do próximo dispositivo (ESP32 #3)
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, enderecoProximo, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (!esp_now_add_peer(&peerInfo)) {
    Serial.println("Peer do próximo dispositivo adicionado.");
  } else {
    Serial.println("Erro ao adicionar peer do próximo dispositivo.");
  }
}

void loop() {
  // Nada a fazer no loop
}
