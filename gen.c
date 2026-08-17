#include <stdbool.h>
#include <stdint.h>
#include "gen.h"
#include "board.h"
uint64_t king_move_board[BOARD_SIZE];
uint64_t knight_move_board[BOARD_SIZE];
uint64_t bishop_move_board[BOARD_SIZE];
uint64_t rook_move_board[BOARD_SIZE];
uint64_t pawn_attack_board[2][BOARD_SIZE];

uint64_t rook_attack_boards[64][8192];
uint64_t bishop_attack_board[64][1024];

int ROOK_RELEVANT_BITS[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};
int BISHOP_RELEVANT_BITS[64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6
};

// magics

void 

uint64_t get_bishop_attacks(int square, uint64_t blockers){
    
}

uint64_t get_rook_attacks(int square, uint64_t blockers){

}

void init_king(void){
    for(int i = 0; i < BOARD_SIZE; i++){
        uint64_t king = (1ULL << i);
        uint64_t board = 0;
        board |= (king & (~H_FILE)) << 1;
        board |= (king & (~A_FILE)) >> 1;
        board |= (king & (~RANK_8)) << 8;
        board |= (king & (~RANK_1)) >> 8;

        board |= (king & (~(RANK_8|H_FILE))) << 9;
        board |= (king & (~(RANK_8|A_FILE))) << 7;
        board |= (king & (~(RANK_1|H_FILE))) >> 7;
        board |= (king & (~(RANK_1|A_FILE))) >> 9;
        king_move_board[i] = board;
    }
}

void init_knight(void){
    for(int i = 0; i < BOARD_SIZE; i++){
        uint64_t board = 0;
        uint64_t knight = (1ULL << i);

        board |= (knight & (~(G_FILE|H_FILE|RANK_8))) << 10;
        board |= (knight & (~(H_FILE|RANK_7|RANK_8))) << 17;
        board |= (knight & (~(A_FILE|RANK_7|RANK_8))) << 15;
        board |= (knight & (~(A_FILE|B_FILE|RANK_8))) << 6;
        board |= (knight & (~(A_FILE|B_FILE|RANK_1))) >> 10;
        board |= (knight & (~(A_FILE|RANK_2|RANK_1))) >> 17;
        board |= (knight & (~(H_FILE|RANK_2|RANK_1))) >> 15;
        board |= (knight & (~(G_FILE|H_FILE|RANK_1))) >> 6;

        knight_move_board[i] = board;
    }
}

void init_pawns(void){
    for(int i = 0; i < BOARD_SIZE; i++){
        uint64_t white = 0, black = 0;

        uint64_t pawn = (1ULL << i);
        white |= (pawn & (~(A_FILE|RANK_8))) << 7;
        white |= (pawn & (~(H_FILE|RANK_8))) << 9;

        black |= (pawn & (~(H_FILE|RANK_1))) >> 7;
        black |= (pawn & (~(A_FILE|RANK_1))) >> 9;
        pawn_attack_board[0][i] = white;
        pawn_attack_board[1][i] = black;
    }
}

void init_bishops(void){
    for(int i = 0; i < 64; i++){
        uint64_t board = 0;
        int rank = i / 8;
        int file = i % 8;
        for(int r = rank + 1, f = file + 1; r <= 7 && f <= 7; r++, f++) board |= (1ULL << (r * 8 + f));
        for(int r = rank + 1, f = file - 1; r <= 7 && f >= 0; r++, f--) board |= (1ULL << (r * 8 + f));
        for(int r = rank - 1, f = file + 1; r >= 0 && f <= 7; r--, f++) board |= (1ULL << (r * 8 + f));
        for(int r = rank - 1, f = file - 1; r >= 0 && f >= 0; r--, f--) board |= (1ULL << (r * 8 + f));
        bishop_move_board[i] = board;
    }
}

void init_rooks(void){
    for(int i = 0; i < 64; i++){
        uint64_t board = 0;
        int rank = i / 8;
        int file = i % 8;
        for(int r = 0; r <= 7; r++) board |= (1ULL << (r * 8 + file));
        for(int f = 0; f <= 7; f++) board |= (1ULL << (rank * 8 + f));
        board &= (1ULL << i);
    }
}



bool is_square_attacked(const Board board, uint16_t square){

}

