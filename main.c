#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Define uma array com os nomes dos 32 registradores MIPS para busca
const char *mips_registers[32] = {
    // 0: Sempre contém o valor zero
    "$zero",
    // 1: Usado pelo montador para auxiliar nas expansões de pseudo-instruções.
    "$at",
    // 2 - 3: Registradores de valor de retorno de funções. (quando ultrapassa 32 bits o $v1 é usado)
    "$v0", "$v1",
    // 4 - 7: Registradores de argumentos. (para passagem a procedimentos)
    "$a0", "$a1", "$a2", "$a3",
    // 8 - 17: Registradores temporários usados para armazenamento temporário.
    "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7","$t8", "$t9",
    // 18 - 25: Registradores salvos usados para armazenar valores que precisam ser preservados durante a chamada de funções.
    "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
    // 26 - 27: Registradores reservados para o kernel.
    "$k0", "$k1",
    // 28: Ponto de base global para dados estáticos.
    "$gp",
    // 29: Ponteiro de pilha.
    "$sp",
    // 30: Ponteiro de quadro (frame) usado para referência a variáveis locais e temporárias.
    "$fp",
    // 31: Registrador de retorno de endereço usado para armazenar o endereço de retorno após uma chamada de função.
    "$ra"
};

int mips_registers_vals[32] = { 0 };

int memory[1024] = { 0 };  // Simulação de memória com 1024 posições

int PC = 0;     

int find_register_index(char *input){
    int index_val;
    int i=0;
    while(mips_registers[i]){
        if(strcmp(input,mips_registers[i]) == 0){
            index_val = i;
            break;
        }
        ++i;
    }
    return index_val; 
}

// Implementação das operações Jump
void J(int address) {
    printf("JUMP: PC alterado de %d para %d\n", PC, address);
    PC = address;
}

void JAL(int address) {
    mips_registers_vals[31] = PC + 1; // Armazena o endereço de retorno no $ra
    printf("JUMP AND LINK: $ra = %d, PC alterado de %d para %d\n", mips_registers_vals[31], PC, address);
    PC = address;
}

void JR(char *reg) {
    int regIndex = find_register_index(reg);
    if (regIndex < 0) {
        printf("Erro: Registrador inválido para JR.\n");
        return;
    }
    printf("JUMP REGISTER: PC alterado de %d para %d\n", PC, mips_registers_vals[regIndex]);
    PC = mips_registers_vals[regIndex];
}

void LW(char *regSave, char *regBase, int offset) {
    printf("[DEBUG] LW: regSave=%s regBase=%s offset=%d\n",
           regSave, regBase, offset);
    int regSaveIndex = find_register_index(regSave);
    printf("1");
    int regBaseIndex = find_register_index(regBase);
    printf("2");

    // Logs de depuração
    printf("[DEBUG] LW: regSave=%s (index=%d), regBase=%s (index=%d), offset=%d\n",
           regSave, regSaveIndex, regBase, regBaseIndex, offset);

    int address = mips_registers_vals[regBaseIndex] + offset;
    printf("[DEBUG] LW: Calculated address = %d\n", address);

    // Verificação de limites
    if (address % 4 != 0 || address / 4 >= 1024 || address < 0) {
        printf("Erro: endereço fora dos limites da memória.\n");
        return;
    }

    mips_registers_vals[regSaveIndex] = memory[address / 4];
    printf("Carregado valor %d de memory[%d] para %s\n",
           mips_registers_vals[regSaveIndex], address / 4, regSave);
}


void SW(char *regSave, char *regBase, int offset) {
    int regSaveIndex = find_register_index(regSave);
    int regBaseIndex = find_register_index(regBase);

    // Logs de depuração
    printf("[DEBUG] SW: regSave=%s (index=%d), regBase=%s (index=%d), offset=%d\n",
           regSave, regSaveIndex, regBase, regBaseIndex, offset);

    int address = mips_registers_vals[regBaseIndex] + offset;
    printf("[DEBUG] SW: Calculated address = %d\n", address);

    // Verificação de limites
    if (address % 4 != 0 || address / 4 >= 1024 || address < 0) {
        printf("Erro: endereço fora dos limites da memória.\n");
        return;
    }

    memory[address / 4] = mips_registers_vals[regSaveIndex];
    printf("Armazenado valor %d de %s em memory[%d]\n",
           mips_registers_vals[regSaveIndex], regSave, address / 4);
}


