#include <stdbool.h>
#include <stdint.h>
#include "gen.h"
#include "board.h"
#include "helpers.h"

uint64_t king_move_board[BOARD_SIZE];
uint64_t knight_move_board[BOARD_SIZE];
uint64_t bishop_move_board[BOARD_SIZE];
uint64_t rook_move_board[BOARD_SIZE];
uint64_t pawn_attack_board[2][BOARD_SIZE];

uint64_t rook_attack_boards[64][4096];
uint64_t bishop_attack_boards[64][512];

// shallow blue magics

const uint64_t rook_magics[64] = {
    0xa8002c000108020ULL, 0x6c00049b0002001ULL, 0x100200010090040ULL, 0x2480041000800801ULL, 0x280028004000800ULL,
    0x900410008040022ULL, 0x280020001001080ULL, 0x2880002041000080ULL, 0xa000800080400034ULL, 0x4808020004000ULL,
    0x2290802004801000ULL, 0x411000d00100020ULL, 0x402800800040080ULL, 0xb000401004208ULL, 0x2409000100040200ULL,
    0x1002100004082ULL, 0x22878001e24000ULL, 0x1090810021004010ULL, 0x801030040200012ULL, 0x500808008001000ULL,
    0xa08018014000880ULL, 0x8000808004000200ULL, 0x201008080010200ULL, 0x801020000441091ULL, 0x800080204005ULL,
    0x1040200040100048ULL, 0x120200402082ULL, 0xd14880480100080ULL, 0x12040280080080ULL, 0x100040080020080ULL,
    0x9020010080800200ULL, 0x813241200148449ULL, 0x491604001800080ULL, 0x100401000402001ULL, 0x4820010021001040ULL,
    0x400402202000812ULL, 0x209009005000802ULL, 0x810800601800400ULL, 0x4301083214000150ULL, 0x204026458e001401ULL,
    0x40204000808000ULL, 0x8001008040010020ULL, 0x8410820820420010ULL, 0x1003001000090020ULL, 0x804040008008080ULL,
    0x12000810020004ULL, 0x1000100200040208ULL, 0x430000a044020001ULL, 0x280009023410300ULL, 0xe0100040002240ULL,
    0x200100401700ULL, 0x2244100408008080ULL, 0x8000400801980ULL, 0x2000810040200ULL, 0x8010100228810400ULL,
    0x2000009044210200ULL, 0x4080008040102101ULL, 0x40002080411d01ULL, 0x2005524060000901ULL, 0x502001008400422ULL,
    0x489a000810200402ULL, 0x1004400080a13ULL, 0x4000011008020084ULL, 0x26002114058042ULL
};

const uint64_t bishop_magics[64] = {
    0x89a1121896040240ULL, 0x2004844802002010ULL, 0x2068080051921000ULL, 0x62880a0220200808ULL, 0x4042004000000ULL,
    0x100822020200011ULL, 0xc00444222012000aULL, 0x28808801216001ULL, 0x400492088408100ULL, 0x201c401040c0084ULL,
    0x840800910a0010ULL, 0x82080240060ULL, 0x2000840504006000ULL, 0x30010c4108405004ULL, 0x1008005410080802ULL,
    0x8144042209100900ULL, 0x208081020014400ULL, 0x4800201208ca00ULL, 0xf18140408012008ULL, 0x1004002802102001ULL,
    0x841000820080811ULL, 0x40200200a42008ULL, 0x800054042000ULL, 0x88010400410c9000ULL, 0x520040470104290ULL,
    0x1004040051500081ULL, 0x2002081833080021ULL, 0x400c00c010142ULL, 0x941408200c002000ULL, 0x658810000806011ULL,
    0x188071040440a00ULL, 0x4800404002011c00ULL, 0x104442040404200ULL, 0x511080202091021ULL, 0x4022401120400ULL,
    0x80c0040400080120ULL, 0x8040010040820802ULL, 0x480810700020090ULL, 0x102008e00040242ULL, 0x809005202050100ULL,
    0x8002024220104080ULL, 0x431008804142000ULL, 0x19001802081400ULL, 0x200014208040080ULL, 0x3308082008200100ULL,
    0x41010500040c020ULL, 0x4012020c04210308ULL, 0x208220a202004080ULL, 0x111040120082000ULL, 0x6803040141280a00ULL,
    0x2101004202410000ULL, 0x8200000041108022ULL, 0x21082088000ULL, 0x2410204010040ULL, 0x40100400809000ULL,
    0x822088220820214ULL, 0x40808090012004ULL, 0x910224040218c9ULL, 0x402814422015008ULL, 0x90014004842410ULL,
    0x1000042304105ULL, 0x10008830412a00ULL, 0x2520081090008908ULL, 0x40102000a0a60140ULL,
}; 

