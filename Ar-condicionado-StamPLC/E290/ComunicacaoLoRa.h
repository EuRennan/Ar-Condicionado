/*
  Código que faz a comunicação LoRa
*/

#ifndef COMUNICACAOLORA_H
#define COMUNICACAOLORA_H

#include <stdint.h>

void SetupLoRa();
void RotinaLoRa();
void OnTxDone(void);
void OnTxTimeout(void);
void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr);

#endif