#include "general.cpp"
#include "vm_general.cpp"
#include "reading_writing.cpp"
#include "stack_operations.cpp"
#include <math.h>
#include <raylib.h>
#include <stdint.h>


const int SCREEN_WIDTH = 120;
const int SCREEN_HEIGHT = 80;


// #define DEBUG


// #define COND_JUMP(spu, arg_1, sign) { long num_1 = 0, num_2 = 0;\
//                                      stack_pop(spu->data_stack, &num_1);\
//                                      stack_pop(spu->data_stack, &num_2);\
//                                      if (num_1 sign num_2) { VM_JMP(spu, arg_1); }\
//                                      return }


#define getarg(arg) memcpy(&(arg), spu->byte_code_buffer + spu->buffer_index, sizeof(long));\
                    spu->buffer_index += sizeof(long)


struct spu_t {

    char* byte_code_buffer;
    long buffer_index;

    long registers[1 + REGISTER_NUM]; // register[0] have to stay empty
    long* ram; // [4 * SCREEN_WIDTH * SCREEN_HEIGHT];

    stack_t* data_stack;
    stack_t* recursion_stack;

};


struct arg_t {

    long arg_1;
    long arg_2;
    bool if_konst;
    bool if_regist;
    bool if_memory;

};


int main(int argc, char* argv[]) {

    spu_t spu_data = { };
    spu_data.data_stack = (stack_t*) calloc (1, sizeof(stack_t));
    spu_data.recursion_stack = (stack_t*) calloc (1, sizeof(stack_t));

    text_to_buffer(argv[1], &(spu_data.byte_code_buffer));
    stack_constructor(spu_data.data_stack, 10);
    stack_constructor(spu_data.recursion_stack, 10);
    spu_data.ram = (long*) calloc(4 * SCREEN_WIDTH * SCREEN_HEIGHT, sizeof(long));

    arg_t arguments = { };

    uchar new_comand = 0;
    while ((new_comand = spu_data.byte_code_buffer[spu_data.buffer_index++]) != HLT) {

#ifdef DEBUG
        printf("new_comand got ");
#endif // DEBUG

        uchar real_comand = new_comand & CMD_MASK;

        arguments.if_konst  = new_comand & KON_MASK;
        arguments.if_regist = new_comand & REG_MASK;
        arguments.if_memory = new_comand & MEM_MASK;

        (*function_p[real_comand])(&spu_data, &arguments);

    }

    VM_HLT(NULL, NULL);

}


void VM_HLT(spu_t* spu, arg_t* arguments) {
    exit(0);
    return;
}


void VM_ADD(spu_t* spu, arg_t* arguments) {
    long num_1 = 0;
    long num_2 = 0;
    stack_pop(spu->data_stack, &num_1);
    stack_pop(spu->data_stack, &num_2);
    stack_push(spu->data_stack, num_2 + num_1);
    return;
}


void VM_SUB(spu_t* spu, arg_t* arguments) {
    long num_1 = 0;
    long num_2 = 0;
    stack_pop(spu->data_stack, &num_1);
    stack_pop(spu->data_stack, &num_2);
    stack_push(spu->data_stack, num_2 - num_1);
    return;
}


void VM_MUL(spu_t* spu, arg_t* arguments) {
    long num_1 = 0;
    long num_2 = 0;
    stack_pop(spu->data_stack, &num_1);
    stack_pop(spu->data_stack, &num_2);
    stack_push(spu->data_stack, num_2 * num_1);
    return;
}


void VM_DIV(spu_t* spu, arg_t* arguments) {
    long num_1 = 0;
    long num_2 = 0;
    stack_pop(spu->data_stack, &num_1);
    stack_pop(spu->data_stack, &num_2);
    if (num_1 == 0) {
        printf("Error: div. 0\n");
        exit(1);
        return;
    }
    stack_push(spu->data_stack, num_2 / num_1);
    return;
}


void VM_OU(spu_t* spu, arg_t* arguments) {
    long num = 0;
    stack_pop(spu->data_stack, &num);
    printf("Output: %ld\n", num);
    return;
}


void VM_JMP(spu_t* spu, arg_t* arguments) {
    getarg(arguments->arg_1);
    spu->buffer_index = arguments->arg_1;
    return;
}


void VM_JA(spu_t* spu, arg_t* arguments) {
    long num_1 = 0;
    long num_2 = 0;
    stack_pop(spu->data_stack, &num_1);
    stack_pop(spu->data_stack, &num_2);
    if (num_2 > num_1) {
        VM_JMP(spu, arguments);
    }
    else {
        getarg(arguments->arg_1);
    }
    return;
}


void VM_JAE(spu_t* spu, arg_t* arguments) {
    long num_1 = 0;
    long num_2 = 0;
    stack_pop(spu->data_stack, &num_1);
    stack_pop(spu->data_stack, &num_2);
    if (num_2 >= num_1) {
        VM_JMP(spu, arguments);
    }
    else {
        getarg(arguments->arg_1);
    }
    return;
}


void VM_JB(spu_t* spu, arg_t* arguments) {
    long num_1 = 0;
    long num_2 = 0;
    stack_pop(spu->data_stack, &num_1);
    stack_pop(spu->data_stack, &num_2);
    if (num_2 < num_1) {
        VM_JMP(spu, arguments);
    }
    else {
        getarg(arguments->arg_1);
    }
    return;
}