void BEQ(char *reg1, char *reg2, int offset) {
    int reg1Index = find_register_index(reg1);
    int reg2Index = find_register_index(reg2);
    if (mips_registers_vals[reg1Index] == mips_registers_vals[reg2Index]) {
        printf("Branch taken: jumping by offset %d\n", offset);
        // Aqui você pode implementar a lógica para ajustar o program counter (PC)
    } else {
        printf("Branch not taken\n");
    }
}

void ADDI(char*regSave, char*regOp, int imediate){
    int regop_index = find_register_index(regOp);
    int regop_val = mips_registers_vals[regop_index];
    int regSave_index = find_register_index(regSave);
    int regSave_val = mips_registers_vals[regSave_index];
    mips_registers_vals[regSave_index] = regop_val + imediate;
}
void ADD(char*regSave, char*regOp1, char*regOp2){
    int regop1_index = find_register_index(regOp1);
    int regop1_val = mips_registers_vals[regop1_index];
    int regop2_index = find_register_index(regOp2);
    int regop2_val = mips_registers_vals[regop2_index];
    int regSave_index = find_register_index(regSave);
    int regSave_val = mips_registers_vals[regSave_index];
    printf("%d", regSave_index);
    mips_registers_vals[regSave_index] = regop1_val + regop2_val;
}

void MUL(char*regSave, char*regOp1, char*regOp2){
    int regop1_index = find_register_index(regOp1);
    int regop1_val = mips_registers_vals[regop1_index];
    int regop2_index = find_register_index(regOp2);
    int regop2_val = mips_registers_vals[regop2_index];
    int regSave_index = find_register_index(regSave);
    int regSave_val = mips_registers_vals[regSave_index];
    printf("%d", regSave_index);
    mips_registers_vals[regSave_index] = regop1_val * regop2_val;
}

void XORI(char *regSave, char *regOp, int immediate) {
    int regOpIndex = find_register_index(regOp);
    int regSaveIndex = find_register_index(regSave);
    mips_registers_vals[regSaveIndex] = mips_registers_vals[regOpIndex] ^ immediate;
}

void SLTI(char *regSave, char *regOp, int immediate) {
    int regOpIndex = find_register_index(regOp);
    int regSaveIndex = find_register_index(regSave);
    mips_registers_vals[regSaveIndex] = mips_registers_vals[regOpIndex] < immediate ? 1 : 0;
}

void LUI(char *regSave, int immediate) {
    int regSaveIndex = find_register_index(regSave);
    mips_registers_vals[regSaveIndex] = (immediate & 0xFFFF) << 16; // Garante que apenas os 16 bits inferiores são deslocados
    printf("LUI aplicado: registrador %s recebe valor %d\n", regSave, mips_registers_vals[regSaveIndex]); // Verificação
}

void ANDI(char *regSave, char *regOp, int immediate) {
    int regOpIndex = find_register_index(regOp);
    int regSaveIndex = find_register_index(regSave);
    mips_registers_vals[regSaveIndex] = mips_registers_vals[regOpIndex] & immediate;
}

void ORI(char *regSave, char *regOp, int immediate) {
    int regOpIndex = find_register_index(regOp);
    int regSaveIndex = find_register_index(regSave);
    mips_registers_vals[regSaveIndex] = mips_registers_vals[regOpIndex] | immediate;
}

void ADDIU(char *regSave, char *regOp, int immediate) {
    int regOpIndex = find_register_index(regOp);
    int regSaveIndex = find_register_index(regSave);
    mips_registers_vals[regSaveIndex] = (unsigned int)mips_registers_vals[regOpIndex] + (unsigned int)immediate;
}

