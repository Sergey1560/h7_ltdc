#ifndef MAIN_H
#define MAIN_H
#include "common_defs.h"
#include "rcc.h"
#include "ltdc.h"
#include <stdio.h>
#include <string.h>

#define   GREEN        0x07E0
#define   RED          0xF800

void gr_test(uint16_t *mem);

#endif