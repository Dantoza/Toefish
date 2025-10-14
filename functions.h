

int8_t is_terminal(uint32_t board_id);//checks if the game is over
uint32_t inverse(uint32_t board_id);
uint8_t getcell(int8_t cell);
void setcell(int8_t cell, uint8_t value);
int8_t minimax(uint32_t board_id, bool player, uint8_t depth);
void get_data(uint32_t board_id);//calculates important data from the board like the move counter
void debug_board(uint32_t board_id); // Temporary debug function to print the board state
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))//copied from stackoverflow