#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // malloc & free
#include <stdint.h> // use guaranteed 64-bit integers
#include "tokenizer.h" // Create header file and reference that
#include "memory.h" // built-in functions to read and write to a specific file

int32_t* reg; // Array of 32 32-bit registers

void init_regs();
bool interpret(char* instr);
void write_read_demo();

/**
 * Initialize register array for usage.
 * Malloc space for each 32-bit register then initializes each register to 0.
 * Do not alter this function!
 */
void init_regs(){
	int reg_amount = 32;
	reg = malloc(reg_amount * sizeof(int32_t)); // 32 * 4 bytes
	for(int i = 0; i < 32; i++)
		reg[i] = i;
}



/**
 * Fill out this function and use it to read interpret user input to execute RV64 instructions.
 * You may expect that a single, properly formatted RISC-V instruction string will be passed
 * as a parameter to this function.
 */


/*added this method from last lab to print all my tokens easily */
void print_tokens(char** tokens){
   char** temp = tokens;
   int num = 0;
   while(*temp != NULL){
      printf("Token[%d]: %s\n", num, *temp);
      num ++;  
      temp++;
   }
}
/*a method to compares 2 string character by charracter by making a temp pointer and seeing if both pointers are in the same character 
the whole time*/
bool compare_strings(char* a, char* b){
	char* temp_a = a;
	char* temp_b = b;

	while (*temp_a != '\0' || *temp_b != '\0'){

		if ((*temp_a != *temp_b))
		{
			return false;
		}

		else if (*temp_a == *temp_b){
			temp_a++;
			temp_b++;
		}
		
	}
	return true;
}
/*returns the amount of characters in a certain word*/
int get_length(char* a){
	int count = 0;
	char* temp = a;
	while (*temp != '\0'){
		temp++;
		count++;
		
	}
	return count;	
}
 //a method that will make an array of integers and will get the number values of the last 3 tokens
 // it will then use the arra of registers to add the values of 2 registers and store it in the desired register
void add(char** tokens){

	int numbers[3];

	for (int i = 1; i < 4; i++){
		if((get_length(tokens[i])) == 2){
			char temp = tokens[i][1];
			numbers[i-1] = temp - '0';
		}else if((get_length(tokens[i])) == 3){
			char temp_1 = tokens[i][1];
			char temp_2 = tokens[i][2];
			numbers[i-1] = ((temp_1 - '0') * 10) + temp_2 - '0';
		}
	}
	reg[numbers[0]] = reg[numbers[1]] + reg[numbers[2]];	
}

 //a method that will make an array of integers and will get the number values of the last 3 tokens
 // it will then use the arra of registers to add the value in a register and the integer value 
 // provided by the user and store it in the desired register
void addi(char** tokens){

	int numbers[3];
	for (int i = 1; i < 3; i++){
		if((get_length(tokens[i])) == 2){
			char temp = tokens[i][1];
			numbers[i-1] = temp - '0';
		}else if((get_length(tokens[i])) == 3){
			char temp_1 = tokens[i][1];
			char temp_2 = tokens[i][2];
			numbers[i-1] = ((temp_1 - '0') * 10) + temp_2 - '0';
		}
	}
	int temp_i = atoi(tokens[3]);
	numbers[2] = temp_i;
	reg[numbers[0]] = reg[numbers[1]] + reg[numbers[2]];
}

/*
lw x1 8(x2)

stores "8(x2)" into register x1

step 1: separate into tokens where t1 = lw; t2 = x1; t3 = 8; t4 = x2

step 2: transform t2, t3, and t4 into ints

step 3: add t3 and reg[t4]

step 4: enter result from step 3 into read_adress and tansform into 32bit int

step 5: enter 32bit int into reg[t2]

*/

// a method that gets the integer values of the tokens and will get the value from the text file and store it in
// the desired register
void lw(char** tokens){

	int numbers[3];

	for (int i = 1; i < 4; i++){
		if(i != 2){
			if((get_length(tokens[i])) == 2){
				char temp = tokens[i][1];
				numbers[i-1] = temp - '0';
			}else if((get_length(tokens[i])) == 3){
				char temp_1 = tokens[i][1];
				char temp_2 = tokens[i][2];
				numbers[i-1] = ((temp_1 - '0') * 10) + temp_2 - '0';
		}}
	}

	numbers[1] = atoi(tokens[2]);

	int32_t from_mem = numbers[1] + reg[numbers[2]];

	reg[numbers[0]] =  read_address(from_mem, "mem.txt");
}


// a method that gets the value inside the desired register and places it in the desired stop in memory
void sw(char** tokens){

	int numbers[3];

	for (int i = 1; i < 4; i++){
		if(i != 2){
			if((get_length(tokens[i])) == 2){
				char temp = tokens[i][1];
				numbers[i-1] = temp - '0';
			}else if((get_length(tokens[i])) == 3){
				char temp_1 = tokens[i][1];
				char temp_2 = tokens[i][2];
				numbers[i-1] = ((temp_1 - '0') * 10) + temp_2 - '0';
		}}
	}

	numbers[1] = atoi(tokens[2]);

	int32_t to_mem = reg[numbers[0]];

	int address = numbers[1] + numbers[2];

	write_address(to_mem, address, "mem.txt");
}


