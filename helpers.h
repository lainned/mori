#pragma once
#include "board.h"
#include <stdint.h>
#include <stdio.h>

void bitboard_print(uint64_t bitboard);
Type char_to_type(char piece);
extern char number_to_pos[64][3]; 