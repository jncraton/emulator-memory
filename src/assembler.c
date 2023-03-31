#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "emulator.h"

void assemble(char* code, int32_t *memory) {
  Instruction * instruction;

  char tokens[4][16];
  int current_instruction = 0;

  char *line = strtok(code, "\n");

  // Iterate over lines in the file
  while (line) {
    // Read opcodes and operands
    memset(tokens, 0, sizeof(tokens));
    sscanf(line, "%15s %15s %15s %15s", tokens[0], tokens[1], tokens[2], tokens[3]);
    line = strtok(NULL, "\n");

    // Handle comments and blank lines
    if (tokens[0][0] == ';' || tokens[0][0] == '\n' || tokens[0][0] == '\0') {
      continue;
    }

    // If we reach this point, We are expecting a valid instruction
    instruction = (Instruction*)(memory + current_instruction);
    instruction->op = Invalid;

    // Search valid opcode names
    for (int i = 0; i<Invalid; i++) {
      if (!strcmp(instruction_names[i], tokens[0])) {
        // TODO: Set the instruction op after finding a matching name
        instruction->op = i;
      }
    }

    if (instruction->op == Invalid) {
      printf("Invalid opcode for instruction %d (%s)\n", current_instruction, tokens[0]);
      exit(1);
    }

    // Process operands
    // Find register values
    if (tokens[1][0] == 'r') {
      instruction->dst = atoi(tokens[1]+1);
      instruction->src = atoi(tokens[1]+1);
    }
    if (tokens[2][0] == 'r') {
      instruction->src = atoi(tokens[2]+1);
    }

    // Find immediate value if present
    for (int i = 1; i <= 3; i++) {
      if (tokens[i][0] != '\0' && tokens[i][0] != 'r') {
        // TODO: Set the instruction immediate value
        instruction->immediate = atoi(tokens[i]);
      }
    }

    current_instruction++;
  }
}

int main(int argc, char ** argv) {
  if (argc != 4 || argv[2][0] != '-' || argv[2][1] != 'o') {
    printf("Usage: assember {infile} -o {outfile}\n");
    exit(1);
  }

  printf("Assembling %s to %s\n", argv[1], argv[3]);

  int32_t binary[256];
  char source[10000];
  FILE * input = fopen(argv[1], "rb");
  fread(source, 1, 10000, input);
  fclose(input);

  memset(binary, 0, sizeof(binary));
  assemble(source, binary);

  FILE * output = fopen(argv[3], "wb");
  fwrite(binary, 4, 256, output);
  fclose(output);

  printf("Done\n");
}