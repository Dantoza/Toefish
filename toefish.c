#define X 0b01
#define O 0b10
#define EMPTY 0b00
#include"include.h"
#include"functions.h"

uint8_t moves_count;
bool invalid_board = false;
uint32_t board_id = 0;//board state encoded in a base-10 integer
bool player;//true for x plays, false for o plays

int main(int argc, char *argv[]) {
    
   if (argc != 3) {
      fprintf(stderr, "Usage: %s <args> <board>\n", argv[0]);
      return 1;
   }
  
   //store the board id and check for possible errors
   char *board_end;
   board_id = strtoul(argv[2], &board_end, 10);
   if (errno == ERANGE) {
       fprintf(stderr, "Overflow: Board state is out of range for a 32-bit unsigned integer.\n");
       return 1;
   }
    if (*board_end != '\0') {
        printf("Warning: Board state contains invalid characters, using board state up to that point: %u\n", board_id);
    }

    //process the board state
    board_id=inverse(board_id);
    debug_board(board_id); // Temporary debug output
    get_data(board_id);
    if(invalid_board){
        printf("Error: Invalid board state %u.\n", board_id);
        return 1;
    }


   if (strcmp(argv[1], "-e") == 0) {

   }

   if (strcmp(argv[1], "-m") == 0) {

   }
   printf("move count: %d\n",moves_count);
   printf("%d\n",minimax(board_id,player,0));
 return 0;
}





void get_data(uint32_t board_id){//calculates important data from the board like the move counter, whose move it is, if the board state is invalid and so on
uint8_t x_moves=0;
uint8_t o_moves=0;
for(int i=0;i<9;i++){
 switch(getcell(i)){
    case X:
    ++x_moves;//i heard this is faster than x_moves++ so idk prolly wont make a difference tbh
    break;
    case O:
    ++o_moves;
    break;
 }
}
moves_count=x_moves+o_moves;
switch(x_moves-o_moves){
    case 1:
        player=false;
        return;
    case 0:
        player=true;
        return;
    default:
        invalid_board=true;
        return;
}
}
uint8_t getcell(int8_t cell){
    return (board_id>>(cell*2))&0b11;
}
void setcell(int8_t cell, uint8_t value){
    board_id=board_id -(getcell(cell)<<(cell*2));//clears the cell
    board_id=board_id +((value&0b11)<<(cell*2));// sets the new value
}
uint32_t inverse(uint32_t board_id){//basicaly turns for example 1001 into 0110-because we are reading the board id from the end to the start
    uint32_t buffer=0b00;
    for (int i=0; i<9;i++){
        buffer=buffer<<2;
        buffer=buffer+((board_id>>(i*2))&0b11);
    }
    return buffer;
        

}
int8_t is_terminal(uint32_t board_id){
    /*return values:
    0: draw
    1: X wins
    2: O wins
    3: game not finished
    */
    for (uint8_t i=0; i<9;i++) {
        if(getcell(i)==EMPTY){//check for each cell if its empty, if it is it skips and instead of returning 0(draw) exits the loop and goes to check who won the game
            goto who_won;
        }
    }
  return 0;
  who_won:
    //rows
    for (uint8_t i=0; i<3;i++){
        if((getcell(i*3)==getcell(i*3+1))&&(getcell(i*3)==getcell(i*3+2))&& (getcell(i*3)!=EMPTY)){
            return getcell(i*3);
        }
    }
    //columns
    for (uint8_t i=0; i<3;i++){
        if((getcell(i)==getcell(i+3))&&(getcell(i)==getcell(i+6))&& (getcell(i)!=EMPTY)){
            return getcell(i);
        }
    }
   //diagonals
    if((getcell(0)==getcell(4))&&(getcell(0)==getcell(8))&& (getcell(0)!=EMPTY)){
        return getcell(0);
    }
    if((getcell(2)==getcell(4))&&(getcell(2)==getcell(6))&& (getcell(2)!=EMPTY)){
        return getcell(2);
    }
    return 3;//game not finished

}

int8_t minimax(uint32_t board_id,bool player, uint8_t depth){
    int8_t bestscore;

    if (player) {
        bestscore = -11;
    } else {
        bestscore = 11;
    }
    switch (is_terminal(board_id))
    {
    case 0:
        return 0;
        break;
    case 1:
        return depth + 1;//X wins in (depth+1) moves
        break;
    case 2:
        return -(depth + 1);//O wins in (depth+1) moves
        break;
    case 3:
        break;
    }
    for(uint8_t i=0; i<9;i++){
        if(getcell(i)==EMPTY){
        //play the move
        setcell(i,player ? X:O );
        //play the opponent's move and get the score
        int8_t score=minimax(board_id,!player,depth+1);
        //update the best score
        if(player){
            bestscore=max(score,bestscore);
        }else{
            bestscore=min(score,bestscore);
        }
        //undo the move
        setcell(i,EMPTY);
        }
    }
    return bestscore;
}

// Add this temporary debug function to see what's in board state 335874
void debug_board(uint32_t board_id) {
    printf("Board state %u:\n", board_id);
    uint8_t x_moves = 0, o_moves = 0;
    
    for(int i = 0; i < 9; i++) {
        uint8_t cell = getcell(i);
        printf("Cell %d: %d ", i, cell);
        if(cell == X) {
            printf("(X)\n");
            x_moves++;
        } else if(cell == O) {
            printf("(O)\n");
            o_moves++;
        } else {
            printf("(EMPTY)\n");
        }
    }
    
    printf("X moves: %d, O moves: %d, Difference: %d\n", x_moves, o_moves, x_moves - o_moves);
}
