#include <stdio.h>
#include <string.h>

//Define uma array com os nomes dos 32 registradores MIPS para busca
    const char *mips_registers[32] = {
        // 0: Constant 0
        "$zero",
        // 1: Reserved for the assembler
        "$at",
        // 2 - 3: Result Registers
        "$v0", "$v1",
        // 4 - 7: Argument Registers [0..3]
        "$a0", "$a1", "$a2", "$a3",
        // 8 - 15, 24 - 25: Temporary Registers [0..9]
        "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7","$t8", "$t9",
        // 16 - 23: Saved Registers [0..7]
        "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
        // 26 - 27: Kernel Registers [0..1]
        "$k0", "$k1",
        // 28: Global Data Pointer
        "$gp",
        // 29: Stack Pointer
        "$sp",
        // 30: Frame Pointer
        "$fp",
        // 31: Return Address
        "$ra"
    };

void get_tokens(char *input) {
    //Remover o \n da string (se tiver)
    input[strcspn(input, "\n")] = 0;

    //Strings agora foram tokenizadas com o /t de parâmetro (espaço)
    char *token = strtok(input, " \t");

    char operation[10];
    char reg1[10];
    char reg2[10];
    char reg3[10];

    //Copia cada token para as variáveis correspondentes
    if (token != NULL) {
        strcpy(operation, token);
        token = strtok(NULL, " \t");
    }
    if (token != NULL) {
        strcpy(reg1, token);
        token = strtok(NULL, " \t");
    }
    if (token != NULL) {
        strcpy(reg2, token);
        token = strtok(NULL, " \t");
    }
    if (token != NULL) {
        strcpy(reg3, token);
    }

    //Separado operação, e os 3 registradores enviados
    printf("Operation: %s\n", operation);
    printf("Register 1: %s\n", reg1);
    printf("Register 2: %s\n", reg2);
    printf("Register 3: %s\n", reg3);
}

void print_registers() {
    for (int i = 0; i < 32; i++) {
        printf("Register %d: %s\n", i, mips_registers[i]);
    }
}

int main() {
    
    char buffer[30];
    printf("Digite seu comando: ");
    fgets(buffer, sizeof(buffer), stdin);

    //Chama a função para separar a operação e os registradores
    get_tokens(buffer);

    return 0;
}
