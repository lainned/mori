#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

int wking_index = 4;
int bking_index = 60;

int is_check(int white, const char* board);

void print_board(const char* board){
    for(int i = 7; i >= 0; i--){
        if(i != 7) putchar('\n');
        printf("%d | ", (i + 1));
        for(int j = i * 8; j < (i + 1) * 8; j++){
            printf("%c ", board[j]);
        }
    }
    putchar('\n');
    printf("    "); 
    for(int i = 0; i < 8; i++){
        printf("~ ");
    }
    printf("\n    "); 
    for(int i = 0; i < 8; i++){
        printf("%c ", ('A' + i));
    }
    putchar('\n');
}

int is_the_same(int cur_index, int next_index, int white, const char* board){
    if(cur_index == next_index) return 0;
    if(board[next_index] == '.') return 0;
    return (white && toupper(board[next_index]) == board[next_index]) || (!white && tolower(board[next_index]) == board[next_index]);
}



int is_legal_pawn_move(int cur_index, int next_index, int white, const char* board){
    if(is_the_same(cur_index, next_index, white, board)) return -1;
    if(white){
        // move forward
        if(board[next_index] == '.' && next_index - cur_index == 8){
            // promotion
            if(next_index >= 56){
                return 1;
            }
            return 0;
        }
        // capture (tolower check used (black are lower case))
        if(board[next_index] != '.'){
            // promotion
            if(next_index >= 56){
                return 1;
            }
            return 0;
        }
        if(board[next_index] == '.' && cur_index >= 8 && cur_index <= 15 && next_index - cur_index == 16){
            return 0;
        }
    }
    else{
        // move forward
        if(board[next_index] == '.' && cur_index - next_index == 8){
            // promotion
            if(next_index <= 7){
                return 1;
            }
            return 0;
        }
        // capture (toupper check used (white are upper case))
        if(board[next_index] != '.' && (toupper(board[next_index]) == board[next_index])){
            // promotion
            if(next_index <= 7){
                return 1;
            }
            return 0;
        }
        if(board[next_index] == '.' && cur_index >= 48 && cur_index <= 55 && cur_index - next_index == 16){
            return 0;
        }
    }
}

int is_legal_rook_move(int cur_index, int next_index, int white, const char* board){
    if(is_the_same(cur_index, next_index, white, board)) return -1;
    if(next_index % 8 == cur_index % 8){
        if(cur_index > next_index){
            for(int i = cur_index - 8; i > next_index; i-=8){
                if(board[i] != '.') return -1;
            }
        }
        else{
            for(int i = cur_index + 8; i < next_index; i+=8){
                if(board[i] != '.') return -1;
            }
        }
        return 0;
    }
    else if(next_index / 8 == cur_index / 8){
        if(cur_index > next_index){
            for(int i = cur_index - 1; i > next_index; i--){
                if(board[i] != '.') return -1;
            }
        }
        else{
            for(int i = cur_index + 1; i < next_index; i++){
                if(board[i] != '.') return -1;
            }
        }
        return 0;
    }
    return -1;
}

int is_legal_bishop_move(int cur_index, int next_index, int white, const char* board){
    if(is_the_same(cur_index, next_index, white, board)) return -1;
    if(next_index % 9 != cur_index % 9) return -1;
    if(cur_index > next_index){
        for(int i = cur_index - 9; i > next_index; i-=9){
            if(board[i] != '.') return -1;
        }
    }
    else{
        for(int i = cur_index + 9; i < next_index; i+=9){
            if(board[i] != '.') return -1;
        }
    }
    return 0;

}

int is_legal_knight_move(int cur_index, int next_index, int white, const char* board){
    if(is_the_same(cur_index, next_index, white, board)) return -1;
    int diff = cur_index - next_index;
    if(diff == 10 || diff == 17 || diff == -6 || diff == -15 || diff == -17 || diff == -10 || diff == 6 || diff == 15) return 0;
    return -1;
}

int is_legal_queen_move(int cur_index, int next_index, int white, const char* board){
    int result;
    if((result = is_legal_rook_move(cur_index, next_index, white, board)) == -1) return -1;
    if((result = is_legal_bishop_move(cur_index, next_index, white, board)) == -1) return -1;
    return 0;
}

