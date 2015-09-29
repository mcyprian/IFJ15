/**
 * @file remove_comments.c
 * @author Michal Cyprian <xcypri01@stud.fit.vutbr.cz>
 * 
 * @section DESCRIPTION
 *
 * Module to remove C style comments from source file, working for 
 * one line and standard comments.
 * */

#include <stdio.h>
#include <stdlib.h>
#include "remove_comments.h"

int remove_comments(FILE *fout, FILE* fin) {
    int c;
    int state = CODE;

    while ((c = fgetc(fin)) != EOF) {
        switch (state) {
            case CODE:
                if (c == '/')
                    state = SLASH;
                else if (c == '\"') {
                    fputc(c, fout);
                    state = BACKSLASH;
                } else 
                    fputc(c, fout);
                break;
            
            case SLASH:
                if (c == '*')
                    state = LONG_COMMENT;
                else if (c == '/')
                    state = LINE_COMMENT;
                else if (c == '\"') {
                    fprintf(fout, "/%c", c);
                    state = BACKSLASH;
                } else {
                    fprintf(fout, "/%c", c);
                    state = CODE;
                }
                break;

            case LONG_COMMENT:
                if (c == '*')
                    state = STAR;
                break;

            case STAR:
                if (c == '/')
                    state = CODE;
                else
                    state = LONG_COMMENT;
                break;
            
            case LINE_COMMENT:
                if (c == '\n')
                    state = CODE;
                break;

            case BACKSLASH:
                if (c == '\\') {
                    fputc(c, fout);
                    state = SPECIAL_CHAR;
                } else if (c == '\"') {
                    fputc(c, fout);
                    state = CODE;
                } else 
                    fputc(c, fout);
                break;
            
            case SPECIAL_CHAR:
                fputc(c, fout);
                state = BACKSLASH;
                break;
        }
    }

    if (state != CODE) {
        fprintf(stderr, "Process ended with state != CODE\n");
        return 1;
    }
    return 0;
}            
