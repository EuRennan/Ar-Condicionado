#include <M5StamPLC.h>
#include "VariaveisGlobais.h"
#include "ComESPNOW.h"

#define BOMBA 0
#define FAN   1
#define DESLIGA_BOMBA 2
#define DESLIGA_FAN 3

bool cmdBombaRemotoAnterior = 0;
bool cmdFanRemotoAnterior   = 0;

bool ultimoComandoBomba     = 0;
bool ultimoComandoFan       = 0;

unsigned int tempoAnterior  = 0;

void setup()
{
  Serial.begin(115200);
  M5StamPLC.begin();
  setupESPNOW();
}

void loop()
{
  comandoBomba() ? M5StamPLC.writePlcRelay(BOMBA, true) : M5StamPLC.writePlcRelay(BOMBA, false);
  comandoFan  () ? M5StamPLC.writePlcRelay(FAN,   true) : M5StamPLC.writePlcRelay(FAN,   false);

  statusBomba = M5StamPLC.readPlcRelay(BOMBA);
  statusFan   = M5StamPLC.readPlcRelay(FAN);

  unsigned int tempoAtual = millis();
  if(tempoAtual - tempoAnterior > 5000)
  {
    saidaSerial();
    tempoAnterior = tempoAtual;
  }
}


/*bool comandoBomba()
{
  // Leitura dos botões locais
  bool estadoBomba = M5StamPLC.readPlcInput(BOMBA);
  bool estadoDesligaBomba = M5StamPLC.readPlcInput(DESLIGA_BOMBA);

  // Se os botões estão em conflito, prioriza o que estiver pressionado
  if (estadoBomba != estadoDesligaBomba)
  {
    ultimoComandoBomba = estadoBomba;  // true se botão de ligar estiver ativo, false se botão de desligar estiver ativo
  }
  else
  {
    // Nenhum botão está pressionado ou ambos estão no mesmo estado
    // Verifica se houve mudança no comando remoto
    if (cmdBombaRemotoAnterior != comandoBombaRemoto)
    {
      cmdBombaRemotoAnterior = comandoBombaRemoto;
      ultimoComandoBomba = comandoBombaRemoto;
    }
    // Se não houve mudança, mantém o valor anterior de ultimoComandoBomba
  }

  return ultimoComandoBomba;
}*/



/*
bool comandoBomba()
{
  if (M5StamPLC.readPlcInput(BOMBA) != M5StamPLC.readPlcInput(DESLIGA_BOMBA))
  {
    if(M5StamPLC.readPlcInput(BOMBA))
    {
      ultimoComandoBomba = true;
    } else
      {
        ultimoComandoBomba = false;
      }
    delay(50);
  } else if (cmdBombaRemotoAnterior != comandoBombaRemoto)
    {
      if(comandoBombaRemoto)
      {
        cmdBombaRemotoAnterior = comandoBombaRemoto;
        ultimoComandoBomba = true;
      } else
        {
          cmdBombaRemotoAnterior = comandoBombaRemoto;
          ultimoComandoBomba = false;
        }
      delay(50);
    }
  return ultimoComandoBomba;
}*/

bool comandoFan()
{
  bool botaoLigaFan = M5StamPLC.readPlcInput(FAN);
  bool botaoDesligaFan = M5StamPLC.readPlcInput(DESLIGA_FAN);

  if (botaoLigaFan != botaoDesligaFan)
  {
    if(botaoLigaFan)
    {
      ultimoComandoFan = true;
    } else
      {
        ultimoComandoFan = false;
      } 
  } else if (cmdFanRemotoAnterior != comandoFanRemoto)
    {
      if(comandoFanRemoto)
      {
        cmdFanRemotoAnterior = comandoFanRemoto;
        ultimoComandoFan = true;
      } else
        {
          cmdFanRemotoAnterior = comandoFanRemoto;
          ultimoComandoFan = false;
        }
    }
  return ultimoComandoFan;
}

bool comandoBomba()
{
  bool botaoLigaBomba = M5StamPLC.readPlcInput(BOMBA);
  bool botaoDesligaBomba = M5StamPLC.readPlcInput(DESLIGA_BOMBA);

  if (botaoLigaBomba != botaoDesligaBomba)
  {
    if(botaoLigaBomba)
    {
      ultimoComandoBomba = true;
    } else
      {
        ultimoComandoBomba = false;
      } 
  } else if (cmdBombaRemotoAnterior != comandoBombaRemoto)
    {
      if(comandoBombaRemoto)
      {
        cmdBombaRemotoAnterior = comandoBombaRemoto;
        ultimoComandoBomba = true;
      } else
        {
          cmdBombaRemotoAnterior = comandoBombaRemoto;
          ultimoComandoBomba = false;
        }
    }
  return ultimoComandoBomba;
}

void saidaSerial()
{
  Serial.println("----------------------STATUS DA BOMBA-----------------------------");
  Serial.println("COMANDO: " + String(M5StamPLC.readPlcInput(BOMBA) ? "LIGAR" : "DESLIGAR"));
  Serial.println("ESTADO DO RELE: " + String(M5StamPLC.readPlcRelay(BOMBA) ? "LIGADO" : "DESLIGADO"));
  Serial.println("--------------------------------------------------------------------");
  Serial.println("");

  Serial.println("----------------------STATUS DA FAN------------------------------");
  Serial.println("COMANDO: " + String(M5StamPLC.readPlcInput(FAN) ? "LIGAR" : "DESLIGAR"));
  Serial.println("ESTADO DO RELE: " + String(M5StamPLC.readPlcRelay(FAN) ? "LIGADO" : "DESLIGADO"));
  Serial.println("--------------------------------------------------------------------");
  Serial.println("");

  

  
}