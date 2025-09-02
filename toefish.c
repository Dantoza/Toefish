#define MB1 1024*1024//cuz i cant put 1MB 
#include <stdio.h>
#include <cjson/cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>
#include <stdint.h>
char board_state[MB1] = "";
char winner = ' ';//declares if x wins,o wins or its a draw(_)
uint8_t move_count = 0;//uses 8 bits to store the move count instead of default 32 bits
char symbols[9]; //data in tiles
bool is_terminal(char *parsed_board_state);//checks if the game is finished
char *minimax(char *parsed_board_state, char current_player, int depth);//minimax algorithm
void parse(char *board_state);//turns a json into an array
char turn(char *parsed_board_state);//checks whose turn it is and the current move number

void output(char *symbols, clock_t start){

   for (int i = 0; i < 9; i++) {
       printf("%d : %c ",i, symbols[i]);
   }
   printf("\n");
   clock_t end = clock();
   double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
   printf("Execution time: %f seconds\n", cpu_time_used);
}
int main(int argc, char *argv[]) {
   clock_t start = clock();
   if (argc < 2) {
      fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
      return 1;
   }
   FILE *fp = fopen(argv[1], "rb");//apparently rb and r work the same on linux but not on windows and rb is safer for cross-platform
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
   if (read_size == 0 && ferror(fp)) {
      perror("fread");
      fclose(fp);
      return 1;
   }
   board_state[read_size] = '\0';//null-terminate the string, read_size is the size of the string
   fclose(fp);//and since we count from 0 we can use that as a part of an array index(of by one error go brrrrrrrrrrrrrrrrr)
   parse(board_state);
   //JSON is parsed correctly, time for the minimax algorithm
   char *result = minimax(symbols, turn(symbols), move_count);
   printf("Minimax result: %s\n", result);
   output(symbols, start);
   return 0;
}
 void parse(char *board_state) {
    cJSON *json = cJSON_Parse(board_state);
    if (!json) {
        fprintf(stderr, "Error parsing JSON\n");
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

char *minimax(char *parsed_board_state, char current_player, int depth) {
   //terminal-state check
   char *new_board_state = NULL;
   if(is_terminal(parsed_board_state)){
    switch(winner)
    {
    case 'X':
        return "X";
        break;
    case 'O':
        return "O"; 
        break;
    case '_':
        return "_";
        break;
    }

}
}
bool is_terminal(char *parsed_board_state) {
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
char turn(char *parsed_board_state) {
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

    }
    move_count = x_count + o_count;
    if (x_count > o_count) {//yes here would be a good place to put a check if the board is valid but whatever ill do it later
        return 'O';
    } else {
        return 'X';
    }
}
