#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "board.h"
#include "fen.h"

int main(int argc, char** argv){
    FEN fen;
    if(argc > 1 && argc < 8){
        size_t len = strlen(argv[1]);
        if(len > sizeof(fen.pieces) / sizeof(char)){
            printf("FEN pieces too long\n");
            return -1;
        }
        memcpy(fen.pieces, argv[1], len);
        len = strlen(argv[2]);
        if(len > 1){
            printf("FEN active color too long\n");
            return -1;
        }
        fen.active_color = argv[2][0];
        len = strlen(argv[3]);
        if(len > sizeof(fen.castling) / sizeof(char)){  
            printf("FEN castling too long\n");
            return -1;
        }
        memcpy(fen.castling, argv[3], len);
        len = strlen(argv[4]);
        if(len > sizeof(fen.en_passant) / sizeof(char)){
            printf("FEN en passant too long\n");
            return -1;
        }
        memcpy(fen.en_passant, argv[4], len);
        fen.half_move = atoi(argv[5]);
        fen.full_move = atoi(argv[6]);
    }
    else if(argc > 7){
        printf("Too many arguments\n");
        return -1;
    }
    else{
        printf("Please enter the FEN after the command\n");
        return -1;
    }

    Board board;
}