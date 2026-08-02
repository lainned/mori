#include "fen.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

void fen_to_board(FEN* fen, Board* board){
    // default fen is rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
    int idx = 0;
    int pos = 63;
    char c;
    while(!(fen->pieces[idx] != '\0' || idx < 71)){
        c = fen->pieces[idx];
        switch(c){
            case 'p':
                board->bitboard[BLACK_PAWNS] |= (1 << pos--);
                break;
            case 'r':
                board->bitboard[BLACK_ROOKS] |= (1 << pos--);
                break;
            case 'b':
                board->bitboard[BLACK_BISHOPS] |= (1 << pos--);
                break;
            case 'n':
                board->bitboard[BLACK_KNIGHTS] |= (1 << pos--);
                break;
            case 'q':
                board->bitboard[BLACK_QUEENS] |= (1 << pos--);
                break;
            case 'k':
                board->bitboard[BLACK_KINGS] |= (1 << pos--);
                break;
            case 'P':
                board->bitboard[WHITE_PAWNS] |= (1 << pos--);
                break;
            case 'R':
                board->bitboard[WHITE_ROOKS] |= (1 << pos--);
                break;
            case 'B':
                board->bitboard[WHITE_BISHOPS] |= (1 << pos--);
                break;
            case 'N':
                board->bitboard[WHITE_KNIGHTS] |= (1 << pos--);
                break;
            case 'Q':
                board->bitboard[WHITE_QUEENS] |= (1 << pos--);
                break;
            case 'K':
                board->bitboard[WHITE_KINGS] |= (1 << pos--);
                break;
            case '/':
                break;
            default:
                if(c >= '1' && c <= '8'){
                    pos-=(int)(c - '0');
                    break;
                }
                printf("Error when parsing FEN\n");
                exit(-1);
        }
        idx++;
    }
    c = fen->active_color;
    if(c != 'w' && c != 'b'){
        printf("Error when parsing FEN\n");
        exit(-1);
    }
    
    board->white_turn = (c == 'w');
    
    idx = 0;
    while(idx < 2){
        c = fen->castling[idx];
        switch(c){
            case 'K':
                board->white_king_castling = true;
                break;
            case 'Q':
                board->white_queen_castling = true;
                break;
            case 'k':
                board->white_king_castling = true;
                break;
            case 'q':
                board->white_queen_castling = true;
                break;
            default:
                printf("Error when parsing FEN\n");
                exit(-1);
        }
        idx++;
    }
    char file = fen->en_passant[0];
    char rank = fen->en_passant[1];

    if(isupper(file) || !(rank >= '1' && rank <= '8')){
        printf("Error when parsing FEN\n");
        exit(-1);
    }
    // is en passant valid (turn)
    if((board->white_turn && rank != '6') || (!board->white_turn && file != '3')){
        printf("Error when parsing FEN\n");
        exit(-1);
    }

    board->half_move = fen->half_move;
    board->full_move = fen->full_move;
}