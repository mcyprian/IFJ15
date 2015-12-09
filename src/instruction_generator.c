#include <instruction_generator.h>

int generate_function_call(Resources *resouces, index_t function_id) {
		
	int is_built_in, iRet;
	index_t func_index;
	index_t *jump_paddr = NULL;
	char *func_str;
	int cmp;

	if ( (is_built_in = is_built_in(resources, function_id)) == SEMANTIC_ERROR){
		debug_print("%s\n","GENERATE_FUNCTION_CALL_RETURN_SEMANTIC_ERROR");
		return SEMANTIC_ERROR;
	}
	else {
		if (is_built_in){
			func_str = load_token(&(resources->string_buff), function_id);

			if ( (cmp = strcmp (func_str, "concat")) == 0 ){
				if ((iRet = load_func_index(resources, currently_analyzed_function, &func_index)) != 0)goto EXIT;
	        		if ((iRet = new_instruction_mem_mem(&(resources->instruction_buffer), func_index, 0lu, 0lu, CONCAT_MEM_MEM)) != 0)goto EXIT;

				if ((iRet = check_syntax(FUNC_CALL, resources)) != 0)goto EXIT;
			}
			else if ( (cmp = strcmp (func_str, "substr")) == 0 ){
				if ((iRet = load_func_index(resources, currently_analyzed_function, &func_index)) != 0)goto EXIT;
	        		if ((iRet = new_instruction_mem_mem(&(resources->instruction_buffer), func_index, 0lu, 0lu, SUBSTR_MEM_MEM)) != 0)goto EXIT;

				if ((iRet = check_syntax(FUNC_CALL, resources)) != 0)goto EXIT;
			}
			else if ( (cmp = strcmp (func_str, "length")) == 0 ){
				if ((iRet = load_func_index(resources, currently_analyzed_function, &func_index)) != 0)goto EXIT;
	        		if ((iRet = new_instruction_mem_mem(&(resources->instruction_buffer), func_index, 0lu, 0lu, LENGTH_MEM)) != 0)goto EXIT;

				if ((iRet = check_syntax(FUNC_CALL, resources)) != 0)goto EXIT;
			}
			else if ( (cmp = strcmp (func_str, "find")) == 0 ){
				if ((iRet = load_func_index(resources, currently_analyzed_function, &func_index)) != 0)goto EXIT;
	        		if ((iRet = new_instruction_mem_mem(&(resources->instruction_buffer), func_index, 0lu, 0lu, FIND_MEM_MEM)) != 0)goto EXIT;

				if ((iRet = check_syntax(FUNC_CALL, resources)) != 0)goto EXIT;
			}
			else if ( (cmp = strcmp (func_str, "sort")) == 0 ){
				if ((iRet = load_func_index(resources, currently_analyzed_function, &func_index)) != 0)goto EXIT;
	        		if ((iRet = new_instruction_mem_mem(&(resources->instruction_buffer), func_index, 0lu, 0lu, SORT_MEM)) != 0)goto EXIT;

				if ((iRet = check_syntax(FUNC_CALL, resources)) != 0)goto EXIT;
			}

		}
		else {
			if ((iRet = load_func_index(resources, currently_analyzed_function, &func_index)) != 0)goto EXIT;
	        	if ((iRet = new_instruction_mem_mem(&(resources->instruction_buffer), func_index, 0lu, 0lu, FCE_CALL)) != 0)goto EXIT;
			jump_paddr = &(access(resources->instruction_buffer.buffer, TInstruction, (resources->instruction_buffer.next_free - 1))->dest.index);

			if ((iRet = check_syntax(FUNC_CALL, resources)) != 0)goto EXIT;

			*jump_paddr = resources->instruction_buffer.next_free;
			if ((iRet = new_instruction_mem_mem(&(resources->instruction_buffer), func_index, 0, 0, JMP_MEM )) != 0)goto EXIT;
		}
	}
	
	return RETURN_OK;

	EXIT:
	return iRet;
}
