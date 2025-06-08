#include "ComunicacaoModBusIP.h"

#include <Arduino.h>
#include "time.h"

/*  CONFIGURAÇÃO DO WIFI  */
const char* ssid = "SEU_SSID";
const char* password = "SUA_SENHA";
ModbusIP mb;
/*FIM-- CONFIGURAÇÃO DO WIFI --FIM*/

unsigned int tempoAnterior1 = 0;
unsigned int tempoAnterior2 = 0;

void SetupComunicacaoModBusIP()
{
  // Conectar ao Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    conexao = "Online";
    Serial.println("\nConectado ao Wi-Fi!");
    Serial.print("IP do ESP32: ");
    Serial.println(WiFi.localIP());

    mb.server();

    // Adicionar um registro Modbus (holding register) para armazenar o estado dos dispositivos
    mb.addIsts(REGISTRO_DA_BOMBA);
    mb.addIsts(REGISTRO_DA_FAN);
    mb.addIreg(REGISTRO_DE_INSUFLAMENTO);
    mb.addIreg(REGISTRO_DE_RETORNO);
    mb.addCoil(COMANDO_BOMBA_REMOTO);
    mb.addCoil(COMANDO_FAN_REMOTO);
}

void EnvioDosRegistros()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    conexao = "online";
  }else
    {
      conexao = "offline";
    }

  unsigned int tempoAtual = millis();
   
  /*  Armazena os estados no registro Modbus  */
  mb.Ists(REGISTRO_DA_BOMBA,        statusBombaRecebido );
  mb.Ists(REGISTRO_DA_FAN,          statusFanRecebido   );
  mb.Ireg(REGISTRO_DE_INSUFLAMENTO, insuflamentoRecebido);
  mb.Ireg(REGISTRO_DE_RETORNO,      retornoRecebido     );

  mb.Coil(COMANDO_BOMBA_REMOTO,         comandoBombaRemoto  );
  mb.Coil(COMANDO_FAN_REMOTO,           comandoFanRemoto    );

  mb.task(); //recebe as requisições do modbus

  comandoBombaRemoto = mb.Coil(COMANDO_BOMBA_REMOTO);
  comandoFanRemoto   = mb.Coil(COMANDO_FAN_REMOTO  );

  if(tempoAtual - tempoAnterior2 > 1000)
  {
    Serial.println("COIL ESTADO DA BOMBA: " + String(mb.Ists(REGISTRO_DA_BOMBA)) + " Comando Remoto " + String(comandoBombaRemoto));
    tempoAnterior2 = tempoAtual;
  }
  /*FIM-- Armazena os estados no registro Modbus --FIM*/ 
}

/*CONFIGURAÇÃO HORA E DATA*/
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -3 * 3600;  // Ajuste para GMT-3 (Brasilia)
const int   daylightOffset_sec = 0;     // Horário de verão (0 se não for usado)

void SetupHoraData()
{
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}
void AtualizaDataHora(int &dia, int &mes, int &ano, int &hora, int &minuto)
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Falha ao obter o tempo");
    return;
  }
  dia    = timeinfo.tm_mday;
  mes    = timeinfo.tm_mon + 1;
  ano    = timeinfo.tm_year;
  hora   = timeinfo.tm_hour;
  minuto = timeinfo.tm_min;
}
/*FIM-- CONFIGURAÇÃO HORA E DATA --FIM*/

