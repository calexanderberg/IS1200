//
// Created by Dennis Hadzialic on 2021-03-01.
//

#include <stdint.h>
#include <pic32mx.h>
#include "display.h"
#include "font.h"
#include "logo.h"

#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)

#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)

#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)

#define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)
#define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)

//Written by teacher
void display_string(int line, char *s) {
    int i;
    if (line < 0 || line >= 4)
        return;
    if (!s)
        return;

    for (i = 0; i < 16; i++)
        if (*s) {
            textbuffer[line][i] = *s;
            s++;
        } else
            textbuffer[line][i] = ' ';
}

//Written by teacher
void quicksleep(int cyc) {
    int i;
    for (i = cyc; i > 0; i--);
}

void setPixel(const short x, const short y, const uint8_t state) {
    if (x < 128 && x >= 0 && y < 32 && y >= 0) {// om den finns inuti skärmen
            uint8_t bit = 0x1 << (y - (y / 8 * 8));
            bufferstate[x + (y / 8) * 128] |= bit;
    }
}

/*

the proper code is

(lfsr[bit / 8] >> (bit % 8)) & 1

To explain it:

bit / 8 chooses an element from your array. Each element contains 8 bits, so dividing by 8 is an easy way to convert a bit index to an element index.

bit % 8 chooses a bit inside the element. This is most straightforward choice of indexing;
 it counts bits from the least significant bit to most significant bit (little-endian). Another variant is

7 - bit % 8

This variant counts the bits in reverse order (big-endian).
Sometimes you have to use it (e.g. in JPEG) for compatibility reasons;
if you are free to decide which bit order to choose, use little-endian (because it's easier).

 */

//written by teacher
void init_mcu(void) {
    /* Configure switches and buttonpins as inputs */
    TRISDSET = 0x0FE0; // Switches 4:1, Buttons 4:2
    TRISFSET = 0x0001; // Button 1

    /* Output pins for display signals */
    PORTF = 0xFFFF;
    PORTG = (1 << 9);
    ODCF = 0x0;
    ODCG = 0x0;
    TRISFCLR = 0x70;
    TRISGCLR = 0x200;

    /* Set up SPI as master */
    SPI2CON = 0;
    SPI2BRG = 4;
    SPI2STATCLR = 0x40;    /* SPI2STAT bit SPIROV = 0; */
    SPI2CONSET = 0x40;        /* SPI2CON bit CKP = 1; */
    SPI2CONSET = 0x20;        /* SPI2CON bit MSTEN = 1; */
    SPI2CONSET = 0x8000;    /* SPI2CON bit ON = 1; */
}

//Written by teacher
uint8_t spi_send_recv(uint8_t data) {
    while (!(SPI2STAT & 0x08));
    SPI2BUF = data;
    while (!(SPI2STAT & 1));
    return SPI2BUF;
}

//Written by teacher
void display_init(void) {
    DISPLAY_CHANGE_TO_COMMAND_MODE;
    quicksleep(10);
    DISPLAY_ACTIVATE_VDD;
    quicksleep(1000000);

    spi_send_recv(0xAE);
    DISPLAY_ACTIVATE_RESET;
    quicksleep(10);
    DISPLAY_DO_NOT_RESET;
    quicksleep(10);

    spi_send_recv(0x8D);
    spi_send_recv(0x14);

    spi_send_recv(0xD9);
    spi_send_recv(0xF1);

    DISPLAY_ACTIVATE_VBAT;
    quicksleep(10000000);

    spi_send_recv(0xA1);
    spi_send_recv(0xC8);

    spi_send_recv(0xDA);
    spi_send_recv(0x20);

    spi_send_recv(0xAF);
}

//Written by teacher
void display_update(void) {
    int i, j, k;
    int c;
    for (i = 0; i < 4; i++) {
        DISPLAY_CHANGE_TO_COMMAND_MODE;
        spi_send_recv(0x22);
        spi_send_recv(i);

        spi_send_recv(0x0);
        spi_send_recv(0x10);

        DISPLAY_CHANGE_TO_DATA_MODE;

        for (j = 0; j < 16; j++) {
            c = textbuffer[i][j];
            if (c & 0x80)
                continue;

            for (k = 0; k < 8; k++)
                spi_send_recv(font[c * 8 + k]);
        }
    }
}

//Written by teacher
void display_image(int x, const uint8_t *data) {
    int i, j;

    for (i = 0; i < 4; i++) {
        DISPLAY_CHANGE_TO_COMMAND_MODE;

        spi_send_recv(0x22);
        spi_send_recv(i);

        spi_send_recv(x & 0xF);
        spi_send_recv(0x10 | ((x >> 4) & 0xF));

        DISPLAY_CHANGE_TO_DATA_MODE;

        for (j = 0; j < 128; j++)
            spi_send_recv(data[i * 128 + j]);
    }
}


void clear() { //Clear screen
    int i;
    for (i = 0; i < 512; i++) {
        bufferstate[i] = 0;
    }

}