/* Exemplo de tela de tinta eletrônica Heltec Automation
 *
 * Função:
 * 1. Demonstração de atualização completa da tela de tinta eletrônica
 *
 * Descrição:
 * 1. Herdado do SSD1306 para desenhar pontos, linhas e funções
 * 
 * Biblioteca: https://github.com/HelTecAutomation/Heltec_ESP32
 * Suporte: support@heltec.cn
 *
 * HelTec AutoMation, Chengdu, China
 * 成都惠利特自动化科技有限公司
 * https://www.heltec.org
 * */
#include "HT_DEPG0290BxS800FxX_BW.h"
#include "images.h"
// Inicializa o display
DEPG0290BxS800FxX_BW   display(5, 4, 3,6, 2,1,-1, 6000000);//rst,dc,cs,busy,sck,mosi,miso,frequência
typedef void (*Demo)(void);
/* Rotação da tela
 * ANGLE_0_DEGREE
 * ANGLE_90_DEGREE
 * ANGLE_180_DEGREE
 * ANGLE_270_DEGREE
 */
#define DIRECTION ANGLE_0_DEGREE
int demoMode = 0;
void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  VextON();
  delay(100);
  // A inicialização da interface de usuário (UI) também inicializa o display.
  display.init();
  display.screenRotate(DIRECTION);
  display.setFont(ArialMT_Plain_10);
}

void drawFontFaceDemo() {
    // Demonstração de fontes
    // Crie mais fontes em http://oleddisplay.squix.ch/
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, "Hello world");
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 10, "Hello world");
    display.setFont(ArialMT_Plain_24);
    display.drawString(0, 26, "Hello world");
}

void drawTextFlowDemo() {
    // Demonstração de fluxo de texto
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawStringMaxWidth(0, 0, 128,
      "Exemplo de tela de tinta eletronica Heltec Automation\nFuncao:\n1. Demonstracao de atualização completa da tela de tinta eletronica\nDescrição:\n1. Herdado do SSD1306 para desenhar pontos, linhas e funções\nBiblioteca: \nSuporte: \nHelTec AutoMation, Chengdu, China\n" );
}

void drawTextAlignmentDemo() {
  // Demonstração de alinhamento de texto
  char str[30];
  int x = 0;
  int y = 0;
  display.setFont(ArialMT_Plain_10);
  // As coordenadas definem o ponto inicial à esquerda do texto
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(x, y, "Left aligned (0,0)");
  // As coordenadas definem o centro do texto
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  x = display.width()/2;
  y = display.height()/2-5;
  sprintf(str,"Center aligned (%d,%d)",x,y);
  display.drawString(x, y, str);
  // As coordenadas definem o final à direita do texto
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  x = display.width();
  y = display.height()-12;
  sprintf(str,"Right aligned (%d,%d)",x,y);
  display.drawString(x, y, str);
}

void drawRectDemo() {
      // Desenha um pixel na posição especificada
    for (int i = 0; i < 10; i++) {
      display.setPixel(i, i);
      display.setPixel(10 - i, i);
    }
    display.drawRect(12, 12, 20, 20);
    // Preenche o retângulo
    display.fillRect(14, 14, 17, 17);
    // Desenha uma linha horizontal
    display.drawHorizontalLine(0, 40, 20);
    // Desenha uma linha vertical
    display.drawVerticalLine(40, 0, 20);
}

void drawCircleDemo() {
  // Demonstração de círculos
  int x = display.width()/4;
  int y = display.height()/2;
  for (int i=1; i < 8; i++) {
    display.setColor(WHITE);
    display.drawCircle(x, y, i*3);
    if (i % 2 == 0) {
      display.setColor(BLACK);
    }
    int x = display.width()/4*3;
    display.fillCircle(x, y, 32 - i* 3);
  }
}

void drawImageDemo() {
    // Consulte http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
    // para saber como criar arquivos XBM
    int x = display.width()/2-WiFi_Logo_width/2;
    int y = display.height()/2-WiFi_Logo_height/2;
    display.drawXbm(x ,y  , WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
}

// Liga o regulador de tensão
void VextON(void)
{
  pinMode(18,OUTPUT);
  digitalWrite(18, HIGH);
}

// Desliga o regulador de tensão (Vext desligado por padrão)
void VextOFF(void)
{
  pinMode(18,OUTPUT);
  digitalWrite(18, LOW);
}

Demo demos[] = {drawFontFaceDemo, drawTextFlowDemo, drawTextAlignmentDemo, drawRectDemo, drawCircleDemo, drawImageDemo};
int demoLength = (sizeof(demos) / sizeof(Demo));
long timeSinceLastModeSwitch = 0;

void loop() {
  // Limpa o display
  display.clear();
  // Executa o método da demonstração atual
  demos[demoMode]();
  display.display();
  demoMode = (demoMode + 1)  % demoLength;
  delay(5000);
}
