// VariaveisGlobais.cpp
#include "VariaveisGlobais.h"
#include <Arduino.h>

// Aqui definimos as variáveis globais
float   insuflamentoRecebido  = 0.0;
float   retornoRecebido       = 0.0;
bool    statusBombaRecebido   = 0;
bool    statusFanRecebido     = 0;
bool    comandoBombaRemoto    = 0;
bool    comandoFanRemoto      = 0;
String  conexao               = "off fff";

int16_t Rssi;
int     dia;
int     mes;
int     ano;
int     hora;
int     minuto;
