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
    get_data(board_id);
    if(invalid_board){
        printf("Error: Invalid board state %u.\n", inverse(board_id));
        return 1;
    }


   if (strcmp(argv[1], "-e") == 0) {

   }

   if (strcmp(argv[1], "-m") == 0) {

   }

   printf("move count: %d\n",moves_count);
   printf("%d\n",minimax(board_id,player,0));
   printf("board id: %u\n",board_id);
   printf("mate in ");
 return 0;
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
    // not draw
    for(uint8_t i=0; i<9;i++){
        if(getcell(i)==EMPTY){
            return 3;
        }
    }
    return 0;

}
void get_data(uint32_t board_id){
    uint8_t x_counter=0;
    uint8_t o_counter=0;
    for(uint8_t i=0; i<9;i++){
        switch(getcell(i)){
            case X:
                x_counter++;
                break;
            case O:
                o_counter++;
                break;
            case EMPTY:
                break;
            default:
                invalid_board=true;
                return;
        }
    }
    switch (x_counter-o_counter)
    {
    case 0:
        player=true;//x plays
        break;
    case 1:
        player=false;//o plays
        break;
    default:
        invalid_board=true;
        return;
        break;
    
}
moves_count=x_counter+o_counter;
}
int8_t minimax(uint32_t board_id,bool player, uint8_t depth){
    int8_t bestscore=0;

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
        return 10-depth;
        break;
    case 2:
     return depth-10;
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
