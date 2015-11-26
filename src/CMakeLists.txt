add_executable(interpreter main.c scanner.c dynamic_buffer.c dynamic_structure_buffer.c 
	syntax_analysis.c precedence_analysis.c stack.c built_functions.c ial.c
	semantics.c symbol_table.c interpreter.c instructions.c built_functions.c)
