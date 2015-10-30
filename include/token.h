/**
 * @file token.h
 * @author Michal Cyprian <xcypri01@stud.fit.vutbr.cz> Tomas Scavnicky <xscavn00@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Header file of module token.c, including typedef of general TToken struct.
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <datatypes.h>
#include <dynamic_structure_buffer.h>


typedef struct {
    index_t token_index;           // Index to token string in dynamic_buffer
    int token_type;
    index_t token_next;            // Index to next token/expr in dynamic_structure
} TToken;

enum token_types
{
	T_DOUBLE, // 0                         } <TYPE>
	T_INT, // 1                            }
	T_STRING, // 2                         }
    K_CIN, // 3                               } <KEY>
	K_COUT, // 4                              }
    K_DO,   // 5                              }
	K_ELSE, // 6                              }
	K_FOR, // 7                               }
	K_IF,	// 8                              }
	K_RETURN, // 9                            }
    K_WHILE,  // 10                           }
	L_INT, // 11                                 } <RVALUE>
    L_DOUBLE, // 12                              }
	L_STRING, // 13                              }
	IDENTIFIER, // 14                            }
    O_ASSIGN, // 15                  =
    O_DIV,   // 16                   /
	O_EQUALS, // 17                  ==
	O_G, // 18                       >
    O_GE, // 19                      >=
	O_L, // 20                       <
    O_LE, // 21                      <=
	O_LEFT_ARROW, // 22              <<
    O_PLUS,  // 23                   +
	O_RIGHT_ARROW, // 24             >>
    O_MUL,   // 25                   *
	O_MINUS, // 26                   -
    O_NE,  // 27                     !=
	UO_EXCLAMATION, // 28            !
	AUTO, // 29                      auto
	COMMA, // 30                     ,
	CLOSING_BRACKET, // 31           )
	CLOSING_CURLY_BRACKET, // 32     }
    OPENING_BRACKET, // 33           (
	OPENING_CURLY_BRACKET, // 34     {
	SEMICOLON, // 35                 ;
    COMMENT, // 36
    LINE_COMMENT, // 37
    BLOCK_COMMENT, // 38
    BLOCK_COMMENT_END, // 39
    SCIENTIFIC, // 40
	START, // 41
	ERRORT, // 42
	EOFT, // 43
    SHIFT, // 44                      } precedence analysis tokens types
    RVALUE //45                       }
};


#endif // !TOKEN_H
