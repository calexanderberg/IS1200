//
// Created by Dennis Hadzialic on 2021-03-01.
//

#include "inputs.h"
#include <pic32mx.h>

int getsw(void) { //Switcharna

    int switches = (PORTD >> 8) & 0x000f;
    return switches;
}

int getbtns(void) {    //Input för knapp 4-2

    int button = (PORTD >> 5) & 0x0007;
    return button;
}

int getbtn1(void) { //Inputen för knapp1
    int button1 = (PORTF >> 1) & 0x0001;
    return button1;
}