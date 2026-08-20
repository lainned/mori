#include "helpers.h"

void bitboard_print(uint64_t bitboard){
    for(int i = 7; i >= 0; i--){
        for(int j = 0; j <= 7; j++){
            char c = ((bitboard >> (i*8+j)) & 1)+'0';
            if(c == '0') c = '.';
            printf("%c ", c);
        }
        putchar('\n');
    }
    putchar('\n');
}