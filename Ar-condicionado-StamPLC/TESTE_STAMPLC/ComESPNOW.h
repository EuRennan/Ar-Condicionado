#ifndef COMESPNOW_H
#define COMESPNOW_H

#include <Arduino.h>         // Para uint8_t, String, etc.
#include <esp_now.h>         // Para esp_now_send_status_t e funções ESP-NOW
#include <WiFi.h>            // Necessário para inicializar WiFi antes do ESP-NOW

void onReceiveData(const uint8_t *mac, const uint8_t *incomingData, int len);
void onSend(const uint8_t *mac_addr, esp_now_send_status_t status);
void setupESPNOW();

#endif