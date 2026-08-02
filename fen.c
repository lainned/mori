#include "fen.h"

void fen_to_board(FEN* fen, Board* board){
    // default fen is rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
    int idx = 0;
    while(fen->pieces[idx++] != '\0'){
        
    }
}