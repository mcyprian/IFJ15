#include <instructions.h>
#include <dynamic_structure_buffer.h>
#include <error_macros.h>
#include <debug.h>

int main() {

    int iRet = RETURN_OK;
    int instruction_ret = 0;
    Buffers buffers;
    index_t ip = 1;
    TInstruction *instruction;

    if ((iRet = init_structure_buffer(&(buffers.reg_buff), 16, sizeof(TReg))) != RETURN_OK)
        goto DEFAULT;

    if ((iRet = init_structure_buffer(&(buffers.data_buff), 128, sizeof(int))) != RETURN_OK)
        goto REG_BUFF;

    if ((iRet = init_structure_buffer(&(buffers.instruction_buff), 128, sizeof(TInstruction))) != RETURN_OK)
        goto DATA_BUFF;

    int (*execute_instruction[NUM_OF_INSTRUCTIONS])(Buffers *buffers, TInstruction *instruction);

    debug_print("%s\n", "FILLING ARRAY OF FUNCTIONS");
    execute_instruction[MOV] = mov;
    execute_instruction[ADD_DATA] = add_data;
    execute_instruction[ADD_REG] = add_reg;
    execute_instruction[ADD_CONST] = add_const;
    execute_instruction[MUL_DATA] = mul_data;
    execute_instruction[HALT] = halt;

    debug_print("%s\n", "FILLING INT BUFFER");
    // set int values in data_buff
    *((int*)buffers.data_buff.buffer + 1) = 3;
    *((int*)buffers.data_buff.buffer + 2) = 2;
    *((int*)buffers.data_buff.buffer + 3) = 5;

    debug_print("%s\n", "GENERATING INSTRUCTIONS");
    new_instruction(&buffers.instruction_buff, instruction, 1lu, 3lu, 2lu, MUL_DATA, BBB);
    new_instruction(&buffers.instruction_buff, instruction, 2lu, 1lu, 1lu, ADD_REG, BBB);
    new_instruction(&buffers.instruction_buff, instruction, 1lu, 2lu, 9, ADD_CONST, BBC);
    new_instruction(&buffers.instruction_buff, instruction, 0lu, 0lu, 0lu, HALT, BBB);

    debug_print("%s\n", "INTERPRETING");
    do {
        dereference_structure(&buffers.instruction_buff, ip, (void**)&instruction);
        ip++;
        debug_print("%s: %lu, %s: %d\n", "IP", ip, "INSTRUCTION", instruction->ins);
    
        instruction_ret = execute_instruction[instruction->ins](&buffers, instruction);
    
    
        debug_print("%s: %d\n", "REGISTER1", access(buffers.reg_buff.buffer, TReg, 1lu)->i);
        debug_print("%s: %d\n", "REGISTER2:", access(buffers.reg_buff.buffer, TReg, 2lu)->i);

        debug_print("%s: %d\n", "INSTRUCT RET", instruction_ret);
    } while (instruction_ret != -1);

    debug_print("%s\n", "INTERPRETING FINISHED");
    debug_print("%s: %d\n", "REGISTER1",  access(buffers.reg_buff.buffer, TReg, 1lu)->i);
    debug_print("%s: %d\n", "REGISTER2",  access(buffers.reg_buff.buffer, TReg, 2lu)->i);

    free_structure_buffer(&(buffers.instruction_buff));

DATA_BUFF:
    free_structure_buffer(&(buffers.data_buff));

REG_BUFF:
    free_structure_buffer(&(buffers.reg_buff));

DEFAULT:
    debug_print("%s: %d\n", "RETURN", iRet);
    return iRet;
}