const int rook_relevant_bits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};
const int bishop_relevant_bits[64] = {
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


uint64_t generate_rook_blocked_attacks(int square, uint64_t blockers){
    uint64_t board = 0;
    int rank = square / 8;
    int file = square % 8;

    for(int r = rank + 1; r <= 7; r++){
        int pos = r * 8 + file;
        board |= (1ULL << pos);
        if(blockers & (1ULL << pos)) break;
    }
    for(int r = rank - 1; r >= 0; r--){
        int pos = r * 8 + file;
        board |= (1ULL << pos);
        if(blockers & (1ULL << pos)) break;
    }
    for(int f = file + 1; f <= 7; f++){
        int pos = rank * 8 + f;
        board |= (1ULL << pos);
        if(blockers & (1ULL << pos)) break;
    }
    for(int f = file - 1; f >= 0; f--){
        int pos = rank * 8 + f;
        board |= (1ULL << pos);
        if(blockers & (1ULL << pos)) break;
    }

    return board;
}

uint64_t generate_bishop_blocked_attacks(int square, uint64_t blockers){
    uint64_t board = 0;

    int rank = square / 8;
    int file = square % 8;

    for(int r = rank + 1, f = file + 1; r <= 7 && f <= 7; r++, f++){
        board |= (1ULL << (r * 8 + f));
        if(blockers & (1ULL << (r * 8 + f))) break;
    }
    for(int r = rank + 1, f = file - 1; r <= 7 && f >= 0; r++, f--){
        board |= (1ULL << (r * 8 + f));
        if(blockers & (1ULL << (r * 8 + f))) break;
    }
    for(int r = rank - 1, f = file + 1; r >= 0 && f <= 7; r--, f++){
        board |= (1ULL << (r * 8 + f));
        if(blockers & (1ULL << (r * 8 + f))) break;
    }
    for(int r = rank - 1, f = file - 1; r >= 0 && f >= 0; r--, f--){
        board |= (1ULL << (r * 8 + f));
        if(blockers & (1ULL << (r * 8 + f))) break;
    }
    //bitboard_print(board);
    return board;
}


void init_magic_attack_table(bool rook){
    for(int square = 0; square < 64; square++){
        uint64_t move_board = rook ? rook_move_board[square] : bishop_move_board[square];
        
        int relevant_bits = rook ? rook_relevant_bits[square] : bishop_relevant_bits[square];
        int variations = (1 << relevant_bits);
        // we iterate through all blockers variations
        for(int i = 0; i < variations; i++){
            uint64_t blockers = 0;
            uint64_t temp_board = move_board;
            for(int bit = 0; bit < relevant_bits; bit++){
                // there are maximum n where n is relevant_bits bits set in the rook move board at that square
                // so we iterate through all possible variations [0, 2^(relevant_bits)]
                // lets say we have 10 bits set in the move mask so lsb is first, bit after lsb is second and it goes on
                // we iterate through them like an array using ctz instruction so each of them has an index (bit)
                // if that index was set in current variations we set them at their original position in the blockers mask
                // and that gives us all of the possible variations of blockers (bits set) on the rook rays on a given square

                // we get the lsb and zero it 
                int sq = __builtin_ctzll(temp_board);
                temp_board &= ~(1ULL << sq);
                // if that bit was 1 in this variation we set it as a blocker
                if(i & (1 << bit)){
                    blockers |= (1ULL << sq);
                }
            }
            // generating the attack board with current variation of blockers
            uint64_t attack_board = 0;
            if(rook) attack_board = generate_rook_blocked_attacks(square, blockers);
            else attack_board = generate_bishop_blocked_attacks(square, blockers);

            // calculating the key using magic numbers for a given square
            // it allows us to fit a range of [0, 2^64) in [0, 2^(relevant_bits)]
            // collisions map to the same attack board so its a perfect hashing technique
            if(rook){
                uint64_t key = (blockers * rook_magics[square]) >> (64 - rook_relevant_bits[square]);
                rook_attack_boards[square][key] = attack_board;
            }
            else{
                uint64_t key = (blockers * bishop_magics[square]) >> (64 - bishop_relevant_bits[square]);
                bishop_attack_boards[square][key] = attack_board;
            }
        }
        
    }
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
        for(int r = rank + 1, f = file + 1; r <= 6 && f <= 6; r++, f++) board |= (1ULL << (r * 8 + f));
        for(int r = rank + 1, f = file - 1; r <= 6 && f >= 1; r++, f--) board |= (1ULL << (r * 8 + f));
        for(int r = rank - 1, f = file + 1; r >= 1 && f <= 6; r--, f++) board |= (1ULL << (r * 8 + f));
        for(int r = rank - 1, f = file - 1; r >= 1 && f >= 1; r--, f--) board |= (1ULL << (r * 8 + f));
        board &= ~(1ULL << i);
        bishop_move_board[i] = board;
    }
}

