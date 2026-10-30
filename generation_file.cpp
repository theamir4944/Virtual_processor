#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>


#define ALL_PERMISSION (S_IRUSR + S_IWUSR + S_IRGRP + S_IROTH)
#define LIST_LEN(list) (sizeof(list) / sizeof(list[0]))
#define SPECIFICATOR "%ld"


typedef long hash_t;


const char* COMANDS[] = {"HLT", "ADD", "SUB", "MUL", "DIV", "SQRT", "IN", "RET", "DRW", "OU", 
                         "PUSH", "POP", "JMP", "JA", "JAE", "JB", "JBE", "JE", "JNE", "CALL", "MAXP"};

const size_t N_COMANDS = LIST_LEN(COMANDS);
hash_t CMD_HASHES[N_COMANDS] = { };


void fill_comands_hashes() {

    for (size_t ind = 0; ind < N_COMANDS; ind++) {

        hash_t dur_cmd = 0;
        memcpy(
            &dur_cmd,
            COMANDS[ind],
            sizeof(char) * strlen(COMANDS[ind])
        );
        CMD_HASHES[ind] = dur_cmd;

    }
    return;

}


void enum_forming(FILE* file_stream) {

    fprintf(file_stream, "\nenum operation_t {\n");
    for (size_t ind = 0; ind < N_COMANDS; ind++) {
        fprintf(
            file_stream, 
            "\t%s = %zd,\n", 
            COMANDS[ind], ind
        );
    }

    fprintf(file_stream, "\tERR = -1\n};\n");
    return;

}


void hashlist_forming(FILE* file_stream) {

    fprintf(file_stream, "\nhash_t CMD_HASHES[N_COMANDS] = {\n");
    for (size_t ind = 0; ind < N_COMANDS; ind++) {
        fprintf(
            file_stream, 
            "\t" SPECIFICATOR ", // %s\n", 
            CMD_HASHES[ind], COMANDS[ind]
        );
    }

    fprintf(file_stream, "};\n");
    return;

}


void funclist_forming(FILE* file_stream) {

    fprintf(file_stream, "\nstruct spu_t;\nstruct arg_t;\n");

    for (size_t ind = 0; ind < N_COMANDS - 1; ind++) {
        fprintf(
            file_stream,
            "\nvoid VM_%-5s(spu_t* spu, arg_t* arguments);",
            COMANDS[ind]
        );
    }

    fprintf(file_stream, "\n\n");
    fprintf(file_stream, "void (* function_p[]) (spu_t*, arg_t*) = {");

    for (size_t ind = 0; ind < N_COMANDS - 1; ind++) {
        fprintf(
            file_stream,
            "\n\t&VM_%s",
            COMANDS[ind]
        );
        if (ind != N_COMANDS - 2) {
            fprintf(file_stream, ",");
        }
    }

    fprintf(file_stream, "\n};\n");

}


int main(const int argc, const char* argv[]) {

    const char* main_filename = NULL;
    const char* vm_file = NULL;

    if (argc < 2) {
        main_filename = "a.cpp";
    } else {
        main_filename = argv[1];
    }

    if (argc < 3) {
        main_filename = "b.cpp";
    } else {
        vm_file = argv[2];
    }

    FILE* file_stream = fopen(main_filename, "w");
    FILE* file_vm_stream = fopen(vm_file, "w");

    fprintf(file_stream, "#include <stdbool.h>\n\n");

    fprintf(file_stream, "typedef long hash_t;\n");
    fprintf(file_stream, "const long N_COMANDS = %zd;\n", N_COMANDS);
    fprintf(file_stream, "const int REGISTER_NUM = 8;\n");
    
    fprintf(file_stream, "\n#define MEM_MASK 0b10000000\n");
    fprintf(file_stream,   "#define REG_MASK 0b01000000\n");
    fprintf(file_stream,   "#define KON_MASK 0b00100000\n");
    fprintf(file_stream,   "#define CMD_MASK 0b00011111\n");


    fill_comands_hashes();
    enum_forming(file_stream);
    hashlist_forming(file_stream);
    funclist_forming(file_vm_stream);

    fclose(file_stream);
    return 0;

}



// #define INSTRUCTION_LIST(X) \
//     X(HLT, "HLT", 0)

// typedef enum
// {
// #define BUILD_ENUM(name, symbol, op) \
// name = (op),
// INSTRUCTION_LIST(BUILD_ENUM)
// #undef BUILD_ENUM
// };
