
#define X 0b01
#define O 0b10
#define EMPTY 0b00
#include <stdio.h>//standard input output
#include <stdlib.h>//standard C library(functions like malloc, free, printf,...)
#include <string.h>//string manipulation
#include <stdbool.h>//boolean type
#include <errno.h>//error handling
#include <stdint.h>//fixed width integers for smaller memory footprint
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))//copied from stackoverflow
uint8_t moves_count;
uint8_t winner;
bool invalid_board = false;
uint32_t board_id = 0;//board state encoded in a base-10 integer
int8_t is_terminal(uint32_t board_id);//checks if the game is over
uint32_t inverse(uint32_t board_id);
uint8_t getcell(int8_t cell);
void setcell(int8_t cell, uint8_t value);
uint8_t minimax(uint32_t board_id, uint8_t player, uint8_t depth);
int main(int argc, char *argv[]) {
    
   if (argc != 3) {
      fprintf(stderr, "Usage: %s <args> <board>\n", argv[0]);
      return 1;
   }
  
   
   char *board_end;
   board_id = strtoul(argv[2], &board_end, 10);
   if (errno == ERANGE) {
       fprintf(stderr, "Overflow: Board state is out of range for a 32-bit unsigned integer.\n");
       return 1;
   }
    if (*board_end != '\0') {
        printf("Warning: Board state contains invalid characters, using board state up to that point: %u\n", board_id);
    }
    board_id=inverse(board_id);
    
   if (strcmp(argv[1], "-e") == 0) {

   }

   if (strcmp(argv[1], "-m") == 0) {

   }
   printf("%d\n",is_terminal(board_id));

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
bool who_plays(){//checks if the board state is valid and whose move it is
    uint8_t x_moves;
    uint8_t o_moves;
    for(uint8_t i=0;i<9;i++){
        switch(getcell(i)){
            case X:++x_moves; break;
            case O:++o_moves;break;
        }
    }
    moves_count=x_moves+o_moves;
    return (o_moves) < (x_moves) ? (false) : (true);
    
}
uint8_t minimax(uint32_t board_state,uint8_t player, uint8_t depth){
    uint8_t player=who_plays();
    uint8_t bestscore;
    switch(player){
        case X:
        bestscore=-11;
        break;
        case O:
        bestscore=11;
        break;

    }
    switch (is_terminal(board_state))
    {
    case 0:
        return 0;
        break;
    case 1:
        return 10-depth;//the algorithm to win faster, so it wants to minimize the depth
        break;
    case 2:
        return depth-10;//the algorithm to make the opponent lose slower, so it wants to maximize the depth
        break;
    case 3:
        break;
    }
    for(uint8_t i=0; i<9;i++){
        setcell(i,player);
        
        uint8_t score=minimax(board_id,player,moves_count);
        setcell(i,EMPTY);
    }
}
