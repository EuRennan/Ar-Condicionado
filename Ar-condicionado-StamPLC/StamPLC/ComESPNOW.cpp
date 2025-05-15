/*
  ESP32 #2 (Intermediário) - Recebe dados do ESP32 #1 (sensor DHT)
  Repassa para o ESP32 #3, que responde com confirmação
*/

#include "Comespnow.h"
#include "VariaveisGlobais.h"
#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

// ======= STRUCTS DE DADOS =======
// Dados recebidos do ESP32 #1
typedef struct {
  char texto[20];
  float insuflamento;
  float retorno;
} struct_entrada;

// Dados enviados para o ESP32 #3
typedef struct {
  char texto[20];
  bool statusBomba;
  bool statusFan;
  float insuflamento;
  float retorno;
} struct_saida;

// Resposta recebida do ESP32 #3
typedef struct {
  char resposta[20];
  bool comandoBombaRemoto;
  bool comandoFanRemoto;
} struct_resposta;

// Instâncias
struct_entrada dadosRecebidos_1;
struct_resposta dadosRecebidos_3;
struct_saida dadosPara3;

// ======= ENDEREÇOS MAC =======
// MAC do ESP32 #1 (com sensor DHT11)
uint8_t macESP1[] = {0x48, 0xCA, 0x43, 0x3E, 0x5E, 0xD8};
// MAC do ESP32 #3 (receptor final)
uint8_t macESP3[] = {0x48, 0xCA, 0x43, 0x3D, 0xE3, 0x70};

// ======= CALLBACK: RECEBIMENTO DE DADOS =======
void onReceiveData(const uint8_t *mac, const uint8_t *incomingData, int len) {
  // Identifica o remetente
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.print("Recebido de: ");
  Serial.println(macStr);

  // Verifica se veio do ESP32 #1
  if (memcmp(mac, macESP1, 6) == 0) {
    memcpy(&dadosRecebidos_1, incomingData, sizeof(dadosRecebidos_1));

    Serial.println("-------------------Dados do ESP32 #1------------------");
    Serial.println(dadosRecebidos_1.texto);
    Serial.print("Temp insuflamento: ");
    Serial.println(dadosRecebidos_1.insuflamento);
    Serial.print("Temp retorno: ");
    Serial.println(dadosRecebidos_1.retorno);
    Serial.println("--------------------------------------------------------");

    // Prepara dados para o ESP32 #3
    memset(&dadosPara3, 0, sizeof(dadosPara3));  // Limpa a struct
    strcpy(dadosPara3.texto, "Ola ESP #3");
    dadosPara3.statusBomba  = statusBomba;
    dadosPara3.statusFan    = statusFan;
    dadosPara3.insuflamento = dadosRecebidos_1.insuflamento;
    dadosPara3.retorno      = dadosRecebidos_1.retorno;

    // Envia para ESP32 #3
    esp_now_send(macESP3, (uint8_t *)&dadosPara3, sizeof(dadosPara3));

  } else if (memcmp(mac, macESP3, 6) == 0) {
    // Se veio do ESP32 #3, trata como resposta
    memcpy(&dadosRecebidos_3, incomingData, sizeof(dadosRecebidos_3));
    Serial.println("------------ Resposta do ESP32 #3----------------");
    comandoBombaRemoto = dadosRecebidos_3.comandoBombaRemoto;
    Serial.print("Comando Bomba Remoto: ");
    Serial.println(dadosRecebidos_3.comandoBombaRemoto ? "Liga" : "Desliga");
    comandoFanRemoto = dadosRecebidos_3.comandoFanRemoto;
    Serial.print("Comando Fan Remoto: ");
    Serial.println(dadosRecebidos_3.comandoFanRemoto ? "Liga" : "Desliga");
  }
}

// ======= CALLBACK: ENVIO DE DADOS =======
void onSend(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           mac_addr[0], mac_addr[1], mac_addr[2],
           mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Enviado para ");
  Serial.print(macStr);
  Serial.print(" => ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Sucesso" : "Falha");
}

// ======= SETUP =======
void setupESPNOW() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);  // Modo estação (necessário para ESP-NOW)

  Serial.print("MAC do ESP32 #2 (intermediário): ");
  Serial.println(WiFi.macAddress());

  // Inicializa ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Erro ao iniciar ESP-NOW");
    return;
  }

  // Registra callbacks
  esp_now_register_recv_cb(onReceiveData);
  esp_now_register_send_cb(onSend);

  // Registra ESP32 #1 como peer
  esp_now_peer_info_t peer1 = {};
  memcpy(peer1.peer_addr, macESP1, 6);
  peer1.channel = 0;
  peer1.encrypt = false;
  esp_now_add_peer(&peer1);

  // Registra ESP32 #3 como peer
  esp_now_peer_info_t peer3 = {};
  memcpy(peer3.peer_addr, macESP3, 6);
  peer3.channel = 0;
  peer3.encrypt = false;
  esp_now_add_peer(&peer3);
}



