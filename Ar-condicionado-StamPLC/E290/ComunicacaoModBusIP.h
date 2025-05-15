#ifndef COMUNICACAOMODBUSIP_H
#define COMUNICACAOMODBUSIP_H

#include <WiFi.h>
#include <ModbusIP_ESP8266.h>  // Biblioteca Modbus TCP
#include "VariaveisGlobais.h"


/* ENDEREÇOS DE REGISTRO MODBUS */
const int REGISTRO_DA_BOMBA         = 100;
const int REGISTRO_DA_FAN           = 101;
const int REGISTRO_DE_INSUFLAMENTO  = 102;
const int REGISTRO_DE_RETORNO       = 103;
const int COMANDO_BOMBA_REMOTO      = 104;
const int COMANDO_FAN_REMOTO        = 105;
/*FIM-- ENDEREÇOS DE REGISTRO MODBUS --FIM*/

/*  CONFIGURAÇÕES DA COMUNICAÇÃO MODBUS  */
void SetupComunicacaoModBusIP();
void EnvioDosRegistros();
/*FIM-- CONFIGURAÇÕES DA COMUNICAÇÃO MODBUS --FIM*/

/*  CONFIGURAÇÃO DE ATUALIZAÇÃO DE DATA E HORA  */
void SetupHoraData();
void AtualizaDataHora(int &dia, int &mes, int &ano, int &hora, int &minuto);
/*FIM-- CONFIGURAÇÃO DE ATUALIZAÇÃO DE DATA E HORA --FIM*/

#endif