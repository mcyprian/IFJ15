#include <instructions.h>
#include <dynamic_structure_buffer.h>
#include <error_macros.h>
#include <debug.h>

int main() {

    int iRet = RETURN_OK;
    int instruction_ret = 0;
    int *int_setter; 
    Buffers buffers;
    index_t ip = 1;
    TInstruction *instruction;
    TReg *reg;

    if ((iRet = init_structure_buffer(&(buffers.reg_buff), 16, sizeof(TReg))) != RETURN_OK)
        goto DEFAULT;

    if ((iRet = init_structure_buffer(&(buffers.data_buff), 128, sizeof(int))) != RETURN_OK)
        goto REG_BUFF;

    if ((iRet = init_structure_buffer(&(buffers.instruction_buff), 128, sizeof(TInstruction))) != RETURN_OK)
        goto DATA_BUFF;

    int (*execute_instruction[NUM_OF_INSTRUCTIONS])(Buffers *buffers, index_t first_op, index_t second_op, index_t dest);

    debug_print("%s\n", "FILLING ARRAY OF FUNCTIONS");
    execute_instruction[MOV] = mov;
    execute_instruction[ADD_DATA] = add_data;
    execute_instruction[ADD_REG] = add_reg;
    execute_instruction[MUL_DATA] = mul_data;
    execute_instruction[HALT] = halt;

    debug_print("%s\n", "FILLING INT BUFFER");
    // set int values in data_buff
    int_setter = ((int*)buffers.data_buff.buffer + 1);
    *int_setter = 3;
    int_setter = ((int*)buffers.data_buff.buffer + 2);
    *int_setter = 2;
    int_setter = ((int*)buffers.data_buff.buffer + 3);
    *int_setter = 5;

    debug_print("%s\n", "GENERATING INSTRUCTIONS");
    new_instruction(&buffers.instruction_buff, instruction, 1lu, 3lu, 2lu, MUL_DATA);
    new_instruction(&buffers.instruction_buff, instruction, 2lu, 1lu, 1lu, ADD_REG);
    new_instruction(&buffers.instruction_buff, instruction, 0lu, 0lu, 0lu, HALT);

    ip = 1lu;
    debug_print("%s\n", "INTERPRETING");
    do {
        dereference_structure(&buffers.instruction_buff, ip, (void**)&instruction);
        ip++;
        debug_print("%s: %lu, %s: %d\n", "IP", ip, "INSTRUCTION", instruction->ins);
    
    instruction_ret = execute_instruction[instruction->ins](&buffers,
                                                instruction->dest,
                                                instruction->first_operand,
                                                instruction->second_operand);
    
    reg = access(buffers.reg_buff.buffer, TReg, 1lu);
    printf("REGISTER1: %d\n", reg->i);
    reg = access(buffers.reg_buff.buffer, TReg, 2lu);
    printf("REGISTER2: %d\n", reg->i);

    debug_print("%s: %d\n", "INSTRUCT RET", instruction_ret);
    } while (instruction_ret != -1);

    debug_print("%s\n", "INTERPRETING FINISHED");
    reg = access(buffers.reg_buff.buffer, TReg, 2lu);
    printf("REGISTER2: %d\n", reg->i);

    free_structure_buffer(&(buffers.instruction_buff));

DATA_BUFF:
    free_structure_buffer(&(buffers.data_buff));

REG_BUFF:
    free_structure_buffer(&(buffers.reg_buff));

DEFAULT:
    debug_print("%s: %d\n", "RETURN", iRet);
    return iRet;
}
