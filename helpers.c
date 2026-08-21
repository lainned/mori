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

Type char_to_type(char piece){
    Type type;
    switch(piece){
        case 'K':
            type = WHITE_KINGS;
            break;
        case 'k':
            type = BLACK_KINGS;
            break;
        case 'N':
            type = WHITE_KNIGHTS;
            break;
        case 'n':
            type = BLACK_KNIGHTS;
            break;
        case 'B':
            type = WHITE_BISHOPS;
            break;
        case 'b':
            type = BLACK_BISHOPS;
            break;
        case 'R':
            type = WHITE_ROOKS;
            break;
        case 'r':
            type = BLACK_ROOKS;
            break;
        case 'Q':
            type = WHITE_QUEENS;
            break;
        case 'q':
            type = BLACK_QUEENS;
            break;
        case 'P':
            type = WHITE_PAWNS;
            break;
        case 'p':
            type = BLACK_PAWNS;
            break;
        default:
            type = BOTH;
            break;
    }
    return type;
}

char number_to_pos[64][3] = {"a1", "b1", "c1", "d1","e1","f1","g1","h1","a2", "b2", "c2", "d2","e2","f2","g2","h2", "a3", "b3", "c3", "d3","e3","f3","g3","h3","a4", "b4", "c4", "d4","e4","f4","g4","h4", "a5", "b5", "c5", "d5","e5","f5","g5","h5","a6", "b6", "c6", "d6","e6","f6","g6","h6", "a7", "b7", "c7", "d7","e7","f7","g7","h7","a8", "b8", "c8", "d8","e8","f8","g8","h8" };