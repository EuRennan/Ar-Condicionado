#include "Display.h"
#include "HT_DEPG0290BxS800FxX_BW.h"
#include "VariaveisGlobais.h"
#include "ComunicacaoModBusIP.h"
#include "imagens.h"


int RssiAtual = 0;
int insuflamentoAtual = 0;
int retornoAtual = 0;
int statusBombaAtual = 0;
int statusFanAtual = 0;
int minutoAtual = 0;

// Inicializa o display
DEPG0290BxS800FxX_BW display(5, 4, 3, 6, 2, 1, -1, 6000000); // rst, dc, cs, busy, sck, mosi, miso, frequência

typedef void (*Demo)(void);

/* Rotação da tela
 * ANGLE_0_DEGREE
 * ANGLE_90_DEGREE
 * ANGLE_180_DEGREE
 * ANGLE_270_DEGREE
 */
#define DIRECTION ANGLE_0_DEGREE
int demoMode = 0;

// Liga a alimentação externa
void VextON(void) {
  pinMode(18, OUTPUT);
  digitalWrite(18, HIGH);
}

// Desliga a alimentação externa (padrão é OFF)
void VextOFF(void) {
  pinMode(18, OUTPUT);
  digitalWrite(18, LOW);
}

void SetupDisplay()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  VextON(); // Liga a alimentação externa
  delay(100);
  
  // A inicialização da UI também inicializa o display.
  display.init();
  display.screenRotate(DIRECTION);
  display.setFont(ArialMT_Plain_10);
}

void AtualizaDisplay()
{
  AtualizaDataHora(dia, mes, ano, hora, minuto);

  display.clear();
  display.setColor(WHITE); // Define a cor de fundo (branco)

  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(display.width(), 0, ((hora < 10) ? "0" + String (hora) : String (hora)) + ":" + ((minuto < 10 )? "0" + String(minuto) : String(minuto)));
  display.drawString(display.width() - 40, 0, ((dia < 10 ) ? "0" + String(dia) : String(dia)) + " / " + ((mes < 10) ? "0" + String(mes) : String(mes)) + " / " + String(ano-100));

  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0, "Conexao: " + String(conexao) + " RSSI: " + String(Rssi));
  display.drawString(0, 10, "Temp de Insuflamento: " + String(insuflamentoRecebido) + "ºC");
  display.drawString(0, 20, "Temp de Retorno: " + String(retornoRecebido) + "ºC");
  display.drawString(0, 30, "BOMBA: " + String(statusBombaRecebido ? "Ligado" : "Desligado"));
  display.drawString(0, 40, "FAN: "   + String(statusFanRecebido   ? "Ligado" : "Desligado"));
  /*Atualiza o display caso a diferença de valores mostrada no display sejam maiores do que 
    a tolerancia, quando o estado do led é alterado ou quando o minuto é atualizado*/

      // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
    // on how to create xbm files
    display.drawXbm(10 ,60, 64, 64, motorLigado);


  if(abs(Rssi - RssiAtual) >= 10 || abs(insuflamentoRecebido - insuflamentoAtual) >= 5 ||
      abs(retornoRecebido - retornoAtual) >= 5 || statusBombaRecebido != statusBombaAtual ||
      statusFanRecebido != statusFanAtual || minuto != minutoAtual )
      {
        display.display();
        RssiAtual         = Rssi;
        insuflamentoAtual = insuflamentoRecebido;
        retornoAtual      = retornoRecebido;
        statusBombaAtual  = statusBombaRecebido;
        statusFanAtual    = statusFanRecebido;
        minutoAtual       = minuto;
      }

  


}













