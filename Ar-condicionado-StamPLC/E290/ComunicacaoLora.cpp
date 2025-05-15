#include "ComunicacaoLoRa.h"

#include <Arduino.h>
#include "LoRaWan_APP.h"
#include "Display.h"
#include "VariaveisGlobais.h"

/* ==== CONFIGURAÇÃO LoRa ==== */

#define DEBOUNCE_DELAY           50        // Atraso para debounce (ms)
#define RF_FREQUENCY             915E6     // Frequência (Hz)
#define TX_OUTPUT_POWER          5         // Potência de saída (dBm)
#define LORA_BANDWIDTH           0         // [0: 125 kHz]
#define LORA_SPREADING_FACTOR    12        // [SF7..SF12]
#define LORA_CODINGRATE          2         // Taxa de correção [1: 4/5]
#define LORA_PREAMBLE_LENGTH     16        // Preambulo longo para alcance
#define LORA_SYMBOL_TIMEOUT      0
#define LORA_FIX_LENGTH_PAYLOAD_ON false
#define LORA_IQ_INVERSION_ON     false

#define RX_TIMEOUT_VALUE         1000
#define BUFFER_SIZE              100       // Aumentado para comportar mensagens maiores

char txpacket[BUFFER_SIZE];
char rxpacket[BUFFER_SIZE];

static RadioEvents_t RadioEvents;
void OnTxDone(void);
void OnTxTimeout(void);
void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr);

typedef enum {
  LOWPOWER,
  STATE_RX,
  STATE_TX
} States_t;

States_t state;
int16_t txNumber;
int16_t rxSize;
int txCount = 0;
unsigned int tempo_em_rx = 0;

// Variáveis para debounce do botão
uint32_t lastPressTime = 0;
char estadoBotao[15] = "";

/* ==== FIM CONFIGURAÇÃO LoRa ==== */


void SetupLoRa()
{
  // Inicializa MCU e rádio LoRa
  Mcu.begin(HELTEC_BOARD, SLOW_CLK_TPYE);
  txNumber = 0;
  Rssi = 0;

  // Registra funções de callback
  RadioEvents.TxDone = OnTxDone;
  RadioEvents.TxTimeout = OnTxTimeout;
  RadioEvents.RxDone = OnRxDone;

  // Inicializa rádio
  Radio.Init(&RadioEvents);
  Radio.SetChannel(RF_FREQUENCY);
  Radio.SetTxConfig(MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                    LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                    LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                    true, 0, 0, LORA_IQ_INVERSION_ON, 3000);

  Radio.SetRxConfig(MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                    LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                    LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                    0, true, 0, 0, LORA_IQ_INVERSION_ON, true);

  state = STATE_TX;
}


void RotinaLoRa()
{
  // Lógica principal de transmissão e recepção
  switch (state)
  {
    case STATE_TX:
      delay(10);
      txNumber++;
      memset(txpacket, 0, BUFFER_SIZE);  // Limpa o buffer de envio
      sprintf(txpacket, "CMD Bomba: %d, CMD Fan: %d",
        comandoBombaRemoto,
        comandoFanRemoto);
      Serial.printf("\r\nEnviando pacote: \"%s\" (length %d)\r\n", txpacket, strlen(txpacket));
      Radio.Send((uint8_t *)txpacket, strlen(txpacket));
      state = LOWPOWER;
      break;

    case STATE_RX:
      Serial.println("Entrando no modo RX");
      Radio.Rx(0);
      tempo_em_rx = millis();
      state = LOWPOWER;
      break;

    case LOWPOWER:
      Radio.IrqProcess();  // Processa interrupções pendentes
      break;

    default:
      break;
  }
}


/* ===== CALLBACKS LoRa ===== */

void OnTxDone(void)
{
  Serial.println("TX concluído.");
  state = STATE_RX;
}

void OnTxTimeout(void)
{
  Radio.Sleep();
  Serial.println("TX Timeout.");
  state = STATE_TX;
}

void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr)
{
  Rssi = rssi;
  rxSize = size;
  memcpy(rxpacket, payload, size);
  rxpacket[size] = '\0';
  Radio.Sleep();

  Serial.printf("\r\nPacote recebido: \"%s\" com Rssi %d (length %d)\r\n", rxpacket, Rssi, rxSize);

  // Identifica tipo de mensagem
  if (strstr(rxpacket, "Insuflamento:") != NULL)
  {
    // Temperaturas
    sscanf(rxpacket, "Insuflamento: %f C, Retorno: %f C", &insuflamentoRecebido, &retornoRecebido);
    Serial.printf("Dados Recebidos:\n - Insuflamento: %.1f C\n - Retorno: %.1f C\n",
                  insuflamentoRecebido, retornoRecebido);
  }
  else if (strstr(rxpacket, "BOMBA:") != NULL)
  {
    // Estado da bomba e ventilador
    sscanf(rxpacket, "BOMBA: %hhd, FAN: %hhd", &statusBombaRecebido, &statusFanRecebido);
    Serial.printf("Estado da Bomba Recebido: %s, Estado da Fan Recebida: %s\n",
                  statusBombaRecebido ? "Ligado" : "Desligado",
                  statusFanRecebido ? "Ligado" : "Desligado");
  }

  AtualizaDisplay();  // Atualiza o conteúdo visual do display

  state = STATE_TX;
}

/* ===== FIM CALLBACKS LoRa ===== */
