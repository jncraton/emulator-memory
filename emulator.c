#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define registers
typedef enum {R0, R1, R2, R3, R4, IP, FLAGS}  __attribute__ ((__packed__)) Register;

// Define operations
typedef enum {Halt=0, LoadImmediate, Add, AddImmediate, And, Or, Xor, Jump, BranchOnEqual, Move, LoadDirect, StoreDirect, LoadIndirect, StoreIndirect, Invalid}  __attribute__ ((__packed__)) Operation;

// Define plain-text instruction names for use in assembler
char* instruction_names[14] = {"Halt", "LoadImmediate", "Add", "AddImmediate", "And", "Or", "Xor", "Jump", "BranchOnEqual", "Move", "LoadDirect", "StoreDirect", "LoadIndirect", "StoreIndirect"};

// Define instruction format
typedef struct {
  Operation op;
  Register src;
  Register dst;
  signed char immediate;
} Instruction;

// Helper function to print register contents for debugging
void print_registers(int * registers) {
  printf("IP: %d ", registers[IP]);
  for (int i = 0; i<5; i++) {
    printf("R%d: %d ", i, registers[i]);
  }
  printf("\n");
}

// Helper function to print instruction information for debugging
void print_instruction(Instruction inst) {
  printf("%s src=%d dst=%d imm=%d\n", instruction_names[inst.op], inst.src, inst.dst, inst.immediate);
}

void execute(int32_t * memory, int * registers) {
  Instruction inst = ((Instruction*)memory)[registers[IP]];

  while (inst.op != Halt) {
    if (inst.op == LoadImmediate) {
      registers[inst.dst] = inst.immediate;
    } else if (inst.op == Jump) {
      registers[IP] = inst.immediate-1;
    } else if (inst.op == BranchOnEqual) {
      if (registers[inst.src] == registers[inst.dst]) {
        registers[IP] = inst.immediate-1;
      }
    } else if (inst.op == Add) {
      registers[inst.dst] = registers[inst.src] + registers[inst.dst];
    } else if (inst.op == Move) {
      registers[inst.dst] = registers[inst.src];
    } else if (inst.op == And) {
      registers[inst.dst] = registers[inst.src] & registers[inst.dst];
    } else if (inst.op == Or) {
      registers[inst.dst] = registers[inst.src] | registers[inst.dst];
    } else if (inst.op == Xor) {
      registers[inst.dst] = registers[inst.src] ^ registers[inst.dst];
    } else if (inst.op == AddImmediate) {
      registers[inst.dst] = registers[inst.src] + inst.immediate;
    }

    registers[IP]++;
    inst = ((Instruction*)memory)[registers[IP]];
  }
}

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
    if (tokens[0][0] == ';' || tokens[0][0] == '\n') {
      continue;
    }

    // If we reach this point, We are expecting a valid instruction
    instruction = (Instruction*)(memory + current_instruction);
    instruction->op = Invalid;

    // Process opcode
    for (int i = 0; i<Invalid; i++) {
      if (!strcmp(instruction_names[i], tokens[0])) {
        instruction->op = i;
      }
    }

    if (instruction->op == Invalid) {
      printf("Invalid opcode for instruction %d\n", current_instruction);
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
        instruction->immediate = atoi(tokens[i]);      
      }
    }

    current_instruction++;
  }
}

/* 
Test code 

It is not recommended to modify any code below this point
*/

