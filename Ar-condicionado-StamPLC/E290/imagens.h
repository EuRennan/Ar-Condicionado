/*
  Utilizar o LCD image converter para converter as imagens para bitmap
*/

#include <stdint.h>
static const uint8_t motorLigado[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xfc, 0x00, 
    0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 
    0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 
    0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 
    0x01, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 
    0x01, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 
    0x01, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 
    0x01, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 
    0x01, 0xf1, 0xfe, 0x00, 0x00, 0x00, 0x7f, 0x8f, 
    0x01, 0xf1, 0xfe, 0x00, 0x00, 0x00, 0x7f, 0x8f, 
    0x01, 0xf1, 0xfe, 0x00, 0x00, 0x00, 0x7f, 0x8f, 
    0xf1, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 
    0xf1, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 
    0xf1, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 
    0xf1, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 
    0xf1, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 
    0xf1, 0xf1, 0xfe, 0x00, 0x00, 0x00, 0x7f, 0x8f, 
    0xf1, 0xf1, 0xfe, 0x00, 0x00, 0x00, 0x7f, 0x8f, 
    0xf1, 0xf1, 0xfe, 0x00, 0x00, 0x00, 0x7f, 0x8f, 
    0xf1, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 
    0xf1, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 
    0xf1, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 
    0xf1, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 
    0x01, 0xf1, 0xfe, 0x00, 0x00, 0x00, 0x7f, 0x8f, 
    0x01, 0xf1, 0xfe, 0x00, 0x00, 0x00, 0x7f, 0x8f, 
    0x01, 0xf1, 0xfe, 0x00, 0x00, 0x00, 0x7f, 0x8f, 
    0x01, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 
    0x01, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 
    0x01, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 
    0x01, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 
    0x01, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 
    0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 
    0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 
    0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xfe, 0x00, 
    0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xf8, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x1e, 0x00, 
    0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 
    0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 
    0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 
    0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 
    0x00, 0x01, 0xff, 0xf8, 0x00, 0x3f, 0xff, 0x80, 
    0x00, 0x03, 0xff, 0xf0, 0x00, 0x1f, 0xff, 0x80, 
    0x00, 0x03, 0xff, 0xe0, 0x00, 0x0f, 0xff, 0xc0, 
    0x00, 0x03, 0xff, 0xc0, 0x00, 0x07, 0xff, 0xc0, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};