void VM_JBE(spu_t* spu, arg_t* arguments) {
    long num_1 = 0;
    long num_2 = 0;
    stack_pop(spu->data_stack, &num_1);
    stack_pop(spu->data_stack, &num_2);
    if (num_2 <= num_1) {
        VM_JMP(spu, arguments);
    }
    else {
        getarg(arguments->arg_1);
    }
    return;
}


void VM_JE(spu_t* spu, arg_t* arguments) {
    long num_1 = 0;
    long num_2 = 0;
    stack_pop(spu->data_stack, &num_1);
    stack_pop(spu->data_stack, &num_2);
    if (num_1 == num_2) {
        VM_JMP(spu, arguments);
    }
    else {
        getarg(arguments->arg_1);
    }
    return;
}


void VM_JNE(spu_t* spu, arg_t* arguments) {
    long num_1 = 0;
    long num_2 = 0;
    stack_pop(spu->data_stack, &num_1);
    stack_pop(spu->data_stack, &num_2);
    if (num_1 != num_2) {
        VM_JMP(spu, arguments);
    }
    else {
        getarg(arguments->arg_1);
    }
    return;
}


void VM_IN(spu_t* spu, arg_t* arguments) {
    long num = 0;
    printf("Input: ");
    scanf("%ld", &num);
    stack_push(spu->data_stack, num);
    return;
}


void VM_RET(spu_t* spu, arg_t* arguments) {
    stack_pop(spu->recursion_stack, &(spu->buffer_index));
    return;
}


void VM_CALL(spu_t* spu, arg_t* arguments) {
    stack_push(spu->recursion_stack, spu->buffer_index + sizeof(long));
    VM_JMP(spu, arguments);
    return;
}


void VM_PUSH(spu_t* spu, arg_t* arguments) {

#ifdef DEBUG
    printf("PUSH\t");
#endif // DEBUG

    arguments->arg_1 = 0;
    arguments->arg_2 = 0;
    if (arguments->if_regist) {
        getarg(arguments->arg_1);
    }

    // printf("Arg-1: %7lg\t\t", arguments->arg_1);

    if (arguments->if_konst) {
        getarg(arguments->arg_2);
    }

    // printf("Arg-2: %7lg\t\t", arguments->arg_2);

    long res_arg = spu->registers[arguments->arg_1] + arguments->arg_2;

    // printf("Res_arg: %7lg\t\t\n", res_arg);
    
    if (arguments->if_memory) {
        stack_push(spu->data_stack, spu->ram[res_arg]);
    }
    else {
        stack_push(spu->data_stack, res_arg);
    }
    
#ifdef DEBUG
    printf(" \tArg 1 = %3ld ; Arg 2 = %3ld\n", arguments->arg_1, arguments->arg_2);
#endif // DEBUG

    return;
    
}


void VM_POP(spu_t* spu, arg_t* arguments) {

#ifdef DEBUG
    printf("POP \t");
    printf("%5d\t", spu->byte_code_buffer[spu->buffer_index - 1] & 0b11100000);
#endif // DEBUG

    // printf("if_regist: %d ", arguments->if_regist);
    // printf("if_konst : %d ", arguments->if_konst );
    // printf("if_memory: %d \n", arguments->if_memory);

    arguments->arg_1 = 0;
    arguments->arg_2 = 0;
    if (arguments->if_regist) {
        getarg(arguments->arg_1);
    }

    // printf("Reg-num: %d\n", arguments->arg_1);

    if (arguments->if_konst) {
        getarg(arguments->arg_2);
    }

    if (arguments->if_memory) {
        long res_arg = *(spu->registers + arguments->arg_1) + arguments->arg_2;
        stack_pop(spu->data_stack, spu->ram + res_arg);
    }
    else {
        stack_pop(spu->data_stack, spu->registers + arguments->arg_1);
    }

#ifdef DEBUG
    printf("Arg 1 = %3ld ; Arg 2 = %3ld\n", arguments->arg_1, arguments->arg_2);
#endif // DEBUG

    return;

}


void VM_SQRT(spu_t* spu, arg_t* arguments) {
    long num = 0;
    stack_pop(spu->data_stack, &num);
    num = (long) sqrt((double) num);
    stack_push(spu->data_stack, num);
    return;
}


void VM_DRW(spu_t* spu, arg_t* arguments) {

    // printf("DRW began\n");

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib - main window");

    while (!WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        for (int pos_y = 0; pos_y < SCREEN_HEIGHT; pos_y += 1) {
            for (int pos_x = 0; pos_x < 4 * SCREEN_WIDTH; pos_x += 4) {

                size_t idx = 4 * SCREEN_WIDTH * pos_y + pos_x;

                Color color = {
                    .r = (unsigned char) spu->ram[idx + 0],
                    .g = (unsigned char) spu->ram[idx + 1],
                    .b = (unsigned char) spu->ram[idx + 2],
                    .a = (unsigned char) spu->ram[idx + 3],
                };

                DrawPixel(pos_x / 4, pos_y, (Color) color);

            }
        }

        EndDrawing();

    }

    CloseWindow();
    return;

}



