// arial_normal_rev.c
// Font type    : Full (95 characters)
// Font size    : 16x16 pixels
// Memory usage : 3044 bytes
// Submitted by : DMOSS

#if defined(__AVR__)
#include <avr/pgmspace.h>
#define fontdatatype const uint8_t
#elif defined(__PIC32MX__)
#define PROGMEM
#define fontdatatype const unsigned char
#elif defined(__arm__)
#define PROGMEM
#define fontdatatype const unsigned char
#endif

fontdatatype arial_small_rev[3044] PROGMEM={
0x10,0x10,0x20,0x5F,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,// ' '
0xFF,0xFF,0xFF,0xFF,0xFD,0xFF,0xFD,0xFF,0xFD,0xFF,0xFD,0xFF,0xFD,0xFF,0xFD,0xFF,0xFD,0xFF,0xFD,0xFF,0xFD,0xFF,0xFD,0xFF,0xFF,0xFF,0xFD,0xFF,0xFF,0xFF,0xFF,0xFF,// '!'
0xFF,0xFF,0xFF,0xFF,0xFD,0xBF,0xFD,0xBF,0xFD,0xBF,0xFD,0xBF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,// '"'
0xFF,0xFF,0xFF,0xFF,0xFD,0xBF,0xFD,0xBF,0xFB,0x7F,0xFB,0x7F,0xC0,0x1F,0xFB,0x7F,0xF6,0xFF,0xF6,0xFF,0xC0,0x1F,0xF6,0xFF,0xED,0xFF,0xED,0xFF,0xFF,0xFF,0xFF,0xFF,// '#'
0xFF,0xFF,0xFE,0xFF,0xFC,0x3F,0xFA,0xDF,0xF6,0xDF,0xF6,0xFF,0xF6,0xFF,0xF8,0xFF,0xFE,0x3F,0xFE,0xDF,0xFE,0xDF,0xF6,0xDF,0xFA,0xBF,0xFC,0x7F,0xFE,0xFF,0xFF,0xFF,// '$'
0xFF,0xFF,0xFF,0xFF,0xE3,0xEF,0xDD,0xDF,0xDD,0xDF,0xDD,0xBF,0xDD,0xBF,0xDD,0x63,0xE3,0x5D,0xFE,0xDD,0xFD,0xDD,0xFD,0xDD,0xFB,0xDD,0xFB,0xE3,0xFF,0xFF,0xFF,0xFF,// '%'
0xFF,0xFF,0xFF,0xFF,0xFE,0x3F,0xFD,0xDF,0xFD,0xDF,0xFD,0xDF,0xFE,0xBF,0xFC,0x7F,0xFB,0x7F,0xF7,0xB7,0xF7,0xAF,0xF7,0xDF,0xFB,0xCF,0xFC,0x37,0xFF,0xFF,0xFF,0xFF,// '&'
0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,// '''
0xFF,0xFF,0xFE,0xFF,0xFD,0xFF,0xFD,0xFF,0xFB,0xFF,0xFB,0xFF,0xFB,0xFF,0xFB,0xFF,0xFB,0xFF,0xFB,0xFF,0xFB,0xFF,0xFD,0xFF,0xFD,0xFF,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,// '('
0xFF,0xFF,0xFD,0xFF,0xFE,0xFF,0xFE,0xFF,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFE,0xFF,0xFE,0xFF,0xFD,0xFF,0xFF,0xFF,0xFF,0xFF,// ')'
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0x7F,0xFE,0x7F,0xF6,0x6F,0xE0,0x07,0xFC,0x1F,0xFC,0x3F,0xF9,0x9F,0xF9,0xDF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,// '*'
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xF8,0x0F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,// '+'
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0xFF,0xFF,0xFF,// ','
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF8,0x3F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,// '-'
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,// '.'
0xFF,0xFF,0xFF,0xFF,0xFF,0xDF,0xFF,0xBF,0xFF,0xBF,0xFF,0xBF,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xFD,0xFF,0xFD,0xFF,0xFF,0xFF,0xFF,0xFF,// '/'
0xFF,0xFF,0xFF,0xFF,0xFC,0x5F,0xFB,0xBF,0xF7,0x9F,0xF7,0x5F,0xF7,0x5F,0xF6,0xDF,0xF6,0xDF,0xF6,0xDF,0xF6,0xDF,0xF3,0xDF,0xFB,0xBF,0xF4,0x7F,0xFF,0xFF,0xFF,0xFF,// '0'
0xFF,0xFF,0xFF,0xFF,0xFF,0xBF,0xFF,0x3F,0xFE,0xBF,0xFD,0xBF,0xFF,0xBF,0xFF,0xBF,0xFF,0xBF,0xFF,0xBF,0xFF,0xBF,0xFF,0xBF,0xFF,0xBF,0xFC,0x07,0xFF,0xFF,0xFF,0xFF,// '1'
0xFF,0xFF,0xFF,0xFF,0xFC,0x3F,0xFB,0xBF,0xF7,0xDF,0xFF,0xDF,0xFF,0xDF,0xFF,0xBF,0xFF,0xBF,0xFF,0x7F,0xFE,0xFF,0xFD,0xFF,0xFB,0xFF,0xF0,0x1F,0xFF,0xFF,0xFF,0xFF,// '2'
0xFF,0xFF,0xFF,0xFF,0xFC,0x7F,0xFB,0xBF,0xF7,0xBF,0xFF,0xBF,0xFF,0x7F,0xFC,0x7F,0xFF,0xBF,0xFF,0xDF,0xFF,0xDF,0xF7,0xDF,0xF3,0xBF,0xFC,0x7F,0xFF,0xFF,0xFF,0xFF,// '3'
0xFF,0xFF,0xFF,0xFF,0xFF,0xBF,0xFF,0x3F,0xFE,0xBF,0xFE,0xBF,0xFD,0xBF,0xFD,0xBF,0xFB,0xBF,0xF7,0xBF,0xF0,0x0F,0xFF,0xBF,0xFF,0xBF,0xFF,0xBF,0xFF,0xFF,0xFF,0xFF,// '4'
0xFF,0xFF,0xFF,0xFF,0xFE,0x07,0xFE,0xFF,0xFE,0xFF,0xFD,0xFF,0xFC,0x1F,0xFD,0xEF,0xFF,0xF7,0xFF,0xF7,0xFF,0xF7,0xFD,0xF7,0xFE,0xEF,0xFF,0x1F,0xFF,0xFF,0xFF,0xFF,// '5'
0xFF,0xFF,0xFF,0xFF,0xFC,0x7F,0xFB,0x9F,0xF7,0xDF,0xF7,0xFF,0xF4,0x7F,0xF3,0xBF,0xF7,0xDF,0xF7,0xDF,0xF7,0xDF,0xF7,0xDF,0xFB,0xBF,0xFC,0x7F,0xFF,0xFF,0xFF,0xFF,// '6'
0xFF,0xFF,0xFF,0xFF,0xFC,0x07,0xFF,0xEF,0xFF,0xEF,0xFF,0xDF,0xFF,0xDF,0xFF,0xBF,0xFF,0xBF,0xFF,0xBF,0xFF,0xBF,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0xFF,0xFF,0xFF,// '7'
0xFF,0xFF,0xFF,0xFF,0xFF,0x1F,0xFE,0xEF,0xFD,0xF7,0xFD,0xF7,0xFE,0xEF,0xFF,0x1F,0xFE,0xEF,0xFD,0xF7,0xFD,0xF7,0xFD,0xF7,0xFE,0xEF,0xFF,0x1F,0xFF,0xFF,0xFF,0xFF,// '8'
0xFF,0xFF,0xFF,0xFF,0xFC,0x7F,0xFB,0xBF,0xF7,0xDF,0xF7,0xDF,0xF7,0xDF,0xF7,0xDF,0xFB,0x9F,0xFC,0x5F,0xFF,0xDF,0xF7,0xDF,0xFB,0xBF,0xF8,0x7F,0xFF,0xFF,0xFF,0xFF,// '9'
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,// ':'
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0xFF,0xFF,0xFF,// ';'
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xEF,0xFF,0x9F,0xFE,0x7F,0xF9,0xFF,0xF7,0xFF,0xF9,0xFF,0xFE,0x7F,0xFF,0x9F,0xFF,0xEF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,// '<'
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,// '='
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFB,0xFF,0xFC,0xFF,0xFF,0x3F,0xFF,0xCF,0xFF,0xF7,0xFF,0xCF,0xFF,0x3F,0xFC,0xFF,0xFB,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,// '>'
0xFF,0xFF,0xFF,0xFF,0xFC,0x7F,0xFB,0xBF,0xF7,0xDF,0xF7,0xDF,0xFF,0xDF,0xFF,0xBF,0xFF,0x7F,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xFF,0xFF,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,// '?'
0xFF,0xFF,0xFC,0x3F,0xF3,0xCF,0xEF,0xEF,0xEC,0xB7,0xDB,0x37,0xD7,0x77,0xD7,0x77,0xD7,0x6F,0xD8,0x1F,0xEF,0xF7,0xF7,0xCF,0xF8,0x3F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,// '@'
0xFF,0xFF,0xFF,0xFF,0xFE,0xFF,0xFD,0x7F,0xFD,0x7F,0xFB,0xBF,0xFB,0xBF,0xFB,0xBF,0xF7,0xDF,0xF0,0x1F,0xEF,0xEF,0xEF,0xEF,0xDF,0xF7,0xDF,0xF7,0xFF,0xFF,0xFF,0xFF,// 'A'
0xFF,0xFF,0xFF,0xFF,0xF0,0x1F,0xF7,0xEF,0xF7,0xF7,0xF7,0xF7,0xF7,0xEF,0xF0,0x0F,0xF7,0xEF,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xEF,0xF0,0x1F,0xFF,0xFF,0xFF,0xFF,// 'B'
0xFF,0xFF,0xFF,0xFF,0xFC,0x1F,0xFB,0xEF,0xF7,0xF7,0xEF,0xFF,0xEF,0xFF,0xEF,0xFF,0xEF,0xFF,0xEF,0xFF,0xEF,0xFF,0xF7,0xF7,0xFB,0xEF,0xFC,0x1F,0xFF,0xFF,0xFF,0xFF,// 'C'
0xFF,0xFF,0xFF,0xFF,0xE0,0x3F,0xEF,0xDF,0xEF,0xEF,0xEF,0xF7,0xEF,0xF7,0xEF,0xF7,0xEF,0xF7,0xEF,0xF7,0xEF,0xF7,0xEF,0xEF,0xEF,0xDF,0xE0,0x3F,0xFF,0xFF,0xFF,0xFF,// 'D'
0xFF,0xFF,0xFF,0xFF,0xF0,0x07,0xF7,0xFF,0xF7,0xFF,0xF7,0xFF,0xF7,0xFF,0xF0,0x0F,0xF7,0xFF,0xF7,0xFF,0xF7,0xFF,0xF7,0xFF,0xF7,0xFF,0xF0,0x07,0xFF,0xFF,0xFF,0xFF,// 'E'
0xFF,0xFF,0xFF,0xFF,0xF0,0x0F,0xF7,0xFF,0xF7,0xFF,0xF7,0xFF,0xF7,0xFF,0xF0,0x1F,0xF7,0xFF,0xF7,0xFF,0xF7,0xFF,0xF7,0xFF,0xF7,0xFF,0xF7,0xFF,0xFF,0xFF,0xFF,0xFF,// 'F'
0xFF,0xFF,0xFF,0xFF,0xFE,0x1F,0xFD,0xEF,0xFB,0xF7,0xF7,0xFF,0xF7,0xFF,0xF7,0xFF,0xF7,0x83,0xF7,0xFB,0xF7,0xFB,0xFB,0xF7,0xFD,0xEF,0xFE,0x1F,0xFF,0xFF,0xFF,0xFF,// 'G'
0xFF,0xFF,0xFF,0xFF,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF0,0x07,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xFF,0xFF,0xFF,0xFF,// 'H'
0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0xFF,0xFF,0xFF,// 'I'
0xFF,0xFF,0xFF,0xFF,0xFF,0xDF,0xFF,0xDF,0xFF,0xDF,0xFF,0xDF,0xFF,0xDF,0xFF,0xDF,0xFF,0xDF,0xFF,0xDF,0xF7,0xDF,0xF7,0xDF,0xFB,0xBF,0xF8,0x7F,0xFF,0xFF,0xFF,0xFF,// 'J'
0xFF,0xFF,0xFF,0xFF,0xF7,0xF7,0xF7,0xEF,0xF7,0xDF,0xF7,0xBF,0xF7,0x7F,0xF6,0x7F,0xF5,0xBF,0xF3,0xDF,0xF7,0xDF,0xF7,0xEF,0xF7,0xEF,0xF7,0xF7,0xFF,0xFF,0xFF,0xFF,// 'K'
0xFF,0xFF,0xFF,0xFF,0xF7,0xFF,0xF7,0xFF,0xF7,0xFF,0xF7,0xFF,0xF7,0xFF,0xF7,0xFF,0xF7,0xFF,0xF7,0xFF,0xF7,0xFF,0xF7,0xFF,0xF7,0xFF,0xF0,0x1F,0xFF,0xFF,0xFF,0xFF,// 'L'
0xFF,0xFF,0xFF,0xFF,0xDF,0xF7,0xCF,0xE7,0xCF,0xE7,0xD7,0xD7,0xD7,0xD7,0xDB,0xB7,0xDB,0xB7,0xDB,0x77,0xDD,0x77,0xDD,0x77,0xDE,0xF7,0xDE,0xF7,0xFF,0xFF,0xFF,0xFF,// 'M'
0xFF,0xFF,0xFF,0xFF,0xF7,0xF7,0xF3,0xF7,0xF5,0xF7,0xF5,0xF7,0xF6,0xF7,0xF7,0x77,0xF7,0x77,0xF7,0xB7,0xF7,0xD7,0xF7,0xD7,0xF7,0xE7,0xF7,0xF7,0xFF,0xFF,0xFF,0xFF,// 'N'
0xFF,0xFF,0xFF,0xFF,0xFC,0x3F,0xFB,0xDF,0xF7,0xEF,0xEF,0xF7,0xEF,0xF7,0xEF,0xF7,0xEF,0xF7,0xEF,0xF7,0xEF,0xF7,0xF7,0xEF,0xFB,0xDF,0xFC,0x3F,0xFF,0xFF,0xFF,0xFF,// 'O'
0xFF,0xFF,0xFF,0xFF,0xF0,0x1F,0xF7,0xEF,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xEF,0xF0,0x1F,0xF7,0xFF,0xF7,0xFF,0xF7,0xFF,0xF7,0xFF,0xF7,0xFF,0xFF,0xFF,0xFF,0xFF,// 'P'
0xFF,0xFF,0xFF,0xFF,0xFC,0x3F,0xFB,0xDF,0xF7,0xEF,0xEF,0xF7,0xEF,0xF7,0xEF,0xF7,0xEF,0xF7,0xEF,0xF7,0xEF,0xF7,0xF7,0x27,0xFB,0xCF,0xFC,0x27,0xFF,0xFF,0xFF,0xFF,// 'Q'
0xFF,0xFF,0xFF,0xFF,0xF0,0x1F,0xF7,0xEF,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xEF,0xF0,0x1F,0xF7,0xBF,0xF7,0xDF,0xF7,0xDF,0xF7,0xEF,0xF7,0xF7,0xFF,0xFF,0xFF,0xFF,// 'R'
0xFF,0xFF,0xFF,0xFF,0xFC,0x1F,0xFB,0xEF,0xF7,0xF7,0xF7,0xFF,0xFB,0xFF,0xFC,0x7F,0xFF,0x8F,0xFF,0xF7,0xF7,0xF7,0xF7,0xF7,0xFB,0xEF,0xFC,0x1F,0xFF,0xFF,0xFF,0xFF,// 'S'
0xFF,0xFF,0xFF,0xFF,0xF0,0x07,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0xFF,0xFF,0xFF,// 'T'
0xFF,0xFF,0xFF,0xFF,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xFB,0xEF,0xFC,0x1F,0xFF,0xFF,0xFF,0xFF,// 'U'
0xFF,0xFF,0xFF,0xFF,0xF7,0xF7,0xF7,0xF7,0xFB,0xEF,0xFB,0xEF,0xFB,0xEF,0xFD,0xDF,0xFD,0xDF,0xFD,0xDF,0xFE,0xBF,0xFE,0xBF,0xFF,0x7F,0xFF,0x7F,0xFF,0xFF,0xFF,0xFF,// 'V'
0xFF,0xFF,0xFF,0xFF,0xDF,0x7D,0xDE,0x7D,0xEE,0xBB,0xEE,0xBB,0xEE,0xBB,0xEE,0xBB,0xF5,0xD7,0xF5,0xD7,0xF5,0xD7,0xF5,0xD7,0xFB,0xEF,0xFB,0xEF,0xFF,0xFF,0xFF,0xFF,// 'W'
0xFF,0xFF,0xFF,0xFF,0xEF,0xEF,0xF7,0xDF,0xFB,0xBF,0xFB,0xBF,0xFD,0x7F,0xFE,0xFF,0xFD,0x7F,0xFB,0xBF,0xFB,0xBF,0xF7,0xDF,0xEF,0xEF,0xDF,0xF7,0xFF,0xFF,0xFF,0xFF,// 'X'
0xFF,0xFF,0xFF,0xFF,0xF7,0xF7,0xFB,0xEF,0xFB,0xEF,0xFD,0xDF,0xFE,0xBF,0xFE,0xBF,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0xFF,0xFF,0xFF,// 'Y'
0xFF,0xFF,0xFF,0xFF,0xF8,0x07,0xFF,0xEF,0xFF,0xDF,0xFF,0xDF,0xFF,0xBF,0xFF,0x7F,0xFF,0x7F,0xFE,0xFF,0xFD,0xFF,0xFD,0xFF,0xFB,0xFF,0xF0,0x07,0xFF,0xFF,0xFF,0xFF,// 'Z'
0xFF,0xFF,0xFC,0x7F,0xFD,0xFF,0xFD,0xFF,0xFD,0xFF,0xFD,0xFF,0xFD,0xFF,0xFD,0xFF,0xFD,0xFF,0xFD,0xFF,0xFD,0xFF,0xFD,0xFF,0xFD,0xFF,0xFD,0xFF,0xFC,0x7F,0xFF,0xFF,// '['
0xFF,0xFF,0xFF,0xFF,0xFD,0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0xBF,0xFF,0xBF,0xFF,0xBF,0xFF,0xDF,0xFF,0xDF,0xFF,0xFF,0xFF,0xFF,// '\'
0xFF,0xFF,0xFC,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFC,0x7F,0xFF,0xFF,// ']'
0xFF,0xFF,0xFE,0xFF,0xFD,0x7F,0xFD,0x7F,0xFB,0xBF,0xFB,0xBF,0xFB,0xBF,0xF7,0xDF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,// '^'
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0xFF,0xFF,// '_'
0xFF,0xFF,0xFF,0xFF,0xFE,0xFF,0xFF,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,// '`'
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFD,0xFF,0xFB,0xFF,0xF7,0xFF,0xE0,0x1F,0xE0,0x1F,0xF7,0xFF,0xFB,0xFF,0xFD,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,// 'a' sub for Left Arrow
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xBF,0xFF,0xDF,0xFF,0xEF,0xF8,0x07,0xF8,0x07,0xFF,0xEF,0xFF,0xDF,0xFF,0xBF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,// 'b' sub for Right Arrow
0xFF,0xFF,0xCF,0xFF,0xB7,0xFF,0xB7,0xFF,0xCF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,// 'c' sub for Degree symbol
0xFF,0xFF,0xFF,0xDF,0xFF,0xDF,0xFF,0xDF,0xFC,0x5F,0xFB,0x9F,0xF7,0xDF,0xF7,0xDF,0xF7,0xDF,0xF7,0xDF,0xF7,0xDF,0xFB,0x9F,0xFC,0x5F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,// 'd' sub for Box symbol
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0x7F,0xFB,0xBF,0xF7,0xDF,0xF7,0xDF,0xF0,0x1F,0xF7,0xFF,0xF7,0xDF,0xFB,0xBF,0xFC,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,// 'e'
0xFF,0xFF,0xFF,0x3F,0xFE,0xFF,0xFE,0xFF,0xFC,0x3F,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,// 'f'
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0x5F,0xFB,0x9F,0xF7,0xDF,0xF7,0xDF,0xF7,0xDF,0xF7,0xDF,0xF7,0xDF,0xFB,0x9F,0xFC,0x5F,0xFF,0xDF,0xF7,0xBF,0xF8,0x7F,0xFF,0xFF,// 'g'
0xFF,0xFF,0xF7,0xFF,0xF7,0xFF,0xF7,0xFF,0xF4,0x3F,0xF3,0xDF,0xF7,0xDF,0xF7,0xDF,0xF7,0xDF,0xF7,0xDF,0xF7,0xDF,0xF7,0xDF,0xF7,0xDF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,// 'h'
0xFF,0xFF,0xFF,0xFF,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,// 'i'
0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFC,0xFF,// 'j'
0xFF,0xFF,0xFB,0xFF,0xFB,0xFF,0xFB,0xFF,0xFB,0xEF,0xFB,0xDF,0xFB,0xBF,0xFB,0x7F,0xFA,0x7F,0xF9,0xBF,0xFB,0xDF,0xFB,0xDF,0xFB,0xEF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,// 'k'
0xFF,0xFF,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,// 'l'
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xD1,0x8F,0xCE,0x77,0xDE,0xF7,0xDE,0xF7,0xDE,0xF7,0xDE,0xF7,0xDE,0xF7,0xDE,0xF7,0xDE,0xF7,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,// 'm'
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF4,0x3F,0xF3,0xDF,0xF7,0xDF,0xF7,0xDF,0xF7,0xDF,0xF7,0xDF,0xF7,0xDF,0xF7,0xDF,0xF7,0xDF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,// 'n'
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0x7F,0xFB,0xBF,0xF7,0xDF,0xF7,0xDF,0xF7,0xDF,0xF7,0xDF,0xF7,0xDF,0xFB,0xBF,0xFC,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,// 'o'
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF4,0x7F,0xF3,0xBF,0xF7,0xDF,0xF7,0xDF,0xF7,0xDF,0xF7,0xDF,0xF7,0xDF,0xF3,0xBF,0xF4,0x7F,0xF7,0xFF,0xF7,0xFF,0xF7,0xFF,// 'p'
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0x5F,0xFB,0x9F,0xF7,0xDF,0xF7,0xDF,0xF7,0xDF,0xF7,0xDF,0xF7,0xDF,0xFB,0x9F,0xFC,0x5F,0xFF,0xDF,0xFF,0xDF,0xFF,0xDF,// 'q'
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFD,0x1F,0xFC,0xFF,0xFD,0xFF,0xFD,0xFF,0xFD,0xFF,0xFD,0xFF,0xFD,0xFF,0xFD,0xFF,0xFD,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,// 'r'
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0x3F,0xFB,0xDF,0xFB,0xFF,0xFB,0xFF,0xFC,0x3F,0xFF,0xDF,0xFF,0xDF,0xFB,0xDF,0xFC,0x3F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,// 's'
0xFF,0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xFC,0x3F,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0x3F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,// 't'
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF7,0xDF,0xF7,0xDF,0xF7,0xDF,0xF7,0xDF,0xF7,0xDF,0xF7,0xDF,0xF7,0xDF,0xF7,0x9F,0xF8,0x5F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,// 'u'
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF7,0xDF,0xF7,0xDF,0xFB,0xBF,0xFB,0xBF,0xFB,0xBF,0xFD,0x7F,0xFD,0x7F,0xFE,0xFF,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,// 'v'
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xDE,0xF7,0xDE,0xF7,0xED,0x6F,0xED,0x6F,0xEB,0xAF,0xEB,0xAF,0xEB,0xAF,0xF7,0xDF,0xF7,0xDF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,// 'w'
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF7,0xDF,0xFB,0xBF,0xFD,0x7F,0xFD,0x7F,0xFE,0xFF,0xFD,0x7F,0xFD,0x7F,0xFB,0xBF,0xF7,0xDF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,// 'x'
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF7,0xDF,0xF7,0xDF,0xF7,0xDF,0xFB,0xBF,0xFB,0xBF,0xFB,0x3F,0xFD,0x7F,0xFD,0x7F,0xFE,0xFF,0xFE,0xFF,0xFD,0xFF,0xF3,0xFF,// 'y'
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF8,0x0F,0xFF,0xDF,0xFF,0xBF,0xFF,0xBF,0xFF,0x7F,0xFE,0xFF,0xFE,0xFF,0xFD,0xFF,0xF8,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,// 'z'
0xFF,0x3F,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xF9,0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xFF,0x3F,0xFF,0xFF,// '{'
0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,// '|'
0xFC,0xFF,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x9F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFC,0xFF,0xFF,0xFF,// '}'
0xFF,0xFF,0xFF,0xFF,0xF8,0x77,0xF7,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF// '~'
};
