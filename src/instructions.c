#include <instructions.h>



int (*execute_instruction[NUM_OF_INSTRUCTIONS])(Resources *resources, TInstruction *instruction) = { 
        mov_int_reg,
        mov_int_const,
        mov_dbl_reg,
        mov_dbl_const,
        add_int_reg_reg,
        add_int_reg_const,
        add_int_const_const,
        add_dbl_reg_reg,
        add_dbl_reg_const,
        add_dbl_const_const,
        sub_int_reg_reg,
        sub_int_reg_const,
        sub_int_const_const,
        sub_dbl_reg_reg,
        sub_dbl_reg_const,
        sub_dbl_const_const,
        mul_int_reg_reg,
        mul_int_reg_const,
        mul_int_const_const,
        mul_dbl_reg_reg,
        mul_dbl_reg_const,
        mul_dbl_const_const,
        div_int_reg_reg,
        div_int_reg_const,
        div_int_const_const,
        div_dbl_reg_reg,
        div_dbl_reg_const,
        div_dbl_const_const,
        eq_int_reg_reg,
        eq_int_reg_const,
        eq_int_const_const,
        eq_dbl_reg_reg,
        eq_dbl_reg_const,
        eq_dbl_const_const,
        g_int_reg_reg,
        g_int_reg_const,
        g_int_const_const,
        g_dbl_reg_reg,
        g_dbl_reg_const,
        g_dbl_const_const,
        l_int_reg_reg,
        l_int_reg_const,
        l_int_const_const,
        l_dbl_reg_reg,
        l_dbl_reg_const,
        l_dbl_const_const,
        ge_int_reg_reg,
        ge_int_reg_const,
        ge_int_const_const,
        ge_dbl_reg_reg,
        ge_dbl_reg_const,
        ge_dbl_const_const,
        le_int_reg_reg,
        le_int_reg_const,
        le_int_const_const,
        le_dbl_reg_reg,
        le_dbl_reg_const,
        le_dbl_const_const,
        ne_int_reg_reg,
        ne_int_reg_const,
        ne_int_const_const,
        ne_dbl_reg_reg,
        ne_dbl_reg_const,
        ne_dbl_const_const,
        cast_int_reg,
        cast_int_const,
        cast_dbl_reg,
        cast_dbl_const,
        halt
    };