#define test_int_equal(exp, result) \
printf("Test %s == %d ", #exp, result); \
if ((exp) != result) { \
  printf("FAILED as %d.\n", exp); \
} else { \
  printf("succeeded.\n"); \
}

#define test_float_equal(exp, result) \
printf("Test %s == %f ", #exp, result); \
if ((exp) != result) { \
  printf("FAILED as %f.\n", exp); \
} else { \
  printf("succeeded.\n"); \
}

int main() {
  printf("Halt Tests\n");
  Instruction halt_instructions[4]={
    {Halt, 0, 0, 0}
  };
  int registers[6] = {0,0,0,0,0,0};
  execute((void*)halt_instructions, registers);
  test_int_equal(registers[IP], 0);
  test_int_equal(registers[R0], 0);
  test_int_equal(registers[R1], 0);
  test_int_equal(registers[R2], 0);

  printf("Load Immediate Tests\n");
  Instruction load_imm_instructions[4]={
    {LoadImmediate, 0, R0, 1},
    {LoadImmediate, 0, R1, 80},
    {LoadImmediate, 0, R2, 13},
    {Halt, 0, 0, 0}
  };
  memset(registers, 0, sizeof(registers));
  execute((void*)load_imm_instructions, registers);
  test_int_equal(registers[IP], 3);
  test_int_equal(registers[R0], 1);
  test_int_equal(registers[R1], 80);
  test_int_equal(registers[R2], 13);

  printf("Add Tests\n");
  Instruction add_instructions[4]={
    {LoadImmediate, 0, R0, 42},
    {LoadImmediate, 0, R1, 100},
    {Add, R0, R1, 0},
    {Halt, 0, 0, 0}
  };
  memset(registers, 0, sizeof(registers));
  execute((void*)add_instructions, registers);
  test_int_equal(registers[IP], 3);
  test_int_equal(registers[R0], 42);
  test_int_equal(registers[R1], 142);
  test_int_equal(registers[R2], 0);

  printf("Add Immediate Tests\n");
  Instruction addi_instructions[6]={
    {AddImmediate, R0, R0, 1},
    {AddImmediate, R0, R0, 2},
    {AddImmediate, R1, R1, 21},
    {AddImmediate, R1, R1, 30},
    {AddImmediate, R1, R2, 1},
    {Halt, 0, 0, 0}
  };
  memset(registers, 0, sizeof(registers));
  execute((void*)addi_instructions, registers);
  test_int_equal(registers[IP], 5);
  test_int_equal(registers[R0], 3);
  test_int_equal(registers[R1], 51);
  test_int_equal(registers[R2], 52);

  printf("Bitwise Tests\n");
  Instruction bitwise_instructions[9]={
    {LoadImmediate, 0, R0, 1},
    {LoadImmediate, 0, R1, 1},
    {And, R0, R1, 0},
    {LoadImmediate, 0, R2, 0},
    {Or, R0, R2, 0},
    {LoadImmediate, 0, R3, 0},
    {Xor, R0, R3, 0},
    {Xor, R0, R3, 0},
    {Halt, 0, 0, 0}
  };
  memset(registers, 0, sizeof(registers));
  execute((void*)bitwise_instructions, registers);
  test_int_equal(registers[IP], 8);
  test_int_equal(registers[R0], 1);
  test_int_equal(registers[R1], 1);
  test_int_equal(registers[R2], 1);
  test_int_equal(registers[R3], 0);


  printf("Jump Tests\n");
  Instruction jump_instructions[4]={
    {LoadImmediate, 0, R0, 1},
    {Jump, 0, 0, 3},
    {LoadImmediate, 0, R1, 2},
    {Halt, 0, 0, 0}
  };
  memset(registers, 0, sizeof(registers));
  execute((void*)jump_instructions, registers);
  test_int_equal(registers[IP], 3);
  test_int_equal(registers[R0], 1);
  test_int_equal(registers[R1], 0);
  test_int_equal(registers[R2], 0);

  printf("BranchOnEqual equal test\n");
  Instruction beq_instructions[5]={
    {LoadImmediate, 0, R0, 1},
    {LoadImmediate, 0, R1, 1},
    {BranchOnEqual, R0, R1, 4},
    {Halt, 0, 0, 0},
    {Halt, 0, 0, 0}
  };
  memset(registers, 0, sizeof(registers));
  execute((void*)beq_instructions, registers);
  test_int_equal(registers[IP], 4);
  test_int_equal(registers[R0], 1);
  test_int_equal(registers[R1], 1);
  test_int_equal(registers[R2], 0);

  printf("BranchOnEqual not equal Test\n");
  Instruction beq2_instructions[5]={
    {LoadImmediate, 0, R0, 1},
    {LoadImmediate, 0, R1, 2},
    {BranchOnEqual, R0, R1, 4},
    {Halt, 0, 0, 0},
    {Halt, 0, 0, 0}
  };
  memset(registers, 0, sizeof(registers));
  execute((void*)beq2_instructions, registers);
  test_int_equal(registers[IP], 3);
  test_int_equal(registers[R0], 1);
  test_int_equal(registers[R1], 2);
  test_int_equal(registers[R2], 0);

  printf("Move Tests\n");
  Instruction move_instructions[5]={
    {LoadImmediate, 0, R0, 42},
    {LoadImmediate, 0, R1, 100},
    {Move, R0, R2, 0},
    {Move, R1, R0, 0},
    {Halt, 0, 0, 0}
  };
  memset(registers, 0, sizeof(registers));
  execute((void*)move_instructions, registers);
  test_int_equal(registers[IP], 4);
  test_int_equal(registers[R0], 100);
  test_int_equal(registers[R1], 100);
  test_int_equal(registers[R2], 42);

  printf("Extended Tests\n");
  Instruction ext_instructions[11]={
    {LoadImmediate, 0, R0, 1},
    {LoadImmediate, 0, R1, 12},
    {LoadImmediate, 0, R2, 100},
    {Add, R0, R1, 0},
    {Add, R2, R1, 0},
    {Add, R2, R1, 0},
    {LoadImmediate, 0, R0, -12},
    {Add, R0, R1, 0},
    {LoadImmediate, 0, R0, 1},
    {LoadImmediate, 0, R2, 3},
    {Halt, 0, 0, 0}
  };
  memset(registers, 0, sizeof(registers));
  execute((void*)ext_instructions, registers);
  test_int_equal(registers[IP], 10);
  test_int_equal(registers[R0], 1);
  test_int_equal(registers[R1], 201);
  test_int_equal(registers[R2], 3);

  printf("Loop Test\n");
  Instruction loop_instructions[11]={
    {LoadImmediate, 0, R0, 0}, // Stays zero
    {LoadImmediate, 0, R1, -1}, // Stays -1
    {LoadImmediate, 0, R2, 11}, // Loop counter
    {LoadImmediate, 0, R3, 3}, // Stays 3
    {LoadImmediate, 0, R4, 0}, // Counting by 3s
    // Decrement loop counter
    {Add, R1, R2, 0}, // Instruction 5
    {BranchOnEqual, R2, R0, 9}, // Exit loop
    // Increment output
    {Add, R3, R4, 0},
    {Jump, 0, 0, 5},
    {Halt, 0, 0, 0}
  };
  memset(registers, 0, sizeof(registers));
  execute((void*)loop_instructions, registers);
  test_int_equal(registers[IP], 9);
  test_int_equal(registers[R0], 0);
  test_int_equal(registers[R1], -1);
  test_int_equal(registers[R2], 0);
  test_int_equal(registers[R3], 3);
  test_int_equal(registers[R4], 30);

  printf("Assembler Test\n");
  char program[10000];

  FILE * f = fopen("test-program.asm", "r");
  fread(program, 1, 10000, f);

  int32_t memory[256];
      
  memset(memory, 0, sizeof(memory));
  memset(registers, 0, sizeof(registers));
  assemble(program, memory);
  execute(memory, registers);
  test_int_equal(registers[IP], 9);
  test_int_equal(registers[R0], 0);
  test_int_equal(registers[R1], -1);
  test_int_equal(registers[R2], 0);
  test_int_equal(registers[R3], 3);
  test_int_equal(registers[R4], 30);

  printf("Fibonacci Test\n");

  f = fopen("fibonacci.asm", "r");
  fread(program, 1, 10000, f);
  
  memset(memory, 0, sizeof(memory));
  memset(registers, 0, sizeof(registers));
  assemble(program, memory);

  registers[R0] = 0;
  execute(memory, registers);
  test_int_equal(registers[R0], 0);

  memset(registers, 0, sizeof(registers));
  registers[R0] = 1;
  execute(memory, registers);
  test_int_equal(registers[R0], 1);

  memset(registers, 0, sizeof(registers));
  registers[R0] = 2;
  execute(memory, registers);
  test_int_equal(registers[R0], 1);

  memset(registers, 0, sizeof(registers));
  registers[R0] = 3;
  execute(memory, registers);
  test_int_equal(registers[R0], 2);

  memset(registers, 0, sizeof(registers));
  registers[R0] = 20;
  execute(memory, registers);
  test_int_equal(registers[R0], 6765);

  printf("Memory Test\n");

  f = fopen("memory-test.asm", "r");
  memset(program, 0, 10000);
  fread(program, 1, 10000, f);
  
  memset(memory, 0, sizeof(memory));
  memset(registers, 0, sizeof(registers));
  assemble(program, memory);

  execute(memory, registers);
  test_int_equal(registers[R0], LoadDirect);
  test_int_equal(memory[101], 71);
  test_int_equal(memory[102], -19);
  test_int_equal(memory[103], 37);
  test_int_equal(memory[104], -18);

  printf("Projectile Test\n");

  f = fopen("projectile.asm", "r");
  memset(program, 0, 10000);
  fread(program, 1, 10000, f);
  
  memset(memory, 0, sizeof(memory));
  memset(registers, 0, sizeof(registers));
  assemble(program, memory);

  memory[102] = 70; // vx in m/s
  memory[103] = 70; // vy in m/s
  memory[105] = -10; // ay in m/s^2

  execute(memory, registers);
  test_int_equal(memory[100], 910);
  test_int_equal(memory[101], 0);

  printf("Array Sum Test\n");

  f = fopen("array-sum.asm", "r");
  memset(program, 0, 10000);
  fread(program, 1, 10000, f);
  
  memset(memory, 0, sizeof(memory));
  memset(registers, 0, sizeof(registers));
  assemble(program, memory);

  memory[100] = 2; // Scalar operand
  memory[101] = 3; // Length
  memory[102] = 1; // Items
  memory[103] = 2; // Items
  memory[104] = 3; // Items

  execute(memory, registers);
  test_int_equal(memory[102], 3);
  test_int_equal(memory[103], 4);
  test_int_equal(memory[104], 5);
}
