#include "VariaveisGlobais.h"
#include <Arduino.h>

bool    statusBomba        = 0;
bool    comandoBombaLocal  = 0;
bool    comandoBombaRemoto = 0;

bool    statusFan          = 0;
bool    comandoFanLocal    = 0;
bool    comandoFanRemoto   = 0;

int     posicaoDoServo     = 0;
int     comandoServoLocal  = 0;
int     comandoServoRemoto = 0;

float   insuflamento       = 0;
float   retorno            = 0;

bool statusBombaLoRa = 0;
bool statusFanLoRa   = 0;

bool cmdBombaRemoto  = 0;