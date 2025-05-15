#include <stdint.h>
#include <Arduino.h>
#include <string.h>
#include "ComunicacaoLora.h"
#include "LoRaWan_APP.h"
#include "VariaveisGlobais.h"

byte ciclo = 0;

/* CONFIGURAÇÃO INICIAL LORA */
#define RF_FREQUENCY               915E6  // Frequência LoRa em Hz
#define TX_OUTPUT_POWER            5     // Potência de transmissão em dBm
#define LORA_BANDWIDTH             0      // Largura de banda (0: 125 kHz)
#define LORA_SPREADING_FACTOR      12     // Fator de espalhamento (SF7-SF12)
#define LORA_CODINGRATE            2      // Taxa de codificação (4/5)
#define LORA_PREAMBLE_LENGTH       16     // Comprimento do preâmbulo
#define LORA_SYMBOL_TIMEOUT        0      // Tempo limite para símbolo
#define LORA_FIX_LENGTH_PAYLOAD_ON false  // Comprimento do payload fixo/desativado
#define LORA_IQ_INVERSION_ON       false  // Inversão de IQ desativada

#define RX_TIMEOUT_VALUE 1000  // Tempo limite de recepção em ms
#define BUFFER_SIZE 100        // Tamanho máximo do buffer para mensagens

// Buffers para pacotes de transmissão e recepção
char txpacket[BUFFER_SIZE];
char rxpacket[BUFFER_SIZE];

// Estrutura para eventos do rádio
static RadioEvents_t RadioEvents;

// Protótipos das funções de eventos
void OnTxDone(void);                                                       // Chamado quando a transmissão é concluída
void OnTxTimeout(void);                                                    // Chamado quando a transmissão excede o tempo limite
void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr);  // Evento ao receber

typedef enum {
    LOWPOWER,
    STATE_RX,
    STATE_TX
} States_t;

int16_t Rssi, rxSize;
States_t state;
/*FIM--CONFIGURAÇÃO INICIAL LORA--FIM*/

void SetupLoRa()
{
    Mcu.begin(HELTEC_BOARD, SLOW_CLK_TPYE);  // Configura o microcontrolador

    /*configuração de evento de radio*/
    RadioEvents.TxDone = OnTxDone;        // Define a função para o evento de transmissão concluída
    RadioEvents.TxTimeout = OnTxTimeout;  // Define a função para o evento de timeout de transmissão
    RadioEvents.RxDone = OnRxDone;        // Define a função para o evento de pacote recebido

    Radio.Init(&RadioEvents);        // Inicializa o rádio LoRa com os eventos configurados
    Radio.SetChannel(RF_FREQUENCY);  // Define a frequência de operação do rádio

    /*CONFIGURAÇÃO DO RADIO LORA PARA TRANSMISSAO*/
    Radio.SetTxConfig(MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                      LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                      LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                      true, 0, 0, LORA_IQ_INVERSION_ON, 3000);
    /*FIM--CONFIGURAÇÃO DO RADIO LORA PARA TRANSMISSAO--FIM*/

    /*CONFIGURAÇÃO DO RADIO LORA PARA RECEPÇÃO*/
    Radio.SetRxConfig(MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                      LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                      LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                      0, true, 0, 0, LORA_IQ_INVERSION_ON, true);
    /*FIM--CONFIGURAÇÃO DO RADIO LORA PARA RECEPÇÃO--FIM*/
    state = STATE_TX;
}

void RotinaLoRa()
{
    switch (state)
    {
        case STATE_TX: //MODO TRANSMISSAO

            /*CICLIO DE ENVIO*/
            if (ciclo == 0)
            {
              sprintf(txpacket, "Insuflamento: %.1f C, Retorno: %.1f %%",
                      insuflamento, retorno);
              ciclo++;
            } else if (ciclo == 1)
              {
                sprintf(txpacket, "BOMBA: %d, FAN: %d", statusBombaLoRa, statusFanLoRa);
                ciclo++;
              } else
                {
                  ciclo = 0; //REINICIA O CICLIO
                }
              /*FIM--CICLIO DE ENVIO--FIM*/
            Serial.println("--- ENVIO DE PACOTE ---");
            Serial.printf("Enviando pacote: \"%s\", tamanho %d\r\n", txpacket, strlen(txpacket));
            Serial.println("---------------------------\n");
            Radio.Send((uint8_t *)txpacket, strlen(txpacket)); // ENVIA PACOTE LORA

            state = LOWPOWER;
            break;

        case STATE_RX: //MODO RECEPÇÃO
            Serial.println("Entrando no modo RX\n");
            Radio.Rx(0);
            state = LOWPOWER;
            break;

        case LOWPOWER: //MODO ECONOMIA DE ENERGIA
            Radio.IrqProcess();
            break;

        default:
            break;
    }
}

/*EVENTO CHAMADO AO CONCLUIR TRANSMISSÃO*/
void OnTxDone(void)
{
    Serial.println("Transmissão concluída com sucesso.");
    Radio.Rx(0);
    state = STATE_RX;
}
/*FIM--EVENTO CHAMADO AO CONCLUIR TRANSMISSÃO--FIM*/

/*EVENTO CHAMADO AO RECEBER UM PACOTE*/
void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr)
{
    Rssi = rssi;
    rxSize = size;
    memcpy(rxpacket, payload, size);
    rxpacket[size] = '\0';
    Radio.Sleep();

    Serial.println("--- RECEBIMENTO DE PACOTE ---");
    Serial.printf("Pacote recebido: \"%s\", RSSI: %d\r\n", rxpacket, rssi);
    Serial.println("---------------------------\n");

    if (strstr(rxpacket, "CMD Bomba:") != NULL)
    {
        // Exemplo: CMD Bomba: Ligado, CMD Fan: Desligado
        char bomba[10], fan[10];
        sscanf(rxpacket, "CMD Bomba: %d, CMD Fan: %d", &cmdBombaRemoto, &comandoFanRemoto);
        Serial.printf(">> comandoBombaRemoto = %d, comandoFanRemoto = %d\n", cmdBombaRemoto, comandoFanRemoto);
    }

    state = STATE_TX;
}

/*FIM--EVENTO CHAMADO AO RECEBER UM PACOTE--FIM*/

/*EVENTO CHAMADO EM CASO DE TIMEOUT DE TRANSMISSÃO*/
void OnTxTimeout(void)
{
    Radio.Sleep();
    Serial.println("Timeout de transmissão.");
    state = STATE_TX;
}
/*FIM--EVENTO CHAMADO EM CASO DE TIMEOUT DE TRANSMISSÃO--FIM*/