int is_check(int white, const char* board){
    // checking if king can capture any piece using its legal moves
    int king_index = white ? wking_index : bking_index;
    // pawn
    if(king_index % 8 != 0 && king_index < 48){
        if(board[king_index + 7] == (white ? 'p' : 'P')){
            return 1;
        } 
    }
    if(king_index % 8 != 7 && king_index < 48){
        if(board[king_index + 9] == (white ? 'p' : 'P')){
            return 1;
        }
    }
    // rook or queen
    for(int i = king_index; i < (king_index / 8) * 8; i++){
        if(board[i] == (white ? 'r' : 'R') || board[i] == (white ? 'q' : 'Q')) return 1;
    }
    for(int i = king_index; i >= (king_index / 8 - 1) * 8; i--){
        if(board[i] == (white ? 'r' : 'R') || board[i] == (white ? 'q' : 'Q')) return 1;
    }
    for(int i = king_index; i <= 56 + (king_index % 8); i+=8){
        if(board[i] == (white ? 'r' : 'R') || board[i] == (white ? 'q' : 'Q')) return 1;
    }
    for(int i = king_index; i >= (king_index % 8); i-=8){
        if(board[i] == (white ? 'r' : 'R') || board[i] == (white ? 'q' : 'Q')) return 1;
    }
    // bishop or queen
    for(int i = king_index; i < )

}

// if -1 not legal, if 0 legal, if 1 a promotion happens, if 2 a draw happens, if 3 a win for white happens, if 4 a win for black happens
int make_move(char* board, const char* cur_pos, const char* next_pos){
    // checking are cur_pos and next_pos in valid format (e.g. E4 E5)
    if(!(tolower(cur_pos[0]) >= 'a' && tolower(cur_pos[0]) <= 'h') || !(cur_pos[1] >= '1' && cur_pos[1] <= '8') || !(tolower(next_pos[0]) >= 'a' && tolower(next_pos[0]) <= 'h') || !(next_pos[1] >= '1' && next_pos[1] <= '8')) 
        return -1;

    int cur_index = (cur_pos[1] - '1') * 8 + (tolower(cur_pos[0]) - 'a');
    int next_index = (next_pos[1] - '1') * 8 + (tolower(next_pos[0]) - 'a');

    if(cur_index == next_index) return -1;

    int result = -1;
    switch(tolower(board[cur_index])){
        case 'p':
            result = is_legal_pawn_move(cur_index, next_index, (board[cur_index] == 'P'), board);
            break;
         case 'r':
            result = is_legal_rook_move(cur_index, next_index, (board[cur_index] == 'R'), board);
            break;
         case 'n':
            result = is_legal_knight_move(cur_index, next_index, (board[cur_index] == 'N'), board);
            break;
        case 'b':
            result =  is_legal_bishop_move(cur_index, next_index, (board[cur_index] == 'B'), board);
            break;
        case 'q':
            result =  is_legal_queen_move(cur_index, next_index, (board[cur_index] == 'Q'), board);
            break;
        case 'k':
            //result = is_legal_king_move(cur_index, next_index, (board[cur_index] == 'K'), board);
            break;
        default:
            return -1;
    }
    if(result >= 0){
        board[next_index] = board[cur_index];
        board[cur_index] = '.';
    }
    return result;
}

int main(void){
    char board[64] = {'R','N','B','Q','K','B','N','R','P','P','P','P','P','P','P','P','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','p','p','p','p','p','p','p','p','r','n','b','q','k','b','n','r'};
    
    print_board(board);
    while(1){
        printf("Enter white move [Current position] [Next position]: \n");
        char cur_pos[3], next_pos[3];
        scanf("%s", cur_pos);
        scanf("%s", next_pos);
        int state = make_move(board, cur_pos, next_pos);
        printf("%d\n", state);
        while(state != 0){
            printf("The move was incorrect, enter white move again: \n");
            scanf("%s", cur_pos);
            scanf("%s", next_pos);
        }
        print_board(board);
        printf("Enter black move [Current position] [Next position]: \n");
        scanf("%s", cur_pos);
        scanf("%s", next_pos);
        state = make_move(board, cur_pos, next_pos);
        while(state != 0){
            printf("The move was incorrect, enter black move again: \n");
            scanf("%s", cur_pos);
            scanf("%s", next_pos);
        }
        print_board(board);
    }


    return 0;
}

