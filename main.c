//
// Trevor Schmidt CS_224
//
// Lab 1: Y86 Assembler
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void parseString(char* inputString, char** tokens) {
	int i = 0;
	char delim[4] = " (\t\n";
	tokens[0] = strtok(inputString,delim);

	while (tokens[i] != NULL){
		++i;
		tokens[i] = strtok(NULL, delim);
	}
}

char returnRegister(char* inputString) {
	if (strcmp(inputString, "%rax")==0) {
		return 0;
	}
	else if (strcmp(inputString, "%rcx")==0) {
		return 1;
	}
	else if (strcmp(inputString, "%rdx")==0) {
		return 2;
	}
	else if (strcmp(inputString, "%rbx")==0) {
		return 3;
	}
	else if (strcmp(inputString, "%rsp")==0) {
		return 4;
	}
	else if (strcmp(inputString, "%rbp")==0) {
		return 5;
	}
	else if (strcmp(inputString, "%rsi")==0) {
		return 6;
	}
	else if (strcmp(inputString, "%rdi")==0) {
		return 7;
	}
	else if (strcmp(inputString, "%r8")==0) {
		return 8;
	}
	else if (strcmp(inputString, "%r9")==0) {
		return 9;
	}
	else if (strcmp(inputString, "%r10")==0) {
		return 0x0a;
	}
	else if (strcmp(inputString, "%r11")==0) {
		return 0x0b;
	}
	else if (strcmp(inputString, "%r12")==0) {
		return 0x0c;
	}
	else if (strcmp(inputString, "%r13")==0) {
		return 0x0d;
	}
	else if (strcmp(inputString, "%r14")==0) {
		return 0x0e;
	}
	else {
		return 0x0f;
	}
}


