#include "general.cpp"
#include "reading_writing.cpp"
#include <ctype.h>


#define MAX_COMAND_LEN 10
#define MAX_LABELS 20

#define raise_buffer_if(condition)  if(condition)\
                                    { asm_st->read_buffer_copy += asm_st->delta_read_buffer; }

#define skip_space(buffer)          while(isspace(*buffer))\
                                    { buffer++; }


struct asm_t {

    char* read_buffer;
    char* read_buffer_copy;
    size_t read_size;

    char* code_buffer;
    char* code_buffer_copy;
    size_t code_size;

    size_t labels[MAX_LABELS];
    size_t delta_read_buffer;
    char string_comand[MAX_COMAND_LEN];
    
};


void get_new_label(asm_t* asm_st);
void op_error(void);
void get_jump_func(asm_t* asm_st, operation_t op);
void get_p_func(asm_t* asm_st, operation_t op);
void get_only_func(asm_t* asm_st, operation_t op);

operation_t translate(const char* string_comand);
size_t my_strlen(const char* string);
hash_t op_hash(const char* comand);
void buffer_pass(asm_t* asm_data, bool not_first_pass);


int main(int argc, char* argv[]) {

    if (argc < 2) {
        printf("Error: No file-name\n");
        return -1;
    }

    asm_t asm_data = { };

    char* file_read_name = argv[1];
    asm_data.read_size = text_to_buffer(file_read_name, &asm_data.read_buffer);

    asm_data.code_buffer = (char*) calloc (asm_data.read_size * 4 + 64, sizeof(char));

    asm_data.read_buffer_copy = asm_data.read_buffer;
    asm_data.code_buffer_copy = asm_data.code_buffer;
    
    buffer_pass(&asm_data, 0);

    asm_data.code_buffer_copy = asm_data.code_buffer;
    asm_data.code_size = 0;
    asm_data.read_buffer_copy = asm_data.read_buffer;

    buffer_pass(&asm_data, 1);

    text_to_file("out.bin", asm_data.code_buffer, asm_data.code_size);

}


void get_new_label(asm_t* asm_st) {

    asm_st->read_buffer_copy += asm_st->delta_read_buffer;

    long new_label = atoi(asm_st->string_comand + 1);
    asm_st->labels[new_label] = asm_st->code_size;
    return;

}


void op_error(char* string_comand) {

    printf("!!! Error operation !!!\n");
    printf("Comand %s is unknown\n", string_comand);
    exit(1);

}


void get_jump_func(asm_t* asm_st, operation_t op) {

    asm_st->read_buffer_copy += asm_st->delta_read_buffer;

    memcpy(asm_st->code_buffer_copy, &op, sizeof(char));
    asm_st->code_buffer_copy++;
    asm_st->code_size++;

    long new_arg = 0;
    sscanf(asm_st->read_buffer_copy, "%ld%n", &new_arg, &(asm_st->delta_read_buffer));
    new_arg = asm_st->labels[new_arg];
    asm_st->read_buffer_copy += asm_st->delta_read_buffer;

    memcpy(asm_st->code_buffer_copy, &new_arg, sizeof(long));
    asm_st->code_buffer_copy += sizeof(long);
    asm_st->code_size += sizeof(long);
    return;

}