void init_rooks(void){
    for(int i = 0; i < 64; i++){
        uint64_t board = 0;
        int rank = i / 8;
        int file = i % 8;
        for(int r = 1; r <= 6; r++) board |= (1ULL << (r * 8 + file));
        for(int f = 1; f <= 6; f++) board |= (1ULL << (rank * 8 + f));
        board &= ~(1ULL << i);
        rook_move_board[i] = board;
    }
}


void gen_init(void){
    init_king();
    init_knight();
    init_bishops();
    init_rooks();
    init_pawns();
    init_magic_attack_table(true);
    init_magic_attack_table(false);
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

void generate_rook_moves(const Board board, Move* moves, uint16_t* len){
    uint64_t rook_board = board.white_turn ? board.bitboard[WHITE_ROOKS] : board.bitboard[BLACK_ROOKS];
    uint64_t all_pieces = board.bitboard[BOTH];
    uint64_t own_pieces = board.white_turn ? board.bitboard[WHITE] : board.bitboard[BLACK];

    while(rook_board){
        int sq = __builtin_ctzll(rook_board);
        uint64_t blockers = rook_move_board[sq] & all_pieces;

        uint64_t key = (blockers * rook_magics[sq]) >> (64 - rook_relevant_bits[sq]);
        uint64_t move_board = rook_attack_boards[sq][key];
        move_board &= ~(own_pieces);
        bitboard_print(move_board);
        while(move_board){
            int to = __builtin_ctzll(move_board);
            Move new_move = {sq, to, NOT_VALIDATED, CASTLING_NONE, PROMOTION_NONE, board.white_turn ? 'R' : 'r'};
            moves[(*len)] = new_move;
            (*len)++;

            move_board &= (move_board - 1);
        }
        rook_board &= (rook_board - 1);
    }
}

void generate_bishop_moves(const Board board, Move* moves, uint16_t* len){
    uint64_t bishop_board = board.white_turn ? board.bitboard[WHITE_BISHOPS] : board.bitboard[BLACK_BISHOPS];
    uint64_t all_pieces = board.bitboard[BOTH];
    uint64_t own_pieces = board.white_turn ? board.bitboard[WHITE] : board.bitboard[BLACK];

    while(bishop_board){
        int sq = __builtin_ctzll(bishop_board);
        uint64_t blockers = bishop_move_board[sq] & all_pieces;

        uint64_t key = (blockers * bishop_magics[sq]) >> (64 - bishop_relevant_bits[sq]);
        uint64_t move_board = bishop_attack_boards[sq][key];
        move_board &= ~(own_pieces);
        bitboard_print(move_board);
        while(move_board){
            int to = __builtin_ctzll(move_board);
            Move new_move = {sq, to, NOT_VALIDATED, CASTLING_NONE, PROMOTION_NONE, board.white_turn ? 'B' : 'b'};
            moves[(*len)] = new_move;
            (*len)++;

            move_board &= (move_board - 1);
        }
        bishop_board &= (bishop_board - 1);
    }
}
void generate_queen_moves(const Board board, Move* moves, uint16_t* len){
    uint64_t queen_board = board.white_turn ? board.bitboard[WHITE_QUEENS] : board.bitboard[BLACK_QUEENS];
    uint64_t all_pieces = board.bitboard[BOTH];
    uint64_t own_pieces = board.white_turn ? board.bitboard[WHITE] : board.bitboard[BLACK];

    while(queen_board){
        int sq = __builtin_ctzll(queen_board);

        // bishop
        uint64_t blockers = bishop_move_board[sq] & all_pieces;
        uint64_t key = (blockers * bishop_magics[sq]) >> (64 - bishop_relevant_bits[sq]);
        uint64_t move_board = bishop_attack_boards[sq][key];

        // rook
        blockers = rook_move_board[sq] & all_pieces;
        key = (blockers * rook_magics[sq]) >> (64 - rook_relevant_bits[sq]);
        move_board |= rook_attack_boards[sq][key];

        move_board &= ~(own_pieces);

        while(move_board){
            int to = __builtin_ctzll(move_board);
            Move new_move = {sq, to, NOT_VALIDATED, CASTLING_NONE, PROMOTION_NONE, board.white_turn ? 'Q' : 'q'};
            moves[(*len)] = new_move;
            (*len)++;
            move_board &= (move_board - 1);
        }

        queen_board &= (queen_board - 1);
    }
}

bool is_square_attacked(const Board board, uint16_t square){
    
}

void generate_legal_moves(const Board board, Move* moves, uint16_t* len){
    // firstly generate all pseudo legal moves
    generate_pawn_moves(board, moves,len);
    generate_king_moves(board, moves, len);
    generate_knight_moves(board, moves, len);
    generate_rook_moves(board,moves, len);
    generate_bishop_moves(board, moves, len);
    generate_queen_moves(board, moves, len);
}
