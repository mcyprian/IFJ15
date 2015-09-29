/**
 * @file remove_comments.h
 * @author Michal Cyprian <xcypri01@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Header file for module remove_comments.c.
 */

 enum states {CODE, SLASH, LONG_COMMENT, STAR, LINE_COMMENT, BACKSLASH, SPECIAL_CHAR};

/** Reads from input file and writes only code to output file.
 *  @param fout output file
 *  @param fin input file
 *  @return Zero on success one on error.
 */
int remove_comments(FILE *fout, FILE* fin);