void get_p_func(asm_t* asm_st, operation_t op) {

    asm_st->read_buffer_copy += asm_st->delta_read_buffer;
    skip_space(asm_st->read_buffer_copy)

    char str_regist_arg[3] = { };
    long regist_arg = 0;
    long konst_arg = 0;

    bool if_memory = (asm_st->read_buffer_copy[0] == '[');
    asm_st->read_buffer_copy += if_memory; // TODO: javn sizeof(char) => sizeof(uint8_t) ;
    asm_st->delta_read_buffer = 0;
    skip_space(asm_st->read_buffer_copy)

    bool if_regist = (bool) sscanf(asm_st->read_buffer_copy, "%2[A-ZX]%n", str_regist_arg, &(asm_st->delta_read_buffer));
    raise_buffer_if(if_regist)
    regist_arg = (str_regist_arg[0] >= 'A' && str_regist_arg[0] <= 'D') ? (str_regist_arg[0] - 'A' + 1) : 0;
    asm_st->delta_read_buffer = 0;

    skip_space(asm_st->read_buffer_copy)
    sscanf(asm_st->read_buffer_copy, "+%n", &(asm_st->delta_read_buffer));
    asm_st->read_buffer_copy += asm_st->delta_read_buffer;
    asm_st->delta_read_buffer = 0;

    bool if_konst = (bool) sscanf(asm_st->read_buffer_copy, "%ld%n", &konst_arg, &(asm_st->delta_read_buffer));
    raise_buffer_if(if_konst)
    asm_st->delta_read_buffer = 0;
    skip_space(asm_st->read_buffer_copy)

    sscanf(asm_st->read_buffer_copy, "]%n", &(asm_st->delta_read_buffer));
    asm_st->read_buffer_copy += asm_st->delta_read_buffer;
    asm_st->delta_read_buffer = 0;

    char result = (char) op + (if_memory * MEM_MASK) + (if_regist * REG_MASK) + (if_konst * KON_MASK);
    memcpy(asm_st->code_buffer_copy, &result, sizeof(char));
    asm_st->code_buffer_copy++;
    asm_st->code_size++;
    // TODO: char => uint8_t ;

    if (if_regist) {
        memcpy(asm_st->code_buffer_copy, &regist_arg, sizeof(long));
        asm_st->code_buffer_copy += sizeof(long);
        asm_st->code_size += sizeof(long);
    }

    if (if_konst) {
        memcpy(asm_st->code_buffer_copy, &konst_arg, sizeof(long));
        asm_st->code_buffer_copy += sizeof(long);
        asm_st->code_size += sizeof(long);
    }
    
}


void get_only_func(asm_t* asm_st, operation_t op) {

    asm_st->read_buffer_copy += asm_st->delta_read_buffer;

    memcpy(asm_st->code_buffer_copy, &op, sizeof(char));
    asm_st->code_buffer_copy++;
    asm_st->code_size++;
    return;

}


operation_t translate(const char* comand) {

    hash_t comand_hash = op_hash(comand);
    for (long num = 0; num < N_COMANDS; num++) {

        if (comand_hash == CMD_HASHES[num]) {
            return (operation_t) num;
        }

    }
    return ERR;

}


size_t my_strlen(const char* string) {
    
    size_t index = 0;
    for (index; string[index] != '\0'; index++) {
        ;
    }
    return index;

}


hash_t op_hash(const char* comand) {

    hash_t during_comand = 0;
    memcpy(&during_comand, comand, my_strlen(comand) * sizeof(char));
    return during_comand;

}


void buffer_pass(asm_t* asm_data, bool not_first_pass) {

    while (sscanf(asm_data->read_buffer_copy, "%s%n", asm_data->string_comand, &(asm_data->delta_read_buffer)) == 1) {

        if (asm_data->string_comand[0] == ':') {
            if (! not_first_pass) { 
                get_new_label(asm_data); 
            }
            else { 
                asm_data->read_buffer_copy += asm_data->delta_read_buffer; 
            }
            continue;
        }

        operation_t new_operation = translate(asm_data->string_comand);

        if (new_operation == ERR) {
            op_error(asm_data->string_comand);
        }

        if (new_operation == PUSH || new_operation == POP) {
            get_p_func(asm_data, new_operation);
        }
        else if (new_operation >= JMP && new_operation <= CALL) {
            get_jump_func(asm_data, new_operation);
        }
        else {
            get_only_func(asm_data, new_operation);
        }

        memset(asm_data->string_comand, '\0', MAX_COMAND_LEN * sizeof(char));
        
        skip_space(asm_data->read_buffer_copy);

    }

}
