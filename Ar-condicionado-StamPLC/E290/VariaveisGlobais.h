/*
  Código responsavel pelas variaveis entre os diretorios
*/
#ifndef VARIAVEIS_GLOBAIS_H
#define VARIAVEIS_GLOBAIS_H

#include <Arduino.h>
#include <cstdint>

#define BUTTON_PIN      35      // Pino onde o botão está conectado

// Declaração das variáveis como externas
extern float    insuflamentoRecebido;
extern float    retornoRecebido;
extern bool     statusBombaRecebido;
extern bool     statusFanRecebido;

extern bool     comandoBombaRemoto;
extern bool     comandoFanRemoto;

extern  String  conexao;
extern  int16_t Rssi;

extern int      dia;
extern int      mes;
extern int      ano;
extern int      hora;
extern int      minuto;

#endif
