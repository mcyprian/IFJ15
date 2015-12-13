/**
 * project: Implementace interpretu imperativního jazyka IFJ15
 * @file instrucrion_generator.h
 * @author Tomas Sykora <xsykor25@stud.fit.vutbr.cz>
 *
 * @section DESCROPTION
 *
 * Header file of module instrucrion_generator.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "dynamic_buffer.h"
#include "dynamic_structure_buffer.h"
#include "stack.h"
#include "ial.h"
#include "semantics.h"
#include "debug.h"
#include "datatypes.h"
#include "instructions.h"

#ifndef INS_GENERATOR_H
#define INS_GENERATOR_H

int generate_function_call(Resources *resouces, index_t function_id);

#endif // ! INS_GENERATOR_H
