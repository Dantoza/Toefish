#define MB1 1024*1024//cuz i cant put 1MB 
#include <stdio.h>
#include <cjson/cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
char board_state[MB1] = "";
int numbers[8];//numbers for tiles
char symbols[8]; //data in tiles
void process(char *board_state);

void output(char *symbols, clock_t start){

   for (int i = 0; i < 8; i++) {
       printf("%c ", symbols[i]);
   }
   printf("\n");
   clock_t end = clock();
   double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
   printf("Execution time: %f seconds\n", cpu_time_used);
}

int main(int argc, char *argv[]) {
   
   clock_t start = clock();
   /*if (argc < 2) {
      fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
      return 1;
   }*/

   FILE *fp = fopen(argv[1], "rb");
   if (!fp) {
      perror("fopen");
      return 1;
   }
// to prevent buffer overflows i capped it at 1 MB, the board state shouldnt be bigger than like 200B so yeah
   char *buffer = malloc(MB1);
   if (!buffer) {
      perror("malloc");
      fclose(fp);
      return 1;
   }
   fseek(fp, 0, SEEK_END);
   size_t size_in_bytes = ftell(fp);
   rewind(fp);
   if(size_in_bytes > MB1) {//check for file size
      fprintf(stderr, "Maximum file size is 1 MB\n");
      free(buffer);
      fclose(fp);
      return 1;
   }
   size_t read_size = fread(buffer, 1, MB1, fp);
   if (read_size == 0 && ferror(fp)) {
      perror("fread");
      free(buffer);
      fclose(fp);
      return 1;
   }

   buffer[read_size] = '\0';// cuz counting starts at 0, i dont need to add read_size+1(off by one errors go brrrrrrrrrrrrrrrrrr)

   strcpy(board_state, buffer);
   free(buffer);
   process(board_state);
   output(symbols, start);
   fclose(fp);

   return 0;
}
 void process(char *board_state) {
    cJSON *json = cJSON_Parse(board_state);
    if (!json) {
        fprintf(stderr, "Error parsing JSON\n");
        
    }

    
    
    int count = 0;

    for (int i = 0; i < 8; i++) {
        char key[2];
        snprintf(key, sizeof(key), "%d", i);
        cJSON *item = cJSON_GetObjectItemCaseSensitive(json, key);
        if (cJSON_IsString(item) && (item->valuestring != NULL)) {
            numbers[count] = i;
            symbols[count] = item->valuestring[0]; 
            count++;
        }
    }
    cJSON_Delete(json);
}
