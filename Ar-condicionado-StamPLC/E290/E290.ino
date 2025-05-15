/*
  obs: Caso a distancia seja insuficiente, mudar o codigo deixando apenas os arquivos 
  Display.h, ComunicacaoLoRa.h e jogar o codigo no arquivo .cpp pra eles (houve uma oscilação do sinal apos
  incluir os arquivos .cpp do display e comunicacao LoRa)
*/

#include "ComunicacaoLoRa.h"
#include "Display.h"
#include "VariaveisGlobais.h"
#include "ComunicacaoModBusIP.h"

void setup()
{
  Serial.begin(115200); // Inicia comunicação serial para debug
  pinMode(BUTTON_PIN, INPUT); // Configuração do Botão
  SetupDisplay();
  SetupLoRa(); // Setup do Lora
  SetupComunicacaoModBusIP();
  SetupHoraData();
}

void loop()
{
  RotinaLoRa(); //Codigo que faz a comunicação via LoRa
  EnvioDosRegistros(); //Codigo que envia os registros para o SCADA
}

