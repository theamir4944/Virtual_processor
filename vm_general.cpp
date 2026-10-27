
struct spu_t;
struct arg_t;

void VM_HLT  (spu_t* spu, arg_t* arguments);
void VM_ADD  (spu_t* spu, arg_t* arguments);
void VM_SUB  (spu_t* spu, arg_t* arguments);
void VM_MUL  (spu_t* spu, arg_t* arguments);
void VM_DIV  (spu_t* spu, arg_t* arguments);
void VM_SQRT (spu_t* spu, arg_t* arguments);
void VM_IN   (spu_t* spu, arg_t* arguments);
void VM_RET  (spu_t* spu, arg_t* arguments);
void VM_DRW  (spu_t* spu, arg_t* arguments);
void VM_OU   (spu_t* spu, arg_t* arguments);
void VM_PUSH (spu_t* spu, arg_t* arguments);
void VM_POP  (spu_t* spu, arg_t* arguments);
void VM_JMP  (spu_t* spu, arg_t* arguments);
void VM_JA   (spu_t* spu, arg_t* arguments);
void VM_JAE  (spu_t* spu, arg_t* arguments);
void VM_JB   (spu_t* spu, arg_t* arguments);
void VM_JBE  (spu_t* spu, arg_t* arguments);
void VM_JE   (spu_t* spu, arg_t* arguments);
void VM_JNE  (spu_t* spu, arg_t* arguments);
void VM_CALL (spu_t* spu, arg_t* arguments);

void (* function_p[]) (spu_t*, arg_t*) = {
	&VM_HLT,
	&VM_ADD,
	&VM_SUB,
	&VM_MUL,
	&VM_DIV,
	&VM_SQRT,
	&VM_IN,
	&VM_RET,
	&VM_DRW,
	&VM_OU,
	&VM_PUSH,
	&VM_POP,
	&VM_JMP,
	&VM_JA,
	&VM_JAE,
	&VM_JB,
	&VM_JBE,
	&VM_JE,
	&VM_JNE,
	&VM_CALL
};
