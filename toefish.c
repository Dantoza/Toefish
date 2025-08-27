#define MB1 1024*1024//cuz i cant put 1MB 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
char board_state[MB1] = "";
char *process(char *board_state);
void output(char *board_state, clock_t start){

   printf("%s\n", board_state);
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

   FILE *fp = fopen(argv[1], "rb");
   if (!fp) {
      perror("fopen");
      return 1;
   }

   const size_t board = MB1; // to prevent buffer overflows i capped it at 1 MB, the board state shouldnt be bigger than like 200B so yeah
   char *buffer = malloc(board);
   if (!buffer) {
      perror("malloc");
      fclose(fp);
      return 1;
   }

   size_t read_size = fread(buffer, 1, board, fp);
   if (read_size == 0 && ferror(fp)) {
      perror("fread");
      free(buffer);
      fclose(fp);
      return 1;
   }

   buffer[read_size] = '\0';// cuz counting starts at 0, i dont need to add read_size+1

   strcpy(board_state, buffer);
   strcpy(board_state, process(board_state));
   output(board_state, start);
   free(buffer);
   fclose(fp);

   return 0;
}
char *process(char *board_state) {
   // Process the board state
   return board_state;
}
