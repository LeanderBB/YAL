#include "yalvm/yalvm_bytecode.h"










static const char* yalvm_bytecode_inst_strings [] =
{
    /* End of execution */
    "halt",
    /* Load Global */
    "load global32",
    "load global64",
    "load globalptr",
     /* Store Global */
    "store global32",
    "store global64",
    "store globalptr",
    /* Load value from memory */
    "load mem",
    /* Store value in memory */
    "store mem",
     /* Load value from bytecode */
    "load val",
    /* Load constant  */
    "load const32",
    "load const64",
    /* Summation instruction */
    "addi",
    "addil",
    "addu",
    "addul",
    "addf",
    "addfl",
    /* Substraction instruction */
    "subi",
    "subil",
    "subu",
    "subul",
    "subf",
    "subfl",
     /* Multiplication instruction */
    "muli",
    "mulil",
    "mulu",
    "mulul",
    "mulf",
    "mulfl",
    /* Division instruction */
    "divi",
    "divil",
    "divu",
    "divul",
    "divf",
    "divfl",
    /* Negative (-var) */
    "negi",
    "negil",
    "negf",
    "negfl",
    /* Bit and */
    "bitand32",
    "bitand64",
    /* Bit or */
    "bitor32",
    "bitor64",
    /* Bit xor */
    "bitxor32",
    "bitxor64"
    /* Bit not */,
    "bitnot32",
    "bitnot64",
     /* Shift Left */
    "shiftl32",
    "shiftl64",
    /* Shift Right */
    "shiftr32",
    "shiftr64",
     /* Logical And */
    "and",
    /* Logical Or */
    "or",
    /* Logical not */
    "not",
    /* Compare Greater */
    "compare gti",
    "compare gtil",
    "compare gtu",
    "compare gtul",
    "compare gtf",
    "compare gtfl",
    /* Compare Greater or Equal */
    "compare gei",
    "compare geil",
    "compare geu",
    "compare geul",
    "compare gef",
    "compare gefl",
    /* Compare Lesser */
    "compare lti",
    "compare ltil",
    "compare ltu",
    "compare ltul",
    "compare ltf",
    "compare ltfl",
    /* Compare Lesser or Equal */
    "compare lei",
    "compare leil",
    "compare leu",
    "compare leul",
    "compare lef",
    "compare lefl",
    /* Compare Equal */
    "compare eqi",
    "compare eqil",
    "compare equ",
    "compare equl",
    "compare eqf",
    "compare eqfl",
    /* Compare not Equal*/
    "compare nei",
    "compare neil",
    "compare nequ",
    "compare neul",
    "compare nef",
    "compare nefl",
    /* Uncoditional Jmp */
    "jump",
    /* Conditional Jmp */
    "jump true",
    "jump false",
    /* Load Function */
    "laod func",
    /* Push Function Argument */
    "push arg",
    /* Call function */
    "call",
    /* Return Instruction */
    "return",
    /* Copy Register */
    "copy reg",
    /* Load String */
    "load string",
    /* Print instruction */
    "printi",
    "printil",
    "printu",
    "printul",
    "printf",
    "printfl",
    "printstr",
    "printstrobj",
    "printnl",
    /* Objects */
    "obj alloc",
    "obj acquire",
    "obj release",
    /* Strings Objects */
    "str create",
    "str release"
    /* end of strings */
};

const char*
yalvm_bytecode_inst_to_str(const yalvm_bytecode_inst_t inst)
{
    static const yalvm_size strings_size = YALVM_ARRAY_SIZE(yalvm_bytecode_inst_strings);
    return (inst < strings_size) ? yalvm_bytecode_inst_strings[inst] : "Unknown";
}
