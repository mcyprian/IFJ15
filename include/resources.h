/**
 * @file resources.h
 * @author Radovan Sroka <xsroka00@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Header file of incommon main.c syntax_analysis.c
 */

#ifndef RESOURCES_H
#define RESOURCES_H
#include <resources.h>
#include <dynamic_buffer.h>
#include <dynamic_structure_buffer.h>
#include <resources.h>

struct res{
	FILE * source;
	TDynamic_buffer string_buff;
	TDynamic_structure_buffer struct_buff;
};

#endif //!RESOURCES_H