void get_tokens(char *input) {
    //Remover o \n da string (se tiver)
    input[strcspn(input, "\n")] = 0;

    //Strings agora foram tokenizadas com o /t de parâmetro (espaço)
    char *token = strtok(input, " \t");

    char operation[10];
    char reg1[10];
    char reg2[10];
    char reg3[10] = "";

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
    
    // Converte o valor imediato após verificar se ele está no lugar certo
    int immediate = 0;
    // tive que adicionar esse if pois LUI usa dois argumentos apenas e nisso dando o comando
    // LUI $t9 255
    // o valor imediato ia para o re2 e não reg3
    // Verifica o formato "offset(base)" para LW e SW
    if (strcmp(operation, "LW") == 0 || strcmp(operation, "SW") == 0) {
        // Extrai offset e registrador base do formato "offset(base)"
        sscanf(reg2, "%d(%[^)])", &immediate, reg2);
    } else if (strcmp(operation, "LUI") == 0) {
        // LUI usa apenas dois argumentos, então o imediato vem de reg2
        immediate = atoi(reg2);
    } else {
        // Para outras instruções, o imediato está em reg3
        immediate = atoi(reg3);
    }


    // Adiciona suporte às operações J
    if (strcmp(operation, "J") == 0) {
        immediate = atoi(reg1); // Endereço é o primeiro argumento
        J(immediate);
    } else if (strcmp(operation, "JAL") == 0) {
        immediate = atoi(reg1); // Endereço é o primeiro argumento
        JAL(immediate);
    } else if (strcmp(operation, "JR") == 0) {
        JR(reg1);
    }

    if (strcmp(operation, "LW") == 0) {
        LW(reg1, reg2, immediate);
    } else if (strcmp(operation, "SW") == 0) {
        SW(reg1, reg2, immediate);
    } else if (strcmp(operation, "BEQ") == 0) {
        BEQ(reg1, reg2, immediate);
    }

    if (strcmp(operation, "ADD") == 0) {
        ADD(reg1, reg2, reg3);
    } else if (strcmp(operation, "ADDI") == 0) {
        ADDI(reg1, reg2, immediate);
    } else if (strcmp(operation, "ADDIU") == 0) {
        ADDIU(reg1, reg2, immediate);
    } else if (strcmp(operation, "ANDI") == 0) {
        ANDI(reg1, reg2, immediate);
    } else if (strcmp(operation, "ORI") == 0) {
        ORI(reg1, reg2, immediate);
    } else if (strcmp(operation, "XORI") == 0) {
        XORI(reg1, reg2, immediate);
    } else if (strcmp(operation, "SLTI") == 0) {
        SLTI(reg1, reg2, immediate);
    } else if (strcmp(operation, "LUI") == 0) {
        LUI(reg1, immediate);
    } else if (strcmp(operation, "MUL") == 0) {
        MUL(reg1, reg2, reg3);
    }

}

void print_registers() {
    for (int i = 0; i < 32; i++) {
        printf("Register %s Valor: %d\n", mips_registers[i], mips_registers_vals[i]);
    }
    printf("PC: %d\n", PC); // Adiciona o valor do PC ao log
}

int main() {
    char buffer[30];
    
    // Inicializa alguns registradores com valores específicos
    mips_registers_vals[8] = 5;  // $t0 = 5
    mips_registers_vals[9] = 2;  // $t1 = 2
    mips_registers_vals[10] = 8; // $t2 = 8
    memory[3] = 100;             // Valor de exemplo na memória, em posição 3 (offset 12 bytes)

    while (1) { // Loop infinito
        printf("Digite seu comando (ou 'exit' para sair): ");
        fgets(buffer, sizeof(buffer), stdin);

        // Remove o caractere de nova linha '\n' do final do input
        buffer[strcspn(buffer, "\n")] = 0;

        // Verifica se o usuário digitou "exit"
        if (strcmp(buffer, "exit") == 0) {
            printf("Encerrando o programa.\n");
            break;
        }

        // Chama a função para separar a operação e os registradores
        get_tokens(buffer);

        // Exibe os registradores após a execução do comando
        print_registers();
    }
    return 0;
}
