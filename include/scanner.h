/**
 * @file scanner.h
 * @author Tomas Scavnicky <xscavn00@stud.fit.vutbr.cz> Michal Durista <xduris04@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Header file for scanner.c module
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

enum {START, IDENTIFIER, OPERATOR};


int tokenize(FILE *fin);