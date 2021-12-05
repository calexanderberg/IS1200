//
// Created by Dennis Hadzialic on 2021-03-01.
//
#ifndef MINI_PROJECT_DISPLAY_H
#define MINI_PROJECT_DISPLAY_H

void display_string(int line, char *s);

void quicksleep(int cyc);

void setPixel(const short x, const short y, const uint8_t state);

void init_mcu(void);

uint8_t spi_send_recv(uint8_t data);

void display_init(void);

void display_update(void);

void display_image(int x, const uint8_t *data);

void clear();

char textbuffer[4][16];

uint8_t bufferstate[512];

#endif //MINI_PROJECT_DISPLAY_H
