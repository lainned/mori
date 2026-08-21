#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "board.h"
#include "fen.h"
#include "gen.h"
#include "helpers.h"

int compare(const void* a, const void* b){
    if(((Move*)a)->from == ((Move*)b)->from) return ((Move*)a)->to > ((Move*)b)->to;
    return ((Move*)a)->from > ((Move*)b)->from;
}

uint64_t perft(Board* board, int depth, int max_depth){
    Move moves[MAX_MOVE_COUNT];
    uint16_t len = 0;
    generate_legal_moves(*board, moves, &len);
    uint64_t nodes = 0;
    if(depth == 1){
        for(int i = 0; i < len; i++){
            printf("%s%s: 1\n", number_to_pos[moves[i].from], number_to_pos[moves[i].to]);
            
        }
        return len;
    }
    for(int i = 0; i < len; i++){
        Board copy = *board;
        make_move(&copy, moves[i]);
        copy.white_turn = !copy.white_turn;
        uint64_t res = perft(&copy, depth - 1, max_depth);
        nodes += res;
        if(depth == max_depth)
        printf("%s%s: %d\n", number_to_pos[moves[i].from], number_to_pos[moves[i].to], res);
    }
    return nodes;
}

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
        char* pieces = "r3k2r/p1ppqpb1/1n2pnp1/3PN3/1p2P3/2N2Q1p/PPPBbPPP/1R2K2R";
        //char* kings = "8/2k5/8/8/8/3K4/8/8";
        char color = 'w';
        char* castling = "Kkq";
        char* en_passant = "-";
        int half_move = 0;
        int full_move = 1;
        memcpy(fen.pieces, pieces, strlen(pieces));
        fen.active_color = color;
        memcpy(fen.castling, castling, strlen(castling));
        memcpy(fen.en_passant, en_passant, strlen(en_passant));
        fen.half_move = half_move;
        fen.full_move = full_move;
    }

    Board board;
    // for(int i = 0; i < 64; i++){
    //     board.mailbox[i] = '.';
    // }
    for(int i = 0; i < TYPES; i++){
        board.bitboard[i] = 0;
    }
    gen_init();
    fen_to_board(&fen, &board);
    int depth = 1;
    scanf("%d", &depth);
    uint64_t perft_result = perft(&board, depth, depth);
    printf("Perft depth %d result nodes: %llu", depth, (unsigned long long)perft_result);

    
}