// this method makes sure the tokens are entered correctly and then proceeds to call the corresponding method for the 
// token entered
bool interpret(char* instr){
	
	if (count_tokens(instr) < 4 || count_tokens(instr) > 4){
		printf("invalid input (took many or took few tokens)\n");
		return false;
	}

	char** tokens = tokenize(instr);
	char** temp = tokens;
	
	if(compare_strings(tokens[0], "ADD")){

		if (count_tokens(instr) != 4){
			printf("doesn't have 4 tokens\n");
			return false;
		}

		if(temp[1][0] != 'X' || temp[2][0] != 'X' || temp[3][0] != 'X'){
			printf("tokens doesn't start with X\n");
			return false;
		}
		
		if(get_length(temp[1]) > 3 || get_length(temp[1]) < 2){
			printf("second token is not 2 or 3 characters long\n");
			return false;
		}
		if(get_length(temp[1]) == 3){
			char num_1 = temp[1][1];
			char num_2 = temp[1][2];
			int new_num = (num_1 - '0') * 10 + (num_2 - '0');
			printf("%d\n", new_num);
			if(new_num > 31){
				printf("second token is greater than 32\n");
				return false;
			}
		}
		add(temp);
		return true;
	}
	else if(compare_strings(tokens[0], "ADDI")){

		if (count_tokens(instr) != 4){
			printf("doesn't have 4 tokens\n");
			return false;
		}
		if(temp[1][0] != 'X' || temp[2][0] != 'X' || temp[3][0] == 'X'){
			printf("a token is wrong\n");
			return false;
		}
		if(get_length(temp[1]) > 3 || get_length(temp[1]) < 2){
			printf("second token is not 2 or 3 characters long\n");
			return false;
		}
		if(get_length(temp[1]) == 3){
			char num_1 = temp[1][1];
			char num_2 = temp[1][2];
			int new_num = (num_1 - '0') * 10 + (num_2 - '0');
			printf("%d\n", new_num);
			if(new_num > 31){
				printf("second token is greater than 32\n");
				return false;
			}
		}
		addi(temp);
		return true;
	}
	else if(compare_strings(tokens[0], "LW")){
		lw(temp);
		return true;
	}
	else if(compare_strings(tokens[0], "SW")){
		sw(temp);
		return true;
	}
	else{
		printf("invalid instruction");
	}

	return false;
	}



void print_regs(){
	int col_size = 10;
	for(int i = 0; i < 8; i++){
		printf("X%02i:%.*lld", i, col_size, (long long int) reg[i]);
		printf(" X%02i:%.*lld", i+8, col_size, (long long int) reg[i+8]);
		printf(" X%02i:%.*lld", i+16, col_size, (long long int) reg[i+16]);
		printf(" X%02i:%.*lld\n", i+24, col_size, (long long int) reg[i+24]);
	}
}


/**
 * Simple demo program to show the usage of read_address() and write_address() found in memory.c
 * Before and after running this program, look at mem.txt to see how the values change.
 * Feel free to change "data_to_write" and "address" variables to see how these affect mem.txt
 * Use 0x before an int in C to hardcode it as text, but you may enter base 10 as you see fit.
 */
void write_read_demo(){
	int32_t data_to_write = 0xFFF; // equal to 4095
	int32_t address = 0x98; // equal to 152
	char* mem_file = "mem.txt";

	// Write 4095 (or "0000000 00000FFF") in the 20th address (address 152 == 0x98)
	int32_t write = write_address(data_to_write, address, mem_file);
	if(write == (int32_t) NULL)
		printf("ERROR: Unsucessful write to address %0X\n", 0x40);
	int32_t read = read_address(address, mem_file);

	printf("Read address %lu (0x%lX): %lu (0x%lX)\n", address, address, read, read); // %lu -> format as an long-unsigned
}

/*
 * Your code goes in the main
 *
 */

int main(){
	// Do not write any code between init_regs
	init_regs(); // DO NOT REMOVE THIS LINE

	print_regs();

	// Below is a sample program to a write-read. Overwrite this with your own code.
	//write_read_demo();

	printf(" RV32 Interpreter.\nType RV32 instructions. Use upper-case letters and space as a delimiter.\nEnter 'EOF' character to end program\n");

	char* instruction = malloc(1000 * sizeof(char));
	bool is_null = false;
	// fgets() returns null if EOF is reached.
	is_null = fgets(instruction, 1000, stdin) == NULL;
	while(!is_null){
		interpret(instruction);
		printf("\n");
		print_regs();
		printf("\n");

		is_null = fgets(instruction, 1000, stdin) == NULL;
	}

	printf("Good bye!\n");

	return 0;
}