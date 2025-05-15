#include <WiFi.h>
#include <esp_now.h>

// Estrutura esperada
typedef struct struct_message
{
  char texto[20];
  bool valor;
  float insuflamento;
  float retorno;
} struct_message;

struct_message dadosRecebidos;

// NOVA ASSINATURA OBRIGATÓRIA PARA ESP32-S3 E NOVAS VERSÕES
void onReceiveData(const esp_now_recv_info_t* info, const uint8_t* incomingData, int len) {
  memcpy(&dadosRecebidos, incomingData, sizeof(dadosRecebidos));

  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           info->src_addr[0], info->src_addr[1], info->src_addr[2],
           info->src_addr[3], info->src_addr[4], info->src_addr[5]);

  Serial.print(" Dados recebidos de: ");
  Serial.println(macStr);
  Serial.print("Texto recebido: ");
  Serial.println(dadosRecebidos.texto);
  Serial.print("Valor booleano: ");
  Serial.println(dadosRecebidos.valor ? "true" : "false");
  Serial.print("Insuflamento: ");
  Serial.println(dadosRecebidos.insuflamento);
  Serial.print("Retorno: ");
  Serial.println(dadosRecebidos.retorno);
  Serial.println("------------------------------------");
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

  // REGISTRA A CALLBACK COM A NOVA ASSINATURA
  esp_now_register_recv_cb(onReceiveData);
}

void loop() {
  // Nada a fazer no loop
}
