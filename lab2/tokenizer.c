#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
/* Return true (non-zero) if c is a whitespace characer
   ('\t' or ' ').
   Zero terminators are not printable (therefore false) */
bool delim_character(char c){
   if(c == ' ' || c == '\t' || c == '\n' || c == '(' || c == ')'){
      return true;
   }
   return false;
}

/* Return true (non-zero) if c is a non-whitespace
   character (not tab or space).
   Zero terminators are not printable (therefore false) */
bool non_delim_character(char c){
   if(c == ' ' || c == '\t' || c == '\0' || c == '\n' || c == ')' || c == '(') {
      return false;
   }
   return true;
}

/* Returns a pointer to the first character of the next
   space-separated word*/
char *word_start(char* str){
   while(delim_character(*str)){
      str++;
   }
   return str;
}

/* Returns a pointer to the first space character of the zero
terminated string*/
char *end_word(char* str){
   while(non_delim_character(*str)){
      str++;
   }
   return str;
}
// counts the number of words or tokens
int count_tokens(char* str){
   int count = 0; 
   char* temp = str;
   temp = word_start(temp);
   while (*temp != '\0'){
      temp = end_word(temp);
      temp = word_start(temp);
      count = count + 1;
   }
   return count;
}
/* Returns a freshly allocated zero-terminated vector of freshly allocated
   space-separated tokens from zero-terminated str.
   For example, tokenize("hello world string") would result in:
     tokens[0] = "hello"
     tokens[1] = "world"
     tokens[2] = "string"
     tokens[3] = 0
*/
char *copy_str(char *inStr, short len){
   char* token = (char*) malloc(len*sizeof(char*));
   for (int i = 0; i < len; i++){
      token[i] = inStr[i];
   }
   token[len] = '\0';
   return token;
}

char** tokenize(char* str){
   int size = count_tokens(str);
   char** tokens = (char**) malloc(size * sizeof(char**)+1);
   char* start = word_start(str);
   char* end = end_word(start);

   for(int i = 0; i < size; i++){
      tokens[i] = copy_str(start, end - start);
      start = word_start(end);
      end = end_word(start);
   }
   return tokens;
}


void print_all_tokens(char** tokens){
   char** temp = tokens;
   int num = 0;
   while(*temp != NULL){
      printf("Token[%d]: %s\n", num, *temp);
      num ++;  
      temp++;
   }
}