void generate_king_moves(const Board board, Move moves[], uint16_t* len){
    uint64_t king_board = board.white_turn ? board.bitboard[WHITE_KINGS] : board.bitboard[BLACK_KINGS];
    Type color = board.white_turn ? WHITE : BLACK;
    uint16_t square = __builtin_ctzll(king_board);
    while(king_board){
        uint64_t move_board = king_move_board[square];
        move_board &= (~board.bitboard[color]);
        while(move_board){
            uint16_t to_square = __builtin_ctzll(move_board);
            Move new_move;
            new_move.castling = CASTLING_NONE;
            new_move.from = square;
            new_move.to = to_square;
            new_move.legality = NOT_VALIDATED;
            new_move.piece = board.white_turn ? 'K' : 'k';
            moves[*len] = new_move;
            (*len)++;
            move_board &= (move_board - 1);
        }
        king_board &= (king_board - 1);
        square = __builtin_ctzll(king_board);
    }
}

void generate_knight_moves(const Board board, Move* moves, uint16_t* len){
    uint64_t knight_board = board.white_turn ? board.bitboard[WHITE_KNIGHTS] : board.bitboard[BLACK_KNIGHTS];
    Type color = board.white_turn ? WHITE : BLACK;
    uint16_t square = __builtin_ctzll(knight_board);
    while(knight_board){
        uint64_t move_board = knight_move_board[square];
        move_board &= (~board.bitboard[color]);
        while(move_board){
            uint16_t to_square = __builtin_ctzll(move_board);
            Move new_move;
            new_move.castling = CASTLING_NONE;
            new_move.from = square;
            new_move.to = to_square;
            new_move.legality = NOT_VALIDATED;
            new_move.piece = board.white_turn ? 'N' : 'n';
            moves[*len] = new_move;
            (*len)++;
            move_board &= (move_board - 1);
        }
        knight_board &= (knight_board - 1);
        square = __builtin_ctzll(knight_board);
    }
}

void generate_pawn_moves(const Board board, Move* moves, uint16_t* len){
    uint64_t pawn_board = board.white_turn ? board.bitboard[WHITE_PAWNS] : board.bitboard[BLACK_PAWNS];
    uint64_t own_occupancy = board.white_turn ? board.bitboard[WHITE] : board.bitboard[BLACK];
    uint64_t enemy_occupancy = board.white_turn ? board.bitboard[BLACK] : board.bitboard[WHITE];
    int white = board.white_turn;
    while(pawn_board){
        uint16_t square = __builtin_ctzll(pawn_board);
        // single push
        uint64_t all = 0;
        uint64_t pawn = (1ULL << square);
        uint64_t after = 0;
        if(white) after = (pawn & ~(RANK_8)) << 8;
        else after = (pawn & ~(RANK_1)) >> 8;
        after &= (~(board.bitboard[BOTH]));
        all |= after;
        // double push
        // firstly check if we can move one ahead
        after = 0;
        if(white)after = (pawn & RANK_2) << 8;
        else after = (pawn & RANK_7) >> 8;
        after &= (~(board.bitboard[BOTH]));
        
        // if we can check if we can move two ahead
        if(white)after <<= 8;
        else after >>= 8;
        after &= (~(board.bitboard[BOTH]));


        // attack 
        after=0;
        // [0] is white array, [1] is black
        after |= (pawn_attack_board[!white][square] & enemy_occupancy & ~own_occupancy);
        all |= after;

        // en passant
        after -= 0;
        after |= (pawn_attack_board[!white][square] & board.en_passant);
        all |= after;

        while(all){
            uint16_t to = __builtin_ctzll(all);
            Move new_move = {square, to, NOT_VALIDATED, CASTLING_NONE, PROMOTION_NONE, board.white_turn ? 'P' : 'p'};
            bool is_promoting = (white ? (1ULL << to) & RANK_8 : (1ULL << to) & RANK_1);
            moves[(*len)] = new_move;
            (*len)++;
            if(is_promoting){
                for(int i = 1; i <= 4; i++){
                    new_move.promotion = i;
                    moves[(*len)] = new_move;
                    (*len)++;
                }
            }
            all &= (all - 1);
        }
        pawn_board &= (pawn_board - 1);
    }
}
void generate_legal_moves(const Board board, Move* moves, uint16_t* len){
    generate_pawn_moves(board, moves,len);
    generate_king_moves(board, moves, len);
    generate_knight_moves(board, moves, len);
}
