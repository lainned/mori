#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "board.h"
#include "fen.h"
#include "gen.h"
#include "helpers.h"


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
        printf("FEN not provided, using the default FEN\n");
        //rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
        char* pieces = "1k6/3q4/8/8/8/1N1r1n2/8/1K1P4";
        //char* kings = "8/2k5/8/8/8/3K4/8/8";
        char color = 'b';
        char* castling = "KQkq";
        char* en_passant = "-";
        int half_move = 1;
        int full_move = 0;
        memcpy(fen.pieces, pieces, strlen(pieces));
        fen.active_color = color;
        memcpy(fen.castling, castling, strlen(castling));
        memcpy(fen.en_passant, en_passant, strlen(en_passant));
        fen.half_move = half_move;
        fen.full_move = full_move;
    }

    Board board;
    for(int i = 0; i < 64; i++){
        board.mailbox[i] = '.';
    }
    for(int i = 0; i < TYPES; i++){
        board.bitboard[i] = 0;
    }
    fen_to_board(&fen, &board);
    Move moves[MAX_MOVE_COUNT];
    uint16_t len = 0;
    gen_init();
    generate_legal_moves(board, moves, &len);
    // for(int i = 0; i < len; i++){
    //     printf("Move %d: \n", i);
    //     printf("From %d to %d\n", moves[i].from, moves[i].to);
    // }

    
}