/*
  Codigo teste em que o ESP #3 recebe os dados do 
  ESP #2 e envia uma resposta
*/
#include <WiFi.h>
#include <esp_now.h>
#include "ComunicacaoLora.h"
#include "VariaveisGlobais.h"

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

  statusBomba  = dadosRecebidos.statusBomba;
  statusFan    = dadosRecebidos.statusFan;
  insuflamento = dadosRecebidos.insuflamento;
  retorno      = dadosRecebidos.retorno;
  statusBombaLoRa = dadosRecebidos.statusBomba;
  statusFanLoRa   = dadosRecebidos.statusFan;

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
  dadosResposta.comandoBombaRemoto = cmdBombaRemoto;
  dadosResposta.comandoFanRemoto   = comandoFanRemoto;

  // Envia resposta
  esp_now_send(macESP2, (uint8_t*)&dadosResposta, sizeof(dadosResposta));
  Serial.println("------------RESPOSTA ENVIADA------------");
  Serial.println(dadosResposta.resposta);
  Serial.print("Comando Bomba Remoto: ");
  Serial.println(dadosResposta.comandoBombaRemoto);
  Serial.print("Comando Fan Remoto: ");
  Serial.println(dadosResposta.comandoFanRemoto);
  Serial.println("[DEBUG]" + String(cmdBombaRemoto));

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
  SetupLoRa();
}

unsigned int tempoAnterior = 0;

void loop()
{
  RotinaLoRa();
/*
  unsigned int tempoAtual = millis();
  if (tempoAtual - tempoAnterior > 2000)
  {
  Serial.printf("[DEBUG MAIN] statusBomba em LoRa = %d\n", statusBomba);
  Serial.printf("[DEBUG MAIN] statusFan em LoRa = %d\n", statusFan);
  tempoAnterior = tempoAtual;
  }*/
}