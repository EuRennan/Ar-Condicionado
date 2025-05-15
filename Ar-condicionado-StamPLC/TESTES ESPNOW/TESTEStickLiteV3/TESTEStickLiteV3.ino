/*
  Codigo teste em que o ESP #3 recebe os dados do 
  ESP #2 e envia uma resposta
*/
#include <WiFi.h>
#include <esp_now.h>

// Struct que ESP32 #3 vai receber do ESP32 #2
typedef struct {
  char texto[20];
  bool statusBomba;
  bool statusFan;
  float insuflamento;
  float retorno;
} struct_saida;

struct_saida dadosRecebidos;

// Struct de resposta do ESP32 #3 para o ESP32 #2
typedef struct {
  char resposta[20];
  bool comandoBombaRemoto;
  bool comandoFanRemoto;
} struct_resposta;

struct_resposta dadosResposta;

// MAC do ESP32 #2 (StamPLC)
uint8_t macESP2[] = {0xB0, 0x81, 0x84, 0x96, 0x78, 0xA4};  // Substitua se necessário

void onReceiveData(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  memcpy(&dadosRecebidos, incomingData, sizeof(dadosRecebidos));

  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           info->src_addr[0], info->src_addr[1], info->src_addr[2],
           info->src_addr[3], info->src_addr[4], info->src_addr[5]);
  Serial.println("-----------MENSAGEM RECEBIDA------------");
  Serial.print("Recebido de: ");
  Serial.println(macStr);

  Serial.println(dadosRecebidos.texto);
  Serial.println(dadosRecebidos.statusBomba ? "Bomba Ligada" : "Bomba Desligada");
  Serial.println(dadosRecebidos.statusFan ? "Fan Ligada"   : "Fan Desligada");
  Serial.print("Insuflamento: ");
  Serial.println(dadosRecebidos.insuflamento);
  Serial.print("Retorno: ");
  Serial.println(dadosRecebidos.retorno);
  Serial.println("----------------------------------------");

  // Prepara resposta
  strcpy(dadosResposta.resposta, "Recebido #2");
  dadosResposta.comandoBombaRemoto = true;
  dadosResposta.comandoFanRemoto   = false;

  // Envia resposta
  esp_now_send(macESP2, (uint8_t*)&dadosResposta, sizeof(dadosResposta));
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  Serial.print("MAC do ESP32 #3: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("Erro ao iniciar ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(onReceiveData);

  // Registra o ESP32 #2 como peer
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, macESP2, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (!esp_now_add_peer(&peerInfo)) {
    Serial.println("Peer adicionado com sucesso (ESP32 #2)");
  }
}

void loop() {
  // Nada no loop
}