int main(int argc, char** argv) {
	// Checks number of command line arguments
	if (argc != 2) {
		printf("\tInvalid number of command line arguments\n");
		printf("\tNow printing a usage error and exiting gracefully\n");
		return 1;
	}

	// Checks if input file ends in .s
	int len = strlen(argv[1]);
	char* checkName = &argv[1][len-2];
	if (strcmp(checkName, ".s")!=0) {
		printf("\tThe input file given does not end in '.s'\n");
		printf("\tNow printing a usage error and exiting gracefully\n");
		return 2;
	}

	// Creates new output file name based off input file name
	char fileName[256];
	strncpy(fileName, argv[1], len - 2);
	strcat(fileName, ".o");

	// Creates both files to read to and write from
	FILE *fp;
	FILE *fpw;
	fp = fopen(argv[1], "r");
	fpw = fopen(fileName, "w");

	// Creates and scans in the first word of the input file
	char* str1 = NULL;
	size_t buffer;

	char* tokens[4];

	size_t lineCount = 0;

	while (getline(&str1, &buffer, fp) > 0)
	{
		++lineCount;

		str1[strlen(str1) -1] = '\0';

		parseString(str1, tokens);

		if (strcmp(tokens[0], "nop")==0) {
			putc(0x10, fpw);
		}

		else if (strcmp(tokens[0], "halt")==0) {
			putc(0x00, fpw);
		}

		else if (strcmp(tokens[0], "irmovq")==0) {
			putc(0x30, fpw);

			//pushes the registers into the file
			char reg = returnRegister(tokens[2]);
			putc(reg ^ 240, fpw);

			//Takes off the $ and the , from the numbers
			tokens[1] += 1;
			tokens[1][strlen(tokens[1])-1] = 0;

			//Uses strtol to convert number into base 10
			char *ptr;
			long ret = strtol(tokens[1], &ptr, 0);

			//Writes the number to the file
			fwrite(&ret, 8, 1, fpw);
		}
		else if (strcmp(tokens[0], "rmmovq")==0) {
			// puts the first two chars into the file
			putc(0x40, fpw);

			//there are two different rmmovq's, one has a D, the other doesn't
			if(isdigit(tokens[2][0])) { //this one has a D
				// Format the register strings to not include parens or commas
				tokens[1][strlen(tokens[1])-1] = 0;
				tokens[3][strlen(tokens[3])-1] = 0;

				// grab registers, put them together, then push to file
				char reg = returnRegister(tokens[1]);
				char secReg = returnRegister(tokens[3]);
				reg <<=4;
				putc(reg ^ secReg, fpw);

				//uses strtol to convert number into base 10
				char *ptr;
				long ret = strtol(tokens[2], &ptr, 0);

				// writes the number to the file
				fwrite(&ret, 8, 1, fpw);
			}
			else { // This one doesn't have a D
				// Format the register strings to not include parens or commas
				tokens[1][strlen(tokens[1])-1] = 0;
				tokens[2][strlen(tokens[2])-1] = 0;

				// grab registers, then put them together, then push to file
				char reg = returnRegister(tokens[1]);
				char secReg = returnRegister(tokens[2]);
				reg <<=4;
				putc(reg ^ secReg, fpw);

				long ret = 0;
				fwrite(&ret, 8, 1, fpw);
			}
		}
		else if (strcmp(tokens[0], "mrmovq")==0) {
			// Puts the first two chars into the file
			putc(0x50, fpw);

			// theres two different flavors of mrmovq
			if (isdigit(tokens[1][0])) { // this has a d

				tokens[2][strlen(tokens[2])-1]=0;
				tokens[2][strlen(tokens[2])-1]=0;

				char reg = returnRegister(tokens[3]);
				char secReg = returnRegister(tokens[2]);
				reg <<=4;
				putc(reg ^ secReg, fpw);

				char *ptr;
				long ret = strtol(tokens[1], &ptr, 0);

				fwrite(&ret, 8, 1, fpw);
			}
			else { // this doesn't have a d
				tokens[1][strlen(tokens[1])-1]=0;
                                tokens[1][strlen(tokens[1])-1]=0;

				char reg = returnRegister(tokens[2]);
				char secReg = returnRegister(tokens[1]);
				reg <<=4;
				putc(reg ^ secReg, fpw);

				long ret = 0;
				fwrite(&ret, 8, 1, fpw);
			}
		}
		else if (strcmp(tokens[0], "rrmovq")==0) {
			// Puts 20 into the file for the instruction
			putc(0x20, fpw);

			// Cleans up the tokens
			tokens[1][strlen(tokens[1])-1]=0;

			// returns the regsiters
			char reg = returnRegister(tokens[1]);
			char secReg = returnRegister(tokens[2]);

			// Sets the registers
			reg <<=4;
			putc(reg ^ secReg, fpw);

		}
		else if (strcmp(tokens[0], "cmovle")==0) {
			// puts the 21 into the file
			putc(0x21, fpw);

			// cleans up the tokens
			tokens[1][strlen(tokens[1])-1]=0;

			//Returns registers
			char reg = returnRegister(tokens[1]);
			char secReg = returnRegister(tokens[2]);

			//Sets the registers
			reg <<=4;
			putc(reg ^ secReg, fpw);
		}
		else if (strcmp(tokens[0], "cmovl")==0) {
			// puts the 22 into the file
			putc(0x22, fpw);

			// cleans up the tokens
			tokens[1][strlen(tokens[1])-1]=0;

			// returns registers
			char reg = returnRegister(tokens[1]);
			char secReg = returnRegister(tokens[2]);

			//Sets the registers
			reg <<=4;
			putc(reg ^ secReg, fpw);
		}
		else if (strcmp(tokens[0], "cmove")==0) {
			// puts the 23 into the file
			putc(0x23, fpw);

			//cleans up the tokens
			tokens[1][strlen(tokens[1])-1]=0;

			//returns registers
                        char reg = returnRegister(tokens[1]);
                        char secReg = returnRegister(tokens[2]);

                        //Sets the registers
                        reg <<=4;
                        putc(reg ^ secReg, fpw);
		}
		else if (strcmp(tokens[0], "cmovne")==0) {
                        // puts the 25 into the file
                        putc(0x24, fpw);

                        //cleans up the tokens
                        tokens[1][strlen(tokens[1])-1]=0;

                        //returns registers
                        char reg = returnRegister(tokens[1]);
                        char secReg = returnRegister(tokens[2]);

                        //Sets the registers
                        reg <<=4;
			//putc(reg, fpw);
			//putc(secReg, fpw);
                        putc(reg ^ secReg, fpw);
		}
		else if (strcmp(tokens[0], "cmovge")==0) {
                        // puts the 25 into the file
                        putc(0x25, fpw);

                        //cleans up the tokens
                        tokens[1][strlen(tokens[1])-1]=0;

                        //returns registers
                        char reg = returnRegister(tokens[1]);
                        char secReg = returnRegister(tokens[2]);

                        //Sets the registers
                        reg <<=4;
                        putc(reg ^ secReg, fpw);
		}
		else if (strcmp(tokens[0], "cmovg")==0) {
                        // puts the 26 into the file
                        putc(0x26, fpw);

                        //cleans up the tokens
                        tokens[1][strlen(tokens[1])-1]=0;

                        //returns registers
                        char reg = returnRegister(tokens[1]);
                        char secReg = returnRegister(tokens[2]);

                        //Sets the registers
                        reg <<=4;
                        putc(reg ^ secReg, fpw);
		}
		else if (strcmp(tokens[0], "addq")==0) {
			// puts the 60 into the file
                        putc(0x60, fpw);

                        //cleans up the tokens
                        tokens[1][strlen(tokens[1])-1]=0;

                        //returns registers
                        char reg = returnRegister(tokens[1]);
                        char secReg = returnRegister(tokens[2]);

                        //Sets the registers
                        reg <<=4;
                        putc(reg ^ secReg, fpw);
		}
		else if (strcmp(tokens[0], "subq")==0) {
                        // puts the 61 into the file
                        putc(0x61, fpw);

                        //cleans up the tokens
                        tokens[1][strlen(tokens[1])-1]=0;

                        //returns registers
                        char reg = returnRegister(tokens[1]);
                        char secReg = returnRegister(tokens[2]);

                        //Sets the registers
                        reg <<=4;
                        putc(reg ^ secReg, fpw);
		}
		else if (strcmp(tokens[0], "andq")==0) {
                        // puts the 62 into the file
                        putc(0x62, fpw);

                        //cleans up the tokens
                        tokens[1][strlen(tokens[1])-1]=0;

                        //returns registers
                        char reg = returnRegister(tokens[1]);
                        char secReg = returnRegister(tokens[2]);

                        //Sets the registers
                        reg <<=4;
                        putc(reg ^ secReg, fpw);
		}
		else if (strcmp(tokens[0], "xorq")==0) {
                        // puts the 63 into the file
                        putc(0x63, fpw);

                        //cleans up the tokens
                        tokens[1][strlen(tokens[1])-1]=0;

                        //returns registers
                        char reg = returnRegister(tokens[1]);
                        char secReg = returnRegister(tokens[2]);

                        //Sets the registers
                        reg <<=4;
                        putc(reg ^ secReg, fpw);
		}

		else {
			printf("Invalid instruction found on line %ld.\n", lineCount);
		}
	}

	// Closes the two files and frees the string
	fclose(fp);
	fclose(fpw);
	free(str1);
	return 0;
}


