#define MB1 1024*1024//cuz i cant put 1MB 
#include <stdio.h>//standard input output
#include <cjson/cJSON.h>//cJSON library for manipulating JSONs
#include <stdlib.h>//standard C library(functions like malloc, free, printf,...)
#include <string.h>//string manipulation
#include <stdbool.h>//boolean type
#include <time.h>//time manipulation
#include <errno.h>//error handling
#include <stdint.h>//fixed width integers for smaller memory footprint
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))//copied from stackoverflow
char board_state[MB1] = "";
uint8_t moves_count;
char winner = ' ';//declares if x wins,o wins or its a draw(_)
bool invalid_board = false;//self-explanatory
char symbols[9]; //data in tiles
bool is_terminal(char *parsed_board_state);//checks if the game is finished
int minimax(char *parsed_board_state, bool current_player, int depth);//minimax algorithm
void parse(char *board_state);//turns a json into an array
bool turn(char *parsed_board_state);//checks whose turn it is and the current move number
int next_move(char *parsed_board_state, bool current_player);

void output(char *symbols, clock_t start){

   for (int i = 0; i < 9; i++) {
       printf("%d : %c ",i, symbols[i]);
   }
   printf("\n");
   clock_t end = clock();
   double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC*1000;
   printf("Execution time: %f milliseconds\n", cpu_time_used);
}
int main(int argc, char *argv[]) {
   clock_t start = clock();
   if (argc != 3) {
      fprintf(stderr, "Usage: %s <args> <file_path>\n", argv[0]);
      return 1;
   }
   FILE *fp = fopen(argv[2], "rb");//apparently rb and r work the same on linux but not on windows and rb is safer for cross-platform
   if (!fp) {
      perror("fopen");
      return 1;
   }
   fseek(fp, 0, SEEK_END);
   size_t size_in_bytes = ftell(fp);
   rewind(fp);
   if(size_in_bytes > MB1) {//check for file size
      fprintf(stderr, "Maximum file size is 1 MB\n");
      fclose(fp);
      return 1;
   }
   size_t read_size = fread(board_state, 1, MB1, fp);
   if (read_size == 0 || ferror(fp)) {
      perror("fread");
      fclose(fp);
      return 1;
   }
   board_state[read_size] = '\0';//null-terminate the string, read_size is the size of the string
   fclose(fp);//and since we count from 0 we can use that as a part of an array index(of by one error go brrrrrrrrrrrrrrrrr)
   parse(board_state);
   //JSON is parsed correctly, time for the minimax algorithm
switch (argv[1][1]) {
    case 'e':
        if (strcmp(argv[1], "-e") == 0) {
            
            int8_t score = minimax(symbols, turn(symbols), moves_count);
            if(score==0){printf("Forced draw\n");}else{
                printf("Mate in: %d for %c\n ", 10-abs(score), score > 0 ? 'X' : 'O');
            }
            return 0;
            break;
        }
        
    case 'm':
        if (strcmp(argv[1], "-m") == 0) {
            int move = next_move(symbols, turn(symbols));
            printf("Best move on tile : %d\n", move);
            return 0;
            break;
        }
        
    default:
        fprintf(stderr, "Unknown argument: %s\n", argv[1]);
        return 1;
}
   output(symbols, start);
   return 0;
}
 void parse(char *board_state) {
    cJSON *json = cJSON_Parse(board_state);
    if (!json) {
        printf("Error parsing JSON\n");
        perror("cJSON_Parse");
        return;
    }
    for (int i = 0; i < 9; i++) {
        char key[2];
        snprintf(key, sizeof(key), "%d", i);
        cJSON *item = cJSON_GetObjectItemCaseSensitive(json, key);
        if (cJSON_IsString(item) && (item->valuestring != NULL)) {
           
            symbols[i] = item->valuestring[0]; 
        } else {
            fprintf(stderr, "Error: Invalid JSON format at key %d, using fallback value (empty tile)\n", i);
            symbols[i] = '_'; // fallback
        }
    }
    cJSON_Delete(json);

}

int minimax(char *parsed_board_state, bool current_player, int depth) {//this is a minimax algorithm in c and can evaluate the board 
                                                                        //(how many moves are left till the game ends)
    //terminal-state check
    if (is_terminal(parsed_board_state)) {
        if (winner == 'X') {
            return 10 - depth;
        } else if (winner == 'O') {
            return depth - 10;
        } else if (winner == '_') {
            return 0;
        }
    }
    if (invalid_board) {
        return -11;//impossible to get when evaluating, so ill use this when getting an error
    }

    
        int best_move = current_player ? -11 : 11;
        for (int i = 0; i < 9; i++) {
            if (parsed_board_state[i] == '_') {
                parsed_board_state[i] = current_player ? 'X' : 'O';
                int score = minimax(parsed_board_state, !current_player, depth + 1);
                parsed_board_state[i] = '_';
                best_move = (current_player) ? max(best_move, score) : min(best_move, score);
            }
        }
        return best_move;

}
bool is_terminal(char *parsed_board_state) {//subfunc of minimax
    static const uint8_t winning_pos[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8},//rows
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8},//columns
        {0, 4, 8}, {2, 4, 6} //diagonals
        //its easier to hardcode these than to calculate all possible winning positions, also faster for the program not just for me
    };

    for (size_t i = 0; i < 8; i++) {
        char s = parsed_board_state[winning_pos[i][0]];
        if (s != '_' &&
            s == parsed_board_state[winning_pos[i][1]] &&
            s == parsed_board_state[winning_pos[i][2]]) {
            winner = s;
            return true;
        }
    }

    if (!strchr(parsed_board_state, '_')) {
        winner='_';
        return true;
    }
    return false;
}
bool turn(char *parsed_board_state) {//subfunc of minimax
    int x_count = 0;
    int o_count = 0;
    for (int i = 0; i < 9; i++) {
        switch (parsed_board_state[i]) {
            case 'X':
            x_count++;
            break;
            case 'O':
            o_count++;
            break;
            default:
            break;
        }
        moves_count=x_count+o_count;

    }
    switch(x_count-o_count){
        case 1:
        return false;
        case 0:
        return true;
        default:
        invalid_board = true;
        return false;


    }
}
int next_move(char *parsed_board_state, bool current_player) {
    int best_move = -1;
    int best_score = current_player ? -11 : 11;
    for(int i=0; i<9; i++){
        if (parsed_board_state[i] == '_') {
            parsed_board_state[i] = current_player ? 'X' : 'O';
            int score = minimax(parsed_board_state, !current_player, 0);
            parsed_board_state[i] = '_';
            if ((current_player && score > best_score) || (!current_player && score < best_score)) {//yes i know nesting ill fix it later
                best_score = score;
                best_move = i;
            }
        }
    }
    return best_move;
}