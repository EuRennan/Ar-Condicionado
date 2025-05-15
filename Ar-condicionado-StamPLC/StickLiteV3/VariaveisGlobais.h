#ifndef VARIAVEIS_GLOBAIS_H
#define VARIAVEIS_GLOBAIS_H

#include <Arduino.h>
#include <cstdint>

#define INPUT_SERVO
#define INPUT_BOMBA
#define INPUT_FAN  

#define OUTPUT_SERVO
#define OUTPUT_BOMBA
#define OUTPUT_FAN  

extern bool    statusBomba;
extern bool    comandoBombaLocal;
extern bool    comandoBombaRemoto;

extern bool    statusFan;
extern bool    comandoFanLocal;
extern bool    comandoFanRemoto;

extern int     posicaoDoServo;
extern int     comandoServoLocal;
extern int     comandoServoRemoto;

extern float   insuflamento;
extern float   retorno;

extern bool statusBombaLoRa;
extern bool statusFanLoRa;

extern bool cmdBombaRemoto;

#endif